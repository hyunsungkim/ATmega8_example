/*
 * usart.c
 *
 * Created: Aug, 2015
 * Author: hyunsungkim@postech.ac.kr
 */ 

/**
 * Example for communicating by USART
 * 
 * This example shows how to communicate with USART interface. Connect PC and ATmega8
 * via TTL/USB converter such as FT232, CP2102, or CH340. Send small letters through
 * terminal programs such as HTerm, TeraTerm, or Putty, then ATmega8 converts letters
 * to upper cases and returns it.
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

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

int main(void)
{
	usart_init(9600);
    while (1) {
		unsigned char inByte = usart_receive();
		usart_transmit(inByte+('A'-'a'));
    }
}

