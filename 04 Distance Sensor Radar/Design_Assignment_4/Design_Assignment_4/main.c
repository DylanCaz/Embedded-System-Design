#define F_CPU 16000000UL
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUDRATE 9600
#define UBBR_9600 103 // 16Mhz with .2% error
#define Trigger_pin PB1 // Trigger Pin
int TimerOverflow = 0;

void USART_init(unsigned int ubrr); //Function to initialize and configure USART
void USART_send(unsigned char data); // Function to send char to serial port
void USART_tx_string( char *stringPtr ); // Function to send string to serial port

int main()
{
	char string[10];
	long cnt;
	double distance;
	
	DDRB = 0x02; // PB0 and PB1 as an output
	USART_init(UBBR_9600);
	
	sei(); // Global Interrupt
	TIMSK1 = (1 << TOIE1); // Enable Timer1 overflow interrupt
	TCCR1A = 0; // Setting bits to zero for Normal operation
	while(1)
	{
		PORTB |= (1 << Trigger_pin); // setting PB1 to trigger pin on HC-SR04
		_delay_us(340);
		PORTB &= ~(1 << Trigger_pin); // Toggle pin off
		
		TCNT1 = 0;
		TCCR1B = (1 << ICES1) | (1 < CS10); // No prescaler, Input Capture Noise Canceler High
		TIFR1 = (1 << ICF1); // Clear Input Capture flag,
		TIFR1 =  (1 << TOV1); // Clear Timer Overflow flag
		
		while((TIFR1 & (1 << ICF1)) == 0); // Waiting for falling edge
		TCNT1 = 0; // Clear timer counter
		TCCR1B = (1 << CS10); // No prescaler, Input Capture Noise Canceler High
		TIFR1 = (1 << ICF1); // Clear Input Capture flag,
		TIFR1 =  (1 << TOV1); // Clear Timer Overflow flag
		TimerOverflow = 0; // Clear Timer overflow count
		
		while((TIFR1 & (1 << ICF1)) == 0); // Waiting for falling edge
		cnt = ICR1 + (65535 * TimerOverflow); // take count
		
		distance = (double)cnt/933; // 16Mhz Timer Frequency, sound speed = 343ms
		
		dtostrf(distance, 2, 2, string); // Convert distance into string
		strcat(string, " cm ");
		USART_tx_string("Distance = ");
		USART_tx_string(string);
		USART_tx_string("\r\n");
		_delay_ms(1000); // Task 3 waiting for 1 second after each output read	

	}
}

//Function to initialize and configure USART
void USART_init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable transmission and reception
	UCSR0B |= (1 << RXCIE0); // enable RX interrupt
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // set frame formate to 8bits, no parity, 1
}
// Function to send char to serial port
void USART_send(unsigned char data)
{
	while (!(UCSR0A & (1 <<UDRE0)));
	UDR0 = data;
}
// Function to send string to serial port
void USART_tx_string(char* stringPtr)
{
	while ((*stringPtr != 0x00))
	{
		USART_send(*stringPtr);
		stringPtr++;
	}
}

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++; // Increment timer overflow count
}