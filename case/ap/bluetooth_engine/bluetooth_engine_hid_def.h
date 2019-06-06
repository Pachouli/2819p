/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen   2014-7-28 14:48     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine_hid_def.h
 * \brief    cailizhen
 * \version  1.0
 * \date     2014-7-28
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_HID_DEF_H
#define _BLUETOOTH_ENGINE_HID_DEF_H

typedef enum
{
    hid_volume_down = 0, //音量减
    hid_volume_up, //音量加
    hid_mute, //静音
    hid_play_pause, //播放/暂停
    hid_scan_prev, //切换上一首
    hid_scan_next, //切换下一首
    hid_fast_forward, //向前快进
    hid_rewind //卷绕
} hid_consumer_ctl_e;

typedef enum
{
    hid_keyboard_enter = 0, //for Photograph
    hid_keyboard_escape = 1, //退出当前应用
    hid_keyboard_backspace = 2, //回格
    hid_keyboard_tab = 3, //TAB
} hid_keyboard_e;

typedef enum
{
    hid_keyboard = 1, hid_mouse = 2, hid_consumer_control = 3
} hid_msg_type_e;

void HID_deal_keyboard_msg(uint8 msg_type);
void HID_deal_consumer_ctl_msg(uint8 msg_type);
void HID_disconnect(void);

#endif
