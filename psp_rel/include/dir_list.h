/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2012-12-25 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __DIR_LIST_H__
#define __DIR_LIST_H__

#include "vfs_interface.h"

//���Ŀ¼����
#define SUPPORT_MUSIC_DIR 204

//����ļ�����
#define SUPPORT_MUSIC_FILE 9999

//��ǰ��λ��־
#define PREV_FALG 0x80000000


//�б�ͷ��Ϣ
typedef struct
{
    //�ļ�����
    uint16 file_total;
    //Ŀ¼����
    uint16 dir_total;
    //����������ռ�õ�������
    uint8  total_sectors;
    //����������չ
    uint8 retain[27];
} list_head_info_t;

#define DIR_LIST_BUF_ADDR 0x9fc2f800

//Ŀ¼�б�buffer ��С
#define DIR_LIST_SIZE 3*1024

#endif //__DIR_LIST_H__
