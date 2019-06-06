/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM����������Ӧ��ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __APP_FMENGINE_H__
#define __APP_FMENGINE_H__

#include "common_engine_app_def.h"
#include "fm_common.h"
#include "mmm_pp.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //��Ӧ��engine�ɶ��������Ƿ�֧�ֿ���ģʽOK MIC

/* FM ģ��״̬ö��*/
typedef enum
{
    MODULE_NO_INIT = 0, //ģ�鴦��δ��ʼ��״̬
    MODULE_WORK, //ģ���ʼ����ɣ�������������״̬
    MODULE_STANDBY, //ģ�鴦��Standby ״̬
} Module_Status_e;

/* extern ��������*/
extern FM_Drv_Status_t g_fmengine_status;

extern Module_Status_e g_module_status;

extern uint32 fm_clock_freq;

extern fmeg_init_para_t g_init_para_backup;

extern bool g_stop_by_tts_flag;

extern uint8 g_enter_mode;

//�����м�����
extern void *pp_handle;
extern mmm_pp_status_t g_pp_status;

//����״̬�����ṹ��ָ��
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

/* extern ��������*/
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
