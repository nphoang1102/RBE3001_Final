///*
// * armCheck.c
// *
// *  Created on: Feb 21, 2016
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
//	while(1) {
//		if (PINBbits._P0 == LOW) {
//			x = 11.66726189;
//			y = 26.90726189;
//			pos = 0;
//		}
//		if (PINBbits._P1 == LOW) {
//			x = 21.5;
//			y = -3.0;
//			pos = 180;
//		}
//		if (goTo(x,y,error0,error1)) {
//			setServo(0, pos);
//		}
//	}
//	return 0;
//}
