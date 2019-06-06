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
#ifndef __USOUND_EXTERN_H__
#define __USOUND_EXTERN_H__

#define USB_IDLE         0x00
#define PLAY_RUNNING     0x01
#define RECORD_RUNNING   0x02
#define USOUND_PLUG_OUT  0x04

typedef enum
{
    DEVICE_VID = 0,
    DEVICE_PID,
    DEVICE_MANUFNAME,
    DEVICE_PRODNAME,
    DEVICE_FOB_INFO,
    DEVICE_VOLUME_FLAG
} device_info_e;

extern uint8 usb_state;
extern uint8 card_detect_state;
extern uint8 install_card_flag;
extern uint32 timer_id;
extern uint8 SwitchToAdfuFlag;
extern uint8 UsbIdleCounter;
extern uint8 UpOrDownLoad;
extern uint8 usb_reset_count;
extern uint8 usb_state;
extern uint8 suspend_flag;
extern uint8 hid_report;
extern uint8 card_exist_flag;

extern void msc_cmd_process(void)__FAR__;
extern void set_auip_vol(uint8 set_type)__FAR__;
extern void deal_card_out(void)__FAR__;
extern void set_usb_data(uint8 index,uint8 *info_p);

#endif
