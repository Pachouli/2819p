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

//显示多国语言字符串
void Sdrv_PutMultiS(uint8 x, uint8 y, uint32 str_id)
{
    region_t r_str;
    res_langid_entry_t res_langid_entry;
    uint32 str_content_addr;
    uint32 str_len;

    //坐标出了屏幕，则退出
    if ((x >= Display_Length) || (y >= Display_Height))
    {
        return;
    }

    if (string_entry_offset == 0)
    {
        ui30_file_parse();
    }

    Sdrv_seek_lib(res_lib_fd, SEEK_SET, string_entry_offset + (uint32) (str_id * sizeof(res_langid_entry_t))); // 指针指向字符在文件中的位置
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

//取屏幕坐标
void Sdrv_GetTextPos(uint8 *x,uint8 *y)
{
    *x = region.x;
    *y = region.y;
}

//设置屏幕坐标
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

//处理阿拉伯和希伯莱的特殊性
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

// 对字符串进行显示输出
uint8 CommonScan(uint8 *str, bool IsUnicode, uint32 strlen, bool IsConvert)
{
    uint8 ScanLen=0;
    uint8 ScanLenbk;
    uint16 ucode;
    uint8 result;
    uint8 lineNum_sav,rowNum_sav;//行号记录

    lineNum_sav = region.y;
    rowNum_sav = region.x;
    StrHeight = UNICODE_HEIGHT_DEF;

    while (1)
    {
        //获取当前字符编码
        ucode = ((IsUnicode != 0)? (*(uint16 *)(str+ScanLen)): *(str+ScanLen));

        if(IsUnicode != 0)
        {
            ucode = (*(uint16 *)(str+ScanLen));
        }
        if((ucode ==0x00)||(strlen==0x00)||(ucode =='\r'))
        {
            break;
        }
        //换行处理
        if (ucode == '\n')
        {
            region.x = rowNum_sav;
            region.y += StrHeight;
            TextColumn = region.x;
            TextPage = region.y;
            lineNum_sav = region.y;//保存当前行顶的y轴坐标
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
            (uint8)strlen--;//不理负号直接减(因为0xff时表示显示全部)
            continue;
        }
        ScanLenbk = ScanLen;    //备份地址以在putchar失败后恢复
        if(IsUnicode != 0)
        {
            ScanLen ++;
            ScanLen ++;
        }
        else
        {
            //内码和ASCII的不同处理
            //需进行多国语言的转换
            //中日韩是用两个字节表示内码,其余都是用一个字节
            if ((ucode >= 0x80) && (MBCodeTwoFlag != 0))
            {
                //ucode = *(uint16 *)(str+ScanLen);  // 这里不能用(uint16*)指针访问，可能地址非对齐

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
            StrHeight = UNICODE_HEIGHT_DEF;//把当前行的高度设为汉字的高度
        }

        //输出该字符
        result = Sdrv_PutChar(str+ScanLen, IsUnicode, ucode);   //str+ScanLen指向后续字符,用于处理泰语的叠加
        if (result == (uint8)-1)
        {
            return ScanLenbk;
        }
        ScanLen += result;
        strlen--;//不理负号直接减(因为0xff时表示显示全部)
        region.y = lineNum_sav;//恢复当前行顶y的轴坐标
    }

    return ScanLen;
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutS
//**
//** Description:  在屏幕上输出一个ANSI字符串，如串的长度超过屏幕，则多出部分不显示
//**                   指针停在最后一个能显示的字的下一个字符左上角,碰到回车或
//**                   串尾时退出，碰到换行时从下一行开始显示
//**  input
//**     str: 字符串首址 ;
//**     strlen: 字符串的长度
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

    // 增加对阿拉伯和希伯莱语的处理
    if(IsArabicOrHebrew != 0)
    {
        // 限于空间，阿拉伯一次最多能处理26个字符
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
    //如果是阿拉伯和希伯来则返回与辅助字符串空间之对应的源字符串的位置
    return str_bk + ((IsUnicode != 0)? (ScanLen/2): ScanLen);
}
////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutUnS
//**
//** Description:  在屏幕上输出一个unicode字符串，如串的长度超过屏幕，则多出部分不显示
//**                   指针停在最后一个能显示的字的下一个字符左上角,碰到回车或
//**                   串尾时退出，碰到换行时从下一行开始显示
//**  input
//**     str: 字符串首址 ;
//**     strlen: 字符串的长度
//**
//**  output
//**     none
//*************************************************************************************/
uint8 * Sdrv_PutUnS(uint8 *str, uint32 strlen)
{
    uint8 *str_bk=str;
    uint32 len = strlen;
    bool IsSwap = 0;
    // 增加对阿拉伯和希伯莱语的处理
    if(IsArabicOrHebrew != 0)
    {
        len = CopyStr(AuxiliaryBuffer,str,len,IsSwap);
        str = AuxiliaryBuffer;
        IsSwap = 1;
    }
    //如果是阿拉伯和希伯来则返回与辅助字符串空间之对应的源字符串的位置
    return str_bk + CommonScan(str, TRUE, strlen,IsSwap);
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutStringExt
//**
//** Description:  在屏幕上输出一个ANSI字符串，如串的长度超过屏幕，则多出部分不显示
//**                   指针停在最后一个能显示的字的下一个字符左上角,碰到回车或
//**                   串尾时退出，碰到换行时从下一行开始显示
//**  input
//**     str: 字符串首址 ;
//**     strlen: 字符串的长度
//**        IsUnicode:是否Unicode字符串
//**     mode:处理模式
//**          0--等同于Puts 1--对阿拉伯和希伯莱语言进行转换
//**          2--对阿拉伯和希伯莱进行输出 4--对阿拉伯和希伯莱进行转换并且输出
//**  output
//**     char * 返回处理完后的当前字符指针
//*************************************************************************************/
uint8 * Sdrv_PutStringExt(uint8 *str, uint32 strlen, bool IsUnicode, uint8 mode)
{
    switch(mode)
    {
        #if 0
    case 0x00://等同于puts或者putuns
        if(IsUnicode != 0)
        {
            return Sdrv_PutUnS(str, strlen);
        }
        else
        {
            return Sdrv_PutS(str, strlen);
        }
        #endif
    case 0x01://仅仅对阿拉伯和希伯莱字符串进行加工处理
        if(IsArabicOrHebrew != 0)
        {
            MakeArabicHebrewLink(str, IsUnicode);
        }
        return str;
    case 0x02://仅仅输出字符串,非阿拉伯和希伯莱情况下等同于0x00模式,
        //在阿拉伯和希伯莱情况下如果没有先经过0x01处理,等于非连写模式
        return str + CommonScan(str, IsUnicode, strlen,0);
    case 0x04://等于0x01模式加上0x02模式
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
