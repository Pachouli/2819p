/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌引擎应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _BTPLAY_ENGINE_H
#define _BTPLAY_ENGINE_H

#include  "common_engine_app_def.h"
#include  "bluetooth_common.h"
#include  "btplay_common.h"
#include  "mmm_bp.h"

#include  "app_btplay_engine_tws.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //各应用engine可独立定义是否支持空闲模式OK MIC

extern void *bp_handle;
extern mmm_bp_status_t bp_status;
extern btengine_info_t btplay_bt_info;

extern btplay_info_t g_btplay_info;

extern bool g_filter_by_tts_flag;
extern bool g_filter_by_tws_flag;
extern bool g_filter_by_tws_mode_tdb;

extern btplay_data_source_e g_btplay_data_source;
extern btplay_data_sub_source_e g_btplay_data_sub_source;

extern uint8 g_bt_tws_enable;

extern bool g_lowenergy_poweroff_enable;
#define LOWENERGY_TIME_THRESHOLD  5000 //5S

extern record_param_t g_record_param;

extern void btplay_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals);
extern app_result_e btplay_engine_control_block(void);
extern app_result_e btplay_engine_eq_set(private_msg_t *msg_ptr);
extern app_result_e btplay_engine_tts_start(private_msg_t *msg_ptr);
extern app_result_e btplay_engine_tts_stop(private_msg_t *msg_ptr);
extern app_result_e btplay_engine_enter_s2(private_msg_t *cur_msg);

extern bool _play(void);
extern bool _stop(bool filter_flag);
extern bool _get_mmm_status(void);
extern bool _eq_set(void);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
extern record_param_t g_record_param;
extern record_module_status_t g_rm_status;

extern bool _record_start(uint8 channel, uint8 record_source);
extern bool _record_stop(void);
extern bool _get_mmm_record_status(void);
extern bool _record_create_file(void);
extern bool _record_close_file(void);
extern app_result_e btplay_engine_record_start(private_msg_t *cur_msg);
extern app_result_e btplay_engine_record_stop(private_msg_t *cur_msg);
#endif

#endif

