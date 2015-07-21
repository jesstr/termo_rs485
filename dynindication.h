#ifndef  DYNINDICATION_H
#define  DYNINDICATION_H


#define DIGITS_COUNT 4
#define DIGIT_SWITCH_DELAY 200
#define DISPLAY_UPDATE_DELAY 10

#define NONE	100;
#define GRAD	101;
#define CEL		102;
#define MINUS	103;

#define a_PIN PC5
#define b_PIN PC4
#define c_PIN PD2
#define d_PIN PB1
#define e_PIN PC2
#define f_PIN PC3
#define g_PIN PC1

#define D1_PIN PC0
#define D2_PIN PB3
#define D3_PIN PB4
#define D6_PIN PD6

#define DIG1_PIN PB2
#define DIG2_PIN PB0
#define DIG3_PIN PD7
#define DIG4_PIN PD5

#define a_PORT PORTC
#define b_PORT PORTC
#define c_PORT PORTD
#define d_PORT PORTB
#define e_PORT PORTC
#define f_PORT PORTC
#define g_PORT PORTC

#define a_DDR DDRC
#define b_DDR DDRC
#define c_DDR DDRD
#define d_DDR DDRB
#define e_DDR DDRC
#define f_DDR DDRC
#define g_DDR DDRC

#define D1_PORT PORTC
#define D2_PORT PORTB
#define D3_PORT PORTB
#define D6_PORT PORTD

#define D1_DDR DDRC
#define D2_DDR DDRB
#define D3_DDR DDRB
#define D6_DDR DDRD

#define DIG1_PORT PORTB
#define DIG2_PORT PORTB
#define DIG3_PORT PORTD
#define DIG4_PORT PORTD

#define DIG1_DDR DDRB
#define DIG2_DDR DDRB
#define DIG3_DDR DDRD
#define DIG4_DDR DDRD


#define CH_ONE		All_OFF;\
					b_ON;\
					c_ON;

#define CH_TWO		All_OFF;\
					a_ON;\
					b_ON;\
					g_ON;\
					e_ON;\
					d_ON;
				
#define CH_THREE	All_OFF;\
					a_ON;\
					b_ON;\
					c_ON;\
					d_ON;\
					g_ON;

#define CH_FOUR		All_OFF;\
					f_ON;\
					g_ON;\
					b_ON;\
					c_ON;
					
#define CH_FIVE		All_OFF;\
					a_ON;\
					f_ON;\
					g_ON;\
					c_ON;\
					d_ON;
				
#define CH_SIX		All_OFF;\
					a_ON;\
					c_ON;\
					d_ON;\
					e_ON;\
					f_ON;\
					g_ON;
					
#define CH_SEVEN	All_OFF;\
					a_ON;\
					b_ON;\
					c_ON;
					
#define CH_EIGHT	All_OFF;\
					a_ON;\
					b_ON;\
					c_ON;\
					d_ON;\
					e_ON;\
					f_ON;\
					g_ON;
					
#define CH_NINE		All_OFF;\
					a_ON;\
					b_ON;\
					c_ON;\
					d_ON;\
					f_ON;\
					g_ON;	
					
#define CH_ZERO 	All_OFF;\
					a_ON;\
					b_ON;\
					c_ON;\
					d_ON;\
					e_ON;\
					f_ON;
					
#define CH_GRAD	    All_OFF;\
					a_ON;\
					b_ON;\
					g_ON;\
					f_ON;
									
#define CH_CEL	    All_OFF;\
					a_ON;\
					f_ON;\
					e_ON;\
					d_ON;
					
#define CH_MINUS    All_OFF;\
					g_ON;	
					
#define CH_NONE    All_OFF;		


#define a_ON a_PORT&=~(1<<a_PIN) 
#define b_ON b_PORT&=~(1<<b_PIN) 
#define c_ON c_PORT&=~(1<<c_PIN) 
#define d_ON d_PORT&=~(1<<d_PIN) 
#define e_ON e_PORT&=~(1<<e_PIN) 
#define f_ON f_PORT&=~(1<<f_PIN) 
#define g_ON g_PORT&=~(1<<g_PIN) 

#define All_OFF a_PORT|=(1<<a_PIN);\
				b_PORT|=(1<<b_PIN);\
				c_PORT|=(1<<c_PIN);\
				d_PORT|=(1<<d_PIN);\
				e_PORT|=(1<<e_PIN);\
				f_PORT|=(1<<f_PIN);\
				g_PORT|=(1<<g_PIN);

#define D1_ON D1_PORT&=~(1<<D1_PIN) 
#define D2_ON D2_PORT&=~(1<<D2_PIN) 
#define D3_ON D3_PORT&=~(1<<D3_PIN)
#define D6_ON D6_PORT&=~(1<<D6_PIN)

#define D1_OFF D1_PORT|=(1<<D1_PIN) 
#define D2_OFF D2_PORT|=(1<<D2_PIN) 
#define D3_OFF D3_PORT|=(1<<D3_PIN) 
#define D6_OFF D6_PORT|=(1<<D6_PIN) 

#define DIG1	DIG1_PORT|= (1<<DIG1_PIN);\
				DIG2_PORT&=~(1<<DIG2_PIN);\
				DIG3_PORT&=~(1<<DIG3_PIN);\
				DIG4_PORT&=~(1<<DIG4_PIN);
#define DIG2	DIG2_PORT|= (1<<DIG2_PIN);\
				DIG1_PORT&=~(1<<DIG1_PIN);\
				DIG3_PORT&=~(1<<DIG3_PIN);\
				DIG4_PORT&=~(1<<DIG4_PIN);
#define DIG3	DIG3_PORT|= (1<<DIG3_PIN);\
				DIG1_PORT&=~(1<<DIG1_PIN);\
				DIG2_PORT&=~(1<<DIG2_PIN);\
				DIG4_PORT&=~(1<<DIG4_PIN); 
#define DIG4	DIG4_PORT|= (1<<DIG4_PIN);\
				DIG2_PORT&=~(1<<DIG2_PIN);\
				DIG3_PORT&=~(1<<DIG3_PIN);\
				DIG1_PORT&=~(1<<DIG1_PIN); 


extern unsigned char DisplayBuffer[];

void DI_IO_Init(void);

void DI_DrawDisplay(void);


#endif /* DYNINDICATION_H */

