/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ���м��ֹͣͨ������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "speech_player_global.h"
#include "sysdef.h"

void destroy_decode_thread(speech_player_t *speech_player)
{
    if (speech_player == NULL)
    {
        return;
    }

    speech_player->thread_terminate_flag = TRUE;
    while (speech_player->thread_terminate_flag == TRUE)
    {
        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }
}

/* �ȴ��������� */
int32 stop_speech_decoder_wait_pcmbuf_empty(speech_player_t *speech_player)
{
    uint32 fade_out_timeout;

#ifdef SPEECH_MMM_DEBUG_INFO
    libc_printf("wait pcm buf empty\n");
#endif

    fade_out_timeout = sys_get_ab_timer();
    //�ȴ�PCM_BUF�ĵ�����Ƶȫ���������
    while ((speech_player->g_sp_pcm_buf_pending & 0x02) == 0)
    {
        //ͨ�����PCM BUF�в����������ж��Ƿ�Ϊ��
        if (get_pcmbuf_length() == 0)
        {
            break;
        }

        //���500ms���������ˣ�Ӧ���ǳ�������
        if ((sys_get_ab_timer() - fade_out_timeout) > 500)
        {
            libc_printf_warning("fadeout overtime!!\n");
            break;
        }
    }

    //PCM_BUFĩβ������zero�㣬�����´β���������������
    if ((speech_player->g_sp_pcm_buf_pending & 0x02) != 0)
    {
        short zero_samples[4];
        libc_memset(zero_samples, 0x00, sizeof(short)*4);
        send_pcm_to_pcm_buf_by_dac_fifo(zero_samples, 4);
        sys_mdelay(1);
    }

#ifdef SPEECH_MMM_DEBUG_INFO
    libc_printf("pcm buf empty\n");
#endif

    return 0;
}

