#ifndef _HID_DEF_
#define _HID_DEF_

#define HID_HANDSHAKE			0
#define HID_CONTROL				1
#define HID_GET_REPORT			4
#define HID_SET_REPORT			5
#define HID_GET_PROTOCAL		6
#define HID_SET_PROTOCAL		7
#define HID_GET_IDLE			8
#define HID_SET_IDLE			9
#define HID_DATA				10
#define HID_DATC				11

typedef enum
{
    Hid_success = 0,
    Hid_busy = 1,
    Hid_invalid_id = 2,
    Hid_unsupported = 3,
    Hid_invalid_param = 4,
    Hid_failed = 14,
    Hid_timeout = 15,
    Hid_resource_error = 16,
} HID_HANDSHAKE_MESSAGE;

typedef enum
{
    Hid_control_op_nop = 0,
    Hid_control_op_hard_reset = 1,
    Hid_control_op_soft_reset = 2,
    Hid_control_op_suspend = 3,
    Hid_control_op_exit_suspend = 4,
    Hid_control_op_unplug = 5,

} HID_CONTROL_OP;

#endif // _HID_DEF_
