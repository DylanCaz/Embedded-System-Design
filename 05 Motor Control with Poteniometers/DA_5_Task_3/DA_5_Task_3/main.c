/***********************************
Dylan Cazares
CPE301 - Design Assignment 5
Task 3
***********************************/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
void adc_init(void);
void read_adc(void);
void timer_init(void);

int main(void)
{
	DDRB = (1 << 1); // Setting PB1 as an output
	adc_init();
	timer_init();
	while (1)
	{
		read_adc();
		OCR1A = 0.48 * ADC + 150;
	}
}
void adc_init(void)
{
	ADMUX =
	(0 << REFS1) | (1 << REFS0)| // AVCC w/ external cap at AREF pin
	(0 << ADLAR)| // ADC Left Adjust Result
	(0 << MUX3) | (0 << MUX2)| (0<<MUX1)| (0<<MUX0); // Channel ADC0
	ADCSRA =
	(1 << ADEN) | // ADC Enable
	(0 << ADSC) | // ADC Start Conversion Disable
	(1 << ADATE) | // ADC Auto Trigger Disable
	(0 << ADIF) | // ADC Interrupt Flag Disable
	(1 << ADIE) | // ADC Interrupt Enable
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
	
	ADCSRB = 0x00; //set auto trigger to free running mode
}
void read_adc(void)
{
	ADCSRA |= (1 << ADSC); // Start ADC conversion
}
void timer_init(void)
{
	TCCR1A |=
	(1 << COM1A1) | // Clear OC1A on compare match
	(0 << COM1A0) | // Clear OC1A on compare match
	(1 << COM1B1) | // Clear OC1B on compare match
	(0 << COM1B0) | // Clear OC1B on compare match
	(1 << WGM11) | // Mode Selection bits
	(0 << WGM10); // PWM mode
	
	TCCR1B |=
	(0 << ICNC1) | // Input Capture Noise Canceler
	(0 << ICES1) | // Input Capture Edge Select
	(1 << WGM13) | //Wavefrom Generation Mode
	(1 << WGM12) | //Fast PWM Mode
	(0 << CS12) | // Ckock Select
	(1 << CS11) | // Prescaler = 64
	(1 << CS10);
	
	ICR1 = 4999; // Period = 20ms (50Hz)
	OCR1A = 150;
}