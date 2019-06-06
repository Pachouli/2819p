/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м�������߳���ѭ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"

//0x01��ʾPCM_BUF����жϣ�0x02��ʾPCM_BUFȫ���жϣ�0x04��ʾPCM_BUF�����ж�
void bt_pcm_buf_int_cbk(uint8 pending)
{
    if (music_player_global->bt_dec_sem != NULL)
    {
        music_player_global->g_bp_pcm_buf_pending |= pending;
        if ((pending & 0x03) != 0)
        {
            libc_sem_post(music_player_global->bt_dec_sem);
        }
    }
}

int32 player_decode_daedeal(uint8 *output_buf, uint16 output_len)
{
    music_bt_player_t *music_player = music_player_global;
    music_frame_info_t sbc_frame_info;
    uint8 *sbc_dec_temp_buf = (uint8 *) SBC_DEC_TEMP_BUFFER_ADDR; //������ʱ�ռ�
    uint8 *mic_temp_buffer = (uint8 *) SBC_DEC_TEMP_BUFFER_ADDR; //PEQ��ʱ�ռ䣬���Ը���
    int32 ret = 0;
    int32 dec_frame_length = 0;
    bool fill_empty_flag = FALSE;
    uint16 read_data_len;
    bool try_again_flag;
    uint32 t1 = 0, t2;
    uint32 temp_flag;
    bool need_decode_flag, need_play_mute;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if ((p_tws_info->dev_role == TWS_MASTER)
        && (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN))
    {
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_ENERGY, (uint32)(&(music_player->music_status.adc_energy)));
    }
