#ifndef __USB_HARDWARE_EXTERN_H__
#define __USB_HARDWARE_EXTERN_H__

#include "hal_usb_interface.h"

//isr_source
#define  SHORT_PK_ISR             0
#define  SETUP_DATAVALID_ISR      1
#define  SUSPEND_ISR              2
#define  RESET_ISR                3
#define  DISCONNECT_ISR           4

#define SET_USB_DISCONNECTTED       (0)
#define SET_USB_CONNECTTED          (1)
#define GET_USB_CONNECT_STATE       (2)


#endif
