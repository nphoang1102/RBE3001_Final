/*
 * main.c
 *
 *  Created on: Feb 24, 2016
 *      Author: hpnguyen
 */

//include library
#include "RBELib/RBELib.h" //RBELib
#include <avr/io.h> // AVR IO
#include <avr/iom644p.h> //include this to fully regconize registers
#include <math.h>
#include "helper.h"

//declare some global variables
volatile long unsigned int cnt;

//global interrupt service routine
ISR(TIMER0_OVF_vect) {
	cnt++; //increment counter every interrupt
}

int main(void) {
	//local variables
	float x = 11.66726189; //starting x coordinate
	float y = 26.90726189; //starting y coordinate
	int pos = 0; //gripper servo direction
	float poll = 20.0; //polling the IR reading
	float stored = 20.0; //the stored IR reading to use later
	char state = 0; //for state machine
	unsigned long int time = 0; //time stamp
	unsigned int addTime = 0; //delayed time

	//arrays to store errors for PID control
	int error0[4] = { 0, 0, 0, 0 };
	int error1[4] = { 0, 0, 0, 0 };
	float resultIR[5] = { 0, 0, 0, 0, 0 };

	//hardware initialize
	USARTinit();
	initRBELib();
	buttonInit();
	SPI_DACinit();
	timer0init();

//Notes: arm0 -90 degree : 324 arm0 0 degree : 756
// final mapped function: (223/45)x + 767
//		 arm1 0 degree: 296 arm1 90 degree: 763
// final mapped function: (467/90)x + 296

// for servo, 0 is open, 180 is closed
// please operate at 2.0A and 7.2V
	while (1) {
		if (cnt % 180) {
			setServo(1,180); //set the conveyer belt speed once every 3 millisecond
			printf("%d\n\r", state); //for debug purpose
		}
		if (cnt % 9000){ //send data to MatLab once every second for extra credit
			//current sense must be read to assure accurate pot reading
			readCurrent0();
			readCurrent1();
			printf("%d,%d\n\r",readPot1(), readPot0()); //send signal to MatLab
		}
		switch(state){
		case 0: //starting position and polling for value
			goTo(11.66726189, 26.90726189, error0, error1);
			if (cnt % 180) { //once every 3 milliseconds
				poll = refinedIR(resultIR); //keep polling
				setServo(0, 0); //and keep the gripper open
			}
			if (poll < 18.0) { //if see the weight
				state = 1; //increase state
				time = cnt; //get time stamp
			}
			break;
		case 1: //detect the weight
			if (cnt > time + 1500) { //wait 0.3 second so the IR sensor can get accurate reading
				stored = refinedIR(resultIR); //this will be used for calculation
//				printf("IR %3.2f\n\r", stored); //for debug purpose

				//the conveyor belt is divided into three sections
				//each section required different delay time before the arm start moving
				//to accurately pick up the weight
				if (stored >= 11.0) {
					addTime = 5700;
				}
				else if (stored >= 9) {
					addTime = 10000;
				}
				else{
					addTime = 4200;
				}
				state = 2;
				time = cnt; //get time stamp
			}
			break;
		case 2: //start going to the weight position
			//go to different positions depends on which conveyer belt section the weight is on
			if (stored >= 11.0) {
				x = 38.7 - 1.2 - stored; //works fine now
			} else if (stored >= 9.0) {
				x = 39.2 - 1.2 - stored;
			} else {
				x = 39.7 - 1.2 - stored;
			}
			y = -2.85; //working for -2.7
			pos = 180;
			if (cnt > time + addTime) { //wait an amount before going to next state
				state = 3;
			}
			break;
		case 3: //got to the desired position
			if (goTo(x,y,error0,error1)){
				state = 4;
				time = cnt;
			}
			break;
		case 4: //grab the weight, wait a bit to stabilize
			goTo(x,y,error0,error1);
			if (cnt % 90) {
				setServo(0, 180);
			}
			if (cnt > time + 3500) {
				state = 5;
				time = cnt;
			}
			break;
		case 5: //go to a common position to weight the blocks
			if (cnt%90) {
				setServo(0,180);
			}

			//either get there or go and wait a bit to stabilize
			if ((goTo(29.83 , 10.86, error0, error1)) || (cnt > time + 9900)) {
				time = cnt;
				state = 6;
			}
			break;
		case 6:
			if (cnt % 90) {
				setServo(0,180);
			}
			goTo(29.83, 10.86, error0, error1);
			if (cnt > time + 4500) {
				time = cnt;
				state = 7;
			}
			break;
		case 7: //get back to starting position, measure time since last state
			if (cnt%90) {
				setServo(0,180);
			}
			if (goTo(11.66726189, 26.90726189,error0,error1)) {

				//go to different stage based on the time going from the common position to
				//the starting position
				if (cnt <= time + 15075) { //was working with 16875
					state = 0; //light, drop it at the starting point
				}
				else { //heavy, do something else
					x = 28.5;
					y = 0.0;
					state = 90; //I do this because I can
				}
			}
			break;
		case 90: //get the weight back onto the conveyer belt
			if (cnt%90){
				setServo(0,180);
			}
			if (goTo(x,y,error0, error1)) {
				state = 99;
				time = cnt;
			}
			break;
		case 99: //check if it has got to the desired position and drop the block
			goTo(x,y,error0,error1);
			if (cnt % 90) {
				setServo(0,0);
			}
			if (cnt > time + 3500) {
				state = 0; //go to the starting position
			}
			break;
		}
	}
	return 0;
}