#endif

    decode_try_again:
    try_again_flag = FALSE;

    need_decode_flag = FALSE;
    need_play_mute = FALSE;
    if ((music_player->decode_pp_frame_total == 0)
        || (music_player->decode_pp_frame_index >= music_player->decode_pp_frame_total))
    {
        need_decode_flag = TRUE;
    }

    if (music_player->music_status.status == MMM_BP_ENGINE_READY)
    {
        need_decode_flag = FALSE;
        need_play_mute = TRUE;
    }

    if (need_decode_flag == TRUE)
    {
        //����1֡
        if ((music_player->frame_count > 0) && (music_player->input_buf->length > 0))
        {
            read_data_len = read_bt_input_buf(music_player->input_buf, sbc_dec_temp_buf, SBC_DEC_TEMP_INPUT_LEN);
            if (read_data_len > 0)
            {
                sbc_frame_info.input = sbc_dec_temp_buf;
                sbc_frame_info.input_len = read_data_len;
                sbc_frame_info.output = output_buf;

                if (music_player->need_print_time == TRUE)
                {
                    t1 = sys_get_ab_timer_us();
                }

                ret = sbc_decoder_cmd(music_player->audiodec_handle, AD_CMD_FRAME_DECODE, (uint32) &sbc_frame_info);
                if (ret != 0)
                {
                    libc_printf_error("decode error!%d\n", ret);
    #ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                    if (p_tws_info->dev_role != NORMAL_DEV)
                    {
                        //TWS��������ֽ������һ����������˳��������������ֳ������¿�ʼ���䲥��
                        return ret;
                    }
                    else
    #endif
                    {
                        switch (ret)
                        {
                            case BITSTREAM_TOO_SHORT: //Data stream too short
                            case SYNC_BYTE_INCORRECT: //Sync byte incorrect
                            case CRC8_INCORRECT: //CRC8 incorrect
                            case BITPOOL_VALUE_INCORRECT: //Bitpool value out of bounds
                            case SUBBAND_VALUE_INCORRECT: //subbands value error
                            case SYNC_SEARCH_ERROR: //can not syn
                            case SBCDEC_LEN_ERROR: //frame len is zero
                            try_again_flag = TRUE;
                            break;

                            case POINTER_VALUE_INCORRECT: //decode-strcut pointer is NULL
                            default:
                            while (1)
                            {
                                ; //�����ܳ����������������ʱ��Ҫ���ֳ�
                            }
                            break;
                        }
                    }
                }
                else
                {
    #ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                    if (p_tws_info->dev_role != NORMAL_DEV)
                    {
                        uint16 frame_checksum;
                        frame_checksum = tws_frame_check_sum_deal(sbc_dec_temp_buf, sbc_frame_info.frame_len);
                    }
    #endif
                }

                if (try_again_flag == FALSE)
                {
                    if (music_player->need_print_time == TRUE)
                    {
                        t2 = sys_get_ab_timer_us();

                        music_player->dec_play_time += (t2 - t1);

                        music_player->play_samples_add += POST_PROCESS_FRAME_SAMPLE; //SBC֡�̶�Ϊ128������
                    }

                    dec_frame_length = sbc_frame_info.decode_len;
                    if (dec_frame_length < 0)
                    {
                        libc_printf("decode length:%d\n", dec_frame_length);
                        try_again_flag = TRUE;
                    }
                    else
                    {
                        music_player->g_sbc_frame_size = sbc_frame_info.frame_len;
                        if (music_player->g_sbc_frame_size > SBC_MAX_FRAME_LEN)
                        {
                            music_player->g_sbc_frame_size = SBC_MAX_FRAME_LEN;
                        }
                    }
                }

                if (try_again_flag == TRUE)
                {
                    update_bt_input_buf(music_player->input_buf, music_player->input_buf->length);
                    music_player->frame_count = 0;
                    goto decode_try_again;
                }
            }
        }
        else
        {
            libc_printf_warning("empty!\n");

    #ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
            if (p_tws_info->dev_role != NORMAL_DEV)
            {
                if ((music_player->music_status.status != MMM_BP_ENGINE_FADEOUT)
                    && (music_player->music_status.status != MMM_BP_ENGINE_FADEOUT_PAUSE))
                {
                    //TWS��������ֶ�������һ����������˳��������������ֳ������¿�ʼ���䲥��
                    return -1;
                }
            }
    #endif

            libc_memset(output_buf, 0x00, 512);
            music_player->frame_count = 0;
            fill_empty_flag = TRUE;
        }

        music_player->decode_pp_frame_total = POST_PROCESS_FRAME_SAMPLE/MY_POST_PROCESS_FRAME_SAMPLE;
        music_player->decode_pp_frame_index = 0;

        //����/������֡��⣬�������ھ���/������ʱ�ⲿ�����������ⲿ���ŵ����Ӱ��
        energy_calc((short *) output_buf, POST_PROCESS_FRAME_SAMPLE, TRUE,
                &(music_player->music_status.bt_cur_energy), &(music_player->music_status.bt_cur_energy_rms));

        //������⣬����Ƿ񵽸���β��
        if (music_player->music_status.bt_cur_energy_rms <= 3)
        {
            music_player->bt_zero_frame++;
        }
        else
        {
            music_player->bt_zero_frame = 0;
        }

        //���������
        if ((music_player->music_setting.bt_lowpw_th_max != 0) || (music_player->music_setting.bt_lowpw_th_rms != 0))
        {
            bool max_match_flag = TRUE;
            bool rms_match_flag = TRUE;

            if (music_player->music_setting.bt_lowpw_th_max != 0)
            {
                if (music_player->music_status.bt_cur_energy > music_player->music_setting.bt_lowpw_th_max)
                {
                    max_match_flag = FALSE;
                }
            }

            if (music_player->music_setting.bt_lowpw_th_rms != 0)
            {
                if (music_player->music_status.bt_cur_energy_rms > music_player->music_setting.bt_lowpw_th_rms)
                {
                    rms_match_flag = FALSE;
                }
            }

            if ((max_match_flag == TRUE) && (rms_match_flag == TRUE))
            {
                music_player->bt_lowpw_frame++;
            }
            else
            {
                music_player->bt_lowpw_frame = 0;
            }

            music_player->music_status.bt_lowpw_time = music_player->bt_lowpw_frame
                * POST_PROCESS_FRAME_SAMPLE / music_player->music_setting.default_samplerate; //������������㣬������֡128������������
        }
    }

    if (music_player->need_print_time == TRUE)
    {
        t1 = sys_get_ab_timer_us();
    }

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == BP_WORK_MODE_MIC)
    {
        //libc_printf("mic:%d,%d=%d\n", music_player->p_mic_input_buf_r->length, get_pcmbuf_length(),\
        //    (music_player->p_mic_input_buf_r->length + get_pcmbuf_length())*5/music_player->music_setting.default_samplerate);

        ok_mic_data_read_again:
        if (music_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH)
        {
            if (music_player->music_status.status == MMM_BP_ENGINE_PLAYING)
            {
                music_player->ok_mic_start_flag = OK_MIC_DATA_BUFFER;
                goto ok_mic_data_read_again;
            }
            //���Buffer
            buffer_rw_flush(music_player->p_mic_input_buf_r);
            libc_memset(mic_temp_buffer, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
        }
        else if (music_player->ok_mic_start_flag == OK_MIC_DATA_BUFFER)
        {
            if (music_player->p_mic_input_buf_r->length >= OK_MIC_RAW_BUF_THRESHOLD)
            {
                music_player->ok_mic_start_flag = OK_MIC_DATA_READ;
                goto ok_mic_data_read_again;
            }
            libc_memset(mic_temp_buffer, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
        }
        else
        {
            if (buffer_rw_read(music_player->p_mic_input_buf_r, (short *) mic_temp_buffer, MY_POST_PROCESS_FRAME_SAMPLE*2) == FALSE)
            {
                libc_memset(mic_temp_buffer, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
                libc_printf_warning("mic empty!\n");
            }
        }
        music_player->bp_aout.pcm[1] = mic_temp_buffer;
        if (music_player->ok_mic_mute_flag == TRUE)
        {
            libc_memset(mic_temp_buffer, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
        }

        energy_calc(mic_temp_buffer, MY_POST_PROCESS_FRAME_SAMPLE, FALSE,
                &(music_player->music_status.mic_cur_energy), &(music_player->music_status.mic_cur_energy_rms));
    }
#endif

    music_player->bp_aout.pcm[0] = (output_buf + MY_POST_PROCESS_FRAME_SAMPLE*4*music_player->decode_pp_frame_index);
    if (need_play_mute == TRUE)
    {
        libc_memset(output_buf, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*4);
        music_player->bp_aout.pcm[0] = output_buf;
    }
    music_player->bp_aout.samples = MY_POST_PROCESS_FRAME_SAMPLE;

    ret = post_process_ops(music_player->audiopp_handle, PP_CMD_FRAME_PROCESS, (uint32) &music_player->bp_aout);
    if (ret != 0)
    {
        libc_printf_error("post_process_ops frame error:%d\n", ret);
    }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1))
    //����DAE����Ƶ������д�ļ�����
    if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta == MMM_RECORD_RECORDING))
    {
        player_encode_source_deal(music_player, music_player->bp_aout.pcm[0], music_player->bp_aout.samples);
    }
