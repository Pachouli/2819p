/********************************************************************************
 *                               US281A
 *                             Module: record
 *                 Copyright(c) 2017-2027 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_recover.c
 * History:
 *   <author>      huanganbang
 *   <time>        16:33:12
 *   <version >    v1.0
 *   <desc>        create this file
 ********************************************************************************/
#include "common_record_inner.h"

#if (SUPPORT_RECORD_FUNC == 1)

bool rec_file_recover(record_file_path_t* p_file_path, uint8 disk_type)
{
    bool ret = TRUE;

    //设置path和file信息
    if (!vfs_file_dir_offset(p_record_file_info->fs_param.vfs_handle, &p_file_path->ptr_layer, &p_file_path->ptr_file_offset, 1))
    {
        libc_printf("set rec file offset fail\n");
        return FALSE;
    }

    //恢复录音文件信息
    p_record_file_info->fs_param.fhandle = vfs_rec_file_recover(p_record_file_info->fs_param.vfs_handle, p_file_path->rec_filename, \
            p_record_file_info->file_path.start_cluster);

    if (!p_record_file_info->fs_param.fhandle)
    {
        libc_printf("recover failed\n");
        ret = FALSE;
    }

    ((fhandle_t *)(p_record_file_info->fs_param.fhandle))->start_cluster = p_record_file_info->file_path.start_cluster;
    ((fhandle_t *)(p_record_file_info->fs_param.fhandle))->file_size = p_record_file_info->file_num_info.file_bytes;

    //更新文件头
    {
        uint32 mount_id = p_record_file_info->fs_param.vfs_handle;
        uint32 fhandle = p_record_file_info->fs_param.fhandle;
        uint32 file_size = p_record_file_info->file_num_info.file_bytes;

        vfs_rec_file_update(mount_id, file_size, fhandle);
    }

    vfs_file_close(p_record_file_info->fs_param.vfs_handle, p_record_file_info->fs_param.fhandle);

    return ret;
}

bool rec_file_recover_deal(uint8 disk_type)
{
    bool ret = FALSE;
    record_file_path_t* p_file_path;

    if (p_record_file_info == NULL)
    {
        p_record_file_info = sys_malloc(sizeof(record_file_info_t));
        libc_memset(p_record_file_info, 0x0, sizeof(record_file_info_t));
    }

    libc_printf_info("try record file recover : %d\n", disk_type);

    p_record_file_info->disk_type = disk_type;
    p_file_path = &(p_record_file_info->file_path);

    if (rec_file_recover_load_fs_vram(p_file_path, disk_type) == FALSE)
    {
        rec_file_recover_save_fs_vram(p_file_path, disk_type);
        libc_printf("read p_file_path from vram fail\n");
        goto recover_exit;
    }

    if (rec_file_recover_load_file_vram(&(p_record_file_info->file_num_info), disk_type) == FALSE)
    {
        rec_file_recover_save_file_vram(&(p_record_file_info->file_num_info), disk_type);
        libc_printf("read file info from vram fail\n");
        goto recover_exit;
    }

    if ((p_file_path->start_cluster == 0xffffffff) || (p_file_path->start_cluster < 2))
    {
        libc_printf("invalid start_cluster\n");
        goto recover_exit;
    }

    if (_record_disk_exist(disk_type) == REC_FS_ERR_NONE)
    {
        if (_record_disk_init(disk_type) == REC_FS_ERR_NONE)
        {
            ret = rec_file_recover(p_file_path, disk_type);

            if (ret == TRUE)
            {
                libc_printf("record file recover success! %d\n", disk_type);
            }
            else
            {
                libc_printf("record file recover faild! %d\n", disk_type);
            }
        }
        else
        {
            libc_printf_error("disk init fail\n");
        }

        _record_disk_exit();
    }
    else
    {
        libc_printf("disk not exist\n");
        goto recover_exit;
    }

    p_file_path->start_cluster = 0xffffffff;
    rec_file_recover_save_fs_vram(p_file_path, disk_type);

    p_record_file_info->file_num_info.file_bytes = 0;
    rec_file_recover_save_file_vram(&(p_record_file_info->file_num_info), disk_type);

    recover_exit:

    sys_free(p_record_file_info);
    p_record_file_info = NULL;

    return ret;
}

#endif
