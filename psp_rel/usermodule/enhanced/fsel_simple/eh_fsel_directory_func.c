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
 * \par  Description: 获取当前目录下的下个文件
 *
 * \param[in]
 * \param[out]      strfile--存放文件的扩展名
 * \return          TRUE OR FALSE
 * \note
 *******************************************************************************/
bool fsel_total_dir_next(void)
{
    uint8 dir_type;
    if (eh_file_no == 0)
    {
        //序号为0则从头开始搜索
        dir_type = DIR_HEAD;
    }
    else
    {
        //序号不为0则dir下一个
        dir_type = DIR_NEXT;
    }
    //dir文件
    if (vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit) != 0)
    {
        //找到文件则序号增加
        eh_file_no++;
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: 获取磁盘的下一个文件
 *
 * \param[in]

 * \param[out]        strfile--存放文件的扩展名

 * \return            TRUE OR FALSE

 * \note
 *******************************************************************************/

bool fsel_total_next_layer(void)
{
    bool vfs_dir_ret = FALSE;
    uint8 dir_type;
    uint32 dir_start_timer = sys_get_ab_timer();

    //在当前目录向下找文件
    if (FALSE == fsel_total_dir_next())
    {
        CurrentDiragain:
        if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
        {
            return FALSE;
        }
        if (eh_dir_layer >= (MAX_DIR_LAYER - 1)) //如果当前是8级目录就不找子文件夹
        {
            goto ParentDiragainNext;
        }

        vfs_dir_ret = FALSE;
        //寻找该目录的第一个子文件夹
        dir_type = DIR_HEAD;
        while (1)
        {
            if (0 == vfs_dir(eh_vfs_mount, dir_type, NULL, EXT_NAME_ALL_DIR))
            {
                break;
            }
            dir_type = DIR_NEXT;

            if ((eh_dir_layer == 0) && (eh_fsel_filter_record_dir_flag == TRUE))//只需过滤根目录下的RECORD和ALARM目录
            {
                if (TRUE == filtrate_dir())
                {
                    continue; //被过滤掉了，需要继续查找下一个目录
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
            //进入子文件夹，获得该子文件夹的文件总数
            if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                //目录层级增加
                eh_dir_layer++;
                //当前目录下的文件序号归零
                eh_file_no = 0;
                // 检查当前目录是否有合适的文件
                if (FALSE != fsel_total_dir_next())
                {
                    return TRUE;
                }
                else //检查子目录
                {
                    goto CurrentDiragain;
                }
            }
            else //出错了
            {
                return FALSE;
            }
        }
        else //如果没有子文件夹，则向上跳一层，寻找与当前目录平级的下一个目录（平级寻找）
        {
            ParentDiragainNext:
            if ((sys_get_ab_timer() - dir_start_timer) > DIR_OVERTIME)
            {
                return FALSE;
            }
            //如果当前是根目录则返回
            if (eh_dir_layer == 0)
            {
                return FALSE;
            }
            //跳到父目录
            if (FALSE == vfs_cd(eh_vfs_mount, CD_BACK, NULL))
            {
                //异常出错
                return FALSE;
            }
            //目录层级减
            eh_dir_layer--;

            vfs_dir_ret = FALSE;

            while (1)
            {
                if (0 == vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR))
                {
                    break;
                }

                if ((eh_dir_layer == 0) && (eh_fsel_filter_record_dir_flag == TRUE))//只需过滤根目录下的RECORD和ALARM目录
                {
                    if (TRUE == filtrate_dir())
                    {
                        continue; //被过滤掉了，需要继续查找下一个目录
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
                //进入目录,查找该目录下的文件
                if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
                {
                    //目录层级增加
                    eh_dir_layer++;
                    //文件序号为0
                    eh_file_no = 0;

                    if (FALSE != fsel_total_dir_next())
                    {
                        return TRUE;
                    }
                    else
                    {
                        // 检查子目录.
                        goto CurrentDiragain;
                    }
                }
                else //出错了
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

    //申请堆空间
    pdir_layer_total_tmp = (pdir_layer_t *) sys_malloc(sizeof(pdir_layer_t));
    if (pdir_layer_total_tmp == NULL)
    {
        return 0;
    }

    //保存当前目录的文件序号和总数
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

    //释放堆空间
    sys_free(pdir_layer_total_tmp);
    pdir_layer_total_tmp = NULL;

    //eh_shuffle_sw = save_shuffle_flag;

#ifdef SUPPORT_SHUFFLE_MODE
    if ((TRUE == eh_shuffle_sw) && (total > 0))
    {
        fsel_init_shuffle(total);
    }
#endif

    //恢复当前目录的序号和总数
    eh_file_no = save_cur_no;
    eh_file_total = save_cur_total;
    eh_dir_layer = temp_dir_layer;

    eh_file_no_all = save_file_no_all;

    eh_file_total_all = total;

    return total;

}

/******************************************************************************
 * \Description: 根据序号获取文件(用于数字点歌,序号为全盘序号)
 * \bool fsel_dir_get_byno_all(uint8 *strfile, uint16 num)
 * \
 * \param[in] strfile param1 存放后缀名指针
 * \param[in] num param2 文件序号

 * \param[out] none

 * \return  bool TRUE 成功
 FALSE 失败

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

    //如果从未获取过总文件数,则扫描磁盘
    if (eh_file_total_all == 0)
    {
        eh_file_total_all = fsel_total_file_all();
    }

    //无效序号则直接返回失败
    if ((num > eh_file_total_all) || (num == 0))
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }
    save_shuffle_flag = eh_shuffle_sw;
    eh_shuffle_sw = FALSE;
    if (eh_file_no_all < num)
    {
        //大于当前文件号，向后Dir
        dir_num = num - eh_file_no_all;
        i = 0;
        while (i < dir_num)
        {
            //向后dir文件
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
        //小于当前文件号，向前Dir
        dir_num = eh_file_no_all - num;

        i = 0;

        while (i < dir_num)
        {
            //向前dir文件
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
        //和当前的文件号相等
        if (eh_file_no_all == 1)
        {
            //第一个文件
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
 * \Description: 根据序号获取文件
 * \bool fsel_dir_get_byno(uint8 *strfile, uint16 num)
 * \
 * \param[in] strfile param1 存放后缀名指针
 * \param[in] num param2 文件序号

 * \param[out] none

 * \return  bool TRUE 成功
 FALSE 失败

 * \note  会改变eh_file_no的值;取文件时调用前不能改变前一首的eh_file_no
 因为该接口要根据eh_file_no走不同的流程
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
        //从头开始dir
        dir_type = DIR_HEAD;
    }
    else
    {
        //取下一个
        dir_type = DIR_NEXT;
    }
    if (eh_file_no < num)
    {
        //大于当前文件号，向后Dir
        dir_num = num - eh_file_no;

        eh_file_no = 0;
        while (eh_file_no < dir_num)
        {
            //向后dir文件
            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            //找到文件
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
        //小于当前文件号，向前Dir
        dir_type = DIR_PREV;

        dir_num = eh_file_no - num;

        eh_file_no = 0;

        while (eh_file_no < dir_num)
        {
            //向前dir文件
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
        //和当前的文件号相等
        if (eh_file_no == 1)
        {
            //第一个文件
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

 * \par  Description:获取当前文件路径信息
 * \bool fsel_dir_get_location(file_location_t *location)
 * \param[in]  location param1 存放目录路径信息指针
 * \param[in]  type param2 no use now
 * \param[out]  none
 * \return  bool TRUE 成功
 FALSE 失败
 * \ingroup      eh_fsel_directory_func.c
 * \note
 *******************************************************************************/
bool fsel_dir_get_location(file_location_t *location)
{

    *(uint32 *) &location->filename = eh_cur_ext_name;
    //获取当前文件的路径信息
    if (FALSE == vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, (pfile_offset_t *) &location->cluster_no, 0))//cluster_no&dir_entry
    {
        return FALSE;
    }

    location->disk = fsel_init_val.disk;
    location->dir_layer = eh_dir_layer;

    if (eh_file_total_all == 0)
    {
        //当前目录文件总数
        location->file_total = eh_file_total;
    }
    else
    {
        location->file_total = eh_file_total_all;
    }
    //文件序号
    location->file_num = eh_file_no_all;

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:设置当前路径信息
 * \bool fsel_dir_set_location(file_location_t *location)
 * \param[in]  location param1 存放路径信息指针
 * \param[in]  type param2 no use now
 * \param[out]  none
 * \return  bool TRUE 成功
 FALSE 失败

 * \ingroup      eh_fsel_directory_func.c
 * \note
 *******************************************************************************/
bool fsel_dir_set_location(file_location_t *location)
{
    bool ret_val = TRUE;
    uint8 bret = 0;
    
    //如果输入的location为无效值
    if ((location == NULL) || (*(uint32 *) &location->filename == 0))
    {
        if (eh_scan_mode != 0) //music_common.h -> ENTER_MUSIC_START
        {
            //当前目录下的文件总数
            eh_file_total = fsel_total_file_cur(NULL, 0);
            //文件序号
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
        //定位文件
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
            //获取当前文件的location信息
            bret = fsel_get_location(location);        }
        else
        {
            return FALSE;
        }
    }

    fsel_init_sysdata();
    eh_file_total_all = 0;
    
    //快速全盘扫描
    if (eh_scan_mode == 0) //music_common.h -> ENTER_MUSIC_START
    {
        creat_disk_scan_thread(location); //create thread and wait thread quit
    }

    if (ret_val == FALSE)
    {
        //目录空或目录信息有误，回到根目录
        fsel_init_sysdata();

        return FALSE;
    }
    else
    {
        ret_val = vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, (pfile_offset_t *) &location->cluster_no, 1);//cluster_no&dir_entry
    }

    eh_dir_layer = location->dir_layer;

    //赋值更新eh_cur_ext_name
    libc_memcpy(&eh_cur_ext_name, location->filename, 4);

    //启动断点播放
    return TRUE;
}