#endif

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == BP_WORK_MODE_MIC)
    {
        //ԭ������ֻ�ܵ��������
        if (g_PP_para_info.voice_removal_para.voice_removal_enable == 1)
        {
            int i;
            short *pcm_result = music_player->bp_aout.pcm[0];

            for (i = 0; i < music_player->bp_aout.samples; i++)
            {
                pcm_result[2*i + 1] = pcm_result[2*i];
            }
        }
    }
#endif

    //��ȡ��Ƶ������ֵ
    if (g_freq_point_cfg.freq_point_enable == TRUE)
    {
        if (post_process_ops(music_player->audiopp_handle, PP_CMD_GET_FREQ_POINT_ENERGY,
            (uint32) music_player->music_status.freq_point_energy) < 0)
        {
            libc_memset(music_player->music_status.freq_point_energy, 0x00, sizeof(music_player->music_status.freq_point_energy));
        }
    }

    if (music_player->decode_pp_frame_index < music_player->decode_pp_frame_total)
    {
        music_player->decode_pp_frame_index++;
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    if ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
    {
        keytone_frame_deal(music_player, music_player->bp_aout.pcm[0], MY_POST_PROCESS_FRAME_SAMPLE);
    }
#endif

    if (music_player->need_print_time == TRUE)
    {
        t2 = sys_get_ab_timer_us();

        music_player->dae_play_time += (t2 - t1);
    }

    //��λ���������ݵ�PCM_BUF��־
    music_player->is_sending_to_pcm_buf_flag = TRUE;

    send_pcm_to_pcm_buf_by_dac_fifo(music_player->bp_aout.pcm[0], music_player->bp_aout.samples * 2, TRUE);

    //ͳ��ʵ�ʲ����ٶ�
    temp_flag = sys_local_irq_save();
    //������������ݵ�PCM_BUF��־
    music_player->is_sending_to_pcm_buf_flag = FALSE;
    music_player->send_to_pcm_buf_count += music_player->bp_aout.samples;
    sys_local_irq_restore(temp_flag);

    if ((need_decode_flag == TRUE) && (fill_empty_flag == FALSE))
    {
        update_bt_input_buf(music_player->input_buf, dec_frame_length);

        if (music_player->frame_count > 0)
        {
            music_player->frame_count--;
        }
    }

    return 0;
}

