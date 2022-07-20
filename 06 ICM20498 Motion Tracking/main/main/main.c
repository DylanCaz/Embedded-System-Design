/*********************
Design Assignment 6
By: Dylan Cazares
*********************/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "i2cmaster.h"

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) -1)

#define ICM20948	(0x68<<1)     // (1101001 << 1) I2C slave address when AD0=1
#define WHO_AM_I	 0x00
#define PWR_MGMT_1	 0x06
#define ACCEL_XOUT_H 0x2D
#define ACCEL_YOUT_H 0x2F
#define ACCEL_ZOUT_H 0x31
#define GYRO_XOUT_H	 0x33
#define GYRO_YOUT_H	 0x35
#define GYRO_ZOUT_H	 0x37

/*Magnitometer*/
#define MAGNE_XOUT_H 0x39
#define MAGNE_YOUT_H 0x3B
#define MAGNE_ZOUT_H 0x3D

#define DEVICE_ID	 0xEA

uint8_t raw;
uint16_t bigraw;

void USART_init(unsigned int ubrr);
void USART_tx_string(char * data);

//void icm_start_loc(void);
//void read_display_icm(void);

void ICM20948_writereg(uint8_t reg, uint8_t val);
	
/* functions i modified*/
uint16_t ICM20948_readreg16(uint8_t reg);
void ICM20948_Init(void);
void ICM20948_verify_whoami(void);
void ICM_write(uint8_t reg, uint8_t data);

char buffer[30], myfloat[5];

uint16_t accel_x;
uint16_t accel_y;
uint16_t accel_z;

uint16_t gyro_x;
uint16_t gyro_y;
uint16_t gyro_z;

uint16_t magne_x;
uint16_t magne_y;
uint16_t magne_z;

int main(void)
{
	PORTC |= (1<<5) | (1<<4);			// enable pull ups for TWI pins
	
	i2c_init();							// initialize TWI
	USART_init(BAUD_PRESCALLER);		// initialize USART
	USART_tx_string("UART Connected!\r\n");
    
	ICM20948_Init();				// change clksel on icm
	ICM20948_verify_whoami();			// verify we are connected
	
	_delay_ms(200);

	while(1){
		accel_x = ICM20948_readreg16(ACCEL_XOUT_H);
		accel_y = ICM20948_readreg16(ACCEL_YOUT_H);
		accel_z = ICM20948_readreg16(ACCEL_ZOUT_H);
		
		gyro_x = ICM20948_readreg16(GYRO_XOUT_H);
		gyro_y = ICM20948_readreg16(GYRO_YOUT_H);
		gyro_z = ICM20948_readreg16(GYRO_ZOUT_H);
		
		magne_x = ICM20948_readreg16(MAGNE_XOUT_H);
		magne_y = ICM20948_readreg16(MAGNE_YOUT_H);
		magne_z = ICM20948_readreg16(MAGNE_ZOUT_H);
		
		snprintf(buffer,sizeof(buffer),"ACCEL X: %d\r\n",accel_x);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"ACCEL Y: %d\r\n",accel_y);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"ACCEL Z: %d\r\n",accel_z);
		USART_tx_string(buffer);
		
		USART_tx_string("\r\n");
		_delay_ms(500); 
		
		snprintf(buffer,sizeof(buffer),"GYRO X: %d\r\n",gyro_x);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"GYRO Y: %d\r\n",gyro_y);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"GYRO Z: %d\r\n",gyro_z);
		USART_tx_string(buffer);
		
		USART_tx_string("\r\n");
		_delay_ms(500);
		
		snprintf(buffer,sizeof(buffer),"MAGNE X: %d\r\n", magne_x); //
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"MAGNE Y: %d\r\n", magne_y);//
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"MAGNE Z: %d\r\n", magne_z);//
		USART_tx_string(buffer);
		
		USART_tx_string("\r\n");
		//read_display_icm();
		_delay_ms(1000); 
	}
}

void USART_init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_tx_string(char * data){
	while((*data != '\0')){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

/* Ensure we are talking to ICM, print WHO_AM_I to terminal, should be 0xEA or 234 */
void ICM20948_verify_whoami(void){
	uint8_t who_am_i = 0;
	i2c_start(ICM20948+I2C_WRITE);		// 68 << 1 = D0
	i2c_write(WHO_AM_I);				// select who_am_i
	i2c_stop();							// halt i2c
	i2c_start(ICM20948+I2C_READ);		// D0 + 1(TWI READ)
	who_am_i = i2c_readNak();			// save to variable
	snprintf(buffer,sizeof(buffer),"DEVICE ID: %02x\r\n",who_am_i);
	USART_tx_string(buffer);
	i2c_stop();
}

/* Change clksel to use best available clock source  */
void ICM20948_Init(void){
	i2c_start(0xD0);					// select ICM20948 (0x68<<1)+0
	i2c_write(0x06);					// select pwr_mgmt_1
	i2c_write(0x01);					// set bit 1
	i2c_stop();
	

}

/* modified to writes to and reads from reg+1 */
uint16_t ICM20948_readreg16(uint8_t reg)
{
	i2c_start(ICM20948+I2C_WRITE);		// set device address and write mode
	i2c_write(reg);						// ACCEL_XOUT
	i2c_start(ICM20948+I2C_READ);		// set device address and read mode
	raw = i2c_readNak();                // read one intermediate byte
	i2c_start(ICM20948+I2C_WRITE);
	i2c_write(reg + 1);
	i2c_start(ICM20948+I2C_READ);
	bigraw = (raw<<8) | i2c_readNak();  // read last byte
	i2c_stop();
	return bigraw;
}

void ICM_write(uint8_t reg, uint8_t data){
	i2c_start(ICM20948+I2C_WRITE);    // start I2C for writing 0xD0
	i2c_write(reg);        // write register address to read
	i2c_write(data);    // write data to be saved to register
	i2c_stop();            // stop I2C
	
	ICM_write(0x06, 0x01);    // exit sleep mode, set clk to auto
	ICM_write(0x7F, 0x20);    // select User Bank 2
	ICM_write(0x01, 0x29);    // set gyro rate for 250 with LPF of 17Hz
	ICM_write(0x00, 0x0A);    // set gyroscope sample rate for 100Hz
	ICM_write(0x14, 0x15);    // set accelerometer low pass filter to 136Hz and the rate to 8G
	ICM_write(0x11, 0x0A);    // set accelerometer rate to 100hz
	ICM_write(0x7F, 0x00);    // select User Bank 0
	
		/*Initializing Magnetometer */
	ICM_write(0x03, 0x20);	  // Enable I2C master
	ICM_write(0x7F, 0x30);	  // Select User Bank 3
	ICM_write(0x01, 0x07);	  // Set Master Clk speed to 400k
}