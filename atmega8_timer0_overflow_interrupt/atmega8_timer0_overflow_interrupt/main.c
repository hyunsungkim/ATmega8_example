/*
 *	Created: Jan 2016
 *  Author: hyunsungkim <hyunsungkim@postech.ac.kr>
 *
 */

/**
 * Example for timer 0 overflow interrupt of ATmega8.
 * 
 * This example shows how to count seconds by seconds 0 overflow interrupt on ATmega8. Connect an
 * LED to PB0 with resistor(e.g. 100 Ohm) in series. LED will blink every seconds. Note that it
 * counts one second "approximately". You can count 1 second more precisely by setting pescaler
 * to 1 and _ONE_SEC_CNT as 31250. But without clock generator such as crystal, it still has
 * quite big error.
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define _ONE_SEC_CNT 31


// Adding 'volatile' keyword is recommended for global variables used in ISR
volatile int counter = 0;
volatile int seconds = 0;

/*
 * Interrupt Service Routine(ISR) is executed with period (F_CPU/(256*prescaler)).
 * In this case, ISR is executed 30.517... times per second.
 */
ISR(TIMER0_OVF_vect)
{
	counter++;

	if(counter == _ONE_SEC_CNT) {
		counter = 0;
		seconds++;
	}
}

int main(void)
{
	DDRB = 0x01;		// To blink LED, set PB0 as output
	TIMSK = 0x01;		// Set TOIE0(Timer Overflow Interrupt Enable0) bit
	TCCR0 = 0x05;		// Set prescaler as 1024.

	sei();				// Activate enabled interrupts setting I flag in SREG register

	while (1) {
		if(seconds%2)
			PORTB = 0x01;
		else
			PORTB = 0x00;
	}
}
