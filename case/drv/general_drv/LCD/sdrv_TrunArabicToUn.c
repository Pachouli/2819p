/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_TrunArabicToUn.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint8 UnicodeBuffer[128];

/*
*******************************************************************************
** FUNCTION:    TurnArabicToUn
**
** PURPOSE:     将阿拉伯和希伯莱非Unicode字符串转换为Unicode
**
** PARAMETERS:
**  input
**      charbuf: 字符串指针
**      length: 要转换为Unicode的字符数目，为-1表示全部转换
**  output
**      char: 返回转换的字符数
** DESCRIPTION:
****************************************************************************
*/
uint8 TurnArabicToUn(uint8* charbuf, bool IsUnicode, uint16 CharCount)
{
    uint32 bufindex;
    uint8 i;
    uint32 length;
    uint16 ucode;

    if(CharCount == (uint16)-1)
    {
        if(IsUnicode != 0)   //仅仅用来求取该Unicode字符串的字符数目
        {
            char totallength = 0;
            while(1)
            {
                if(*(uint16 *)charbuf == 0x00)
                {
                    break;
                }
                totallength ++;
                charbuf ++;
                charbuf ++;
            }
            
            return totallength;
        }
        length = 254;
    }
    else
    {
        length = CharCount*2;
    }

    bufindex = 0;
    for(i = 0; i < length; i++)
    {
        ucode = charbuf[i];
        if(ucode == 0)
        {
            break;
        }
        if(ucode>=0x80)
        {
            /*需进行多国语言的转换*/
            Sdrv_GetUniCode(ucode, &UnicodeBuffer[bufindex]);
        }
        else
        {
            *((int16*)(&UnicodeBuffer[bufindex])) = ucode;
        }
        bufindex++;
        bufindex++;
        if(bufindex >=126)
        {
            break;
        }
    }
    *((int16*)(&UnicodeBuffer[bufindex])) = 0;
    bufindex++;
    bufindex++;
    libc_memcpy(charbuf,UnicodeBuffer,bufindex);
    
    return (bufindex/2) - 1;
}

#endif
