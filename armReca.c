///*
// * armReca.c
// *
// *  Created on: Feb 19, 2016
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
//ISR(TIMER0_OVF_vect){
//	cnt++; //increment counter every interrupt
//}
//
//int main(void){
//	//local variables
//	int angle = 0; //desired angle for recalibration
//	float current0 = 0;
//	float current1 = 0;
//	//arrays to store errors for PID control
//	int error0[4] = {0, 0, 0, 0};
//	int error1[4] = {0, 0, 0, 0};
//
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
//	while(1) {
//		//get current value
//		current0 = readCurrent0();
//		current1 = readCurrent1();
//		//need to enable this or the pot reading will go haywire
//
//		if (PINBbits._P0 == LOW)
//			angle = 0;
//		else if (PINBbits._P1 == LOW)
//			angle = 90;
//
//		//100Hz PID control
//		if (cnt % 90) {
////			setArm0(angle, error0);
//			setArm1(angle, error1);
//		}
//
//		printf("%d %d\n\r", readPot0(), readPot1());
//	}
//
//	return 0;
//}
