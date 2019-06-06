/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入前台应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __APP_LINEIN_H__
#define __APP_LINEIN_H__

#include "common_front_app_def.h"
#include "linein_common.h"
#include "bluetooth_common.h"
#include "btplay_common.h"

//当前播放状态信息
extern lineineg_status_t g_linein_status;
extern play_status_e g_last_linein_status;

extern uint8 g_linein_play_mode;

extern bool g_linein_tws_flag;

#if (SUPPORT_RECORD_FUNC == 1)
extern uint32 rec_time_bak;
extern uint32 rec_time_cur;
extern uint32 rec_time_display;
extern bool linein_save_record_restart(void);
#endif

extern app_result_e scene_linein_play(void);

extern bool linein_pause(void);

extern bool linein_resume(void);

extern bool linein_get_status(lineineg_status_t *play_status);

app_result_e linein_message_deal(private_msg_t *msg);

extern app_result_e _linein_loop_deal(void);

extern app_result_e linein_key_play(void);

#ifdef ENABLE_TRUE_WIRELESS_STEREO

extern btengine_info_t g_linein_bt_info;
extern uint8 last_tws_pair_status;
extern uint8 last_tws_dev_role;
extern uint8 last_phone_status;
extern bool g_linein_tws_enable;
extern btplay_info_t g_btplay_info;
extern uint8 btplay_status_count;

extern app_result_e tws_dev_spk_pos_switch(void);
extern bool tws_tts_display_deal(void);

#endif

extern void btplay_tws_conform_mode(bool filter);
extern app_result_e btplay_key_play(void);
extern void _btplay_playing_check_status(void);

extern void _kill_conflict_engine(void);
extern bool _create_linein_engine(bool play);
extern bool _create_btplay_engine(void);

#endif //__APP_LINEIN_H__
