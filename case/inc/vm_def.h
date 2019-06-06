/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用间消息通信相关宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _VM_DEF_H
#define _VM_DEF_H

/* AP VRAM 数据，0x00000000 - 0x001f0000，支持32个VARM ID，每个VRAM空间大小没有规定 */
/* !!! 0x00200000 - 0x003f0000的空间被底层模块使用，请应用不要使用这段空间 */
/* AP VRAM 数据区标志 = 0x55AA + ID (0x00 - 0x1f) */

#define  VM_BTENGINE            0x00000000
#define  VM_AP_ENV_VAR          0x00010000
#define  VM_AP_ALARMINFO        0x00020000
#define  VM_AP_CONFIG           0x00030000
#define  VM_AP_MUSIC            0x00040000
#define  VM_UHOST_BKINFO        0x00050000
#define  VM_CARD_BKINFO         0x00060000
#define  VM_RES_BKINFO          0x00070000
#define  VM_BT_DEV_CFG          0x00080000
#define  VM_BT_PLIST0           0x00090000
#define  VM_BT_PLIST1           0x000a0000
#define  VM_BT_PLIST2           0x000b0000
#define  VM_BT_PLIST3           0x000c0000
#define  VM_BT_PLIST4           0x000d0000
#define  VM_BT_PLIST5           0x000e0000
#define  VM_BT_PLIST6           0x000f0000
#define  VM_BT_PLIST7           0x00100000
#define  VM_BTENGINE_HID        0x00110000
#define  VM_BT_DEV_CFG_HID      0x00120000
#define  VM_BT_PLIST0_HID       0x00130000 //HID拍照设备固定配置为只支持1个，可以修改
#define  VM_BT_TWS_INFO         0x00140000
#define  VM_AP_RECORD_UHOST_FS    0x00150000
#define  VM_AP_RECORD_UHOST_FILE  0x00160000
#define  VM_AP_RECORD_CARD_FS     0x00170000
#define  VM_AP_RECORD_CARD_FILE   0x00180000
#define  VM_AP_RADIO            0x00190000

#endif
