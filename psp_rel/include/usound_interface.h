/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：
 ********************************************************************************/
#ifndef __USOUND_INTERFACE_H__
#define __USOUND_INTERFACE_H__

#include "typeext.h"

//for comunication
typedef struct
{
    uint8 set_sample;//sample flag
    uint8 set_adc_sample;//adc sample flag
    uint8 set_dac_sample;//dac sample flag
    uint16 line_sta;//if is insert
    uint16 switch_flag;//if need switch_to stub
    int16 sound_energy;//usound 能量
    uint32 sample_rate;//rate
    uint32 adc_sample_rate;//adc rate
    uint32 dac_sample_rate;//dac rate
    uint32 start_play;//if is start play
    uint32 start_record;//if is start record
    uint32 volume_chg;//if voluem syn
    uint32 volume_value;//syn volume value
    uint32 volume_max;
    uint32 volume_min;
    uint32 hid_idle_time;//hid idle time
    uint32 uspeaker24_energy;
    uint32 start_cnt;
    uint32 usb_set_ok;
    int16 max_energy;
} usound_status_t;

//for comunication
typedef struct
{
    uint32 line_sta;//if is insert
} uhid_status_t;

//for commond set
typedef enum
{
    SET_HID_OPERS = 0, //cmd use
    SET_SAMPLE_RATE, //sample set
    SET_ADJUST_TIMER, //adjust
    SET_LINE_STATUS,//status
    SET_VOLUME_FLAG,//volume syn flag
    SET_HID_CHANGE,//hid need report
    SET_FILTER_FLAG,
    SET_USPEAKER_24BIT,
    FIX_SAMPLE_RATE,
    SET_VOLUME,
} usound_set_cmd_e;

/*
 STUB 向上层提供多个函数指针, 以数组形式存放起来, 以下是这些函数指针地址
 */
#define USOUND_INTERFACE_ENTRY_ADDR(x)     (HAL_USOUND_ENTRY_VA_START + (4 * x))

#define USOUND_GET_STATUS					(*((uint32*)(USOUND_INTERFACE_ENTRY_ADDR(0))))
#define USOUND_SET_USB_CFG            		(*((uint32*)(USOUND_INTERFACE_ENTRY_ADDR(1))))
#define USOUND_SET_CMD        				(*((uint32*)(USOUND_INTERFACE_ENTRY_ADDR(2))))
#define USOUND_START						(*((uint32*)(USOUND_INTERFACE_ENTRY_ADDR(3))))
#define USOUND_STOP							(*((uint32*)(USOUND_INTERFACE_ENTRY_ADDR(4))))
#define USOUND_TIMER_DEAL					(*((uint32*)(USOUND_INTERFACE_ENTRY_ADDR(5))))

#define usound_get_status(a)             	((void (*)(usound_status_t *))USOUND_GET_STATUS)((a))
#define usound_set_usb_cfg(a,b)             ((void (*)(uint8,uint8*))USOUND_SET_USB_CFG)((a),(b))
#define usound_set_cmd(a,b)            		((bool (*)(uint32,uint32))USOUND_SET_CMD)((a),(b))
#define usb_sound_start()             		((void (*)(void))USOUND_START)()
#define usb_sound_stop()					((void (*)(void))USOUND_STOP)()
#define usound_timer_deal()					((void (*)(void))USOUND_TIMER_DEAL)()

#endif
