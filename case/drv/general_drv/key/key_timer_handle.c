/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动硬件定时器接口，包括按键扫描、电池电量检测。
 * 作者：cailizhen
 ********************************************************************************/

#include "key_inner.h"

void key_inner_timer_handle(void)
{
    //按键扫描
    key_inner_key_scan();

    //电池电量采样
    key_sampling_battery_voltage();
}
