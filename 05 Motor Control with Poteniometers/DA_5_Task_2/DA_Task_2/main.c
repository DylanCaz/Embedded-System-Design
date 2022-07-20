/***********************************
Dylan Cazares
CPE301 - Design Assignment 5
Task 2
***********************************/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void adc_init(void);
void read_adc();
void timer_init();
int main(void)
{
	DDRD |= 0x0F; // Set PD0, PD1, PD2, PD3 as an Output
	timer_init();
	adc_init();
	while (1)
	{
		read_adc();
		// 100ms period at lowest ADC val
		OCR1A = (((ADC) * (3124.0)) / (1023.0));
		if((PIND & 0x10) == 0x10){
			PORTD = 0x09;
			while((TIFR1 & 0x2) != 0x2); // OCF1A (Wait for flag to match)
			TIFR1 |= (1 << OCF1A); // Clear flag
			PORTD = 0x03;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
			PORTD = 0x06;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
			PORTD = 0x0C;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
		}
		else
		{
			PORTD = 0x0C;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
			PORTD = 0x06;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
			PORTD = 0x03;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
			PORTD = 0x09;
			while((TIFR1 & 0x2) != 0x2);
			TIFR1 |= (1 << OCF1A);
		}
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
	(0 << ADATE) | // ADC Auto Trigger Disable
	(0 << ADIF) | // ADC Interrupt Flag Disable
	(0 << ADIE) | // ADC Interrupt Disable
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}
void read_adc()
{
	ADCSRA |= (1 << ADSC); // Start ADC conversion
	while(ADCSRA & (1 << ADSC));
}
void timer_init()
{
	TCCR1B |= (1 << WGM12) | (1 << CS12); // Prescaler 256, CTC Mode
}