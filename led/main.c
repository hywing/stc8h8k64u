#include<STC8H.H>
#include "intrins.h"

unsigned char LED;

void Delay300ms();

void main(void)
{
	P0M0 = 0x00;    P0M1 = 0x00;
    P1M0 = 0x00;    P1M1 = 0x00;
    P2M0 = 0x00;    P2M1 = 0x00;
    P3M0 = 0x00;    P3M1 = 0x00;
    P4M0 = 0x00;    P4M1 = 0x00;
    P5M0 = 0x00;    P5M1 = 0x00;
    P6M0 = 0x00;    P6M1 = 0x00;
    P7M0 = 0x00;    P7M1 = 0x00;
	while(1)
	{
		P20=1;
		Delay300ms();	
		P20=0;
		Delay300ms();	
	}
}

void Delay300ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 28;
	j = 92;
	k = 196;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
