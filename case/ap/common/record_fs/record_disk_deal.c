/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_file.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2013-03-05          v1.0                 create this file
 ********************************************************************************/
#include "common_record_inner.h"
#include "case_type.h"

#if (SUPPORT_RECORD_FUNC == 1)

const uint32 rec_bitmap[] =
{ RECORD_WAV_BITMAP, RECORD_MP3_BITMAP };

static const uint32 rec_bit_rate[4] =
{ 2, 4, 8, 16};

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_file_init(uint8 diskno)
 * \装载磁盘驱动与文件系统
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       TRUE/FALSE
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
uint8 _record_file_sys_init(uint8 diskno)
{
    record_fs_err_type_e ret_err_type;
    ret_err_type = _record_disk_exist(diskno);

    if(ret_err_type != REC_FS_ERR_NONE)
    {
        return ret_err_type;
    }

    ret_err_type = _record_disk_init(diskno);

    if(ret_err_type != REC_FS_ERR_NONE)
    {
        return ret_err_type;
    }
    ret_err_type = _record_disk_get_disk_capacity();

    if(ret_err_type != REC_FS_ERR_NONE)
    {
        return ret_err_type;
    }

    return REC_FS_ERR_NONE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool record_disk_exit(void)
 * \初始化磁盘
 * \param[in]    void
 * \param[out]   none
 * \return       bool
 * \retval       false:no exist; true:exits
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
bool _record_disk_init(uint8 disk_no)
{
    fsel_param_t init_param;//文件选择器初始化参数

    /*装载disk和fs驱动*/
    init_param.disk = disk_no;
    init_param.fsel_type = FSEL_TYPE_COMMONDIR;
    init_param.file_type_bit = ALLFILE_BITMAP; //显示所有文件和目录
    init_param.fsel_mode = FSEL_MODE_NORMAL;

    //文件选择器初始化
    p_record_file_info->fs_param.vfs_handle = fsel_init(&init_param, 0);
    if (p_record_file_info->fs_param.vfs_handle == 0)
    {
        libc_printf("fsel init fail \r\nvfs_mount->%x\n",p_record_file_info->fs_param.vfs_handle);  //for debug
        fsel_exit();
        return REC_FS_ERR_SCENCE_DISK_ERR;
    }

    return  REC_FS_ERR_NONE;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool record_disk_exit(void)
 * \检查磁盘是否存在
 * \param[in]    void
 * \param[out]   none
 * \return       bool
 * \retval       false:no exist; true:exits
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
bool _record_disk_exist(uint8 disk_no)
{
    if (disk_no ==  DISK_H)
    {
        if (sys_card_detect() == FALSE)
        {
            libc_printf("->card no exits\n");
            return REC_FS_ERR_DISK_NOT_EXIST;
        }
    }

    if (disk_no ==  DISK_U)
    {
        if (sys_uhost_detect()== FALSE)
        {
            libc_printf("->uhost no exits\n");
            return REC_FS_ERR_DISK_NOT_EXIST;
        }
    }
    return REC_FS_ERR_NONE;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool record_disk_exit(void)
 * \卸载文件系统
 * \param[in]    void
 * \param[out]   none
 * \return       bool
 * \retval       false:space low; true:space ok
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
bool _record_disk_exit(void)
{
    if ((p_record_file_info == NULL)
        || (p_record_file_info->fs_param.vfs_handle == 0))
    {
        return FALSE;
    }
    fsel_exit();
    p_record_file_info->fs_param.vfs_handle = 0;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_parse_freetime(void)
 * \通过磁盘容量和比特率，计算磁盘剩余录音时间
 * \param[in]    void
 * \param[out]   none
 * \return       bool
 * \retval       false:space low; true:space ok
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
bool _record_disk_get_disk_capacity(void)
{
    //get total space
    if (!vfs_get_space( p_record_file_info->fs_param.vfs_handle, (void*) &p_record_file_info->fs_param.free_space, GET_SPARE_SPACE))
    {
        libc_printf("disk get space faild\n");
        fsel_exit();
        return REC_FS_ERR_SCENCE_CANNOT_GET_DISK_SPACE;
    }

    //根据规格书 剩余空间必须大于等于10MB，才能继续录音。
    if (p_record_file_info->fs_param.free_space < REC_SPACE_BASE)
    {
        p_record_file_info->fs_param.free_space = 0;
        libc_printf("disk full\n");
        fsel_exit();
        return REC_FS_ERR_SCENCE_DISK_FULL;
    }
    //每次录音的最大size必须保留100kb余量
    p_record_file_info->fs_param.free_space -= REC_SPACE_LOW;

    return REC_FS_ERR_NONE;
}

#endif

