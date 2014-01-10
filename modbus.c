#include <util/delay.h>
#include "modbus.h"
#include "uart.h"
#include "dynindication.h"


// ������� ������ slave-����������

#define AOHR_Num 4
#define AIR_Num 0
#define DIC_Num 1
#define DOC_Num 1

BYTE DOC_registers[DOC_Num]={0}; // 1-9999, Discrete Output Coils (Read-Write) - �������� ������ (1 ���)

BYTE DIC_registers[DIC_Num]={0}; // 10001-19999, Discrete Input Contacts (Read-Only) - ���������� ����� (1 ���)

BYTE AIR_registers[AIR_Num]; // 30001-39999, Analog Input Registers (Read-Only) - �������� ����� (16 ���)

BYTE AOHR_registers[AOHR_Num]={0,0,0,0}; // 40001-49999, Analog Output Holding Registers (Read-Write) - �������� �������� (16 ���)


//������� ��� �������� ������� ���� CRC-16
const BYTE srCRCHi[256]={
//const BYTE srCRCHi[256]={
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
         0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
         0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
         0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
         0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
         0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
         0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
         0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
         0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
         0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
         0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
         0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
         0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
         0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
         0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
const BYTE srCRCLo[256]={
//const BYTE srCRCLo[256]={
         0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
         0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
         0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
         0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
         0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
         0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
         0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
         0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
         0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
         0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
         0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
         0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
         0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
         0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
         0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
         0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
         0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
         0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
         0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
         0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
         0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
         0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
         0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
         0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
         0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
         0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

bool StartRec = FALSE;

//��������� ��������� ���������� ModBus
void MODBUS1(void){  
  
    if (bModBus){
      cNumTrByte1=ModBus(cNumRcByte1);


		
// 		DisplayBuffer[3]=cNumRcByte1%10;
// 		DisplayBuffer[2]=(cmRcBuf1[0])%10;
// 		DisplayBuffer[1]=(cmRcBuf1[0]/10)%10;
// 		DisplayBuffer[0]=(cmRcBuf1[0]/100)%10; 
		
/*
		//��� ������������, ����� �� ����� � ��������� �������� ����������
	  	LcdWriteCom(0x85);
		LcdWriteString(IntToStr(AOHR_registers[1], 10));
		LcdWriteCom(0xC6);
		LcdWriteData(AOHR_registers[1]);
	  	AOHR_registers[1]++;
*/
      if (cNumTrByte1!=0) StartTrans1();
      bModBus=FALSE;
    }//end if (bModBus)
  
}//end MODBUS()


//������� ��������� ��� CRC-16
//�� ����� ��������� �� ������ ������
//� ���������� ���� ��������� (��� ��������� ���� CRC-16)
unsigned int GetCRC16(BYTE *buf, BYTE bufsize)
{
  BYTE CRC_Low = 0xFF;
  BYTE CRC_High = 0xFF;
  BYTE k;
  BYTE carry;
  for (k=0; k<bufsize; k++)
   {
    carry = CRC_Low ^ buf[k];
    CRC_Low = CRC_High ^ srCRCHi[carry];
    CRC_High = srCRCLo[carry];
   };
  //return (CRC_High);
  return((CRC_High<<8)|CRC_Low);
}//end GetCRC16()

//������������ ������ �� ������
char ErrorMessage(char Error){
  unsigned int TempI;
  cmTrBuf1[1]=cmRcBuf1[1]+0x80;//������� � ������� 
  cmTrBuf1[2]=Error;
  TempI=GetCRC16(cmTrBuf1,3);//������� �� �������
  cmTrBuf1[3]=Low(TempI);
  cmTrBuf1[4]=Hi(TempI);  
  return 5;
}//end ErrorMessage()

unsigned int CRC16;


BYTE ModBus(BYTE NumByte){
  BYTE RNum=0;  //����� ������ ������� � ������� ������ �������� ������
  BYTE RSum=0;  //���������� ������������ ����
  
unsigned int TempI=0;

   //����� ������� �� �����
  cmRcBuf1[NumByte]=0x00;


  //��������� �������
  if (cmRcBuf1[0]!=DEVICE_ADDRESS) return 0x00; //����� ����������  //����� �� �����
 
  CRC16=GetCRC16(cmRcBuf1,NumByte-2);//������� CRC � �������� �������
  //	LcdWriteData(CRC16);
  TempI=(unsigned int) (cmRcBuf1[NumByte-1]<<8) + cmRcBuf1[NumByte-2];  
  if (CRC16!=TempI) return 0x00;  //����������� ����� //����� �� �����
		
  
  cmTrBuf1[0]=DEVICE_ADDRESS;//����� ����������
  //��� �������
  switch(cmRcBuf1[1]){
    case 0x03:{//������ ���������
      TempI=(unsigned int) (cmRcBuf1[2]<<8) + cmRcBuf1[3];
      RNum=TempI;     //��������� ����� ������� ��������, � ������� ������ ��������
      if (RNum>=0x64){//�������� ������ ��������, ���� ������ 100 ��������� (200 ����)
        return ErrorMessage(0x02);  //������ ����� �� ����� ���� ���������
      }
      TempI=(unsigned int) (cmRcBuf1[4]<<8) + cmRcBuf1[5];
      RSum=TempI;   //��������� ���������� ������������ ���������
      if ((RSum>0x64)||((RNum+RSum)>0x64)){//�������� ���-�� ������������� ���������, ���� ������ 100 ��������� ||
                                           //���� ������ ������ ������ � ����� � ����������� ������������ ����� ������
                                           // ������ ���������� ��������� �����, �� ����� ������
                                           return ErrorMessage(0x02); //������ ����� �� ����� ���� ���������
                                          }
      cmTrBuf1[1]=0x03;//�������      
      
      BYTE i=RSum*2;      
      BYTE j=RNum*2; 
      cmTrBuf1[2]=i;//���-�� ������������ ���� ������
      
      
      BYTE n=3;     //�������� �������� � 3-�� �������� ������� /0-������, 1-�������, 2-���-�� ���� ��� CRC/
      
      while(i--){
                 //cmTrBuf1[n++]=MODBUS_MAS[j++];       //����� RNum ������������ ��� ������ ������ ������������ ������ MODBUS_MAS � �����
				 cmTrBuf1[n++]=AOHR_registers[j++]; 
                }                                     //������������ ��� ������ ������� ������������ ������
      
      TempI=GetCRC16(cmTrBuf1,n);//������� �� �������
      cmTrBuf1[n++]=Low(TempI);
      cmTrBuf1[n++]=Hi(TempI); 
      return n;
    }
    case 0x06:{//������ � ��������� �������
      TempI=(unsigned int) (cmRcBuf1[2]<<8) + cmRcBuf1[3];
      RNum=TempI;
      if (RNum>=0x64){ //�������� ������ ��������, ���� ������ 100 ���������
                       return ErrorMessage(0x02); //������ ����� �� ����� ���� ���������
                     }
      TempI=(unsigned int) (cmRcBuf1[4]<<8) + cmRcBuf1[5];      
      BYTE c=RNum*2;   //� MODBUS ���� ������� ��� ��� �����, ������� ��� ��������� ������� ���������� ����� 
      //MODBUS_MAS[c++]=Hi(TempI);    //����� ������ � MODBUS_MAS
      //MODBUS_MAS[c]=Low(TempI);
      AOHR_registers[c++]=Hi(TempI);    //����� ������ � AOHR_registers
	  AOHR_registers[c]=Low(TempI);
	  
	  
      //WriteMODBUS_MAS_ToEEPROM();   //����� ������ � EEPROM
      //ReadSettings();      //��������� ����������, ��� �� EEPROM                 
      
      cmTrBuf1[1]=cmRcBuf1[1];//�������
      cmTrBuf1[2]=cmRcBuf1[2];//�����
      cmTrBuf1[3]=cmRcBuf1[3];//
      cmTrBuf1[4]=cmRcBuf1[4];//������
      cmTrBuf1[5]=cmRcBuf1[5];//
      cmTrBuf1[6]=cmRcBuf1[6];//��
      cmTrBuf1[7]=cmRcBuf1[7];//
      return 8;
    }
    
    case 0x10:{//������ � ��������� ���������
      TempI=(unsigned int) (cmRcBuf1[2]<<8) + cmRcBuf1[3];
      RNum=TempI;
      if (RNum>=0x64){ //�������� ������ ��������, ���� ������ 100 ���������
         return ErrorMessage(0x02); //������ ����� �� ����� ���� ���������
      }
      
      TempI=(unsigned int) (cmRcBuf1[4]<<8) + cmRcBuf1[5];
      RSum=TempI;   //��������� ���������� ���������� ���������
      if ((RSum>0x64)||((RNum+RSum)>0x64)){//�������� ���-�� ������������� ���������, ���� ������ 100 ��������� ||
                                           //���� ������ ������ ������ � ����� � ����������� ������������ ����� ������
                                           // ������ ���������� ��������� �����, �� ����� ������
                                           return ErrorMessage(0x02); //������ ����� �� ����� ���� ���������
                                          }
      
      
      TempI=(unsigned int) cmRcBuf1[6];   //���������� ���������� ���� 
      BYTE BSum=TempI;    //��������� ���������� ���������� ������ ��� ������������� ��� ���������� ������� ������
      
      BYTE n=7;     //������ �������� � 7-�� �������� ������� /0-������, 1-�������, 2,3-����� ��������, 4,5-���.�� ���������, 6-���.�� ����
      
      while(BSum--){
                    //MODBUS_MAS[RNum++]=cmRcBuf1[n++];    //�������� ������ ������� MODBUS_MAS
					AOHR_registers[RNum++]=cmRcBuf1[n++];    //�������� ������ ������� AOHR_registers
                   } 
      //WriteMODBUS_MAS_ToEEPROM();   //����� ������ � EEPROM
      //ReadSettings();      //��������� ����������, ��� �� EEPROM
      
      cmTrBuf1[1]=cmRcBuf1[1];//�������
      cmTrBuf1[2]=cmRcBuf1[2];//�����
      cmTrBuf1[3]=cmRcBuf1[3];//
      cmTrBuf1[4]=cmRcBuf1[4];//���������� ���������� ���������      
      cmTrBuf1[5]=cmRcBuf1[5];//      
      TempI=GetCRC16(cmTrBuf1,6);//������� �� �������      
      cmTrBuf1[6]=Low(TempI);
      cmTrBuf1[7]=Hi(TempI); 
      return 8;
    }
    
    
    
    default:{
       return ErrorMessage(0x01); //������������ �������
    }
  }
}//end ModBus()


//��������� UART
void MODBUS_Init(void){  

  UART_Init(MYUBRR);
  INIT_IO;
  SWITCH_Receive;   
  EnableReceive1;
  InitTimer2;
  StartTimer2;
}//end void StartUART0()

char cTempUART;

//����� ���������� ������ UART 
ISR(USART_RXC_vect){
	
  cTempUART=UDR;
  

  if  (UCSRA&(1<<FE)) return;   //FE-������ �����, OVR - ������������ ������ (�����)


  if (!StartRec){ //���� ��� ������ ����, �� �������� �����
      StartRec=TRUE;
      RcCount=0;
      DataPause=0;
      cmRcBuf1[RcCount++]=cTempUART;
      StartTimer2;
  }
  else{// end if (StartRec==0) //���������� �����
    if (RcCount<MaxLenghtRecBuf){//���� ��� �� ����� ������
      cmRcBuf1[RcCount++]=cTempUART;
    }else{//����� ����������
      cmRcBuf1[MaxLenghtRecBuf-1]=cTempUART;
    }
    DataPause=0;
    TCNT2=dTCNT2;//���������� �������
  }//end else if (StartRec==0)
}//end  __interrupt UART0_RX_interrupt()


//����� ���������� �������� � ����� UART
ISR(USART_UDRE_vect){
  if (TrCount<cNumTrByte1){
    UDR=cmTrBuf1[TrCount];
    TrCount++;
  }else{
    //_delay_ms(200); //PAUSE_US(100) 4800
    //_delay_ms(100);	//9600
	_delay_ms(5);
	SWITCH_Receive;
    StopTrans1;
	TrCount=0;
  }
}//end __interrupt UART0_UDRE_interrupt()


//���������� �������� �� UART, � ��������� ��-�� ������������ ������
void StartTrans1(void){
  //		LcdWriteData('S');					//
  TrCount=0;
  SWITCH_Transmit;
  
  		D1_ON;
		//_delay_ms(5);
		// D1_OFF;
  
  //_delay_ms(50); //4800
  //_delay_ms(25); //9600
  _delay_ms(5);
  GoTrans1;
}//end  void StartTrans1()


 //����� ���������� �������/�������� 2 �� ������������
ISR(TIMER2_OVF_vect){
  if (StartRec){
    StartRec=FALSE; //������� �������
    cNumRcByte1=RcCount;  //���-�� �������� ����
    bModBus=TRUE;//
    TCCR2=0;//��������� ������  
  }
}//end __interrupt void Timer2_overflowed_interrupt()
