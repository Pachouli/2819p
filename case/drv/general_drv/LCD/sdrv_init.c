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
    {"V936GBK.TBL",  /*��������codepage table*/
     "V950BIG.TBL",  /*��������codepage table*/
     "V1252.TBL",      /*Ӣ��,��ŷ��ϵ codepage table*/
     "V932JIS.TBL",   /*���� codepage table*/
     "V949KOR.TBL", /*���� codepage table*/
     "V1257.TBL",      /*���޵غ���ϵ*/
     "V1251.TBL",      /*������ϵ*/
     "V1254.TBL",      /*��������ϵ*/
     "V1255.TBL",      /*ϣ������*/
     "V874.TBL" ,       /*̩��*/
     "V1250.TBL",       /*��ŷ����ŷ��ϵ*/
     "V1256.TBL",      /*��������ϵ*/
     "V1253.TBL"      /*ϣ���� codepage table*/
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

    Sdrv_LanguageSet(0);  //Ĭ����������
    return 1;
}

void Sdrv_Exit(void)
{
    return;
}

bool Sdrv_LanguageSet(uint8 lang_id)
{
    unsigned char table_index = 0;
    //MBCodeTwoFlag������ʶ��ǰ���������Ƿ���2���ֽ�����ʾһ������
    //IsArabicOrHebrew������ʶ��ǰ�����Ƿ���������ϣ�������������Լ����������ط������ж�
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

    //����ȫ�ֱ�����������
    Language_ID = lang_id;

    string_entry_offset = 0;  // ����ID�仯�ˣ���Ҫ���½���
    /*��ʼ��������Ե�Unicode��ת����ľ��*/
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
