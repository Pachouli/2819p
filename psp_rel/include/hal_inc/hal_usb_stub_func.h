#ifndef __HAL_USB_STUB_FUNC_H__
#define __HAL_USB_STUB_FUNC_H__


#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_usb_interface.h"
#include "usb_hardware_extern.h"

#define  EP_IN    (0)
#define  EP_OUT   (1)
#define  R_BYTE   (0)
#define  R_WORD   (1)
extern void hal_usb_short_packet_set(uint32 mode);
extern void hal_usb_read_fifo1(uint8 mode,void *pdata);
extern int hal_usb_fill_print_head(uint32*cmd, UINT32 *head_info, uint32 head_info_size);
extern uint8 hal_get_usb_ep1_count(void);
extern bool hal_usb_ep1_is_busy(uint8 ep_dir);

#endif
