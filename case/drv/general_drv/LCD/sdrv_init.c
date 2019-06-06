/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_init.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

void *FontLib_lib_fd;
void *FontAscLib_lib_fd;
void *MBtoUnicTab_lib_fd = NULL;
uint8 g_table_index;

const uint8 FontLibName[] ="UNICODE.BIN";
const uint8 FontAscLibName[]="ASCII.bin";
const uint8 MBtoUnicTabName[][13]=
    {"V936GBK.TBL",  /*简体中文codepage table*/
     "V950BIG.TBL",  /*繁体中文codepage table*/
     "V1252.TBL",      /*英文,西欧语系 codepage table*/
     "V932JIS.TBL",   /*日文 codepage table*/
     "V949KOR.TBL", /*韩文 codepage table*/
     "V1257.TBL",      /*波罗地海语系*/
     "V1251.TBL",      /*俄语语系*/
     "V1254.TBL",      /*土耳其语系*/
     "V1255.TBL",      /*希伯莱语*/
     "V874.TBL" ,       /*泰语*/
     "V1250.TBL",       /*北欧，东欧语系*/
     "V1256.TBL",      /*阿拉伯语系*/
     "V1253.TBL"      /*希腊文 codepage table*/
    };

bool Sdrv_LanguageSet(uint8 lang_id);
bool Sdrv_Init(uint8 mode)
{
    FontLib_lib_fd = Sdrv_open_lib(FontLibName);
    if (FontLib_lib_fd == NULL)
    {
        libc_printf_error("can't find %s!\n", FontLibName);
        while (1);
    }
    FontAscLib_lib_fd = Sdrv_open_lib(FontAscLibName);
    if (FontAscLib_lib_fd == NULL)
    {
        libc_printf_error("can't find %s!\n", FontAscLibName);
        while (1);
    }

    Sdrv_LanguageSet(0);  //默认语言设置
    return 1;
}

void Sdrv_Exit(void)
{
    return;
}

bool Sdrv_LanguageSet(uint8 lang_id)
{
    unsigned char table_index = 0;
    //MBCodeTwoFlag用来标识当前语言类型是否用2个字节来表示一个内码
    //IsArabicOrHebrew用来标识当前语言是否阿拉伯或者希伯莱，这样可以减少在其他地方进行判断
    MBCodeTwoFlag = 0;
    IsArabicOrHebrew = 0;

    switch(lang_id)
    {
    case  CHINESE_SIMPLIFIED:
        table_index = 0;
        MBCodeTwoFlag = 1;

        break;

    case   CHINESE_TRADITIONAL:
        table_index = 1;
        MBCodeTwoFlag = 1;
        break;

    case  ENGLISH:
    case  FRENCH:
    case  GERMAN:
    case  ITALIAN:
    case  DUTCH:
    case  PORTUGUESE:
    case  SPANISH:
    case  SWEDISH:
    case  INDONESIAN:
    case  DANISH:
        table_index = 2;
        break;

    case   JAPANESE:
        table_index = 3;
        MBCodeTwoFlag = 1;
        break;

    case   KOREAN:
        table_index = 4;
        MBCodeTwoFlag = 1;
        break;

    case  CZECH:
    case  POLISH:
        table_index = 5;
        break;

    case  RUSSIAN:
        table_index = 6;
        break;

    case  TURKISH:
        table_index = 7;
        break;

    case  HEBREW:
        table_index = 8;
        IsArabicOrHebrew = 1;
        break;

    case  THAI:
        table_index = 9;
        break;

    case  HUNGARIAN:
    case  ROMANIAN:
        //case  CROATIA:
        //case  SLOVENIAN:
        table_index = 10;
        break;

    case  ARABIC:
        table_index = 11;
        IsArabicOrHebrew = 1;
        break;
    case GREEK:
        table_index = 12;
        break;

    default:
        table_index = 0;
        break;
    }

    //设置全局变量语言类型
    Language_ID = lang_id;

    string_entry_offset = 0;  // 语言ID变化了，需要重新解析
    /*初始化多国语言到Unicode的转换表的句柄*/
    g_table_index = table_index;

    if(MBtoUnicTab_lib_fd != NULL)
    {
        libc_printf("#MBtoUnicTab_lib_fd != NULL\r\n");
        Sdrv_close_lib(MBtoUnicTab_lib_fd);
        MBtoUnicTab_lib_fd = NULL;
    }

    MBtoUnicTab_lib_fd = Sdrv_open_lib(MBtoUnicTabName[table_index]);

    if (MBtoUnicTab_lib_fd == NULL)
    {
        libc_printf_error("can't find %s!\n", MBtoUnicTabName[table_index]);
        while (1);
    }

    return TRUE;
}

#endif
