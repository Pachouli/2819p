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

/******************************************************************************/
/*
 * \par  Description: ��ȡ��ǰĿ¼�µ��¸��ļ�
 *
 * \param[in]
 * \param[out]      strfile--����ļ�����չ��
 * \return          TRUE OR FALSE
 * \note
 *******************************************************************************/
bool fsel_total_dir_next(void)
{
    uint8 dir_type;
    if (eh_file_no == 0)
    {
        //���Ϊ0���ͷ��ʼ����
        dir_type = DIR_HEAD;
    }
    else
    {
        //��Ų�Ϊ0��dir��һ��
        dir_type = DIR_NEXT;
    }
    //dir�ļ�
    if (vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit) != 0)
    {
        //�ҵ��ļ����������
        eh_file_no++;
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ���̵���һ���ļ�
 *
 * \param[in]

 * \param[out]        strfile--����ļ�����չ��

 * \return            TRUE OR FALSE

 * \note
 *******************************************************************************/

bool fsel_total_next_layer(void)
{
    bool vfs_dir_ret = FALSE;
    uint8 dir_type;
    uint32 dir_start_timer = sys_get_ab_timer();

    //�ڵ�ǰĿ¼�������ļ�
    if (FALSE == fsel_total_dir_next())
    {
        CurrentDiragain:
        if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
        {
            return FALSE;
        }
        if (eh_dir_layer >= (MAX_DIR_LAYER - 1)) //�����ǰ��8��Ŀ¼�Ͳ������ļ���
        {
            goto ParentDiragainNext;
        }

        vfs_dir_ret = FALSE;
        //Ѱ�Ҹ�Ŀ¼�ĵ�һ�����ļ���
        dir_type = DIR_HEAD;
        while (1)
        {
            if (0 == vfs_dir(eh_vfs_mount, dir_type, NULL, EXT_NAME_ALL_DIR))
            {
                break;
            }
            dir_type = DIR_NEXT;

            if ((eh_dir_layer == 0) && (eh_fsel_filter_record_dir_flag == TRUE))//ֻ����˸�Ŀ¼�µ�RECORD��ALARMĿ¼
            {
                if (TRUE == filtrate_dir())
                {
                    continue; //�����˵��ˣ���Ҫ����������һ��Ŀ¼
                }
                else
                {
                    vfs_dir_ret = TRUE;
                    break;
                }
            }
            else
            {
                vfs_dir_ret = TRUE;
                break;
            }
        }

        if (vfs_dir_ret == TRUE)
        {
            //�������ļ��У���ø����ļ��е��ļ�����
            if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                //Ŀ¼�㼶����
                eh_dir_layer++;
                //��ǰĿ¼�µ��ļ���Ź���
                eh_file_no = 0;
                // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
                if (FALSE != fsel_total_dir_next())
                {
                    return TRUE;
                }
                else //�����Ŀ¼
                {
                    goto CurrentDiragain;
                }
            }
            else //������
            {
                return FALSE;
            }
        }
        else //���û�����ļ��У���������һ�㣬Ѱ���뵱ǰĿ¼ƽ������һ��Ŀ¼��ƽ��Ѱ�ң�
        {
            ParentDiragainNext:
            if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
            {
                return FALSE;
            }
            //�����ǰ�Ǹ�Ŀ¼�򷵻�
            if (eh_dir_layer == 0)
            {
                return FALSE;
            }
            //������Ŀ¼
            if (FALSE == vfs_cd(eh_vfs_mount, CD_BACK, NULL))
            {
                //�쳣����
                return FALSE;
            }
            //Ŀ¼�㼶��
            eh_dir_layer--;

            vfs_dir_ret = FALSE;

            while (1)
            {
                if (0 == vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR))
                {
                    break;
                }

                if ((eh_dir_layer == 0) && (eh_fsel_filter_record_dir_flag == TRUE))//ֻ����˸�Ŀ¼�µ�RECORD��ALARMĿ¼
                {
                    if (TRUE == filtrate_dir())
                    {
                        continue; //�����˵��ˣ���Ҫ����������һ��Ŀ¼
                    }
                    else
                    {
                        vfs_dir_ret = TRUE;
                        break;
                    }
                }
                else
                {
                    vfs_dir_ret = TRUE;
                    break;
                }
            }

            if (vfs_dir_ret == TRUE)
            {
                //����Ŀ¼,���Ҹ�Ŀ¼�µ��ļ�
                if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
                {
                    //Ŀ¼�㼶����
                    eh_dir_layer++;
                    //�ļ����Ϊ0
                    eh_file_no = 0;

                    if (FALSE != fsel_total_dir_next())
                    {
                        return TRUE;
                    }
                    else
                    {
                        // �����Ŀ¼.
                        goto CurrentDiragain;
                    }
                }
                else //������
                {
                    return FALSE;
                }
            }
            else
            {
                goto ParentDiragainNext;
            }
        }
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
uint16 fsel_total_file_all(void)
{
    uint8 temp_dir_layer;
    //bool save_shuffle_flag;
    uint16 total;
    //uint32 ext_name;

    uint16 save_cur_no, save_cur_total;
    uint16 save_file_no_all;
    pdir_layer_t *pdir_layer_total_tmp;
    uint32 dir_start_timer = sys_get_ab_timer();

    if (eh_vfs_mount == 0)
    {
        return 0;
    }

    //����ѿռ�
    pdir_layer_total_tmp = (pdir_layer_t *) sys_malloc(sizeof(pdir_layer_t));
    if (pdir_layer_total_tmp == NULL)
    {
        return 0;
    }

    //���浱ǰĿ¼���ļ���ź�����
    save_cur_no = eh_file_no;
    save_cur_total = eh_file_total;
    temp_dir_layer = eh_dir_layer;

    save_file_no_all = eh_file_no_all;

    //save current path
    vfs_file_dir_offset(eh_vfs_mount, pdir_layer_total_tmp, &pfile_offset_total, 0);

    fsel_init_sysdata();

    total = 0;
    do
    {
        //scan disk for all file
        if (fsel_total_next_layer() != FALSE)
        {
            total++;
            //support 9999
            if (total >= SUPPORT_MUSIC_FILE)
            {
                break;
            }
        }
        else
        {
            break;
        }

        if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
        {
            break;
        }
    }
    while (1);

    //back up path
    vfs_file_dir_offset(eh_vfs_mount, pdir_layer_total_tmp, &pfile_offset_total, 1);

    //�ͷŶѿռ�
    sys_free(pdir_layer_total_tmp);
    pdir_layer_total_tmp = NULL;

    //eh_shuffle_sw = save_shuffle_flag;

#ifdef SUPPORT_SHUFFLE_MODE
    if ((TRUE == eh_shuffle_sw) && (total > 0))
    {
        fsel_init_shuffle(total);
    }
#endif

    //�ָ���ǰĿ¼����ź�����
    eh_file_no = save_cur_no;
    eh_file_total = save_cur_total;
    eh_dir_layer = temp_dir_layer;

    eh_file_no_all = save_file_no_all;

    eh_file_total_all = total;

    return total;

}

/******************************************************************************
 * \Description: ������Ż�ȡ�ļ�(�������ֵ��,���Ϊȫ�����)
 * \bool fsel_dir_get_byno_all(uint8 *strfile, uint16 num)
 * \
 * \param[in] strfile param1 ��ź�׺��ָ��
 * \param[in] num param2 �ļ����

 * \param[out] none

 * \return  bool TRUE �ɹ�
 FALSE ʧ��

 * \note
 *******************************************************************************/
bool fsel_dir_get_byno_all(uint8 *strfile, uint16 num)
{
    uint8 dir_type;

    //save shuffle flag;
    uint8 save_shuffle_flag;

    bool ret_val = TRUE;

    uint16 dir_num, i;
    uint32 dir_start_timer = sys_get_ab_timer();

    //�����δ��ȡ�����ļ���,��ɨ�����
    if (eh_file_total_all == 0)
    {
        eh_file_total_all = fsel_total_file_all();
    }

    //��Ч�����ֱ�ӷ���ʧ��
    if ((num > eh_file_total_all) || (num == 0))
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }
    save_shuffle_flag = eh_shuffle_sw;
    eh_shuffle_sw = FALSE;
    if (eh_file_no_all < num)
    {
        //���ڵ�ǰ�ļ��ţ����Dir
        dir_num = num - eh_file_no_all;
        i = 0;
        while (i < dir_num)
        {
            //���dir�ļ�
            if (fsel_dir_next_layer((uint8 *) &eh_cur_ext_name) != FALSE)
            {
                i++;
            }
            else
            {
                ret_val = FALSE;
                break;
            }

            if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
            {
                break;
            }
        }
    }
    else if (eh_file_no_all > num)
    {
        //С�ڵ�ǰ�ļ��ţ���ǰDir
        dir_num = eh_file_no_all - num;

        i = 0;

        while (i < dir_num)
        {
            //��ǰdir�ļ�
            if (fsel_dir_prev_layer((uint8 *) &eh_cur_ext_name) != FALSE)
            {
                i++;
            }
            else
            {
                //return FALSE;
                ret_val = FALSE;
                break;
            }

            if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
            {
                break;
            }
        }
    }
    else
    {
        //�͵�ǰ���ļ������
        if (eh_file_no_all == 1)
        {
            //��һ���ļ�
            dir_type = DIR_HEAD;
            eh_file_no = 0;

            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 == eh_cur_ext_name)
            {
                //return FALSE;
                ret_val = FALSE;
            }
        }
    }
    if (ret_val == TRUE)
    {
        eh_file_no_all = num;
        if (strfile != NULL)
        {
            *(uint32 *) strfile = eh_cur_ext_name;
        }
    }
    eh_shuffle_sw = save_shuffle_flag;
    return ret_val;
}

