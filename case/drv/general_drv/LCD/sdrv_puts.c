/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_puts.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint8 scroll_buf[104];

//��ʾ��������ַ���
void Sdrv_PutMultiS(uint8 x, uint8 y, uint32 str_id)
{
    region_t r_str;
    res_langid_entry_t res_langid_entry;
    uint32 str_content_addr;
    uint32 str_len;

    //���������Ļ�����˳�
    if ((x >= Display_Length) || (y >= Display_Height))
    {
        return;
    }

    if (string_entry_offset == 0)
    {
        ui30_file_parse();
    }

    Sdrv_seek_lib(res_lib_fd, SEEK_SET, string_entry_offset + (uint32) (str_id * sizeof(res_langid_entry_t))); // ָ��ָ���ַ����ļ��е�λ��
    Sdrv_read_lib(res_lib_fd, &res_langid_entry, sizeof(res_langid_entry_t));
    str_content_addr = res_langid_entry.dwOffset[1] * 0x10000 + res_langid_entry.dwOffset[0];
    str_len = res_langid_entry.dwLength;
    if (str_len > 100)
    {
        str_len = 100;
    }
    Sdrv_seek_lib(res_lib_fd, SEEK_SET, str_content_addr);
    scroll_buf[0] = 0xff;
    scroll_buf[1] = 0xfe;
    Sdrv_read_lib(res_lib_fd, scroll_buf + 2, str_len);
    if (str_len == 100)
    {
        scroll_buf[102] = 0x00;
        scroll_buf[103] = 0x00;
        str_len += 2; //0x00,0x00
    }
    str_len += 2; //0xff,0xfe

    r_str.x = x;
    r_str.y = y;
    r_str.width = Display_Length - x;
    r_str.height = 16;
    Hdrv_SetWindow(&r_str);
    Sdrv_SetTextPos(x, y);

    Sdrv_PutUnS(scroll_buf, -1);
}

//ȡ��Ļ����
void Sdrv_GetTextPos(uint8 *x,uint8 *y)
{
    *x = region.x;
    *y = region.y;
}

//������Ļ����
void Sdrv_SetTextPos(uint8 x,uint8 y)
{
    if(x<Display_Length)
    {
        region.x=x;
        TextColumn=x;
    }
    if(y<Display_Height)
    {
        TextPage=y;
        region.y=y;
    }
}

//����������ϣ������������
void MakeArabicHebrewLink(uint8 *str, uint8 IsUnicode)
{
    char totallength;
    char cur_length;

    totallength = TurnArabicToUn(str, IsUnicode, -1);

    while(totallength>0)
    {
        cur_length = CopyStr(AuxiliaryBuffer,str,RowWordsCountMax,1);

        libc_memcpy(str, AuxiliaryBuffer, cur_length*2);

        // add by jc0245
        if(cur_length == 0)
        {
            cur_length=1;
        }

        str += cur_length*2;
        totallength -= cur_length;
    }

    str[0] = 0;
    str[1] = 0;

}

