/*
 * atmega8_library.h
 * Hyunsung Kim
 * July 15,2016
 *
 *
 * hyunsung's library
 */

#ifndef _ATMEGA8_H_
#define _ATMEGA8_H_
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>



void AT8_TIMER0_interrupt_init(int prescaler)
{
	TIMSK = 1;
	switch(prescaler)
	{
		case 0:	TCCR0 = 0x00; break;	//stop timer
		case 1:	TCCR0 = 0x01; break;	// no prescaling
		case 8:	TCCR0 = 0x02; break;
		case 64: TCCR0 = 0x03; break;
		case 256: TCCR0 = 0x04; break;
		case 1024: TCCR0 = 0x05; break;
	}
}


void AT8_PWM_init(int channel)
{
	DDRB = DDRB | (1 << DDB0) | (1 << DDB1);

	ICR1 = 19999;
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

	switch(channel)
	{
		case 1:	TCCR1A = (1 << COM1A1) | (1 << WGM11) ; break;
		case 2: TCCR1A = (1 << COM1B1) | (1 << WGM11) ; break;
		case 3: TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) ; break;
	}
}


void AT8_PWM_width(int channel, double width)
{
	switch(channel)
	{
		case 1: OCR1A = (ICR1 / 20) * width; break;
		case 2: OCR1B = (ICR1 / 20) * width; break;
	}
}

void AT8_UART_init(long baud, int rxint, int txint)
{
	int ubrr = 0;
	switch(baud)
	{
		case 2400: ubrr = 207; break;
		case 4800: ubrr = 103; break;
		case 9600: ubrr = 51; break;
		case 14400: ubrr = 34; break;
		case 19200: ubrr = 25; break;
		case 28800: ubrr = 16; break;
		case 38400: ubrr = 12; break;
		case 57600: ubrr = 8; break;
		case 76800: ubrr = 6; break;
		case 115200: ubrr = 3; break;			//115200 이상으로는 오차로 인해 작동하지 않을 수 있음
	}
	
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (0<<USBS) | (3<<UCSZ0);

	if(rxint) UCSRB |= (1<<RXCIE);
	if(txint) UCSRB |= (1<<TXCIE);
}

void AT8_UART_char_transmit(unsigned char data)
{
	while(!(UCSRA&0b00100000));
	UDR=data;
}

unsigned char AT8_UART_receive()
{
	unsigned char data;
	while(!(UCSRA&0b10000000));
	data=UDR;

	return data;
}

void AT8_UART_digit_transmit(int data)
{
	unsigned char buffer[10] = {NULL};
	int length = 0;

	itoa(data, buffer, 10);
	for(int i=0; i++; buffer[i] != 0)
	{
		while(!(UCSRA&0b00100000));
		UDR=buffer[i];
	}
}

void AT8_UART_str_transmit(unsigned char str[])
{
	for(int i=0; i++; str[i] != 0)
	{
		while(!(UCSRA&0b00100000));
		UDR=str[i];
	}
}


void AT8_ADC_init(int channel)
{
	switch(channel)
	{
		case 0 : ADMUX = 1 << REFS0 | 0 << MUX2| 0 << MUX1| 0 << MUX0 ;break;
		case 1 : ADMUX = 1 << REFS0 | 0 << MUX2| 0 << MUX1| 1 << MUX0 ;break;
		case 2 : ADMUX = 1 << REFS0 | 0 << MUX2| 1 << MUX1| 0 << MUX0 ;break;
		case 3 : ADMUX = 1 << REFS0 | 0 << MUX2| 1 << MUX1| 1 << MUX0 ;break;
		case 4 : ADMUX = 1 << REFS0 | 1 << MUX2| 0 << MUX1| 0 << MUX0 ;break;
		case 5 : ADMUX = 1 << REFS0 | 1 << MUX2| 0 << MUX1| 1 << MUX0 ;break;
		case 6 : ADMUX = 1 << REFS0 | 1 << MUX2| 1 << MUX1| 0 << MUX0 ;break;
		case 7 : ADMUX = 1 << REFS0 | 1 << MUX2| 1 << MUX1| 1 << MUX0 ;break;
	}
	
	ADCSRA = 1 << ADEN | 1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2 ;
}

int AT8_ADC_read()
{
	int data = 0;
	ADCSRA = (1 << ADSC) | ADCSRA;
	while(ADCSRA & (1<<ADSC));
	data = (ADCH << 8) | ADCL;

	return data;
}

void AT8_DYNAMIXEL_init(int id, int baud)
{
	//http://support.robotis.com/ko/product/dynamixel/communication/dxl_packet.htm
	//http://support.robotis.com/ko/product/dynamixel/mx_series/mx-28.htm

	int check_sum=0;

	check_sum = ~(id+4+3+16);

	AT8_UART_init(baud, 0, 0);
	AT8_UART_transmit(255);
	AT8_UART_transmit(255);
	AT8_UART_transmit(id);
	AT8_UART_transmit(4);
	AT8_UART_transmit(3);
	AT8_UART_transmit(16);
	AT8_UART_transmit(2);
	AT8_UART_transmit(check_sum)

}

void AT8_DYNAMIXEL_position(int id, int angle)
{
	//angle: 0~1023
	int check_sum = 0;
	int angle_L = 0;
	int angle_H = 0;

	angle_L=(int)angle%255;
	angle_H=(int)(angle/255);
	check_sum=~(id + 5 + 3 + 30 + angle_L + angle_H);
	
	check_sum = ~(id+4+3+16);

	AT8_UART_transmit(255);
	AT8_UART_transmit(255);
	AT8_UART_transmit(id);
	AT8_UART_transmit(5);
	AT8_UART_transmit(3);
	AT8_UART_transmit(30);
	AT8_UART_transmit(angle_L);
	AT8_UART_transmit(angle_H);
	AT8_UART_transmit(check_sum);

}

#endif