/* �������ֽ����� */
int32 stop_speech_decoder(speech_player_t *speech_player)
{
    int32 ret = 0;
    bool need_install_hfp = FALSE;

    if ((speech_player->g_p_speech_pre_agc_para->agc_enable == 1)
        || (speech_player->g_p_speech_post_agc_para->agc_enable == 1)
        || (speech_player->g_p_speech_aec_prms->aec_enable == 1))
    {
        need_install_hfp = TRUE;
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    if (speech_player->kt_play_enable == TRUE)
    {
        while ((speech_player->kt_play_enable == TRUE)
            && (speech_player->speech_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
        {
            libc_printf("wait keytone stop!\n");
            keytone_playend_deal(speech_player);
        }
        stop_speech_decoder_wait_pcmbuf_empty(speech_player);
    }
#endif

    //AECʧЧ��ָ����̣���STOP����START���������ٽ����߳�
    if (speech_player->g_speech_reset_buffer_flag == FALSE)
    {
        stop_speech_decoder_wait_pcmbuf_empty(speech_player);

        //�����̲߳�����Ҫ���У����ٽ����߳�
        speech_player->is_player_loop_enable = FALSE;
        destroy_decode_thread(speech_player);
    }

    //ֹͣAUDIO Ӳ������
    mmm_sp_aout_close(speech_player);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
    if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta != MMM_RECORD_STOP))
    {
        ret = stop_speech_record(speech_player);
    }
#endif

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    //�ر��ز������
    if (speech_player->resample_info.enable_resample == TRUE)
    {
        if (speech_player->resample_info.resample_handle_left != NULL)
        {
            pp_resample_close(speech_player->resample_info.resample_handle_left);
            speech_player->resample_info.resample_handle_left = NULL;
        }
    }
#endif

    //�ر���Ч��
    ret = post_process_ops(speech_player->audiopp_handle, PP_CMD_CLOSE, 0);
    libc_memset(&g_PP_para_info, 0x00, sizeof(g_PP_para_info)); //�ر���Ч��Ҫͬʱ��ȫ�ֱ�����λ
    speech_player->audiopp_handle = NULL;
    if (ret != 0)
    {
        libc_printf_warning("PP_CMD_CLOSE fail!!\n");
    }

    //ж��������Ч��
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    if (speech_player->g_p_speech_plc_para->plc_enable == 1)
    {
        ret = hfp_plc_cmd(NULL, HS_CMD_PLC_CLOSE, 0);
        sys_drv_uninstall (PLC_GROUP);
    }

    if (need_install_hfp == TRUE)
    {
        //�ر�����ͨ���㷨��
        ret = hfp_speech_cmd(NULL, HS_CMD_CLOSE, 0);
        speech_player->agc_handle_pre = NULL;
        speech_player->agc_handle_post = NULL;
        speech_player->aec_handle = NULL;
        speech_player->plc_handle = NULL;

        //ж��������Ч��
        sys_drv_uninstall (SPEECH_GROUP);
        libc_printf_info("speech close!\n");
    }

    if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
    {
        //�ͷ�Ԥ������Դ
        if (speech_player->sco_data_predeal_buf != NULL)
        {
            sys_free(speech_player->sco_data_predeal_buf);
            speech_player->sco_data_predeal_buf = NULL;
        }

        //�ر�Ԥ�����
        ret = sp_pre_cmd(speech_player->sp_pre_handle, SP_PRE_CMD_CLOSE, 0);

        //ж��Ԥ�����
        sys_drv_uninstall (SP_PRE_GROUP);
        libc_printf_info("sp_pre close!\n");

        speech_player->sp_pre_buffer_remain = 0;
        speech_player->sco_pkg_flag = TRUE;
    }

    if (speech_player->speech_setting.use_sco_data_postdeal == TRUE)
    {
        //�رպ����
        ret = sp_post_cmd(speech_player->sp_post_handle, SP_POST_CMD_CLOSE, 0);

        //ж�غ����
        sys_drv_uninstall (SP_POST_GROUP);
        libc_printf_info("sp_post close!\n");

        speech_player->sp_post_buffer_remain = 0;
    }

    //�����ź���
    if (speech_player_global->speech_dec_sem != NULL)
    {
        libc_sem_post(speech_player_global->speech_dec_sem);
        libc_sem_destroy(&(speech_player_global->speech_dec_sem));
        speech_player_global->speech_dec_sem = NULL;
    }

    speech_player->g_speech_pcm_buf_int_en_flag = FALSE;
    speech_player_global->g_sp_pcm_buf_pending = 0;
    speech_player->g_speech_pre_enough_flag = FALSE;
    speech_player->start_play_flag = FALSE;
    speech_player->g_speech_need_start = FALSE;

    //AECʧЧ��ָ����̣���STOP����START
    if (speech_player->g_speech_reset_buffer_flag == TRUE)
    {
        //��ʼ��MIC Input Buffer
        speech_player->mic_input_buf.raw_data = (uint8 *) (MIC_INPUT_BUF_ADDR);
        speech_player->mic_input_buf.capacity = MIC_INPUT_BUF_LEN;
        speech_player->mic_input_buf.length = 0;
        speech_player->mic_input_buf.write_ptr = 0;
        speech_player->mic_input_buf.read_ptr = 0;

        //��ʼ��PRE Process Buffer
        speech_player->pre_process_buf.raw_data = (uint8 *) (PRE_PROCESS_BUF_ADDR);
        speech_player->pre_process_buf.capacity = PRE_PROCESS_BUF_LEN;
        speech_player->pre_process_buf.length = 0;
        speech_player->pre_process_buf.write_ptr = 0;
        speech_player->pre_process_buf.read_ptr = 0;
        libc_memcpy(&(speech_player->post_process_buf), &(speech_player->pre_process_buf), sizeof(buffer_rw_t));

        return ret;
    }

    //״̬�޸�Ϊ MMM_BP_ENGINE_STOPPED��STOP���̽���
    speech_player->speech_status.status = MMM_SP_ENGINE_STOPPED;

    return ret;
}

