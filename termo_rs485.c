#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "OWI/OWIBitFunctions.h"
#include "dynindication.h"
#include "modbus.h"
#include "temperature.h"

unsigned char display_mode = 0; //Определяет режим отображения температуры на индикаторе: 0 - с десятыми долями, 1 - без десятых долей

void Animation(void)
{
		PORTC &= ~(1<<0);
		_delay_ms(50);
		PORTC |= (1<<0);
		PORTB &= ~(1<<3);
		_delay_ms(50);
		PORTB |= (1<<3);
		PORTB &= ~(1<<4);
		_delay_ms(50);
       	PORTB |= (1<<4);
} //Animation(void)

int main(void)
{
    cli();
	DI_IO_Init();
	OWI_Init(OWI_BUS);
	MODBUS_Init();
	StartRec = FALSE;
	sei();
	
	SWITCH_Receive;
	
	long CurrentTemperature = 0;
	float fTemperature;
	float *ptr;
	
    while(1)
    {	
		DI_DrawDisplay();
		_delay_us(DISPLAY_UPDATE_DELAY);	
		if (GetTemperature() != 0xFFFF)
		{
			CurrentTemperature = Temperature_calc1();
			
			unsigned char i;
			for (i = 0; i < DIGITS_COUNT; i++) {
				DisplayBuffer[i] = NONE;
			}

			switch (display_mode)
			{	/* TODO wrap this realization to a function or separate file */
				case 0:
				case 1:
				{
					if (CurrentTemperature < 0) {
						CurrentTemperature = 0 - CurrentTemperature;

						if (CurrentTemperature / 100 == 0) {
							DisplayBuffer[1] = MINUS;
						}
						else {
							DisplayBuffer[0] = MINUS;
							DisplayBuffer[1] = (CurrentTemperature / 100) % 10;
						}
					}
					else {
						DisplayBuffer[1] = (CurrentTemperature / 100) % 10;
					}

					DisplayBuffer[2] = (CurrentTemperature / 10) % 10;

					if (display_mode == 0) {
						DisplayBuffer[3] = GRAD;
					}
					else if (display_mode == 1) {
						DisplayBuffer[3] = CurrentTemperature % 10;
					}

					/*
					if (CurrentTemperature < 0) {
						CurrentTemperature = 0 - CurrentTemperature;

						if (CurrentTemperature / 100 == 0) {
							DisplayBuffer[1] = MINUS;
						}
						else {
							DisplayBuffer[0] = MINUS;
							DisplayBuffer[1] = (CurrentTemperature / 100) % 10;
						}
					}
					else {
						DisplayBuffer[1] = (CurrentTemperature / 100) % 10;
					}
					DisplayBuffer[2] = (CurrentTemperature / 10) % 10;
					if (display_mode == 0) {
					DisplayBuffer[3] = CurrentTemperature % 10;
					}
					else if (display_mode == 1) {
						DisplayBuffer[3] = GRAD;
					}
					*/

				}
			}
		}
		fTemperature = (float) (CurrentTemperature / 10);
		ptr = &fTemperature;
		AOHR_registers[1] = *ptr++;
		AOHR_registers[0] = *ptr++;
		AOHR_registers[3] = *ptr++;
		AOHR_registers[2] = *ptr++;
		MODBUS1();
    }
} //main(void)
