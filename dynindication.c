
#include <avr/io.h>
#include <util/delay.h>
#include "dynindication.h"

unsigned char DisplayBuffer[DIGITS_COUNT] = {0,0,0,0};

void DI_IO_Init(void)
{
	a_DDR|=(1<<a_PIN);
	b_DDR|=(1<<b_PIN);
	c_DDR|=(1<<c_PIN);
	d_DDR|=(1<<d_PIN);
	e_DDR|=(1<<e_PIN);
	f_DDR|=(1<<f_PIN);
	g_DDR|=(1<<g_PIN);
	
	All_OFF;
	
	D1_DDR|=(1<<D1_PIN);
	D2_DDR|=(1<<D2_PIN);
	D3_DDR|=(1<<D3_PIN);
	D6_DDR|=(1<<D6_PIN);
	
	D1_OFF;
	D2_OFF;
	D3_OFF;
	D6_OFF;
	
	DIG1_DDR|=(1<<DIG1_PIN);
	DIG2_DDR|=(1<<DIG2_PIN);
	DIG3_DDR|=(1<<DIG3_PIN);
	DIG4_DDR|=(1<<DIG4_PIN);
}


void DI_DrawDisplay(void)
{
	unsigned char marker;
	//D6_ON;
	for (marker=0; marker<DIGITS_COUNT; marker++)
	{	
		switch (marker)
		{
			case 0: DIG1; break;
			case 1: DIG2; break;
			case 2: DIG3; break;
			case 3: DIG4; break;
		}		
		switch(DisplayBuffer[marker])
		{
			case 1: CH_ONE;	    break;
			case 2: CH_TWO;     break;
			case 3: CH_THREE;   break;
			case 4: CH_FOUR;    break;
			case 5: CH_FIVE;    break;
			case 6: CH_SIX;     break;
			case 7: CH_SEVEN;   break;
			case 8: CH_EIGHT;   break;
			case 9: CH_NINE;    break;
			case 0: CH_ZERO;    break;
			case 100: CH_NONE; break;
			case 101: CH_GRAD;  break;
			case 102: CH_CEL;     break;
			case 103: CH_MINUS; break;
			
			
		}		
		_delay_us(DIGIT_SWITCH_DELAY);
	    All_OFF;
		//D6_OFF;
	}
	marker=0;
	//All_OFF;	
}
