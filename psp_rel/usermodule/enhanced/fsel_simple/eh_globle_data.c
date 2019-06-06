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

//����ļ�λ����Ϣ
pfile_offset_t pfile_offset_total;

//for shuffle
uint16 shuffle_total; //shuffle���ܸ���
uint8 group_map[SET_GROUP_MAX / 8]; //���飬��64��(bitΪ1-δ��,0-����)
uint8 member_map[SET_MEMBER_MAX / 8]; //���ڳ�Ա����256(bitΪ1-δ��,0-����)
uint8 member_avg; //��Աƽ��ֵ����ֵ
uint8 member_avg_group; //ƽ��ֵ��Ա�������ʼ
uint8 group_max; //��Ч�����
uint8 member_max; //��ǰ�������Ա����
uint8 use_member_flag; //ʹ������shuffle����
uint8 cur_group; //��ǰ����ţ�1��2��������
uint16 cur_member; //��ǰ���ڵĳ�Ա�ţ�1��2��������
uint16 filtrate_shuffle_no; //shuffle���˲�����ţ������ظ���ȡֵΪ0,1~n

uint16 play_num_list[SET_PREV_MAX];
uint8 cur_save_pt; //��ǰ���ָ��(0~)
uint8 cur_save_prev_count;
uint8 cur_save_next_count;
uint8 cur_overturn_flag; //��ת���,1-yes,0-no
//end for shuffle

disk_scan_info_t *g_p_disk_scan_info;
