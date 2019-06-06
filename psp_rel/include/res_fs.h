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
 * \file     sdfs.h
 * \brief    sd文件系统接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _RES_FS_H
#define _RES_FS_H
#include <attr.h>
#include <typeext.h>

#define MAX_RES_HEANDLE_NUM                 (1)
#define RES_MAP_SIZE						(128)
typedef struct
{
    unsigned char isvalid;
    unsigned char reserve[3];
    unsigned int startaddr;
    unsigned int endaddr;
    unsigned int rwpointer;
} res_file_t;

typedef struct
{
    uint32 offset;
    uint32 length;
    uint8 name[12];
    uint32 reserved[3];
} res_dir_info_t;

typedef res_file_t* res_handle;

#endif

