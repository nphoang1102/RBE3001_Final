/*
 * helper.c
 *
 *  Created on: Feb 17, 2016
 *      Author: Duong D.T. Nguyen
 */


/*
 * helper.c
 *
 *  Created on: Feb 12, 2016
 *      Author: Duong D.T. Nguyen
 */


/*
 * helper.c
 *
 *  Created on: Feb 4, 2016
 *      Author: Hoang P Nguyen
 */

//include library
#include "RBELib/RBELib.h" //RBELib
#include <avr/io.h> // AVR IO
#include <avr/iom644p.h> //include this to fully regconize registers
#include <math.h>

void USARTinit(void); //initialize USART
unsigned char getCharDebug(void); //get character from usb usart
int getIntDebug(void); //receive an integer from usb usart
void putCharDebug(char byteToSend); //send character to usb usart
void buttonInit(void); //initialize switch 4-7 to PB0-3
void timer0init(void); //initialize timer 0, use overflow mode and start timer
void SPI_DACinit(void); //initialize SPI for DAC 12
void SPI_EncoderInit(void); //initialize SPI for Encoder 0 and 1
unsigned long readEncoder0(int channel);//send and read Encoder 0
//unsigned long int readEncoder1(void);//send and read Encoder 1
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
int map(int value, int from0, int to0, int from1, int to1);

unsigned char singleByteRead(unsigned char op_code,int channel);
void singleByteWrite(unsigned char op_code, unsigned char data,int channel);
void loadRegister(unsigned char op_code,int channel);


void USARTinit(void) {
	// Set baudrate
	UBRR1 = 0x09; //the baudrate will be 115200
	//Enable receiver and transmitter
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);
	//Set frame format: 8data, 2 stop bit
	//USBSn = 1: 2 stop bit ; USBSn = 0: 1 stop bit
	UCSR1C = (1 << USBS1) | (3 << UCSZ10);
}

unsigned char getCharDebug(void) {
	//wait for data to be received
	while (!(UCSR1A & (1 << RXC1)))
		;
	//get and return received data from buffer
	return UDR1;
}

int getIntDebug(void){
	int result;

	result = getCharDebug();
	result = result<<8;

	result |= getCharDebug();
	return result;
}

void putCharDebug(char byteToSend)
{
	while ( !(UCSR1A & (1 << UDRE1)) );
	UDR1 = byteToSend;
}

void timer0init(void) {
	//MCU frequency 18,432,000
	TCCR0A = 0x00; //toggle overflow mode, disconnect compare A and B
				   //normal operation
	TCCR0B = 0x02; //turn off force output compare A and B
				   //use 8 prescaler
				   //final clock frequency is 2.304 MHz
	TIMSK0 |= 0x01; //enable timer0 overflow interrupt
	SREG |= 0x80; //global interrupt enable

	//so basically, 9 interrupts == 1 millisecond
}

void SPI_DACinit(void) {
	/* Set MOSI and SCK output, MISO input, SS output */
	DDRB = (1 << DDB7) | (1 << DDB5) | (0 << DDB6) | (1 << DDB4);

	/* Enable SPI, Master, set clock rate fck/2 */
	SPCR = (1 << SPE) | (1 << MSTR); //|(1<<SPR1)|(1<<SPR0);

	SPSR = (1 << SPI2X); //set prescaler to 2
}

void SPI_EncoderInit(void) {
	/* Set MOSI and SCK output, MISO input */
	DDRB = (1 << DDB7) | (1 << DDB5) | (0 << DDB6);

	/* Set SS encoder 1 and encoder 0 output  */

	/* Enable SPI, Master, set clock rate fck/2 */
	SPCR = (1 << SPE) | (1 << MSTR); //|(1<<SPR1)|(1<<SPR0);

	SPSR = (1 << SPI2X); //set prescaler to 2

	DDRC = (1 << DDC4)|(1<<DDC5);

	PORTCbits._P4 = HIGH; //pull high for SS
	PORTCbits._P5 = HIGH;
}

void singleByteWrite(unsigned char op_code, unsigned char data,int channel) {

	unsigned char spi_data;

	//reset SS before doing anything
	if (channel == 1){
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS
		PORTCbits._P4 = LOW; //pull LOW for SS
	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS
		PORTCbits._P5 = LOW; //pull LOW for SS
	}

	SPDR = op_code;
	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;
	SPDR = data;

	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;

	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS

	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS

	}
}

