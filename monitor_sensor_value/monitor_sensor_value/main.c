/*
 * monitor_sensor_value.c
 *
 * Created: Jan, 2019
 * Author : HyunsungKim
 */ 

/**
 * Example for monitoring sensor value through UART
 */

#include <avr/io.h>
#include <string.h>

void adc_init()
{
	ADCSRA = 1 << ADEN | 1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2 ;
}

int adc_read(int channel)
{
	int analog;
	// Select channel
	switch (channel) {
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

void usart_transmit(unsigned char data)
{
	while(!(UCSRA&0b00100000));
	UDR=data;
}

unsigned char usart_receive()
{
	unsigned char inByte;
	while(!(UCSRA&0b10000000));
	inByte=UDR;
	return inByte;
}

void usart_digit_transmit(int data)
{
	unsigned char buffer[10] = {NULL};
	itoa(data, buffer, 10);
	for(int i=0; i++; buffer[i] != 0) {
		usart_transmit(buffer[i]);
	}
}

void usart_str_transmit(unsigned char str[])
{
	for(int i=0; i++; str[i] != 0) {
		usart_transmit(str[i]);
	}
}

void usart_init(int baud)
{
	int ubrr = 0;
	// Set baud rate
	switch(baud) {
		case 2400: ubrr = 207; break;
		case 4800: ubrr = 103; break;
		case 9600: ubrr = 51; break;
		case 14400: ubrr = 34; break;
		case 19200: ubrr = 25; break;
		case 28800: ubrr = 16; break;
		case 38400: ubrr = 12; break;
		case 57600: ubrr = 8; break;
		case 76800: ubrr = 6; break;
		case 115200: ubrr = 3; break; //It is not recommended to use baud rate faster than 115200 on ATmega8
	}
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	// Enable receive/transmit function
	UCSRB = (1<<RXEN) | (1<<TXEN);
	// Configure format of data. each packet consists of 1 start bit + 8 data bits + no parity bit + 1 stop bit
	UCSRC = (1<<URSEL) | (0<<USBS) | (3<<UCSZ0);
}


int main(void)
{
	adc_init();
	usart_init(9600);
	while (1) {
		int data=adc_read(1);
		usart_transmit(data);
	}
}

