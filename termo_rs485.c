#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "OWI/OWIBitFunctions.h"
#include "dynindication.h"
#include "modbus.h"
#include "temperature.h"


unsigned char display_mode = 0; //Temperature output mode: 0 - %2.1f, 1 - %2d
long CurrentTemperature = 0;

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
}


int main(void)
{
    cli();
	DI_IO_Init();
	OWI_Init(OWI_BUS);
	MODBUS_Init();
	StartRec = FALSE;
	sei();
	SWITCH_Receive;
	
	/* Skip first value 85C */
	GetTemperature();

    while(1)
    {	
		DI_DrawDisplay();
		_delay_us(DISPLAY_UPDATE_DELAY);	
		if (GetTemperature() != 0xFFFF)
		{
			CurrentTemperature = Temperature_calc();
			
			unsigned char i;
			for (i = 0; i < DIGITS_COUNT; i++) {
				DisplayBuffer[i] = NONE;
			}


			if (CurrentTemperature < 0) {
				CurrentTemperature = 0 - CurrentTemperature;

				if (CurrentTemperature / 100 != 0) {
					DisplayBuffer[0] = MINUS;
					DisplayBuffer[1] = (CurrentTemperature / 100) % 10;
				}
				else {
					DisplayBuffer[1] = MINUS;
				}
			}
			else {
				if (CurrentTemperature / 100 != 0) {
					DisplayBuffer[1] = NONE;
					DisplayBuffer[1] = (CurrentTemperature / 100) % 10;
				}
				else {
					DisplayBuffer[1] = NONE;
				}
			}

			DisplayBuffer[2] = (CurrentTemperature / 10) % 10;

			/* Display mode 0: [ |-][1-8][0-9][*]  Example: -10*  */
			if (display_mode == 0) {
				DisplayBuffer[3] = GRAD;
				D6_OFF;
			}
			/* Display mode 1: [ |-][1-8][0-9].[0-9]  Example: -10.5*  */
			else if (display_mode == 1) {
				DisplayBuffer[3] = CurrentTemperature % 10;
				D6_ON;
			}
		}

		/* Send temperature as Word */
		signed int temp = CurrentTemperature / 10;

		AOHR_registers[0] = temp >> 8;
		AOHR_registers[1] = (unsigned char) (temp);

		MODBUS1();
    }
}

