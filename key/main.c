#include<STC8H.H>
#include "intrins.h"


void Delay20ms()		//@24.000MHz
{
	unsigned char data i, j, k;

	_nop_();
	i = 3;
	j = 112;
	k = 91;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

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
		if(P21 == 0) 
		{
			Delay20ms();
			if(P21 == 0)
			{
				P20 = 0;
			}
			else 
			{
			    P20 = 1;
			}
		}
		else 
		{
			P20 = 1;
		}	
	}
}