unsigned char singleByteRead(unsigned char op_code,int channel){
	unsigned char spi_data;

	//reset SS before doing anything
	if (channel == 1){
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS
		PORTCbits._P4 = LOW; //pull LOW for SS
	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS
		PORTCbits._P5 = LOW; //pull LOW for SS
	}

	SPDR = op_code;

	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;	//reset SPIF

	SPDR = 0xFF;	//Start dummy transmission to read data
	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;

	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS

	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS

	}
	return spi_data;
}

void loadRegister(unsigned char op_code, int channel){

	unsigned char spi_data;

	//reset SS before doing anything
	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS
		PORTCbits._P4 = LOW; //pull LOW for SS
	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS
		PORTCbits._P5 = LOW; //pull LOW for SS
	}

	SPDR = op_code;

	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;

	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS

	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS

	}
}

unsigned long read4Bytes(unsigned char op_code,int channel){
	unsigned long spi_data;

	//reset SS before doing anything
	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS
		PORTCbits._P4 = LOW; //pull LOW for SS
	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS
		PORTCbits._P5 = LOW; //pull LOW for SS
	}
	SPDR = op_code;

	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;	//reset SPIF

	SPDR = 0xFF;	//Start dummy transmission to read data
	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data = SPDR;	//reset SPIF
	spi_data = spi_data << 8;
	SPDR = 0xFF;	//Start dummy transmission to read data
	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data |= SPDR;	//reset SPIF
	spi_data = spi_data << 8;
	SPDR = 0xFF;	//Start dummy transmission to read data
	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data |= SPDR;	//reset SPIF
	spi_data = spi_data << 8;
	SPDR = 0xFF;	//Start dummy transmission to read data
	while (!(SPSR & (1 << SPIF))) {
	};

	spi_data |= SPDR;	//reset SPIF

	return spi_data;

	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS

	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS

	}
}

unsigned long readEncoder0(int channel){

	if (channel == 1){
	DDRCbits._P4 = OUTPUT; //Encoder0 slave select

	PORTCbits._P4 = LOW; //pull LOW for SS
	}
	if (channel == 0){
	DDRCbits._P5 = OUTPUT; //Encoder0 slave select

	PORTCbits._P5 = LOW; //pull LOW for SS
	}
	unsigned char IRSTRcommand = 0xB0; //write STR 1011 0000
	unsigned char STRcommand = 0x0A; //STR command 00001010 //count up and counting enable
	unsigned char IRMDR0command = 0x88; //IR command for write MDR0 10001000
	unsigned char MDR0command = 0x03;//MDR0 0000 0011 x4 count mode,

	unsigned char IRMDR1command = 0x90; //IR command for write MDR1 10010000
	unsigned char MDR1command = 0x00; //MDR1  0000 0000 4 bytes data

	unsigned char IROTRcommand = 0x68; //IR command for read OTR 0110 1000

	unsigned long encoderData;
	unsigned int spi_data;

	singleByteWrite(IRSTRcommand,STRcommand,channel);
	spi_data = singleByteRead(0x70,channel); //read STR
	singleByteWrite(IRMDR0command,MDR0command,channel);
	spi_data = singleByteRead(0x48,channel); //read MDR0 01001000
	loadRegister(0xE8,channel); //load OTR 1110 1000
	encoderData = read4Bytes(IROTRcommand,channel);

//	printf("%li \n\r", encoderData);
	if (channel == 1) {
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS
		PORTCbits._P4 = LOW; //pull LOW for SS
		PORTCbits._P4 = HIGH; //pull HIGH to disable SS
	}
	if (channel == 0) {
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS
		PORTCbits._P5 = LOW; //pull LOW for SS
		PORTCbits._P5 = HIGH; //pull HIGH to disable SS

	}
	return encoderData;
}

void setDAC(int DACn, int SPIVal) {
	DDRDbits._P4 = OUTPUT;	//DAC slave select
	PORTDbits._P4 = LOW; //pull low the SS line

	unsigned char command = 0; //first byte to config DAC
	unsigned char lower4; //lower 4-bit data
	unsigned char higher8; //higher 8-bit data

	//configuration byte
	if (DACn == 0) { //4-bit configuration: 0111 is desirable
		command = 0x30; //send data to DAC0
	} else if (DACn == 1) {
		command = 0x31; //send data to DAC1
	} else if (DACn == 2) {
		command = 0x32; //send data to DAC2
	} else if (DACn == 3) {
		command = 0x33; //send data to DAC3
	}
	//since we don't need all 4 channels here,
	//we decided not to include it to reduce confusion

	//sending data
	lower4 = SPIVal << 4; //take the lower 4-bit
	higher8 = SPIVal >> 4; //take the higher 8-bit
	SPDR = command; //start sending the command
	/* Wait for transmission complete */
	while (!(SPSR & (1 << SPIF)))
		;
	SPDR = higher8; //start sending the higher 8-bit
	/* Wait for transmission complete */
	while (!(SPSR & (1 << SPIF)))
		;
	SPDR = lower4; //start sending the lower 4-bit
	/* Wait for transmission complete */
	while (!(SPSR & (1 << SPIF)))
		;
	PORTDbits._P4 = HIGH; //pull high the SS line
	PORTDbits._P4 = LOW; //pull low the SS line
	PORTDbits._P4 = HIGH; //pull high the SS line
	//reset the DAC-12 every time to make sure the value is updated.
}

