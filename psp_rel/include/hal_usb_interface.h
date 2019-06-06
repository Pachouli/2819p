
#ifndef __HAL_USB_INTERFACE_H__
#define __HAL_USB_INTERFACE_H__

#include "typeext.h"

//define ep num
#define EP0                         (0)
#define EP1IN                       (1)
#define EP1OUT                      (2)
#define EP2IN                       (3)
#define EP2OUT                      (4)

#define HD_INIT_FOR_INSTALL_DRV   0
#define HD_INIT_FOR_USB_DETECT    1

typedef struct
{
    uint16 usb_isr_en : 1; //标识符，表示是否开启USB中断
    uint16 short_pk_isr_en : 1;
    uint16 setup_datavalid_isr_en : 1;
    uint16 suspend_isr_en : 1;
    uint16 reset_isr_en : 1;
    uint16 disconnect_isr_en : 1;
    uint16 iso_out_token_isr_en : 1;
    uint16 iso_in_token_isr_en : 1;

    uint32 short_pk_isr_cbk;
    uint32 setup_datavalid_isr_cbk;
    uint32 suspend_isr_cbk;
    uint32 reset_isr_cbk;
    uint32 disconnect_isr_cbk;
    uint32 iso_out_token_isr_cbk;
    uint32 iso_in_token_isr_cbk;
} usb_isr_setting_t;

extern void audio_usb_hardware_init(usb_isr_setting_t *p_usb_isr_sett);
extern void audio_usb_hardware_exit(void);
extern int32 hal_set_usb_detect_inner(bool enable);
extern int32 hal_usb_detect_init_inner(void);
extern int32 hal_usb_detect_inner(void);

#endif  //__HAL_USB_INTERFACE_H__
