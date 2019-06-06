/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona  2012-12-27 15:37    1.0             build this file
 *******************************************************************************/

#include "enhanced.h"

#include "eh_globle_data.h"

#include "dir_list.h"

uint8 eh_fsel_type = 0; //模块类型
bool  eh_fsel_filter_record_dir_flag; //是否过滤录音目录 RECORD

uint8 eh_scan_mode = 0; //music_common.h -> ENTER_MUSIC_START
bool disk_scan_cur_dir_first_file_flag = FALSE;

fsel_param_t fsel_init_val; //模块参数

fsel_error_e fsel_error; //错误类型

uint8 eh_dir_layer; //当前目录或列表的层次，0表示根目录，1表示根目录的子目录，以此类推
uint8 eh_shuffle_sw; //SHUFFLE 开关

uint16 eh_file_no; //当前文件在目录或列表中的序号
uint16 eh_file_total; //当前目录或列表的总文件个数

uint16 eh_file_no_all; //当前文件在磁盘中的序号
uint16 eh_file_total_all; //当前磁盘总文件个数

uint32 eh_cur_ext_name; //当前选择歌曲的扩展名
uint16 eh_root_dir; //根目录的子目录指针，1,2，。。。

uint32 eh_vfs_mount = 0; //vFS 识别号

uint16 eh_dir_no = 1; //dir的个数

//存放文件位置信息
pfile_offset_t pfile_offset_buffer;

