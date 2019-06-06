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

#ifndef _EH_GLOBLE_DATA_H
#define _EH_GLOBLE_DATA_H

#include "enhanced.h"
#include "dir_list.h"

#define SET_GROUP_MAX   64
#define SET_MEMBER_MAX  256
#define SET_PREV_MAX    16

extern uint8 eh_fsel_type; //模块类型
extern bool  eh_fsel_filter_record_dir_flag; //是否过滤录音目录 RECORD

extern uint8 eh_scan_mode;
extern bool disk_scan_cur_dir_first_file_flag;

extern fsel_param_t fsel_init_val; //模块参数
extern fsel_error_e fsel_error; //错误类型
extern uint8 eh_dir_layer; //当前目录或列表的层次，0表示根目录，1表示根目录的子目录，以此类推
extern uint8 eh_shuffle_sw; //SHUFFLE 开关

extern uint16 eh_file_no; //当前文件在目录或列表中的序号
extern uint16 eh_file_total; //当前目录或列表的总文件个数

extern uint16 eh_file_no_all; //当前文件在磁盘中的序号
extern uint16 eh_file_total_all; //当前磁盘总文件个数

extern uint32 eh_cur_ext_name; //当前选择歌曲的扩展名
extern uint16 eh_root_dir; //根目录的子目录指针，1,2，。。。

extern uint32 eh_vfs_mount; //FS 识别号

extern uint16 eh_dir_no; //dir的个数

//存放文件位置信息
extern pfile_offset_t pfile_offset_buffer;

//存放文件位置信息
extern pfile_offset_t pfile_offset_total;

//for shuffle
extern uint16 shuffle_total; //shuffle的总个数
extern uint8 group_map[SET_GROUP_MAX / 8]; //分组，共64组(bit为1-未用,0-已用)
extern uint8 member_map[SET_MEMBER_MAX / 8]; //组内成员，共256(bit为1-未用,0-已用)
extern uint8 member_avg; //成员平均值，商值
extern uint8 member_avg_group; //平均值成员的组号起始
extern uint8 group_max; //有效的组号
extern uint8 member_max; //当前组的最大成员个数
extern uint8 use_member_flag; //使用组内shuffle功能
extern uint8 cur_group; //当前的组号（1，2，。。。
extern uint16 cur_member; //当前组内的成员号（1，2，。。。
extern uint16 filtrate_shuffle_no; //shuffle过滤播放序号，避免重复；取值为0,1~n

extern uint16 play_num_list[SET_PREV_MAX];
extern uint8 cur_save_pt; //当前存的指针(0~)
extern uint8 cur_save_prev_count;
extern uint8 cur_save_next_count;
extern uint8 cur_overturn_flag; //表翻转标记,1-yes,0-no

//end for shuffle

typedef struct
{
    file_location_t search_location;   //当前扫描到的文件或文件夹的location
    volatile bool disk_scan_over_flag; //是否已经扫描结束
    uint8 dir_type;                    //dir head or next
    uint16 current_file;               //记录正在扫面的当前目录的文件数，即序号
    uint32 search_start_timer;         //开始扫描时间
} disk_scan_info_t;
extern disk_scan_info_t *g_p_disk_scan_info;

#endif //#ifndef _EH_GLOBLE_DATA_H
