/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：USB音箱相关宏定义，枚举类型，结构体定义等，为USB音箱前台和引擎共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __USOUND_COMMON_H__
#define __USOUND_COMMON_H__

#include "psp_includes.h"
#include "record_common.h"

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

typedef enum
{
    USOUND_VOL_SYNC_ONLY_DEVICE = 1,  //仅小机可调
    USOUND_VOL_SYNC_ONLY_PC = 2,      //仅PC可调
    USOUND_VOL_SYNC_PC_FIRST = 3,     //小机与PC均可调且PC优先
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
 *  引擎状态
 */
typedef struct
{
    /*! 播放状态  */
    play_status_e play_status;
    /*! 录音状态 */
    record_status_e record_sta;
    /*! PC音量同步到小机 */
    usoundeg_vol_sync_t vol_sync;
    /*! 低能量持续时间 */
    int32 lowpw_time;
} usoundeg_status_t;

#endif
