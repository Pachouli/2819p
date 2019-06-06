/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙拍照前台应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _AP_BTHID_H
#define _AP_BTHID_H

#include  "common_front_app_def.h"
#include  "bluetooth_common.h"

extern uint8 g_disp_status;
extern uint8 last_disp_status;
extern bool g_need_draw;
extern int8 display_timer_id;
extern bool display_flag;

extern btengine_info_t g_bthid_bt_info;

extern app_result_e get_message_loop(void);

#endif

