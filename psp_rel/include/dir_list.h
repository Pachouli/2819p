/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2012-12-25 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __DIR_LIST_H__
#define __DIR_LIST_H__

#include "vfs_interface.h"

//最大目录个数
#define SUPPORT_MUSIC_DIR 204

//最大文件个数
#define SUPPORT_MUSIC_FILE 9999

//向前定位标志
#define PREV_FALG 0x80000000


//列表头信息
typedef struct
{
    //文件总数
    uint16 file_total;
    //目录总数
    uint16 dir_total;
    //所有数据所占用的扇区数
    uint8  total_sectors;
    //保留方便扩展
    uint8 retain[27];
} list_head_info_t;

#define DIR_LIST_BUF_ADDR 0x9fc2f800

//目录列表buffer 大小
#define DIR_LIST_SIZE 3*1024

#endif //__DIR_LIST_H__
