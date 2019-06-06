/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌前台应用头文件
 ********************************************************************************/

#ifndef __APP_MUSIC_H__
#define __APP_MUSIC_H__

#include "common_front_app_def.h"
#include "music_common.h"


typedef struct
{
    /*! 魔数 */
    uint16 magic;
    /*! eq模式 */
    com_eqtype_e eq_type;
    /*! 循环模式 */
    fsel_mode_e repeat_mode;
    /*! shuffle 开关 取高4bit*/
    uint8 shuffle_flag;
    /*! 播放速度*/
    uint16 playspeed;
    /*!支持卡播放*/
    uint8 card_open;

} music_config_t;

typedef struct
{
    uint8 enter_mode;
    uint8 play_disk;
} mode2disk_t;


//for globle variables

//globle variable

//进入music的方式
extern uint8 g_enter_mode;

//连续错误歌曲数
extern uint16 g_error_num;

//全出错切换盘标志
extern bool g_all_error_switch;

//向前切
extern uint16 g_switch_prev;

//文件路径来源信息保存，播放时候用
extern file_path_info_t g_file_path_info;

//当前播放状态信息
extern mengine_status_t g_music_status;
extern play_status_e last_play_status;

//当前播放时间和比特率信息
extern mengine_playinfo_t g_play_info;

//music总时间信息
extern mmm_mp_file_info_t g_music_fileinfo;

//music ui的配置项信息
extern music_config_t g_music_config;

//当前歌曲总时间
extern uint32 g_total_time;

//当前播放时间
extern uint32 g_cur_time;

//前一次记录播放时间
extern uint32 g_prev_time;

extern uint8 last_minute, last_col;

//文件系统句柄
extern uint32 eh_vfs_mount;

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
extern uint8 g_fname_buffer[32];
extern bool  g_fname_scroll_flag;
extern bool  g_scroll_flag;
#endif

extern app_result_e music_scene_playing(void);

extern app_result_e music_scene_linein(void);

//for function rcode
extern app_result_e music_message_deal(private_msg_t *msg);

extern bool music_get_filepath(file_path_info_t *filepath);

extern bool music_get_status(mengine_status_t *play_status);

extern bool music_get_playinfo(mengine_playinfo_t *play_info);

extern bool music_get_playmode(uint8 *playmode);

extern bool music_get_filename(music_file_name_t *music_file_name);

extern bool music_play(void);

extern bool music_pause(void);

extern bool music_resume(void);

extern bool music_stop(void);

extern bool music_fast_forward(void);

extern bool music_fast_backward(void);

extern bool music_cancel_ffb(void);

extern bool music_send_msg(void *input_param, void *output_param, uint32 msg_type);

extern bool music_set_volume(uint8 volume);

extern bool music_set_playmode(uint8 playmode);

extern bool music_play_next(void);

extern bool music_play_prev(void);

extern bool music_switch_disk(uint8 disk_flag);

extern bool music_clear_error(void);

extern bool music_get_fileinfo(mmm_mp_file_info_t *fileinfo);

extern bool music_get_disknumber(uint16 *disk_num);

//手动切歌播报曲目号或切歌TTS，之后再恢复播放
extern bool music_tts_play_song_seq(uint16 song_seq);
extern bool music_switch_song_play(void);

//主动切歌到第几首
extern uint16 g_manul_switch_song_seq;

extern app_result_e play_eror_deal(eg_err_e error_number);

//for play functions

extern app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg);

extern app_result_e play_key_mode(void);

extern app_result_e play_key_prev(void);

extern app_result_e play_key_next(void);

extern app_result_e play_key_play(void);

extern app_result_e play_key_playfast(uint8 direct);

extern app_result_e _scene_play_fast_deal(uint8 ffb);

extern app_result_e play_key_repeat_next(void);

extern app_result_e play_key_repeat_prev(void);

extern app_result_e play_key_folderadd(void);

extern app_result_e play_key_foldersub(void);

extern app_result_e _playing_loop_deal(void);

extern app_result_e play_disk_out_deal(uint8 out_disk);

extern app_result_e play_disk_switch_deal(uint8 dest_disk);

extern void _playing_show_seq(uint32 song_seq);

#endif //__APP_MUSIC_H__
