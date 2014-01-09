#!/bin/sh

MK=m8
PRG=avr910
BAUD=57600
PORT=/dev/ttyACM0
FLASHFILE=./Release/termo_rs485.hex

sudo avrdude -p $MK -c $PRG -b $BAUD -P $PORT -U flash:w:$FLASHFILE

# Manual run:
# avrdude -p m8 -c avr910 -b 57600 -P /dev/ttyACM03 -U flash:w:"./Release/termo_rs485.hex":i
