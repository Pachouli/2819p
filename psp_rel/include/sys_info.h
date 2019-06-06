/********************************************************************************
 *							   USDK(GL5110)
 *							 Module: KERNEL
 *				  Copyright(c) 2003-2011 Actions Semiconductor,
 *							 All Rights Reserved.
 *
 * History:
 *	   <author> 	 <time> 		  <version >			 <desc>
 *	   wuyueqian	 2011-9-10 09:00	 1.0			 build this file
 ********************************************************************************/
/*!
 * \file 	sys_info.h
 * \brief	sys_info.bin
 * \author	wuyueqian
 * \version	1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __SYS_INFO_H__
#define __SYS_INFO_H__

#include <typeext.h>
#include <ucos/init.h>

#define DIR_COUNT      126

typedef struct
{
    uint8 filename[11]; /* �ļ���8.3 */
    uint8 type; /* ��A��,��B��,��H��,��F��, ��S��, ��U��, ��I�� */
    uint32 DownloadAddr; /* ���صĵ�ַ */
    uint32 offset; /* ���ļ��е�ƫ�ƣ�byteΪ��λ */
    uint32 length; /* ����byteΪ��λ��512ȡ����*/
    uint32 Subtype; /* ������ */
    uint32 checksum; /* DWORD�ۼ� */
} AFI_DIR_t;

typedef struct
{
    uint8 afi_flag[4];
    uint16 reserved0;
    uint16 reserved1;
    uint32 version;
    uint32 date;
    uint32 dLength; //afi���ܳ��ȣ��ֽ�Ϊ��λ
    uint32 dVersion; //afi�İ汾����CFG�ļ�ָ��
    uint8 reserved2[8];
    AFI_DIR_t afi_dirs[DIR_COUNT];
    uint8 reserved3[28];
    uint32 checksum; /* afi headУ��ͣ�DWORD�ۼ� */
} AFI_Head_t; /* 4kB */

typedef struct
{
    uint32 SD_Capacity;
    uint32 VM_Capacity;
    uint32 udisk_addr;
    uint32 udisk_cap;
    uint32 hide_disk_cap;
    uint32 auto_disk_cap;
    uint32 Reserved[2];
} CapInfo_t;

typedef struct
{
    uint32 LFIFlag;
    uint8 sdk_version[4];
    uint8 version[4];
    uint16 VID;
    uint16 PID;
    uint32 DIRItemCheckSum;

    //20�ֽ�ƫ��
    CapInfo_t capinfo; //32byte
    uint8 reserve0[16];

    //68�ֽ�����LAP��ַ
    uint8 bt_lap[3];

    //71�ֽ�ƫ��
    uint8 save_bt_flag;

    //72�ֽ�ƫ��
    uint8 reserve1[7];

    //79�ֽ�ƫ��
    uint8 udisk_setting;

    //80�ֽ�ƫ��, usb sn
    uint8 usb_setup_info[48];

    //128�ֽ�ƫ��
    uint8 sdk_description[336];

    //464�ֽ�ƫ��
    uint8 reserve2[46];

    uint16 Headchecksum;
} LFIHead_t;

typedef struct
{
    uint8 bLength; //�˽ṹ�ĳ��ȣ�����Ϊ��λ��ĿǰΪ0x1
    uint8 bType; //���ݽṹ���ͣ�ĿǰΪ0x7
    uint8 Reserved1[14];
    uint32 dFwuChecksum; // fwu��У��ͣ�������������
    uint32 dFlag; //�ṹ����ֵ��־��ĿǰΪ0x55aa55aa
    uint8 pbDescriptor[8]; //�ṹ���ַ�����־��ĿǰΪ��FwuTail��
    uint8 pbFwuCrcChecksum[32]; //fwu��crcУ��ͣ�������������
    uint8 Reserved2[444];
    uint32 dFwuTailChecksum; //�˽ṹ��У��ͣ�ĿǰΪǰ508bytes��У���
} FwuTail_t;

#define FW_INFO_VERSION_ADDR           4
#define FW_INFO_VERSION_LEN            8
#define FW_INFO_USB_VIDPID_ADDR        12
#define FW_INFO_USB_VIDPID_LEN         8
#define FW_INFO_CAPINFO_ADDR           40
#define FW_INFO_CAPINFO_LEN            16
//#define FW_INFO_USB_VENDOR_ADDR        256
//#define FW_INFO_USB_VENDOR_LEN         8
#define FW_INFO_USB_ID_ADDR            80
#define FW_INFO_USB_ID_LEN             48
#define FW_INFO_USB_SN_TYPE_ADDR       79
#define FW_INFO_USB_SN_TYPE_LEN        1
#define FW_INFO_BT_LAP_ADDR            68
#define FW_INFO_BT_LAP_LEN             3
#define FW_INFO_BT_SAVE_DATA           71
#define FW_INFO_BT_SAVE_DATA_LEN       1
#endif




