/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：usound播歌中间件缓冲区监控并实时微调CLOCK，当数据不足时启动淡出。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"

void monitor_aps_adjust(music_pcm_player_t *music_player)
{
    uint16 cur_length;
    uint16 tmp_block = 0;

    cur_length = music_player->p_input_buf_r->length;
    tmp_block = cur_length / PCM_DEC_BUF_BLOCK;

    if (tmp_block < music_player->usound_min_block)
    {
        music_player->usound_min_block = tmp_block;
        libc_printf_info("min block:%d\n", music_player->usound_min_block);
    }
    if (tmp_block > music_player->usound_max_block)
    {
        music_player->usound_max_block = tmp_block;
        libc_printf_info("max block:%d\n", music_player->usound_max_block);
    }

    //sample deal
    music_player->adjust_sample_counter++;
    if (cur_length >= music_player->adjust_block_thres)
    {
        music_player->adjust_sample_high_counter++;
    }

    if (music_player->adjust_sample_counter > MY_ADJUST_SAMPLE_COUNT)
    {
        if (music_player->adjust_sample_high_counter < MY_ADJUST_SAMPLE_HIGH_COUNT)
        {
            if (music_player->audio_aps != music_player->audio_aps_info.slow_aps)
            {
                set_audio_aps (music_player->audio_aps_info.slow_aps);
#ifdef PCM_MMM_DEBUG_INFO
                libc_printf("AUD_APS_SLOW! block:%d\n", tmp_block);
#endif
                music_player->audio_aps = music_player->audio_aps_info.slow_aps;
                music_player->adjust_step_cnt = 0;
            }
        }
        else
        {
            if (music_player->audio_aps != music_player->audio_aps_info.fast_aps)
            {
                set_audio_aps (music_player->audio_aps_info.fast_aps);
#ifdef PCM_MMM_DEBUG_INFO
                libc_printf("AUD_APS_FAST! block:%d\n", tmp_block);
#endif
                music_player->audio_aps = music_player->audio_aps_info.fast_aps;
                music_player->adjust_step_cnt = 0;
            }
        }
        music_player->adjust_sample_high_counter = 0;
        music_player->adjust_sample_counter = 0;
    }
}

void monitor_fadeout_check(music_pcm_player_t *music_player)
{
    if (music_player->p_input_buf_r->length < MY_FADE_OUT_LENGTH_THRES)
    {
        uint16 fade_out_time_bk = music_player->music_setting.fade_out_time;

        //开始淡出，淡出时间可以很短，比如2ms
        music_player->music_setting.fade_out_time = 2;

        //启动淡出
        stop_music_decoder_fadeout_start(music_player, FALSE);

        music_player->music_setting.fade_out_time = fade_out_time_bk;
    }
}

