/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_file.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *              leiming    2011-10-27          v1.0                 create this file
 ********************************************************************************/

#include "common_record_inner.h"

#if (SUPPORT_RECORD_FUNC == 1)

record_file_info_t *p_record_file_info;

uint8 com_record_disk_init_inner(uint8 diskno)
{
    record_fs_err_type_e ret_err_type;

    if (diskno == DISK_DEFAULT)
    {
        //优先U盘录音
        if (sys_uhost_detect()== TRUE)
        {
            diskno = DISK_U;
        }
        else if (sys_card_detect() == TRUE)
        {
            diskno = DISK_H;
        }
        else
        {
            return REC_FS_ERR_DISK_NOT_EXIST;
        }
    }

    //创建文件之前，检查是否需要恢复
    rec_file_recover_deal(diskno);

    if (p_record_file_info == NULL)
    {
        p_record_file_info = sys_malloc(sizeof(record_file_info_t));
        libc_memset(p_record_file_info, 0x0, sizeof(record_file_info_t));

        if (rec_file_recover_load_fs_vram(&(p_record_file_info->file_path), diskno) == FALSE)
        {
            rec_file_recover_save_fs_vram(&(p_record_file_info->file_path), diskno);
        }
        if (rec_file_recover_load_file_vram(&(p_record_file_info->file_num_info), diskno) == FALSE)
        {
            rec_file_recover_save_file_vram(&(p_record_file_info->file_num_info), diskno);
        }
    }

    p_record_file_info->disk_type = diskno;

    ret_err_type = _record_file_sys_init(diskno);
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        goto open_err_ret;
    }

    return REC_FS_ERR_NONE;

    open_err_ret:
    sys_free(p_record_file_info);
    p_record_file_info = NULL;
    return ret_err_type;
}

uint8 com_record_create_file_inner(uint8 mode, record_param_t* p_param)
{
    record_fs_err_type_e ret_err_type;

    /*创建录音文件*/
    ret_err_type = _record_file_first_create();
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        goto open_err_ret;
    }

    //更新输出参数，返回文件句柄等信息给引擎
    p_param->file_handle = p_record_file_info->fs_param.fhandle;
    p_param->file_sys_id = p_record_file_info->fs_param.vfs_handle;
    p_param->fs_free_space = p_record_file_info->fs_param.free_space;
    p_param->name = p_record_file_info->fs_param.rec_filename;

    p_record_file_info->last_record_sta = p_record_file_info->record_sta = RECORD_STA_STOP;

    return REC_FS_ERR_NONE;

    open_err_ret:

    return ret_err_type;
}


uint8 com_record_close_file_inner(uint8 mode)
{
    record_fs_err_type_e ret_err_type = REC_FS_ERR_NONE;
    uint32 file_sys_id;
    fhandle_t *file_handle;

    if (p_record_file_info == NULL)
    {
        return REC_FS_ERR_OTHER;
    }

    file_sys_id = p_record_file_info->fs_param.vfs_handle;
    if (file_sys_id == 0)
    {
        return REC_FS_ERR_OTHER;
    }
    file_handle = (fhandle_t *) p_record_file_info->fs_param.fhandle;
    if (file_handle == NULL || file_handle->mode == FHANDLE_FREE)
    {
        return REC_FS_ERR_OTHER;
    }
    libc_printf("file close...\n");

    if (vfs_file_close(file_sys_id,(uint32)file_handle) == FALSE)
    {
        ret_err_type = REC_FS_ERR_OTHER;
    }

    p_record_file_info->file_path.start_cluster = 0xffffffff;
    rec_file_recover_save_fs_vram(&(p_record_file_info->file_path), p_record_file_info->disk_type);

    p_record_file_info->file_num_info.file_bytes = 0;
    rec_file_recover_save_file_vram(&(p_record_file_info->file_num_info), p_record_file_info->disk_type);

    return ret_err_type;
}


uint8 com_record_disk_exit_inner(void)
{
    record_fs_err_type_e ret_err_type = REC_FS_ERR_NONE;

    com_record_update_record_status_inner(RECORD_STA_STOP);

    if (p_record_file_info != NULL)
    {
        rec_file_recover_save_fs_vram(&(p_record_file_info->file_path), p_record_file_info->disk_type);

        _record_disk_exit();

        sys_free(p_record_file_info);
        p_record_file_info = NULL;
    }

    return ret_err_type;
}

