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

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_prev(uint8 *strfile)
 * \获取当前目录中上一个文件
 * \param[in]    strfile  para1 存放文件后缀名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_perv.c
 * \note
 */
/*******************************************************************************/
bool fsel_dir_prev(uint8 *strfile)
{
    uint8 dir_type = DIR_PREV;
    uint16 temp_num;
    //文件总数为0
    if (eh_file_total == 0)
    {
        return FALSE;
    }

#ifdef SUPPORT_SHUFFLE_MODE
    //如果是shuffle
    if (TRUE == eh_shuffle_sw)
    {
        //shuffle
        temp_num = fsel_shuffle_prev_file();
        if (temp_num == 0)
        {
            fsel_init_shuffle (eh_file_total_all);
            return FALSE;
        }
        return fsel_dir_get_byno_all(strfile, temp_num);
    }
#endif

    //表示前一个已经目录的第一个
    if (eh_file_no == 1)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_file_no == 0) //表示刚进入该目录
    {
        eh_file_no = eh_file_total;
        //定位到最后一个目录项
        dir_type = DIR_TAIL;
    }
    else
    {
        eh_file_no--;
    }

    //dir文件
    eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
    if (eh_cur_ext_name != 0)
    {
        if ((eh_file_no_all == 0) || (eh_file_no_all == 1))
        {
            //如果从未获取过总文件数,则扫描磁盘
            if (eh_file_total_all == 0)
            {
                eh_file_total_all = fsel_total_file_all();
            }
            eh_file_no_all = eh_file_total_all;
        }
        else
        {
            //序号用于显示
            eh_file_no_all--;
        }
        if (strfile != NULL)
        {
            *(uint32 *) strfile = eh_cur_ext_name;
        }
        return TRUE;
    }

    fsel_error = FSEL_ERR_FAIL;
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:定位到当前目录的最后一个文件夹目录项位置
 * \bool fsel_last_dir(void)
 * \param[in] void
 * \param[out] none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 *******************************************************************************/
