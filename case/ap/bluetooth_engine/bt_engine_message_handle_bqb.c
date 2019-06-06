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
#include  "bluetooth_engine.h"

static uint8 write_scan_disable[] =
{ 0x01, 0x1a, 0x0c, 0x01, 0x00 };
static uint8 write_scan_enaable[] =
{ 0x01, 0x1a, 0x0c, 0x01, 0x03 };
static uint8 write_cod[] =
{ 0x01, 0x24, 0x0c, 0x03, 0x00, 0x00, 0x00 };
static uint8 write_liac_giac[] =
{ 0x01, 0x3a, 0x0c, 0x07, 0x02, 0x00, 0x8b, 0x9e, 0x33, 0x8b, 0x9e };
static uint8 write_giac[] =
{ 0x01, 0x3a, 0x0c, 0x04, 0x01, 0x33, 0x8b, 0x9e };
static uint8 write_inquiry_scan[] =
{ 0x01, 0x1e, 0x0c, 0x04, 0x00, 0x10, 0x12, 0x00 };

static void delay(void)
{
    int i;
    for (i = 0; i < 0x1000; i++)
        ;
}

void bt_set_DiscoverableMode(uint16 inquiry_scan_interval, uint16 inquiry_scan_window, uint8 default_scan_mode,
        uint8 isLimitedMode)
{
    uint32 tmp32;

    Send(0, write_scan_disable, sizeof(write_scan_disable));
    delay();

    tmp32 = btengine_param.class_device;
    write_cod[4] = (uint8) (tmp32); //minor device
    write_cod[5] = (uint8) (tmp32 >> 8); //major device
    write_cod[6] = (uint8) (tmp32 >> 16); //major service
    if (isLimitedMode)
    {
        write_cod[5] |= 0x20;
    }
    else
    {
        write_cod[5] &= ~0x20;
    }
    Send(0, write_cod, sizeof(write_cod));
    delay();

    if (isLimitedMode)
    {
        Send(0, write_liac_giac, sizeof(write_liac_giac));
    }
    else
    {
        Send(0, write_giac, sizeof(write_giac));
    }
    delay();

    write_inquiry_scan[4] = inquiry_scan_interval & 0xff;
    write_inquiry_scan[5] = inquiry_scan_interval >> 8;
    write_inquiry_scan[6] = inquiry_scan_window & 0xff;
    write_inquiry_scan[7] = inquiry_scan_window >> 8;
    Send(0, write_inquiry_scan, sizeof(write_inquiry_scan));
    delay();

    write_scan_enaable[4] = default_scan_mode;
    Send(0, write_scan_enaable, sizeof(write_scan_enaable));
    delay();
}

uint8 app_message_deal_bqb(private_msg_t *pri_msg)
{
    uint8 rst;
    rst = 0;

    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_SET_TO_LIMITED_DISCOVERABLE_MODE_SYNC:
        BTEG_PRINTF("limited discoverable mode\n")
        ;
        bt_set_DiscoverableMode(0x0800, 0x0024, 3, TRUE);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_SET_TO_GENERAL_DISCOVERABLE_MODE_SYNC:
        BTEG_PRINTF("general discoverable mode\n")
        ;
        bt_set_DiscoverableMode(0x1000, 0x0012, 3, FALSE);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_SPP_CREATE_CONNECTION_SYNC:
#ifdef BT_SUPPORT_SPP
        if (bt_manager_cfg_info.sp_spp == 1)
        {
            BTEG_PRINTF("create spp connection\n");
            SppAppCreateConnection();
        }
        else
#endif
        {
            BTEG_PRINTF("spp not support\n");
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_SPP_DISCON_CONNECTION_SYNC:
#ifdef BT_SUPPORT_SPP
        if (bt_manager_cfg_info.sp_spp == 1)
        {
            BTEG_PRINTF("discon spp connection\n");
            SppAppDisconnect();
        }
        else
#endif
        {
            BTEG_PRINTF("spp not support\n");
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}