/******************************************************************************
 * \Description: ������Ż�ȡ�ļ�
 * \bool fsel_dir_get_byno(uint8 *strfile, uint16 num)
 * \
 * \param[in] strfile param1 ��ź�׺��ָ��
 * \param[in] num param2 �ļ����

 * \param[out] none

 * \return  bool TRUE �ɹ�
 FALSE ʧ��

 * \note  ��ı�eh_file_no��ֵ;ȡ�ļ�ʱ����ǰ���ܸı�ǰһ�׵�eh_file_no
 ��Ϊ�ýӿ�Ҫ����eh_file_no�߲�ͬ������
 *******************************************************************************/
bool fsel_dir_get_byno(uint8 *strfile, uint16 num)
{
    uint8 dir_type;

    uint16 dir_num;
    uint32 dir_start_timer = sys_get_ab_timer();

    if ((num > eh_file_total) || (num == 0))
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_file_no == 0)
    {
        //��ͷ��ʼdir
        dir_type = DIR_HEAD;
    }
    else
    {
        //ȡ��һ��
        dir_type = DIR_NEXT;
    }
    if (eh_file_no < num)
    {
        //���ڵ�ǰ�ļ��ţ����Dir
        dir_num = num - eh_file_no;

        eh_file_no = 0;
        while (eh_file_no < dir_num)
        {
            //���dir�ļ�
            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            //�ҵ��ļ�
            if (0 != eh_cur_ext_name)
            {
                eh_file_no++;
            }
            else
            {
                return FALSE;
            }
            dir_type = DIR_NEXT;

            if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
            {
                break;
            }
        }
    }
    else if (eh_file_no > num)
    {
        //С�ڵ�ǰ�ļ��ţ���ǰDir
        dir_type = DIR_PREV;

        dir_num = eh_file_no - num;

        eh_file_no = 0;

        while (eh_file_no < dir_num)
        {
            //��ǰdir�ļ�
            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);

            if (0 != eh_cur_ext_name)
            {
                eh_file_no++;
            }
            else
            {
                return FALSE;
            }

            if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
            {
                break;
            }
        }
    }
    else
    {
        //�͵�ǰ���ļ������
        if (eh_file_no == 1)
        {
            //��һ���ļ�
            dir_type = DIR_HEAD;
            eh_file_no = 0;

            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 == eh_cur_ext_name)
            {
                return FALSE;
            }
        }
    }

    eh_file_no = num;
    eh_file_no_all = eh_file_no;
    if (strfile != NULL)
    {
        *(uint32 *) strfile = eh_cur_ext_name;
    }
    return TRUE;
}

