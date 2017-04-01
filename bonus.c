///*
// * bonus.c
// *
// *  Created on: Feb 26, 2016
// *      Author: hpnguyen
// */
//
//
///*
// * main.c
// *
// *  Created on: Feb 24, 2016
// *      Author: hpnguyen
// */
//
////include library
//#include "RBELib/RBELib.h" //RBELib
//#include <avr/io.h> // AVR IO
//#include <avr/iom644p.h> //include this to fully regconize registers
//#include <math.h>
//#include "helper.h"
//
////declare some global variables
//volatile long unsigned int cnt;
//
////global interrupt service routine
//ISR(TIMER0_OVF_vect) {
//	cnt++; //increment counter every interrupt
//}
//
//int main(void) {
//	//local variables
//	float x = 11.66726189;
//	float y = 26.90726189;
//	int pos = 0;
//	float poll = 20.0;
//	float stored = 20.0;
//	char state = 0;
//	unsigned long int time = 0;
//	unsigned int addTime = 0;
//	//arrays to store errors for PID control
//	int error0[4] = { 0, 0, 0, 0 };
//	int error1[4] = { 0, 0, 0, 0 };
//	float resultIR[5] = { 0, 0, 0, 0, 0 };
//	int theta0 = -45;
//	int theta1 = 90;
//	//hardware initialize
//	USARTinit();
//	initRBELib();
//	buttonInit();
//	SPI_DACinit();
//	timer0init();
//
////Notes: arm0 -90 degree : 324 arm0 0 degree : 756
//// final mapped function: (223/45)x + 767
////		 arm1 0 degree: 296 arm1 90 degree: 763
//// final mapped function: (467/90)x + 296
//
//// for servo, 0 is open, 180 is closed
//// please operate at 2.0A and 7.2V
//	while (1) {
//		//38.8 - refinedIR
//		if (cnt % 180) {
//			setServo(1,180);
////			printf("%d\n\r", state);
//		}
//		if (cnt % 4500){
//			readCurrent0();
//			readCurrent1();
////			printf("%f\n\r",refinedIR(resultIR));
//		}
//		switch(state){
//		case 0: //starting position and polling for value
//			if (cnt % 4500) {
////				readCurrent0();
////				readCurrent1();
//				printf("%f\n\r",refinedIR(resultIR));
//			}
//			setArm0(-45, error0);
//			setArm1(90,error1);
//			if (cnt % 180) {
//				poll = refinedIR(resultIR);
//				setServo(0, 0);
//			}
//			if (poll < 18.0) {
//				state = 1;
//				time = cnt;
//			}
//			break;
//		case 1: //detect the weight
//			if (cnt % 4500){
//			if (cnt > time + 1500) {
//				stored = refinedIR(resultIR);
//				printf("%f\n\r", stored); //for debug purpose
//				if (stored >= 11.0) {
//					addTime = 5700;
//				}
//				else if (stored >= 9) {
//					addTime = 6100;
//				}
//				else{
//					addTime = 4200;
//				}
//				state = 2;
//				time = cnt;
//			}
//			}
//			break;
//		case 2: //start going to the weight position
//			// Wait to get value of two angles
//
//
//			theta1 = getIntDebug();//receive the first integer
//			theta0 = getIntDebug();//receive the second integer
//
//
//
//
////			printf("%d,%d\n\r", theta1,theta0); //for debug purpose
////			if (stored >= 11.0) {
////				x = 38.7 - 1.2 - stored; //works fine now
////			} else if (stored >= 9.0) {
////				x = 39.2 - 1.2 - stored;
////			} else {
////				x = 39.7 - 1.2 - stored;
////			}
////			y = -2.85; //working for -2.7
//			pos = 180;
//			if (cnt > time + addTime) {
//
//				state = 3;
//
//			}
//
//			break;
//		case 3: //got to the desired position
//
//			if (setArm0(theta0, error0) * setArm1(theta1, error1)){
//				state = 4;
//
//				time = cnt;
//			}
//			break;
//		case 4: //grab the weight, wait a bit to stablize
//			setArm0(theta0, error0);
//			setArm1(theta1, error1);
//			if (cnt % 90) {
//				setServo(0, 180);
//			}
//			if (cnt > time + 3500) {
//				state = 5;
//				time = cnt;
//			}
//			break;
//		case 5:
//			if (cnt%90) {
//				setServo(0,180);
//			}
//			if ((setArm0(0,error0) * setArm1(20,error1)) || (cnt > time + 9900)) {
//				time = cnt;
//				state = 6;
//			}
//			break;
//		case 6:
//			if (cnt % 90) {
//				setServo(0,180);
//			}
//			setArm0(0,error0);
//			setArm1(20,error1);
//			if (cnt > time + 4500) {
//				time = cnt;
//				state = 7;
//			}
//			break;
//		case 7: //get back to starting position, measure time since last state
//			if (cnt%90) {
//				setServo(0,180);
//			}
//			if (setArm0(-45,error0) * setArm1(90,error1)) {
////				if () outeredge : 8.5, inneredge : 14, middle: 11.53
//				if (cnt <= time + 16775) { //was working with 16875
//					state = 0;
//				}
//				else {
//					x = 28.5;
//					y = 0.0;
//					state = 90; //I do this because I can
//				}
//			}
//			break;
//		case 90:
//			if (cnt%90){
//				setServo(0,180);
//			}
//			if (setArm0(-60,error0) * setArm1(30,error1)) {
//				state = 99;
//				time = cnt;
//			}
//			break;
//		case 99:
//			setArm0(-60, error0);
//			setArm1(30,error1);
//			if (cnt % 90) {
//				setServo(0,0);
//			}
//			if (cnt > time + 3500) {
//				state = 0;
//			}
//			break;
//		}
//	}
//	return 0;
//}
