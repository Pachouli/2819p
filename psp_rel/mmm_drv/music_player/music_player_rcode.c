/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز����м��  �������ѭ����ʵ�ֶԽ������̵Ŀ���
 ********************************************************************************/

#include "music_player_global.h"

PP_para_info_t g_PP_para_info;

//�ļ���ʽ���ͼ��ʱ����ʱbuf;
uint8* format_check_temp_buf;

const uint8 g_sample_rate_list[] =
    { 48, 44, 32, 24, 22, 16, 12, 11, 8 };

const uint16 g_pcm_buf_fu_thres[] =
    {
        //48K,  44K,    32K,   24K,   22K,   16K,   12K,   11K,   8K
        0x1000, 0x1000, 0xAC0, 0x800, 0x800, 0x560, 0x400, 0x400, 0x2C0
    };

const uint16 g_pcm_buf_he_diff[] =
    {
        //48K,  44K,    32K,   24K,   22K,   16K,   12K,   11K,   8K
        512, 512, 360, 256, 256, 180, 128, 128, 100
    };

const uint16 g_pcm_buf_block[] =
    {
        //48K,  44K,    32K,   24K,   22K,   16K,   12K,   11K,   8K
        256, 256, 128, 128, 128, 64, 64, 64, 32
    };

bool get_sample_rate_index(music_player_t *music_player)
{
    uint8 i;
    int32 cmp_sample_rate;

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    //Ҫ�����Ƿ�֧���ز�����ѡ�����Ŀ��
    if (music_player->resample_info.enable_resample == TRUE)
    {
        //֧���ز�����ѡ���ز������������
        cmp_sample_rate = music_player->resample_info.convert_output_rate_hz / 1000;
    }
    else
#endif
    {
        //��֧���ز�����ѡ��Դ�����ļ�������
        cmp_sample_rate = music_player->music_info.sample_rate;
    }

    for (i = 0; i < sizeof(g_sample_rate_list) / sizeof(uint8); i++)
    {
        if (cmp_sample_rate == g_sample_rate_list[i])
        {
            music_player->sample_rate_index = i;
            break;
        }
    }

    if (i == sizeof(g_sample_rate_list) / sizeof(uint8))
    {
        return FALSE;
    }

    return TRUE;
}

//0x01��ʾPCM_BUF����жϣ�0x02��ʾPCM_BUFȫ���жϣ�0x04��ʾPCM_BUF�����ж�
void music_pcm_buf_int_cbk(uint8 pending)
{
    if (music_player_global->music_pcmbuf_sem != NULL)
    {
        music_player_global->g_mp_pcm_buf_pending |= pending;
        if ((pending & 0x03) != 0)
        {
            libc_sem_post(music_player_global->music_pcmbuf_sem);
        }
    }
}

int32 get_media_info(music_player_t *music_player, uint32 args)
{
    /* ��parser�ṩ������������Ϣת��ap�����ý����Ϣ */
    mmm_mp_file_info_t *fileinfo = (mmm_mp_file_info_t *) args;
    //      fileinfo->file_format[0] = music_player->music_info.extension[0];
    //      fileinfo->file_format[1] = music_player->music_info.extension[1];
    //      fileinfo->file_format[2] = music_player->music_info.extension[2];
    //      fileinfo->file_format[3] = music_player->music_info.extension[3];
    fileinfo->total_time = music_player->music_info.total_time;
    fileinfo->bitrate = music_player->music_info.avg_bitrate;
    fileinfo->sample_rate = music_player->music_info.sample_rate;
    fileinfo->channels = music_player->music_info.channels;
    fileinfo->drm_type = music_player->music_info.drm_flag;
    fileinfo->sample_rate_hz = music_player->music_info.sample_rate_hz;
    fileinfo->music_type = music_player->music_info.music_type;
    fileinfo->bps = music_player->music_info.bps;
    return 0;
}

