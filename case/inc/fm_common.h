/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������Ӧ����غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ������ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __FM_COMMON_H__
#define __FM_COMMON_H__

#include "psp_includes.h"
#include "fm_interface.h"
#include "record_common.h"

#define FM_EG_ENTER_NORMAL   0 //������������
#define FM_EG_ENTER_STANDBY  1 //FM����Standby
#define FM_EG_ENTER_IDLE     2 //FM����Idle

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
    /*! ֹͣ */
    StopSta = 0,
    /*! ��ͣ */
    PauseSta,
    /*! ���� */
    PlaySta,
} play_status_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */
    play_status_e play_status;
    /*! ¼��״̬ */
    record_status_e record_sta;
    /*! ����������ʱ�� */
    int32 lowpw_time;
} fmeg_status_t;

typedef struct
{
    uint32 freq_init; //frequency init
    uint32 freq_low; //frequency lowest
    uint32 freq_high; //frequency highest
    uint8 span; //channel span: 100KHZ, or 50KHZ
    uint8 threshold; //search threshold
    uint8 de_emphasis_tc; //ȥ����ʱ�䳣�� ��������75us(f1=2.123KHz),ŷ�ޡ��ձ����й�����50us(f1=3.185KHz);
} fmeg_init_para_t;

#endif //__FM_COMMON_H__
