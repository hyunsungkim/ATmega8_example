/*
 * Servo.c
 *
 * Created: Oct, 2015
 * Author : hyunsungkim@postech.ac.kr
*/

/**
 * Example for handling pwm output on ATmega8
 * 
 * This example shows how to handle pwm output to control servo. Connect servo motor's signal pin
 * with PB1. PWM width changes 0ms to 10ms and 90ms to 180ms and 180ms to 0ms, rotating servo horn.
 * Servo motor is supposed to be a standard servo (pwm period 20ms)
 */


#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

#define PWM_ON_PB1 1
#define PWM_ON_PB2 2

void pwm_init()
{
	// Setting TOP value 19999 makes period of pwm signal to 20ms
	ICR1 = 19999;
	// Setting COM1A1 enables pwm output on PB1
	// Setting COM1B1 enables pwm output on PB2
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	// This configuration means Fast PWM mode with TOP=ICR1
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
}

/*
 * This function sets pwm width on pwm channels
 * Width should be in 0~20
 */
void pwm_width_ms(int channel, float width)
{
	if(channel == PWM_ON_PB1)
		OCR1A = (ICR1 / 20) * width;
	else if(channel == PWM_ON_PB2)
		OCR1B = (ICR1 / 20) * width;
	else return;
}

int main(void)
{
	DDRB |= (1 << PB1) | (1 << PB2);
	pwm_init();
    while (1) {
		pwm_width_ms(1, 0);
		_delay_ms(1000);
		pwm_width_ms(1, 10);
		_delay_ms(1000);
		pwm_width_ms(1, 20);
		_delay_ms(1000);
    }
}