int32 player_decode_daedeal_playend(music_player_t *music_player)
{
    int32 ret = 0;

    if (music_player->last_unalign_frame_sc == 0)
    {
        return 0;
    }

    if (music_player->last_unalign_frame_sc < MY_POST_PROCESS_FRAME_SAMPLE)
    {
        libc_memset((void* )(DAE_128_ALIGN_BUF_ADDR + music_player->last_unalign_frame_sc * 4),
                0x00, (MY_POST_PROCESS_FRAME_SAMPLE - music_player->last_unalign_frame_sc) * 4);
    }

    music_player->aout_pp.pcm[0] = (void*) DAE_128_ALIGN_BUF_ADDR;
    music_player->aout_pp.samples = MY_POST_PROCESS_FRAME_SAMPLE;
    ret = post_process_ops(music_player->audiopp_handle, PP_CMD_FRAME_PROCESS, (uint32 ) &music_player->aout_pp);
    if (ret != 0)
    {
        libc_printf_error("post_process_ops frame error:%d\n", ret);
        music_player->music_status.status = MMM_MP_ENGINE_ERROR;
        return ret;
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    if ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
    {
        keytone_frame_deal(music_player, music_player->aout_pp.pcm[0], MY_POST_PROCESS_FRAME_SAMPLE);
    }
#endif

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (music_player->resample_info.enable_resample == TRUE)
    {
        uint8 pcm_buf_full_flag;
        resample_and_send_pcm_to_pcm_buf(music_player, (short*)(music_player->aout_pp.pcm[0]), \
                music_player->aout_pp.samples, &pcm_buf_full_flag, TRUE);
    }
    else
#endif
    {
        send_pcm_to_pcm_buf_by_dac_fifo(music_player->aout_pp.pcm[0], music_player->aout_pp.samples * 2);
    }

    return 0;
}

static void player_frame_lowpw_deal(music_player_t *music_player)
{
    if ((music_player->music_setting.music_lowpw_th_max != 0)
            || (music_player->music_setting.music_lowpw_th_rms != 0))
    {
        bool max_match_flag = TRUE;
        bool rms_match_flag = TRUE;

        if (music_player->music_setting.music_lowpw_th_max != 0)
        {
            if (music_player->music_status.music_cur_energy
                    > music_player->music_setting.music_lowpw_th_max)
            {
                max_match_flag = FALSE;
            }
        }

        if (music_player->music_setting.music_lowpw_th_rms != 0)
        {
            if (music_player->music_status.music_cur_energy_rms
                    > music_player->music_setting.music_lowpw_th_rms)
            {
                rms_match_flag = FALSE;
            }
        }

        if ((max_match_flag == TRUE) && (rms_match_flag == TRUE))
        {
            music_player->music_lowpw_frame++;
        }
        else
        {
            music_player->music_lowpw_frame = 0;
        }

        music_player->music_status.music_lowpw_time = music_player->music_lowpw_frame
                * MY_POST_PROCESS_FRAME_SAMPLE / music_player->music_info.sample_rate;
    }
}

//samplesָ��������
//����Ϊ music_player->aout_lch_buf/aout_rch_buf
void aout_turn_to_lrlr_16bit_format(music_player_t *music_player, short *result_buf, uint16 samples)
{
    uint32 i;

    if (music_player->aout_ch_num == 1)
    {
        if (music_player->aout_sample_bits == 16)
        {
            libc_memcpy(result_buf, music_player->aout_lch_buf.raw_data + music_player->aout_lch_buf.read_ptr,
                    samples * 4);
        }
        else
        {
            short *dest;
            short *src;

            dest = result_buf;
            src = (short *) (music_player->aout_lch_buf.raw_data + music_player->aout_lch_buf.read_ptr);
            for (i = 0; i < samples; i++)
            {
                dest[i * 2 + 0] = src[i * 4 + 1];
                dest[i * 2 + 1] = src[i * 4 + 3];
            }
        }
    }
    else
    {
        if (music_player->aout_sample_bits == 16)
        {
            short *dest;
            short *src_lch, *src_rch;

            dest = result_buf;
            src_lch = (short *) (music_player->aout_lch_buf.raw_data + music_player->aout_lch_buf.read_ptr);
            src_rch = (short *) (music_player->aout_rch_buf.raw_data + music_player->aout_rch_buf.read_ptr);
            for (i = 0; i < samples; i++)
            {
                dest[i * 2 + 0] = src_lch[i];
                dest[i * 2 + 1] = src_rch[i];
            }
        }
        else
        {
            short *dest;
            short *src_lch, *src_rch;

            dest = result_buf;
            src_lch = (short *) (music_player->aout_lch_buf.raw_data + music_player->aout_lch_buf.read_ptr);
            src_rch = (short *) (music_player->aout_rch_buf.raw_data + music_player->aout_rch_buf.read_ptr);
            for (i = 0; i < samples; i++)
            {
                dest[i * 2 + 0] = src_lch[i * 2 + 1];
                dest[i * 2 + 1] = src_rch[i * 2 + 1];
            }
        }
    }
}

//����ǣ���buffer����˫buffer��λ��������ͨ�� aout_lch_buf aout_rch_buf ����ȡ
int32 init_decode_aout_data(music_player_t *music_player)
{
    music_player->aout_sample_bits = music_player->aout.sample_bits;
    if (music_player->aout_sample_bits == 16)
    {
        music_player->aout_sample_bytes = 2;
    }
    else if ((music_player->aout_sample_bits == 24) || (music_player->aout_sample_bits == 32))
    {
        music_player->aout_sample_bytes = 4;
    }
    else
    {
        libc_printf_warning("invalid aout sample bits! %d\n", music_player->aout_sample_bits);
        while (1)
            ;
    }
    music_player->aout_ch_num = 1;
    music_player->aout_lch_sample_bytes = music_player->aout_sample_bytes * 2;
    if (music_player->aout.pcm[1] != NULL)
    {
        music_player->aout_ch_num = 2;
        music_player->aout_lch_sample_bytes = music_player->aout_sample_bytes;
    }

    sys_os_sched_lock();

    //left channel
    music_player->aout_lch_buf.raw_data = (uint8 *) (music_player->aout.pcm[0]);
    music_player->aout_lch_buf.capacity = music_player->aout.samples * music_player->aout_sample_bytes;
    if (music_player->aout_ch_num == 1)
    {
        music_player->aout_lch_buf.capacity *= 2;
    }
    music_player->aout_lch_buf.length = music_player->aout_lch_buf.capacity;
    music_player->aout_lch_buf.read_ptr = 0;
    music_player->aout_lch_buf.write_ptr = 0;

    //right channel
    if (music_player->aout_ch_num == 2)
    {
        music_player->aout_rch_buf.raw_data = (uint8 *) (music_player->aout.pcm[1]);
        music_player->aout_rch_buf.capacity = music_player->aout.samples * music_player->aout_sample_bytes;
        music_player->aout_rch_buf.length = music_player->aout_rch_buf.capacity;
        music_player->aout_rch_buf.read_ptr = 0;
        music_player->aout_rch_buf.write_ptr = 0;
    }

    sys_os_sched_unlock();

    return 0;
}

bool align_decode_aout_data(music_player_t *music_player)
{
    uint32 cur_deal_length;
    bool need_pp_flag = TRUE;

    if (music_player->last_unalign_frame_sc != 0)
    {
        if ((music_player->last_unalign_frame_sc
                + (music_player->aout_lch_buf.length / music_player->aout_lch_sample_bytes))
                >= MY_POST_PROCESS_FRAME_SAMPLE)
        {
            cur_deal_length = (MY_POST_PROCESS_FRAME_SAMPLE - music_player->last_unalign_frame_sc)
                    * music_player->aout_lch_sample_bytes;
        }
        else
        {
            cur_deal_length = music_player->aout_lch_buf.length;
            need_pp_flag = FALSE;
        }

        //ת��Ϊ L/R/L/R & 16Bits��ʽ
        aout_turn_to_lrlr_16bit_format(music_player,
                (short *) (DAE_128_ALIGN_BUF_ADDR + music_player->last_unalign_frame_sc * 4),
                cur_deal_length / music_player->aout_lch_sample_bytes);

        music_player->last_unalign_frame_sc += cur_deal_length / music_player->aout_lch_sample_bytes;
    }
    else
    {
        if (music_player->aout_lch_buf.length
                >= (MY_POST_PROCESS_FRAME_SAMPLE * music_player->aout_lch_sample_bytes))
        {
            cur_deal_length = MY_POST_PROCESS_FRAME_SAMPLE * music_player->aout_lch_sample_bytes;

            music_player->last_unalign_frame_sc = MY_POST_PROCESS_FRAME_SAMPLE;
        }
        else
        {
            cur_deal_length = music_player->aout_lch_buf.length;

            music_player->last_unalign_frame_sc = cur_deal_length / music_player->aout_lch_sample_bytes;
            need_pp_flag = FALSE;
        }

        //��ת��Ϊ L/R/L/R & 16Bits��ʽ
        aout_turn_to_lrlr_16bit_format(music_player, (short *) (DAE_128_ALIGN_BUF_ADDR),
                cur_deal_length / music_player->aout_lch_sample_bytes);
    }

    music_player->aout_lch_buf.length -= cur_deal_length;
    music_player->aout_lch_buf.read_ptr += cur_deal_length;
    if (music_player->aout_ch_num == 2)
    {
        music_player->aout_rch_buf.length -= cur_deal_length;
        music_player->aout_rch_buf.read_ptr += cur_deal_length;
    }

    return need_pp_flag;
}

/*
    �ڽ���ͺ���󣬵��ô˺������������ݴ���dac_fifo
    �ú�������player_loop�������ѭ���У�����ֵ��Ӱ��player_loop�Ĵ����߼�
������
    pcm_buf_full_flag: player_loop��Ҫ��һ������ֵ
����ֵ��
    TRUE: player_loop������ú�����ִ��goto loop_continue
    FALSE: player_loop������ú����������ǰִ��
*/
int32 loop_send_pcm_to_pcm_buf(music_player_t *music_player, short *send_data, uint16 samples_cnt, \
    bool *pcm_buf_full_flag, bool ignore_dec_too_late)
{
    uint32 t1 = 0, t2;

    music_player->dac_fifo_buf.raw_data = (uint8 *) (send_data);
    music_player->dac_fifo_buf.capacity = samples_cnt * 2 * 2; //��������˫����16bit PCM����
    music_player->dac_fifo_buf.length = music_player->dac_fifo_buf.capacity;
    music_player->dac_fifo_buf.read_ptr = 0;
    music_player->dac_fifo_buf.write_ptr = 0;

    while (music_player->dac_fifo_buf.length > 0)
    {
        uint16 tmp_pcm_buf_fu_thres, tmp_pcm_buf_diff_thres, tmp_pcm_buf_block;
        int32 send_length;

#if (SUPPORT_OK_MIC == 1)
        if (music_player->work_mode == MP_WORK_MODE_MIC)
        {
            tmp_pcm_buf_block = MY_POST_PROCESS_FRAME_SAMPLE*2;
        }
        else
#endif
        {
            tmp_pcm_buf_fu_thres = g_pcm_buf_fu_thres[music_player->sample_rate_index];
            tmp_pcm_buf_diff_thres = g_pcm_buf_he_diff[music_player->sample_rate_index];
            tmp_pcm_buf_block = g_pcm_buf_block[music_player->sample_rate_index];
        }

        if (music_player->dac_fifo_buf.length >= (tmp_pcm_buf_block * 2))
        {
            send_length = tmp_pcm_buf_block * 2;
        }
        else
        {
            send_length = music_player->dac_fifo_buf.length;
        }

        send_pcm_to_pcm_buf_by_dac_fifo((short *) (music_player->dac_fifo_buf.raw_data
                + music_player->dac_fifo_buf.read_ptr), send_length / 2); //��λ��������

        if (music_player->g_mp_pcm_buf_int_en_flag == FALSE)
        {
#if (SUPPORT_OK_MIC == 1)
            if (music_player->work_mode == MP_WORK_MODE_MIC)
            {
                music_player->g_half_empty_thres = OK_MIC_PCM_BUF_HE_THRESHOLD;
            }
            else
#endif
            {
                music_player->g_half_empty_thres = tmp_pcm_buf_fu_thres - tmp_pcm_buf_diff_thres;
            }
            music_player->g_half_full_thres = music_player->g_half_empty_thres + 16;

            init_pcm_buf(music_player->g_half_empty_thres, music_player->g_half_full_thres);

            //start DAC PLAY
            music_player->g_mp_pcm_buf_int_en_flag = TRUE;
            if (libc_sem_init(&(music_player->music_pcmbuf_sem), 0) == -1)
            {
                libc_printf("music_pcmbuf_sem\n");
                while (1)
                ;
            }
        }

        if (((music_player->g_mp_pcm_buf_pending & 0x04) != 0)
                || (get_pcmbuf_length() >= music_player->g_half_full_thres))
        {
            *pcm_buf_full_flag = TRUE;

            if ((music_player->g_mp_pcm_buf_pending & 0x04) == 0)
            {
#ifdef MUSIC_MMM_DEBUG_INFO
                libc_printf_warning("pcmbuf length unexpect!\n");
#endif
                music_player->pcm_buf_hd_err = TRUE;
            }

            music_player->g_mp_pcm_buf_pending &= ~0x04;

            if (music_player->pcm_buf_hd_err == TRUE)
            {
                uint32 hd_err_timer = sys_get_ab_timer();

                while (1)
                {
                    if (get_pcmbuf_length() < music_player->g_half_empty_thres)
                    {
#ifdef MUSIC_MMM_DEBUG_INFO
                        libc_printf("wait length ok\n");
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
                //�ͷ�ʱ��Ƭ���ȴ�PCM_BUF����ж�
                if (music_player->need_print_time == 1)
                {
                    t1 = sys_get_ab_timer_us();
                }

#if (SUPPORT_OK_MIC == 1)
                if ((music_player->work_mode == MP_WORK_MODE_MIC)
                    && (music_player->ok_mic_start_flag == OK_MIC_DATA_READ))
                {
                    //��ֹ��Ϊ��������MIC�ۼ���ʱ�䳤����������
                    while (music_player->p_mic_input_buf_r->length > OK_MIC_RAW_BUF_THRESHOLD)
                    {
                        libc_printf_warning("drop mic data!!\n");
                        buffer_rw_read(music_player->p_mic_input_buf_r, (short *) OK_MIC_TEMP_BUFF, MY_POST_PROCESS_FRAME_SAMPLE*2);
                    }
                }
#endif

                if (libc_sem_wait(music_player->music_pcmbuf_sem, 20) < 0)
                {
                    libc_printf("music_pcmbuf_sem\n");
                    while (1)
                    ;
                }

                if (music_player->need_print_time == 1)
                {
                    t2 = sys_get_ab_timer_us();
                    music_player->release_time += (t2 - t1);
                }
            }
        }
        else
        {
            if ((music_player->g_mp_pcm_buf_pending & 0x02) != 0)
            {
                if (ignore_dec_too_late == FALSE)
                {
                    libc_printf_warning("dec too late!\n");
                }
            }

            while (libc_sem_trywait(music_player->music_pcmbuf_sem) == 0)
            {
                ; //����ź���
            }
            music_player->g_mp_pcm_buf_pending = 0;
        }

        music_player->dac_fifo_buf.length -= send_length;
        music_player->dac_fifo_buf.read_ptr += send_length;

        if ((music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
                || (music_player->is_player_loop_enable == FALSE))
        {
            //goto loop_continue; //�ղ���
            return TRUE;
        }
    }

    return FALSE;
}

void *player_loop(music_player_t *music_player)
{
    int32 ret = 0;
    uint32 tmp_fill_timer;
    bool pcm_buf_full_flag = FALSE;
    bool need_sleep_flag = FALSE;
    bool seek_back_to_play = FALSE;
    bool ignore_dec_too_late = FALSE;
    uint32 seek_delay = 0;
    uint32 t1 = 0, t2;
    bool print_aout_info = TRUE;
    uint32 start_print_time_timer = sys_get_ab_timer();
    bool need_start_print_time = TRUE;
    bool need_start_ok_mic = TRUE;
    uint32 decode_freq_backup = 0;

    for (;;)
    {
        /* �ж�������Ҫ����߳����������Ĺ��ܱ�־λ */
        if (music_player->thread_terminate_flag == TRUE)
        {
            goto player_loop_exit;
        }

        if ((music_player->music_status.status == MMM_MP_ENGINE_ERROR)
                || (music_player->is_player_loop_enable == FALSE))
        {
            goto loop_continue;
        }

        if ((music_player->music_status.status == MMM_MP_ENGINE_PLAYEND)
                || (music_player->music_status.status == MMM_MP_ENGINE_PAUSED))
        {
            goto loop_continue;
        }

        if ((need_start_print_time == TRUE) && (start_print_time_timer > 0))
        {
            if ((sys_get_ab_timer() - start_print_time_timer) > 10 * 1000) //��ʼ���ź�10S��ʼͳ��
            {
                libc_printf("start moniter\n");
                need_start_print_time = FALSE;
                music_player->need_print_time = TRUE;
                sys_performance_monitoring_restart();
            }
        }

        //�ǿ������״̬��
        if ((music_player->music_ffrev_flag == 0) && (music_player->music_seek_flag == 0))
        {
            seek_delay = 0;
            ignore_dec_too_late = FALSE;
            if (seek_back_to_play == TRUE)
            {
                ignore_dec_too_late = TRUE;
            }
            seek_back_to_play = FALSE;

            if ((music_player->music_stop_flag != 0) && (music_player->music_fade_flag != 0x2))
            {
                ret = AD_RET_DATAUNDERFLOW;
                music_player->music_stop_flag = 0;
                music_player->music_fade_flag = 0;
                break;
            }
            else
            {
                pcm_buf_full_flag = FALSE;
                tmp_fill_timer = sys_get_ab_timer();

                decode_cont:

                if (music_player->need_print_time == TRUE)
                {
                    t1 = sys_get_ab_timer_us();
                }

#if (SUPPORT_LOOP_PLAY == 1)
                //loopplay:�ж��Ƿ�ͷ��β������ǣ�������Ҫ��Ƶ
                if (music_player->music_loopplay_info.filter_mute_frame == TRUE)
                {
                    decode_freq_backup = sys_adjust_clk(FREQ_222M, SET_DECODE_CLK);
                }
#endif
                ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_FRAME_DECODE,
                        (uint32 )&music_player->music_decode_info);
#if (SUPPORT_LOOP_PLAY == 1)
                if (music_player->music_loopplay_info.filter_mute_frame == TRUE)
                {
                    sys_adjust_clk(decode_freq_backup, SET_DECODE_CLK);
                }
#endif
                if (ret != 0)
                {
                    goto err_deal;
                }

                music_player->ok_mic_wait_decode_first = FALSE;

                if (music_player_global->storage_read_err == TRUE)
                {
                    ret = AD_RET_UNEXPECTED;
                    music_player->music_status.status = MMM_MP_ENGINE_ERROR;
                    music_player->music_status.err_no = MMM_MP_ERR_DISK_ERROR;
                    goto err_deal;
                }

                if (music_player->need_print_time == TRUE)
                {
                    t2 = sys_get_ab_timer_us();

                    music_player->dec_play_time += (t2 - t1);

                    music_player->play_samples_add += music_player->aout.samples;
                }

                if (music_player->need_print_time == TRUE)
                {
                    uint32 tmp_time_ms;

                    tmp_time_ms = music_player->play_samples_add / music_player->music_info.sample_rate;
                    if (tmp_time_ms > 5000)
                    {
                        sys_performance_monitoring_report();
                        sys_performance_monitoring_stop();

                        music_player->need_print_time = FALSE;
                        libc_printf_info("music al cost:dec=%d,dae=%d,rs=%d,rcard=%d,release=%d\n",
                                (music_player->dec_play_time / 1000),
                                (music_player->dae_play_time / 1000),
                                (music_player->resample_time / 1000),
                                (music_player->read_card_time / 1000),
                                (music_player->release_time / 1000));
                    }
                }

                if (print_aout_info == TRUE)
                {
                    libc_printf_info("music decode aout : 0x%x,0x%x,%d bits,%d khz\n",
                            music_player->aout.pcm[0], music_player->aout.pcm[1], music_player->aout.sample_bits,
                            music_player->music_info.sample_rate);
                    print_aout_info = FALSE;
                }

                init_decode_aout_data(music_player);

#if (SUPPORT_OK_MIC == 1)
                if (music_player->work_mode == MP_WORK_MODE_MIC)
                {
                    //������Ч����ת�Ƶ�OK MIC�����ȼ��̣߳��ȴ��ź������ͷ�ʱ��Ƭ
                    music_player->decode_wait_ok_mic_post_sem = TRUE;
                    if (libc_sem_wait(music_player->music_dec_sem, 20) < 0)
                    {
                        libc_printf("music_dec_sem\n");
                        while (1)
                        ;
                    }
                    while (libc_sem_trywait(music_player->music_dec_sem) == 0)
                    {
                        ; //����ź���
                    }
                    music_player->decode_wait_ok_mic_post_sem = FALSE;
                }
                else
#endif
                {
                    //�������
                    while (music_player->aout_lch_buf.length > 0)
                    {
                        if (align_decode_aout_data(music_player) == TRUE)
                        {
                            if (music_player->last_unalign_frame_sc == MY_POST_PROCESS_FRAME_SAMPLE)
                            //��ǰ��Щ��ʽ����Ҫ���� DAE_128_ALIGN_BUF_ADDR ��ת�����жϾ��������ж��Ƿ���ת������
                            {
                                music_player->aout_pp.pcm[0] = (void *) DAE_128_ALIGN_BUF_ADDR;
                                music_player->last_unalign_frame_sc = 0;
                            }

                            music_player->aout_pp.samples = MY_POST_PROCESS_FRAME_SAMPLE;
                        }
                        else
                        {
                            break;
                        }

                        if (music_player->need_print_time == TRUE)
                        {
                            t1 = sys_get_ab_timer_us();
                        }

                        //��������
                        energy_calc(music_player->aout_pp.pcm[0], MY_POST_PROCESS_FRAME_SAMPLE, TRUE,
                                &(music_player->music_status.music_cur_energy),
                                &(music_player->music_status.music_cur_energy_rms));
                        player_frame_lowpw_deal(music_player);

#if (SUPPORT_LOOP_PLAY == 1)
                        if (music_player->music_loopplay_info.is_loop_play == TRUE)
                        {
                            if (filter_mute_frame(music_player) == TRUE)
                            {
                                continue;
                            }
                        }
#endif

                        //������Ч����
                        ret = post_process_ops(music_player->audiopp_handle, PP_CMD_FRAME_PROCESS,
                                (uint32 ) &music_player->aout_pp);
                        if (ret != 0)
                        {
                            libc_printf_error("post_process_ops frame error:%d\n", ret);
                            music_player->music_status.status = MMM_MP_ENGINE_ERROR;
                            break;
                        }

#if (SUPPORT_SOFT_KEYTONE == 1)
                        if ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
                        {
                            keytone_frame_deal(music_player, music_player->aout_pp.pcm[0], MY_POST_PROCESS_FRAME_SAMPLE);
                        }
#endif

                        //��ȡ��Ƶ������ֵ
                        if (g_freq_point_cfg.freq_point_enable == TRUE)
                        {
                            if (post_process_ops(music_player->audiopp_handle, PP_CMD_GET_FREQ_POINT_ENERGY,
                                    (uint32) music_player->music_status.freq_point_energy) < 0)
                            {
                                libc_memset(music_player->music_status.freq_point_energy, 0x00,
                                        sizeof(music_player->music_status.freq_point_energy));
                            }
                        }

                        if (music_player->need_print_time == TRUE)
                        {
                            t2 = sys_get_ab_timer_us();

                            music_player->dae_play_time += (t2 - t1);
                        }

#if (SUPPORT_AUDIO_RESAMPLE == 1)
                        if (music_player->resample_info.enable_resample == TRUE)
                        {
                            if (resample_and_send_pcm_to_pcm_buf(music_player, (short*)(music_player->aout_pp.pcm[0]), \
                                    music_player->aout_pp.samples, &pcm_buf_full_flag, ignore_dec_too_late) == TRUE)
                            {
                                goto loop_continue;
                            }
                        }
                        else
#endif
                        {
                            if (loop_send_pcm_to_pcm_buf(music_player, music_player->aout_pp.pcm[0], music_player->aout_pp.samples, \
                                    &pcm_buf_full_flag, ignore_dec_too_late) == TRUE)
                            {
                                goto loop_continue;
                            }
                        }

                        /*
                         fadeout ����
                         */
                        if (music_player->music_status.status == MMM_MP_ENGINE_FADEOUT)
                        {
                            audiopp_status_t tmp_pp_status;

                            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32 ) &tmp_pp_status);

                            if (tmp_pp_status.fade_out_status == PP_FADE_OUT_STANDBY)
                            {
                                break;
                            }
                        }

                        if ((music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
                                || (music_player->is_player_loop_enable == FALSE))
                        {
                            goto loop_continue; //�ղ���
                        }
                    }
                }

#if (SUPPORT_OK_MIC == 1)
                if (music_player->work_mode == MP_WORK_MODE_MIC)
                {
                    //��OK MIC�̴߳����뵭��
                }
                else
#endif
                {
                    /*
                     fadein ����
                     */
                    if (music_player->music_status.status == MMM_MP_ENGINE_FADEIN)
                    {
                        audiopp_status_t tmp_pp_status;

                        post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32 ) &tmp_pp_status);
#ifdef MUSIC_MMM_DEBUG_INFO
                        //libc_printf("fadein:%d\n", tmp_pp_status.fade_in_status);
#endif
                        if (tmp_pp_status.fade_in_status == PP_FADE_IN_STANDBY)
                        {
                            libc_printf_info("fadein over\n");
                            music_player->music_status.status = MMM_MP_ENGINE_PLAYING;
                        }
                    }

                    /*
                     fadeout ����
                     */
                    if (music_player->music_status.status == MMM_MP_ENGINE_FADEOUT)
                    {
                        audiopp_status_t tmp_pp_status;

                        post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32 ) &tmp_pp_status);
#ifdef MUSIC_MMM_DEBUG_INFO
                        //libc_printf("fadeout:%d\n", tmp_pp_status.fade_out_status);
#endif
                        if (tmp_pp_status.fade_out_status == PP_FADE_OUT_STANDBY)
                        {
                            libc_printf_info("fadeout over\n");
                            music_player->music_status.status = MMM_MP_ENGINE_PAUSED;
                        }
                    }
                }

                if ((music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
                        || (music_player->is_player_loop_enable == FALSE))
                {
                    goto loop_continue; //�ղ���
                }

#if (SUPPORT_OK_MIC == 1)
                if (music_player->work_mode == MP_WORK_MODE_MIC)
                {
                    //��OK MIC�̴߳������ʱ��û�������ͷ�ʱ��Ƭ
                }
                else
#endif
                {
                    if (pcm_buf_full_flag == FALSE)
                    {
                        if ((sys_get_ab_timer() - tmp_fill_timer) > 1000) //1����û�������ͷ�ʱ��Ƭ����ֹ����
                        {
                            need_sleep_flag = TRUE;
                            libc_printf_warning("fill overtime!\n");
                        }
                        else
                        {
                            goto decode_cont;
                        }
                    }
                }
            }
        }
        else
        {
            uint32 delay_time;

            if (seek_delay == 0)
            {
                ret = seek_process(music_player, &delay_time);
                seek_delay = music_player->music_setting.ffb_delay/10;
            }

            if (seek_delay > 0)
            {
                seek_delay--;
            }

            need_sleep_flag = TRUE;
            seek_back_to_play = TRUE;
        }

        err_deal:
        if (ret != 0)
        {
            if ((ret == AD_RET_DATAUNDERFLOW) && (music_player->music_status.status == MMM_MP_ENGINE_PLAYING))
            {
#if (SUPPORT_OK_MIC == 1)
                if (music_player->work_mode == MP_WORK_MODE_MIC)
                {
                    //OK MIC�ݲ���������֡
                }
                else
#endif
                {
                    //��������һ֡DAE��������128����β�������
                    if ((ret == AD_RET_DATAUNDERFLOW) && (music_player->storage_read_err == FALSE))
                    {
                        player_decode_daedeal_playend(music_player);
                    }
                }
            }

            if (ret == AD_RET_DATAUNDERFLOW)
            {
                music_player->music_status.status = MMM_MP_ENGINE_PLAYEND;
                ret = 0;
            }
            else
            {
                music_player->music_status.status = MMM_MP_ENGINE_ERROR;
                ret = -1;
            }

            //���������޸�״̬���Ա�OK MIC�߳̿�ת������PCM_BUF
            stop_music_decoder_wait_pcmbuf_empty(music_player);
        }

        loop_continue:

        if ((music_player->g_mp_pcm_buf_int_en_flag == FALSE)
                || (music_player->music_status.status == MMM_MP_ENGINE_PLAYEND)
                || (music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
                || (music_player->music_status.status == MMM_MP_ENGINE_ERROR)
                || (music_player->is_player_loop_enable == FALSE)
                || (need_sleep_flag == TRUE))
        {
            //����������״̬��
            //����ȴ�����ķǶ��뻺����
            music_player->last_unalign_frame_sc = 0;
#if (SUPPORT_AUDIO_RESAMPLE == 1)
            //����ز������뻺����
            music_player->resample_info.cur_resample_input_len = 0;
#endif
            sys_os_time_dly(1); //unit 10ms
            need_sleep_flag = FALSE;
        }
        ret = 0;
    }
    player_loop_exit:

    music_player->thread_terminate_flag = FALSE;

    /*ע�⣺libc_pthread_exit�޷����ص�*/
    libc_pthread_exit();

    return (void *) NULL;
}

