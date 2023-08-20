/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

#include "stc.h"
#include "usb.h"
#include "usb_req_class.h"

BYTE bHidIdle;
BYTE bKeyCode = 0;
BYTE bKeyDebounce = 0;
BOOL fKeyOK;

void usb_req_class()
{
    switch (Setup.bRequest)
    {
    case GET_REPORT:
        usb_get_report();
        break;
    case SET_REPORT:
        usb_set_report();
        break;
    case GET_IDLE:
        usb_get_idle();
        break;
    case SET_IDLE:
        usb_set_idle();
        break;
    case GET_PROTOCOL:
        usb_get_protocol();
        break;
    case SET_PROTOCOL:
        usb_set_protocol();
        break;
    default:
        usb_setup_stall();
        return;
    }
}

void usb_get_report()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (IN_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    Ep0State.pData = UsbBuffer;
    Ep0State.wSize = Setup.wLength;

    usb_setup_in();
}

void usb_set_report()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (OUT_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    Ep0State.pData = UsbBuffer;
    Ep0State.wSize = Setup.wLength;

    usb_setup_out();
}

void usb_get_idle()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (IN_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    Ep0State.pData = &bHidIdle;
    Ep0State.wSize = 1;

    usb_setup_in();
}

void usb_set_idle()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (OUT_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    bHidIdle = Setup.wValueH;

    usb_setup_status();
}

void usb_get_protocol()
{
    usb_setup_stall();
}

void usb_set_protocol()
{
    usb_setup_stall();
}

void usb_class_in()
{
    BYTE button[3];
    BYTE i;
        
    if (DeviceState != DEVSTATE_CONFIGURED)
        return;

    if (!UsbInBusy && fKeyOK)
    {
        fKeyOK = 0;

        button[0] = 0;
        button[1] = 0;
        button[2] = 0;

        switch (bKeyCode)
        {
        case 0xfe:  button[0] = 0x01; break;
        case 0xfd:  button[0] = 0x04; break;
        case 0xfb:  button[0] = 0x02; break;
        case 0xef:  button[1] = 0xfc; break;
        case 0xdf:  button[1] = 0x04; break;
        case 0xbf:  button[2] = 0xfc; break;
        case 0x7f:  button[2] = 0x04; break;
        }
        
        IE2 &= ~0x80;   //EUSB = 0;
        UsbInBusy = 1;
        usb_write_reg(INDEX, 1);
        for (i=0; i<3; i++)
        {
            usb_write_reg(FIFO1, button[i]);
        }
        usb_write_reg(INCSR1, INIPRDY);
        IE2 |= 0x80;    //EUSB = 1;
    }
}

void scan_key()
{
    BYTE key;
    
    P0M0 = 0x00;
    P0M1 = 0x00;

	P2M0 = 0x00;
    P2M1 = 0x00;
    
    key = 0;
    if(P24 == 0) 
	{
		if(P24 == 0)
		{
			P20 = 0;
			key = 0xfe;
		}
		else 
		{
			P20 = 1;
		}
	}
	if(P25 == 0) 
	{
		if(P25 == 0)
		{
			P20 = 0;
			key = 0xfb;
		}
		else 
		{
			P20 = 1;
		}
	}
	if(P26 == 0) 
	{
		if(P26 == 0)
		{
			P20 = 0;
			key = 0xbf;
		}
		else 
		{
			P20 = 1;
		}
	}
	if(P27 == 0) 
	{
		if(P27 == 0)
		{
			P20 = 0;
			key = 0x7f;
		}
		else 
		{
			P20 = 1;
		}
	}
    
    if (key != bKeyCode)
    {
        bKeyCode = key;
        bKeyDebounce = 20;
    }
    else
    {
        if (bKeyDebounce)
        {
            bKeyDebounce--;
            if (bKeyDebounce == 0)
            {
                fKeyOK = 1;
            }
        }
    }
    
}

