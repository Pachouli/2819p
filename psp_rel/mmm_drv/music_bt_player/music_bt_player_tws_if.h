/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

extern bool tws_get_dac_samples_bt_clock(music_bt_player_t *music_player, uint32 *p_pcm_buf_remain, bt_clock_t *p_dl_clock) __FAR__;

extern int32 tws_start_music_decoder_sub_pre(music_bt_player_t *music_player);
extern int32 tws_stop_music_decoder_sub(music_bt_player_t *music_player, bool stop);

extern uint8 tws_monitor_aps_adjust_sync(music_bt_player_t *music_player, uint8 aps, uint32 bt_clock);
extern uint8 tws_monitor_aps_adjust_fastslow(music_bt_player_t *music_player);

extern bool tws_cmd_and_event_deal(music_bt_player_t *music_player);

extern int32 tws_update_play_info_s2m(music_bt_player_t *music_player);
extern int32 tws_exec_update_play_info(music_bt_player_t *music_player, tws_play_info_t *p_tws_play_info);

extern int32 tws_update_ip_frame_info_s2m(music_bt_player_t *music_player);
extern int32 tws_exec_update_ip_frame_info(music_bt_player_t *music_player, tws_ip_frame_info_t *p_tws_ip_frame_info);

extern int32 tws_update_audio_aps_m2s(music_bt_player_t *music_player, tws_m2s_set_audio_aps_para_t *p_set_audio_aps_para);
extern int32 tws_exec_update_audio_aps(music_bt_player_t *music_player, tws_m2s_set_audio_aps_para_t *p_set_audio_aps_para);

extern int32 tws_set_start_play_m2s(music_bt_player_t *music_player, bt_clock_t *p_dl_clock);
extern int32 tws_exec_start_play(music_bt_player_t *music_player, tws_m2s_start_play_para_t *p_start_play_para);

extern bool tws_set_and_wait_sync_play(music_bt_player_t *music_player, bt_clock_t *p_should_start_con_clock);
extern int32 tws_ture_start_play(music_bt_player_t *music_player, uint32 start_dac_con_clock);

extern int32 tws_master_adjust_audio_aps(music_bt_player_t *music_player, uint8 aps);
extern int32 tws_slave_adjust_audio_aps_ing(music_bt_player_t *music_player);
extern int32 tws_master_wait_adjust_audio_aps_over(music_bt_player_t *music_player);

extern uint16 tws_frame_check_sum_deal(uint8 *frame_data, uint16 frame_len);

#endif
