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

extern uint8 eh_fsel_type; //ģ������
extern bool  eh_fsel_filter_record_dir_flag; //�Ƿ����¼��Ŀ¼ RECORD

extern uint8 eh_scan_mode;
extern bool disk_scan_cur_dir_first_file_flag;

extern fsel_param_t fsel_init_val; //ģ�����
extern fsel_error_e fsel_error; //��������
extern uint8 eh_dir_layer; //��ǰĿ¼���б�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
extern uint8 eh_shuffle_sw; //SHUFFLE ����

extern uint16 eh_file_no; //��ǰ�ļ���Ŀ¼���б��е����
extern uint16 eh_file_total; //��ǰĿ¼���б�����ļ�����

extern uint16 eh_file_no_all; //��ǰ�ļ��ڴ����е����
extern uint16 eh_file_total_all; //��ǰ�������ļ�����

extern uint32 eh_cur_ext_name; //��ǰѡ���������չ��
extern uint16 eh_root_dir; //��Ŀ¼����Ŀ¼ָ�룬1,2��������

extern uint32 eh_vfs_mount; //FS ʶ���

extern uint16 eh_dir_no; //dir�ĸ���

//����ļ�λ����Ϣ
extern pfile_offset_t pfile_offset_buffer;

//����ļ�λ����Ϣ
extern pfile_offset_t pfile_offset_total;

//for shuffle
extern uint16 shuffle_total; //shuffle���ܸ���
extern uint8 group_map[SET_GROUP_MAX / 8]; //���飬��64��(bitΪ1-δ��,0-����)
extern uint8 member_map[SET_MEMBER_MAX / 8]; //���ڳ�Ա����256(bitΪ1-δ��,0-����)
extern uint8 member_avg; //��Աƽ��ֵ����ֵ
extern uint8 member_avg_group; //ƽ��ֵ��Ա�������ʼ
extern uint8 group_max; //��Ч�����
extern uint8 member_max; //��ǰ�������Ա����
extern uint8 use_member_flag; //ʹ������shuffle����
extern uint8 cur_group; //��ǰ����ţ�1��2��������
extern uint16 cur_member; //��ǰ���ڵĳ�Ա�ţ�1��2��������
extern uint16 filtrate_shuffle_no; //shuffle���˲�����ţ������ظ���ȡֵΪ0,1~n

extern uint16 play_num_list[SET_PREV_MAX];
extern uint8 cur_save_pt; //��ǰ���ָ��(0~)
extern uint8 cur_save_prev_count;
extern uint8 cur_save_next_count;
extern uint8 cur_overturn_flag; //��ת���,1-yes,0-no

//end for shuffle

typedef struct
{
    file_location_t search_location;   //��ǰɨ�赽���ļ����ļ��е�location
    volatile bool disk_scan_over_flag; //�Ƿ��Ѿ�ɨ�����
    uint8 dir_type;                    //dir head or next
    uint16 current_file;               //��¼����ɨ��ĵ�ǰĿ¼���ļ����������
    uint32 search_start_timer;         //��ʼɨ��ʱ��
} disk_scan_info_t;
extern disk_scan_info_t *g_p_disk_scan_info;

#endif //#ifndef _EH_GLOBLE_DATA_H
