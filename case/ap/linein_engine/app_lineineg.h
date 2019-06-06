/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入引擎应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __APP_LINEINEG_H__
#define __APP_LINEINEG_H__

#include "common_engine_app_def.h"
#include "linein_common.h"
#include "mmm_pp.h"
#include "record_common.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //各应用engine可独立定义是否支持空闲模式OK MIC

/*录音采样率*/
typedef enum
{
    /*录音采样率：8KHZ*/
    RATE_8KHZ = 0,
    /*录音采样率：11KHZ*/
    RATE_11KHZ = 1,
    /*录音采样率：12KHZ*/
    RATE_12KHZ = 2,
    /*录音采样率：16KHZ*/
    RATE_16KHZ = 3,
    /*录音采样率：22KHZ*/
    RATE_22KHZ = 4,
    /*录音采样率：24KHZ*/
    RATE_24KHZ = 5,
    /*录音采样率：32KHZ*/
    RATE_32KHZ = 6,
    /*录音采样率：44KHZ*/
    RATE_44KHZ = 7,
    /*录音采样率：48KHZ*/
    RATE_48KHZ = 8,

    /*采样率最大可选范围*/
    RATE_MAX,
} record_sample_rate_e;

/*录音比特率*/
typedef enum
{
    /*录音比特率：32KBPS*/
    BIT_32KBPS = 0,
    /*录音比特率：44KBPS*/
    BIT_44KBPS = 1,
    /*录音比特率：48KBPS*/
    BIT_48KBPS = 2,
    /*录音比特率：64KBPS*/
    BIT_64KBPS = 3,
    /*录音比特率：88KBPS*/
    BIT_88KBPS = 4,
    /*录音比特率：96KBPS*/
    BIT_96KBPS = 5,
    /*录音比特率：128KBPS*/
    BIT_128KBPS = 6,
    /*录音比特率：176KBPS*/
    BIT_176KBPS = 7,
    /*录音比特率：192KBPS*/
    BIT_192KBPS = 8,

    /*录音比特率最大可选范围*/
    BIT_MAX,
} record_bitrate_e;

//音乐播放模式
typedef enum
{
    //正常播放
    PLAY_NORMAL,
    //续播
    PLAY_RESUME,
    //继续快进
    PLAY_FFON,
    //快退播放
    PLAY_FBON,
    //不需要播放
    PLAY_NO_PLAY
} play_mode_e;

//音乐停止模式
typedef enum
{
    //正常停止
    STOP_NORMAL,
    //暂停
    STOP_PAUSE
} stop_mode_e;

//globle variable
//保存中间件句柄
extern void *pp_handle;
extern mmm_pp_status_t g_pp_status;

//引擎状态变量结构体指针
extern lineineg_status_t g_eg_status;

extern bool g_need_auto_play_flag;

extern uint8 g_linein_play_chan;

//系统全局变量

//因TTS播报而暂停播放音乐，TTS播报结束后需要恢复播放音乐
extern bool g_stop_by_tts_flag;
extern bool g_lowenergy_poweroff_enable;
#define LOWENERGY_TIME_THRESHOLD  5000 //5S

//functions

extern bool _play(void);
extern bool _stop(void);
extern bool _eq_set(void);
extern bool _get_mmm_status(void);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
extern record_param_t g_record_param;
extern record_module_status_t g_rm_status;

extern bool _record_start(uint8 channel, uint8 record_source);
extern bool _record_stop(void);
extern bool _get_mmm_record_status(void);
extern bool _record_create_file(void);
extern bool _record_close_file(void);
extern app_result_e lineineg_record_start(private_msg_t *msg_ptr);
extern app_result_e lineineg_record_stop(private_msg_t *msg_ptr);
extern bool lineineg_record_save_restart_do(void);
extern app_result_e lineineg_record_save_restart(private_msg_t *msg_ptr);
#endif

extern void lineineg_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

extern app_result_e lineineg_control_block(void);

extern app_result_e lineineg_message_done(private_msg_t *cur_msg);

extern app_result_e lineineg_message_done_bank(private_msg_t *cur_msg);

//for event

extern app_result_e lineineg_get_status(private_msg_t *msg_ptr);

extern app_result_e lineineg_eq_set(private_msg_t *msg_ptr);

extern app_result_e lineineg_pause(private_msg_t *msg_ptr);

extern app_result_e lineineg_resume(private_msg_t *msg_ptr);

//for tts sychonous
extern app_result_e lineineg_tts_start(private_msg_t *msg_ptr);

extern app_result_e lineineg_tts_stop(private_msg_t *msg_ptr);

extern app_result_e lineineg_enter_s2(private_msg_t *cur_msg);

#endif //__APP_LINEINEG_H__
