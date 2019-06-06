/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开关机应用相关宏定义，枚举类型，结构体定义等，为所有应用共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __CONFIG_COMMON_H__
#define __CONFIG_COMMON_H__

#include "psp_includes.h"

/* 开机应用模式 */
#define CONFIG_MODE_POWER_ON     0 //开机，开机触发原因向系统获取
#define CONFIG_MODE_POWER_OFF    1 //按键关机或省电关机
#define CONFIG_MODE_LOWPOWER     2 //低电关机
#define CONFIG_MODE_ALARM_OFF    3 //闹钟响应完自动关机
#define CONFIG_MODE_CHARGE_LOOP  4 //低功耗模式USB唤醒进入空闲充电模式
#define CONFIG_MODE_JUST_OFF     5 //低功耗模式电量不足直接关机，不播报TTS

#endif //__CONFIG_COMMON_H__
