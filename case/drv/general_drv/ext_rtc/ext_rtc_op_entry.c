/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM驱动外部接口列表。
 * 作者：cailizhen
 ********************************************************************************/

#include "ext_rtc_inner.h"

/*RTC 驱动对外接口函数*/
const ext_rtc_driver_operations ext_rtc_drv_op =
{
#if (SUPPORT_EXT_RTC == 1)
    (ext_rtc_op_func) rtcdrv_init, //KEY驱动中的RTC模块装载
    (ext_rtc_op_func) rtcdrv_exit, //KEY驱动中的RTC模块卸载
    (ext_rtc_op_func) sRTC_Init,  //初始化

    (ext_rtc_op_func) sRTC_set_Time, //设置时间
    (ext_rtc_op_func) sRTC_get_Time, //获取时间
    (ext_rtc_op_func) sRTC_set_Date, //设置日期
    (ext_rtc_op_func) sRTC_get_Date,      //获取日期
    (ext_rtc_op_func) sRTC_set_AlarmTime,  //设置闹钟
    (ext_rtc_op_func) sRTC_get_AlarmTime,  //设置闹钟
    (ext_rtc_op_func) sRTC_enable_Alarm,    //使能闹钟
    (ext_rtc_op_func) sRTC_disable_Alarm,    //关闭闹钟
    (ext_rtc_op_func) sRTC_get_status,    //获取时钟/闹钟状态
#else
    0,
#endif
};

