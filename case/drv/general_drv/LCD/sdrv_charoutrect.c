/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_charoutrect.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

////*******************************************************************************/
///*
//** FUNCTION:     Sdrv_CharOutRect
//**
//** Description:  ��ָ���������ϴ��һ���ַ�,�������򲿷ֲ���ʾ��û�г�����Ļ�Ĳ��ֽ�������ʾ��
//**
//**  input
//**     str: ��ǰ�ַ�ָ�룬�Է��ʺ����ַ���֧��̩����ʾ
//**     CRregion: Ҫ�����������
//**     colcnt:   Ҫ��Ӻ��ֵ���һ�п�ʼ���
//**     language: �Ƿ�Unicode����
//**  output
//**     char: ��ָ�������������ֵ�������ʱ������Ҫ���Ƶ��ַ��������򷵻�0
//********************************************************************************/

uint8 Sdrv_CharOutRect(uint8 *str, region_t *CRregion, uint8 colcnt, uint8 language)
{
//  uint8 charmode;
//  bool putchar_result;
    uint8 fontwidth;
    uint8 ScanLen;
    uint16 ucode;
//  uint16 ThaiCode;
    uint16 i,k;
    uint8 IsUnicode = ((language == UNICODELANGUAGE) || (language == (uint8)(ARABICHASJOIN)));
    uint16 CharPointLen;

    // �����Ƿ���Ҫ�ж�str ?

    if(IsUnicode != 0)
    {
        ucode = *(uint16 *)str;
        ScanLen = 2;
    }
    else
    {
        //���ֺ�ASCII�Ĳ�ͬ����
        if((*str >= 0x80) && (MBCodeTwoFlag != 0))
        {
            //ucode = *(uint16 *)str;  // ������(uint16*)ָ����ʣ����ܵ�ַ�Ƕ���

            ucode = (uint16)( (str[1]<<8) | str[0] );

            ScanLen = 2;
        }
        else
        {
            ucode = *str;
            ScanLen =1;
        }
        ucode = Sdrv_FontCodeMBToUni(ucode);
    }

    //PutCharErrFlag = RESULT_PUTCHAR_OK; //Ĭ��ΪOK
    //Hdrv_SetDrawMode(DrawModeHPic_def);    //����ʾģʽ���ó�������ʾģʽ������ֿ���ݣ�

    Sdrv_GetFontPoint(ucode, CharPointBuffer); //ȡ����

    //��̩�����⴦��
    if((ucode>=0x0e01)&&(ucode<=0x0e7F))
    {
        ScanLen += Sdrv_ThaiOverlap(str +ScanLen, IsUnicode);
    }


    // ����Ƿ��Ѿ������� ,  �ַ��������õ���by jc0245
    if(IsUnicode != 0)
    {
        uint16 tmp_word;

        tmp_word = (uint16) ( (str[ScanLen+1] << 8) | str[ScanLen] );

        if(tmp_word == 0)
        {
            g_is_str_end = 1;
        }
        else
        {
            g_is_str_end = 0;
        }
    }
    else
    {
        if(str[ScanLen] == 0)
        {
            g_is_str_end = 1;
        }
        else
        {
            g_is_str_end = 0;
        }
    }

    g_last_scan_len = ScanLen;  // ����Ҫ��ʾ�ַ������ֽ���


    if (CRregion->width == 0)
    {
        return ScanLen;
    }

    if((AsciiSelect==0)&&((uint16)ucode<0x0080))
    {
        CharPointBuffer[32]=0x06;
    }

    // add by jc0245
    g_last_char_width = CharPointBuffer[32];  // ���浱ǰ��ʾ�ַ����

    if(colcnt >= CharPointBuffer[32] )                 //���������ȷ���
    {
        //Hdrv_SetDrawMode(DrawModeH_def);        //����ʾģʽ���óɺ�����ʾģ
        return ScanLen;                        //ָ����һ������
    }
    //���ַ���ȴ���8ʱ��ת��16X16�ĵ�����Ϣ
    if(CharPointBuffer[32] > 8)
    {
        fontwidth = UNICODE_WIDTH_DEF;
    }
    //���ַ���ȵ���С��8ʱ��ֻת��8X16�ĵ�����Ϣ
    else
    {
        fontwidth = UNICODE_WIDTH_DEF/2;
    }

    //ȷ���������
    region.x = CRregion->x;
    region.y = CRregion->y;
    TextPage = CRregion->y;
    region.width = (CRregion->width < (fontwidth-colcnt))? (CRregion->width):(fontwidth-colcnt);
    region.height = UNICODE_HEIGHT_DEF;
    CharPointLen = region.width*2;

    if((AsciiSelect==0)&&((uint16)ucode<0x80))
    {
        region.height = UNICODE_HEIGHT_DEF/2;
        CharPointLen = region.width;
    }
    for(k=0;k<(region.height/8);k++)
    {
        for(i=0;i<region.width;i++)
        {
            // �԰�������ϣ��������ƽ���ٶԻ��Ĳ��� by cailizhen 2010-06-28
            // ����������ȥ��common�еĹ����������⴦��
            uint8 start = CharPointBuffer[32]- 1 - colcnt;
            uint8 temp = (start * 2) - region.width + 1;
            if(IsArabicOrHebrew != 0)
            {
                CharPointBuffer[i+k*fontwidth]=
                CharPointBuffer[temp - ((CharPointBuffer[32]- 1) - (colcnt+i)) + (k*fontwidth)];
            }
            else
            {
                CharPointBuffer[i+k*fontwidth]=CharPointBuffer[colcnt+i+(k*fontwidth)];
            }
        }
    }
    if(IsArabicOrHebrew != 0)
    {

    }
    else
    {
        if (region.width != 0x10)
        {
            libc_memcpy(CharPointBuffer + region.width, CharPointBuffer + fontwidth, region.width);
        }
    }

    Hdrv_SetWindow(&region);
    //BlackCharToColor(fontwidth, CharPointBuffer);
    CharPointBuffer[32]=region.width;
    if(TextColumn>Display_Length)
    {
        TextColumn=CRregion->x;
    }
    if(TextColumn >= (CRregion->x+CRregion->width))
    {
        TextColumn-=region.width;
    }
    else
    {
        TextColumn = CRregion->x;
    }
    //����ʱ�Դ����������͵�LCM
    //����ǰ�������ϣ������,���Ƚ��������ӳ�䴦��,Ȼ�������buffer�����ӳ����� by cailizhen
    RegionMapping(&region, CharPointBuffer[32]);
    Hdrv_BuffDataTrans(CharPointBuffer, CharPointLen);
    RegionMapping(&region, CharPointBuffer[32]);

    //Hdrv_SetDrawMode(DrawModeH_def);    //����ʾģʽ���óɺ�����ʾģ
    return 0;
}

#endif
