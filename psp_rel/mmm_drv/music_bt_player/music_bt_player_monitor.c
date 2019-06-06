/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件缓冲区监控并实时微调CLOCK，当数据不足时启动淡出。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"

uint16 calc_buffer_time_length(music_bt_player_t *music_player)
{
    uint32 buffer_time_length;
    uint16 temp_write_ptr;
    uint32 temp_flag = sys_local_intc_save(0x01);
    temp_write_ptr = music_player->input_buf->write_ptr;
    sys_local_intc_restore(temp_flag);

    music_player->cur_timestamp = sys_get_ab_timer();

    if (music_player->buffer_time_length == (uint16)(-1))
    {
        music_player->last_frame_timestamp = music_player->cur_timestamp;
    }
    else
    {
        if (temp_write_ptr != music_player->write_ptr_backup)
        {
            uint32 frame_space = music_player->cur_timestamp - music_player->last_frame_timestamp;

            if (frame_space > MY_FRAME_SPACE_THRESHOLD)
            {
                libc_printf("frame space:%d\n", frame_space);
            }

            music_player->last_frame_timestamp = music_player->cur_timestamp;
        }
    }

    //备份以便确定是否有接收到新数据包
    music_player->write_ptr_backup = temp_write_ptr;

    sbc_calc_frame_count(music_player);

    buffer_time_length = 128 * music_player->frame_count / music_player->music_setting.default_samplerate;

    music_player->buffer_time_length = (uint16) buffer_time_length;

    return (uint16) buffer_time_length;
}

void monitor_aps_adjust(music_bt_player_t *music_player)
{
    uint16 tmp_time_length = music_player->buffer_time_length;

    if (tmp_time_length < music_player->buffer_time_min)
    {
        libc_printf("buffer remain:%d\n", tmp_time_length);

        music_player->buffer_time_min = tmp_time_length;
    }

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    //副箱不需要自己调CLOCK，而是根据主箱的命令与主箱协调调CLOCK
    if (p_tws_info->dev_role == TWS_SLAVE)
    {
        if (p_tws_info->need_slave_set_audio_aps == TRUE)
        {
            tws_slave_adjust_audio_aps_ing(music_player);
        }
        return;
    }

    //对箱正在调节Audio APS，暂时不要再去判断是否需要调节Audio APS
    if (p_tws_info->dev_role == TWS_MASTER)
    {
        if (p_tws_info->slave_is_setting_audio_aps_m == TRUE)
        {
            tws_master_wait_adjust_audio_aps_over(music_player);

            music_player->adjust_sample_high_counter = 0;
            music_player->adjust_sample_counter = 0;

            return;
        }
    }
#endif

    //sample deal
    music_player->adjust_sample_counter++;

    if (tmp_time_length >= music_player->music_setting.start_delay_time)
    {
        music_player->adjust_sample_high_counter++;
    }

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

    if (p_tws_info->dev_role == TWS_MASTER)
    {
        uint8 tws_aps_adjust_flag = 0; //0表示不需要调节副箱CLOCK，1表示需要调节副箱CLOCK，2表示待定下个循环继续判断

        if ((sys_get_ab_timer() - p_tws_info->adjust_audio_aps_timer) < TWS_ADJUST_AUDIO_APS_TIMER)
        { //每隔较短时间就进行一次对齐判断，可能会微调副箱的CLOCK
            return;
        }

        tws_aps_adjust_flag = tws_monitor_aps_adjust_fastslow(music_player);

        if (tws_aps_adjust_flag != 2)
        {
            p_tws_info->adjust_audio_aps_timer = sys_get_ab_timer();
        }

        if (tws_aps_adjust_flag > 0)
        {
            return;
        }
    }

#endif

    if ((sys_get_ab_timer() - music_player->adjust_sample_timer) >= MY_ADJUST_SAMPLE_TIMER)
    { //1S进行一次统计

        music_player->adjust_sample_timer = sys_get_ab_timer();

        if (music_player->adjust_sample_high_counter < (music_player->adjust_sample_counter * MY_ADJUST_SAMPLE_HIGH_COUNT_FAST / 100))
        { //调慢DAC CLOCK
            if (music_player->audio_aps != music_player->audio_aps_info.slow_aps)
            {
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                if (p_tws_info->dev_role == TWS_MASTER)
                {
                    if (tws_master_adjust_audio_aps(music_player, music_player->audio_aps_info.slow_aps) < 0)
                    {
                        //命令发送失败，稍等50ms后再尝试
                        return;
                    }
                }
                else
#endif
                {
                    set_audio_aps (music_player->audio_aps_info.slow_aps);
                    music_player->audio_aps = music_player->audio_aps_info.slow_aps;
                }

#ifdef BT_MMM_DEBUG_INFO
                libc_printf("AUD_APS_SLOW:%d\n", music_player->buffer_time_length);
#endif
            }
        }
        else if (music_player->adjust_sample_high_counter > (music_player->adjust_sample_counter * MY_ADJUST_SAMPLE_HIGH_COUNT_SLOW / 100))
        { //调快DAC CLOCK
            if (music_player->audio_aps != music_player->audio_aps_info.fast_aps)
            {
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                if (p_tws_info->dev_role == TWS_MASTER)
                {
                    if (tws_master_adjust_audio_aps(music_player, music_player->audio_aps_info.fast_aps) < 0)
                    {
                        //命令发送失败，稍等50ms后再尝试
                        return;
                    }
                }
                else
#endif
                {
                    set_audio_aps (music_player->audio_aps_info.fast_aps);
                    music_player->audio_aps = music_player->audio_aps_info.fast_aps;
                }

#ifdef BT_MMM_DEBUG_INFO
                libc_printf("AUD_APS_FAST:%d\n", music_player->buffer_time_length);
#endif
            }
        }

        music_player->adjust_sample_high_counter = 0;
        music_player->adjust_sample_counter = 0;
    }
}

void monitor_fadeout_check(music_bt_player_t *music_player)
{
#if (TAIL_AUTO_DETECT_ENABLE == 1)
    uint32 zero_time, cur_frame_space;

    //检查是否到歌曲末尾，即如果持续一段时间静音，且有较长时间没有数据传输，就认为是到尾部，就可以强制淡出
    zero_time = 128 * music_player->bt_zero_frame / music_player->music_setting.default_samplerate;
    if (zero_time > TAIL_SILENT_TIME)
    {
        cur_frame_space = sys_get_ab_timer() - music_player->last_frame_timestamp;
        if (cur_frame_space > TAIL_FRAME_SPACE_TIME)
        {
            libc_printf_info("tail fadeout:%d, %d\n", zero_time, cur_frame_space);
            if (music_player->buffer_time_length > 5)
            {
                music_player->buffer_time_length = 5; //强制淡出5ms
            }
        }
    }
#endif

    if (music_player->buffer_time_length < MY_FADE_OUT_THRESHOLD)
    {
        uint16 fade_out_time_bk = music_player->music_setting.fade_out_time;

        //开始淡出
        if (music_player->buffer_time_length < fade_out_time_bk)
        {
            music_player->music_setting.fade_out_time = music_player->buffer_time_length;
            if (music_player->music_setting.fade_out_time < 5)
            {
                music_player->music_setting.fade_out_time = 5;
            }
        }

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
        if (p_tws_info->dev_role != NORMAL_DEV)
        {
            stop_music_decoder_fadeout_start(music_player, TRUE);
        }
        else
#endif
        {
            stop_music_decoder_fadeout_start(music_player, FALSE);
        }

        music_player->music_setting.fade_out_time = fade_out_time_bk;
    }
}
