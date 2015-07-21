
#include "OWI/OWIBitFunctions.h"
#include "OWI/OWIHighLevelFunctions.h"
#include "temperature.h"

unsigned char celoe, drobnoe;
int delitel;
char sign;
char ram[10];


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
}


long Temperature_calc(void)
{	
	float a;
	a = (float)((ram[1]<<8) + ram[0])/16;
	return (long) (a*10);
}

