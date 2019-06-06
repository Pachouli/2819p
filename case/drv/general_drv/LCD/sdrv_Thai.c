/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_Thai.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

/*
*******************************************************************************
** FUNCTION:    Sdrv_FontCodeMBToUni
**
** PURPOSE:     ��������������ַ�ת��ΪUnicode
**
** PARAMETERS:
**  input
**      de=����    
**  output
**      de= unicode ����
** DESCRIPTION:
****************************************************************************
*/
uint16 Sdrv_FontCodeMBToUni(uint16 ucode)
{
    uint16  UniCode;
    Sdrv_GetUniCode(ucode, &UniCode);
    return UniCode;
}

/*
*******************************************************************************
** FUNCTION:    Sdrv_ThaiOverlap
**
** PURPOSE:     �����ǰ��̩���ַ�����ͽ����ŵ�̩���ַ��ص�����ô�����ص�����
**
** PARAMETERS:
**  input
**      str: ��ǰ�ַ�ָ��,�Է��ʽ��Ӻ�����ַ�
**      IsUnicode: �ַ����Ƿ�ʹ��Unicode����
**  output
**      char: �����ص����ַ���
** DESCRIPTION:
****************************************************************************
*/
uint8 Sdrv_ThaiOverlap(uint8 * str, bool IsUnicode)
{
    uint8 width1,width2;
    uint8 i;
    uint16 thai_ch;
    uint8 ScanLen=0;
    
    while(1)
    {
        thai_ch = ((IsUnicode != 0)? (*(uint16 *)(str+ScanLen)): (*(str+ScanLen)));
        if(thai_ch==0x00)
        {
            break;
        }
        //����ж�����Ե�ת��
        if(!IsUnicode)
        {
            thai_ch = Sdrv_FontCodeMBToUni(thai_ch);
        }
        if((thai_ch==0x0e31) || ((thai_ch<=0x0e3A)&&(thai_ch>=0x0e34)) || ((thai_ch<=0x0e4E)&&(thai_ch>=0x0e47)))
        {
            ScanLen += ((IsUnicode != 0)? 2: 1);
            width1= CharPointBuffer[32];
            if(width1<=8)
            {
                width1 =UNICODE_WIDTH_DEF/2;
            }
            else
            {
                width1 =UNICODE_WIDTH_DEF;
            }
            
            Sdrv_GetFontPoint(thai_ch, AuxiliaryBuffer);
            width2=AuxiliaryBuffer[32];
            if(width2<=8)
            {
                width2 =UNICODE_WIDTH_DEF/2;
            }
            else
            {
                width2 =UNICODE_WIDTH_DEF;
            }
            
            if(width1>=width2)
            {
                for (i = 0; i < width1; i++)
                {
                    CharPointBuffer[i]|=AuxiliaryBuffer[i];
                    CharPointBuffer[i+width1]|=AuxiliaryBuffer[i+width2];
                }
            }
            else
            {
                for (i = 0; i < width2; i++)
                {
                    AuxiliaryBuffer[i]|=CharPointBuffer[i];
                    AuxiliaryBuffer[i+width2]|=CharPointBuffer[i+width1];
                }
                libc_memcpy(CharPointBuffer,AuxiliaryBuffer,33);
            }
        }
        else
        {
            break;
        }
    }
    
    return ScanLen;
}

#endif
