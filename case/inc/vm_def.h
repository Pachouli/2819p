/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ü���Ϣͨ����غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _VM_DEF_H
#define _VM_DEF_H

/* AP VRAM ���ݣ�0x00000000 - 0x001f0000��֧��32��VARM ID��ÿ��VRAM�ռ��Сû�й涨 */
/* !!! 0x00200000 - 0x003f0000�Ŀռ䱻�ײ�ģ��ʹ�ã���Ӧ�ò�Ҫʹ����οռ� */
/* AP VRAM ��������־ = 0x55AA + ID (0x00 - 0x1f) */

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
#define  VM_BT_PLIST0_HID       0x00130000 //HID�����豸�̶�����Ϊֻ֧��1���������޸�
#define  VM_BT_TWS_INFO         0x00140000
#define  VM_AP_RECORD_UHOST_FS    0x00150000
#define  VM_AP_RECORD_UHOST_FILE  0x00160000
#define  VM_AP_RECORD_CARD_FS     0x00170000
#define  VM_AP_RECORD_CARD_FILE   0x00180000
#define  VM_AP_RADIO            0x00190000

#endif
