#include "reg51.h"
#include "intrins.h"
#include "stdio.h"

sfr     TH2     =   0xD6;
sfr     TL2     =   0xD7;
sfr     AUXR    =   0x8E;
sfr     P_SW2   =   0xBA;

sfr     P1M1    =   0x91;
sfr     P1M0    =   0x92;
sfr     P0M1    =   0x93;
sfr     P0M0    =   0x94;
sfr     P2M1    =   0x95;
sfr     P2M0    =   0x96;
sfr     P3M1    =   0xb1;
sfr     P3M0    =   0xb2;
sfr     P4M1    =   0xb3;
sfr     P4M0    =   0xb4;
sfr     P5M1    =   0xc9;
sfr     P5M0    =   0xca;
sfr     ADC_CONTR   =   0xbc;
sfr     ADC_RES     =   0xbd;
sfr     ADC_RESL    =   0xbe;
sfr     ADCCFG      =   0xde;

#define ADCTIM  (*(unsigned char volatile xdata *)0xfea8)


#define MAIN_Fosc   22118400L
#define Baudrate    115200L
#define TM          (65536 -(MAIN_Fosc/Baudrate/4))

bit B1S_Flag;

int *BGV;

void RTC_config(void);

void UartInit(void)
{
    SCON = (SCON & 0x3f) | 0x40;
    TL2  = TM;
    TH2  = TM>>8;
    AUXR |= 0x15;
}

void UartPutc(unsigned char dat)
{
    SBUF = dat;
    while(TI==0);
    TI = 0;
}

char putchar(char c)
{
    UartPutc(c);
    return c;
}

void Delay500ms()		//@22.1184MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


float ADCRead() 
{
	unsigned int res;
	float val;

	ADC_CONTR |= 0x40;
	_nop_();
	_nop_();
	while(!(ADC_CONTR & 0x20));
	ADC_CONTR &= ~0x20;
	res = ADC_RES & 0x0f;
	res <<= 8;
	res = res | (ADC_RESL);
	//printf("res : %u\n", res);
	val = 2.5 * res * 100 / 4096;
	return val;
}


void main(void)
{
    P0M1 = 0;   P0M0 = 0;                       //设置为准双向口
    P1M1 = 0;   P1M0 = 0;                       //设置为准双向口
    P2M1 = 0;   P2M0 = 0;                       //设置为准双向口
    P3M1 = 0;   P3M0 = 0;                       //设置为准双向口
    P4M1 = 0;   P4M0 = 0;                       //设置为准双向口
    P5M1 = 0;   P5M0 = 0;                       //设置为准双向口

    UartInit();
    EA = 1;
    printf("STC8H4K64TLR RTC Test Programme!\r\n"); //UART发送一个字符串

    P1M0 = 0x00;                                //设置P1.0为ADC口
    P1M1 = 0x01;
    P_SW2 |= 0x80;
    ADCTIM = 0x3f;                              //设置ADC内部时序
    P_SW2 &= 0x7f;
    ADCCFG = 0x2f;                              //设置ADC时钟为系统时钟/2/16
    ADC_CONTR = 0x80;                           //使能ADC模块

    while (1)
    {
		float vcc = 0;
		unsigned int result;
		int i;
		for(i = 0; i < 8; i++)
		{
			vcc += ADCRead();
		}
		vcc /= 8;	
		result = vcc;
		printf("Voltage = %.2fV\n", vcc / 100);
		Delay500ms();
    }

}