// ���ַ���������ʾ���
uint8 CommonScan(uint8 *str, bool IsUnicode, uint32 strlen, bool IsConvert)
{
    uint8 ScanLen=0;
    uint8 ScanLenbk;
    uint16 ucode;
    uint8 result;
    uint8 lineNum_sav,rowNum_sav;//�кż�¼

    lineNum_sav = region.y;
    rowNum_sav = region.x;
    StrHeight = UNICODE_HEIGHT_DEF;

    while (1)
    {
        //��ȡ��ǰ�ַ�����
        ucode = ((IsUnicode != 0)? (*(uint16 *)(str+ScanLen)): *(str+ScanLen));

        if(IsUnicode != 0)
        {
            ucode = (*(uint16 *)(str+ScanLen));
        }
        if((ucode ==0x00)||(strlen==0x00)||(ucode =='\r'))
        {
            break;
        }
        //���д���
        if (ucode == '\n')
        {
            region.x = rowNum_sav;
            region.y += StrHeight;
            TextColumn = region.x;
            TextPage = region.y;
            lineNum_sav = region.y;//���浱ǰ�ж���y������
            StrHeight = UNICODE_HEIGHT_DEF;
            if(IsUnicode != 0)
            {
                ScanLen++;
                ScanLen++;
            }
            else
            {
                ScanLen++;
            }
            (uint8)strlen--;//������ֱ�Ӽ�(��Ϊ0xffʱ��ʾ��ʾȫ��)
            continue;
        }
        ScanLenbk = ScanLen;    //���ݵ�ַ����putcharʧ�ܺ�ָ�
        if(IsUnicode != 0)
        {
            ScanLen ++;
            ScanLen ++;
        }
        else
        {
            //�����ASCII�Ĳ�ͬ����
            //����ж�����Ե�ת��
            //���պ����������ֽڱ�ʾ����,���඼����һ���ֽ�
            if ((ucode >= 0x80) && (MBCodeTwoFlag != 0))
            {
                //ucode = *(uint16 *)(str+ScanLen);  // ���ﲻ����(uint16*)ָ����ʣ����ܵ�ַ�Ƕ���

                ucode = (uint16)( (str[ScanLen+1]<<8) | str[ScanLen] );

                ScanLen++;
                ScanLen++;
            }
            else //ASCII
            {
                ucode = *(str+ScanLen);
                ScanLen++;
            }
            ucode = Sdrv_FontCodeMBToUni(ucode);
            StrHeight = UNICODE_HEIGHT_DEF;//�ѵ�ǰ�еĸ߶���Ϊ���ֵĸ߶�
        }

        //������ַ�
        result = Sdrv_PutChar(str+ScanLen, IsUnicode, ucode);   //str+ScanLenָ������ַ�,���ڴ���̩��ĵ���
        if (result == (uint8)-1)
        {
            return ScanLenbk;
        }
        ScanLen += result;
        strlen--;//������ֱ�Ӽ�(��Ϊ0xffʱ��ʾ��ʾȫ��)
        region.y = lineNum_sav;//�ָ���ǰ�ж�y��������
    }

    return ScanLen;
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutS
//**
//** Description:  ����Ļ�����һ��ANSI�ַ������紮�ĳ��ȳ�����Ļ���������ֲ���ʾ
//**                   ָ��ͣ�����һ������ʾ���ֵ���һ���ַ����Ͻ�,�����س���
//**                   ��βʱ�˳�����������ʱ����һ�п�ʼ��ʾ
//**  input
//**     str: �ַ�����ַ ;
//**     strlen: �ַ����ĳ���
//**
//**  output
//**     none
//*************************************************************************************/
uint8 * Sdrv_PutS(uint8 *str, uint32 strlen)
{
    uint8 *str_bk=str;
    bool IsUnicode = FALSE;
    uint32 len = strlen;
    uint8 ScanLen;
    bool IsSwap = 0;

    if ((*str == 0xff) && (*(str+1) == 0xfe))
    {
        return Sdrv_PutUnS(str, strlen);
    }

    // ���Ӷ԰�������ϣ������Ĵ���
    if(IsArabicOrHebrew != 0)
    {
        // ���ڿռ䣬������һ������ܴ���26���ַ�
        if ((len < 0) || (len > RowWordsCountMax))
        {
            len = RowWordsCountMax;
        }
        libc_memcpy(AuxiliaryBuffer, str, len);
        AuxiliaryBuffer[len] = '\0';
        len = TurnArabicToUn(AuxiliaryBuffer, FALSE, len);
        str = AuxiliaryBuffer;
        IsSwap = 1;
        len = CopyStr(AuxiliaryBuffer,str,len,IsSwap);
        IsUnicode = TRUE;
    }
    ScanLen = CommonScan(str, IsUnicode, len, IsSwap);
    //����ǰ�������ϣ�����򷵻��븨���ַ����ռ�֮��Ӧ��Դ�ַ�����λ��
    return str_bk + ((IsUnicode != 0)? (ScanLen/2): ScanLen);
}
////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutUnS
//**
//** Description:  ����Ļ�����һ��unicode�ַ������紮�ĳ��ȳ�����Ļ���������ֲ���ʾ
//**                   ָ��ͣ�����һ������ʾ���ֵ���һ���ַ����Ͻ�,�����س���
//**                   ��βʱ�˳�����������ʱ����һ�п�ʼ��ʾ
//**  input
//**     str: �ַ�����ַ ;
//**     strlen: �ַ����ĳ���
//**
//**  output
//**     none
//*************************************************************************************/
uint8 * Sdrv_PutUnS(uint8 *str, uint32 strlen)
{
    uint8 *str_bk=str;
    uint32 len = strlen;
    bool IsSwap = 0;
    // ���Ӷ԰�������ϣ������Ĵ���
    if(IsArabicOrHebrew != 0)
    {
        len = CopyStr(AuxiliaryBuffer,str,len,IsSwap);
        str = AuxiliaryBuffer;
        IsSwap = 1;
    }
    //����ǰ�������ϣ�����򷵻��븨���ַ����ռ�֮��Ӧ��Դ�ַ�����λ��
    return str_bk + CommonScan(str, TRUE, strlen,IsSwap);
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutStringExt
//**
//** Description:  ����Ļ�����һ��ANSI�ַ������紮�ĳ��ȳ�����Ļ���������ֲ���ʾ
//**                   ָ��ͣ�����һ������ʾ���ֵ���һ���ַ����Ͻ�,�����س���
//**                   ��βʱ�˳�����������ʱ����һ�п�ʼ��ʾ
//**  input
//**     str: �ַ�����ַ ;
//**     strlen: �ַ����ĳ���
//**        IsUnicode:�Ƿ�Unicode�ַ���
//**     mode:����ģʽ
//**          0--��ͬ��Puts 1--�԰�������ϣ�������Խ���ת��
//**          2--�԰�������ϣ����������� 4--�԰�������ϣ��������ת���������
//**  output
//**     char * ���ش������ĵ�ǰ�ַ�ָ��
//*************************************************************************************/
uint8 * Sdrv_PutStringExt(uint8 *str, uint32 strlen, bool IsUnicode, uint8 mode)
{
    switch(mode)
    {
        #if 0
    case 0x00://��ͬ��puts����putuns
        if(IsUnicode != 0)
        {
            return Sdrv_PutUnS(str, strlen);
        }
        else
        {
            return Sdrv_PutS(str, strlen);
        }
        #endif
    case 0x01://�����԰�������ϣ�����ַ������мӹ�����
        if(IsArabicOrHebrew != 0)
        {
            MakeArabicHebrewLink(str, IsUnicode);
        }
        return str;
    case 0x02://��������ַ���,�ǰ�������ϣ��������µ�ͬ��0x00ģʽ,
        //�ڰ�������ϣ������������û���Ⱦ���0x01����,���ڷ���дģʽ
        return str + CommonScan(str, IsUnicode, strlen,0);
    case 0x04://����0x01ģʽ����0x02ģʽ
        if(IsArabicOrHebrew != 0)
        {
            MakeArabicHebrewLink(str, IsUnicode);
            IsUnicode = TRUE;
        }
        return str + CommonScan(str, IsUnicode, strlen,0);
    default:
        return str;
    }
}

#endif
