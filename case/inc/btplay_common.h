/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌相关宏定义，枚举类型，结构体定义等，为蓝牙推歌前台和引擎共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __BTPLAY_COMMON_H__
#define __BTPLAY_COMMON_H__

#include "psp_includes.h"
#include "record_common.h"

//BTPLAY 状态机
typedef enum
{
    BTPLAY_IDLE  = 0,
    BTPLAY_STOP  = 1,
    BTPLAY_PLAY  = 2,
    BTPLAY_PAUSE = 3,
    BTPLAY_EXCPT = 4, //未知异常，需要RESET
    //后续考虑添加快进退
} btplay_status_e;

typedef struct
{
    /*! 蓝牙播歌状态 */
    btplay_status_e status;
    /*! 录音状态 */
    record_status_e record_sta;
    /*! 低能量持续时间 */
    int32 lowpw_time;
    /*! ADC低能量支持时间 */
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
