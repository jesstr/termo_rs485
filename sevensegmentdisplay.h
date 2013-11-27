
#ifndef SEVENSEGMENTDISPLAY_H_
#define SEVENSEGMENTDISPLAY_H_


#define DIGITS_COUNT 4 //����� ���������
#define DIGIT_SWITCH_DELAY 200 //�������� ����� �������������� ���������, ���
#define DISPLAY_UPDATE_DELAY 10 //�������� ����� ������������ �������, ���

//����������� ����� ������ ��������
#define NONE	100;
#define GRAD	101;
#define CEL		102;
#define MINUS	103;


extern unsigned char DisplayBuffer[];

void DI_IO_Init(void);

void DI_DrawDisplay(void);


#endif /* SEVENSEGMENTDISPLAY_H_ */
