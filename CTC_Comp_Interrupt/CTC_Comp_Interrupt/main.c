/*
 * CTC_Comp_Interrupt.cpp
 *
 * Created: 2015-07-23 오후 2:43:07
 * Author: hyunsungkim
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int flag=0;

ISR(TIMER1_COMPA_vect)
{
	if(flag)
	{
		PORTB = 1;
		flag = 0;
	}
	else
	{
		PORTB = 0;
		flag = 1;
	}
}

int main()
{
	TIMSK = (1 << OCIE1A);					// Compare Match Interrupt Enable
	TCCR1B = (1 << WGM12) | (3 << CS10);	// OCR1A를 Compare Value로 사용. Prescaler=64
	OCR1A = 24999;							// 비교값 OCR1A = 24999
	
	DDRB = 0xFF;
	
	sei();
	
	while (1)
	{
		
	}

}
