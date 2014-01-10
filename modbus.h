#ifndef  MODBUS_H
#define  MODBUS_H

//#include "defines.h"
//#include "eeprom.h"
//#include "defs.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define TRUE 1 
#define FALSE 0

typedef unsigned short bool;
typedef unsigned char BYTE;
typedef unsigned short WORD;

//������ ������ ����������� �� UART ������
#define MaxLenghtRecBuf 8 //255
//������ ������ ������������ �� UART ������
#define MaxLenghtTrBuf 8  //255

#define SetBit(Port,bit) Port|=(1<<bit)
#define ClrBit(Port,bit) Port&=~(1<<bit)
#define InvBit(Port,bit) Port^=(1<<bit)
#define Hi(Int) (BYTE) (Int>>8)
#define Low(Int) (BYTE) (Int)
#define TestBit(Port,bit) ((Port) & 1 << (bit))

#define DEVICE_ADDRESS 0x20
#define RS485_CONTROL_DDR DDRD
#define RS485_CONTROL_PORT PORTD
#define RS485_RE_PIN 3
#define RS485_DE_PIN 3

//����������� �����/�������� � ������������ RS485
// #define INIT_IO  RS485_CONTROL_DDR|=(1<<RS485_RE_PIN)|(1<<RS485_DE_PIN);
// #define SWITCH_Transmit  RS485_CONTROL_PORT|=(1<<RS485_RE_PIN)|(1<<RS485_DE_PIN); 
// #define SWITCH_Receive   RS485_CONTROL_PORT&=~((1<<RS485_RE_PIN)|(1<<RS485_DE_PIN));

#define INIT_IO  RS485_CONTROL_DDR|=(1<<RS485_RE_PIN);
#define SWITCH_Transmit  RS485_CONTROL_PORT|=(1<<RS485_RE_PIN); 
#define SWITCH_Receive   RS485_CONTROL_PORT&=~(1<<RS485_RE_PIN);

//��������� ���������� ������
#define DisableReceive1 ClrBit(UCSRB,RXEN); ClrBit(UCSRB,RXCIE)
//��������� ���������� ������
#define EnableReceive1 SetBit(UCSRB,RXEN); SetBit(UCSRB,RXCIE)
//�������� �� ���������� ���������� ������
#define TestReceive1 TestBit(UCSRB,RXCIE)
//��������� ���������� �� ������������ ������ ��������, ������ ��������
#define GoTrans1 SetBit(UCSRB,TXEN); SetBit(UCSRB,UDRIE)
//��������� ���������� �� ������������ ������ ��������, ��������� ��������
#define StopTrans1 	do {					\
					ClrBit(UCSRB,TXEN); 	\
					ClrBit(UCSRB,UDRIE); 	\
					D1_OFF;					\
					} while(0)
//�������� �� ���������� ���������� ��������
#define TestTrans1 TestBit(UCSRB,UDRIE)


#define StartTimer2 TCNT2=dTCNT2;TCCR2=0x05; //������ ������� 2 (128 prescaler)
#define InitTimer2 TIFR&=0x7F;TIMSK|=(1<<TOIE2); //������������� �������2

//MaxPause = 1.5*(8+1+2)/bod =  for 19200 - 0.86ms  -> TCNT0~155         //for 115200 - 0.143ms -> TCNT0~238
//MaxPause = 1.5*(8+1+2)/bod =  for 4800 - 3.43ms  -> TCNT0~214 
//MaxPause = 1.5*(8+1+2)/bod =  for 9600 - 1.71ms  -> TCNT0~106
//MaxPause = 1.5*(8+1+2)/bod =  for 19200 - 1.71ms  -> TCNT0~53

#define dTCNT2 53;


//ModBus
BYTE ModBus(BYTE NumByte);
bool bModBus;  //���� ��������� �������
BYTE cNumRcByte1; //�������� � ���������� ���-�� �������� ����
BYTE cNumTrByte1;  //���-�� ������������ ������

BYTE RcCount, TrCount;  //������� ��������/���������� ������ �������
bool StartRec;// FALSE/TRUE ������/����� �������

BYTE DataPause;  //����� ����� �������

//UART
BYTE cmRcBuf1[MaxLenghtRecBuf] ; //����� ����������� ������
BYTE cmTrBuf1[MaxLenghtTrBuf] ; //����� ������������ ������

/*
BYTE  MODBUS_MAS[200]={
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0
                            };
*/

extern BYTE DOC_registers[]; // 1-9999, Discrete Output Coils (Read-Write) - �������� ������ (1 ���)

extern BYTE DIC_registers[]; // 10001-19999, Discrete Input Contacts (Read-Only) - ���������� ����� (1 ���)

extern BYTE AIR_registers[]; // 30001-39999, Analog Input Registers (Read-Only) - �������� ����� (16 ���)

extern BYTE AOHR_registers[]; // 40001-49999, Analog Output Holding Registers (Read-Write) - �������� �������� (16 ���)


void MODBUS1(void);
void StartUART1(void);
void MODBUS_Init(void);
void StartTrans1(void);

///extern BYTE  MODBUS_MAS[200];
//extern BYTE MODBUS_MAS[8];


#endif /* MODBUS_H */
