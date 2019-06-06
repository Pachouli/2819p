/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提相关宏定义，枚举类型，结构体定义等，为蓝牙推歌前台和引擎共享。
 * 作者：cailizhen
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
    /*! 录音状态 */
    record_status_e record_sta;
} btcall_info_t;

//PLAY参数
typedef struct
{
    asqt_configs_t *p_asqt_config;
    asqt_dump_buf_t *p_asqt_dump_buf;
    bool oncall;
    bool att_flag;//自动化测试标志，自动化测试时关闭AEC
} btcall_speech_play_para_t;

#endif //__BTCALL_COMMON_H__
