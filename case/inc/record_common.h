/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：本地播歌相关宏定义，枚举类型，结构体定义等，为本地播歌前台和引擎共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __RECORD_COMMON_H__
#define __RECORD_COMMON_H__

#include "psp_includes.h"
#include "record_module_extern.h"
#include "enhanced.h"

//RECORD前台进入模式
#define PARAM_RECORD_CARD_DIR     0 //卡盘目录录音
#define PARAM_RECORD_UHOST_DIR    1 //U盘目录录音
#define PARAM_RECORD_RECOVER      2 //录音文件恢复

typedef enum
{
    RECORD_STA_STOP = 0,
    RECORD_STA_RECORDING,
    RECORD_STA_PAUSE,
    RECORD_STA_ERROR,
} record_status_e;

#endif //__RECORD_COMMON_H__
