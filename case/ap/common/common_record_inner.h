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

//¼���ļ�������
#define     FILE_NUM_MAX          999
//���̿ռ�����С����
#define     REC_SPACE_LOW         200

//���̿ռ�����С����(10MB)
#define     REC_SPACE_BASE       (20*1024)


/*
    FIRST��ʾ��һ�ν���¼�������ļ�ϵͳ�������ļ�
    NEXT��ʾ����f1����һ��¼�������ʱ�����´�����һ���ļ��������ļ�ϵͳ����Ҫ�ر�
*/
#define FILE_FIRST 0
#define FILE_NEXT  1

/*¼���ļ���ʽ*/
typedef enum
{
    /*¼����ʽ ��wav¼��*/
    REC_FORMAT_WAV = 0,
    /*¼����ʽ ��mp3¼��   �ݲ�֧��*/
    REC_FORMAT_MP3 = 1,
} record_file_type_e;

typedef struct
{
    /*��¼�ļ���С*/
    uint32 file_bytes;
} record_rec_num_t;

/*wav¼�������д�ļ�ͷ�ṹ��*/
typedef struct
{
    /*ħ�������ж�vm�����Ƿ���Ч*/
    uint16 magic;
    /* �ļ���� */
    uint16 file_num;
    /*����*/
    uint8 rec_filename[28];
    /*�ļ�ƫ����Ϣ*/
    pfile_offset_t ptr_file_offset; //must word align
    /*Ŀ¼��Ϣ,73byte,��Ҫ������*/
    pdir_layer_t ptr_layer;
    /*�ļ��״�*/
    uint32 start_cluster;
} record_file_path_t;

/*¼���ļ���ز�������*/
typedef struct
{
    /*¼���ļ�mount id*/
    uint32 vfs_handle;
    /*ʣ����̿ռ�,������*/
    uint32 free_space;
    /*fhandle�ļ����*/
    HANDLE fhandle;
    /*¼���ļ�������8+3��ʽ*/
    //uint8 rec_filename[12];
    /*¼�����ļ���*/
    uint8 rec_filename[28];
    /*��ʾ¼���ļ���������ȥ�ո��ʽ*/
    //uint8 show_filename[13];  //��֧��
    /*flash��ʽ����ʶ*/
    uint8 disk_flag;
    /*����ļ�����*/
    uint32 file_size_max;
} record_fs_param_t;

typedef struct
{
    record_fs_param_t  fs_param;          /*¼���ļ���ز�������*/
    record_file_path_t file_path;         /*��¼����¼���ļ�·��*/
    record_rec_num_t   file_num_info;     /*��¼����¼���ļ��ţ��Լ���ǰ¼���ļ���ʼ�غź��ļ�����*/
    uint16             rec_num;           //����¼���ļ���ŵļ���
    uint16             file_total;        //record�ļ����ļ�����
    uint32             record_time_last; //unit 1S
    bool               first_cal_free_space; //for free time calcute
    uint8              disk_type;
    record_status_e    last_record_sta;   /*��һ��¼��״̬*/
    record_status_e    record_sta;        /*¼��״̬*/
    uint8 *            record_file_bitmap_addr;
} record_file_info_t;

extern record_file_info_t *p_record_file_info;

/*¼���ļ����ִ���*/
extern void com_ansi_to_unicode(uint8 *file_name);

/* �ļ�ϵͳ���� */
extern uint8 _record_file_sys_init(uint8 diskno);

extern bool _record_disk_exist(uint8 disk_no);
extern bool _record_disk_init(uint8 disk_no);
extern bool _record_disk_exit(void);
extern bool _record_disk_get_disk_capacity(void);

/* �ļ������� */
bool _record_file_close(void);
extern bool _record_file_first_create(void);

/* ����ָ���غ��� */
bool com_record_recover_record_file_inner(void);
bool rec_file_recover_deal(uint8 disk_type);

/*vram������غ���*/
bool rec_file_recover_load_fs_vram(record_file_path_t* p_file_path, uint8 disk_type);
bool rec_file_recover_save_fs_vram(record_file_path_t* p_file_path, uint8 disk_type);
bool rec_file_recover_load_file_vram(record_rec_num_t * ap_param, uint8 disk_type);
bool rec_file_recover_save_file_vram(record_rec_num_t * ap_param, uint8 disk_type);

#endif
