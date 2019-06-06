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


#ifndef _SDFS_H
#define _SDFS_H
#include <attr.h>
#include <typeext.h>

#define MAX_HEANDLE_NUM                 (8)



#define SDFILE_CLOSED                   (0xFF)

#define SD_DIR_OFFSET                   (0x200)
#define SD_DIR_NAME_LEN                 (0x0B)
#define SD_DIR_ENTRY_LEN                (0x20)

#define MAX_USED_SD_DIR_ITEM            (240)

typedef struct
{
    unsigned char   isvalid;
    unsigned char   reserve[3];
    unsigned int    startaddr;
    unsigned int    endaddr;
    unsigned int    rwpointer;
} sd_file_t;

typedef struct
{
    unsigned char   fname[11];
    unsigned char   fattr;
    unsigned char   reserve0[2];
    unsigned short  version;
    unsigned int    offset;
    unsigned int    size;
    unsigned int    reserve1;
    unsigned int    checksum;
} sd_dir_t;

typedef sd_file_t* sd_handle;

#endif



