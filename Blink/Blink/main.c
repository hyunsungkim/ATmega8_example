/*
 *	Created: Jan 2016
 *  Author: hyunsungkim <hyunsungkim@postech.ac.kr>
 */

/**
 * Blink LED
 * 
 * This example shows how to handle GPIO. Connect an LED to PB0 with properly valued resistor (e.g. 220 Ohm)
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0x01;		// To blink LED, set PB0 as output

	while (1)
	{
		PORTB = 0x01;		// Apply HIGH (5V or 3.3V) to PB0
		_delay_ms(1000);	// Do nothing for some interval (I know it seems nonsense but, actually it's not 1000ms)
		PORTB = 0x00;		// Apply LOW (0V) to PB0
		_delay_ms(1000);	// Do nothing for some interval
	}
}