unsigned int readADC(void) {
	ADCSRA &= ~(1 << ADIF);
	ADCSRA |= (1 << ADSC); //start conversion
	while (!(ADCSRA & (1 << ADIF)))
		//wait for conversion complete
		;
	unsigned int dataL = ADCL; //get the high bits
	unsigned int dataH = ADCH; //get the low bits
	unsigned int data = (dataH << 2) | (dataL >> 6);
	ADCSRA &= ~(1 << ADIF);
	ADCSRA &= (0 << ADEN); //turn off ADC
	return data; //return data
}

unsigned int readPot0(void) {
	ADMUX = 0;
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR) | 0x02; //Vcc voltage reference with external capacitor at AREF pin
																// left adjust result
																//single ended input at ADC2
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1)
			| (1 << ADPS0); //enable ADC
							//don't start conversion
							//ADC interrupt disable
							//ADC prescaler is 128
	DIDR0 |= 0x04; //disable digital I/O on port A pin 2
	while (!(ADCSRA & (1 << ADIF)))
		;
	return readADC();
}

float readIR0(void){
	ADMUX = 0;
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR) | 0x07; //Vcc voltage reference with external capacitor at AREF pin
																// left adjust result
																//single ended input at ADC7
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1)
			| (1 << ADPS0); //enable ADC
							//don't start conversion
							//ADC interrupt disable
							//ADC prescaler is 128
	DIDR0 |= 0x80; //disable digital I/O on port A pin 7
	while (!(ADCSRA & (1 << ADIF)))
		;

	//for debug purpose
//	unsigned int dummy = readADC() ;
//	printf("ADC %d\n\r",dummy );

	return (26.6197 / (((float) readADC() / 1023.0) * 5.0 + 0.00209662)) - 0.42;
}

float refinedIR(float *a) {
	for (int i = 0; i < 5; i++) {
		*(a+i) = readIR0();
	}
	if ((*a < 18.0) && (*(a+1) < 18.0) && (*(a+2) < 18.0) && (*(a+3) < 18.0) && (*(a+4) < 18.0)) {
		return (*a + *(a + 1) + *(a + 2) + *(a + 3) + *(a + 4)) / 5;
	} else return 18.0;
}


float readCurrent0(void) {
	DDRAbits._P0 = OUTPUT; //set analog pin A0 to output
	PORTAbits._P0 = HIGH; //output signal HIGH to enable current sense

	ADMUX = 0;
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR) | 0x04; //Vcc voltage reference with external capacitor at AREF pin
																// left adjust result
																//single ended input at ADC4
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1)
			| (1 << ADPS0); //enable ADC
							//don't start conversion
							//ADC interrupt disable
							//ADC prescaler is 128
	DIDR0 |= 0x10; //disable digital I/O on port A pin 4
	while (!(ADCSRA & (1 << ADIF)))
		;
	return ((float) readADC() - 655.0) * 6.4286; //conversion from ADC reading to mA
}

unsigned int readPot1(void) {
	ADMUX = 0;
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR) | 0x03; //Vcc voltage reference with external capacitor at AREF pin
																// left adjust result
																//single ended input at ADC3
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1)
			| (1 << ADPS0); //enable ADC
							//don't start conversion
							//ADC interrupt disable
							//ADC prescaler is 128
	DIDR0 |= 0x08; //disable digital I/O on port A pin 3
	while (!(ADCSRA & (1 << ADIF)))
		;
	return readADC();
}

float readCurrent1(void) {
	DDRAbits._P1 = OUTPUT; //set analog pin A0 to output
	PORTAbits._P1 = HIGH; //output signal HIGH to enable current sense

	ADMUX = 0;
	ADMUX |= (0 << REFS1) | (1 << REFS0) | (1 << ADLAR) | 0x05; //Vcc voltage reference with external capacitor at AREF pin
																// left adjust result
																//single ended input at ADC5
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1)
			| (1 << ADPS0); //enable ADC
							//don't start conversion
							//ADC interrupt disable
							//ADC prescaler is 128
	DIDR0 |= 0x20; //disable digital I/O on port A pin 5
	while (!(ADCSRA & (1 << ADIF)))
		;
	return (float) readADC() * 5.281690141 - 3511.478873; //conversion from ADC reading to mA
}

