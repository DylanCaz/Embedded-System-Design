/*
 * Design_Assignment_2_C.c
 *
 * Created: 2/19/2022 9:06:08 PM
 * Author : Dylan Cazares
 * Task 2
 */ 
#define F_CPU 16000000			// 16MHz			
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>			// Delay Library

int main(void)
{
	DDRC &= (0 << 3);			// setting PORTC.3 as an input
	PORTC |= (1 << 3);			// enabling pull up
	
	
	DDRB |= (1 << 2);			// setting PORTB.2 as an output
	DDRB |= (1 << 3);			// setting PORTB.3 as an output
	PORTB |= (1 << 2);			// enabling pull up
	
						
    while (1) 
    {
		if(!(PINC & (1 << PINC3))) 
		{
			PORTB &= ~(1 << 2);	// turn PORTB.2 LED on
			_delay_ms(1250);	// delay 1250 ms
			PORTB |= (1 << 2);	// turn PORTB.2 LED off
		}
		else if 
		{
			PORTB |= (1 << 2);	// turn PORTB.2 LED off

		}
    }
}

/*
 * Design_Assignment_2_C.c
 *
 * Created: 2/19/2022 9:06:08 PM
 * Author : Dylan Cazares
 * Task 3
 */ 
#define F_CPU 16000000			// 16MHz			
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>			// Delay Library



int main(void)
{
	DDRC &= (0 << 3);			// setting PORTC.3 as an input
	PORTC |= (1 << 3);			// enabling pull up
	
	
	DDRD &= (1 << 2);			// setting PORTD.2 as an input
	PORTD |= (1 << 2);			// enabling pull up

	
	DDRB |= (1 << 2);			// setting PORTB.2 as an output
	DDRB |= (1 << 1);			// setting PORTB.1 as an output
	PORTB |= (1 << 2);			// enabling pull up
	
						
    while (1) 
    {
		if(!(PINC & (1 << PINC3))) 
		{
			PORTB &= ~(1 << 2);	// turn PORTB.2 LED on
			_delay_ms(1250);	// delay 1250 ms
			PORTB |= (1 << 2);	// turn PORTB.2 LED off
		}
		else if (!(PIND & (1 << PIND2)))
		{
	
			EICRA = 0x02;				// make INT0 falling edge triggered
			EIMSK = (1 << INT0);		// enable external interrupt 0
			sei();						// enable interrupts
		}
    }
}
ISR(INT0_vect)						// ISR for external interrupt 0
{
		PORTB ^= ~(1 << 3);			// toggle PORTB.3 on
		_delay_ms(500);				// delaying for 500ms
		PORTB |= (1 << 3);			// toggle PORTB.3 off
}