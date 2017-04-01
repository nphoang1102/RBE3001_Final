/*
 * Servo.c
 *
 *  Created on: Feb 17, 2016
 *      Author: Duong D.T. Nguyen
 */
////include library
//#include "RBELib/RBELib.h" //RBELib
//#include <avr/io.h> // AVR IO
//#include <avr/iom644p.h> //include this to fully regconize registers
//#include <math.h>
//#include "helper.h" //helper
//
//
////declare some global variable
//volatile unsigned long int cnt;
//
//ISR(TIMER0_OVF_vect){
//	cnt++; //increment counter every interrupt
//}



//int main(void) {
//	//function init
//	initRBELib();
//	USARTinit();
//	buttonInit();
//	SPI_DACinit();
//	timer0init();
//	int state = 0;
//	int state1 = 0;
//	int speed0 = 0;
//	int speed1 = 0;
//
////	printf("%d %d %d %d\n\r", PINBbits._P0, PINBbits._P1, PINBbits._P2, PINBbits._P3);
//	while (1){
//
//		if (PINBbits._P0 == LOW) state = 0;
//		if (PINBbits._P1 == LOW) state = 1;
//		if (PINBbits._P2 == LOW) state1 = 2;
//		if (PINBbits._P3 == LOW) state1 = 3;
//
////		speed0 = (PINBbits._P1 == LOW)? 0 : 180;
////		speed1 = (PINDbits._P0 == LOW)? 0 : 180;
////
////		if (cnt % 9) setServo(0, speed0);
////		if (cnt % 9) setServo(1, speed1);
//
//		switch (state) {
//		case 0:
//			if (cnt % 9) setServo(0, 0);
//			break;
//		case 1:
//			if (cnt % 9) setServo(0, 180);
//			break;
//		}
//		switch (state1) {
//		case 2:
//			if (cnt % 9) setServo(1, 0);
//			break;
//		case 3:
//			if (cnt % 9) setServo(1, 180);
//			break;
//		}
//	}
//
//	return 0;
//}

