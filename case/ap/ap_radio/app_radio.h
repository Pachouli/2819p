/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __APP_RADIO_H__
#define __APP_RADIO_H__

#include "common_front_app_def.h"
#include "fm_common.h"

//#define FM_THRESHOLD_TEST

#ifdef FM_THRESHOLD_TEST

#define MAX_TH_VALUE 9
#define MIN_TH_VALUE 0
extern int16 fm_threshold_values[512]; //门限值记录,-1表示搜索不到

#endif

/* 定义预设电台列表中所支持的保存电台数，需开出相应大小buffer*/
#define    MAX_STATION_COUNT    40

/* 定义各波段最小频点*/
#define    MIN_US      87500
#define    MIN_JP      76000
#define    MIN_EU      87500
/* 定义各波段最大频点*/
#define    MAX_US      108000
#define    MAX_JP      90000
#define    MAX_EU      108000

typedef enum
{
    DIR_DOWN = 0, //向下搜台
    DIR_UP //向上搜台
} FM_SeekDir_e;

typedef enum
{
    Band_China_US = 0, //china/USA band   87500---108000,  step 100kHz
    Band_Japan, //Japan band   76000---90000, step 100kHz
    Band_Europe, //Europe band  87500---108000, step 50kHz
    Band_MAX //播放用户电台时，播放模式可以为Band_MAX
} radio_band_e;

//radio AP场景定义
typedef enum
{
    /* FM 播放场景*/
    FM_SCENE_PLAYING,
    /* FM自动搜台场景 */
    FM_SCENE_AUTO_SEARCH,
    /* FM手动搜台场景-向后搜索 */
    FM_SCENE_MANUAL_UP_SEARCH,
    /* FM手动搜台场景-向前搜索 */
    FM_SCENE_MANUAL_DOWN_SEARCH,
    /* 场景退出*/
    FM_SCENE_EXIT,
} radio_scene_e;

typedef struct
{
    uint32 freq; //当前电台的频率。单位为1k (最高位恒为1)，取最低2 字节
    uint8 station; //当前电台对应的电台表项号，from 1 to station_count，当前电台未保存时为0
    uint8 station_count; //电台表中的保存电台数。
} fm_play_status_t;

//radio ui配置参数
typedef struct
{
    /* 魔术数*/
    uint16 magic;
    /* 快速预览信息，注意这几个变量位置要保持与 com_fast_preview_ui.c 中的相对地址一致，上电后会保存到 fast_preview_info_t 里 */
    uint8 preview_station_num; //0或0xff表示不是有效电台，1-40表示有效电台号
    uint8 reserve;
    uint32 preview_freq;

    /* 预设列表*/
    uint32 Auto_tab[MAX_STATION_COUNT];
    /* 当前播放的电台信息*/
    fm_play_status_t FMStatus;
    /* 当前播放波段*/
    radio_band_e band_mode;
} radio_config_t;


//全局变量声明
extern radio_scene_e g_radio_scene;

extern radio_config_t g_radio_config;

extern uint32 *pfreq;

extern FM_Drv_Status_t g_engine_status;

extern fmeg_status_t g_fmeg_status;
extern play_status_e g_last_fmeg_status;

extern const uint32 max_freq[];

extern const uint32 min_freq[];

extern uint32 startfreq;

extern uint8 g_enter_param;

extern bool need_tts;

extern uint32 g_radio_search_th;

extern uint8 g_freq_tts_sec_ids[10];

//for search cancel
extern const key_map_t cancel_key_map_list[];

extern bool  g_radio_seek_audition_enable;
extern uint8 g_radio_seek_audition_time;

extern uint32 g_number_input_freq;
extern uint32 g_number_input_timer;
extern uint8 g_number_input_high;
extern uint8 g_number_input_keys;
extern void get_border_freq(uint32 *start_freq, uint32 *end_freq, FM_SeekDir_e dir);

//函数声明

extern app_result_e radio_app_msg_callback(private_msg_t *pri_msg);

extern app_result_e scene_play_sysmsg(void);

extern bool radio_set_mute(FM_MUTE_e mode);

extern bool radio_set_freq(uint32 freq);

extern bool radio_set_softseek(uint32 freq, uint8 dir);

extern bool radio_set_throd(uint8 val);

extern bool radio_get_freq(void);

extern uint8 whether_in_list(uint32 freq, uint32 *p_freq);

extern app_result_e radio_scene_playing(bool first_flag);

extern app_result_e _playing_loop_deal(void);

extern app_result_e scene_play_guimsg(input_gui_msg_t *cur_msg);

extern app_result_e radio_scene_auto_search(FM_SeekDir_e dir);

extern app_result_e radio_scene_manual_search(FM_SeekDir_e dir);

#define SELECT_PREV_DIRECTION  0

#define SELECT_NEXT_DIRECTION  1

extern app_result_e select_station(uint8 direction);

extern app_result_e change_freq(FM_SeekDir_e dir);

extern app_result_e play_next_station(uint8 direction);

extern app_result_e radio_scene_play_search_deal(FM_SeekDir_e search_dir);

extern app_result_e radio_play_next_freq(void);

extern app_result_e radio_play_prev_freq(void);

extern app_result_e radio_play_pause_station(void);

extern void radio_freq_tts_play(uint32 freq);

extern app_result_e deal_soft_auto_seek(FM_SeekDir_e dir);

extern bool radio_send_msg(void *input_param, void *output_param, uint32 msg_type);

extern bool radio_resume(void);

extern bool radio_pause(void);

extern bool radio_get_play_status(fmeg_status_t *p_fmeg_status);

extern bool radio_get_fm_status(FM_Drv_Status_t *p_fm_drv_status);

extern void radio_show_frequency(uint32 freq, bool high_number_show);
extern void radio_show_station_num(uint8 station_num, bool msg_show);
extern void _create_engine(void);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
extern bool radio_record_start(void);
extern bool radio_record_stop(void);
#endif

#endif //__APP_RADIO_H__
