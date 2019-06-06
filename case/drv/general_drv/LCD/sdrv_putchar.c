/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_putchar.c
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
*
** FUNCTION:     CheckDisplaySpace
**
** Description:  �����Ļ�ռ��Ƿ�������ʾҪ���糬����Χ�򱨴�
**
**  input
**     codemod: �����ģʽ���ַ������ͼƬ����
**
**  output
**     True: ��Ļ�ռ���������ʾҪ��
**     False: ��Ļ�ռ䲻��������ʾҪ��
********************************************************************************
*/
bool CheckDisplaySpace(uint8 codemod,uint8 fontwidth)
{
    uint8 justwidth;

    switch(codemod)
    {
    case UnicodeMode:
        justwidth = fontwidth;
        if( (StrEndX != 0) && ( (region.x+justwidth) > StrEndX))
        {
//          DUMP(RESULT_PUTCHAR_X_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X�ᳬ��
            return FALSE;
        }
        goto NextStep;
    case PicDisMode:
        justwidth = region.width;
        NextStep:
        //�ж��Ƿ�X���y�ᳬ��
        if ((region.x+justwidth) > (region_win.x+region_win.width))
        {
//          DUMP(RESULT_PUTCHAR_X_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X�ᳬ��
            return FALSE;
        }
        if ((region.y+region.height) > (region_win.y+region_win.height))
        {
//          DUMP(RESULT_PUTCHAR_Y_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_Y_ERR;//Y�ᳬ��
            return FALSE;
        }
        break;
    default:
//      DUMP(RESULT_PUTCHAR_OTHER_ERR);
        //PutCharErrFlag = RESULT_PUTCHAR_OTHER_ERR;//��֪���͵�Ҳ����ʾ
        return FALSE;
    }

    return TRUE;
}

//char CheckCharMode(int char_code);
//bool CheckDisplaySpace(char codemod);
////*******************************************************************************/
///*
//** FUNCTION:     PutChar
//**
//** Description:  ����Ļ�ϴ��һ���ַ�,�������ǰָ���Զ�������һ�����ֵ����Ͻ�
//**
//**  input
//**     code: �ַ�����ֵ
//**     str: ����str��Ϊ���ܹ����ʺ������ַ�����֧��̩������⴦��
//**     IsUnicode: ��ʾstr�����ͣ��Ƿ�Unicode����
//**  output
//**     signed char:-1��ʾԽ�磬0��ʾ����������0��ʾ̩���ص��˺�����ַ���
//********************************************************************************/

uint8 Sdrv_PutChar(uint8 *str, bool IsUnicode, uint16 ucode)
{
    uint8 charmode,fontwidth;
    bool putchar_result;
    uint8 ScanLen=0;
    uint8 CharWidth;
    uint16 CharPointLen;

    if ((IsUnicode != 0) && (ucode == 0xfeff))
    {
        return 0;
    }

    //PutCharErrFlag = RESULT_PUTCHAR_OK; //Ĭ��ΪOK
    Sdrv_GetFontPoint(ucode, CharPointBuffer);//ȡ����

    //̩�����⴦��
    if((ucode>=0x0e01)&&(ucode<=0x0e7F))
    {
        ScanLen = Sdrv_ThaiOverlap(str, IsUnicode);
    }

    //Hdrv_SetDrawMode(DrawModeHPic_def);    //����ʾģʽ���ó�������ʾģʽ
    charmode = UnicodeMode;
    //�����������Ƿ񳬹���ʾ���ֱ���
    if((AsciiSelect==0)&&((uint16)ucode<0x0080))
    {
        CharPointBuffer[32] = 6;
    }
    CharWidth = CharPointBuffer[32];

    if (CharPointBuffer[32] > 8)
    {
        fontwidth = UNICODE_WIDTH_DEF;
    }
    else
    {
        fontwidth = UNICODE_WIDTH_DEF/2;
    }
    if ((AsciiSelect==0)&&((WORD)ucode<0x0080))
    {
        region.height = UNICODE_HEIGHT_DEF/2;
        CharPointLen = CharWidth;
    }
    else
    {
        region.height = UNICODE_HEIGHT_DEF;
        CharPointLen = CharWidth*2;
        if (CharWidth != 0x10)
        {
            libc_memcpy(CharPointBuffer + CharWidth, CharPointBuffer + fontwidth, CharWidth);
        }
    }

    putchar_result = CheckDisplaySpace(charmode, CharWidth);
    if (putchar_result == FALSE)
    {
        //Hdrv_SetDrawMode(DrawModeH_def);    //����ʾģʽ���óɺ�����ʾģʽ
        return -1;//����ʾ��Χ���ԣ�������ʾ������ֱ���˳�
    }

    //������ģ����Ĵ�С
    region.width = CharWidth;
    Hdrv_SetWindow(&region);
    //����ǰ�������ϣ������,���Ƚ��������ӳ�䴦��,Ȼ�������buffer�����ӳ����� by cailizhen
    RegionMapping(&region, CharWidth);
    //����ʱ�Դ����������͵�LCM
    Hdrv_BuffDataTrans(CharPointBuffer, CharPointLen);
    RegionMapping(&region, CharWidth);
    region.x += CharWidth;//�����Ƶ���һ���ֵ����Ͻ�
    //TextColumn += CharPointBuffer[32];
    //Hdrv_SetDrawMode(DrawModeH_def);    //����ʾģʽ���óɺ�����ʾģ
    return ScanLen;
}

#endif
