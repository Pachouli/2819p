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
 * \file     ap_udevice.h
 * \brief    MainMenuµÄmainº¯ÊýÄ£¿é
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#ifndef _AP_USOUND_H
#define _AP_USOUND_H

#include "common_front_app_def.h"
#include "ud_extern.h"
#include "usound_common.h"
#include "bluetooth_common.h"

extern usoundeg_status_t g_cur_play_status;
extern usound_vol_sync_e g_volume_sync;
extern bool g_need_volume_sync;
extern play_status_e g_last_usbbox_status;

extern app_result_e scene_usound_play(void);

extern app_result_e usound_msg_deal(input_gui_msg_t* ptr_gui_msg);
extern bool usound_volume_add(void);
extern bool usound_volume_sub(void);
extern bool usound_play_pause(void);
extern bool usound_play_next(void);
extern bool usound_play_prev(void);
extern bool usound_get_play_status(usoundeg_status_t* ptr);

extern bool mannul_set_play(void);
extern bool mannul_set_pause(void);

extern bool usound_tts_start(void);
extern bool usound_tts_stop(void);
extern void pc_set_volume_sync(void);

#endif
