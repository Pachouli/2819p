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
* \file     ap_bank_define.h
* \brief    bank管理接口
* \author   wuyueqian
* \version  1.0
* \date  2011/9/10
*******************************************************************************/
#ifndef __AP_BANK_DEFINE_H__
#define __AP_BANK_DEFINE_H__

#include <typeext.h>
#include <attr.h>


typedef struct{
    unsigned int    text_offset;     
    unsigned int    text_length;         
    unsigned int    text_addr;    
}text_code_index_t;


typedef struct
{
    unsigned char file_type;            //'P'
    unsigned char ap_type;              //0:ap_system, 1:ap_user
    unsigned char major_version;
    unsigned char minor_version;
    unsigned char magic[4];
    unsigned int sram_text_offset;      //sram text在文件中相对偏移
    unsigned int sram_text_length;      //sram text段有效长度，为0表示没有需要在SRAM运行的代码
    unsigned int sram_text_addr;        //sram text段起始逻辑地址，必须为SRAM地址
    unsigned int data_offset;           //第一个data段在文件中相对偏移
    unsigned int data_length;           //第一个data段长度
    unsigned int data_addr;             //第一个data段逻辑地址
    unsigned int data1_offset;          //第二个data段在文件中相对偏移
    unsigned int data1_length;          //第二个data段长度
    unsigned int data1_addr;            //第二个data段逻辑地址
    unsigned int bss_length;            //bss段长度
    unsigned int bss_addr;              //bss段逻辑地址
    unsigned int text_offset;           //text code在文件中相对偏移
    unsigned int text_addr;             //text段起始逻辑地址
    unsigned int entry;                 //AP入口函数地址
}ap_head_t;

#endif


