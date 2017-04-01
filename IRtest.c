///*
// * IRtest.c
// *
// *  Created on: Feb 20, 2016
// *      Author: hpnguyen
// */
//
///*
// * armTest.c
// *
// *  Created on: Feb 20, 2016
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
//		}
//		if (cnt % 9000){
//			readCurrent0();
//			readCurrent1();
//			printf("%d,%d\n\r",readPot1(), readPot0());
//		}
//		switch(state){
//		case 0:
//			goTo(11.66726189, 26.90726189, error0, error1);
//			if (cnt % 180) {
//				poll = refinedIR(resultIR);
//				setServo(0, 0);
//			}
//			if (poll < 18.0) {
//				state = 1;
//				time = cnt;
//			}
//			break;
//		case 1:
//			if (cnt > time + 1500) {
//				stored = refinedIR(resultIR);
////				printf("IR %3.2f\n\r", stored);
//				if (stored >= 11.0) {
//					addTime = 12600;
//				}
//				else if (stored >= 9) {
//					addTime = 14600;
//				}
//				else{
//					addTime = 16600;
//				}
//				state = 2;
//				time = cnt;
//			}
//			break;
//		case 2:
//			if (cnt > time + 1400) {
//				if (stored >= 11.0) {
//					x = 38.7 - 1.2 - stored; //works fine now
//				}
//				else if (stored >= 9.0) {
//					x = 39.2 - 1.2 - stored;
//				}
//				else {
//					x = 39.7 - 1.2 - stored;
//				}
//				y = -2.85; //working for -2.7
//				pos = 180;
//				state = 3;
//			}
//			break;
//		case 3:
//			if (goTo(x,y,error0,error1)){
//				state = 4;
//				time = cnt;
//			}
//			break;
//		case 4:
//			goTo(x,y,error0,error1);
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
//			if (goTo(11.66726189, 26.90726189,error0,error1)) {
////				if () outeredge : 8.5, inneredge : 14, middle: 11.53
//				if (cnt <= time + addTime) { //was working with 16875
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
//			if (goTo(x,y,error0, error1)) {
//				state = 99;
//				time = cnt;
//			}
//			break;
//		case 99:
//			goTo(x,y,error0,error1);
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
//
//
