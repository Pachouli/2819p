/********************************************************************************
 *                               US281A
 *                             Module: record
 *                 Copyright(c) 2017-2027 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_info_save.c
 * History:
 *   <author>      huanganbang
 *   <time>        17:27:12
 *   <version >    v1.0
 *   <desc>        create this file
 ********************************************************************************/
#include "common_record_inner.h"

bool rec_file_recover_load_fs_vram(record_file_path_t* p_file_path, uint8 disk_type)
{
    int32 ret = 0;
    if (disk_type == DISK_U)
    {
        ret = sys_vm_read(p_file_path, VM_AP_RECORD_UHOST_FS, sizeof(record_file_path_t));
    }
    else
    {
        ret = sys_vm_read(p_file_path, VM_AP_RECORD_CARD_FS, sizeof(record_file_path_t));
    }
    if (ret < 0)
    {
        p_file_path->magic = 0xffff;
        return FALSE;
    }

    if (disk_type == DISK_U)
    {
        if (p_file_path->magic != VRAM_MAGIC(VM_AP_RECORD_UHOST_FS))
        {
            return FALSE;
        }
    }
    else
    {
        if (p_file_path->magic != VRAM_MAGIC(VM_AP_RECORD_CARD_FS))
        {
            return FALSE;
        }
    }

    return TRUE;
}

bool rec_file_recover_save_fs_vram(record_file_path_t* p_file_path, uint8 disk_type)
{
    int32 ret = 0;
    bool try_after_merge = TRUE;

    save_fs_vram_retry:

    if (disk_type == DISK_U)
    {
        p_file_path->magic = VRAM_MAGIC(VM_AP_RECORD_UHOST_FS);
        ret = sys_vm_write(p_file_path, VM_AP_RECORD_UHOST_FS, sizeof(record_file_path_t));
    }
    else
    {
        p_file_path->magic = VRAM_MAGIC(VM_AP_RECORD_CARD_FS);
        ret = sys_vm_write(p_file_path, VM_AP_RECORD_CARD_FS, sizeof(record_file_path_t));
    }

    if(ret < 0)
    {
        if (try_after_merge == TRUE)
        {
            try_after_merge = FALSE;
            com_vram_merge();
            libc_printf("record vram merge!\n");
            goto save_fs_vram_retry;
        }
        return FALSE;
    }
    return TRUE;
}

bool rec_file_recover_load_file_vram(record_rec_num_t * ap_param, uint8 disk_type)
{
    int32 ret = 0;

    if (disk_type == DISK_U)
    {
        ret = sys_vm_read(ap_param, VM_AP_RECORD_UHOST_FILE, sizeof(record_rec_num_t));
    }
    else
    {
        ret = sys_vm_read(ap_param, VM_AP_RECORD_CARD_FILE, sizeof(record_rec_num_t));
    }
    if (ret < 0)
    {
        return FALSE;
    }

    return TRUE;
}

bool rec_file_recover_save_file_vram(record_rec_num_t * ap_param, uint8 disk_type)
{
    int32 ret = 0;
    bool try_after_merge = TRUE;

    save_file_vram_retry:

    if (disk_type == DISK_U)
    {
        ret = sys_vm_write(ap_param, VM_AP_RECORD_UHOST_FILE, sizeof(record_rec_num_t));
    }
    else
    {
        ret = sys_vm_write(ap_param, VM_AP_RECORD_CARD_FILE, sizeof(record_rec_num_t));
    }

    if(ret < 0)
    {
        if (try_after_merge == TRUE)
        {
            try_after_merge = FALSE;
            com_vram_merge();
            libc_printf("record vram merge!\n");
            goto save_file_vram_retry;
        }

        return FALSE;
    }
    return TRUE;
}

