/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：录音模块开始录音接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "record_module.h"

#define DRV_NAME_SIZE 12

static const char encdrv_name[DRV_NAME_SIZE] = "enc_wav.al";

int32 creat_write_file_thread(record_module_t *p_record_module)
{
    pthread_param_t player_thread_para;

    player_thread_para.start_rtn = write_loop;
    player_thread_para.arg = (void *) 0;
    player_thread_para.ptos = (void *) p_record_module->buffer_set.ptos;
    player_thread_para.stk_size = (void *) p_record_module->buffer_set.stk_size;

    /* 创建中间件线程并设置属性 */
    if (libc_pthread_create(&player_thread_para, AP_BACK_HIGH_REC_FS_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        return -1;
    }

    return 0;
}

static void debug_printf_record_setting(record_module_t *p_record_module)
{
    libc_printf("# encode_mode : %d\n", p_record_module->encode_para.encode_mode);
    libc_printf("# sample_rate : %d\n", p_record_module->encode_para.sample_rate_hz);
    libc_printf("# channel     : %d\n", p_record_module->encode_para.channel);
    libc_printf("# bps         : %d\n", p_record_module->encode_para.bps);
    libc_printf("# block_size  : %d\n", p_record_module->music_info.block_size);
    libc_printf("# frame_len   : %d\n", p_record_module->encode_frame_len);
}

/* 启动录音编码 */
int32 start_record_module(record_module_t *p_record_module)
{
    int32 ret = 0;
    fhandle_t *fhandle = (fhandle_t *) (p_record_module->record_fs.file_handle);

    if (p_record_module->encode_para.encode_mode == ENCODE_BYPASS)
    {
        p_record_module->encode_frame_len = BYPASS_FRAME_SIZE;
    }
    else
    {
        //加载encode 编码库
        ret = (int32) sys_drv_install(REC_WAV_GROUP, 0, (void *) encdrv_name);
        if (ret == -1)
        {
            p_record_module->record_module_status.err_no = MMM_MR_ERR_UNEXPECTED;
            p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
            goto start_record_encode_fail_ret_0;
        }

        p_record_module->music_info.sub_format     = p_record_module->encode_para.encode_mode;
        p_record_module->music_info.sample_rate_hz = p_record_module->encode_para.sample_rate_hz;
        p_record_module->music_info.channels       = p_record_module->encode_para.channel;
        p_record_module->music_info.bps            = p_record_module->encode_para.bps;
        p_record_module->music_info.data_buf_addr  = p_record_module->buffer_set.output_buffer;
        p_record_module->music_info.data_buf_len   = ENCODE_OUT_BUFFER_SIZE;
        p_record_module->music_info.frame_size     = ENCODE_OUT_FRAME_SIZE*p_record_module->encode_para.channel;

        //打开编码器
        ret = audio_encoder_cmd(&(p_record_module->audioenc_handle),AE_CMD_OPEN ,(uint32) &p_record_module->music_info);
        if ((ret == -1) || (p_record_module->audioenc_handle == NULL))
        {
            p_record_module->record_module_status.err_no = MMM_MR_ERR_UNEXPECTED;
            p_record_module->record_module_status.status = RECORD_MODULE_STA_ERROR;
            libc_printf_error("audioenc open fail!!\n");
            goto start_record_encode_fail_ret_1;
        }
        p_record_module->encode_frame_len = p_record_module->music_info.channels * p_record_module->music_info.block_size
                * sizeof(short);

        debug_printf_record_setting(p_record_module);   //for debuf
    }

    //创建写文件线程
    if (creat_write_file_thread(p_record_module) == -1)
    {
        ret = -1;
        goto start_record_encode_fail_ret_2;
    }

    //初始化Encode output Buffer管理器
    p_record_module->encode_output_buf_w.read_ptr = 0;
    p_record_module->encode_output_buf_w.write_ptr = 0;
    p_record_module->encode_output_buf_w.length = 0;
    p_record_module->encode_output_buf_w.capacity = p_record_module->buffer_set.output_cache_len;
    p_record_module->encode_output_buf_w.raw_data = p_record_module->buffer_set.output_cache;

    p_record_module->record_module_status.status = RECORD_MODULE_STA_START;
    p_record_module->record_module_status.err_no = MMM_MR_ERR_OK;

    if (p_record_module->encode_para.encode_mode != ENCODE_BYPASS)
    {
        if (fhandle->file_size == 0)
        {
            //get wav head info
            audio_encoder_cmd(p_record_module->audioenc_handle, AE_CMD_GET_FHDR, (uint32) (&(p_record_module->music_info)));
            //write must be align at 2KB，or some disk write slow
            buffer_rw_write(&(p_record_module->encode_output_buf_w), p_record_module->music_info.data_buf_addr, ENCODE_WAV_HEAD_SIZE);
        }
        p_record_module->record_module_status.write_pages = fhandle->file_size/512;
        if (p_record_module->record_module_status.write_pages != 0)
        {
            libc_printf("write_pages:%d\n", p_record_module->record_module_status.write_pages);
        }
    }

    return ret;

    start_record_encode_fail_ret_2:

    if (p_record_module->encode_para.encode_mode != ENCODE_BYPASS)
    {
        audio_encoder_cmd(p_record_module->audioenc_handle, AE_CMD_CLOSE, 0);
        p_record_module->audioenc_handle = NULL;
    }

    start_record_encode_fail_ret_1:

    if (p_record_module->encode_para.encode_mode != ENCODE_BYPASS)
    {
        //卸载数字音效库
        sys_drv_uninstall (REC_WAV_GROUP);
    }

    start_record_encode_fail_ret_0:

    return ret;
}
