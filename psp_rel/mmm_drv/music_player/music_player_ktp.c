/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：本地播歌中间件软按键音播放接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_SOFT_KEYTONE == 1)

uint8 __section__(".bss.keytone_input_buf") keytone_input_buffer[0x280];
uint8 __section__(".bss.keytone_output_buf") keytone_output_buffer[0x280];

int32 keytone_start(music_player_t *music_player, void *param)
{
    uint8 *ktd_file = (uint8 *)param;
    int kt_pcm_buf[4];

    music_player->ktd_handle = sys_sd_fopen(ktd_file);
    if (music_player->ktd_handle == NULL)
    {
        libc_printf_warning("open %s fail!\n", ktd_file);
        return -1;
    }

    sys_sd_fseek(music_player->ktd_handle, SEEK_END, 0);
    music_player->ktd_length = sys_sd_ftell(music_player->ktd_handle);
    music_player->ktd_length += 1; //定位到尾接口不对，定位到最后一个有效字节，所以需要加1
    music_player->ktd_remain = music_player->ktd_length;
    sys_sd_fseek(music_player->ktd_handle, SEEK_SET, 0);

    music_player->rs_src_sr = 8000;
    if (music_player->music_info.sample_rate == 48)
    {
        music_player->rs_dest_sr = 48000;
    }
    else if (music_player->music_info.sample_rate == 44)
    {
        music_player->rs_dest_sr = 44100;
    }
    else if (music_player->music_info.sample_rate == 32)
    {
        music_player->rs_dest_sr = 32000;
    }
    else if (music_player->music_info.sample_rate == 24)
    {
        music_player->rs_dest_sr = 24000;
    }
    else if (music_player->music_info.sample_rate == 22)
    {
        music_player->rs_dest_sr = 22050;
    }
    else if (music_player->music_info.sample_rate == 16)
    {
        music_player->rs_dest_sr = 16000;
    }
    else if (music_player->music_info.sample_rate == 12)
    {
        music_player->rs_dest_sr = 12000;
    }
    else if (music_player->music_info.sample_rate == 11)
    {
        music_player->rs_dest_sr = 11025;
    }
    else
    {
        music_player->rs_dest_sr = 8000;
    }
    if (music_player->rs_src_sr != music_player->rs_dest_sr)
    {
        resample_reset(&(music_player->rs_handle), music_player->rs_src_sr, music_player->rs_dest_sr);
    }
    music_player->rs_remain_len = 0;

    libc_printf_info("start keytone:%s,0x%x\n", ktd_file, music_player->ktd_length);

    music_player->music_status.ktp_status = MMM_KTP_ENGINE_PLAYING;
    music_player->kt_play_enable = TRUE;

    return 0;
}

int32 keytone_stop(music_player_t *music_player)
{
    if (music_player->kt_play_enable == FALSE)
    {
        return 0;
    }

    if (music_player->ktd_handle == NULL)
    {
        sys_sd_fclose(music_player->ktd_handle);
        music_player->ktd_handle = NULL;
    }

    music_player->music_status.ktp_status = MMM_KTP_ENGINE_STOPPED;
    music_player->kt_play_enable = FALSE;
    libc_printf_info("stop keytone\n");

    return 0;
}

int32 keytone_frame_deal(music_player_t *music_player, short *pcm_out, uint16 samples_count)
{
    int32 ret = 0;
    uint16 need_rs_length, need_load_length;
    uint16 i;
    short *kt_src = keytone_output_buffer;
    uint8 *rs_input = keytone_input_buffer;
    uint8 *rs_output = keytone_output_buffer;

    if (music_player->rs_remain_len < samples_count*sizeof(short))
    {
        int rs_input_samples;
        int rs_output_samples;

        if (music_player->ktd_remain > 0)
        {
            need_rs_length = samples_count*sizeof(short) - music_player->rs_remain_len;
            if (music_player->rs_src_sr != music_player->rs_dest_sr)
            {
                need_load_length = (need_rs_length*music_player->rs_src_sr)/music_player->rs_dest_sr;
                need_load_length &= ~0x3;
                need_load_length += 8; //每次多取至少2个数据源点
            }
            else
            {
                need_load_length = need_rs_length;
            }

            if (need_load_length > music_player->ktd_remain)
            {
                need_load_length = music_player->ktd_remain;
            }

            sys_sd_fread(music_player->ktd_handle, rs_input, need_load_length);

            music_player->ktd_remain -= need_load_length;
            if (music_player->ktd_remain == 0)
            {
                sys_sd_fclose(music_player->ktd_handle);
                music_player->ktd_handle = NULL;
            }

            if (music_player->rs_src_sr != music_player->rs_dest_sr)
            {
                rs_input_samples = need_load_length/sizeof(short);
                resample(&(music_player->rs_handle), rs_input, rs_input_samples, rs_output + music_player->rs_remain_len, &rs_output_samples, 1);
                //libc_printf("rs:%d,%d\n", need_load_length, rs_output_samples*sizeof(short));
                music_player->rs_remain_len += rs_output_samples*sizeof(short);
            }
            else
            {
                libc_memcpy(rs_output, rs_input, need_load_length);
                music_player->rs_remain_len += need_load_length;
            }
        }

        if (music_player->rs_remain_len < samples_count*sizeof(short))
        {
            libc_memset(rs_output + music_player->rs_remain_len, 0x0, samples_count*sizeof(short) - music_player->rs_remain_len);
            music_player->rs_remain_len = samples_count*sizeof(short);
        }
    }

    if (music_player->rs_remain_len >= samples_count*sizeof(short))
    {
        for (i = 0; i < samples_count; i++)
        {
            pcm_out[i*2 + 0] = (short) mix_process(pcm_out[i*2 + 0], kt_src[i]);
            pcm_out[i*2 + 1] = (short) mix_process(pcm_out[i*2 + 1], kt_src[i]);
        }

        music_player->rs_remain_len -= samples_count*sizeof(short);
        if (music_player->rs_remain_len > 0)
        {
            libc_memcpy(kt_src, kt_src + samples_count, music_player->rs_remain_len);
        }
    }

    if ((music_player->ktd_remain == 0) && (music_player->rs_remain_len == 0))
    {
        music_player->music_status.ktp_status = MMM_KTP_ENGINE_STOPPED;
        music_player->kt_play_enable = FALSE;
        libc_printf_info("stop keytone\n");
    }

    return ret;
}

int32 keytone_playend_deal(music_player_t *music_player)
{
    int32 ret = 0;

    libc_memset((void*)DAE_128_ALIGN_BUF_ADDR, 0x00, MY_POST_PROCESS_FRAME_SAMPLE * 4);

    keytone_frame_deal(music_player, DAE_128_ALIGN_BUF_ADDR, MY_POST_PROCESS_FRAME_SAMPLE);

    send_pcm_to_pcm_buf_by_dac_fifo(DAE_128_ALIGN_BUF_ADDR, MY_POST_PROCESS_FRAME_SAMPLE * 2);

    return ret;
}

#endif
