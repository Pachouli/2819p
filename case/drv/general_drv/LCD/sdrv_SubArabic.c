/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_SubArabic.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

void ConvertEnglish(uint8 *str, uint16 CharCount)
{
    uint16 Glyph;
    uint16 prevGlyph = 0;
    uint16 nextGlyph = 0;
    uint16 store;
    uint8 start;
    uint8 end;
    uint8 length;
    uint8 i =0 ;
    uint8 *src_ptr, *dst_ptr;

    //���ַ���ת��Ϊ�ֽ���
    length = (BYTE)CharCount + (BYTE)CharCount;

    // 2010-04-16 change by cailizhen from fiona at as211a unicode driver
    // �����ڰ����������������Ե�unicode��ʾ����
    while(((str[i] != 0) || (str[i+1] !=0))&& (i < length))
    {
        bool flag = FALSE;

        src_ptr = str + i;
        Glyph = (uint16)( (src_ptr[1]<<8) | src_ptr[0] );

        if(i < (length - 2))
        {
            src_ptr = str + i + 2;
            nextGlyph = (uint16)( (src_ptr[1]<<8) | src_ptr[0] );
        }
        start = i;

        while((i < length)
         && (Glyph < 0x80)
         && ( ((Glyph >= 0x41)&&(Glyph <= 0x5a))//A-Z
            ||((Glyph >= 0x61)&&(Glyph <= 0x7a))//a-z
            ||((Glyph >= 0x30)&&(Glyph <= 0x39))//0-9
            ||( (Glyph == 0x2e)
              &&(prevGlyph >= 0x30)&&(prevGlyph <= 0x39)
              &&(nextGlyph >= 0x30)&&(nextGlyph <= 0x39)) ) )//1.1
        {
            prevGlyph = Glyph;
            Glyph = nextGlyph;
            i+=2;
            if(i < (length - 2))
            {
                src_ptr = str + i + 2;
                nextGlyph = (uint16)( (src_ptr[1]<<8) | src_ptr[0] );
            }
            else
            {
                nextGlyph = 0;
            }
            flag = TRUE;
        }
        if(flag == TRUE)
        {
            i-=2;
        }

        end =i;
        if(start == end )//||start == (end-2) )
        {
            i+=2;
        }
        else //if(start > (end+2) )
        {
            while(end > start)
            {
                src_ptr = str + end;
                store = (uint16)( (src_ptr[1]<<8) | src_ptr[0] );

                src_ptr = str + start;
                dst_ptr = str + end;

                dst_ptr[0] = src_ptr[0];
                dst_ptr[1] = src_ptr[1];


                dst_ptr = str + start;
                dst_ptr[0] = (uint8)(store & 0xff);
                dst_ptr[1] = (uint8)((store>>8) & 0xff);

                end -=2;
                start +=2;
            }
            i+=2;
        }
    }
}

////*********************************************************************************/
///*
//** FUNCTION:     CopyStr
//**
//** Description:  ����������ϣ�����ַ���ת��Ϊ��д�͵��õ��ַ���
//**  input
//**     src: ԭ�ַ�����ַ
//**     DstBufLength: Ŀ���ַ�������󳤶�
//**     Dst: ���ڱ���ת����Ľ��
//**
//**  output
//**     char ����ת����Ľ������
//*************************************************************************************/
uint8 CopyStr(uint8 *Dst, uint8 *src, uint16 CharCount, bool IsSwap)
{
    uint16 ucode;
    uint8 * cp = Dst;
    uint8 NowCharCount=0;

    //if(CharCount > RowWordsCountMax)
    CharCount = RowWordsCountMax;

    //���Dst������src����ô��src������Dst��
    while(1)
    {
        //ucode = *(uint16*)src;
        ucode = (uint16)( (src[1]<<8) | src[0] );  // ��ֹ�Ƕ������

        if((ucode == 0) || (NowCharCount >= CharCount))
        {
            break;
        }

        cp[0] = src[0];
        cp[1] = src[1];

        cp+=2;
        src+=2;
        NowCharCount ++;
    }

    cp[0] = 0;
    cp[1] = 0;

    //���а�������ϣ���������ֺ�Ӣ����ĸ����
    ConvertEnglish(Dst, NowCharCount);
    //�԰�����������д����
    if(Language_ID == ARABIC)
    {
        // ���س��ȿ��ܺʹ���ȥ�ĳ��Ȳ�һ��
        NowCharCount = UnicodeTOArabic(Dst, NowCharCount);
    }

    return NowCharCount;
}

#endif
