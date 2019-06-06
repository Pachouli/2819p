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

uint8 eh_fsel_type = 0; //ģ������
bool  eh_fsel_filter_record_dir_flag; //�Ƿ����¼��Ŀ¼ RECORD

uint8 eh_scan_mode = 0; //music_common.h -> ENTER_MUSIC_START
bool disk_scan_cur_dir_first_file_flag = FALSE;

fsel_param_t fsel_init_val; //ģ�����

fsel_error_e fsel_error; //��������

uint8 eh_dir_layer; //��ǰĿ¼���б�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
uint8 eh_shuffle_sw; //SHUFFLE ����

uint16 eh_file_no; //��ǰ�ļ���Ŀ¼���б��е����
uint16 eh_file_total; //��ǰĿ¼���б�����ļ�����

uint16 eh_file_no_all; //��ǰ�ļ��ڴ����е����
uint16 eh_file_total_all; //��ǰ�������ļ�����

uint32 eh_cur_ext_name; //��ǰѡ���������չ��
uint16 eh_root_dir; //��Ŀ¼����Ŀ¼ָ�룬1,2��������

uint32 eh_vfs_mount = 0; //vFS ʶ���

uint16 eh_dir_no = 1; //dir�ĸ���

//����ļ�λ����Ϣ
pfile_offset_t pfile_offset_buffer;

