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
* \brief    bank����ӿ�
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
    unsigned int sram_text_offset;      //sram text���ļ������ƫ��
    unsigned int sram_text_length;      //sram text����Ч���ȣ�Ϊ0��ʾû����Ҫ��SRAM���еĴ���
    unsigned int sram_text_addr;        //sram text����ʼ�߼���ַ������ΪSRAM��ַ
    unsigned int data_offset;           //��һ��data�����ļ������ƫ��
    unsigned int data_length;           //��һ��data�γ���
    unsigned int data_addr;             //��һ��data���߼���ַ
    unsigned int data1_offset;          //�ڶ���data�����ļ������ƫ��
    unsigned int data1_length;          //�ڶ���data�γ���
    unsigned int data1_addr;            //�ڶ���data���߼���ַ
    unsigned int bss_length;            //bss�γ���
    unsigned int bss_addr;              //bss���߼���ַ
    unsigned int text_offset;           //text code���ļ������ƫ��
    unsigned int text_addr;             //text����ʼ�߼���ַ
    unsigned int entry;                 //AP��ں�����ַ
}ap_head_t;

#endif


