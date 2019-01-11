/*
 * CTC_Compare_Output_Match.cpp
 *
 * Created: 2015-07-23 오전 11:47:24
 *  Author: Hyunsungkim
 */ 


#include <avr/io.h>

int main(){
	// set up 2 PWM channels on PB1 and PB2 using Timer1
	OCR1A = 24999;						//비교값 OCR1A = 24999 (Compare Value 설정)
	TCCR1A = (1 << COM1A0);					//Non PWM Mode(Normal Mode), Compare Match -> OC1A Toggle
	TCCR1B = (1 << WGM12) | (3 << CS10);			//OCR1A를 Compare Value로 사용하겠음. Prescaler는 64.
	
	DDRB = 0xFF;

	while(1)
	{
		
	}

	return 0;
}