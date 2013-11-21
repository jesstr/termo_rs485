
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "dynindication.h"
#include "OWIBitFunctions.h"
#include "modbus.h"
#include "temperature.h"

/*
#include "dynindication.c"
#include "temperature.c"
#include "uart.c"
#include "modbus.c"
*/

extern unsigned char DisplayBuffer[];

extern BYTE AOHR_registers[];

unsigned char display_mode=0; //Определяет режим отображения температуры на индикаторе: 0 - с десятыми долями, 1 - без десятых долей

void Animation(void)
{
		PORTC&=~(1<<0);
		_delay_ms(50);
		PORTC|=(1<<0);
		PORTB&=~(1<<3);
		_delay_ms(50);
		PORTB|=(1<<3);
		PORTB&=~(1<<4);
		_delay_ms(50);
       	PORTB|=(1<<4); 
} //Animation(void)


int main(void)
{
    cli();
	DI_IO_Init();
	OWI_Init(OWI_BUS);
	MODBUS_Init();
	StartRec = FALSE;
	sei();
//	short unsigned int temperature=5678;
// 	DisplayBuffer[0]=1;
// 	DisplayBuffer[1]=2;
// 	DisplayBuffer[2]=3;
// 	DisplayBuffer[3]=4;
	
	//Animation();
	
	SWITCH_Receive;
	
	long int temp;
	float ftemp;
	BYTE *ptr;
	
    while(1)
    {	
		DI_DrawDisplay();
		_delay_us(DISPLAY_UPDATE_DELAY);	
		if (GetTemperature()!=0xFFFF)
		{

// 			DisplayBuffer[3]=GRAD;
// 			DisplayBuffer[0]=(celoe/10)%10;
// 			DisplayBuffer[1]=celoe%10;
// 			DisplayBuffer[2]=drobnoe;

// 			Temperature_calc();
// 			DisplayBuffer[0]=(celoe/10)%10;
// 			DisplayBuffer[1]=celoe%10;
// 			DisplayBuffer[2]=GRAD;
// 			DisplayBuffer[3]=CEL;

			temp=Temperature_calc1();
			
			switch (display_mode)
			{
				case 0: 
				{
					DisplayBuffer[1]=(temp/100)%10;
					DisplayBuffer[2]=(temp/10)%10;
					DisplayBuffer[3]=temp%10;	
					if (temp/100==0) { if (temp<0) {DisplayBuffer[0]=NONE; DisplayBuffer[1]=MINUS;} 
										else {DisplayBuffer[0]=NONE; DisplayBuffer[1]=NONE;} }					
					else if (temp<0) {DisplayBuffer[0]=MINUS;} 
							 else {DisplayBuffer[0]=NONE;}
				}
				case 1:
				{
					DisplayBuffer[1]=(temp/100)%10;
					DisplayBuffer[2]=(temp/10)%10;
					DisplayBuffer[3]=GRAD;	
					if (temp/100==0) { if (temp<0) {DisplayBuffer[0]=NONE; DisplayBuffer[1]=MINUS;} 
										else {DisplayBuffer[0]=NONE; DisplayBuffer[1]=NONE;} }					
					else if (temp<0) {DisplayBuffer[0]=MINUS;} 
							 else {DisplayBuffer[0]=NONE;}					
				}
			}			
		}
		//temp=304;
		//ptr=&temp;
		ftemp=(float)(temp/10);
		ptr=&ftemp;
		AOHR_registers[1]=*ptr++;
		AOHR_registers[0]=*ptr++;
		AOHR_registers[3]=*ptr++;
		AOHR_registers[2]=*ptr++;
		MODBUS1();		
    }
} //main(void)