bool com_record_recover_record_file_inner(void)
{
    bool ret = FALSE;

    libc_printf("uhost ");
    ret = rec_file_recover_deal(DISK_U);

    libc_printf("card ");
    ret = rec_file_recover_deal(DISK_H);

    return ret;
}

void com_record_update_record_file_inner(bool force_update, record_file_update_t *p_file_update)
{
    bool need_update_vram = FALSE;

    if (p_record_file_info == NULL)
    {
        return;
    }

    if (p_file_update->start_cluster != 0xffffffff && (p_file_update->start_cluster>2))
    {
        if (force_update == TRUE)
        {
            need_update_vram = TRUE;
        }

        if ((p_file_update->record_time/1000) < p_record_file_info->record_time_last)
        {
            p_record_file_info->record_time_last = p_file_update->record_time/1000;
        }

        if ((p_file_update->record_time/1000 - p_record_file_info->record_time_last) >= 30)
        {
            need_update_vram = TRUE;
        }
    }

    if (need_update_vram == TRUE)
    {
        p_record_file_info->file_path.start_cluster = p_file_update->start_cluster;
        rec_file_recover_save_fs_vram(&(p_record_file_info->file_path), p_record_file_info->disk_type);

        p_record_file_info->file_num_info.file_bytes = p_file_update->write_pages * 512;
        rec_file_recover_save_file_vram(&p_record_file_info->file_num_info, p_record_file_info->disk_type);
        libc_printf("update record file\n");

        p_record_file_info->record_time_last = p_file_update->record_time/1000;
    }
}

//0表示没有录音文件被创建，DISK_H表示卡创建了录音文件，DISK_U表示U盘创建了录音文件
uint8 com_record_get_disk_type_inner(void)
{
    if (p_record_file_info == NULL)
    {
        return 0;
    }

    return p_record_file_info->disk_type;
}

//刷新录音UI
void com_record_update_record_status_inner(record_status_e record_sta)
{
    if (p_record_file_info == NULL)
    {
        return;
    }

    p_record_file_info->record_sta = record_sta;

    if ((p_record_file_info->last_record_sta != RECORD_STA_RECORDING) && (p_record_file_info->record_sta == RECORD_STA_RECORDING))
    {
        if (p_record_file_info->disk_type == DISK_H)
        {
            com_led_twinkle_icon(LCD_SD, TRUE, 1000);
        }
        else
        {
            com_led_twinkle_icon(LCD_USB, TRUE, 1000);
        }
    }
    else if ((p_record_file_info->last_record_sta != RECORD_STA_PAUSE) && (p_record_file_info->record_sta == RECORD_STA_PAUSE))
    {
        if (p_record_file_info->disk_type == DISK_H)
        {
            com_led_display_icon(LCD_SD, TRUE);
        }
        else
        {
            com_led_display_icon(LCD_USB, TRUE);
        }
    }
    else if (((p_record_file_info->last_record_sta == RECORD_STA_RECORDING) || (p_record_file_info->last_record_sta == RECORD_STA_PAUSE))
        && ((p_record_file_info->record_sta != RECORD_STA_RECORDING) && (p_record_file_info->record_sta != RECORD_STA_PAUSE)))
    {
        com_led_display_icon(LCD_SD, FALSE);
        com_led_display_icon(LCD_USB, FALSE);
    }

    p_record_file_info->last_record_sta = p_record_file_info->record_sta;
}

//发送同步消息给引擎开关录音
void com_record_switch_record(void)
{
    msg_apps_t msg;

    if ((p_record_file_info == NULL) || (p_record_file_info->record_sta == RECORD_STA_STOP))
    {
        //启动录音
        msg.type = MSG_REC_START_SYNC;
        send_sync_msg(APP_ID_THEENGINE, &msg, NULL, 0);
    }
    else if ((p_record_file_info != NULL) && (p_record_file_info->record_sta == RECORD_STA_RECORDING))
    {
        //停止录音并保存文件
        msg.type = MSG_REC_STOP_SYNC;
        send_sync_msg(APP_ID_THEENGINE, &msg, NULL, 0);
    }
}

#endif