#if (SUPPORT_OK_MIC == 1)
void *ok_mic_loop(music_player_t *music_player)
{
    int32 ret = 0;
    uint32 t1 = 0, t2;
    bool pcm_buf_full_flag;
    bool need_request_decode;
    bool need_sleep_flag = FALSE;
    uint32 tmp_fill_timer = sys_get_ab_timer();

    if (libc_sem_init(&(music_player->music_dec_sem), 0) == -1)
    {
        libc_printf("music_dec_sem\n");
        while (1)
        ;
    }

    while (1)
    {
        if (music_player->ok_mic_thread_terminate_flag == TRUE)
        {
            goto ok_mic_loop_exit;
        }

        if ((music_player->ok_mic_wait_decode_first == TRUE)
            || (music_player->music_status.status == MMM_MP_ENGINE_ERROR)
            || (music_player->is_ok_mic_loop_enable == FALSE))
        {
            goto ok_mic_loop_continue;
        }

        if ((music_player->music_status.status == MMM_MP_ENGINE_PLAYEND)
                || (music_player->music_status.status == MMM_MP_ENGINE_PAUSED))
        {
            goto ok_mic_loop_continue;
        }

        //���cache
        need_request_decode = TRUE;
        while (music_player->aout_lch_buf.length > 0)
        {
            if (music_player->decode_output_cache_rw.length < music_player->decode_output_cache_rw.capacity)
            {
                if (align_decode_aout_data(music_player) == TRUE)
                {
                    buffer_rw_write(&(music_player->decode_output_cache_rw), DAE_128_ALIGN_BUF_ADDR, MY_POST_PROCESS_FRAME_SAMPLE*4);
                    music_player->last_unalign_frame_sc = 0;
                }
            }
            else
            {
                //��������ˣ���Ҫ��ʱHoldס����
                need_request_decode = FALSE;
                break;
            }
        }

        //�����������
        if ((need_request_decode == TRUE) && (music_player->decode_wait_ok_mic_post_sem == TRUE))
        {
            libc_sem_post(music_player->music_dec_sem);
        }

        //�������
        if (music_player->decode_output_cache_rw.length >= MY_POST_PROCESS_FRAME_SAMPLE*4)
        {
            buffer_rw_read(&(music_player->decode_output_cache_rw), OK_MIC_DECODE_BUFF, MY_POST_PROCESS_FRAME_SAMPLE*4);
        }
        else
        {
            libc_memset(OK_MIC_DECODE_BUFF, 0x0, MY_POST_PROCESS_FRAME_SAMPLE*4);

            if ((music_player->music_ffrev_flag == 0) && (music_player->music_seek_flag == 0))
            {//�����״̬�²���ӡ
                libc_printf_warning("dec empty!\n");
            }
        }

        //libc_printf("mic:%d,%d=%d\n", music_player->p_mic_input_buf_r->length, get_pcmbuf_length(),\
        //    (music_player->p_mic_input_buf_r->length + get_pcmbuf_length())*5/music_player->music_info.sample_rate);

        ok_mic_data_read_again:
        if (music_player->ok_mic_start_flag == OK_MIC_DATA_FLUSH)
        {
            if (music_player->music_status.status == MMM_MP_ENGINE_PLAYING)
            {
                music_player->ok_mic_start_flag = OK_MIC_DATA_BUFFER;
                goto ok_mic_data_read_again;
            }
            //���Buffer
            buffer_rw_flush(music_player->p_mic_input_buf_r);
            libc_memset(OK_MIC_TEMP_BUFF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
        }
        else if (music_player->ok_mic_start_flag == OK_MIC_DATA_BUFFER)
        {
            if (music_player->p_mic_input_buf_r->length >= OK_MIC_RAW_BUF_THRESHOLD)
            {
                music_player->ok_mic_start_flag = OK_MIC_DATA_READ;
                goto ok_mic_data_read_again;
            }
            libc_memset(OK_MIC_TEMP_BUFF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
        }
        else
        {
            if (buffer_rw_read(music_player->p_mic_input_buf_r, (short *) OK_MIC_TEMP_BUFF, MY_POST_PROCESS_FRAME_SAMPLE*2) == FALSE)
            {
                libc_memset(OK_MIC_TEMP_BUFF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
                libc_printf_warning("mic empty!\n");
            }
        }

        if (music_player->ok_mic_mute_flag == TRUE)
        {
            libc_memset(OK_MIC_TEMP_BUFF, 0x00, MY_POST_PROCESS_FRAME_SAMPLE*2);
        }

        music_player->aout_pp.pcm[0] = (void *)(music_player->decode_output_cache_rw.raw_data + music_player->decode_output_cache_rw.read_ptr);
        music_player->aout_pp.pcm[1] = OK_MIC_TEMP_BUFF;
        music_player->aout_pp.samples = MY_POST_PROCESS_FRAME_SAMPLE;

        if (music_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        //��������
        energy_calc(music_player->aout_pp.pcm[0], MY_POST_PROCESS_FRAME_SAMPLE, TRUE,
                &(music_player->music_status.music_cur_energy),
                &(music_player->music_status.music_cur_energy_rms));
        player_frame_lowpw_deal(music_player);

        energy_calc(music_player->aout_pp.pcm[1], MY_POST_PROCESS_FRAME_SAMPLE, FALSE,
                &(music_player->music_status.mic_cur_energy),
                &(music_player->music_status.mic_cur_energy_rms));

        //������Ч����
        ret = post_process_ops(music_player->audiopp_handle, PP_CMD_FRAME_PROCESS,
                (uint32 ) &music_player->aout_pp);
        if (ret != 0)
        {
            libc_printf_error("post_process_ops frame error:%d\n", ret);
            music_player->music_status.status = MMM_MP_ENGINE_ERROR;
            goto ok_mic_loop_continue;
        }

        //��ȡ��Ƶ������ֵ
        if (g_freq_point_cfg.freq_point_enable == TRUE)
        {
            if (post_process_ops(music_player->audiopp_handle, PP_CMD_GET_FREQ_POINT_ENERGY,
                    (uint32) music_player->music_status.freq_point_energy) < 0)
            {
                libc_memset(music_player->music_status.freq_point_energy, 0x00,
                        sizeof(music_player->music_status.freq_point_energy));
            }
        }

        if (music_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();

            music_player->dae_play_time += (t2 - t1);
        }

        if (music_player->work_mode == MP_WORK_MODE_MIC)
        {
            //ԭ������ֻ�ܵ��������
            if (g_PP_para_info.voice_removal_para.voice_removal_enable == 1)
            {
                int i;
                short *pcm_result = music_player->aout_pp.pcm[0];

                for (i = 0; i < music_player->aout_pp.samples; i++)
                {
                    pcm_result[2*i + 1] = pcm_result[2*i];
                }
            }
        }

        if (loop_send_pcm_to_pcm_buf(music_player, music_player->aout_pp.pcm[0], music_player->aout_pp.samples, \
                &pcm_buf_full_flag, FALSE) == TRUE)
        {
            goto ok_mic_loop_continue;
        }

        if (pcm_buf_full_flag == FALSE)
        {
            if ((sys_get_ab_timer() - tmp_fill_timer) > 1000) //1����û�������ͷ�ʱ��Ƭ����ֹ����
            {
                need_sleep_flag = TRUE;
                libc_printf_warning("fill overtime!\n");
            }
        }
        else
        {
            tmp_fill_timer = sys_get_ab_timer();
        }

        /*
         fadein ����
         */
        if (music_player->music_status.status == MMM_MP_ENGINE_FADEIN)
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32 ) &tmp_pp_status);
#ifdef MUSIC_MMM_DEBUG_INFO
            //libc_printf("fadein:%d\n", tmp_pp_status.fade_in_status);
#endif
            if (tmp_pp_status.fade_in_status == PP_FADE_IN_STANDBY)
            {
                libc_printf_info("fadein over\n");
                music_player->music_status.status = MMM_MP_ENGINE_PLAYING;
            }
        }

        /*
         fadeout ����
         */
        if (music_player->music_status.status == MMM_MP_ENGINE_FADEOUT)
        {
            audiopp_status_t tmp_pp_status;

            post_process_ops(music_player->audiopp_handle, PP_CMD_GET_STATUS, (uint32 ) &tmp_pp_status);
#ifdef MUSIC_MMM_DEBUG_INFO
            //libc_printf("fadeout:%d\n", tmp_pp_status.fade_out_status);
#endif
            if (tmp_pp_status.fade_out_status == PP_FADE_OUT_STANDBY)
            {
                libc_printf_info("fadeout over\n");
                music_player->music_status.status = MMM_MP_ENGINE_PAUSED;
            }
        }

        ok_mic_loop_continue:

        //�����˳����ţ�Ӧ���ý����߳̿�ת����
        if ((music_player->music_status.status == MMM_MP_ENGINE_PLAYEND)
            || (music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
            || (music_player->music_status.status == MMM_MP_ENGINE_ERROR))
        {
            libc_sem_post(music_player->music_dec_sem);
        }

        if ((music_player->ok_mic_wait_decode_first == TRUE)
            || (music_player->music_status.status == MMM_MP_ENGINE_PLAYEND)
            || (music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
            || (music_player->music_status.status == MMM_MP_ENGINE_ERROR)
            || (music_player->is_ok_mic_loop_enable == FALSE)
            || (need_sleep_flag == TRUE))
        {
            sys_os_time_dly(1);
            need_sleep_flag = FALSE;
        }
    }

    ok_mic_loop_exit:

    //���ٽ����ź���
    if (music_player->music_dec_sem != NULL)
    {
        libc_sem_post(music_player->music_dec_sem);
        libc_sem_destroy(&(music_player->music_dec_sem));
        music_player->music_dec_sem = NULL;
    }

    music_player->ok_mic_thread_terminate_flag = FALSE;

    /*ע�⣺libc_pthread_exit�޷����ص�*/
    libc_pthread_exit();

    return (void *) NULL;
}
#endif
