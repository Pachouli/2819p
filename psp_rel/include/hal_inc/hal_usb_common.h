
#ifndef __HAL_USB_COMMON_H__
#define __HAL_USB_COMMON_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"

//private data

typedef enum
{
	USBC_MODE_NULL   = 0,
	USBC_MODE_DEVICE = 1,
	USBC_MODE_UHOST  = 2,
} usb_controller_mode_e;

extern usb_controller_mode_e g_usb_controller_mode;

//private defines

extern bool hal_usb_is_cable_plug(void);
extern bool hal_uhost_is_device_plug(void);

extern int32 hal_usb_receive_data(uint32 address, uint32 length, bool need_clear_busy, uint32 timeout_ms);
extern int32 hal_usb_send_data(uint32 address, uint32 length, uint32 pre_fill_bytes, uint32 timeout_ms);
extern int32 hal_usb_send_data_ep2(uint32 address, uint32 length, uint32 pre_fill_bytes, uint32 timeout_ms);

#endif
