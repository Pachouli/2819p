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
 * \brief    MainMenu的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#ifndef _USOUND_EG_H
#define _USOUND_EG_H

#include "common_engine_app_def.h"
#include "ud_extern.h"
#include "mmm_pp.h"
#include "usound_common.h"
#include "usound_interface.h"

#define  PLAY_CMD         (0x08)
#define  PAUSE_CMD        (0x08)
#define  NEXT_CMD         (0x10)
#define  PREV_CMD         (0x20)
#define  VOL_ADD_CMD      (0x01)
#define  VOL_DEC_CMD      (0x02)
//globle variable
//保存中间件句柄
extern void *pp_handle;
extern mmm_pp_status_t g_pp_status;

//引擎状态变量结构体指针
extern usoundeg_status_t g_eg_status;

extern bool g_start_play;
extern bool g_auto_set_play;

extern bool s_have_data;//FALSE无数据，TRUE有数据
extern usound_status_t g_status_data;
extern uint8 uengine_para;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
extern record_param_t g_record_param;
extern record_module_status_t g_rm_status;

extern bool _record_start(uint8 channel, uint8 record_source);
extern bool _record_stop(void);
extern bool _get_mmm_record_status(void);
extern bool _record_create_file(void);
extern bool _record_close_file(void);
extern app_result_e usoundeg_record_start(private_msg_t *msg_ptr);
extern app_result_e usoundeg_record_stop(private_msg_t *msg_ptr);
#endif

typedef struct
{
    uint8 s_manual_set_play;
    uint8 s_manual_set_pause;
    uint8 s_pc_set_play;
    uint8 s_pc_set_pause;
} usound_oper_t;

extern usound_oper_t us_oper;
extern uint32 adc_sample;

typedef struct
{
    uint8 hid_idle_cnt;
    uint8 send_data_cnt;
} usound_timer_t;

extern usound_timer_t tusound;
extern int16 valid_energy;

#if 0
typedef struct
{
    uint32 play_status;
    uint32 change_vol_val;
    uint16 line_status;
    uint16 switch_flag;
}usoundeg_status_t;
#endif

//系统全局变量

//因TTS播报而暂停播放音乐，TTS播报结束后需要恢复播放音乐
extern bool g_stop_by_tts_flag;

extern bool _play(uint32 sample);
extern bool _stop(void);
extern bool _eq_set(void);
extern bool _get_mmm_status(void);

extern void usoundeg_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

extern app_result_e usoundeg_control_block(void);

extern app_result_e usoundeg_message_done(private_msg_t *cur_msg);

extern app_result_e usoundeg_message_done_bank(private_msg_t *cur_msg);

//for event

extern app_result_e usoundeg_get_status(private_msg_t *msg_ptr);

extern app_result_e usoundeg_eq_set(private_msg_t *msg_ptr);

extern app_result_e usoundeg_pause(private_msg_t *msg_ptr);

extern app_result_e usoundeg_resume(private_msg_t *msg_ptr);

extern app_result_e usoundeg_next_song(private_msg_t *msg_ptr);

extern app_result_e usoundeg_prev_song(private_msg_t *msg_ptr);

extern app_result_e usoundeg_vol_add(private_msg_t *msg_ptr);

extern app_result_e usoundeg_vol_sub(private_msg_t *msg_ptr);

//for tts sychonous
extern app_result_e usoundeg_tts_start(private_msg_t *msg_ptr);

extern app_result_e usoundeg_tts_stop(private_msg_t *msg_ptr);

extern app_result_e usoundeg_enter_s2(private_msg_t *cur_msg);

//usound timer deal
extern void usound_timer_init(void);
extern void usound_timer_counter(void);

extern void usound_eg_set_filter(uint8 filter);

//设置声音有变化标志
//extern void usound_eg_set_volume_chg_flag(uint32 flag);



#endif
