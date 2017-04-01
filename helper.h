/*
 * helper.h
 *
 *  Created on: Feb 12, 2016
 *      Author: Duong Nguyen Hoang Nguyen Nghia Huynh
 */

#ifndef HELPER_H_
#define HELPER_H_

void USARTinit(void); //initialize USART
unsigned char getCharDebug(void); //get character from usb usart
int getIntDebug(void); //receive an integer from usb usart
void putCharDebug(char byteToSend); //send character to usb usart
void buttonInit(void); //initialize switch 4-7 to PB0-3
void timer0init(void); //initialize timer 0, use overflow mode and start timer
void SPI_DACinit(void); //initialize SPI for DAC 12
void setDAC(int DACn, int SPIVal); //send signal to desired channel
char setArm0(int degree, int *e); //drive the upper arm to this position
void lowerArmDrive(int signal); //send signal to drive lower arm
char setArm1(int degree, int *e); //drive the lower arm to this position
char goTo(float x, float y, int *e0, int *e1); //drive the end effector to the desired coordinate
unsigned int readADC(void); //read the ADC value
unsigned int readPot0(void); //read the upper arm pot value
float readIR0(void); //read IR sensor value
float refinedIR(float *a); //read the average 5 values
float readCurrent0(void); //read the lower arm current value, in mA
unsigned int readPot1(void); //read the lower arm pot value
float readCurrent1(void); //read the lower arm current value, in mA
int getTheta0(float x, float y); //convert x and y positions to the angle 0
int getTheta1(float x, float y); //convert x and y positions to the angle 1
int getX(void); //get the x position, in mm
int getY(void); //get the y position, in mm
void SPI_EncoderInit(void); //initialize SPI for Encoder 0 and 1
unsigned long readEncoder0(int channel);//send and read Encoder 0
//unsigned long int readEncoder1(void);//send and read Encoder 1

#endif /* HELPER_H_ */
