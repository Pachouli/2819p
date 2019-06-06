/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：RTC驱动相关宏定义，枚举类型，结构体定义等
 * 作者：wangli
 ********************************************************************************/

#ifndef _EXT_RTC_INTERFACE_H_
#define _EXT_RTC_INTERFACE_H_

#include "psp_includes.h"
#include "case_independent.h"

typedef void (*ext_rtc_alarm_isr)(void);

/* FM 驱动命令枚举类型*/
typedef enum
{
    EXT_RTC_MODULE_INSTALL = 0,
    EXT_RTC_MODULE_UNINSTALL,
    /*RTC模组初始化*/
    EXT_RTC_INIT,

    EXT_RTC_SET_TIME,
    EXT_RTC_GET_TIME,
    EXT_RTC_SET_DATE,
    EXT_RTC_GET_DATE,

    EXT_RTC_SET_ALARMTIME,
    EXT_RTC_GET_ALARMTIME,
    EXT_RTC_ENABLE_ALARM,
    EXT_RTC_DISABLE_ALARM,

    EXT_RTC_GET_STATUS,
} ext_rtc_cmd_e;

/* rtc 驱动向上层提供的统一入口定义*/
typedef void *(*ext_rtc_op_func)(void *, void *, void *);

/* 驱动内部实现的具体接口函数声明*/
typedef struct
{
    ext_rtc_op_func RtcModuleInstall;
    ext_rtc_op_func RtcModuleUninstall;
    ext_rtc_op_func Init;
    ext_rtc_op_func SetTime;
    ext_rtc_op_func GetTime;
    ext_rtc_op_func SetDate;
    ext_rtc_op_func GetDate;
    ext_rtc_op_func SetAlarmTime;
    ext_rtc_op_func GetAlarmTime;
    ext_rtc_op_func EnableAlarm;
    ext_rtc_op_func DisableAlarm;
    ext_rtc_op_func Get_Status;
} ext_rtc_driver_operations;

#if (SUPPORT_EXT_RTC == 1)

#define RTC_ENTRY_ADDR(x)           (RTC_ENTRY_VA_START + (4 * x))
#define RTC_MODULE_INSTALL_ENTRY    (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_MODULE_INSTALL))))
#define RTC_MODULE_UNINSTALL_ENTRY  (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_MODULE_UNINSTALL))))
#define RTC_INIT_ENTRY              (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_INIT))))
#define RTC_SET_TIME_ENTRY          (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_SET_TIME))))
#define RTC_GET_TIME_ENTRY          (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_GET_TIME))))
#define RTC_SET_DATE_ENTRY          (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_SET_DATE))))
#define RTC_GET_DATE_ENTRY          (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_GET_DATE))))

#define RTC_SET_ALARMTIME_ENTRY     (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_SET_ALARMTIME))))
#define RTC_GET_ALARMTIME_ENTRY     (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_GET_ALARMTIME))))
#define RTC_ENABLE_ALARM_ENTRY      (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_ENABLE_ALARM))))
#define RTC_DISABLE_ALARM_ENTRY     (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_DISABLE_ALARM))))
#define RTC_GET_STATUS_ENTRY        (*((uint32*)(RTC_ENTRY_ADDR(EXT_RTC_GET_STATUS))))


#if 0
extern int32 rtcdrv_init(void);
#endif
#define rtc_module_install()        ((int32(*)(void))RTC_MODULE_INSTALL_ENTRY)()

#if 0
extern int32 rtcdrv_exit(void);
#endif
#define rtc_module_uninstall()      ((int32(*)(void))RTC_MODULE_UNINSTALL_ENTRY)()

#if 0
extern void sRTC_Init(void);
#endif
#define rtc_phy_init()              ((void(*)(void))RTC_INIT_ENTRY)()

#if 0
extern void sRTC_set_Time(time_t* time);
#endif
#define rtc_set_time(a)             ((void(*)(time_t*))RTC_SET_TIME_ENTRY)((a))

#if 0
extern void sRTC_get_Time(time_t* time)
#endif
#define rtc_get_time(a)             ((void(*)(time_t*))RTC_GET_TIME_ENTRY)((a))

#if 0
extern void sRTC_set_Date(date_t* date);
#endif
#define rtc_set_date(a)             ((void(*)(date_t*))RTC_SET_DATE_ENTRY)((a))

#if 0
extern void sRTC_get_Date(date_t* date)

#endif
#define rtc_get_date(a)             ((void(*)(date_t*))RTC_GET_DATE_ENTRY)((a))

#if 0
extern void sRTC_set_AlarmTime(time_t* time)
#endif
#define rtc_set_alarm_time(a)       ((void(*)(time_t*))RTC_SET_ALARMTIME_ENTRY)((a))

#if 0
extern void sRTC_get_AlarmTime(time_t* time)
#endif
#define rtc_get_alarm_time(a)       ((void(*)(time_t*))RTC_GET_ALARMTIME_ENTRY)((a))

#if 0
extern void sRTC_enable_Alarm(ext_rtc_alarm_isr alarm_isr);
#endif
#define rtc_enable_alarm(a)         ((void(*)(ext_rtc_alarm_isr))RTC_ENABLE_ALARM_ENTRY)((a))

#if 0
extern void sRTC_disable_Alarm(void);
#endif
#define rtc_disable_alarm()         ((void(*)(void))RTC_DISABLE_ALARM_ENTRY)()

#if 0
extern void sRTC_get_status(rtc_status_t *p_rtc_status)
#endif
#define rtc_get_status(a)           ((void(*)(rtc_status_t *))RTC_GET_STATUS_ENTRY)((a))


#endif

#endif

