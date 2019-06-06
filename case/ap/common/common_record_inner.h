/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming    2011-10-14          1.0             build this file
 ********************************************************************************/

#ifndef _COMMON_RECORD_INNER_H_
#define _COMMON_RECORD_INNER_H_

#include "common_func_inner.h"
#include "record_common.h"
#include "vm_def.h"

#include "mmm_mr.h"

//录音文件最大个数
#define     FILE_NUM_MAX          999
//磁盘空间满最小扇区
#define     REC_SPACE_LOW         200

//磁盘空间满最小扇区(10MB)
#define     REC_SPACE_BASE       (20*1024)


/*
    FIRST表示第一次进入录音，打开文件系统，创建文件
    NEXT表示按下f1，下一个录音，这个时候重新创建下一个文件，但是文件系统不需要关闭
*/
#define FILE_FIRST 0
#define FILE_NEXT  1

/*录音文件格式*/
typedef enum
{
    /*录音格式 ：wav录音*/
    REC_FORMAT_WAV = 0,
    /*录音格式 ：mp3录音   暂不支持*/
    REC_FORMAT_MP3 = 1,
} record_file_type_e;

typedef struct
{
    /*已录文件大小*/
    uint32 file_bytes;
} record_rec_num_t;

/*wav录音掉电回写文件头结构体*/
typedef struct
{
    /*魔方数：判断vm数据是否有效*/
    uint16 magic;
    /* 文件序号 */
    uint16 file_num;
    /*长名*/
    uint8 rec_filename[28];
    /*文件偏移信息*/
    pfile_offset_t ptr_file_offset; //must word align
    /*目录信息,73byte,需要做对齐*/
    pdir_layer_t ptr_layer;
    /*文件首簇*/
    uint32 start_cluster;
} record_file_path_t;

/*录音文件相关参数集合*/
typedef struct
{
    /*录音文件mount id*/
    uint32 vfs_handle;
    /*剩余磁盘空间,扇区数*/
    uint32 free_space;
    /*fhandle文件句柄*/
    HANDLE fhandle;
    /*录音文件短名，8+3格式*/
    //uint8 rec_filename[12];
    /*录音长文件名*/
    uint8 rec_filename[28];
    /*显示录音文件名，带点去空格格式*/
    //uint8 show_filename[13];  //不支持
    /*flash格式化标识*/
    uint8 disk_flag;
    /*最大文件长度*/
    uint32 file_size_max;
} record_fs_param_t;

typedef struct
{
    record_fs_param_t  fs_param;          /*录音文件相关参数集合*/
    record_file_path_t file_path;         /*记录最新录音文件路径*/
    record_rec_num_t   file_num_info;     /*记录最新录音文件号，以及当前录音文件起始簇号和文件长度*/
    uint16             rec_num;           //用于录音文件序号的计数
    uint16             file_total;        //record文件夹文件总数
    uint32             record_time_last; //unit 1S
    bool               first_cal_free_space; //for free time calcute
    uint8              disk_type;
    record_status_e    last_record_sta;   /*上一个录音状态*/
    record_status_e    record_sta;        /*录音状态*/
    uint8 *            record_file_bitmap_addr;
} record_file_info_t;

extern record_file_info_t *p_record_file_info;

/*录音文件名字处理*/
extern void com_ansi_to_unicode(uint8 *file_name);

/* 文件系统处理 */
extern uint8 _record_file_sys_init(uint8 diskno);

extern bool _record_disk_exist(uint8 disk_no);
extern bool _record_disk_init(uint8 disk_no);
extern bool _record_disk_exit(void);
extern bool _record_disk_get_disk_capacity(void);

/* 文件处理函数 */
bool _record_file_close(void);
extern bool _record_file_first_create(void);

/* 掉电恢复相关函数 */
bool com_record_recover_record_file_inner(void);
bool rec_file_recover_deal(uint8 disk_type);

/*vram操作相关函数*/
bool rec_file_recover_load_fs_vram(record_file_path_t* p_file_path, uint8 disk_type);
bool rec_file_recover_save_fs_vram(record_file_path_t* p_file_path, uint8 disk_type);
bool rec_file_recover_load_file_vram(record_rec_num_t * ap_param, uint8 disk_type);
bool rec_file_recover_save_file_vram(record_rec_num_t * ap_param, uint8 disk_type);

#endif
