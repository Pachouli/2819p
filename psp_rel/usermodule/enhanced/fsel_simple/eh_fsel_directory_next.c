/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-31 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

#define FILTER_DIR_NUM  2
#define DIR_NAME_LEN    8 //段名目录名长度

const uint8 root_str[] = { 'R', 'O', 'O', 'T', 0 };
const uint8 rec_dir_name[] = "RECORD  ";
const uint8 alarm_dir_name[] = "ALARM   ";

/******************************************************************************/
/*
 * \par  Description:过滤根目录下的RECORD目录
 *
 * \param[in]

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool filtrate_dir(void)
{
    uint8 *buf;
    uint16 name_len = 0;
    uint8 *filter_dir_name;
    uint8 dir_name_buffer[32];
    uint8 i, j, dir_index;
    bool is_unicode;
    bool match_flag;

    if (eh_dir_layer != 0)
    {
        return FALSE;
    }

    name_len = vfs_get_name(eh_vfs_mount, dir_name_buffer, 16);
    if (name_len == 0)
    {
        return FALSE;
    }

    is_unicode = FALSE;
    if ((dir_name_buffer[0] == 0xff) && (dir_name_buffer[1] == 0xfe))
    {
        is_unicode = TRUE;
    }

    if (is_unicode == FALSE)
    {
        //8+3 短名
        buf = dir_name_buffer;
        name_len = 8;
    }
    else
    {
        if (name_len > 2)
        {
            //unciode
            buf = dir_name_buffer + 2;
            name_len = name_len - 2;
        }
        else
        {
            return FALSE;
        }
    }

    //0:normal play 2:record 3:alarm 4:dir1 5:dir2
    for (dir_index = 0; dir_index < FILTER_DIR_NUM; dir_index++)
    {
        if (dir_index == 0)
        {
            filter_dir_name = rec_dir_name;
        }
        else if (dir_index == 1)
        {
            filter_dir_name = alarm_dir_name;
        }
        else if (dir_index == 2)
        {
            filter_dir_name = fsel_init_val.dir1;
        }
        else if (dir_index == 3)
        {
            filter_dir_name = fsel_init_val.dir2;
        }
        else
        {
            return FALSE;
        }

        match_flag = TRUE;
        i = 0;
        for (j = 0; ((j < name_len) && (j < DIR_NAME_LEN)); j++)
        {
            if (filter_dir_name[j] != buf[i])
            {
                match_flag = FALSE;
                break;
            }
            if (is_unicode == FALSE)
            {
                i++;
            }
            else
            {
                i += 2;
            }
        }
        if ((match_flag == TRUE)
            && (j >= name_len)
            && ((filter_dir_name[j] == 0x00) || (filter_dir_name[j] == ' ')))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_next(uint8 *strfile)
 * \获取当前目录的下一个文件
 * \param[in]    strfile  para1 存放文件后缀名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_fun.c
 * \note
 */