void upperArmDrive(int signal) {
	if (signal >= 0) { //drive in one direction if signal is positive
		setDAC(0, signal);
		setDAC(1, 0);
	} else { //else drive in other direction
		setDAC(0, 0);
		setDAC(1, -signal); //send negative(of negative signal) = positive signal
	}
}

char setArm0(int degree, int *e) {
	//convert from degree value to pot reading
	int desiredValue = (float) degree * 4.8
			+ 756.0;
	//and yes I put in a lot of 6 on purpose

	//declare some local variable
//	int lowerLimit = 500;

	if (desiredValue >= 1023) //limit the maximum angle to prevent shenanigans
		desiredValue = 1023;
	else if (desiredValue <= 324)
		desiredValue = 324;

	//the gain constants
	unsigned int kp = 24; //P gain
	float ki = 0.7089844; //I gain
	unsigned int kd = 12; //D gain

	//start computing output
	*e = desiredValue - readPot0(); //first error
	int output = -kp * (*e) - kd * ((*e) - *(e + 1)) //PID, yay!!!
	- ki * ((*e) + *(e + 1) + *(e + 2) + *(e + 3)); //this right here is gravity compensation

	//limiting lower output
	if ((output >= 130) && (output <= 600))
		output = 600;
	else if ((-output >= 130) && (-output <= 800))
		output = -800 + (float)readCurrent0() / 1.5;

	//limiting upper output
	if (output >= 4095)
		output = 4095;
	else if (-output >= 4095)
		output = -4095;

	//output the signal
	upperArmDrive(output);

	//update errors
	*(e + 3) = *(e + 2);
	*(e + 2) = *(e + 1);
	*(e + 1) = *e;

	//for debug ouput
//	printf("%d ", output);

	//conditions for return
	if (((*(e + 3) <= 8) && (*(e + 2) <= 8) && (*(e + 1) <= 8) && (*e <= 8))
			&& ((*e >= -8) && (*(e+1) >= -8) && (*(e+2) >= -8) && (*(e+3) >= -8)))
//			&& ((*(e+3) == *(e+2)) || (*(e+2) == *(e+1)) || (*(e+1) == *e)))
	//return true when reach steady state error, in this case when all errors are smaller than 8 and equals
		return 1;
	else
		return 0;
}

void lowerArmDrive(int signal) {
	if (signal >= 0) { //drive in one direction if signal is positive
		if (signal >= 4095) { //limit the signal to 4095 for the 12-bit DAC
			signal = 4095;
		}
		setDAC(2, signal);
		setDAC(3, 0);
	} else { //else drive in other direction
		if (-signal >= 4095) { //limit the signal to -4095 for the 12-bit DAC
			signal = 4095;
		}

		setDAC(2, 0);
		setDAC(3, signal);
	}
}

char setArm1(int degree, int *e) {
	//convert from degree value to pot reading
	int desiredValue = (float) degree
			* 5.188888888888888888888888888888888888888888888888888888888889 + 296.0;
	//and yes I put in a lot of 8s on purpose

	if (desiredValue <= 200)
		desiredValue = 200;
	else if (desiredValue >= 1023)
		desiredValue = 1023;

	//the gain constants
	float kp = 36.0; //P gain 36.0
	float ki = 0.900; //I gain 0.888000
	float kd = 17.0; //D gain 17.0

	//start computing output
	*e = desiredValue - readPot1(); //first error
	int output = kp * (*e) + kd * ((*e) - *(e + 1)) //PID, yay!!!
	+ ki * ((*e) + *(e + 1) + *(e + 2) + *(e + 3)); //this right here is gravity compensation

	//limiting lower output
	if ((output >= 50) && (output <= 270))
		output = 270;
	else if ((-output >= 50) && (-output <= 270))
		output = -270;

	//limiting upper output
	if (output >= 1200)
		output = 1200 + readCurrent1() / 1.6;
	else if (-output >= 400)
		output = -400 - readCurrent1() / 2.8;

	//output the signal
	lowerArmDrive(output);

	//update errors
	*(e + 3) = *(e + 2);
	*(e + 2) = *(e + 1);
	*(e + 1) = *e;

	//for debug purpose
//	printf("%d ", output);

	//conditions for return
	if (((*(e + 3) <= 10) && (*(e + 2) <= 10) && (*(e + 1) <= 10) && (*e <= 10))
		&& ((*e >= -10) && (*(e+1) >= -10) && (*(e+2) >= -10) && (*(e+3) >= -10)))
//		&& ((*(e+3) == *(e+2)) && (*(e+2) == *(e+1)) && (*(e+1) == *e)))
	//return true when reach steady state error, in this case when all errors are smaller than 10 and equals
		return 1;
	else //else return false
		return 0;
}

