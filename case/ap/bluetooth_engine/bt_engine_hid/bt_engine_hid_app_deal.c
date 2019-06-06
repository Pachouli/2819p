/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenuµÄmainº¯ÊýÄ£¿é
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "./../bluetooth_engine.h"

uint8 app_message_deal_hid(private_msg_t *pri_msg)
{
    uint8 rst;
    rst = 0;

    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_HID_TAKE_PHOTO_SYNC:
        if ((g_p_info->service_connect & HID_CONNECTED) != 0)
        {
            uint8 *p_cur_rmt_dev_name = btengine_global_variable.g_cur_rmt_dev->dev_info.name;

            if ((libc_strncmp(p_cur_rmt_dev_name, "ZTE", 3) == 0) || (libc_strncmp(p_cur_rmt_dev_name, "GT-", 3) == 0)
                    || (libc_strncmp(p_cur_rmt_dev_name, "SM-", 3) == 0)
                    || (libc_strncmp(p_cur_rmt_dev_name, "HTC", 3) == 0))
            {
                HID_deal_keyboard_msg(hid_keyboard_enter);
            }
            else
            {
                HID_deal_consumer_ctl_msg(hid_volume_up);
            }
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_HID_CONNECT_SYNC:
        if ((g_p_info->service_connect & HID_CONNECTED) == 0)
        {
            HID_connect(btengine_global_variable.g_cur_rmt_dev);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_HID_DISCONNECT_SYNC:
        if ((g_p_info->service_connect & HID_CONNECTED) != 0)
        {
            HID_disconnect();
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}

