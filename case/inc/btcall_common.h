/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������������غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ�����Ƹ�ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BTCALL_COMMON_H__
#define __BTCALL_COMMON_H__

#include "psp_includes.h"
#include "mmm_sp_common.h"
#include "record_common.h"

typedef enum
{
    BTCALL_IDLE = 0,
    BTCALL_STOP = 1,
    BTCALL_PLAY = 2,
} btcall_status_e;

typedef struct
{
    btcall_status_e status;
    /*! ¼��״̬ */
    record_status_e record_sta;
} btcall_info_t;

//PLAY����
typedef struct
{
    asqt_configs_t *p_asqt_config;
    asqt_dump_buf_t *p_asqt_dump_buf;
    bool oncall;
    bool att_flag;//�Զ������Ա�־���Զ�������ʱ�ر�AEC
} btcall_speech_play_para_t;

#endif //__BTCALL_COMMON_H__
