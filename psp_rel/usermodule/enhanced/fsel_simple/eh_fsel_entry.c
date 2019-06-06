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

#include "dir_list.h"

#define MAX_SEL_MODE   0x01

//函数数组
static const fsel_get_next_p fsel_get_next_array[MAX_SEL_MODE] =
    {
        fsel_dir_get_next, //目录选择下一个文件
    };

static const fsel_get_prev_p fsel_get_prev_array[MAX_SEL_MODE] =
    {
        fsel_dir_get_prev, //目录选择上一个文件
    };

static const fsel_get_byno_p fsel_get_byno_array[MAX_SEL_MODE] =
    {
        fsel_dir_get_byno_all, //目录根据序号选择文件
    };

static const fsel_set_location_p fsel_set_location_array[MAX_SEL_MODE] =
    {
        (void *) fsel_dir_set_location, //目录设置路径信息
    };

static const fsel_get_location_p fsel_get_location_array[MAX_SEL_MODE] =
    {
        (void *) fsel_dir_get_location, //目录获取路径信息
    };

static const fsel_get_nextdir_p fsel_get_nextdir_array[MAX_SEL_MODE] =
    {
        fsel_dir_next_dir, //目录切换下一个目录
    };
static const fsel_get_prevdir_p fsel_get_prevdir_array[MAX_SEL_MODE] =
    {
        fsel_dir_prev_dir, //目录切换上一个目录
    };

static const fsel_get_total_p fsel_get_total_array[MAX_SEL_MODE] =
    {
        fsel_total_file_all, //目录获取文件总数
    };

/******************************************************************************/
/*
 * \par  Description:获取下一个文件
 *
 * \param[in]    none

 * \param[out]      strfile 存放文件扩展名

 * \return        true or false

 * \note
 *******************************************************************************/
bool fsel_get_nextfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (eh_vfs_mount == 0)
    {
        return 0;
    }
    return fsel_get_next_array[eh_fsel_type](strfile);
}

/******************************************************************************/
/*
 * \par  Description:获取上一个文件
 *
 * \param[in]    none

 * \param[out]   strfile 存放文件扩展名

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_prevfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (eh_vfs_mount == 0)
    {
        return 0;
    }
    return fsel_get_prev_array[eh_fsel_type](strfile);
}

/******************************************************************************/
/*
 * \par  Description:按指定序号获取文件
 *
 * \param[in]    num 文件排列序号(1~...)

 * \param[out]      strfile 存放文件扩展名

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_byno(char *strfile, uint16 num)
{
    fsel_error = FSEL_NO_ERR;

    if (num == 0)
    {
        return FALSE;
    }
    return fsel_get_byno_array[eh_fsel_type](strfile, num);
}

/******************************************************************************/
/*
 * \par  Description:当前目录或列表的文件总个数
 *
 * \param[in]    none

 * \param[out]      none

 * \return       文件总个数

 * \note
 *******************************************************************************/
uint16 fsel_get_total(void)
{
    fsel_error = FSEL_NO_ERR;

    if (eh_file_total_all != 0)
    {
        return eh_file_total_all;
    }
    else
    {
        return fsel_get_total_array[eh_fsel_type]();
    }
}

/******************************************************************************/
/*
 * \par  Description:当前磁盘文件总数
 *
 * \param[in]    none

 * \param[out]      none

 * \return       文件总个数

 * \note
 *******************************************************************************/
uint16 fsel_get_disk_total(void)
{
    return fsel_get_total_array[eh_fsel_type]();
}


/******************************************************************************/
/*
 * \par  Description:获取当前选择文件的序号
 *
 * \param[in]    none

 * \param[out]      none

 * \return        序号(1~...)

 * \note
 *******************************************************************************/
uint16 fsel_get_fileno(void)
{
    //fsel_error = FSEL_NO_ERR;
    //return eh_file_no;
    uint16 file_no = eh_file_no_all;

    return file_no;
}

/******************************************************************************/
/*
 * \par  Description:重新设计播放模式
 *
 * \param[in]    mode 播放模式

 * \param[out]      none

 * \return        ture or false

 * \note
 *******************************************************************************/
bool fsel_set_mode(uint8 mode)
{
    fsel_error = FSEL_NO_ERR;

#ifdef SUPPORT_SHUFFLE_MODE
    //check shuffle
    if (((uint8) mode & 0xf0) > 0)
    {
        if (eh_shuffle_sw == FALSE)
        {
            //之前不是shuffle,要初始化
            eh_shuffle_sw = TRUE;
            //if(eh_file_total_all == 0)
            //{
            eh_file_total_all = fsel_get_total();
            //}
            fsel_init_shuffle (eh_file_total_all);
            filtrate_shuffle_no = eh_file_no_all;
        }
    }
    else
    {
        eh_shuffle_sw = FALSE;
    }
#else
    eh_shuffle_sw = FALSE;
#endif

    fsel_init_val.fsel_mode = (uint8)(mode & 0x0f);

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:设置文件location
 *
 * \param[in]    location
 type

 * \param[out]

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_set_location(void *location)
{
    fsel_error = FSEL_NO_ERR;

    if (location == NULL)
    {
        return FALSE;
    }

    return fsel_set_location_array[eh_fsel_type](location);
}

/******************************************************************************/
/*
 * \par  Description:获取当前选择文件的location
 *
 * \param[in]    type

 * \param[out]      location

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_location(void *location)
{
    fsel_error = FSEL_NO_ERR;

    if ((eh_file_no == 0) || (location == NULL))
    {
        return FALSE;
    }

    return fsel_get_location_array[eh_fsel_type](location);
}

/******************************************************************************/
/*
 * \par  Description:获取下一个目录
 *
 * \param[in]        none

 * \param[out]      strfile-返回目录标识

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_nextdir(uint8 *strdir)
{
    fsel_error = FSEL_NO_ERR;

    return fsel_get_nextdir_array[eh_fsel_type](strdir);
}

/******************************************************************************/
/*
 * \par  Description:获取上一个目录
 *
 * \param[in]    none

 * \param[out]          strfile-返回目录标识

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_prevdir(uint8 *strdir)
{
    fsel_error = FSEL_NO_ERR;

    return fsel_get_prevdir_array[eh_fsel_type](strdir);
}
