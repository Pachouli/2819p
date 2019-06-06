/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：AUX/FM/USB播歌中间件解码线程主循环。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"

PP_para_info_t g_PP_para_info;

//0x01表示PCM_BUF半空中断，0x02表示PCM_BUF全空中断，0x04表示PCM_BUF半满中断
void pcm_pcm_buf_int_cbk(uint8 pending)
{
    if (music_player_global->pcm_dec_sem != NULL)
    {
        music_player_global->g_pp_pcm_buf_pending |= pending;
        if ((pending & 0x03) != 0)
        {
            libc_sem_post(music_player_global->pcm_dec_sem);
        }
    }
}

//使用同一片内存进行转换，单声道数据放在缓冲区后半部分
static void single_to_dual_memcpy(short *dest, int sample_cnt)
{
    short *src = dest + sample_cnt;
    short temp_point;
    int index;

    for (index = 0; index < sample_cnt; index++)
    {
        temp_point = *(src + index);
        *(dest + index * 2) = temp_point;
        *(dest + index * 2 + 1) = temp_point;
    }
}

int32 player_resample_frame_deal(void)
{
    music_pcm_player_t *music_player = music_player_global;
    int out_samples;
    int i;

    if (music_player->resample_handle_left != NULL)
    {
        while (music_player->p_resample_lch_r->length >= 256)
        {
            buffer_rw_read(music_player->p_resample_lch_r, RESAMPLE_INPUT_BUFFER, 256);
            out_samples = pp_resample_proccess(music_player->resample_handle_left, RESAMPLE_INPUT_BUFFER, \
                    128, RESAMPLE_OUTPUT_BUFFER[0]);

            buffer_rw_read(music_player->p_resample_rch_r, RESAMPLE_INPUT_BUFFER, 256);
            out_samples = pp_resample_proccess(music_player->resample_handle_right, RESAMPLE_INPUT_BUFFER, \
                    128, RESAMPLE_OUTPUT_BUFFER[1]);

            for (i = 0; i < out_samples; i++)
            {
                RESAMPLE_INPUT_BUFFER[i*2 + 0] = RESAMPLE_OUTPUT_BUFFER[0][i];
                RESAMPLE_INPUT_BUFFER[i*2 + 1] = RESAMPLE_OUTPUT_BUFFER[1][i];
            }

            music_player->feedback_data_cbk(RESAMPLE_INPUT_BUFFER, 256);
        }
    }

    return 0;
}

int32 player_decode_daedeal(uint8 *output_buf, uint16 output_len)
{
    music_pcm_player_t *music_player = music_player_global;
    int32 ret = 0;
    bool fill_empty_flag = FALSE;
    uint32 t1 = 0, t2 = 0;

    if (music_player->need_print_time == TRUE)
    {
        t1 = sys_get_ab_timer_us();
    }

    if (music_player->adc_channel_handle != NULL)
    {
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_ENERGY, (uint32)(&(music_player->music_status.adc_energy)));
    }

    if (music_player->is_adc_dual_flag == TRUE)
    {
        if (buffer_rw_read(music_player->p_input_buf_r, output_buf, MY_PCM_DEC_BUF_BLOCK) == FALSE)
        {
            fill_empty_flag = TRUE;
        }
    }
    else
    {
        if (buffer_rw_read(music_player->p_input_buf_r, (short *) output_buf + MY_PCM_DEC_BUF_BLOCK/4, MY_PCM_DEC_BUF_BLOCK/2) == TRUE)
        {
#if (SUPPORT_OK_MIC == 1)
            if ((music_player->work_mode == PP_WORK_MODE_MIC)
                && (music_player->is_idle_mode == TRUE))
            {
                libc_memset((short *) output_buf + MY_PCM_DEC_BUF_BLOCK/4, 0x00, MY_PCM_DEC_BUF_BLOCK/2);
            }
#endif
            single_to_dual_memcpy((short *) output_buf, MY_PCM_DEC_BUF_BLOCK/4);
        }
        else
        {
            fill_empty_flag = TRUE;
        }
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    if (music_player->work_mode == PP_WORK_KEYTONE)
    {
        libc_memset((short *) output_buf, 0x00, MY_PCM_DEC_BUF_BLOCK);
    }
#endif

    if (music_player->music_setting.pcm_source == PCM_SOURCE_USB)
    {
        energy_calc(output_buf, MY_PCM_DEC_BUF_BLOCK/4, TRUE,
                &(music_player->music_status.adc_energy.adc_cur_energy),
                &(music_player->music_status.adc_energy.adc_cur_energy_rms));
    }

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == PP_WORK_MODE_MIC)
    {
        //libc_printf("mic:%d,%d=%d\n", music_player->p_micin_buf_r->length, get_pcmbuf_length(),\
        //    (music_player->p_micin_buf_r->length + get_pcmbuf_length())*5/music_player->music_setting.default_samplerate);

        ok_mic_data_read_again:
        if (music_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH)
        {
            if (music_player->music_status.status == MMM_PP_ENGINE_PLAYING)
            {
                music_player->ok_mic_start_flag = OK_MIC_DATA_BUFFER;
                goto ok_mic_data_read_again;
            }
            //清空Buffer
            buffer_rw_flush(music_player->p_micin_buf_r);
            libc_memset(ok_mic_temp_buffer, 0x00, MY_PCM_DEC_BUF_BLOCK/2);
        }
        else if (music_player->ok_mic_start_flag == OK_MIC_DATA_BUFFER)
        {
            if (music_player->p_micin_buf_r->length >= OK_MIC_RAW_BUF_THRESHOLD)
            {
                music_player->ok_mic_start_flag = OK_MIC_DATA_READ;
                goto ok_mic_data_read_again;
            }
            libc_memset(ok_mic_temp_buffer, 0x00, MY_PCM_DEC_BUF_BLOCK/2);
        }
        else
        {
            if (buffer_rw_read(music_player->p_micin_buf_r, (short *) ok_mic_temp_buffer, MY_PCM_DEC_BUF_BLOCK/2) == FALSE)
            {
                libc_memset(ok_mic_temp_buffer, 0x00, MY_PCM_DEC_BUF_BLOCK/2);
                libc_printf_warning("mic empty!\n");
            }
        }

        music_player->pp_aout.pcm[1] = ok_mic_temp_buffer;
        if (music_player->ok_mic_mute_flag == TRUE)
        {
            libc_memset(ok_mic_temp_buffer, 0x00, MY_PCM_DEC_BUF_BLOCK/2);
        }
    }