void *player_loop(music_bt_player_t *music_player)
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

        //�������ɻָ��Ĵ��󣻻�����Ҫ�˳������߳�
        if ((music_player->music_status.status == MMM_BP_ENGINE_ERROR)
                || (music_player->is_player_loop_enable == FALSE))
        {
            goto loop_continue;
        }

        if ((need_start_print_time == TRUE) && (start_print_time_timer > 0))
        {
            if ((sys_get_ab_timer() - start_print_time_timer) > 10*1000) //��ʼ���ź�10S��ʼͳ��
            {
                libc_printf("start moniter\n");
                need_start_print_time = FALSE;
                music_player->need_print_time = TRUE;
                sys_performance_monitoring_restart();
            }
        }

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
        if (p_tws_info->dev_role == TWS_MASTER)
        {
            if (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN)
            {
                //SBC���룬���͵�SBC Input Buffer
                ret = sbc_encoder_frame_deal(music_player);
                if (ret < 0)
                {
                    libc_printf_error("sbc_encoder_frame_deal error!\n");
                    goto dec_err_deal;
                }
            }
        }
#endif

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
        if (p_tws_info->dev_role != NORMAL_DEV)
        {
            if (p_tws_info->tws_link_connect_flag == FALSE)
            {
                libc_printf_warning("tws link disconnect!\n");
            }

            if (p_tws_info->tws_slave_frame_lost_flag == TRUE)
            {
                if ((sys_get_ab_timer() - p_tws_info->tws_slave_frame_lost_timer) >= 1000)
                {
                    music_player->music_status.status = MMM_BP_ENGINE_ERROR;
                    goto loop_continue;
                }
            }

            //�Ѿ�����AVDTP���ݰ���ʧ����Ϊ����״̬���˳����䲥��
            if (p_tws_info->tws_has_avdtp_pkg_lost() != FALSE)
            {
                //ֻ��SLAVE��������Ҫ�˳�TWS���裬MASTER�������˳�TWS����Ч������
                if (p_tws_info->dev_role == TWS_SLAVE)
                {
                    libc_printf_warning("avdtp pkg lost!!\n");
                    if (p_tws_info->tws_slave_frame_lost_flag == FALSE)
                    {
                        //���䶪�����ȸ������䶪���ˣ������յ���ᱨ���˳����룻���1S��û�б����˳��Ļ���������Լ������˳�����
                        p_tws_info->tws_slave_frame_lost_flag = TRUE;
                        p_tws_info->tws_slave_frame_lost_timer = sys_get_ab_timer();
                    }
                }
                //���䶪��̫��Ҳ�������ش���Ҳ����ǿ���˳�����
                else
                {
                    if (p_tws_info->tws_start_play_timer > 0)
                    {
                        p_tws_info->tws_master_lost_pkg_count++;
                        if ((sys_get_ab_timer() - p_tws_info->tws_start_play_timer) <= 5000)
                        {
                            uint8 play_second = (sys_get_ab_timer() - p_tws_info->tws_start_play_timer)/1000 + 1;

                            switch(play_second)
                            {
                            case 1:
                                if (p_tws_info->tws_master_lost_pkg_count >= 3)
                                {
                                    music_player->music_status.lost_pkg_too_more_flag = TRUE;
                                }
                                break;

                            case 2:
                                if (p_tws_info->tws_master_lost_pkg_count >= 5)
                                {
                                    music_player->music_status.lost_pkg_too_more_flag = TRUE;
                                }
                                break;

                            default:
                                if (p_tws_info->tws_master_lost_pkg_count/play_second >= 2)
                                {
                                    music_player->music_status.lost_pkg_too_more_flag = TRUE;
                                }
                                break;
                            }
                        }
                        else
                        {
                            p_tws_info->tws_start_play_timer = 0;
                        }
                    }
                }
            }

            //TWS��������������¼�����
            if (tws_cmd_and_event_deal(music_player) == TRUE)
            {
                //�յ�������¼���������󷵻�
            }
        }
