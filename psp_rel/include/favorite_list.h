/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2012-12-25 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __FAVORITE_LIST_H__
#define __FAVORITE_LIST_H__

#include "enhanced.h"
#include "dir_list.h"

typedef struct
{

  uint8 file_status;//标志文件是否已经删除
	pdir_layer_t dir_layer_info;//目录层次信息 	
	uint16 reserv; //   
	pfile_offset_t file_offset_info;//文件偏移信息 
	uint8 file_name[32];
}fsel_favorite_t;

#endif //__FAVORITE_LIST_H__





