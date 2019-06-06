/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_getfontpoint.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint16 gbk_to_unicode(uint16 gbk_code)
{
    uint16 unicode_val;
    uint32 diff;
    uint32 index;

    if ((gbk_code&0xff) <= 0x7f)
    {
        unicode_val = gbk_code&0xff;
    }
    else
    {
        if ((gbk_code&0xff) == 0x80)
        {
            unicode_val = 0x20ac;
        }
        else if ((gbk_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f5;
        }
        else
        {
            if (gbk_code < 0x100)
            {
                unicode_val = 0;
            }
            else if (gbk_code < 0x4080)
            {
                unicode_val = 0x3f;
            }
            else
            {
                diff = gbk_code - 0x4080;
                index = diff/0x100*0x80;
                index += (diff%0x80);

                Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, index*2);
                Sdrv_read_lib(MBtoUnicTab_lib_fd, &unicode_val, 2);
            }
        }
    }

    return unicode_val;
}

uint16 big5_to_unicode(uint16 big5_code)
{
    uint16 unicode_val;
    uint32 diff;
    uint32 index;

    if ((big5_code&0xff) <= 0x7f)
    {
        unicode_val = big5_code&0xff;
    }
    else
    {
        if ((big5_code&0xff) == 0x80)
        {
            unicode_val = 0x80;
        }
        else if ((big5_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f8;
        }
        else
        {
            if (big5_code < 0x100)
            {
                unicode_val = 0;
            }
            else if ((big5_code < 0x4080) || ((big5_code >= 0x7f00)&&(big5_code < 0xa180)))
            {
                unicode_val = 0x3f;
            }
            else if (big5_code < 0x7f00)
            {
                diff = big5_code - 0x4080;
                index = diff/0x100*0x80;
                index += (diff%0x80);

                Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, index*2);
                Sdrv_read_lib(MBtoUnicTab_lib_fd, &unicode_val, 2);
            }
            else
            {
                diff = big5_code - 0x6280;
                index = diff/0x100*0x80;
                index += (diff%0x80);

                Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, index*2);
                Sdrv_read_lib(MBtoUnicTab_lib_fd, &unicode_val, 2);
            }
        }
    }

    return unicode_val;
}

uint16 jis_to_unicode(uint16 jis_code)
{
    uint16 unicode_val;
    uint32 diff;
    uint32 index;

    if ((jis_code&0xff) <= 0x7f)
    {
        unicode_val = jis_code&0xff;
    }
    else
    {
        if ((jis_code&0xff) == 0x80)
        {
            unicode_val = 0x80;
        }
        else if ((jis_code&0xff) == 0xa0)
        {
            unicode_val = 0xf8f0;
        }
        else if (((jis_code&0xff) >= 0xa1) && ((jis_code&0xff) < 0xe0))
        {
            unicode_val = 0xff61 + ((jis_code&0xff)-0xa1);
        }
        else if ((jis_code&0xff) == 0xfd)
        {
            unicode_val = 0xf8f1;
        }
        else if ((jis_code&0xff) == 0xfe)
        {
            unicode_val = 0xf8f2;
        }
        else if ((jis_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f3;
        }
        else if (jis_code < 0x100)
        {
            unicode_val = 0;
        }
        else if (jis_code < 0x4080)
        {
            unicode_val = 0x30fb;
        }
        else
        {
            diff = jis_code - 0x4080;
            index = diff/0x100*0x40;
            if ((diff%0x80) < 0x20)
            {
                index += (diff%0x80);
            }
            else
            {
                index += (diff%0x80-0x40);
            }

            Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, index*2);
            Sdrv_read_lib(MBtoUnicTab_lib_fd, &unicode_val, 2);
        }
    }

    return unicode_val;
}

uint16 kor_to_unicode(uint16 kor_code)
{
    uint16 unicode_val;
    uint32 diff;
    uint32 index;

    if ((kor_code&0xff) <= 0x7f)
    {
        unicode_val = kor_code&0xff;
    }
    else
    {
        if ((kor_code&0xff) == 0x80)
        {
            unicode_val = 0x80;
        }
        else if ((kor_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f7;
        }
        else
        {
            if (kor_code < 0x100)
            {
                unicode_val = 0;
            }
            else if (kor_code < 0x4180)
            {
                unicode_val = 0x3f;
            }
            else
            {
                diff = kor_code - 0x4180;
                index = diff/0x100*0x80;
                index += (diff%0x80);

                Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, index*2);
                Sdrv_read_lib(MBtoUnicTab_lib_fd, &unicode_val, 2);
            }
        }
    }

    return unicode_val;
}

uint16 thai_to_unicode(uint16 thai)
{
    uint16 unicode_val;
    uint32 diff;
    uint32 index;

    if ((thai&0xff) <= 0x7f)
    {
        unicode_val = thai&0xff;
    }
    else
    {
        if ((thai&0xff) == 0xff)
        {
            unicode_val = 0xf8c8;
        }
        else if ((thai&0xff) == 0xfe)
        {
            unicode_val = 0xf8c7;
        }
        else if ((thai&0xff) == 0xfd)
        {
            unicode_val = 0xf8c6;
        }
        else if ((thai&0xff) == 0xfc)
        {
            unicode_val = 0xf8c5;
        }
        else if ((thai&0xff) == 0xde)
        {
            unicode_val = 0xf8c4;
        }
        else if ((thai&0xff) == 0xdd)
        {
            unicode_val = 0xf8c3;
        }
        else if ((thai&0xff) == 0xdc)
        {
            unicode_val = 0xf8c2;
        }
        else if ((thai&0xff) == 0xdb)
        {
            unicode_val = 0xf8c1;
        }
        else if ((thai&0xff) == 0xa0)
        {
            unicode_val = 0xa0;
        }
        else if (((thai&0xff) >= 0xa1) && ((thai&0xff) <= 0xda))
        {
            unicode_val = 0xe01 + ((thai&0xff)-0xa1);
        }
        else if ((thai&0xff) >= 0xdf)
        {
            unicode_val = 0xe3f + ((thai&0xff)-0xdf);
        }
        else
        {
            diff = thai - 0x80;
            index = diff/0x100*0x20;
            index += (diff%0x80);

            Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, index*2);
            Sdrv_read_lib(MBtoUnicTab_lib_fd, &unicode_val, 2);
        }
    }

    return unicode_val;
}

void Sdrv_GetUniCode(uint16 mbcode, uint16 *ucode)
{
    if (g_table_index == 0)
    {
        *ucode = gbk_to_unicode(mbcode);
    }
    else if (g_table_index == 1)
    {
        *ucode = big5_to_unicode(mbcode);
    }
    else if (g_table_index == 3)
    {
        *ucode = jis_to_unicode(mbcode);
    }
    else if (g_table_index == 4)
    {
        *ucode = kor_to_unicode(mbcode);
    }
    else if (g_table_index == 9)
    {
        *ucode = thai_to_unicode(mbcode);
    }
    else
    {
        Sdrv_seek_lib(MBtoUnicTab_lib_fd, SEEK_SET, (uint32) mbcode*2);
        Sdrv_read_lib(MBtoUnicTab_lib_fd, ucode, 2);
    }
}

#endif
