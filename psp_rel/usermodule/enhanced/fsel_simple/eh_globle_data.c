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

//存放文件位置信息
pfile_offset_t pfile_offset_total;

//for shuffle
uint16 shuffle_total; //shuffle的总个数
uint8 group_map[SET_GROUP_MAX / 8]; //分组，共64组(bit为1-未用,0-已用)
uint8 member_map[SET_MEMBER_MAX / 8]; //组内成员，共256(bit为1-未用,0-已用)
uint8 member_avg; //成员平均值，商值
uint8 member_avg_group; //平均值成员的组号起始
uint8 group_max; //有效的组号
uint8 member_max; //当前组的最大成员个数
uint8 use_member_flag; //使用组内shuffle功能
uint8 cur_group; //当前的组号（1，2，。。。
uint16 cur_member; //当前组内的成员号（1，2，。。。
uint16 filtrate_shuffle_no; //shuffle过滤播放序号，避免重复；取值为0,1~n

uint16 play_num_list[SET_PREV_MAX];
uint8 cur_save_pt; //当前存的指针(0~)
uint8 cur_save_prev_count;
uint8 cur_save_next_count;
uint8 cur_overturn_flag; //表翻转标记,1-yes,0-no
//end for shuffle

disk_scan_info_t *g_p_disk_scan_info;
