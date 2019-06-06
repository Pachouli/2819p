/********************************************************************************
 *                              US280A
 *                            Module: USB device
 *                 Copyright(c) 2001-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>            <version >             <desc>
 *      dengtaiping 2013-01-18 16:40     1.0                build this file
 ********************************************************************************/
#ifndef __UDISK_INTERFACE_H__
#define __UDISK_INTERFACE_H__

#include "typeext.h"
#include "card.h"
#include "hal_interface.h"

typedef struct
{
    uint8 install_card_flag;
    uint8 card_exist_flag; //卡读写标志;0-非读写状态;1-读状态;2-写状态
    uint8 card_rw_flag;
    uint8 usb_state;
    uint8 SwitchToAdfuFlag;
} udisk_status_t;

/*
 STUB 向上层提供多个函数指针, 以数组形式存放起来, 以下是这些函数指针地址
 */
#define UDISK_INTERFACE_ENTRY_ADDR(x)     (HAL_USB_ENTRY_VA_START + (4 * x))

#define UDISK_GET_STATUS_ENTRY            (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(0))))
#define UDISK_CARD_CAP_CHECK_ENTRY        (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(1))))
#define UDISK_DEAL_CARD_OUT_ENTRY         (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(2))))
#define UDISK_SWITCH_TO_ADFU_ENTRY        (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(3))))
#define UDISK_SET_USB_DATA_ENTRY  		  (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(4))))
#define UDISK_DEVICE_INIT_ENTRY           (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(5))))
#define UDISK_DEVICE_EXIT_ENTRY           (*((uint32*)(UDISK_INTERFACE_ENTRY_ADDR(6))))

#define udisk_get_status(a)               ((void (*)(udisk_status_t*))UDISK_GET_STATUS_ENTRY)((a))
#define udisk_card_cap_check()            ((void (*)(void))UDISK_CARD_CAP_CHECK_ENTRY)()
#define udisk_deal_card_out()             ((void (*)(void))UDISK_DEAL_CARD_OUT_ENTRY)()
#define udisk_switch_to_adfu()            ((void (*)(void))UDISK_SWITCH_TO_ADFU_ENTRY)()
#define udisk_set_usb_data(a,b)     	  ((void (*)(uint8,uint8*))UDISK_SET_USB_DATA_ENTRY)(a,b)
#define udisk_device_init()               ((void (*)(void))UDISK_DEVICE_INIT_ENTRY)()
#define udisk_device_exit()               ((void (*)(void))UDISK_DEVICE_EXIT_ENTRY)()

#endif  //__UDISK_INTERFACE_H__
