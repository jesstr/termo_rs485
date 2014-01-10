/*
#include "OWISWBitFunctions.c"
#include "OWIHighLevelFunctions.c"
#include "OWIcrc.c"
*/
#include "OWI/OWIBitFunctions.h"
#include "OWI/OWIHighLevelFunctions.h"
#include "temperature.h"

unsigned char celoe, drobnoe;
int delitel;
char sign;
char ram[10];

// ��������� �������� ����������� �� �������
// ���������� ��������� � ���� ������������ ��������� ����� � ������� ����� �������
int GetTemperature(void)
{ if (!OWI_DetectPresence(OWI_BUS)) return 0xffff;
  else
  { OWI_SkipRom(OWI_BUS);
	OWI_SendByte(0x44, OWI_BUS);
	if (!OWI_DetectPresence(OWI_BUS)) return 0xffff;
	else
	{ 
	  OWI_SkipRom(OWI_BUS);
	  OWI_SendByte(0xBE, OWI_BUS);
	  ram[0] = OWI_ReceiveByte(OWI_BUS);
	  ram[1] = OWI_ReceiveByte(OWI_BUS);
      return (ram[1]<<8)+ram[0];
	}
  }
} //GetTemperature(void)


// void Temperature_calc(void)
// {	
// 	int temp;
// 	temp = (ram[1]<<8) + ram[0];
// 	sign = (ram[1]>>7);									//��������� ��� = ����
// 	if (sign==1) temp = 0 - temp; 	
// 	celoe = temp >> 4;									//������������� ����� �����
// 	drobnoe = temp & 0b00001111;						//������� ����� (��������)
// 	switch (drobnoe) 									//������ ��������� ������� ����� � ����������� �� ��������
// 		{
// 		case 0: drobnoe=0; break;
// 		case 1: drobnoe=1; break;
// 		case 2: drobnoe=1; break;
// 		case 3: drobnoe=2; break;
// 		case 4: drobnoe=2; break;
// 		case 5: drobnoe=3; break;
// 		case 6: drobnoe=4; break;
// 		case 7: drobnoe=4; break;
// 		case 8: drobnoe=5; break;
// 		case 9: drobnoe=6; break;
// 		case 10: drobnoe=6; break;
// 		case 11: drobnoe=7; break;
// 		case 12: drobnoe=7; break;
// 		case 13: drobnoe=8; break;
// 		case 14: drobnoe=9; break;
// 		case 15: drobnoe=9; break;
// 		}
// }


void Temperature_calc(void)
{	
	int temp;
	temp = (ram[1]<<8) + ram[0];
	sign = (ram[1]>>7);									//��������� ��� = ����
	if (sign==1) temp = 0 - temp; 	
	celoe = temp >> 4;									//������������� ����� �����
	drobnoe = temp & 0b00001111;						//������� ����� (��������)
	switch (drobnoe) 									//������ ��������� ������� ����� � ����������� �� ��������
		{
		case 0: drobnoe=0; break;
		case 1: drobnoe=1; break;
		case 2: drobnoe=1; break;
		case 3: drobnoe=2; break;
		case 4: drobnoe=2; break;
		case 5: drobnoe=3; break;
		case 6: drobnoe=4; break;
		case 7: drobnoe=4; break;
		case 8: drobnoe=5; break;
		case 9: drobnoe=6; break;
		case 10: drobnoe=6; break;
		case 11: drobnoe=7; break;
		case 12: drobnoe=7; break;
		case 13: drobnoe=8; break;
		case 14: drobnoe=9; break;
		case 15: drobnoe=9; break;
		}
}


long Temperature_calc1(void)
{	
	float a;
	a = (float)((ram[1]<<8) + ram[0])/16;
	return (long) (a*10);
}

/*
 int Temperature_calc(int temp)
{	
	float a;
	sign = (temp>>15);									//��������� ��� = ����
	if (sign==1) temp = 0 - temp; 	
	celoe = temp >> 4;									//������������� ����� �����
	drobnoe = temp & 0x0F;								//������� ����� (��������)
	switch (drobnoe) 									//������ ��������� ������� ����� � ����������� �� ��������
		{
		case 0: drobnoe=0; break;
		case 1: drobnoe=1; break;
		case 2: drobnoe=1; break;
		case 3: drobnoe=2; break;
		case 4: drobnoe=2; break;
		case 5: drobnoe=3; break;
		case 6: drobnoe=4; break;
		case 7: drobnoe=4; break;
		case 8: drobnoe=5; break;
		case 9: drobnoe=6; break;
		case 10: drobnoe=6; break;
		case 11: drobnoe=7; break;
		case 12: drobnoe=7; break;
		case 13: drobnoe=8; break;
		case 14: drobnoe=9; break;
		case 15: drobnoe=9; break;
		}
	return celoe*10+drobnoe;
}
*/
