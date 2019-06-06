/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件软按键音播放接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

#if (SUPPORT_SOFT_KEYTONE == 1)

uint8 __section__(".bss.keytone_input_buf") keytone_input_buffer[0x280];
uint8 __section__(".bss.keytone_output_buf") keytone_output_buffer[0x280];

int32 keytone_start(speech_player_t *speech_player, void *param)
{
    uint8 *ktd_file = (uint8 *)param;
    int kt_pcm_buf[4];

    speech_player->ktd_handle = sys_sd_fopen(ktd_file);
    if (speech_player->ktd_handle == NULL)
    {
        libc_printf_warning("open %s fail!\n", ktd_file);
        return -1;
    }

    sys_sd_fseek(speech_player->ktd_handle, SEEK_END, 0);
    speech_player->ktd_length = sys_sd_ftell(speech_player->ktd_handle);
    speech_player->ktd_length += 1; //定位到尾接口不对，定位到最后一个有效字节，所以需要加1
    speech_player->ktd_remain = speech_player->ktd_length;
    sys_sd_fseek(speech_player->ktd_handle, SEEK_SET, 0);

    libc_printf_info("start keytone:%s,0x%x\n", ktd_file, speech_player->ktd_length);

    speech_player->speech_status.ktp_status = MMM_KTP_ENGINE_PLAYING;
    speech_player->kt_play_enable = TRUE;

    return 0;
}

int32 keytone_stop(speech_player_t *speech_player)
{
    if (speech_player->kt_play_enable == FALSE)
    {
        return 0;
    }

    if (speech_player->ktd_handle == NULL)
    {
        sys_sd_fclose(speech_player->ktd_handle);
        speech_player->ktd_handle = NULL;
    }

    speech_player->speech_status.ktp_status = MMM_KTP_ENGINE_STOPPED;
    speech_player->kt_play_enable = FALSE;
    libc_printf_info("stop keytone\n");

    return 0;
}

int32 keytone_frame_deal(speech_player_t *speech_player, short *pcm_out, uint16 samples_count)
{
    int32 ret = 0;
    uint16 need_load_length;
    uint16 i;
    short *kt_src = keytone_output_buffer;
    uint8 *rs_output = keytone_output_buffer;

    if (speech_player->ktd_remain > 0)
    {
        need_load_length = samples_count*sizeof(short);

        if (need_load_length > speech_player->ktd_remain)
        {
            need_load_length = speech_player->ktd_remain;
        }

        sys_sd_fread(speech_player->ktd_handle, rs_output, need_load_length);

        speech_player->ktd_remain -= need_load_length;
        if (speech_player->ktd_remain == 0)
        {
            sys_sd_fclose(speech_player->ktd_handle);
            speech_player->ktd_handle = NULL;
        }
    }

    if (need_load_length < samples_count*sizeof(short))
    {
        libc_memset(rs_output + need_load_length, 0x0, samples_count*sizeof(short) - need_load_length);
    }

    for (i = 0; i < samples_count; i++)
    {
        pcm_out[i] = (short) mix_process(pcm_out[i], kt_src[i]);
    }

    if (speech_player->ktd_remain == 0)
    {
        speech_player->speech_status.ktp_status = MMM_KTP_ENGINE_STOPPED;
        speech_player->kt_play_enable = FALSE;
        libc_printf_info("stop keytone\n");
    }

    return ret;
}

int32 keytone_playend_deal(speech_player_t *speech_player)
{
    int32 ret = 0;

    libc_memset(PEQ_TEMP_BUFFER_ADDR, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*4);

    keytone_frame_deal(speech_player, PEQ_TEMP_BUFFER_ADDR, MY_POST_PROCESS_FRAME_SAMPLE);

    send_pcm_to_pcm_buf_by_dac_fifo(PEQ_TEMP_BUFFER_ADDR, MY_POST_PROCESS_FRAME_SAMPLE*2);

    return ret;
}

#endif
