/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona  2012-12-28 15:37    1.0             build this file
 *******************************************************************************/

#ifndef _EH_FSEL_ENTRY_

#define _EH_FSEL_ENTRY_

#include "enhanced.h"

#include "eh_globle_data.h"

#define DIR_OVERTIME  4000 //4S超时返回

//先随机定位到组，再随机定位到组内的成员，播放完一组再切到下一组

#define MAX_STORE_INDEX  15

#define EH_GET_EXT_FILE EXT_NAME_ALL_FILE    //前两字节为“*f”，表示dir所有文件
#define EH_GET_EXT_DIR  EXT_NAME_ALL_DIR     //前两字节为“*d”，表示dir所有目录
#define EH_GET_EXT_ALL  EXT_NAME_ALL         //前两字节为“**”，表示dir所有目录和文件
#define EH_DIR_FLAG   ('d'*0x100+'*')
#define EH_FILE_FLAG  ('f'*0x100+'*')

typedef bool (*fsel_get_next_p)(uint8 *strfile);
typedef bool (*fsel_get_prev_p)(uint8 *strfile);
typedef bool (*fsel_get_byno_p)(uint8 *strfile, uint16 num);
typedef bool (*fsel_set_location_p)(void *location);
typedef bool (*fsel_get_location_p)(void *location);
typedef bool (*fsel_get_nextdir_p)(uint8 *strdir);
typedef bool (*fsel_get_prevdir_p)(uint8 *strdir);
typedef uint16 (*fsel_get_total_p)(void);

//
extern void fsel_init_sysdata(void);

//for shffule
extern bool fsel_init_shuffle(uint16 num_total);

extern uint16 fsel_shuffle_next_file(void);

extern uint16 fsel_shuffle_prev_file(void);
//end for shuufle

//for directory functions
//extern uint16 fsel_total_file_cur(uint8* dir_info, uint8 check_flag);

extern uint16 fsel_total_file_all(void);

extern uint8 *creat_disk_scan_thread(file_location_t *cur_file_location);

extern bool fsel_dir_get_byno(uint8 *strfile, uint16 num);

extern bool fsel_dir_get_byno_all(uint8 *strfile, uint16 num);

extern bool fsel_dir_get_location(file_location_t *location);

extern bool fsel_dir_set_location(file_location_t *location);

extern bool fsel_dir_get_next(uint8 *strfile);

extern bool fsel_dir_next_dir(uint8 *strdir);

extern bool fsel_dir_next_layer(uint8 *strfile);

extern bool fsel_dir_get_prev(uint8 *strfile);

extern bool fsel_dir_prev_dir(uint8 *strdir);

extern bool fsel_dir_prev_layer(uint8 *strfile);

extern bool filtrate_dir(void);

extern int32 get_disk_total_num(void);

extern bool get_current_file_name(uint8 index, uint8 *filename);

extern bool get_current_file_num(uint8*in_filename, uint8* out_index);

extern bool get_next_file(uint8*filename);

extern bool get_prev_file(uint8*filename);

extern bool disk_c_fs_init(void);

extern uint16 extlib_get_disk_total_num(void);

extern bool extlib_get_current_file_name(uint16 num, uint8 *filename);

extern bool extlib_get_current_file_num(uint8*in_filename, uint16 *out_index);

extern bool extlib_get_next_file(uint8*filename);

extern bool extlib_get_prev_file(uint8*filename);

extern bool extlib_disk_c_fs_init(void);

#endif //_EH_FSEL_ENTRY_
