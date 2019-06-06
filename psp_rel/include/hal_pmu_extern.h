
#ifndef __HAL_PMU_EXTERN_H__
#define __HAL_PMU_EXTERN_H__

#include <attr.h>
#include <typeext.h>
#include <bat_adc_def.h>
#include <temp_adc_def.h>
#include <lradc_adc_def.h>

//public defines

#define ONOFF_KEY_LPT_125MS  0
#define ONOFF_KEY_LPT_250MS  1
#define ONOFF_KEY_LPT_500MS  2
#define ONOFF_KEY_LPT_1S     3
#define ONOFF_KEY_LPT_1S5    4
#define ONOFF_KEY_LPT_2S     5
#define ONOFF_KEY_LPT_3S     6
#define ONOFF_KEY_LPT_4S     7

//唤醒结果
#define WAKE_NOSLEEP       (0)
#define WAKE_BY_KEY        (1)
#define WAKE_BY_BT         (2)
#define WAKE_BY_LOWPOWER   (3)
#define WAKE_BY_USBDC5V    (4)
#define WAKE_BY_RTCALARM   (5)
#define WAKE_BY_HIGHFREQ   (6) //主动临时切换到高频检测LRADC、IR、卡拔插等

#define WIO_WAKE_DIR_HIGH  0 //高电平
#define WIO_WAKE_DIR_LOW   1 //低电平
#define WIO_WAKE_DIR_RISE  2 //上升沿
#define WIO_WAKE_DIR_FAIL  3 //下降沿

typedef struct
{
    uint8 onoff_short_en : 1;
    uint8 onoff_long_en : 1;
    uint8 alarm_en : 1;
    uint8 wio0_en : 1;
    uint8 wio1_en : 1;
    uint8 onoff_long_time;
    uint8 wio0_dir : 2;
    uint8 wio1_dir : 2;
} wakeup_en_para_t;

typedef struct
{
    uint8 batin : 1; //电池接入
    uint8 reset : 1; //按reset按键reset 或 长按ONOFF 8S reset
    uint8 onoff : 1; //软关机状态下短按ONOFF抬起或长按ONOFF
    uint8 wdog  : 1; //看门狗溢出唤醒，一般是ESD导致代码跑飞异常reset
    uint8 alarm : 1; //闹钟唤醒
    uint8 wio0  : 1; //wake up io唤醒，比如USB DC5V接入唤醒
    uint8 wio1  : 1; //wake up io唤醒，比如USB DC5V接入唤醒
} wakeup_para_t;

typedef struct
{
    uint32 wake_pd;
    wakeup_para_t wakeup_para;
} sys_poweron_info_t;

#endif
