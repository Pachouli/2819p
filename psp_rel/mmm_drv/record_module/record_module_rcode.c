/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：录音模块写文件线程主循环。
 * 作者：cailizhen
 ********************************************************************************/

#include "record_module.h"

static bool record_module_rewrite_wav_head(record_module_t *p_record_module)
{
    uint32 mount_id, fhandle;
    uint32 file_size;

    mount_id = (uint32) p_record_module->record_fs.file_mount_id;
    fhandle = (uint32) p_record_module->record_fs.file_handle;
    file_size = ((fhandle_t *) fhandle)->file_size;

    p_record_module->record_module_status.start_cluster = vfs_rec_file_update(mount_id, file_size, fhandle);

    return TRUE;
}

static int32 record_module_save_encode_data(record_module_t *p_record_module, uint8 *buf, uint32 len)
{
    int32 write_size;
    uint8 write_pege_step = 4; //one page : 512 ,once write size : 2K
    fs_data_t *fs_data = p_record_module->record_fs.file_mount_id;
    uint32 fhandle = (uint32)p_record_module->record_fs.file_handle;

    write_size = vfs_file_write(fs_data, buf, len, fhandle);

    if (write_size != len)
    {
        libc_printf_error("fs_err:%x!\n", fs_data->fs_status);

        //磁盘写满了
        if (fs_data->fs_status == ERR_NO_EMPTY_CLUSTER)
        {
            libc_printf_error("disk full!\n");
            p_record_module->record_module_status.err_no = MMM_MR_ERR_DISK_FULL;
            p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
        }
        else
        {
            libc_printf_error("file write err!\n");
            p_record_module->record_module_status.err_no = MMM_MR_ERR_WRITE_FILE_FAIL;
            p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
        }

        return -1;
    }

    //获取文件首簇，用于录音断电恢复
    if (((fhandle_t *)fhandle)->start_cluster != 0)
    {
        p_record_module->record_module_status.start_cluster = ((fhandle_t *)fhandle)->start_cluster;
    }

    //一个PAGE大小为512，而一次写入的大小为2048
    p_record_module->record_module_status.write_pages += write_pege_step;

    return write_size;
}

void *write_loop(void *param)
{
    record_module_t *p_record_module = (record_module_t *) &g_record_module_global;

    p_record_module->write_thread_terminate_flag = FALSE;

    libc_printf_info("write loop start\n");

    for(;;)
    {
        if (p_record_module->record_module_status.status != RECORD_MODULE_STA_START)
        {
            goto write_loop_sleep;
        }

        if (p_record_module->encode_output_buf_w.length >= WRITE_SIZE_PER_TIME)
        {
            uint8 *write_file_buf = p_record_module->encode_output_buf_w.raw_data
                    + p_record_module->encode_output_buf_w.read_ptr;
            int32 write_bytes;
            uint32 max_pages = p_record_module->record_fs.file_max_length;

            if (p_record_module->record_module_status.write_pages >= max_pages)
            {
                if (p_record_module->record_module_status.err_no != MMM_MR_ERR_FILE_SIZE_OVER_LIMIT)
                {
                    //表示丢弃数据,并且上报应用
                    p_record_module->record_module_status.err_no = MMM_MR_ERR_FILE_SIZE_OVER_LIMIT;
                    p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
                    p_record_module->record_module_status.time = 0; //防止应用层显示错误
                }
            }
            else
            {
                write_bytes = record_module_save_encode_data(p_record_module, write_file_buf, WRITE_SIZE_PER_TIME);

                if (write_bytes < 0)
                {
                    libc_printf_error("record_module write file err!\n");
                    p_record_module->record_module_status.err_no = MMM_MR_ERR_WRITE_FILE_FAIL;
                    p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
                }
                else
                {
                    buffer_rw_update(&p_record_module->encode_output_buf_w, WRITE_SIZE_PER_TIME);
                }
            }
        }

        write_loop_sleep:
        if (p_record_module->write_thread_terminate_flag == TRUE)
        {
            if (p_record_module->encode_para.encode_mode != ENCODE_BYPASS)
            {
                record_module_rewrite_wav_head(p_record_module);
            }
            break;
        }
        else
        {
            sys_os_time_dly(1); //unit 10ms，不需要写的时候必须释放
        }
    }

    libc_printf_info("stop write thread\n");

    p_record_module->write_thread_terminate_flag = FALSE;

    /*注意：libc_pthread_exit无法返回的*/
    libc_pthread_exit();

    return (void *) NULL;
}

int32 record_module_encode_deal(record_module_t *p_record_module, uint8 *encode_in_addr)
{
    int32 result = 0;

    //已经进入STOP录音进程，不再编码新数据
    if (p_record_module->stop_encode_flag == TRUE)
    {
        return 0;
    }

    if (p_record_module->encode_para.encode_mode == ENCODE_BYPASS)
    {
        //缓存编码后数据
        if (buffer_rw_write(&(p_record_module->encode_output_buf_w),
                encode_in_addr, p_record_module->encode_frame_len) == FALSE)
        {
            libc_printf_warning("write encode_output_buf_w full!\n");
        }
    }
    else
    {
        audioin_pcm_t ain;

        //单声道只需要配置pcm[0]，music_record->music_encode_info中的ain指向music_record->ain
        ain.pcm[0] = encode_in_addr;
        if (p_record_module->music_info.channels == 1)
        {
            ain.pcm[1] = ain.pcm[0];
        }
        else
        {
            ain.pcm[1] = encode_in_addr + (p_record_module->music_info.block_size * sizeof(short));
        }
        ain.samples = p_record_module->music_info.block_size;//样本对数

        p_record_module->music_encode_info.ain = &ain;
        p_record_module->music_encode_info.aout = p_record_module->music_info.data_buf_addr;

        result = audio_encoder_cmd(p_record_module->audioenc_handle, AE_CMD_FRAME_ENCODE,
                (uint32) & p_record_module->music_encode_info);
        if (result != 0)
        {
            p_record_module->record_module_status.err_no = MMM_MR_ERR_UNEXPECTED;
            p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
            libc_printf_error("encode err %d!\n", result);
        }
        p_record_module->record_module_status.time = p_record_module->music_encode_info.cur_time;

        if (p_record_module->print_bytes_used == TRUE)
        {
            libc_printf_info("encode_output_frame_len : %d\n", p_record_module->music_encode_info.bytes_used);
            p_record_module->print_bytes_used = FALSE;
        }

        //缓存编码后数据
        if (buffer_rw_write(&(p_record_module->encode_output_buf_w),
                (uint8 *) p_record_module->music_encode_info.aout, p_record_module->music_encode_info.bytes_used) == FALSE)
        {
            libc_printf_warning("write encode_output_buf_w full!\n");
        }
    }

    return result;
}
