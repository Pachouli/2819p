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
 * \file     kernel_sd_drv.h
 * \brief    SD×Ö½Ú¶Á½Ó¿Ú
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _KERNEL_SD_H
#define _KERNEL_SD_H

#include <typeext.h>
#include <ucos/init.h>


typedef struct
{
    uint32 file_offset;
    uint32 sram_addr;
    uint32 len;
} sd_byte_param_t;



typedef struct
{
    uint32 file_offset;
    uint32 sram_addr;
    uint32 sec_num;
} sd_sec_param_t;


#define NOR_BASE_PLATFORM           (0)
#define CARD_BASE_PLATFORM          (1)



typedef struct
{
    uint8 base_type;
    uint8 reserse[3];
} base_op_t;
#endif




