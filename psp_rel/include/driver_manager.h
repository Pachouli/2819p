/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     driver_manager.h
 * \brief    ��������
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _DRIVER_H
#define _DRIVER_H

#include <typeext.h>
#include <attr.h>
#include <ucos/os.h>
//#include <kernel_interface.h>

#ifndef _ASSEMBLER_
typedef struct
{
    //������:0-�޶��壻1-SD ����2-mmc 4.0x����;3mmc 4.0x����
    uint8 card_type : 2;
    //������:0-1 bit��1-4 bit��2-8 bit
    uint8 card_bus_width : 2;
    //��ʱ��:0-û�����óɸ���;1-�ɹ�����Ϊ����
    uint8 card_clock:2;
    //�����ʵ�Ԫ:0-byteΪ��λ��1-sectorΪ��Ԫ
    uint8 card_cap:2;
    //������д���:0-������д��1-����д
    uint8 card_continue_w:1;
    //�����������:0-����������1-������
    uint8 card_continue_r:1;
    //���Ƿ���ɳ�ʼ�����:0-δ��ɣ�1-�����
    uint8 card_had_init:1;

    uint8 reserved: 5;
    //����Ե�ַ
    uint16 card_rca;
    //�ļ�ϵͳһ������fat��Ĵ�С��������Ϊ��λ
    uint32 fs_scan_space;
    uint32 backdoor_flag;
    //����д�뿨�����⻥����
    os_event_t *card_mutex;
    //ud data buffer
    uint8 *ud_data_buffer;
}sys_global_info_t;




#define MODE_NORMAL 0
#define MODE_SPECIAL 1
typedef struct
{
    unsigned char file_type;        //��D��, driver
    unsigned char drv_type;         //driver type
    unsigned char Major_version;
    unsigned char minor_version;
    unsigned char magic[4];
    unsigned int sram_text_offset;  //sram text���ļ������ƫ��
    unsigned int sram_text_length;  //sram text����Ч����,Ϊ0��ʾû����Ҫ��SRAM���еĴ���
    unsigned int sram_text_addr;    //sram text����ʼ�߼���ַ,����ΪSRAM��ַ
    unsigned int data_offset;       //��һ��data�����ļ������ƫ��
    unsigned int data_length;       //��һ��data�γ���
    unsigned int data_addr;         //��һ��data���߼���ַ
    unsigned int data1_offset;      //�ڶ���data�����ļ������ƫ��
    unsigned int data1_length;      //�ڶ���data�γ���
    unsigned int data1_addr;        //�ڶ���data���߼���ַ
    unsigned int sram_text1_offset; //sram text1���ļ������ƫ��
    unsigned int sram_text1_length; //sram text1����Ч����,��������ʱ����load��ram��ȥ,��Ҫ����ϵͳ�ӿڼ���
    unsigned int sram_text1_addr;   //sram text1����ʼ�߼���ַ,����ΪSRAM��ַ
    unsigned int bss_length;        //bss�γ���
    unsigned int bss_addr;          //bss���߼���ַ
    unsigned int drv_op_entry;      //�����ӿ�������ʼ�߼���ַ
    unsigned int entry_table_len;   //�����ӿ����鳤��
    unsigned int text_offset;       //entry table��text code���ļ������ƫ��
    unsigned int text_addr;         //text����ʼ�߼���ַ,��link.xn����
    unsigned int drv_init_entry;    //������ʼ���ӿ��߼���ַ
    unsigned int drv_exit_entry;    //�����˳��ӿ��߼���ַ
} drv_head_t;

typedef struct
{
//    uint32 bank_a_offset;
//    uint32 bank_b_offset;
//    uint32 bank_c_offset;
    uint32 exit_entry;
} drv_bank_info_t;


//����index�������͵�ַ���屣��һ��,����builder���߱���һ��
typedef enum {
    DRV_GROUP_SYS = 0,
    DRV_GROUP_STG_BASE,
    DRV_GROUP_STUB,
    DRV_GROUP_HAL,
    DRV_GROUP_USB_DEVICE,
    DRV_GROUP_STORAGE,
    DRV_GROUP_FS,
    DRV_GROUP_KEY,
    DRV_GROUP_CHANNEL,
    DRV_GROUP_COMMON,
    DRV_GROUP_BT_HOST,
    DRV_GROUP_BT_CON,
    MMM_GROUP,
    CODEC_GROUP,
    SOUND_EFFECT_GROUP,
    SPEECH_GROUP,
    PLC_GROUP,
    SP_PRE_GROUP,
    SP_POST_GROUP,
    REC_MOD_GROUP,
    REC_WAV_GROUP,
    AL_USER1_GROUP,
    AL_USER2_GROUP,
    AL_USER3_GROUP,
    SUPPORT_DRV_GROUP_TOTAL,
} drv_type_t;




/*���¶���init �� exit �Ľӿڣ�����������*/
/*����ͨ��CMD������*/
/*void*  (*vfs_init)(vfs_mount_t* p_vfs_mount, void* null, void* null, void* null);*/
/*void* (*vfs_exit)(vfs_mount_t* p_vfs_mount, void* null1, void* null2, void* null3);*/

typedef int (*drv_entry_i)(void *param1, void *param2, void *param3);

#endif
#endif /*_DRIVER_H */



