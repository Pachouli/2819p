/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-28 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

/******************************************************************************
 * \par  Description:ͳ�Ƶ�ǰĿ¼���ļ�������
 * \uint16 fsel_total_file_cur(uint8* dir_info, uint8 check_flag)
 * \param[in]  dir_info param1 ���Ŀ¼·����Ϣָ��
 * \param[in]  check_flag param2 ����Ƿ��ǵ�ǰĿ¼�ı�־(set_locationʱ��)

 * \param[out]  none

 * \return   uint16 ��ǰĿ¼���ļ�����

 * \ingroup      eh_fsel_directory_func.c
 * \note
 *******************************************************************************/
uint16 fsel_total_file_cur(uint8 *dir_info, uint8 check_flag)
{
    uint8 temp_offset_buf[8];
    uint8 dir_type = DIR_HEAD;
    uint16 total = 0;
    pdir_layer_t *pdir_layer_total_tmp;
    uint32 dir_start_timer = sys_get_ab_timer();

    //����ѿռ�
    pdir_layer_total_tmp = (pdir_layer_t *) sys_malloc(sizeof(pdir_layer_t));
    if (pdir_layer_total_tmp == NULL)
    {
        return 0;
    }

    //save current path
    vfs_file_dir_offset(eh_vfs_mount, pdir_layer_total_tmp, &pfile_offset_buffer, 0);

    do
    {
        if (0 != vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit))
        {
            total++;
            if (check_flag == 1)
            {
                //�Ƚ�8�ֽ�offset
                vfs_file_dir_offset(eh_vfs_mount, NULL, (pfile_offset_t*) temp_offset_buf, 0);
                if (libc_memcmp(dir_info, temp_offset_buf, 8) == 0)
                {
                    eh_file_no = total;
                    check_flag = 0;
                }
            }

            //ֻ��Ҫɨһ�ף����浱ǰĿ¼����Ϣ
            if ((total >= 1 ) && (disk_scan_cur_dir_first_file_flag == TRUE))
            {
                break;
            }
        }
        else
        {
            break;
        }
        dir_type = DIR_NEXT;

        if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
        {
            break;
        }
    }
    while (1);

    //back up path
    vfs_file_dir_offset(eh_vfs_mount, pdir_layer_total_tmp, &pfile_offset_buffer, 1);

    //�ͷŶѿռ�
    sys_free(pdir_layer_total_tmp);
    pdir_layer_total_tmp = NULL;

    if (check_flag == 1)
    {
        eh_file_no = 0;
    }

    return total;
}

