/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话前台应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _AP_BLUETOOTH_H
#define _AP_BLUETOOTH_H

#include  "common_front_app_def.h"
#include  "bluetooth_common.h"
#include  "btcall_common.h"
#include  "ap_bluetooth_asqt.h"

#ifdef bt_auto_test
extern int get_statis_timer_id;
extern uint8 g_get_status_flag;
extern void deal_random_test(void);
#endif

//#define SUPPORT_PB_NAME_PLAY //是否支持电话本名字播报

#ifdef SUPPORT_PB_NAME_PLAY
#define MAX_TTS_INFO_LENGTH 48
#define PB_MAX_NAME_LEN     16
#define PB_MAX_NUM_LEN      16
#else
#define MAX_TTS_INFO_LENGTH 16
#define PB_MAX_NAME_LEN     0
#define PB_MAX_NUM_LEN      16
#endif

#define CALLIN_DEFAULT_RING_SUPPORT 1 //是否支持内置来电铃声

#define CALLIN_HINT_MODE_ONLY_RING  0 //仅铃声
#define CALLIN_HINT_MODE_ONLY_PB    1 //仅电话本播报
#define CALLIN_HINT_MODE_RING_PB    2 //铃声+电话本播报

//电话本播放状态机：停止状态、获取到来电内容、播放状态、间歇状态
typedef enum
{
    PB_STATUS_STOP = 0,
    PB_STATUS_GET_CONTENT,
    PB_STATUS_PLAY,
    PB_STATUS_BREAK,
} phonebook_play_status_e;

extern uint8 TTS_info_buf[MAX_TTS_INFO_LENGTH];
extern uint8 g_pb_result;
extern uint8 g_pb_len;
extern bool g_pb_need_init;
extern phonebook_play_status_e g_pb_play_status;
extern uint32 g_pb_break_timer;

extern uint8 g_disp_status;
extern uint8 last_disp_status;
extern bool last_sco_created_flag;

extern btengine_info_t g_btcall_bt_info;

extern btcall_info_t g_btcall_info;

extern bool g_chf_cag_exch_enable;

extern bool g_speech_play_flag;

extern bool g_speech_peq_flag;

extern void *callring_handle;

//callring 播放状态机：停止状态、播放状态、间歇状态
typedef enum
{
    CR_STATUS_STOP = 0,
    CR_STATUS_PLAY,
    CR_STATUS_BREAK,
} callring_status_e;

extern callring_status_e g_cr_status;
extern uint32 g_cr_timer;

extern bool callring_wait_sco_flag;
extern uint32 g_wait_sco_timer;
extern uint8 ignore_default_ring_flag;
extern uint8 callin_hint_mode;
extern uint8 callring_space_time;
extern uint8 callring_pb_space_time;

extern bool _callring_play(void);
extern bool _callring_stop(void);
extern bool _callring_is_over(void);

extern bool btcall_pb_tts_init(void);
extern void btcall_pb_tts_play(void);
extern void btcall_pb_tts_stop(void);
extern bool btcall_pb_tts_is_over(void);

extern bool _speech_play(bool oncall);
extern bool _speech_stop(bool oncall);

extern app_result_e bthfp_msg_deal(void);
extern app_result_e get_message_loop(void);
extern app_result_e bt_message_deal(private_msg_t *msg);
extern app_result_e deal_gui_bt(input_gui_msg_t *gui_msg);
extern void bt_ui_display(void);
extern void update_volume_syn(void);
extern app_result_e hfp_siri_deal(bool start);

#if (SUPPORT_ASQT_TEST == 1)
extern app_result_e get_message_loop_offline(void);
#endif

#endif

