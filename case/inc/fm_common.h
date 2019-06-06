/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：收音机应用相关宏定义，枚举类型，结构体定义等，为收音机前台和引擎共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __FM_COMMON_H__
#define __FM_COMMON_H__

#include "psp_includes.h"
#include "fm_interface.h"
#include "record_common.h"

#define FM_EG_ENTER_NORMAL   0 //正常收音进入
#define FM_EG_ENTER_STANDBY  1 //FM进入Standby
#define FM_EG_ENTER_IDLE     2 //FM进入Idle

/*!
 * \brief
 *  当前播放状态
 */
typedef enum
{
    /*! 停止 */
    StopSta = 0,
    /*! 暂停 */
    PauseSta,
    /*! 播放 */
    PlaySta,
} play_status_e;

/*!
 * \brief
 *  引擎状态
 */
typedef struct
{
    /*! 播放状态  */
    play_status_e play_status;
    /*! 录音状态 */
    record_status_e record_sta;
    /*! 低能量持续时间 */
    int32 lowpw_time;
} fmeg_status_t;

typedef struct
{
    uint32 freq_init; //frequency init
    uint32 freq_low; //frequency lowest
    uint32 freq_high; //frequency highest
    uint8 span; //channel span: 100KHZ, or 50KHZ
    uint8 threshold; //search threshold
    uint8 de_emphasis_tc; //去加重时间常数 美国采用75us(f1=2.123KHz),欧洲、日本、中国采用50us(f1=3.185KHz);
} fmeg_init_para_t;

#endif //__FM_COMMON_H__
