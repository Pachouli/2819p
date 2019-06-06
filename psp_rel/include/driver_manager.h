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
 * \brief    驱动管理
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
    //卡类型:0-无定义；1-SD 卡；2-mmc 4.0x以下;3mmc 4.0x以上
    uint8 card_type : 2;
    //卡带宽:0-1 bit；1-4 bit；2-8 bit
    uint8 card_bus_width : 2;
    //卡时钟:0-没有配置成高速;1-成功配置为高速
    uint8 card_clock:2;
    //卡访问单元:0-byte为单位；1-sector为单元
    uint8 card_cap:2;
    //卡连续写标记:0-非连续写；1-连续写
    uint8 card_continue_w:1;
    //卡连续读标记:0-非连续读；1-连续读
    uint8 card_continue_r:1;
    //卡是否完成初始化标记:0-未完成；1-已完成
    uint8 card_had_init:1;

    uint8 reserved: 5;
    //卡相对地址
    uint16 card_rca;
    //文件系统一次描述fat表的大小，以扇区为单位
    uint32 fs_scan_space;
    uint32 backdoor_flag;
    //卡读写与卡命令检测互斥锁
    os_event_t *card_mutex;
    //ud data buffer
    uint8 *ud_data_buffer;
}sys_global_info_t;




#define MODE_NORMAL 0
#define MODE_SPECIAL 1
typedef struct
{
    unsigned char file_type;        //’D’, driver
    unsigned char drv_type;         //driver type
    unsigned char Major_version;
    unsigned char minor_version;
    unsigned char magic[4];
    unsigned int sram_text_offset;  //sram text在文件中相对偏移
    unsigned int sram_text_length;  //sram text段有效长度,为0表示没有需要在SRAM运行的代码
    unsigned int sram_text_addr;    //sram text段起始逻辑地址,必须为SRAM地址
    unsigned int data_offset;       //第一个data段在文件中相对偏移
    unsigned int data_length;       //第一个data段长度
    unsigned int data_addr;         //第一个data段逻辑地址
    unsigned int data1_offset;      //第二个data段在文件中相对偏移
    unsigned int data1_length;      //第二个data段长度
    unsigned int data1_addr;        //第二个data段逻辑地址
    unsigned int sram_text1_offset; //sram text1在文件中相对偏移
    unsigned int sram_text1_length; //sram text1段有效长度,驱动加载时不会load到ram中去,需要调用系统接口加载
    unsigned int sram_text1_addr;   //sram text1段起始逻辑地址,必须为SRAM地址
    unsigned int bss_length;        //bss段长度
    unsigned int bss_addr;          //bss段逻辑地址
    unsigned int drv_op_entry;      //函数接口数组起始逻辑地址
    unsigned int entry_table_len;   //函数接口数组长度
    unsigned int text_offset;       //entry table或text code在文件中相对偏移
    unsigned int text_addr;         //text段起始逻辑地址,在link.xn定义
    unsigned int drv_init_entry;    //驱动初始化接口逻辑地址
    unsigned int drv_exit_entry;    //驱动退出接口逻辑地址
} drv_head_t;

typedef struct
{
//    uint32 bank_a_offset;
//    uint32 bank_b_offset;
//    uint32 bank_c_offset;
    uint32 exit_entry;
} drv_bank_info_t;


//驱动index定义必须和地址定义保持一致,且与builder工具保持一致
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




/*重新定义init 和 exit 的接口，有三个参数*/
/*不能通过CMD来调用*/
/*void*  (*vfs_init)(vfs_mount_t* p_vfs_mount, void* null, void* null, void* null);*/
/*void* (*vfs_exit)(vfs_mount_t* p_vfs_mount, void* null1, void* null2, void* null3);*/

typedef int (*drv_entry_i)(void *param1, void *param2, void *param3);

#endif
#endif /*_DRIVER_H */