#endif

    if (music_player->need_print_time == TRUE)
    {
        t2 = sys_get_ab_timer_us();

        music_player->dec_play_time += (t2 - t1);
    }

    if (fill_empty_flag == TRUE)
    {
        libc_memset(output_buf, 0x00, MY_PCM_DEC_BUF_BLOCK);
        libc_printf_warning("empty!\n");
    }

    music_player->pp_aout.pcm[0] = output_buf;
    music_player->pp_aout.samples = MY_POST_PROCESS_FRAME_SAMPLE;
    ret = post_process_ops(music_player->audiopp_handle, PP_CMD_FRAME_PROCESS, (uint32) &music_player->pp_aout);
    if (ret != 0)
    {
        libc_printf_warning("post_process_ops frame error:%d\n", ret);
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    if ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
    {
        keytone_frame_deal(music_player, output_buf, MY_POST_PROCESS_FRAME_SAMPLE);
    }
#endif

    //获取多频点能量值
    if (g_freq_point_cfg.freq_point_enable == TRUE)
    {
        if (post_process_ops(music_player->audiopp_handle, PP_CMD_GET_FREQ_POINT_ENERGY,
            (uint32) music_player->music_status.freq_point_energy) < 0)
        {
            libc_memset(music_player->music_status.freq_point_energy, 0x00, sizeof(music_player->music_status.freq_point_energy));
        }
    }

    if (music_player->need_print_time == TRUE)
    {
        t1 = sys_get_ab_timer_us();

        music_player->dae_play_time += (t1 - t2);

        music_player->play_samples_add += MY_PCM_DEC_BUF_BLOCK/4;
    }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    //经过DAE后音频流编码写文件处理
    if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta == MMM_RECORD_RECORDING))
    {
        player_encode_source_deal(music_player, music_player->pp_aout.pcm[0], music_player->pp_aout.samples);
    }
#endif

    //关闭监听，将数据清0播放
    if (music_player->is_monitoring_flag == FALSE)
    {
        libc_memset(music_player->pp_aout.pcm[0], 0x00, music_player->pp_aout.samples * 2 * 2);
    }

    send_pcm_to_pcm_buf_by_dac_fifo(music_player->pp_aout.pcm[0], music_player->pp_aout.samples * 2);

    return ret;
}

