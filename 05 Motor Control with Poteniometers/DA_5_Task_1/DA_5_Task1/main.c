/*******************************
Dylan Cazares
CPE301 - Design Assignment 5
Task 1
*******************************/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
void adc_init(void);
void read_adc(void);
void timer_init(void);
float duty;

int main(void)
{
	DDRD |= (1 << 5) ; // setting PD5 PD6 as an output
	timer_init();
	adc_init();
	while (1)
	{
		read_adc(); // read Potentiometer Value
		duty = (ADC/1023.0) * 155.0; // scale for percentage
		OCR0B = duty;
		// ClockWise
		PORTD |= (1 << 5);
	}
}

void adc_init(void)
{
	ADMUX |= (0 << REFS1) | // Reference Selection Bits
	(1 << REFS0) | // AVCC with external capacitor at AREF pin
	(0 << ADLAR) | // Left adjusts
	(0 << MUX3) | (0 << MUX2)| (0 << MUX1) | (0 << MUX0); // Channel 0 (PC0)
	
	ADCSRA = (1 << ADEN) | // ADC Enable
	(0 << ADSC) | // ADC Start Conversion
	(0 << ADATE) | //ADC Auto Trigger Enab8le
	(0 << ADIF) | // ADC Interrupt Flag
	(0 << ADIE) | // ADC Interrupt Enable
	(1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0); // prescaler 64
}
void read_adc(void)
{
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
}

void timer_init(void)
{
	//FAST PWM
	TCCR0A =
	(0 << COM0A1) | // Normal Port Operation
	(0 << COM0A0) | // Normal Port Operation
	(1 << COM0B1) | // Clear OC0A on Compare Match
	(0 << COM0B0) | // Set )C0A at Bottom (non-inverting mode)
	(1 << WGM01) | (1 << WGM00); // Fast PWM Mode
	TCCR0B =
	(0 << FOC0A) | // Input Capture Noise Canceler
	(0 << FOC0B) | // Input Capture Edge Select
	(1 << WGM02) | // Top = OCRA,
	(1 << CS02) | (0 << CS01) | (1 << CS00); // Prescaler 1024
	OCR0A = 155; // FOCnx = 100Hz
}
