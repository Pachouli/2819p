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
    hid_volume_down = 0, //������
    hid_volume_up, //������
    hid_mute, //����
    hid_play_pause, //����/��ͣ
    hid_scan_prev, //�л���һ��
    hid_scan_next, //�л���һ��
    hid_fast_forward, //��ǰ���
    hid_rewind //����
} hid_consumer_ctl_e;

typedef enum
{
    hid_keyboard_enter = 0, //for Photograph
    hid_keyboard_escape = 1, //�˳���ǰӦ��
    hid_keyboard_backspace = 2, //�ظ�
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
