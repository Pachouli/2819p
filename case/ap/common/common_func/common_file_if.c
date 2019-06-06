/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：磁盘文件简单读写接口，支持修改磁盘卷标。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func_inner.h"
#include "card.h"

/*! 前台加载文件系统ID */
static uint8 g_disk_type;
static uint32 g_fs_mount_id;

static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";
static uint32 _fs_init(uint8 disk_type);
static bool _fs_deinit(uint8 disk_type, uint32 file_sys_id);

//file_name 为 NULL 表示要进行磁盘操作，返回文件系统mount句柄；否则返回文件句柄
//file_name 支持Unicode和ANSI编码
uint32 com_file_open_inner(uint8 disk_type, uint8 *file_name, uint8 open_mode)
{
    uint32 ret_val = 0;

    g_disk_type = disk_type;
    g_fs_mount_id = _fs_init(disk_type);
    if (g_fs_mount_id == 0)
    {
        return 0;
    }

    if (file_name == NULL)
    {
        return g_fs_mount_id;
    }

    if (vfs_cd(g_fs_mount_id, CD_ROOT, 0) == FALSE)
    {
        goto fs_error;
    }

    ret_val = vfs_file_open(g_fs_mount_id, file_name, open_mode);
    if (ret_val == 0)
    {
        goto fs_error;
    }

    return ret_val;

fs_error:
    libc_printf("com_file_open_inner error: %d\n", vfs_get_err_info(g_fs_mount_id));
    _fs_deinit(disk_type, g_fs_mount_id);
    return 0;
}

//fhandle 为 0，表示磁盘操作，只需要卸载文件系统
bool com_file_close_inner(uint32 fhandle)
{
    if (fhandle != 0)
    {
        if (vfs_file_close(g_fs_mount_id, fhandle) == FALSE)
        {
            return FALSE;
        }
    }

    return _fs_deinit(g_disk_type, g_fs_mount_id);
}

uint32 com_file_read_inner(uint8 *read_buf, uint32 byte_count, uint32 fhandle)
{
    return vfs_file_read(g_fs_mount_id, read_buf, byte_count, fhandle);
}

bool com_file_write_inner(uint8 *write_buf, uint32 byte_count, uint32 fhandle)
{
    return vfs_file_write(g_fs_mount_id, write_buf, byte_count, fhandle);
}

bool com_file_seek_inner(int32 file_offset, uint8 seek_type, uint32 fhandle)
{
    return vfs_file_seek(g_fs_mount_id, file_offset, seek_type, fhandle);
}

uint32 com_file_tell_inner(uint32 fhandle)
{
    uint32 file_offset;

    if (vfs_file_tell(g_fs_mount_id, &file_offset, fhandle) == FALSE)
    {
        return -1;
    }

    return file_offset;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  装载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static uint32 _fs_init(uint8 disk_type)
{
    uint8 drv_type;
    uint8 *drv_name;
    uint32 file_sys_id;

    if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) card_drv_name;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        return 0;
    }

    //装载物理介质
    if (sys_drv_install(drv_type, SLOW_DRIVER, drv_name) != 0)
    {
        return 0;
    }

    //装载文件系统
    file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

    if (file_sys_id == 0)
    {
        //卸载驱动程序
        sys_drv_uninstall(drv_type);
        return 0;
    }

    return file_sys_id;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  卸载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static bool _fs_deinit(uint8 disk_type, uint32 file_sys_id)
{
    int32 ret_val;
    uint8 drv_type;

    if (file_sys_id != 0)
    {
        ret_val = sys_unmount_fs(file_sys_id);
        file_sys_id = 0;
    }
    else
    {
        return TRUE;
    }

    if (0 > ret_val)
    {
        return FALSE;
    }

    if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STORAGE;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STORAGE;
    }
    else
    {
        return FALSE;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (0 > ret_val)
    {
        return FALSE;
    }

    return TRUE;
}