#endif

        do_time_length:

        if ((music_player->music_status.status == MMM_BP_ENGINE_ERROR)
                || (music_player->music_status.status == MMM_BP_ENGINE_PAUSE))
        {
            goto loop_continue;
        }

        if (music_player->g_sbc_parse_flag == 1)
        {
            calc_buffer_time_length(music_player);

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
            if (p_tws_info->dev_role == TWS_SLAVE)
            {
                tws_update_play_info_s2m(music_player);
                tws_update_ip_frame_info_s2m(music_player);
            }
#endif

            //��ػ�������΢��Ƶ�ʣ���ά�ֻ������ȶ��ں���ˮƽ��
            if (music_player->music_status.status == MMM_BP_ENGINE_PLAYING)
            {
                monitor_aps_adjust(music_player);
            }
        }

        if (music_player->music_status.status == MMM_BP_ENGINE_READY)
        {
            bool need_start_play = FALSE;

#if (SUPPORT_OK_MIC == 1)
            if ((music_player->work_mode == BP_WORK_MODE_MIC)
                && (music_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH))
            {
                //���Buffer
                buffer_rw_flush(music_player->p_mic_input_buf_r);
            }
#endif

            //SBC ��Ҫ�ȷ�����������
            if ((music_player->g_sbc_parse_flag == 0)
                    && (music_player->input_buf->length >= SBC_DEC_TEMP_BUFFER_LEN))
            {
                int32 ret_sr = 0;

                //check sample info
                ret_sr = sbc_check_sample_rate(music_player);
                if (ret_sr != -1)
                {
                    music_player->music_setting.default_samplerate = ret_sr;
                    libc_printf_info("sbc frequency:%d\n", ret_sr);

                    music_player->g_sbc_parse_flag = 1;

                    goto do_time_length;
                }
            }

            if (music_player->g_sbc_parse_flag == 1)
            {
                uint16 tmp_time_length;

                tmp_time_length = music_player->buffer_time_length;

                if ((music_player->g_start_sub_pre_flag == 0) && (tmp_time_length >= MY_START_PLAY_TIME))
                {
                    start_music_decoder_sub_pre(music_player);
                    music_player->g_start_sub_pre_flag = 1;
                }

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                if (p_tws_info->dev_role != NORMAL_DEV)
                {
                    if (music_player->g_start_sub_pre_flag == 1)
                    {
                        need_start_play = p_tws_info->tws_need_start_sync_play;
                        //����ͬ��������������
                        if (need_start_play == TRUE)
                        {
                            p_tws_info->tws_start_play_timer = sys_get_ab_timer();

                            //Controller ��ʱ�������ˣ���Ϊ����״̬���˳����䲥��
                            if (tws_set_and_wait_sync_play(music_player, &(p_tws_info->should_start_con_clock)) == FALSE)
                            {
                                libc_printf_error("tws start sync play fail!\n");
                                music_player->music_status.status = MMM_BP_ENGINE_ERROR;
                                goto do_time_length;
                            }
                        }
                        else
                        {
                            //Input buffer ��������Ϊ����״̬���˳����䲥��
                            if (music_player->input_buf->full_error == TRUE)
                            {
                                music_player->input_buf->full_error = FALSE;
                                libc_printf_error("input buffer full error!\n");
                                music_player->music_status.status = MMM_BP_ENGINE_ERROR;
                                goto do_time_length;
                            }
                        }
                    }
                }
                else
#endif
                {
                    if (tmp_time_length > music_player->music_setting.start_delay_time)
                    {
                        need_start_play = TRUE;
                    }
                    else if (tmp_time_length >= MY_START_PLAY_TIME)
                    {
                        tmp_time_length += (music_player->cur_timestamp - music_player->last_frame_timestamp);

                        if (tmp_time_length >= music_player->music_setting.start_delay_time)
                        {
                            need_start_play = TRUE;
                        }
                    }
                }
            }

            if (need_start_play == TRUE)
            {
                //ǿ�� start_music_decoder_sub_pre �����е�������
                if (music_player->ready_because_fadeout_flag == TRUE)
                {
                    music_player->g_start_sub_pre_flag = 0;
                    music_player->ready_because_fadeout_flag = FALSE;
                }

                if (music_player->g_start_sub_pre_flag == 0) //��ֹ��ǰ�����߼�����û��ִ�е�����ִ��
                {
                    start_music_decoder_sub_pre(music_player);
                    music_player->g_start_sub_pre_flag = 1;
                }

                //ĳЩ��Ҫ�ȵ�������ȷ������ܳ�ʼ���Ĺ���
                start_music_decoder_sub(music_player);
            }
            else
            {
                //OK MIC�����ݲ��㵭������Ҫ����������
#if (SUPPORT_OK_MIC == 1)
                if ((music_player->work_mode == BP_WORK_MODE_MIC)
                    && (music_player->ready_because_fadeout_flag == TRUE)
                    && (music_player->g_start_sub_pre_flag == 1))
                {
                    ret = player_decode_daedeal((uint8 *) SBC_DEC_OUTPUT_BUFFER_ADDR, SBC_DEC_OUTPUT_BLOCK_SIZE);
                    if (ret < 0)
                    {
                        libc_printf_error("player_decode_daedeal error!\n");
                        goto dec_err_deal;
                    }
                }
#endif
                goto loop_continue;
            }
        }

        if (music_player->music_status.status == MMM_BP_ENGINE_ERROR)
        {
            goto loop_continue;
        }

        /*�����������Ч����*/
        tmp_fill_timer = sys_get_ab_timer();

        while (1)
        {
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
            if (p_tws_info->dev_role != NORMAL_DEV)
            {
                //Input buffer ��������Ϊ����״̬���˳����䲥��
                if (music_player->input_buf->full_error == TRUE)
                {
                    music_player->input_buf->full_error = FALSE;
                    libc_printf_error("input buffer full error!\n");
                    music_player->music_status.status = MMM_BP_ENGINE_ERROR;
                    goto loop_continue;
                }
            }
#endif

            //��ػ�����������ʱ��������
            if ((music_player->music_status.status == MMM_BP_ENGINE_PLAYING)
                    || (music_player->music_status.status == MMM_BP_ENGINE_FADEIN))
            {
                //�����ȼ���buffer time length���ܵ���monitor_fadeout_check
                calc_buffer_time_length(music_player);
                monitor_fadeout_check(music_player);
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1))
            if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta == MMM_RECORD_RECORDING))
            {
                //��һ֡�������ݣ���ʼ���롣��0|1������Ƭ�������������ݣ�������0�Ż�������ʱ������д��1����������ʾ0������
                ret = player_encode_frame_deal(music_player);
            }
