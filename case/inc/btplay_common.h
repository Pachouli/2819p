/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ���غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ�����Ƹ�ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BTPLAY_COMMON_H__
#define __BTPLAY_COMMON_H__

#include "psp_includes.h"
#include "record_common.h"

//BTPLAY ״̬��
typedef enum
{
    BTPLAY_IDLE  = 0,
    BTPLAY_STOP  = 1,
    BTPLAY_PLAY  = 2,
    BTPLAY_PAUSE = 3,
    BTPLAY_EXCPT = 4, //δ֪�쳣����ҪRESET
    //����������ӿ����
} btplay_status_e;

typedef struct
{
    /*! ��������״̬ */
    btplay_status_e status;
    /*! ¼��״̬ */
    record_status_e record_sta;
    /*! ����������ʱ�� */
    int32 lowpw_time;
    /*! ADC������֧��ʱ�� */
    int32 adc_lowpw_time;
} btplay_info_t;

typedef enum
{
    SOURCE_BT_A2DP = 0,
    SOURCE_LINEIN_ADC = 1,
} btplay_data_source_e;

typedef enum
{
    SUB_SOURCE_AUX = 0,
    SUB_SOURCE_SPDIF = 1,
    SUB_SOURCE_I2SRX = 2,
    SUB_SOURCE_HDMI = 3,
} btplay_data_sub_source_e;

#endif //__BTCALL_COMMON_H__
