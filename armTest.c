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
//	float x = 16.5;
//	float y = 15.24;
//	int pos = 0;
//	float temp = 0;
//	float IRdata[5] = { 0, 0, 0, 0, 0 };
//	//arrays to store errors for PID control
//	int error0[4] = { 0, 0, 0, 0 };
//	int error1[4] = { 0, 0, 0, 0 };
//	unsigned int raw;
//	float resultIR;
//	float a1 = 15.2781;
//	float b1 = -0.00780039;
//	int state = 0;
//	int isMove = 0;
//	long unsigned int dummyTime;
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
//	while (1) {
//		readCurrent0();
//		readCurrent1();
//		setServo(1, 0);
//		switch (state) {
//		case 0:
//			if (resultIR < 15.0){
//				temp = resultIR + 11.0;
//			}
////		printf("%3.2f\n\r", readIR0(IRdata));
//			if (PINBbits._P0 == LOW) {
//				x = /*16.5;*/11.66726189;
//				y = /*15.24;*/26.90726189;
//				pos = 180;
//				isMove = 1;
//			} else if (PINBbits._P1 == LOW) {
//				x = 21.5;
//				y = -3.0;
//				pos = 0;
//				isMove = 1;
//			} else if (PINBbits._P2 == LOW) {
//				x = 15.24 + 16.5;
//				y = 0;
//			}
//
//			if (cnt % 9) {
//				if ((goTo(x, y, error0, error1)) && isMove) {
//					dummyTime = cnt;
//					state = 1;
//					isMove = 0;
//
//				}
//			}
//			break;
//		case 1:
//			//100Hz PID control
//			if (cnt % 90) {
//				goTo(x, y, error0, error1);
//				if (((cnt - dummyTime) > 18000)) {
//					state = 2;
//					dummyTime = cnt;
//					if (pos == 180) {
//						pos = 0;
//					} else if (pos == 0) {
//						pos = 180;
//					}
//				}
//
//			}
//			break;
//		case 2:
//
//			if ((cnt - dummyTime) > 9000) {
//
//				state = 0;
//			}
//
//			break;
//		}
//		printf("%d, %d\n\r", state, pos);
//		if (cnt % 90) {
//			setServo(0, pos);
//		}
//	}
//
//	return 0;
//}
//
