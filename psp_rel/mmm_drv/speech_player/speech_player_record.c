/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件录音接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"
#include "sysdef.h"

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))

uint8 __section__(".stack.ap_rec_write_file_stack_ptos")rec_stk_ptos[1]; //动态数组
extern uint16 _REC_STK_SIZE;

#define DRV_NAME_SIZE 12
static const char rec_mod_name[DRV_NAME_SIZE] = "rec_mod.al";

uint8 __section__(".bss.encode_out_buffer") RECORD_FILE_ENCODE_OUT_BUF_ADDR[0x1000];

uint8 __section__(".bss.enc_source_buffer") ENCODE_SOURCE_BUF[ENCODE_SOURCE_BUF_LEN];
uint8 __section__(".bss.enc_source_buffer") ENCODE_IN_BUF[ENCODE_IN_BUF_LEN];
uint8 __section__(".bss.enc_source_buffer") ENCODE_OUT_BUF[ENCODE_OUT_BUF_LEN];

/* 启动录音 */
int32 start_speech_record(speech_player_t *speech_player)
{
    int32 ret = 0;
    record_module_buffer_set_t buffer_set;
    record_module_encode_detail_t encode_detail;

    //加载encode 编码库
    ret = (int32) sys_drv_install(REC_MOD_GROUP, 0, (void *) rec_mod_name);
    if (ret == -1)
    {
        while (1);
    }

    speech_player->encode_input_buf_w.capacity = ENCODE_SOURCE_BUF_LEN;
    speech_player->encode_input_buf_w.raw_data = ENCODE_SOURCE_BUF;

    //加载录音模块
    record_module_cmd(&(speech_player->rec_mod_handle), RECORD_MODULE_OPEN, 0);
    record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_SET_FS, &(speech_player->record_fs));
    //采样率配置，跟随蓝牙免提音源采样率
    if (speech_player->sample_rate == 16)
    {
        speech_player->encode_para.bitrate = 64000;
        speech_player->encode_para.sample_rate_hz = 16000;
    }
    else
    {
        speech_player->encode_para.bitrate = 32000;
        speech_player->encode_para.sample_rate_hz = 8000;
    }
    record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_SET_ENCODE, &(speech_player->encode_para));
    buffer_set.ptos = (rec_stk_ptos + (uint32)(&_REC_STK_SIZE));
    buffer_set.stk_size = (uint32)(&_REC_STK_SIZE);
    buffer_set.output_cache = RECORD_FILE_ENCODE_OUT_BUF_ADDR;
    buffer_set.output_cache_len = sizeof(RECORD_FILE_ENCODE_OUT_BUF_ADDR);
    buffer_set.output_buffer = ENCODE_OUT_BUF;
    record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_SET_BUFFER, &buffer_set);
    if (record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_START, 0) < 0)
    {
        //speech_player->record_status.err_no = MMM_MR_ERR_ENCODE_THREAD_CREATE_FAIL;
        libc_printf("creat_write_file_thread fail!!\n");
        ret = -1;
        //goto start_record_encode_fail_ret_0;
        while (1);
    }
    record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_GET_ENCODE_DETAIL, &encode_detail);
    speech_player->encode_frame_len = encode_detail.encode_frame_len;
    if (speech_player->encode_frame_len > ENCODE_IN_BUF_LEN)
    {
        libc_printf_error("encode frame in buffer not enough!\n");
        while (1);
    }

    speech_player->mmm_record_sta = MMM_RECORD_RECORDING;

    return ret;
}

int32 stop_speech_record(speech_player_t *speech_player)
{
    int32 ret = 0;

    speech_player->mmm_record_sta = MMM_RECORD_STOP;

    //关闭录音模块
    if (speech_player->g_speech_reset_buffer_flag == FALSE)
    {
        sys_os_sched_unlock();
    }
    record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_STOP, 0);
    if (speech_player->g_speech_reset_buffer_flag == FALSE)
    {
        sys_os_sched_lock();
    }
    record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_CLOSE, 0);
    speech_player->rec_mod_handle = 0;
    sys_drv_uninstall (REC_MOD_GROUP);

    return ret;
}

int32 player_encode_frame_deal(speech_player_t *speech_player)
{
    uint32 t1 = 0, t2;
    int32 ret = 0;

    while (speech_player->encode_input_buf_w.length >= speech_player->encode_frame_len)
    {
        //编码必须一帧一帧编码，帧长可由公式speech_player->music_info.channels*speech_player->music_info.block_size*sizeof(short)计算
        if (buffer_rw_read(&speech_player->encode_input_buf_w, ENCODE_IN_BUF,
                speech_player->encode_frame_len) == FALSE)
        {
            libc_printf("read encode_input_buffer err!\n");
        }

        if (speech_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        ret = record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_FRAME_DEAL, ENCODE_IN_BUF);
        record_module_cmd(speech_player->rec_mod_handle, RECORD_MODULE_GET_STATUS, &(speech_player->record_status));

        if (speech_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();
            speech_player->enc_play_time += (t2 - t1);
        }

        if (speech_player->record_status.status == RECORD_MODULE_STA_ERROR)
        {
            //发生录音错误，暂停录音，等待应用层处理
            speech_player->mmm_record_sta = MMM_RECORD_ERR_PAUSE;
            break;
        }

        if (ret < 0)
        {
            break;
        }
    }

    return ret;
}

int32 player_encode_source_deal(speech_player_t *speech_player, short *spk_src, short *mic_src, int32 samples_count)
{
    bool ret;
    short *dest = (short *) ENCODE_OUT_BUF;
    int i;

    //喇叭与MIC分开处理
    if (mic_src != NULL)
    {
        if (speech_player->encode_para.record_source == RECORD_SOURCE_LR_MIX)
        {
            for (i = 0; i < samples_count; i++)
            {
                dest[i] = (short) mix_process(spk_src[i], mic_src[i]);
            }
        }
        else if (speech_player->encode_para.record_source == RECORD_SOURCE_L_CH)
        {
            dest = spk_src;
        }
        else
        {
            dest = mic_src;
        }
    }
    //喇叭与MIC合并处理
    else
    {
        if ((speech_player->encode_para.record_source == RECORD_SOURCE_LR_MIX)
            || (speech_player->encode_para.record_source == RECORD_SOURCE_L_CH))
        {
            for (i = 0; i < samples_count; i++)
            {
                dest[i] = spk_src[i*2];
            }
        }
        else
        {
            for (i = 0; i < samples_count; i++)
            {
                dest[i] = spk_src[i*2 + 1];
            }
        }
    }

    ret = buffer_rw_write(&(speech_player->encode_input_buf_w), dest, samples_count*sizeof(short));
    if (ret == FALSE)
    {
        libc_printf_warning("write encode_input_buf_w full!\n");
    }

    return 0;
}

#endif
