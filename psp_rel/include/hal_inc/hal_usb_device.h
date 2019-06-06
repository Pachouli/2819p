
#ifndef __HAL_USB_DEVICE_H__
#define __HAL_USB_DEVICE_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_usb_common.h"
#include "hal_usb_interface.h"
#include "usb_hardware_extern.h"

//private defines
typedef struct
{
    uint8 bmRequestType;
    uint8 bRequest;
    uint16 wValue;
    uint16 wIndex;
    uint16 wLength;
} device_requests_t;

typedef struct
{
    uint32 _dCBWSignature;          //命令块组件(Command Block Wrapper)标志，值为0x43425355
    uint32 _dCBWTag;                //CBW 标签，由host发出，设备回复是改组将该值填到CSW的_dCSWTag中。
    uint32 _dCBWDataTransferLength; //后面需要接受或发送的数据长度
    uint8 _bmCBWFlags;              //该值的bit7指示传输方向；bit7=0,表示host to device;bit7=1,表示device to host
    uint8 _bCBWLUN;                 //device Logical Unit Number
    uint8 _bCBWCBLength;            //标识后面跟着的CBWCB的数据长度
    uint8 _CBWCB[9];
} bulkcbw;


extern void hal_usb_controller_reset(void);
extern void hal_usb_phy_init(usb_isr_setting_t *p_usb_isr_sett,uint8 mode);
extern void hal_usb_phy_exit(void);
extern void hal_usb_switch_to_device_mode(void);

extern void hal_usb_set_in_ep(uint8 transfertype,uint8 enpnum,uint16 maxpck,uint32 staddr,uint8 buftype);
extern void hal_usb_set_in_ep_hc(uint8 transfertype,uint8 enpnum,uint16 maxpck,uint32 staddr,uint8 buftype);
extern void hal_usb_set_out_ep(uint8 transfertype,uint16 maxpck,uint32 staddr,uint8 buftype);
extern void hal_usb_set_out_ep_hc(uint8 transfertype,uint16 maxpck,uint32 staddr,uint8 buftype);
extern void hal_usb_stall_ep(uint8 ep_num);
extern void hal_usb_clear_stall_ep(uint8 ep_num);
extern void hal_usb_rest_ep_toggle(uint8 enpnum);
extern int32 hal_usb_send_ctrl_data(uint8 *d_addr, uint8 actul_dlen, uint8 rqst_dlen);
extern int32 hal_usb_recv_ctrl_data(uint8 *d_addr, uint8 actul_dlen, uint8 rqst_dlen);
extern void hal_usb_nak_ctrl(void);

extern void hal_usb_set_isr(usb_isr_setting_t *p_usb_isr_sett);
extern void hal_usb_reset_isr(void);

extern void hal_usb_boot_detect_init(void);
extern bool hal_usb_is_cable_plug(void);

extern void hal_get_usb_setup_data(device_requests_t* dat);
extern void hal_usb_to_adfu(void);
extern uint8 hal_usb_connect_state(uint8 mode);
extern void hal_usb_hid_pro(uint8* p_hid_change,uint16* p_idle_time,uint8* p_hid_value);
extern void hal_usb_get_short_pk_data(bulkcbw* pdata);

#endif