#endif

            ret = player_decode_daedeal((uint8 *) SBC_DEC_OUTPUT_BUFFER_ADDR, SBC_DEC_OUTPUT_BLOCK_SIZE);
            if (ret < 0)
            {
                libc_printf_error("player_decode_daedeal error!\n");
                goto dec_err_deal;
            }

            if (music_player->g_bp_pcm_buf_int_en_flag == FALSE)
            {
#if (SUPPORT_OK_MIC == 1)
                if (music_player->work_mode == BP_WORK_MODE_MIC)
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
                music_player->g_bp_pcm_buf_int_en_flag = TRUE;

                if (libc_sem_init(&(music_player->bt_dec_sem), 0) == -1)
                {
                    libc_printf("bt_dec_sem\n");
                    while (1)
                    {
                        ; //for QAC
                    }
                }
            }

            if (((music_player->g_bp_pcm_buf_pending & 0x04) != 0)
                    || (get_pcmbuf_length() >= music_player->g_half_full_thres))
            {
                if ((music_player->g_bp_pcm_buf_pending & 0x04) == 0)
                {
#ifdef BT_MMM_DEBUG_INFO
                    libc_printf_warning("pcmbuf length unexpect!\n");
#endif
                    music_player->pcm_buf_hd_err = TRUE;
                }

                music_player->g_bp_pcm_buf_pending &= ~0x04;

                if (music_player->pcm_buf_hd_err == TRUE)
                {
                    uint32 hd_err_timer = sys_get_ab_timer();

                    while (1)
                    {
                        if (get_pcmbuf_length() < music_player->g_half_empty_thres)
                        {
#ifdef BT_MMM_DEBUG_INFO
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

            if ((music_player->g_bp_pcm_buf_pending & 0x02) != 0)
            {
                libc_printf_warning("dec too late!!\n");
                //NOTE��TWS �����������������Ķ�����������������϶࣬������������������ʱ��Ϊ����״̬���˳����䲥��
            }

            while (libc_sem_trywait(music_player->bt_dec_sem) == 0)
            {
                ; //����ź���
            }
            music_player->g_bp_pcm_buf_pending = 0;

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
                libc_printf_info("bt al cost:dec=%d,enc=%d,dae=%d,release=%d\n",
                        (music_player->dec_play_time / 1000),
                        (music_player->encoder_time / 1000),
                        (music_player->dae_play_time / 1000),
                        (music_player->release_time / 1000));
            }
        }

        if (music_player->music_status.status == MMM_BP_ENGINE_FADEIN)
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32) &tmp_pp_status);
#ifdef BT_MMM_DEBUG_INFO
            //libc_printf("fadein:%d\n", tmp_pp_status.fade_in_status);