char goTo(float x, float y, int *e0, int *e1) {
	//most accurate Pi ever, up to 101 significant figures
	float pi = 3.1415926535; //897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
	//we just want to be accurate

	//first calculate the theta angles
	float c = (x * x + y * y - 504.5076) / 504.5076; //change 450 to 72 for inches
	float s = sqrt(1 - c * c);
	int theta0 = (acos(c)) * (180.0 / pi);
	int theta1 = (asin((y * (15.24 + 16.5 * c) - 16.5 * x * s) / (x * x + y * y))) * (180.0 / pi);
	//change 15.24 to 6 for inches
	// print angle
//		printf("Theta1, Theta0: %d, %d \n\r", theta1, theta0);
	//check the condition, if the angles are unreachable, change direction
	if ((theta0 < -90) || (theta0 > 0) || (theta1 > 90) || (theta1 < 0)){
		int dummy;
		dummy = theta0;
		theta0 = -theta0; //theta0
		theta1 = theta1+dummy; //theta1 + theta0;
	}
//	printf("Real Theta1, Theta0: %d, %d \n\r", theta1, theta0);
	//indication that the end effector has reached its destination
//	printf("Arm location: %d %d \n\r", setArm0(theta0, e0), setArm1(theta1, e1));
	if ((setArm0(theta0, e0)) * (setArm1(theta1, e1)))
		return 1;
	else return 0;
}

void buttonInit(void) {
	//set inputs for buttons
	DDRBbits._P0 = INPUT;
	DDRBbits._P1 = INPUT;
	DDRBbits._P2 = INPUT;
	DDRBbits._P3 = INPUT;
}


int getTheta0(float x, float y) {
	//most accurate Pi ever, up to 101 significant figures
	float pi = 3.1415926535; //897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
	//we just want to be accurate

	//some calculation
	float c = (x * x + y * y - 72.0) / 72.0;
	float s = sqrt(1 - c * c);
	int theta0 = (acos(c)) * (180.0 / pi);
	int theta1 = (asin((y * (6 + 6 * c) - 6 * x * s) / (x * x + y * y))) * (180.0 / pi);

	//check the condition, if the angle is unreachable, change direction
	if ((theta0 > 45) || (theta1 < -20))
		return theta0 - 180;
	else if((theta0 < -95)||(theta1 > 125))
		return theta0 + 180;
	return theta0;
}

int getTheta1(float x, float y) {
	//most accurate Pi ever, up to 101 significant figures
	float pi = 3.14159265;//35897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
	//we just want to be accurate

	//some calculation
	float c = (x * x + y * y - 72.0) / 72.0;
	float s = sqrt(1 - c * c);
	int theta0 = (acos(c)) * (180.0 / pi);
	int theta1 = (asin((y * (6 + 6 * c) - 6 * x * s) / (x * x + y * y))) * (180.0 / pi);

	//check the condition, if the angle is unreachable, change direction
	if ((theta0 > 45) || (theta1 < -20))
			return theta1 + 90;
		else if((theta0 < -95)||(theta1 > 125))
			return theta1 - 90;
		return theta1;
}

int getX(void) {
	float pi = 3.1415926535;
	float theta0 = (0.202247191 * (float) readPot0() - 156.3370787)
			* (pi / 180);
	float theta1 = (0.1978021978 * (float) readPot1() - 60.32967033)
			* (pi / 180);
	return 152.4 * cos(theta1) + 152.4 * cos(theta1 + theta0);
}

int getY(void) {
	float pi = 3.1415926535;
	float theta0 = (0.202247191 * (float) readPot0() - 156.3370787)
			* (pi / 180);
	float theta1 = (0.1978021978 * (float) readPot1() - 60.32967033)
			* (pi / 180);
	return 152.4 + 152.4 * sin(theta1) + 152.4 * sin(theta1 + theta0);
}

int map(int value, int from0, int to0, int from1, int to1) {
	return (value - from0)/(to0 - from0) * (to1 - from1) + from1;
}
