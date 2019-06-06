/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-27 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

/******************************************************************************/
/*
 * \par  Description: 获取出错信息
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
fsel_error_e fsel_get_error(void)
{
    //return fsel_error;
    return vfs_get_err_info(eh_vfs_mount);
}

/******************************************************************************/
/*
 * \par  Description:设置文件选择类型
 *
 * \param[in]    type 文件选择模式(只有0和1)

 * \param[out]      none

 * \return        ture or false

 * \note
 *******************************************************************************/
bool fsel_set_seltype(fsel_type_e type, uint16 cur_num)
{
    //file_location_t *loca_ptr = (file_location_t *) location;

    //uint16 cur_no;

    fsel_error = FSEL_NO_ERR;

    //如果输入参数出错,则默认进入普通目录模式
    if (type > FSEL_TYPE_SDFILE)
    {
        type = FSEL_TYPE_COMMONDIR;
    }
    if (type != eh_fsel_type)
    {
        //更改类型
        eh_fsel_type = type;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:enhanced change 显示文件序号
 * \void fsel_set_fileno(uint16 set_num)
 * \param[in]    set_num 目标序号
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void fsel_set_fileno(uint16 set_num)
{
    eh_file_no_all = set_num;
}