#endif
            if (tmp_pp_status.fade_in_status == PP_FADE_IN_STANDBY)
            {
                if (need_start_print_time == TRUE)
                {
                    start_print_time_timer = sys_get_ab_timer();
                }

                libc_printf_info("fadein over\n");
                music_player->music_status.status = MMM_BP_ENGINE_PLAYING;

                //monitor΢��CLOCK��ʼ��ʱ
                music_player->adjust_sample_timer = sys_get_ab_timer();
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
                if (p_tws_info->dev_role != NORMAL_DEV)
                {
                    p_tws_info->adjust_audio_aps_timer = sys_get_ab_timer();
                }
#endif
            }
        }

        if ((music_player->music_status.status == MMM_BP_ENGINE_FADEOUT)
            || (music_player->music_status.status == MMM_BP_ENGINE_FADEOUT_PAUSE))
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32) &tmp_pp_status);
#ifdef BT_MMM_DEBUG_INFO
            //libc_printf("fadeout:%d\n", tmp_pp_status.fade_out_status);
#endif
            if (tmp_pp_status.fade_out_status == PP_FADE_OUT_STANDBY)
            {
                libc_printf_info("fadeout over\n");

                if (music_player->music_status.status == MMM_BP_ENGINE_FADEOUT_PAUSE)
                {
                    music_player->music_status.status = MMM_BP_ENGINE_PAUSE;
                }
                else
                {
                    music_player->music_status.status = MMM_BP_ENGINE_READY;
                    music_player->ready_because_fadeout_flag = TRUE;
                    music_player->buffer_time_length = (uint16) - 1;
                    music_player->decode_pp_frame_index = 0;
                    music_player->decode_pp_frame_total = 0;
                    //���Buffer
                    update_bt_input_buf(music_player->input_buf, music_player->input_buf->length);
                    music_player->frame_count = 0;
                }
            }
        }

        dec_err_deal:
        if (ret != 0)
        {
#if (SUPPORT_SOFT_KEYTONE == 1)
            //�ȴ��������������
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

            libc_printf_error("dec err deal:%d\n", ret);
            music_player->music_status.status = MMM_BP_ENGINE_ERROR;
            ret = 0;
        }

        loop_continue:

        //OK MIC�����ݲ��㵭������Ҫ��������������Ҫ sys_os_time_dly
        if (music_player->music_status.status == MMM_BP_ENGINE_READY)
        {
            need_sleep_flag = TRUE;
#if (SUPPORT_OK_MIC == 1)
            if ((music_player->work_mode == BP_WORK_MODE_MIC)
                && (music_player->ready_because_fadeout_flag == TRUE)
                && (music_player->g_start_sub_pre_flag == 1))
            {
                need_sleep_flag = FALSE;
            }
#endif
        }
        if ((music_player->music_status.status == MMM_BP_ENGINE_ERROR)
                || (music_player->music_status.status == MMM_BP_ENGINE_PAUSE))
        {
            need_sleep_flag = TRUE;
        }
        if (music_player->g_bp_pcm_buf_int_en_flag == FALSE)
        {
            need_sleep_flag = TRUE;
        }
        if (music_player->is_player_loop_enable == FALSE)
        {
            need_sleep_flag = TRUE;
        }

        if (need_sleep_flag == TRUE)
        {
            sys_os_time_dly(1); //unit 10ms
            need_sleep_flag = FALSE;
        }
        else if (music_player->g_bp_pcm_buf_int_en_flag == TRUE)
        {
            uint32 t1 = 0, t2;

            if (music_player->need_print_time == TRUE)
            {
                t1 = sys_get_ab_timer_us();
            }

#if (SUPPORT_OK_MIC == 1)
            if (music_player->work_mode == BP_WORK_MODE_MIC)
            {
                if (music_player->ok_mic_start_flag == OK_MIC_DATA_READ)
                {
                    uint8 *mic_temp_buffer = (uint8 *) SBC_DEC_TEMP_BUFFER_ADDR; //PEQ��ʱ�ռ䣬���Ը���
                    //��ֹ��Ϊ��������MIC�ۼ���ʱ�䳤����������
                    while (music_player->p_mic_input_buf_r->length > (OK_MIC_RAW_BUF_THRESHOLD+MY_POST_PROCESS_FRAME_SAMPLE*2))
                    {
                        libc_printf_warning("drop mic data!!\n");
                        buffer_rw_read(music_player->p_mic_input_buf_r, (short *) mic_temp_buffer, MY_POST_PROCESS_FRAME_SAMPLE*2);
                    }
                }
            }
#endif

            if (libc_sem_wait(music_player->bt_dec_sem, 20) < 0)
            {
                libc_printf("bt_dec_sem\n");
                while (1)
                {
                    ; //for QAC
                }
            }

            if (music_player->need_print_time == TRUE)
            {
                t2 = sys_get_ab_timer_us();
                music_player->release_time += (t2 - t1);
            }
        }

        ret = 0;
    }
    player_loop_exit:
    music_player->thread_terminate_flag = FALSE;

    /*ע�⣺libc_pthread_exit�޷����ص�*/
    libc_pthread_exit();

    return (void *) NULL;
}