/******************************************************************************

 * \par  Description:��ȡ��ǰ�ļ�·����Ϣ
 * \bool fsel_dir_get_location(file_location_t *location)
 * \param[in]  location param1 ���Ŀ¼·����Ϣָ��
 * \param[in]  type param2 no use now
 * \param[out]  none
 * \return  bool TRUE �ɹ�
 FALSE ʧ��
 * \ingroup      eh_fsel_directory_func.c
 * \note
 *******************************************************************************/
bool fsel_dir_get_location(file_location_t *location)
{

    *(uint32 *) &location->filename = eh_cur_ext_name;
    //��ȡ��ǰ�ļ���·����Ϣ
    if (FALSE == vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, (pfile_offset_t *) &location->cluster_no, 0))//cluster_no&dir_entry
    {
        return FALSE;
    }

    location->disk = fsel_init_val.disk;
    location->dir_layer = eh_dir_layer;

    if (eh_file_total_all == 0)
    {
        //��ǰĿ¼�ļ�����
        location->file_total = eh_file_total;
    }
    else
    {
        location->file_total = eh_file_total_all;
    }
    //�ļ����
    location->file_num = eh_file_no_all;

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:���õ�ǰ·����Ϣ
 * \bool fsel_dir_set_location(file_location_t *location)
 * \param[in]  location param1 ���·����Ϣָ��
 * \param[in]  type param2 no use now
 * \param[out]  none
 * \return  bool TRUE �ɹ�
 FALSE ʧ��

 * \ingroup      eh_fsel_directory_func.c
 * \note
 *******************************************************************************/
