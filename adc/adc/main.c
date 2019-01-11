/*
 * adc.c
 *
 * Created: May, 2015
 * Author: hyunsungkim@postech.ac.kr
 */ 

/**
 * Example for adc on ATmega8
 * 
 * This example shows how to read analog voltage through ADC on ATmega8. Connect PC1
 * with the node of which you want to measure analog voltage. Note that AREF should
 * be connected with VCC pin. Connect LED on PB0 and change voltage. If the voltage
 * is higher than half of VCC, LED turns on. If you want to monitor the value, use
 * UART function.
 */


#include <avr/io.h>

void adc_init(int channel);
int adc_read();

int main(void)
{
	int data;
	DDRB = 1;
	adc_init();
	while(1)
	{
		data = adc_read(PC1);
		if(data > 512)
			PORTB=1;
		else
			PORTB=0;
		//TODO:: Please write your application code
	}
}

void adc_init(int channel)
{
	ADCSRA = 1 << ADEN | 1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2 ;
}

int adc_read(int channel)
{
	int analog;
	// Select channel
	switch (channel)
	{
		case 0 : ADMUX = 1 << REFS0 | 0 << MUX2| 0 << MUX1| 0 << MUX0; break;
		case 1 : ADMUX = 1 << REFS0 | 0 << MUX2| 0 << MUX1| 1 << MUX0; break;
		case 2 : ADMUX = 1 << REFS0 | 0 << MUX2| 1 << MUX1| 0 << MUX0; break;
		case 3 : ADMUX = 1 << REFS0 | 0 << MUX2| 1 << MUX1| 1 << MUX0; break;
		case 4 : ADMUX = 1 << REFS0 | 1 << MUX2| 0 << MUX1| 0 << MUX0; break;
		case 5 : ADMUX = 1 << REFS0 | 1 << MUX2| 0 << MUX1| 1 << MUX0; break;
		case 6 : ADMUX = 1 << REFS0 | 1 << MUX2| 1 << MUX1| 0 << MUX0; break;
		case 7 : ADMUX = 1 << REFS0 | 1 << MUX2| 1 << MUX1| 1 << MUX0; break;
	}
	ADCSRA = (1 << ADSC) | ADCSRA;	// Trigger conversion
	while(ADCSRA & (1<<ADSC));		// Wait until conversion is completed
	analog=ADCW;					// Read converted value
	return analog;
}