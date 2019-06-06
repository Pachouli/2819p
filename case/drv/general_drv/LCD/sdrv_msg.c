/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Sdrv_msg.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint8 string[7];
int16 i;

const char message[] = "  MsgNo:0x";

void Sdrv_MsgBox(uint16 MsgNo,uint8 x,uint8 y)
{
    string[4]=' ';
    string[5]=' ';
    i= (MsgNo >>12) & 0x0f;
    if(i<0x0a)
    {
        string[0] = i + '0';
    }
    else
    {
        string[0] = i + 'A' -10;
    }

    i= (MsgNo >>8) & 0x0f;
    if(i<0x0a)
    {
        string[1] = i + '0';
    }
    else
    {
        string[1] = i + 'A' -10;
    }

    i= (MsgNo >> 4) & 0x0f;
    if(i< 0x0a )
    {
        string[2] = i + '0';
    }
    else
    {
        string[2] = i + 'A' -10;
    }

    i= MsgNo & 0x0f;
    if(i<0x0a)
    {
        string[3] = i + '0';
    }
    else
    {
        string[3] = i + 'A' -10;
    }

    Sdrv_SetTextPos(x,y);
    Sdrv_PutS(message,-1); 
    Sdrv_PutS(string,6);
    //Sdrv_PutS(message,-1); 
    //Sdrv_PutS(string,6);
    return; 
}   

#endif 