bool fsel_dir_set_location(file_location_t *location)
{
    bool ret_val = TRUE;
    uint8 bret = 0;
    
    //��������locationΪ��Чֵ
    if ((location == NULL) || (*(uint32 *) &location->filename == 0))
    {
        if (eh_scan_mode != 0) //music_common.h -> ENTER_MUSIC_START
        {
            //��ǰĿ¼�µ��ļ�����
            eh_file_total = fsel_total_file_cur(NULL, 0);
            //�ļ����
            eh_file_no = 0;
            return FALSE;
        }
        else
        {
            ret_val = FALSE;
        }
    }

    if (ret_val == TRUE)
    {
        //��λ�ļ�
        ret_val = vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, (pfile_offset_t *) &location->cluster_no, 1);//cluster_no&dir_entry
    }
    
    if (ret_val != FALSE)
    {
        vfs_get_name(eh_vfs_mount, (uint8 *) &eh_cur_ext_name, 0);
        if (*(uint32 *) &location->filename != eh_cur_ext_name)
        {
            ret_val = FALSE;
        }
    }

    if (ret_val == FALSE)
    {
        fsel_init_sysdata();
        eh_file_total_all = 0;
        disk_scan_cur_dir_first_file_flag = TRUE;
        bret = fsel_get_nextfile(location->filename);
        disk_scan_cur_dir_first_file_flag = FALSE;

        if (bret == TRUE)
        {
            //��ȡ��ǰ�ļ���location��Ϣ
            bret = fsel_get_location(location);        }
        else
        {
            return FALSE;
        }
    }

    fsel_init_sysdata();
    eh_file_total_all = 0;
    
    //����ȫ��ɨ��
    if (eh_scan_mode == 0) //music_common.h -> ENTER_MUSIC_START
    {
        creat_disk_scan_thread(location); //create thread and wait thread quit
    }

    if (ret_val == FALSE)
    {
        //Ŀ¼�ջ�Ŀ¼��Ϣ���󣬻ص���Ŀ¼
        fsel_init_sysdata();

        return FALSE;
    }
    else
    {
        ret_val = vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, (pfile_offset_t *) &location->cluster_no, 1);//cluster_no&dir_entry
    }

    eh_dir_layer = location->dir_layer;

    //��ֵ����eh_cur_ext_name
    libc_memcpy(&eh_cur_ext_name, location->filename, 4);

    //�����ϵ㲥��
    return TRUE;
}

