
#ifndef __HAL_USB_UHOST_H__
#define __HAL_USB_UHOST_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_usb_common.h"

#define    BULK_IN_EP_NUM           (1) // BULK IN 端点号,
#define    BULK_OUT_EP_NUM          (2) // BULK OUT 端点号

#define    HOST_BULK_IN_START       (0)
#define    HOST_BULK_IN_STOP        (1)

#define    HOST_BULK_IN             (0)
#define    HOST_BULK_OUT            (1)
#define    HOST_BULK_END            (2)
#define    TOGGLE_BIT_CLEAR         (0)
#define    TOGGLE_BIT_SET           (1)

#define ENUMERATION_MAX_NUMBER         35/*大部分U盘1次就可以了，个别U盘需要超过20次才能枚举成功*/
#define TIME_FOR_WAIT_BUS_RESET        0x05  /* not less than 5ms */
#define TIME_FOR_SETADDRESS            0x40   /* not less than 48ms */
#define TIME_FOR_GETMAXLUN             0xb0   /* not less than 80ms */
#define READ_CAPACITY_NUMBER           0x03
#define COUNT_FOR_BUS_RESET            0x400
#define COUNT_FOR_CONTROL_OUT_STAGE    0x100 /*减小为原来的1/16 即2s*/
#define COUNT_FOR_CONTROL_IN_DATA      0x100 /*update 5106 bug fix*/
#define CONTROL_IN_DATA_MAX_LOOP       0x10
#define TEST_UNIT_READY_NUMBER         0x400
#define GENERAL_TEST_NUMBER            0x08
#define COUNT_FOR_BULK_DATA            0x50000
#define COUNT_FOR_RESEND               0x2000
#define TIME_FOR_INQUIRY               0x05
#define FSM_A_HOST                     0x03 /* The USB FSM State: A_host */
#define ERROR_DEVICE_NOT_IN            0x02
#define ERROR_BUS_RESET_OVERTIME       0x06
#define USB_DIR_OUT                    0x00            /* to device */
#define USB_DIR_IN                     ((uint8)0x80)   /* to host */
#define USB_DIR_ALL                    (0xff)          /*in and out*/

// -------------------Error associated macros---------------------------------
#ifdef GENERAL_OK
#undef GENERAL_OK
#endif
#define GENERAL_OK                     0x00
#define GENERAL_ERROR                  0x01
#define ERROR_DEVICE_NOT_IN            0x02
#define ERROR_NOT_SUPPORT_DEVICE       0x03
#define ERROR_NOT_SUPPORT_PROTOCOL     0x04
#define ERROR_IN_DATA_TOO_LONG         0x05
#define ERROR_BUS_RESET_OVERTIME       0x06
#define ERROR_CONFIG_DES_TOO_BIG       0x07
#define ERROR_IRQ_REGISTER             0x08
#define ERROR_HIGH_SPEED               0x09 // high speed hankshake fail
#define ERROR_MAX_PCK_SIZE             0x0A // max_packet_size of bulk in/out is 0x00
#define ERROR_ILLEGAL_CAPACITY         0x0B

#define ERROR_TRANSFER_BUSY            0x80
#define ERROR_IRQ_OCCUR                0x81
#define ERROR_EP_BUFFER_NOT_EMPTY      0x82  /* not busy, but some endpoint sub-buffers are not empty */
#define ERROR_OTG_STATE                0x83
#define ERROR_EP_STALL                 0x84
#define ERROR_NO_EP_HANDSHAKE          0x85
#define ERROR_PID                      0x86
#define ERROR_UNDERRUN                 0x87
#define ERR_TRANS_REDO                 0x88
#define ERROR_WAIT_TRANS_TIME_OUT      0x89

#define ERROR_PARAMETER_IN             0xF0
#define ERROR_MIDIUM_NOT_PRESENT       0xF1
#define ERROR_MIDIA_CHANGED            0xF2
#define ERROR_BULK_EP_NO_FOUND         0xF3

#define ERROR_READ_WRITE_MODE          0xF8
#define ERROR_NOT_INITED               0xF7
#define ERROR_CSW                      0xF9



//private defines
extern int32 hal_uhost_phy_init(void);
extern int32 hal_uhost_phy_exit(void);
extern void hal_uhost_switch_to_host_mode(void);
extern bool hal_uhost_is_device_plug(void);
extern void hal_host_bulk_control(uint8 bulk_type);
extern void hal_host_bulk_transfer_set(uint8 mode,uint8 packet_num);
extern uint8 hal_host_get_epnum(uint8 mode);
extern void hal_host_clr_irq_err(void);
extern void hal_host_set_ep0_toggle_bit(uint8 toggle_opr);
extern uint8 hal_host_get_transfer_errcode(uint8 mode);
extern void hal_host_set_function_address(uint8 address);
extern void hal_host_transfer_control_set(uint8 mode,uint8 value);
extern void hal_host_set_max_packet(uint8 max_packet);
extern uint8 read_data_from_usb(uint32* address, uint32 data_lenth,uint32 timeout);
extern uint8 write_data_to_usb(uint8* address, uint32 data_lenth,uint32 timeout);

#endif
