/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ��������Ӧ��ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __APP_LINEINEG_H__
#define __APP_LINEINEG_H__

#include "common_engine_app_def.h"
#include "linein_common.h"
#include "mmm_pp.h"
#include "record_common.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //��Ӧ��engine�ɶ��������Ƿ�֧�ֿ���ģʽOK MIC

/*¼��������*/
typedef enum
{
    /*¼�������ʣ�8KHZ*/
    RATE_8KHZ = 0,
    /*¼�������ʣ�11KHZ*/
    RATE_11KHZ = 1,
    /*¼�������ʣ�12KHZ*/
    RATE_12KHZ = 2,
    /*¼�������ʣ�16KHZ*/
    RATE_16KHZ = 3,
    /*¼�������ʣ�22KHZ*/
    RATE_22KHZ = 4,
    /*¼�������ʣ�24KHZ*/
    RATE_24KHZ = 5,
    /*¼�������ʣ�32KHZ*/
    RATE_32KHZ = 6,
    /*¼�������ʣ�44KHZ*/
    RATE_44KHZ = 7,
    /*¼�������ʣ�48KHZ*/
    RATE_48KHZ = 8,

    /*����������ѡ��Χ*/
    RATE_MAX,
} record_sample_rate_e;

/*¼��������*/
typedef enum
{
    /*¼�������ʣ�32KBPS*/
    BIT_32KBPS = 0,
    /*¼�������ʣ�44KBPS*/
    BIT_44KBPS = 1,
    /*¼�������ʣ�48KBPS*/
    BIT_48KBPS = 2,
    /*¼�������ʣ�64KBPS*/
    BIT_64KBPS = 3,
    /*¼�������ʣ�88KBPS*/
    BIT_88KBPS = 4,
    /*¼�������ʣ�96KBPS*/
    BIT_96KBPS = 5,
    /*¼�������ʣ�128KBPS*/
    BIT_128KBPS = 6,
    /*¼�������ʣ�176KBPS*/
    BIT_176KBPS = 7,
    /*¼�������ʣ�192KBPS*/
    BIT_192KBPS = 8,

    /*¼������������ѡ��Χ*/
    BIT_MAX,
} record_bitrate_e;

//���ֲ���ģʽ
typedef enum
{
    //��������
    PLAY_NORMAL,
    //����
    PLAY_RESUME,
    //�������
    PLAY_FFON,
    //���˲���
    PLAY_FBON,
    //����Ҫ����
    PLAY_NO_PLAY
} play_mode_e;

//����ֹͣģʽ
typedef enum
{
    //����ֹͣ
    STOP_NORMAL,
    //��ͣ
    STOP_PAUSE
} stop_mode_e;

//globle variable
//�����м�����
extern void *pp_handle;
extern mmm_pp_status_t g_pp_status;

//����״̬�����ṹ��ָ��
extern lineineg_status_t g_eg_status;

extern bool g_need_auto_play_flag;

extern uint8 g_linein_play_chan;

//ϵͳȫ�ֱ���

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
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