void *player_loop(music_pcm_player_t *music_player)
{
    int32 ret = 0;
    uint32 tmp_fill_timer;
    bool need_sleep_flag = FALSE;
    uint32 start_print_time_timer = 0;
    bool need_start_print_time = TRUE;

    for (;;)
    {
        if (music_player->thread_terminate_flag == TRUE)
        {
            goto player_loop_exit;
        }

        if ((music_player->music_status.status == MMM_PP_ENGINE_ERROR)
                || (music_player->is_player_loop_enable == FALSE))
        {
            goto loop_continue;
        }

        if (music_player->music_status.status == MMM_PP_ENGINE_PAUSE)
        {
            goto loop_continue;
        }

        if ((need_start_print_time == TRUE) && (start_print_time_timer > 0))
        {
            if ((sys_get_ab_timer() - start_print_time_timer) > 10*1000) //开始播放后10S开始统计
            {
                libc_printf("start moniter\n");
                need_start_print_time = FALSE;
                music_player->need_print_time = TRUE;
                sys_performance_monitoring_restart();
            }
        }

        if (music_player->music_status.status == MMM_PP_ENGINE_READY)
        {
            bool has_enough_data = FALSE;

#if (SUPPORT_OK_MIC == 1)
            if ((music_player->work_mode == PP_WORK_MODE_MIC)
                && (music_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH))
            {
                //清空Buffer
                buffer_rw_flush(music_player->p_micin_buf_r);
            }
#endif

            if ((music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
                && (music_player->need_update_samplerate == TRUE))
            {
                get_sample_rate_ret_t get_sample_rate_ret;
                adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_SAMPLERATE, &get_sample_rate_ret);
                if (get_sample_rate_ret.sample_rate_ok == TRUE)
                {
                    music_player->music_setting.default_samplerate = get_sample_rate_ret.sample_rate_khz;
                    music_player->need_update_samplerate = FALSE;

                    music_player->need_resample = get_sample_rate_ret.need_resample;
                    music_player->resample_src_khz = get_sample_rate_ret.resample_src_khz;
                    music_player->resample_dest_khz = get_sample_rate_ret.sample_rate_khz;
                    libc_printf("need_resample:%d,%d->%d\n", music_player->need_resample,
                            music_player->resample_src_khz, music_player->resample_dest_khz);

                    if ((music_player->music_setting.adc_chan_set.user_type == ADC_USER_HDMI_ARC)
                        && (music_player->music_setting.start_delay_time != 0))
                    {
                        music_player->adjust_block_thres = music_player->music_setting.start_delay_time
                            * music_player->music_setting.default_samplerate * 4;
                        if ((music_player->adjust_block_thres % PCM_ADC_BUF_BLOCK) != 0)
                        {
                            music_player->adjust_block_thres = (music_player->adjust_block_thres / PCM_ADC_BUF_BLOCK) * PCM_ADC_BUF_BLOCK;
                            music_player->adjust_block_thres += PCM_ADC_BUF_BLOCK;
                        }
                    }
                }
            }

            player_resample_frame_deal();

            if (music_player->p_input_buf_r->length > 0)
            {
                uint32 wait_enough_data_timer = sys_get_ab_timer();

                while (1)
                {
                    player_resample_frame_deal();

                    if (music_player->need_adjust_aps == TRUE)
                    {
                        if (music_player->is_adc_dual_flag == TRUE)
                        {
                            if (music_player->p_input_buf_r->length >= (music_player->adjust_block_thres + PCM_ADC_BUF_BLOCK))
                            {
                                has_enough_data = TRUE;
                                break;
                            }
                        }
                        else
                        {
                            if (music_player->p_input_buf_r->length >= (music_player->adjust_block_thres + PCM_ADC_BUF_BLOCK)/2)
                            {
                                has_enough_data = TRUE;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (music_player->is_adc_dual_flag == TRUE)
                        {
                            if (music_player->p_input_buf_r->length >= PCM_DEC_BUF_START_THRES)
                            {
                                has_enough_data = TRUE;
                                break;
                            }
                        }
                        else
                        {
                            if (music_player->p_input_buf_r->length >= PCM_DEC_BUF_START_THRES/2)
                            {
                                has_enough_data = TRUE;
                                break;
                            }
                        }
                    }

                    if ((sys_get_ab_timer() - wait_enough_data_timer) > 500)
                    {
                        buffer_rw_flush(music_player->p_input_buf_r); //很长时间都没有足够数据，表明是数据残留，直接清空
                        libc_printf_warning("wait enough data overtime!\n");
                        break;
                    }
                }
            }

            if (has_enough_data == TRUE)
            {
                if (need_start_print_time == TRUE)
                {
                    start_print_time_timer = sys_get_ab_timer();
                }

                //启动淡入-播放
                start_music_decoder_sub(music_player);
            }
            else
            {
                goto loop_continue;
            }
        }

        if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        {
            if ((music_player->music_status.status == MMM_PP_ENGINE_PLAYING)
                || (music_player->music_status.status == MMM_PP_ENGINE_FADEIN))
            {
                get_sample_rate_ret_t get_sample_rate_ret;
                adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_SAMPLERATE, &get_sample_rate_ret);
                if (get_sample_rate_ret.need_update == TRUE)
                {
                    uint32 fadeout_time_backup;
                    //启动淡出，然后会重新调整采样率
                    fadeout_time_backup = music_player->music_setting.fade_out_time;
                    music_player->music_setting.fade_out_time = 5;
                    stop_music_decoder_fadeout_start(music_player, FALSE);
                    music_player->music_setting.fade_out_time = fadeout_time_backup;
                }
            }
        }

        if (music_player->need_adjust_aps == TRUE)
        {
            //监控缓冲区并微调频率，以维持缓冲区稳定在合适水平上
            if (music_player->music_status.status == MMM_PP_ENGINE_PLAYING)
            {
                monitor_aps_adjust(music_player);
            }

            //监控缓冲区，并适时启动淡出
            if ((music_player->music_status.status == MMM_PP_ENGINE_PLAYING)
                    || (music_player->music_status.status == MMM_PP_ENGINE_FADEIN))
            {
                monitor_fadeout_check(music_player);
            }
        }

        /*解码和数字音效处理*/
        tmp_fill_timer = sys_get_ab_timer();

        while (1)
        {
            player_resample_frame_deal();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
            //录音帧处理
            if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta == MMM_RECORD_RECORDING))
            {
                player_encode_frame_deal(music_player);
            }
#endif

            player_decode_daedeal((uint8 *) PCM_DEC_BUF_ADDR, MY_PCM_DEC_BUF_BLOCK);

            if (music_player->g_pp_pcm_buf_int_en_flag == FALSE)
            {
#if (SUPPORT_OK_MIC == 1)
                if (music_player->work_mode == PP_WORK_MODE_MIC)
                {
                    music_player->g_half_empty_thres = OK_MIC_PCM_BUF_HE_THRESHOLD;
                }
                else
#endif
                {
                    music_player->g_half_empty_thres = PCM_BUF_HE_THRESHOLD;
                }
                music_player->g_half_full_thres = music_player->g_half_empty_thres + 16;

                init_pcm_buf(music_player->g_half_empty_thres, music_player->g_half_full_thres);

                //start DAC PLAY
                music_player->g_pp_pcm_buf_int_en_flag = TRUE;

                if (libc_sem_init(&(music_player->pcm_dec_sem), 0) == -1)
                {
                    libc_printf("pcm_dec_sem\n");
                    while (1)
                    {
                        ; //for QAC
                    }
                }
            }

            if (((music_player->g_pp_pcm_buf_pending & 0x04) != 0)
                    || (get_pcmbuf_length() >= music_player->g_half_full_thres))
            {
                if ((music_player->g_pp_pcm_buf_pending & 0x04) == 0)
                {
#ifdef PCM_MMM_DEBUG_INFO
                    libc_printf_warning("pcmbuf length unexpect!\n");
#endif
                    music_player->pcm_buf_hd_err = TRUE;
                }

                music_player->g_pp_pcm_buf_pending &= ~0x04;

                if (music_player->pcm_buf_hd_err == TRUE)
                {
                    uint32 hd_err_timer = sys_get_ab_timer();

                    while (1)
                    {
                        if (get_pcmbuf_length() < music_player->g_half_empty_thres)
                        {
#ifdef PCM_MMM_DEBUG_INFO
                            libc_printf("wait length ok!\n");
#endif
                            break;
                        }

                        if ((sys_get_ab_timer() - hd_err_timer) > 50)
                        {
                            libc_printf_warning("wait length overtime!\n");
                            break;
                        }
                    }

                    music_player->pcm_buf_hd_err = FALSE;
                }
                else
                {
                    break;
                }
            }

            if ((music_player->g_pp_pcm_buf_pending & 0x02) != 0)
            {
                libc_printf_warning("dec too late!!\n");
            }

            while (libc_sem_trywait(music_player->pcm_dec_sem) == 0)
            {
                ; //清空信号量
            }
            music_player->g_pp_pcm_buf_pending = 0;

            if ((sys_get_ab_timer() - tmp_fill_timer) >= 1000)
            {
                need_sleep_flag = TRUE;
                libc_printf_warning("fill overtime!\n");
                break;
            }
        }



        if (music_player->need_print_time == TRUE)
        {
            uint32 tmp_time_ms;

            tmp_time_ms = music_player->play_samples_add / music_player->music_setting.default_samplerate;
            if (tmp_time_ms > 5000)
            {
                sys_performance_monitoring_report();
                sys_performance_monitoring_stop();

                music_player->need_print_time = FALSE;
                libc_printf("pcm al cost:dec=%d,enc=%d,dae=%d,release=%d\n",
                        (music_player->dec_play_time / 1000),
                        (music_player->enc_play_time / 1000),
                        (music_player->dae_play_time / 1000),
                        (music_player->release_time / 1000));
            }
        }

        if (music_player->music_status.status == MMM_PP_ENGINE_FADEIN)
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32) &tmp_pp_status);
#ifdef PCM_MMM_DEBUG_INFO
            libc_printf("fadein:%d\n", tmp_pp_status.fade_in_status);
