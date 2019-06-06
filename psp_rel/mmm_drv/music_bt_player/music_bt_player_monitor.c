/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м����������ز�ʵʱ΢��CLOCK�������ݲ���ʱ����������
 * ���ߣ�cailizhen
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

    //�����Ա�ȷ���Ƿ��н��յ������ݰ�
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
    //���䲻��Ҫ�Լ���CLOCK�����Ǹ������������������Э����CLOCK
    if (p_tws_info->dev_role == TWS_SLAVE)
    {
        if (p_tws_info->need_slave_set_audio_aps == TRUE)
        {
            tws_slave_adjust_audio_aps_ing(music_player);
        }
        return;
    }

    //�������ڵ���Audio APS����ʱ��Ҫ��ȥ�ж��Ƿ���Ҫ����Audio APS
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
        uint8 tws_aps_adjust_flag = 0; //0��ʾ����Ҫ���ڸ���CLOCK��1��ʾ��Ҫ���ڸ���CLOCK��2��ʾ�����¸�ѭ�������ж�

        if ((sys_get_ab_timer() - p_tws_info->adjust_audio_aps_timer) < TWS_ADJUST_AUDIO_APS_TIMER)
        { //ÿ���϶�ʱ��ͽ���һ�ζ����жϣ����ܻ�΢�������CLOCK
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
    { //1S����һ��ͳ��

        music_player->adjust_sample_timer = sys_get_ab_timer();

        if (music_player->adjust_sample_high_counter < (music_player->adjust_sample_counter * MY_ADJUST_SAMPLE_HIGH_COUNT_FAST / 100))
        { //����DAC CLOCK
            if (music_player->audio_aps != music_player->audio_aps_info.slow_aps)
            {
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                if (p_tws_info->dev_role == TWS_MASTER)
                {
                    if (tws_master_adjust_audio_aps(music_player, music_player->audio_aps_info.slow_aps) < 0)
                    {
                        //�����ʧ�ܣ��Ե�50ms���ٳ���
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
        { //����DAC CLOCK
            if (music_player->audio_aps != music_player->audio_aps_info.fast_aps)
            {
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                if (p_tws_info->dev_role == TWS_MASTER)
                {
                    if (tws_master_adjust_audio_aps(music_player, music_player->audio_aps_info.fast_aps) < 0)
                    {
                        //�����ʧ�ܣ��Ե�50ms���ٳ���
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

    //����Ƿ񵽸���ĩβ�����������һ��ʱ�侲�������нϳ�ʱ��û�����ݴ��䣬����Ϊ�ǵ�β�����Ϳ���ǿ�Ƶ���
    zero_time = 128 * music_player->bt_zero_frame / music_player->music_setting.default_samplerate;
    if (zero_time > TAIL_SILENT_TIME)
    {
        cur_frame_space = sys_get_ab_timer() - music_player->last_frame_timestamp;
        if (cur_frame_space > TAIL_FRAME_SPACE_TIME)
        {
            libc_printf_info("tail fadeout:%d, %d\n", zero_time, cur_frame_space);
            if (music_player->buffer_time_length > 5)
            {
                music_player->buffer_time_length = 5; //ǿ�Ƶ���5ms
            }
        }
    }
#endif

    if (music_player->buffer_time_length < MY_FADE_OUT_THRESHOLD)
    {
        uint16 fade_out_time_bk = music_player->music_setting.fade_out_time;

        //��ʼ����
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
