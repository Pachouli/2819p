/*******************************************************************************
 *                              US280A
 *                            Module: bt engine
 *                 Copyright(c) 2003-2014 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      tangweihua   2013-5-7         1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_print_link_key.c
 * \brief    将link key打印出来
 * \author
 * \version  1.0
 * \date
 *******************************************************************************/
#include "bluetooth_engine.h"

#define BT_DEBUG_PRINT_LINKKEY

/******************************************************************************/
/*!
 * \par  Description:
 *  将link key打印出来,用于调试
 * \param[in] unsigned char *buf:需要打印的字符地址
 *            len:打印的字节长度
 * \param[out]   none
 * \return       none
 * \ingroup
 * \note
 *******************************************************************************/
void print_link_key(unsigned char *buf, int32 len)
{
#ifdef BT_DEBUG_PRINT_LINKKEY
    uint8 link_key_str[40];
    int i, index;
    uint8 dat, t1, t2;

    index = 0;
    link_key_str[index] = 'L';
    index++;
    link_key_str[index] = 'I';
    index++;
    link_key_str[index] = 'N';
    index++;
    link_key_str[index] = 'K';
    index++;
    link_key_str[index] = ':';
    index++;

    for (i = 0; i < len; i++)
    {
        dat = *(buf + i);
        t1 = dat & 0x0f;
        t2 = (dat & (uint8) 0xf0) >> 4;

        if (t2 < 10)
        {
            link_key_str[index] = t2 + '0';
            index++;
        }
        else
        {
            link_key_str[index] = t2 - 10 + 'a';
            index++;
        }

        if (t1 < 10)
        {
            link_key_str[index] = t1 + '0';
            index++;
        }
        else
        {
            link_key_str[index] = t1 - 10 + 'a';
            index++;
        }
    }

    link_key_str[index] = '\n';
    index++;
    link_key_str[index] = 0x0;

    libc_printf(link_key_str);
#endif
}

void print_bt_addr(uint8 *addr)
{
    uint8 tmp_addr[20], t, i;

    tmp_addr[0] = 'L';
    tmp_addr[1] = 'A';
    tmp_addr[2] = 'P';
    tmp_addr[3] = ':';

    for (i = 0; i < 6; i++)
    {
        t = addr[5 - i] / 16;
        t = (t >= 10) ? ('A' + t - 10) : ('0' + t);
        tmp_addr[4 + i * 2] = t;
        t = addr[5 - i] % 16;
        t = (t >= 10) ? ('A' + t - 10) : ('0' + t);
        tmp_addr[4 + i * 2 + 1] = t;
    }

    tmp_addr[4 + i * 2] = '\r';
    tmp_addr[4 + i * 2 + 1] = '\n';
    tmp_addr[4 + i * 2 + 2] = 0x00;

    libc_printf("<bt addr> %s\n", tmp_addr);
}

void print_dev_name(uint8 *name)
{
    uint8 idx = 0;

    libc_printf("name:");
    for (idx = 0; idx < MGR_MAX_REMOTE_NAME_LEN; idx++)
    {
        libc_printf("%c ", name[idx]);
    }
    libc_printf("\n");
}

void print_dev_linkkey(uint8 *linkkey)
{
    uint8 idx = 0;

    libc_printf("name:");
    for (idx = 0; idx < LINKKEY_SIZE; idx++)
    {
        libc_printf("%c ", linkkey[idx]);
    }
    libc_printf("\n");
}

void printf_plist_bt_dev(bt_dev_t* p_dev)
{
    libc_printf("------------------------\nplist %p:\n", p_dev);
    print_bt_addr(p_dev->dev_info.bd_addr.bytes);
    print_dev_name(p_dev->dev_info.name);
    print_dev_linkkey(p_dev->linkKey);
    libc_printf("linkkey type:0x%x\n", p_dev->linkKeyType);
    libc_printf("mru_num:%d\n", p_dev->mru_num);
    libc_printf("profile:%d\n", p_dev->profile);
    libc_printf("p_dev_status:%p\n", p_dev->p_dev_status);
    libc_printf("dev_type:%d\n", p_dev->dev_type);
    libc_printf("is_source:%d\n", p_dev->is_source);
    libc_printf("data_validity:%d\n", p_dev->data_validity);
    libc_printf("------------------------\n");
}

