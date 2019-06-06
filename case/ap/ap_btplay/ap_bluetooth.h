/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _AP_BLUETOOTH_H
#define _AP_BLUETOOTH_H

#include  "common_front_app_def.h"
#include  "bluetooth_common.h"
#include  "btplay_common.h"

#ifdef bt_auto_test
extern int8 get_statis_timer_id;
extern uint8 test_mode;
extern void deal_random_test(void);
#endif

//#define DOUBLE_CLICK_KEY_TEST

extern uint8 g_disp_status;
extern uint8 last_disp_status;
extern bool g_need_draw;
extern int8 display_timer_id;
extern bool display_flag;
extern tts_play_ret_e g_tts_enter_ret;
extern uint8 g_bt_tws_enable;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
extern uint8 last_tws_pair_status;
extern uint8 last_tws_dev_role;
extern uint8 g_tws_pos_switch_flag;
extern uint32 g_tws_pos_switch_time;
extern app_result_e tws_dev_spk_pos_switch(void);
extern void tws_tts_display_deal(void);

extern void bt_ui_display_tws(bool redraw_all);

extern void tws_switch_tws_mode(tws_mode_e tws_mode);
#endif

extern bool g_esd_reset_ignore_enter_tts_flag;
extern bool g_esd_reset_ignore_wait_tts_flag;
extern bool g_esd_reset_ignore_link_tts_flag;
extern bool g_esd_reset_need_play_flag;

#define AVRCP_PLAY_START_TIME (2000)
#define AVRCP_FAKE_START_TIME (3000)

extern uint8 g_avrcp_autoplay_cnt;
extern uint8 g_avrcp_autoplay_flag;
extern uint8 g_avrcp_autoplay_try;
extern uint8 g_send_avrcp_play_tick;
extern uint8 g_fake_play_tick;
extern uint8 g_btplay_tick_250ms;
extern int8 tick_timer_id;

extern btengine_info_t g_btplay_bt_info;
extern btplay_info_t g_btplay_info;

extern void get_engine_status(btengine_info_t *bg_info);
extern app_result_e bt_status_deal(void);
extern app_result_e get_message_loop(void);
extern app_result_e bt_message_deal(private_msg_t *msg);
extern app_result_e deal_gui_bt(input_gui_msg_t *gui_msg);
extern void bt_ui_display(bool redraw_all);
extern void bt_tts_play(void);
extern void enter_bt_tts_play(tts_play_background_hook hook);

extern app_result_e bt_error_deal(void);

extern void btplay_send_avrcp_play_pause(void);

extern void btplay_tws_conform_mode(void);

extern void _kill_conflict_engine(void);
extern void _create_btplay_engine(void);

#endif

