/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：rtc驱动内部头文件。
 * 作者：wangli
 ********************************************************************************/

#ifndef _EXT_RTC_INNER_H
#define _EXT_RTC_INNER_H

#include "psp_includes.h"
#include "ext_rtc_interface.h"

#if (SUPPORT_EXT_RTC == 1)

extern uint8 rtc_twi_master_idx;
extern ext_rtc_alarm_isr g_ext_rtc_alram_isr;
extern uint8 rtc_inited_flag;

/*rtc 驱动对外接口函数声明*/
extern void sRTC_Init(void);

extern uint8 sRTC_Check(void);

extern void sRTC_set_Time(time_t* time);

extern void sRTC_get_Time(time_t* time);

extern void sRTC_set_Date(date_t* date);

extern void sRTC_get_Date(date_t* date);

extern void sRTC_get_AlarmTime(time_t* time);

extern void sRTC_set_AlarmTime(time_t* time);

extern void sRTC_enable_Alarm(ext_rtc_alarm_isr alarm_isr);

extern void sRTC_disable_Alarm(void);

extern void sRTC_get_status(rtc_status_t *p_rtc_status);

extern bool sRTC_get_alarm_flag(void);

extern uint8 sRTC_get_rtc_int_level(void);

extern int32 rtcdrv_init(void);
extern int32 rtcdrv_exit(void);

#endif

#endif /*_EXT_RTC_INNER_H */

