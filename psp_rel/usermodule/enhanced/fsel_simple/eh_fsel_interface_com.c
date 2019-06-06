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

static const uint8 card_drv_name[] =
{ "card.drv" };
static const uint8 uhost_drv_name[] =
{ "uhost.drv" };

/******************************************************************************/
/*
 * \par  Description:初始化公共参数
 *
 * \param[in]    none

 * \param[out]      none

 * \return        none

 * \note
 *******************************************************************************/
void fsel_init_sysdata(void)
{
    //当前目录的层次，0表示根目录，1表示根目录的子目录，以此类推
    eh_dir_layer = 0;
    //根目录的子目录指针，1,2，。。。
    eh_root_dir = 0;
    //当前文件在目录或列表中的序号
    eh_file_no = 0;
    //当前目录或列表的总文件个数
    eh_file_total = 0;

    //当前磁盘文件序号
    eh_file_no_all = 0;

    eh_dir_no = 1;

    //默认进入根目录
    vfs_cd(eh_vfs_mount, CD_ROOT, 0);
}

/******************************************************************************/
/*
 * \par  Description:enhanced 模块的初始化
 *
 * \param[in]    init_param 初始化参数结构体
 * \param[in]
 * \param[in]    drv_mode  存储介质驱动模式

 * \param[out]      vfs标识号

 * \return        vfs标识号(0无效)

 * \note
 *******************************************************************************/
uint32 fsel_init(fsel_param_t *init_param, uint8 drv_mode)
{
    //介质驱动名称
    uint8 *drv_name;
    //驱动类型
    uint8 drv_type;
    uint8 tmp_fsel_type = 0;
    //出错信息
    fsel_error = FSEL_NO_ERR;
    //参数为空则直接返回
    if (init_param == NULL)
    {
        return 0;
    }

    eh_scan_mode = init_param->mode;

    tmp_fsel_type = (init_param->fsel_type & 0x7f);
    //保存初始化参数,文件选择器退出时使用
    libc_memcpy(&fsel_init_val, init_param, sizeof(fsel_param_t));

    //循环模式
    fsel_init_val.fsel_mode &= 0x0f;

#ifdef SUPPORT_SHUFFLE_MODE
    //shuffle是否打开
    if ((init_param->fsel_mode & 0xf0) != 0)
    {
        eh_shuffle_sw = TRUE;
    }
    else
    {
        eh_shuffle_sw = FALSE;
    }
#else
    eh_shuffle_sw = FALSE;
#endif

    if (init_param->disk == DISK_H)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) &card_drv_name;
    }
    else if (init_param->disk == DISK_U)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) &uhost_drv_name;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return 0;
    }

    //安装存储驱动
    if (sys_drv_install(drv_type, drv_mode, drv_name) != 0)
    {
        fsel_error = FSEL_ERR_STG;
        return 0;
    }

    //挂载FS
    eh_vfs_mount = sys_mount_fs(drv_type, fsel_init_val.disk, 0);

    if (eh_vfs_mount == 0)
    {
        sys_drv_uninstall(drv_type);
        fsel_error = FSEL_ERR_FS;
        return 0;
    }

    /*if (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL) == 0)
     {
     eh_vfs_mount = -1;
     }
     else*/
    //{
    //初始化参数
    fsel_init_sysdata();
    //}

    //文件选择模式为目录模式
    eh_fsel_type = 0;

    eh_fsel_filter_record_dir_flag = FALSE;
#ifdef FILTER_RECORD_DIR
    if (eh_fsel_type == FSEL_TYPE_COMMONDIR)
    {
        if (init_param->mode < 2)
        {
            eh_fsel_filter_record_dir_flag = TRUE;
        }
    }
#endif

    return eh_vfs_mount;
}

/******************************************************************************/
/*
 * \par  Description: 退出enhanced
 *
 * \param[in]    none

 * \param[out]      none

 * \return        true or false

 * \note
 *******************************************************************************/
bool fsel_exit(void)
{
    int32 ret_val;
    //驱动类型
    uint8 drv_type;

    fsel_error = FSEL_NO_ERR;

    //文件系统挂载成功
    if (eh_vfs_mount != 0)
    {
        ret_val = sys_unmount_fs(eh_vfs_mount);
        eh_vfs_mount = 0;
    }
    else
    {
        return TRUE;
    }

    //文件系统卸载成功
    if (ret_val < 0)
    {
        fsel_error = FSEL_ERR_FS;
        return FALSE;
    }

    if ((fsel_init_val.disk == DISK_H) || (fsel_init_val.disk == DISK_U))
    {
        drv_type = DRV_GROUP_STORAGE;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return -1;
    }
    //卸载介质驱动
    ret_val = sys_drv_uninstall(drv_type);

    if (ret_val < 0)
    {
        fsel_error = FSEL_ERR_STG;
        return FALSE;
    }

    return TRUE;
}

