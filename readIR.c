///*
// * readIR.c
// *
// *  Created on: Feb 17, 2016
// *      Author: Duong D.T. Nguyen
// */
//
//
///*
// * Servo.c
// *
// *  Created on: Feb 17, 2016
// *      Author: Duong D.T. Nguyen
// */
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
//
//
//
//int main(void) {
//	//function init
//	initRBELib();
//	USARTinit();
//	buttonInit();
//	SPI_DACinit();
//	timer0init();
//
//	unsigned int raw;
//	float result = 0;
//
////	printf("%d %d %d %d\n\r", PINBbits._P0, PINBbits._P1, PINBbits._P2, PINBbits._P3);
//	while (1){
//
//		if (cnt % 90){
//			raw = readIR0();
//			// choose as appropriate
//			result = 27.728 * pow(map(raw, 0, 1023, 0, 5000)/1000.0, -1.2045); //model 1080, 10 bit ADC
////			result = 27.728 * pow(map(raw, 0, 4095, 0, 5000)/1000.0, -1.2045); //model 1080, 12 bit ADC
////			result = 60.374 * pow(map(raw, 0, 1023, 0, 5000)/1000.0, -1.16); //model 20150, 10 bit ADC
////			result = 60.374 * pow(map(raw, 0, 4095, 0, 5000)/1000.0, -1.16); //model 20150, 12 bit ADC
//			printf("%f \n\r",result);
//
//		}
//
//	}
//	return 0;
//}
//
