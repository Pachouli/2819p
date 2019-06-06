
/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：字符串处理
 * 作者：zhouxl
 ********************************************************************************/
#ifndef  __CHAR_CONVERT_H
#define  __CHAR_CONVERT_H

#include "ap_test_head.h"

static int32 byte_to_unicode(uint8 byte_value, uint16 *unicode_buffer)
{
    if(byte_value <= 9)
    {
        byte_value = (byte_value + '0');
    }
    else if((byte_value >= 10) && (byte_value <= 15))
    {
        byte_value = byte_value - 10 + 'A';
    }
    else
    {
        byte_value = 0;
    }

    *unicode_buffer = byte_value;

    return 1;
}

static int32 two_bytes_to_unicode(uint8 byte_value, uint16 *unicode_buffer, uint32 base)
{
    uint8 temp_value;
    uint32 i;
    int32 trans_len = 0;

    for(i = 0; i < 2; i++)
    {
        if(i == 0)
        {
            temp_value = byte_value / base;
        }
        else
        {
            temp_value = byte_value % base;
        }

        trans_len++;

        byte_to_unicode(temp_value, &(unicode_buffer[i]));
    }

    return trans_len;
}

static void bytes_to_unicode(uint8 *byte_buffer, uint8 byte_index, uint8 byte_len, uint16 *unicode_buffer, uint16 *unicode_len)
{
    uint32 i;

    int32 trans_len;

    for(i = 0; i < byte_len; i++)
    {
        trans_len = two_bytes_to_unicode(byte_buffer[byte_index - i], unicode_buffer, 16);

        unicode_buffer += trans_len;

        *unicode_len += trans_len;
    }

    return;
}


static void uint32_to_unicode(uint32 value, uint16 *unicode_buffer, uint16 *unicode_len, uint32 base)
{
    uint32 i;
    uint32 trans_byte;
    uint8 temp_data[12];
    uint32 div_val;

    i = 0;

    trans_byte = 0;

    div_val = value;

    while(div_val != 0)
    {
        temp_data[i] = div_val % base;
        div_val = div_val / base;
        i++;
    }

    while(i != 0)
    {
        trans_byte = byte_to_unicode(temp_data[i-1], unicode_buffer);
        unicode_buffer += trans_byte;
        *unicode_len += trans_byte;
        i--;
    }

    return;
}

static void int32_to_unicode(int32 value, uint16 *unicode_buffer, uint16 *unicode_len, uint32 base)
{
    uint32 i;
    uint32 trans_byte;
    uint8 temp_data[12];
    uint32 div_val;

    if(value == 0)
    {
        *unicode_buffer = '0';
        *unicode_len += 1;
        return;
    }

    i = 0;
    trans_byte = 0;

    if(value < 0)
    {
        *unicode_buffer = '-';
        unicode_buffer++;
        *unicode_len += 1;
        value = 0 - value;
    }

    div_val = value;

    while(div_val != 0)
    {
        temp_data[i] = div_val % base;
        div_val = div_val / base;
        i++;
    }

    while(i != 0)
    {
        trans_byte = byte_to_unicode(temp_data[i-1], unicode_buffer);
        unicode_buffer += trans_byte;
        *unicode_len += trans_byte;
        i--;
    }

    return;
}

#endif

