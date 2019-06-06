/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机引擎应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __APP_FMENGINE_H__
#define __APP_FMENGINE_H__

#include "common_engine_app_def.h"
#include "fm_common.h"
#include "mmm_pp.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //各应用engine可独立定义是否支持空闲模式OK MIC

/* FM 模组状态枚举*/
typedef enum
{
    MODULE_NO_INIT = 0, //模组处于未初始化状态
    MODULE_WORK, //模组初始化完成，处于正常工作状态
    MODULE_STANDBY, //模组处于Standby 状态
} Module_Status_e;

/* extern 变量声明*/
extern FM_Drv_Status_t g_fmengine_status;

extern Module_Status_e g_module_status;

extern uint32 fm_clock_freq;

extern fmeg_init_para_t g_init_para_backup;

extern bool g_stop_by_tts_flag;

extern uint8 g_enter_mode;

//保存中间件句柄
extern void *pp_handle;
extern mmm_pp_status_t g_pp_status;

//引擎状态变量结构体指针
extern fmeg_status_t g_eg_status;

extern bool _play(void);
extern bool _stop(void);
extern bool _eq_set(void);
extern bool _get_mmm_status(void);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
extern record_param_t g_record_param;
extern record_module_status_t g_rm_status;

extern bool _record_start(uint8 channel, uint8 record_source);
extern bool _record_stop(void);
extern bool _get_mmm_record_status(void);
extern bool _record_create_file(void);
extern bool _record_close_file(void);
extern app_result_e fmengine_record_start(private_msg_t *msg_ptr);
extern app_result_e fmengine_record_stop(private_msg_t *msg_ptr);
#endif

/* extern 函数声明*/
extern app_result_e fmengine_control_block(void);

extern app_result_e fm_module_init(private_msg_t *cur_msg);
extern bool fm_module_open_do(fmeg_init_para_t *p_fmeg_init_para);

extern app_result_e fm_module_standby(void);

extern app_result_e fmengine_set_freq_sync(private_msg_t *cur_msg);

extern app_result_e fmengine_mute_sync(private_msg_t *cur_msg);

extern app_result_e fmengine_set_throd(private_msg_t *cur_msg);

extern app_result_e fmengine_set_softseek(private_msg_t *cur_msg);

extern app_result_e fmengine_get_fm_status(private_msg_t *cur_msg);

extern app_result_e fmengine_get_play_status(private_msg_t *cur_msg);

extern app_result_e fmengine_get_freq(private_msg_t *cur_msg);

extern app_result_e fmengine_tts_start(private_msg_t *cur_msg);

extern app_result_e fmengine_tts_stop(private_msg_t *cur_msg);

extern app_result_e fmengine_enter_s2(private_msg_t *cur_msg);

extern app_result_e fmengine_pause(private_msg_t *msg_ptr);

extern app_result_e fmengine_resume(private_msg_t *msg_ptr);

extern app_result_e fmengine_set_eq(private_msg_t *msg_ptr);

extern void fmengine_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

#endif //__APP_FMENGINE_H__
