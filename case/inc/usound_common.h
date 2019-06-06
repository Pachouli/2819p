/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������USB������غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�ΪUSB����ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __USOUND_COMMON_H__
#define __USOUND_COMMON_H__

#include "psp_includes.h"
#include "record_common.h"

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

typedef enum
{
    USOUND_VOL_SYNC_ONLY_DEVICE = 1,  //��С���ɵ�
    USOUND_VOL_SYNC_ONLY_PC = 2,      //��PC�ɵ�
    USOUND_VOL_SYNC_PC_FIRST = 3,     //С����PC���ɵ���PC����
} usound_vol_sync_e;

typedef struct
{
    uint32 volume_chg; //if voluem syn
    uint32 volume_value; //syn volume value
    uint32 volume_max;
    uint32 volume_min;
} usoundeg_vol_sync_t;

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
    /*! PC����ͬ����С�� */
    usoundeg_vol_sync_t vol_sync;
    /*! ����������ʱ�� */
    int32 lowpw_time;
} usoundeg_status_t;

#endif
