/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_getstringlengthExt.c
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
 
** FUNCTION:    Sdrv_GetStringLength
**
** PURPOSE:     Get Char Point Info by dma2
**
** PARAMETERS:
**  input
**      str=�ַ����׵�ַ
**      str_length=�ַ�������
**      str_type,  0, char, 1,unicode
**  output
**      unsigned char = �ַ����
** DESCRIPTION:
****************************************************************************
*/
bool Sdrv_GetStringLengthExt(uint8 *str,uint16 CharCount,uint8 str_type, uint8 * ResCharCount, uint8 * ResColCount)
{
    uint8  StrCnt ;
    uint8 cur_length;
    uint16  ucode;
    uint8 StrWidth = 0;
/// uint8 ThaiScanLen;
    uint8 WordsCount = 0;
    uint8 Words_bk;
    uint8 Col = 0;
    uint8 Col_bk;
    uint8 *str_bk = str;
    uint8 str_type_bk = str_type;
/// uint8 result;

    if(CharCount > RowWordsCountMax)
    {
        cur_length = RowWordsCountMax;
    }
    else
    {
        cur_length = (uint8) CharCount;
    }
    if(Language_ID == ARABIC)
    {
        if(str_type==0)
        {
            //ֻ����ǰ��26���ַ����㹻����һ������LCM��ȵ���ֵ���Ա�ʾ���ַ������ȴ���LCM���
            libc_memcpy(AuxiliaryBuffer, str, cur_length);
            AuxiliaryBuffer[cur_length] = '\0';
            cur_length = TurnArabicToUn(AuxiliaryBuffer, FALSE, cur_length);
            str = AuxiliaryBuffer;
        }
        cur_length = CopyStr(AuxiliaryBuffer,str,cur_length,1);
        str = AuxiliaryBuffer;
        str_type=1;
    }
    
    if(str_type == 0)
    {
        for(StrCnt = 0; StrCnt < cur_length; StrCnt ++)
        {
            if((*str == '\r') ||  (*str == '\n') )
            {
                break;
            }

            Col_bk = Col;
            Words_bk = WordsCount;
            if ((*str >= 0x80) && (MBCodeTwoFlag != 0))
            {
                //ucode = *(uint16 *) str;
                ucode = *(str+1);
                ucode <<= 8;
                ucode += *str;
                str += 2;
                StrCnt++; //��Unicodeģʽstr_length���ֽ�����������˫�ֽ��ַ�����Ҫ�����ټ�1
                Col  += 2;
                WordsCount ++;
            }
            else
            {//ASCII
                ucode = *str;
                str++;
                Col ++;
                WordsCount ++;
            }
            ucode = Sdrv_FontCodeMBToUni(ucode);
            StrHeight = UNICODE_HEIGHT_DEF;//�ѵ�ǰ�еĸ߶���Ϊ���ֵĸ߶�
            
            Sdrv_GetFontPoint(ucode, CharPointBuffer);//ȡ����
            if((AsciiSelect==0)&&((uint16)ucode<0x0080))
            {
                CharPointBuffer[32]=0x06;
            }
            else if((ucode==0x0E31) || ((ucode<=0x0E3A)&&(ucode>=0x0E34)) || ((ucode<=0x0E4E)&&(ucode>=0x0E47)))
            {
                continue;
            } 
            else
            {
                //for QAC
            }         
            StrWidth += CharPointBuffer[32];
            if((StrWidth > 128) || (WordsCount > RowWordsCountMax))
            {
                Col = Col_bk;
                WordsCount = Words_bk;
                break;
            }
        }   /*end of  for*/
    } /*end of if*/
    else
    {
        for(StrCnt = 0; StrCnt < cur_length; StrCnt++)
        {
            Col_bk = Col;
            Words_bk = WordsCount;
            //ucode = *(uint16 *) str;
            ucode = *(str+1);
            ucode <<= 8;
            ucode += *str;
            if((ucode == 0x000D) || (ucode == 0x000A) )
            {
                break;
            }
            else
            {
                str += 2;
                Col  += 2;
                WordsCount ++;
            }
            Sdrv_GetFontPoint(ucode, CharPointBuffer);//ȡ����
            if((AsciiSelect==0)&&((uint16)ucode<0x0080))
            {
                CharPointBuffer[32]=0x06;
            }
            else if((ucode==0x0E31) || ((ucode<=0x0E3A)&&(ucode>=0x0E34)) || ((ucode<=0x0E4E)&&(ucode>=0x0E47)))
            {
                continue;
            }
            else
            {
                //for QAC
            }
            StrWidth += CharPointBuffer[32];

            if((StrWidth > 128) || (WordsCount > RowWordsCountMax))
            {
                Col = Col_bk;
                WordsCount = Words_bk;
                break;
            }
        } /*end of for*/
    }   /*end of else*/

    // ���ؽ������ʹȱҳҲ���أ���Ϊ����ǵ����ļ�������������ļ�ĩβ����
    * ResCharCount = WordsCount;
    * ResColCount = Col;

    //���������⴦��
    if(Language_ID == ARABIC)
    {
        //�����жϰ����������Ƿ�ȱҳ
        if((CharCount <= RowWordsCountMax) && (StrCnt >= cur_length))
        {
            return TRUE;
        }
        else if(StrCnt >= cur_length)
        {
            return FALSE;
        }
        else
        {
            //for QAC
        }

        //�е��������ַ����Ĳ�����β��
        cur_length = WordsCount;
        if(str_type_bk==0)
        {
            //ֻ����ǰ��26���ַ����㹻����һ������LCM��ȵ���ֵ���Ա�ʾ���ַ������ȴ���LCM���
            libc_memcpy(AuxiliaryBuffer, str_bk, cur_length);
            AuxiliaryBuffer[cur_length] = '\0';
            cur_length = TurnArabicToUn(AuxiliaryBuffer, FALSE, cur_length);
            str_bk = AuxiliaryBuffer;
        }
        *ResCharCount = WordsCount = CopyStr(AuxiliaryBuffer,str_bk,cur_length,1);
        if(str_type_bk == 0)
        {
            *ResColCount = WordsCount;
        }
        else
        {
            *ResColCount = WordsCount + WordsCount;
        }

        return FALSE;
    }

    if((int)StrCnt >= CharCount)
    {
        return TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif
