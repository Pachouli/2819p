/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_lib_init.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 * 原bt_dev_t中获取到的远端设备名仅16个字符，而实际上手机最大支持30个字符的utf-8编码
 * 字符，增加以下接口供app获取到完整的远端设备蓝牙名称
 *******************************************************************************/
#include "bluetooth_engine.h"

bt_remote_dev_name_st bt_remote_dev_list[MAX_DEVICE_LINK + 1];

bool bt_save_remote_phone_name(bt_dev_t *dev, uint8 *full_name)
{
    uint8 i;
    bt_remote_dev_name_st *p_bt_fullname = &bt_remote_dev_list[0];

    if (dev == NULL)
    {
        return FALSE;
    }

    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if (p_bt_fullname[i].dev == NULL)
        {
            break;
        }

        //防止手机已经修改了蓝牙设备名，即使在记录中也需要更新
        if (dev == p_bt_fullname[i].dev)
        {
            break;
        }

        //mru表明设备最近连接的情况，mru太大的话会被新设备替换
        //由于tws设备回连时会先当成一个正常手机设备进行连接，
        //所以需要多加一台设备记录
        if (p_bt_fullname[i].dev->mru_num >= MAX_DEVICE_LINK + 1)
        {
            break;
        }
    }

    if (i >= MAX_DEVICE_LINK)
    {
        BTEG_PRINTF_WARNNING("save bt fullname error: dev full\n");
        //while(1);
        i = 0;
    }

    p_bt_fullname[i].dev = dev;
    libc_memcpy(p_bt_fullname[i].full_name, full_name, BT_REMOTE_NAME_MAX_LEN);
    p_bt_fullname[i].full_name[BT_REMOTE_NAME_MAX_LEN - 1] = 0;
    return TRUE;
}

bool bt_get_remote_phone_name(bt_dev_t *dev, uint8 *full_name)
{
    uint8 i;
    bt_remote_dev_name_st *p_bt_fullname = &bt_remote_dev_list[0];

    if (dev == NULL)
    {
        BTEG_PRINTF("dev is null\n");
        return FALSE;
    }

    if (full_name == NULL)
    {
        BTEG_PRINTF("fullname buf is null\n");
        return FALSE;
    }

    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if (dev == p_bt_fullname[i].dev)
        {
            libc_memcpy(full_name, p_bt_fullname[i].full_name, BT_REMOTE_NAME_MAX_LEN);
            return TRUE;
        }
    }

    BTEG_PRINTF("not fullname record of this dev\n");
    return FALSE;
}

bool bt_clear_remote_phone_name(bt_dev_t *dev)
{
    uint8 i;
    bt_remote_dev_name_st *p_bt_fullname = &bt_remote_dev_list[0];

    if (dev == NULL)
    {
        BTEG_PRINTF("dev is null\n");
        return FALSE;
    }

    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if (p_bt_fullname[i].dev == NULL)
        {
            continue;
        }

        if ((dev == p_bt_fullname[i].dev) || (p_bt_fullname[i].dev->data_validity == 0))
        {
            libc_memset(p_bt_fullname[i].full_name, 0x00, BT_REMOTE_NAME_MAX_LEN);
            p_bt_fullname[i].dev = NULL;
            return TRUE;
        }
    }

    BTEG_DBG_PRINTF("dev is not in full name record list\n");

    return FALSE;
}

void bt_remote_dev_info_init(void)
{
    libc_memset(bt_remote_dev_list, 0x00, MAX_DEVICE_LINK*sizeof(bt_remote_dev_name_st));
}
void bt_remote_dev_info_deinit(void)
{
    libc_memset(bt_remote_dev_list, 0x00, MAX_DEVICE_LINK*sizeof(bt_remote_dev_name_st));
}