#endif
            if (tmp_pp_status.fade_in_status == PP_FADE_IN_STANDBY)
            {
                libc_printf_info("fadein over\n");
                music_player->music_status.status = MMM_PP_ENGINE_PLAYING;
            }
        }

        if ((music_player->music_status.status == MMM_PP_ENGINE_FADEOUT)
            || (music_player->music_status.status == MMM_PP_ENGINE_FADEOUT_PAUSE))
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32) &tmp_pp_status);
#ifdef PCM_MMM_DEBUG_INFO
            libc_printf("fadeout:%d\n", tmp_pp_status.fade_out_status);
#endif
            if (tmp_pp_status.fade_out_status == PP_FADE_OUT_STANDBY)
            {
                libc_printf_info("fadeout over\n");

                if (music_player->music_status.status == MMM_PP_ENGINE_FADEOUT_PAUSE)
                {
                    music_player->music_status.status = MMM_PP_ENGINE_PAUSE;
                }
                else
                {
                    music_player->music_status.status = MMM_PP_ENGINE_READY;
#if (SUPPORT_OK_MIC == 1)
                    music_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;
#endif
                    stop_music_decoder_fadeout_wait_cp(music_player);
                    stop_music_decoder_sub(music_player);
                }
            }
        }

        if (ret != 0)
        {
#if (SUPPORT_SOFT_KEYTONE == 1)
            //等待按键音播放完成
            while ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
            {
                libc_printf("wait keytone stop!\n");
                keytone_playend_deal(music_player);
            }
            if (music_player->kt_play_enable == TRUE)
            {
                keytone_stop(music_player);
            }
#endif

            music_player->music_status.status = MMM_PP_ENGINE_ERROR;
            ret = 0;
        }

        loop_continue:

        if ((music_player->music_status.status == MMM_PP_ENGINE_ERROR)
                || (music_player->is_player_loop_enable == FALSE)
                || (music_player->g_pp_pcm_buf_int_en_flag == FALSE)
                || (music_player->music_status.status == MMM_PP_ENGINE_READY)
                || (music_player->music_status.status == MMM_PP_ENGINE_PAUSE)
                || (need_sleep_flag == TRUE))
        {
            sys_os_time_dly(1); //unit 10ms
            need_sleep_flag = FALSE;
        }
        else if (music_player->g_pp_pcm_buf_int_en_flag == TRUE)
        {
            uint32 t1 = 0, t2;

            if (music_player->need_print_time == 1)
            {
                t1 = sys_get_ab_timer_us();
            }

#if (SUPPORT_OK_MIC == 1)
            if ((music_player->work_mode == PP_WORK_MODE_MIC)
                && (music_player->ok_mic_start_flag == OK_MIC_DATA_READ))
            {
                //防止因为断音导致MIC累计延时变长，丢弃数据
                while (music_player->p_micin_buf_r->length > OK_MIC_RAW_BUF_THRESHOLD)
                {
                    libc_printf_warning("drop mic data!!\n");
                    buffer_rw_read(music_player->p_micin_buf_r, (short *) ok_mic_temp_buffer, MY_POST_PROCESS_FRAME_SAMPLE*2);
                }
            }
#endif

            if (libc_sem_wait(music_player->pcm_dec_sem, 20) < 0)
            {
                libc_printf("pcm_dec_sem\n");
                while (1)
                {
                    ; //for QAC
                }
            }

            if (music_player->need_print_time == 1)
            {
                t2 = sys_get_ab_timer_us();
                music_player->release_time += (t2 - t1);
            }
        }

        ret = 0;
    }
    player_loop_exit:
    music_player->thread_terminate_flag = FALSE;

    /*注意：libc_pthread_exit无法返回的*/
    libc_pthread_exit();

    return (void *) NULL;
}