/*******************************************************************************/
bool fsel_dir_next(uint8 *strfile)
{
    uint8 dir_type;
    uint16 temp_num;

    if (eh_file_total == 0)
    {
        return FALSE;
    }

#ifdef SUPPORT_SHUFFLE_MODE
    //如果shuffle打开则
    if (TRUE == eh_shuffle_sw)
    {
        temp_num = fsel_shuffle_next_file();
        if (temp_num == 0)
        {
            fsel_init_shuffle (eh_file_total_all);
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
        return fsel_dir_get_byno_all(strfile, temp_num);
    }
#endif

    //表示当前已是最后一个，结束
    if (eh_file_no == eh_file_total)
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
        //dir下一个
        dir_type = DIR_NEXT;
    }
    //调用文件系统接口
    eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
    if (0 != eh_cur_ext_name)
    {
        eh_file_no++;
#if 1
        //只有一个目录,解决录音文件播放的问题
        if (eh_file_total_all == eh_file_total)
        {
            eh_file_no_all = eh_file_no;
        }
        else
        {
            //磁盘文件序号
            eh_file_no_all++;
            if ((eh_file_total_all < eh_file_total) && (disk_scan_cur_dir_first_file_flag == FALSE))
            {
                eh_file_total_all = fsel_total_file_all();
                libc_printf("all disk file: %d", eh_file_total_all);
            }
        }
#else
        eh_file_no_all = eh_file_no;
#endif

        if (strfile != NULL)
        {
            *(uint32 *) strfile = eh_cur_ext_name;
        }
        return TRUE;
    }
    else
    {
        fsel_error = FSEL_ERR_FAIL;
    }
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_next_layer(uint8 *strfile)
 * \获取下一个文件,可以切换目录
 * \param[in]    strfile  para1 存放文件后缀名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_next.c
 * \note
 */
/*******************************************************************************/
bool fsel_dir_next_layer(uint8 *strfile)
{
    bool vfs_dir_ret = FALSE;
    uint8 dir_type;

    //在当前目录向下找文件
    if (eh_file_total == 0)
    {
        eh_file_total = fsel_total_file_cur(NULL, 0);
        eh_file_no = 0;
    }

    if (fsel_dir_next(strfile) == FALSE)
    {
        CurrentDiragain:
        if (eh_dir_layer >= (DIRLAYER_MAX - 1))
        {
            goto ParentDiragainNext;
        }

        vfs_dir_ret = FALSE;
        //寻找该目录的第一个子文件夹
        dir_type = DIR_HEAD;
        while (1)
        {
            if (0 == vfs_dir(eh_vfs_mount, dir_type, NULL, EH_GET_EXT_DIR))
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
            if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                //进入子文件夹，获得该子文件夹的文件总数
                eh_file_total = fsel_total_file_cur(NULL, 0);
                eh_dir_layer++;

                eh_file_no = 0;
                // 检查当前目录是否有合适的文件
                if ((eh_file_total > 0) && (fsel_dir_next(strfile) != FALSE))
                {
                    fsel_error = 0;
                    return TRUE;
                }
                else
                {
                    //检查子目录
                    goto CurrentDiragain;
                }
            }
            else //出错了
            {
                fsel_init_sysdata();
                fsel_error = FSEL_ERR_OUTOF;
                return FALSE;
            }
        }
        else //如果没有子文件夹，则向上跳一层，寻找当前目录的下一个目录（平级寻找）
        {
            ParentDiragainNext:

            //当前是根目录
            if (0 == eh_dir_layer)
            {
                fsel_init_sysdata();
                fsel_error = FSEL_ERR_OUTOF;
                return FALSE;
            }

            if (vfs_cd(eh_vfs_mount, CD_BACK, NULL) != FALSE)
            {
                eh_dir_layer--;

                vfs_dir_ret = FALSE;

                while (1)
                {
                    if (0 == vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EH_GET_EXT_DIR))
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
                    vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                    eh_dir_layer++;
                    //切换目录
                    eh_file_total = fsel_total_file_cur(NULL, 0);

                    eh_file_no = 0;

                    if ((eh_file_total > 0) && (FALSE != fsel_dir_next(strfile)))
                    {
                        fsel_error = 0;
                        return TRUE;
                    }
                    else
                    {
                        // 检查子目录.
                        goto CurrentDiragain;
                    }
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }
            else
            {
                //异常出错
                fsel_init_sysdata();
                fsel_error = FSEL_ERR_EXCEPTION;
                return FALSE;
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
/*static uint16 total_file_all(uint8* dir_info, uint8 check_flag)
 {
 uint8 temp_offset_buf[8];
 uint16 total;
 uint32 ext_name;
 pdir_layer_t *pdir_layer_total_tmp;

 //申请堆空间
 pdir_layer_total_tmp = (pdir_layer_t *) sys_malloc(sizeof(pdir_layer_t));
 if (pdir_layer_total_tmp == NULL)
 {
 return 0;
 }

 //save current path
 vfs_file_dir_offset(vfs_mount, pdir_layer_total_tmp, &pfile_offset_buffer, 0);

 init_fsel_sysdata();
 file_total = 0;
 total = 0;
 do
 {
 if (FALSE != dir_next_layer((uint8*) &ext_name))
 {
 total++;
 if (check_flag == 1)
 {
 //比较8字节offset
 vfs_file_dir_offset(vfs_mount, NULL, &temp_offset_buf, 0);
 if (libc_memcmp(dir_info, &temp_offset_buf, 8) == 0)
 {
 file_no = total;
 check_flag = 0;
 }
 }
 }
 else
 {
 break;
 }
 } while (1);

 //back up path
 vfs_file_dir_offset(vfs_mount, pdir_layer_total_tmp, &pfile_offset_buffer, 1);

 //释放堆空间
 sys_free(pdir_layer_total_tmp);
 pdir_layer_total_tmp = NULL;

 if (check_flag == 1)
 {
 file_no = 0;
 }

 if (TRUE == shuffle_sw)
 {
 init_shuffle(file_total);
 }

 return total;

 }*/

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_get_next(uint8 *strfile)
 * \获取下一个文件
 * \param[in]    strfile  para1 存放文件后缀名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_next.c
 * \note
 * \
*******************************************************************************/
bool fsel_dir_get_next(uint8 *strfile)
{
    bool ret_val;

    switch (fsel_init_val.fsel_mode)
    {
        //重复当前一个文件
        case FSEL_MODE_LOOPONE:

        if (eh_file_no == 0)
        {
            //默认取第一首
            ret_val = fsel_dir_get_byno(strfile, 1);
        }
        else
        {
            //之前已取,直接返回ext
            if (strfile != NULL)
            {
                *(uint32 *) strfile = eh_cur_ext_name;
            }
            ret_val = TRUE;
        }
        break;

        //所有文件顺序读取，不重复
        case FSEL_MODE_NORMAL:
        {
            ret_val = fsel_dir_next_layer(strfile);
            break;
        }

        //所有文件顺序读取，重复
        case FSEL_MODE_LOOPALL:
        if (FALSE != fsel_dir_next_layer(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            fsel_init_sysdata();
            ret_val = fsel_dir_next_layer(strfile);
        }
        break;

        //当前目录重复播放
        case FSEL_MODE_LOOPONEDIR:

        if (FALSE != fsel_dir_next(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            eh_file_no = 0;
            //获取当前目录第一首
            ret_val = fsel_dir_next(strfile);
        }
        break;

        default:
        ret_val = FALSE;
        break;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:查找下一个目录
 * \bool fsel_dir_next_dir(uint8 *strdir)
 * \param[in] strdir param1 存放目录名称的指针
 * \param[out] none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_next.c
 * \note
 *******************************************************************************/
bool fsel_dir_next_dir(uint8 *strdir)
{

    //uint8 dir_type = DIR_NEXT;

    //file no为该目录最后一首歌
    //下一个目录第一个文件
    eh_file_no_all -= eh_file_no;
    eh_file_no_all += eh_file_total;

    //定位到当前目录最后一首,切换歌曲
    eh_file_no = eh_file_total;

    if (fsel_dir_get_next((uint8 *) &eh_cur_ext_name) == TRUE)
    {
        if (eh_cur_ext_name != 0)
        {
            if (strdir != NULL)
            {
                *(uint32 *) strdir = eh_cur_ext_name;
            }
            return TRUE;
        }
    }
    //如果没有找到则返回失败
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:获取当前目录名
 * \bool fsel_get_dir_name(uint8 *strdir,uint8 name_len,file_location_t *location)
 * \param[in] strdir param1 存放目录名称的指针
 * \param[out] none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_next.c
 * \note
 *******************************************************************************/
bool fsel_get_dir_name(uint8 *strdir, uint8 name_len, file_location_t *location)
{
    bool ret_len = FALSE;
    pdir_layer_t *pdir_layer_total_tmp;

    if (strdir == NULL)
    {
        return FALSE;
    }
    //获取当前文件的路径信息
    if (location->dir_layer == 0)
    {
        libc_memcpy(strdir, root_str, sizeof(root_str));
        return TRUE;
    }

    //申请堆空间
    pdir_layer_total_tmp = (pdir_layer_t *) sys_malloc(sizeof(pdir_layer_t));
    if (pdir_layer_total_tmp == NULL)
    {
        return FALSE;
    }

    //save current path
    vfs_file_dir_offset(eh_vfs_mount, pdir_layer_total_tmp, &pfile_offset_total, 0);

    //set to cur dir entry
    if (vfs_cd(eh_vfs_mount, CD_BACK, NULL) == TRUE)
    {
        ret_len = (uint8) vfs_get_name(eh_vfs_mount, strdir, name_len);
    }

    //back up path
    vfs_file_dir_offset(eh_vfs_mount, pdir_layer_total_tmp, &pfile_offset_total, 1);

    //释放堆空间
    sys_free(pdir_layer_total_tmp);
    pdir_layer_total_tmp = NULL;

    return ret_len;

}
