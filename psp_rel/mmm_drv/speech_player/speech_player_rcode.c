/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ���м�������߳���ѭ������Ҫ����Ԥ����/����/���ŵ������д���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

//0x01��ʾPCM_BUF����жϣ�0x02��ʾPCM_BUFȫ���жϣ�0x04��ʾPCM_BUF�����ж�
void speech_pcm_buf_int_cbk(uint8 pending)
{
    if (speech_player_global->speech_dec_sem != NULL)
    {
        speech_player_global->g_sp_pcm_buf_pending |= pending;
        if ((pending & 0x03) != 0)
        {
            libc_sem_post(speech_player_global->speech_dec_sem);
        }
    }
}

//ADC+DMA�жϴ������������ݵ�Input Buffer�У�0x01��ʾ�����жϣ�0x02��ʾȫ���ж�
void speech_adc_dma_int_cbk(uint8 pending)
{
    speech_player_t *speech_player = speech_player_global;
    uint8 * p_data_adc_read;
    uint16 half_length = MY_PCM_ADC_BLOCK_SIZE / 2;

    if (speech_player->speech_status.status == MMM_SP_ENGINE_ERROR)
    {
        return;
    }

    if (speech_player->speech_status.status == MMM_SP_ENGINE_READY)
    {
        //���㹻 sco ���ݣ���֤���������жϣ�
        if (speech_player->g_speech_pre_enough_flag == TRUE)
        {
            if (speech_player->start_play_flag == 0)
            {
                speech_player->start_play_delay_timer = sys_get_ab_timer();
            }
            speech_player->start_play_flag = 1;
        }

        return;
    }

    if (((pending & DMA_IP_HF) != 0) && ((pending & DMA_IP_TC) != 0))
    {
        libc_printf_warning("mic_in int too late!\n");
        //ȫ˫��AEC
        if ((speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
            && (speech_player->g_p_speech_aec_prms->half_mode == 0))
        {
            speech_player->error_need_reset_flag = TRUE;
        }
        return;
    }

    if ((pending & DMA_IP_HF) != 0)
    {
        p_data_adc_read = (uint8 *) (MIC_ADC_BUF_ADDR);
    }
    else
    {
        p_data_adc_read = (uint8 *) (MIC_ADC_BUF_ADDR + half_length*2);
    }

    //ATS2819��ʹֻ����1·ADC��Ҳ�������������2·���ݣ���1·������ȫ0��
    {
        short *p_dual_to_single = (short *) p_data_adc_read;
        uint16 i;

        for (i = 0; i < half_length/sizeof(short); i++)
        {
            *(p_dual_to_single+i) = *(p_dual_to_single + i*2);
        }
    }

    if (buffer_rw_write(&(speech_player->mic_input_buf), p_data_adc_read, half_length) == FALSE)
    {
        libc_printf_warning("mic_in full!\n");
        //ȫ˫��AEC
        if ((speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
            && (speech_player->g_p_speech_aec_prms->half_mode == 0))
        {
            speech_player->error_need_reset_flag = TRUE;
        }
    }
}

void monitor_aps_adjust(speech_player_t *speech_player)
{
    //sample deal
    speech_player->adjust_sample_counter++;

    if (speech_player->pre_process_buf.length >= speech_player->pre_mean_thres)
    {
        speech_player->adjust_sample_high_counter++;
    }

    if (speech_player->adjust_sample_counter >= MY_ADJUST_SAMPLE_COUNT)
    { //1S����һ��ͳ��
        if (speech_player->adjust_sample_high_counter < MY_ADJUST_SAMPLE_HIGH_COUNT)
        { //����DAC CLOCK
            if (speech_player->audio_aps != speech_player->audio_aps_info.slow_aps)
            {
                set_audio_aps (speech_player->audio_aps_info.slow_aps);
#ifdef SPEECH_MMM_DEBUG_INFO
                libc_printf("AUD_APS_SLOW:%d\n", speech_player->pre_process_buf.length);
#endif
                speech_player->audio_aps = speech_player->audio_aps_info.slow_aps;
                speech_player->adjust_step_cnt = 0;
            }
        }
        else
        { //����DAC CLOCK
            if (speech_player->audio_aps != speech_player->audio_aps_info.fast_aps)
            {
                set_audio_aps (speech_player->audio_aps_info.fast_aps);
#ifdef SPEECH_MMM_DEBUG_INFO
                libc_printf("AUD_APS_FAST:%d\n", speech_player->pre_process_buf.length);
#endif
                speech_player->audio_aps = speech_player->audio_aps_info.fast_aps;
                speech_player->adjust_step_cnt = 0;
            }
        }

        speech_player->adjust_sample_high_counter = 0;
        speech_player->adjust_sample_counter = 0;
    }
}

//ʹ��ͬһƬ�ڴ����ת�������������ݷ��ں�뻺����
void single_to_dual_memcpy(short *dest, int sample_cnt)
{
    short *src = dest + sample_cnt;
    short temp_point;
    int32 temp_point_weak;
    int index;

    for (index = 0; index < sample_cnt; index++)
    {
        temp_point_weak = temp_point = *(src + index);
        *(dest + index * 2) = temp_point;
        *(dest + index * 2 + 1) = temp_point;
        if (speech_player_global->speech_setting.default_channels == SPEECH_CHAN_RIGHT_LEFT)
        {
            if (speech_player_global->speech_setting.right_weak != 0)
            {
                 temp_point_weak = (temp_point_weak*speech_player_global->weak_result)>>15;
                 *(dest + index * 2 + 1) = (int16) temp_point_weak;
            }
            else if (speech_player_global->speech_setting.left_weak != 0)
            {
                 temp_point_weak = (temp_point_weak*speech_player_global->weak_result)>>15;
                 *(dest + index * 2) = (int16) temp_point_weak;
            }
        }
        else if (speech_player_global->speech_setting.default_channels == SPEECH_CHAN_ONLY_LEFT)
        {
            *(dest + index * 2 + 1) = 0;
        }
        else
        {
            *(dest + index * 2) = 0;
        }
    }
}

void speech_playback(speech_player_t *speech_player)
{
    uint32 t1 = 0, t2;
    short *speech_playback_buf = (short *) SPEECH_TEMP_BUF_ADDR;
    uint8 *buf_single_2nd_half;
    uint16 pcmbuf_fill_block_bytes = speech_player->pcmbuf_fill_block;
    int32 ret;

    if (speech_player->pre_process_buf.length < pcmbuf_fill_block_bytes)
    {
        //fill empty 128 samples
        speech_pre_process_frame(speech_player, TRUE);

        speech_player->pre_less_count++;
        if ((speech_player->pre_less_count == 1) || ((speech_player->pre_less_count % 50) == 0))
        {
            libc_printf("pre less!%d\n", speech_player->pre_less_count);
        }
    }
    else
    {
        if ((speech_player->pre_less_count % 50) >= 10)
        {
            libc_printf("pre less!%d\n", speech_player->pre_less_count);
        }
        speech_player->pre_less_count = 0;
    }

    buf_single_2nd_half = (uint8 *) speech_playback_buf + pcmbuf_fill_block_bytes;
    if (buffer_rw_read(&(speech_player->pre_process_buf), buf_single_2nd_half, pcmbuf_fill_block_bytes) == FALSE)
    {
        libc_printf_warning("empty!\n");
        libc_memset(buf_single_2nd_half, 0x00, pcmbuf_fill_block_bytes);
    }

    //�������ز�����������ز�������
#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (speech_player->resample_info.enable_resample == TRUE)
    {
        if (speech_player->need_print_time == 1)
        {
            t1 = sys_get_ab_timer_us();
        }

        resample_and_send_pcm_to_pcm_buf(speech_player, (short*)buf_single_2nd_half, pcmbuf_fill_block_bytes/sizeof(short));

        if (speech_player->need_print_time == 1)
        {
            t2 = sys_get_ab_timer_us();

            speech_player->playback_time += (t2 - t1);

            speech_player->play_samples_add += pcmbuf_fill_block_bytes/sizeof(short);
        }
    }
    else
#endif
    {
        //ֱ�Ӹ��Ƴ�˫�������ݲ����ͣ��� buf_single_2nd_half ��չ�� speech_playback_buf
        single_to_dual_memcpy(speech_playback_buf, pcmbuf_fill_block_bytes/sizeof(short));
        pcmbuf_fill_block_bytes *= 2; //������->�����������ȷ���

#if (SUPPORT_OK_MIC == 1)
        if (speech_player->work_mode == SP_WORK_MODE_MIC)
        {
            //libc_printf("mic:%d,%d=%d\n", speech_player->mic_input_buf.length, get_pcmbuf_length(),\
            //    (speech_player->mic_input_buf.length + get_pcmbuf_length())*5/speech_player->sample_rate);

            if (pcmbuf_fill_block_bytes/4 != MY_POST_PROCESS_FRAME_SAMPLE)
            {
                libc_printf_error("ok mic block size error!\n");
                while (1);
            }

            ok_mic_data_read_again:
            if (speech_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH)
            {
                if (speech_player->speech_status.status == MMM_SP_ENGINE_PLAYING)
                {
                    speech_player->ok_mic_start_flag = OK_MIC_DATA_BUFFER;
                    goto ok_mic_data_read_again;
                }
                //���Buffer
                buffer_rw_flush(&(speech_player->mic_input_buf));
                libc_memset(OK_MIC_TEMP_BUF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
            }
            else if (speech_player->ok_mic_start_flag == OK_MIC_DATA_BUFFER)
            {
                if (speech_player->mic_input_buf.length >= OK_MIC_RAW_BUF_THRESHOLD)
                {
                    speech_player->ok_mic_start_flag = OK_MIC_DATA_READ;
                    goto ok_mic_data_read_again;
                }
                libc_memset(OK_MIC_TEMP_BUF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
            }
            else
            {
                if (buffer_rw_read(&(speech_player->mic_input_buf), (short *) OK_MIC_TEMP_BUF, MY_POST_PROCESS_FRAME_SAMPLE*2) == FALSE)
                {
                    libc_memset(OK_MIC_TEMP_BUF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
                    libc_printf_warning("mic empty!\n");
                }
            }

            if (speech_player->ok_mic_mute_flag == TRUE)
            {
                libc_memset(OK_MIC_TEMP_BUF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
            }

            speech_player->sp_aout.pcm[0] = speech_playback_buf;
            speech_player->sp_aout.pcm[1] = OK_MIC_TEMP_BUF;
            speech_player->sp_aout.samples = MY_POST_PROCESS_FRAME_SAMPLE;

            if (speech_player->need_print_time == 1)
            {
                t1 = sys_get_ab_timer_us();
            }

            ret = post_process_ops(speech_player->audiopp_handle, PP_CMD_FRAME_PROCESS, (uint32) &speech_player->sp_aout);
            if (ret != 0)
            {
                libc_printf_warning("post_process_ops frame error:%d\n", ret);
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
            if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta == MMM_RECORD_RECORDING))
            {
                player_encode_source_deal(speech_player, speech_playback_buf, NULL, speech_player->sp_aout.samples);
            }
#endif

            //SCOӲ����
            {
                short *temp_sco_output_buf = (short *)OK_MIC_TEMP_BUF;
                uint8 i;
                for (i = 0; i < MY_POST_PROCESS_FRAME_SAMPLE; i++)
                {
                    temp_sco_output_buf[i] = speech_playback_buf[i*2];
                }

                if (buffer_rw_write(speech_player->sco_output_buf, temp_sco_output_buf, MY_POST_PROCESS_FRAME_SAMPLE*2) == FALSE)
                {
                    if (speech_player->print_sco_out_full_flag == FALSE)
                    {
                        libc_printf_warning("sco_out full!\n");
                        speech_player->print_sco_out_full_flag = TRUE;
                    }
                }
                else
                {
                    speech_player->print_sco_out_full_flag = FALSE;
                }
            }

            if (speech_player->need_print_time == 1)
            {
                t2 = sys_get_ab_timer_us();
                speech_player->pre_peq_time += (t2 - t1);
            }
        }
#endif

        if (speech_player->need_print_time == 1)
        {
            t1 = sys_get_ab_timer_us();
        }

        send_pcm_to_pcm_buf_by_dac_fifo(speech_playback_buf, pcmbuf_fill_block_bytes/sizeof(short));

        if (speech_player->need_print_time == 1)
        {
            t2 = sys_get_ab_timer_us();

            speech_player->playback_time += (t2 - t1);

            speech_player->play_samples_add += pcmbuf_fill_block_bytes/sizeof(short)/2;
        }
    }

    //ASQT ���ߵ���ģʽ������SCO IN
    if ((speech_player->p_asqt_dump_buf != NULL) && (speech_player->p_asqt_dump_buf->sco_in_flag == TRUE))
    {
        if ((speech_player->pre_process_buf.length < speech_player->pre_mean_thres) && (speech_player->sco_input_buf->length == 0))
        {
            if (buffer_rw_read(speech_player->sco_input_buf_asqt, speech_player->soc_input_buf_asqt_tempbuf,
                    speech_player->plc_deal_block*2) == FALSE)
            {
                libc_printf_warning("sco_in asqt empty!\n");
            }
            else
            {
                if (write_raw_data_to_sco_in(speech_player->sco_input_buf, speech_player->soc_input_buf_asqt_tempbuf,
                    speech_player->plc_deal_block*2) == FALSE)
                {
                    libc_printf_warning("sco_in full!\n");
                }
            }
        }
    }
}

void *player_loop(speech_player_t *speech_player)
{
    int32 ret = 0;
    uint32 tmp_fill_timer, tmp_playback_timer;
    bool need_sleep_flag = FALSE;
    bool sem_wait_flag = TRUE;
    bool need_playback_after = FALSE;
    uint32 start_print_time_timer = 0;
    bool need_start_print_time = TRUE;

    tmp_fill_timer = sys_get_ab_timer();

    for (;;)
    {
        /* �ж�������Ҫ����߳����������Ĺ��ܱ�־λ */
        if (speech_player->thread_terminate_flag == TRUE)
        {
            goto player_loop_exit;
        }

        if ((speech_player->speech_status.status == MMM_SP_ENGINE_ERROR)
                || (speech_player->is_player_loop_enable == FALSE))
        {
            goto loop_continue;
        }

        if ((need_start_print_time == TRUE) && (start_print_time_timer > 0))
        {
            if ((sys_get_ab_timer() - start_print_time_timer) > 10*1000) //��ʼ���ź�10S��ʼͳ��
            {
                libc_printf("start moniter\n");
                need_start_print_time = FALSE;
                speech_player->need_print_time = 1;
                sys_performance_monitoring_restart();
            }
        }

        if (speech_player->speech_status.status == MMM_SP_ENGINE_READY)
        {
#if (SUPPORT_OK_MIC == 1)
            if ((speech_player->work_mode == SP_WORK_MODE_MIC)
                && (speech_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH))
            {
                //���Buffer
                buffer_rw_flush(&(speech_player->mic_input_buf));
            }
#endif

            if (speech_player->g_speech_pre_enough_flag == FALSE)
            {
                bool has_enough_data = FALSE;

                if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
                {
                    if (speech_player->sco_input_buf->length >= speech_player->codec_deal_block)
                    {
                        has_enough_data = TRUE;
                    }
                }
                else
                {
                    //ASQT ���ߵ���ģʽ������SCO IN
                    if ((speech_player->p_asqt_dump_buf != NULL) && (speech_player->p_asqt_dump_buf->sco_in_flag == TRUE))
                    {
                        if ((speech_player->pre_process_buf.length < speech_player->pre_mean_thres) && (speech_player->sco_input_buf->length == 0))
                        {
                            if (buffer_rw_read(speech_player->sco_input_buf_asqt, speech_player->soc_input_buf_asqt_tempbuf,
                                    speech_player->plc_deal_block*2) == TRUE)
                            {
                                if (write_raw_data_to_sco_in(speech_player->sco_input_buf, speech_player->soc_input_buf_asqt_tempbuf,
                                    speech_player->plc_deal_block*2) == FALSE)
                                {
                                    libc_printf_warning("sco_in full!\n");
                                }
                            }
                        }
                    }

                    if (speech_player->sco_input_buf->length >= speech_player->plc_deal_block)
                    {
                        has_enough_data = TRUE;
                    }
                }

                //��ֹ��READY״̬����ȥ
                if (speech_player->p_asqt_dump_buf == NULL)
                {
                    if ((sys_get_ab_timer() - speech_player->ready_start_timer) > READY_MAX_TIMER)
                    {
                        libc_printf_warning("ready status overtime!\n");
                        speech_player->g_speech_pre_enough_flag = TRUE;
                    }
                }

                if (has_enough_data == TRUE)
                {
                    speech_pre_process_frame(speech_player, FALSE);
                    if (speech_player->pre_process_buf.length >= speech_player->pre_high_thres)
                    {
                        speech_player->g_speech_pre_enough_flag = TRUE;
                    }
                }
            }

            if (speech_player->g_speech_pre_enough_flag == TRUE)
            {
                uint32 tmp_wait_start_timer;

                if (need_start_print_time == TRUE)
                {
                    start_print_time_timer = sys_get_ab_timer();
                }

                tmp_wait_start_timer = sys_get_ab_timer();
                while (speech_player->start_play_flag == 0)
                {
                    if ((sys_get_ab_timer() - tmp_wait_start_timer) > 1000)
                    {
                        libc_printf_error("speech wait start play fail!!\n");
                        break;
                    }
                }

                while ((sys_get_ab_timer() - speech_player->start_play_delay_timer) < 6)
                {
                    ; //��MIC��SPEAKER��8ms����
                }

                goto playback_loop;
            }
        }

        if (speech_player->speech_status.status == MMM_SP_ENGINE_PLAYING)
        {
            bool has_enough_data = FALSE;

            sem_wait_flag = TRUE;

            if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
            {
                if (speech_player->sco_input_buf->length >= speech_player->codec_deal_block)
                {
                    has_enough_data = TRUE;
                }
            }
            else
            {
                if (speech_player->sco_input_buf->length >= speech_player->plc_deal_block)
                {
                    has_enough_data = TRUE;
                }
            }

            if (has_enough_data == TRUE)
            {
                speech_pre_process_frame(speech_player, FALSE);
            }

            monitor_aps_adjust(speech_player);

#if (SUPPORT_OK_MIC == 1)
            if (speech_player->work_mode == SP_WORK_MODE_MIC)
            {
                //OK MIC����Ҫ����SCO����
            }
            else
#endif
            {
                if (speech_player->post_process_buf.length > speech_player->post_process_buf.capacity)
                {
                    libc_printf_warning("SPK ERR:%d,%d\n", speech_player->post_process_buf.length,
                            speech_player->pre_process_buf.length);

                    speech_player->error_need_reset_flag = TRUE;
                }
            }

            if (speech_player->error_need_reset_flag == TRUE)
            {
                speech_player->error_need_reset_flag = FALSE;
                //AECʧЧ��ָ����̣���STOP����START
                speech_player->speech_status.status = MMM_SP_ENGINE_ERROR;
                speech_player->g_speech_reset_buffer_flag = TRUE;
                goto loop_continue;
            }

#if (SUPPORT_OK_MIC == 1)
            if (speech_player->work_mode == SP_WORK_MODE_MIC)
            {
                //OK MIC����Ҫ����SCO����
            }
            else
#endif
            {
                if ((speech_player->mic_input_buf.length >= speech_player->aec_deal_block)
                        && (speech_player->post_process_buf.length >= speech_player->aec_deal_block))
                {
                    //�����PCM BUFFER������PRE PRECESS + POST PROCESS����ʱ��̫�����¶���
                    tmp_playback_timer = sys_get_ab_timer();

                    //�طŴ���
                    while (1)
                    {
#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
                        if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta == MMM_RECORD_RECORDING))
                        {
                            //��һ֡�������ݣ���ʼ���롣��0|1������Ƭ�������������ݣ�������0�Ż�������ʱ������д��1����������ʾ0������
                            ret = player_encode_frame_deal(speech_player);
                        }
#endif

                        speech_playback(speech_player);

                        if (((speech_player->g_sp_pcm_buf_pending & 0x04) != 0)
                                || (get_pcmbuf_length() >= speech_player->g_half_full_thres))
                        {
                            if ((speech_player->g_sp_pcm_buf_pending & 0x04) == 0)
                            {
#ifdef SPEECH_MMM_DEBUG_INFO
                                libc_printf_warning("pcm_buf length unexpect!\n");
#endif
                                speech_player->pcm_buf_hd_err = TRUE;
                            }

                            if (speech_player->pcm_buf_hd_err == TRUE)
                            {
                                uint32 hd_err_timer = sys_get_ab_timer();

                                while (1)
                                {
                                    if (get_pcmbuf_length() < speech_player->g_half_empty_thres)
                                    {
#ifdef SPEECH_MMM_DEBUG_INFO
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

                                speech_player->pcm_buf_hd_err = FALSE;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if ((sys_get_ab_timer() - tmp_playback_timer) >= 1000)
                        {
                            need_sleep_flag = TRUE;
                            libc_printf_warning("playback overtime!\n");
                            break;
                        }
                    }

                    speech_post_process_frame(speech_player);
                    sem_wait_flag = FALSE;
                }
            }

            if (speech_player->need_print_time == 1)
            {
                if ((speech_player->play_samples_add / speech_player->sample_rate) > 5000)
                {
                    sys_performance_monitoring_report();
                    sys_performance_monitoring_stop();

                    speech_player->need_print_time = 0;
                    libc_printf_info("speech al cost:pre=%d,post=%d,plc=%d,preagc=%d,prepeq=%d\n",
                            (speech_player->pre_deal_time / 1000),
                            (speech_player->post_deal_time / 1000),
                            (speech_player->plc_deal_time / 1000),
                            (speech_player->pre_agc_deal_time / 1000),
                            (speech_player->pre_peq_time / 1000));
                    libc_printf_info("postagc=%d,aec=%d,playback=%d,postpeq=%d,rs=%d,enc=%d,release=%d\n",
                            (speech_player->post_agc_deal_time / 1000),
                            (speech_player->aec_deal_time / 1000),
                            (speech_player->playback_time / 1000),
                            (speech_player->post_peq_time / 1000),
                            (speech_player->resample_time / 1000),
                            (speech_player->enc_play_time / 1000),
                            (speech_player->release_time / 1000));
                }
            }

            need_playback_after = TRUE;
            if ((speech_player->g_speech_pcm_buf_int_en_flag == TRUE) && (speech_player->g_sp_pcm_buf_pending == 0))
            {
                need_playback_after = FALSE;
            }

            if ((speech_player->g_sp_pcm_buf_pending & 0x04) != 0)
            {
                need_playback_after = FALSE;

                if ((speech_player->g_sp_pcm_buf_pending & 0x02) != 0)
                {
                    libc_printf_warning("dec too late!!\n");
                    //ȫ˫��AEC
                    if ((speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
                        && (speech_player->g_p_speech_aec_prms->half_mode == 0))
                    {
                        speech_player->error_need_reset_flag = TRUE;
                    }

                    //����̫��������̫�ã���������һ��PCM BUFFER ����ж��¼�û�д���
                    while (libc_sem_trywait(speech_player->speech_dec_sem) == 0)
                    {
                        ; //����ź���
                    }

                    speech_player->g_sp_pcm_buf_pending = 0;
                    need_playback_after = TRUE;
                }

                if ((speech_player->g_sp_pcm_buf_pending & 0x01) != 0)
                {
                    //����̫��������̫�ã���������һ��PCM BUFFER ����ж��¼�û�д���
                    while (libc_sem_trywait(speech_player->speech_dec_sem) == 0)
                    {
                        ; //����ź���
                    }

                    speech_player->g_sp_pcm_buf_pending = 0;
                    need_playback_after = TRUE;
                }
            }

            if (need_playback_after == TRUE)
            {
                playback_loop:

                tmp_playback_timer = sys_get_ab_timer();

                //�طŴ���
                while (1)
                {
#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
                    if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta == MMM_RECORD_RECORDING))
                    {
                        //��һ֡�������ݣ���ʼ���롣��0|1������Ƭ�������������ݣ�������0�Ż�������ʱ������д��1����������ʾ0������
                        ret = player_encode_frame_deal(speech_player);
                    }
#endif

                    speech_playback(speech_player);

                    if (speech_player->g_speech_pcm_buf_int_en_flag == FALSE)
                    {
#if (SUPPORT_OK_MIC == 1)
                        if (speech_player->work_mode == SP_WORK_MODE_MIC)
                        {
                            speech_player->g_half_empty_thres = OK_MIC_PCM_BUF_HE_THRESHOLD;
                        }
                        else
#endif
                        {
#if (SUPPORT_AUDIO_RESAMPLE == 1)
                            if (speech_player->resample_info.enable_resample == TRUE)
                            {
                                speech_player->g_half_empty_thres = 2*SPEECH_PLAY_THRES_MS*(speech_player->resample_info.convert_output_rate_hz/1000);//˫����
                            }
                            else
#endif
                            {
                                speech_player->g_half_empty_thres = 2*SPEECH_PLAY_THRES_MS*speech_player->sample_rate;//˫����
                            }
                        }

                        speech_player->g_half_full_thres = speech_player->g_half_empty_thres + 8;

                        init_pcm_buf(speech_player->g_half_empty_thres, speech_player->g_half_full_thres);

                        speech_player->g_speech_pcm_buf_int_en_flag = TRUE;

                        if (libc_sem_init(&(speech_player->speech_dec_sem), 0) == -1)
                        {
                            libc_printf("speech_dec_sem\n");
                            while (1)
                            {
                                ; //for QAC
                            }
                        }
                    }

                    if (((speech_player->g_sp_pcm_buf_pending & 0x04) != 0)
                            || (get_pcmbuf_length() >= speech_player->g_half_full_thres))
                    {
                        if ((speech_player->g_sp_pcm_buf_pending & 0x04) == 0)
                        {
#ifdef SPEECH_MMM_DEBUG_INFO
                            libc_printf_warning("pcmbuf length unexpect!\n");
#endif
                            speech_player->pcm_buf_hd_err = TRUE;
                        }

                        speech_player->g_sp_pcm_buf_pending &= ~0x04;

                        if (speech_player->pcm_buf_hd_err == TRUE)
                        {
                            uint32 hd_err_timer = sys_get_ab_timer();

                            while (1)
                            {
                                if (get_pcmbuf_length() < speech_player->g_half_empty_thres)
                                {
#ifdef SPEECH_MMM_DEBUG_INFO
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

                            speech_player->pcm_buf_hd_err = FALSE;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if ((speech_player->g_sp_pcm_buf_pending & 0x02) != 0)
                    {
                        libc_printf_warning("dec too late!!\n");
                        //ȫ˫��AEC
                        if ((speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
                            && (speech_player->g_p_speech_aec_prms->half_mode == 0))
                        {
                            speech_player->error_need_reset_flag = TRUE;
                        }
                    }
                    //����̫��������̫�ã���������һ��PCM BUFFER ����ж��¼�û�д���
                    while (libc_sem_trywait(speech_player->speech_dec_sem) == 0)
                    {
                        ; //����ź���
                    }
                    speech_player->g_sp_pcm_buf_pending = 0;

                    if ((sys_get_ab_timer() - tmp_playback_timer) >= 1000)
                    {
                        need_sleep_flag = TRUE;
                        libc_printf_warning("playback overtime!\n");
                        break;
                    }
                }

                if (speech_player->g_speech_need_start == FALSE)
                {
                    speech_player->speech_status.status = MMM_SP_ENGINE_PLAYING;
                    speech_player->g_speech_need_start = TRUE;

                    libc_printf_info("start!\n");
                }
            }

            if ((need_sleep_flag == FALSE) && (sem_wait_flag == TRUE))
            {
                uint32 t1 = 0, t2;

                if (speech_player->need_print_time == 1)
                {
                    t1 = sys_get_ab_timer_us();
                }

#if (SUPPORT_OK_MIC == 1)
                if ((speech_player->work_mode == SP_WORK_MODE_MIC)
                    && (speech_player->ok_mic_start_flag == OK_MIC_DATA_READ))
                {
                    //��ֹ��Ϊ��������MIC�ۼ���ʱ�䳤����������
                    while (speech_player->mic_input_buf.length > OK_MIC_RAW_BUF_THRESHOLD)
                    {
                        libc_printf_warning("drop mic data!!\n");
                        buffer_rw_read(&(speech_player->mic_input_buf), (short *) OK_MIC_TEMP_BUF, MY_POST_PROCESS_FRAME_SAMPLE*2);
                    }
                }
#endif

                if (libc_sem_wait(speech_player->speech_dec_sem, 20) < 0)
                {
                    libc_printf("speech_dec_sem\n");
                    while (1)
                    {
                        ; //for QAC
                    }
                }

                if (speech_player->need_print_time == 1)
                {
                    t2 = sys_get_ab_timer_us();
                    speech_player->release_time += (t2 - t1);
                }

                tmp_fill_timer = sys_get_ab_timer();
            }
        }

        /*
            fadein ����
        */
        if (speech_player->g_speech_fadein_flag == 1)
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(speech_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32) &tmp_pp_status);
#ifdef SPEECH_MMM_DEBUG_INFO
            libc_printf("fadein:%d\n", tmp_pp_status.fade_in_status);
#endif
            if (tmp_pp_status.fade_in_status == PP_FADE_IN_STANDBY)
            {
                libc_printf_info("fadein over\n");
                speech_player->g_speech_fadein_flag = 0;
            }
        }

        if (ret != 0)
        {
            //AECʧЧ��ָ����̣���STOP����START
            speech_player->speech_status.status = MMM_SP_ENGINE_ERROR;
            speech_player->g_speech_reset_buffer_flag = TRUE;
            ret = 0;
        }

        loop_continue:

        //AECʧЧ��ָ����̣���STOP����START
        if (speech_player->g_speech_reset_buffer_flag == TRUE)
        {
            stop_speech_decoder(speech_player);
            start_speech_decoder(speech_player);

            speech_player->g_speech_reset_buffer_flag = FALSE;
        }

        if ((sys_get_ab_timer() - tmp_fill_timer) >= 1000)
        {
            libc_printf_warning("fill overtime!\n");
            tmp_fill_timer = sys_get_ab_timer();
            need_sleep_flag = TRUE;
        }

        if ((speech_player->speech_status.status == MMM_SP_ENGINE_ERROR)
                || (speech_player->is_player_loop_enable == FALSE)
                || (speech_player->start_play_flag == 0)
                || (need_sleep_flag == TRUE))
        {
            sys_os_time_dly(1); //unit 10ms
            need_sleep_flag = FALSE;
        }

        ret = 0;
    }

    player_loop_exit:
    speech_player->thread_terminate_flag = FALSE;

    /*ע�⣺libc_pthread_exit�޷����ص�*/
    libc_pthread_exit();

    return (void *) NULL;
}