bool fsel_last_dir(void)
{
    bool vfs_dir_ret = FALSE;
    uint8 dir_type;

    //目录已是最大层级,返回失败
    if (eh_dir_layer >= (DIRLAYER_MAX - 1))
    {
        return FALSE;
    }
    // if no dir ,return error
    dir_type = DIR_TAIL;
    while (1)
    {
        if (0 == vfs_dir(eh_vfs_mount, dir_type, NULL, EH_GET_EXT_DIR))
        {
            break;
        }
        dir_type = DIR_PREV;

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

    return vfs_dir_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_prev_layer(uint8 *strfile)
 * \获取上一个文件,可以切换目录
 * \param[in]    strfile  para1 存放文件后缀名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_prev.c
 * \note
 */
/*******************************************************************************/
bool fsel_dir_prev_layer(uint8 *strfile)
{

    //取得当前目录文件总数
    if (eh_file_total == 0)
    {
        eh_file_total = fsel_total_file_cur(NULL, 0);
    }

    if (FALSE == fsel_dir_prev(strfile))
    {
        //检查父目录
        ParentDiragainNext:
        //如果是根目录则返回
        if (eh_dir_layer == 0)
        {
            return FALSE;
        }
        //向上一级
        if (FALSE != vfs_cd(eh_vfs_mount, CD_BACK, NULL))
        {
            bool vfs_dir_ret = FALSE;

            eh_dir_layer--;

            //查找目录
            while (1)
            {
                if (0 == vfs_dir(eh_vfs_mount, DIR_PREV, NULL, EH_GET_EXT_DIR))
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
                //进入子目录
                vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                //目录层级
                eh_dir_layer++;
                //定位到当前目录的最后一个文件夹目录项位置
                while (FALSE != fsel_last_dir())
                {
                    //进入子目录
                    vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                    eh_dir_layer++;
                    eh_file_total = 0;
                    eh_file_no = 0;
                }
                //获取当前目录下的文件总数
                eh_file_total = fsel_total_file_cur(NULL, 0);
                if (eh_file_total != 0)
                {
                    eh_file_no = 0;
                    //取得当前目录下的文件
                    return fsel_dir_prev(strfile);
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }
            else
            {
                //当前目录下已找完文件夹，开始找文件
                eh_file_total = fsel_total_file_cur(NULL, 0);
                if (eh_file_total != 0)
                {
                    eh_file_no = 0;
                    return fsel_dir_prev(strfile);
                }
                goto ParentDiragainNext;
            }
        }
        else
        {
            //出错了
            fsel_init_sysdata();
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_get_prev(uint8 *strfile)
 * \获取上一个文件
 * \param[in]    strfile  para1 存放文件后缀名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_perv.c
 * \note
 */
/*******************************************************************************/
bool fsel_dir_get_prev(uint8 *strfile)
{

    bool ret_val;

    switch (fsel_init_val.fsel_mode)
    {
        case FSEL_MODE_LOOPONE:
        if (eh_file_total == 0)
        {
            ret_val = FALSE;
        }
        else if (eh_file_no == 0)
        {
            //取第一个文件
            ret_val = fsel_dir_get_byno(strfile, 1);
        }
        else
        {
            if (strfile != NULL)
            {
                *(uint32 *) strfile = eh_cur_ext_name;
            }
            ret_val = TRUE;
        }
        break;

        case FSEL_MODE_NORMAL:
        if ((eh_file_no == 0) && (eh_dir_layer == 0))
        {
            fsel_init_sysdata();
            while (FALSE != fsel_last_dir())
            {
                vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                eh_dir_layer++;
                eh_file_total = 0;
                eh_file_no = 0;
            }
        }
        ret_val = fsel_dir_prev_layer(strfile);
        break;

        //全部重复
        case FSEL_MODE_LOOPALL:
        if ((eh_file_no == 0) && (eh_dir_layer == 0))
        {
            goto prev_init_get;
        }

        if (FALSE != fsel_dir_prev_layer(strfile))
        {
            ret_val = TRUE;
        }
        else //已是最前的一个，切换到磁盘目录最后一个文件
        {
            prev_init_get:
            fsel_init_sysdata();
            //定位到当前目录的最后一个文件夹目录项位置
            while (FALSE != fsel_last_dir())
            {
                vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                eh_dir_layer++;
                eh_file_total = 0;
                eh_file_no = 0;
            }
            ret_val = fsel_dir_prev_layer(strfile);
        }
        break;

        case FSEL_MODE_LOOPONEDIR:
        //当前目录下的前一个文件
        if (FALSE != fsel_dir_prev(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            eh_file_no = 0;

            ret_val = fsel_dir_prev(strfile);
        }
        break;

        default:
        ret_val = FALSE;
        break;
    }

    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fsel_dir_prev_dir(uint8 *strfile)
 * \查找上一个目录
 * \param[in]    strfile  para1 存放目录名指针
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      eh_fsel_directory_perv.c
 * \note
 */
/*******************************************************************************/
bool fsel_dir_prev_dir(uint8 *strdir)
{

    //file no为该目录最后一首歌
    //前一个目录最后一个文件
    eh_file_no_all -= (eh_file_no - 1);

    //定位到当前目录第一首,切换歌曲
    eh_file_no = 1;

    if (fsel_dir_get_prev((uint8 *) &eh_cur_ext_name) == TRUE)
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

/*****************************************************************************
 * \par  Description:定位目录
 * \bool fsel_enter_dir(uint8 *name_short,uint8* name_long)
 * \param[in]  name_short param1 目录短名
 * \param[in]  name_long param2 目录长名
 * \param[out]  none
 * \return  bool TRUE 成功
 FALSE 失败
 * \ingroup      eh_fsel_directory_perv.c
 * \note
 *******************************************************************************/
bool fsel_enter_dir(uint8 *name_short, uint8 *name_long, uint8 dir_direct)
{
    bool ret_dir = TRUE;

    uint16 file_number = 0;

    //到根目录
    fsel_init_sysdata();

    //check long name dir
    //ret_dir = vfs_dir(eh_vfs_mount, DIR_HEAD, name_long, EXT_NAME_ALL_DIR);

    //check short name dir
    if (vfs_dir(eh_vfs_mount, DIR_HEAD, name_long, EXT_NAME_ALL_DIR) == FALSE)
    {
        //ret_dir = vfs_dir(eh_vfs_mount, DIR_HEAD, name_short, EXT_NAME_ALL_DIR);
        if (vfs_dir(eh_vfs_mount, DIR_HEAD, name_short, EXT_NAME_ALL_DIR) == FALSE)
        {
            ret_dir = FALSE;
        }
    }
    //enter directory
    if (ret_dir == TRUE)
    {
        //进入目录
        if (vfs_cd(eh_vfs_mount, CD_SUB, NULL) == FALSE)
        {
            ret_dir = FALSE;
        }
    }

    if (ret_dir == TRUE)
    {

        eh_file_total = fsel_total_file_cur(NULL, 0);

        //定位到第一个文件
        //定位到最后一个文件
        eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_direct, NULL, fsel_init_val.file_type_bit);
        if (eh_cur_ext_name != 0)
        {
            //eh_file_no_all = (file_number + 1);
            //last one
            if (dir_direct == DIR_TAIL)
            {
                eh_file_no = eh_file_total;
                eh_file_no_all = eh_file_total;
            }
            else //first one
            {
                eh_file_no = 1;
                eh_file_no_all = 1;
            }
            eh_file_total_all = eh_file_total;
        }
        else
        {
            ret_dir = FALSE;
        }
    }
    if (ret_dir == FALSE)
    {
        //回到根目录
        fsel_init_sysdata();
    }
    return ret_dir;
}

