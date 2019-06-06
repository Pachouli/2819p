/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM����������Ӧ����Ϣѭ��������ǰ̨Ӧ����Ϣ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_fmengine.h"

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))

record_status_e g_last_record_sta = RECORD_STA_STOP;

/*���µ�ǰ¼���ļ���С��vram*/
void fmeg_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void fmeg_recording_check_status(void)
{
    bool ret ;
    //����¼����Ϣ��vram
    if (g_eg_status.record_sta == RECORD_STA_RECORDING)
    {
        fmeg_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_eg_status.record_sta  != RECORD_STA_RECORDING))
    {
        //��play��pause(¼����ͣʱ����Ҫǿ�Ƹ���ʱ��)
        libc_printf("record pause , update record file now\n");
        fmeg_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_eg_status.record_sta ;
}
#endif

void fmengine_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
{
    if (ret_vals == TRUE)
    {
        msg_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        msg_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    libc_sem_post(msg_ptr->sem);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_message_deal(private_msg_t* cur_msg)
 * \������Ϣ����,����������յ���˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       RESULT_APP_QUIT �˳�app
 * \note
 */
/*******************************************************************************/

app_result_e fmengine_message_deal(private_msg_t *cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /* �����������*/

        /* ͬ������Ƶ�㲥��*/
        case MSG_FMENGINE_SETFREQ_SYNC:
        ret_value = fmengine_set_freq_sync(cur_msg);
        break;

        /* ������������*/
        case MSG_FMENGINE_SETMUTE:
        ret_value = fmengine_mute_sync(cur_msg);
        break;

        /* ������̨����*/
        case MSG_FMENGINE_SETTHRED:
        ret_value = fmengine_set_throd(cur_msg);
        break;

        /* ���������̨*/
        case MSG_FMENGINE_SET_SOFTSEEK:
        ret_value = fmengine_set_softseek(cur_msg);
        break;

        /* ֹͣ���� */
        case MSG_FMENGINE_PAUSE_SYNC:
        ret_value = fmengine_pause(cur_msg);
        break;

        /* �������� */
        case MSG_FMENGINE_RESUME_SYNC:
        ret_value = fmengine_resume(cur_msg);
        break;

#if (SUPPORT_SOFT_KEYTONE == 1)
        /*���󲥷�������*/
        case MSG_SOFT_KEYTONE_PLAY_SYNC:
        {
            bool ret_val = FALSE;
            if (pp_handle != NULL)
            {
                if (mmm_pp_cmd(pp_handle, MMM_PP_KEYTONE_PLAY, (uint8 *)(cur_msg->msg.content.addr)) == 0)
                {
                    change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                    ret_val = TRUE;
                }
            }
            fmengine_reply_msg(cur_msg, ret_val);
        }
        break;
#endif

        /*!����������ЧģʽDAE MODE*/
        case MSG_SET_EQ_SYNC:
        ret_value = fmengine_set_eq(cur_msg);
        break;

        /* ��ȡ��Ϣ���*/

        /* ��ȡ��ǰ״̬��Ϣ*/
        case MSG_FMENGINE_GET_FM_STATUS:
        ret_value = fmengine_get_fm_status(cur_msg);
        break;

        case MSG_FMENGINE_GET_PLAY_STATUS:
        ret_value = fmengine_get_play_status(cur_msg);
        break;

        /* ��ȡ��ǰƵ����Ϣ*/
        case MSG_FMENGINE_GET_FREQ:
        ret_value = fmengine_get_freq(cur_msg);
        break;

        /* ģ���ʼ����ж�����*/

        /* ͬ����ʼ��FM  ģ��*/
        case MSG_FMENGINE_INIT:
        ret_value = fm_module_init(cur_msg);
        break;

        /* FM ģ�����Standby */
        case MSG_FMENGINE_STANDBY:
        ret_value = fm_module_standby();
        break;

        /*�����л�����̨Ӧ�ã�ͨ�����������֣�����Ӧ�ñ���״̬���Ա㷵��ʱ�ָ�״̬*/
        case MSG_READY_TO_BG_APP_SYNC:
        g_p_global_var->play_status_before = g_eg_status.play_status;
        fmengine_reply_msg(cur_msg, TRUE);
        break;

        case MSG_APP_QUIT: //�˳�app
        ret_value = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        ret_value = fmengine_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        ret_value = fmengine_tts_stop(cur_msg);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
        case MSG_REC_START_SYNC:
        ret_value = fmengine_record_start(cur_msg);
        break;

        case MSG_REC_STOP_SYNC:
        ret_value = fmengine_record_stop(cur_msg);
        break;
#endif

        case MSG_SYSTEM_ENTER_S2_SYNC:
        ret_value = fmengine_enter_s2(cur_msg);
        break;

        default:
        libc_printf_warning("fm engine invalid msg!0x%x\n", cur_msg->msg.type);
        fmengine_reply_msg(cur_msg, FALSE);
        break;
    }

    return ret_value;
}


/******************************************************************************/
/*!
 * \Description: FM ����Ӧ��Ϊ����ģ�飬Ҳ������Ӧ�ý���
 * \ ��FM ����ͨѶ�Ľӿ�ģ��
 * \app_result_e fmengine_control_block(void)
 * \���湦�ܴ�����
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_control_block(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t fmengine_msg;
    bool ret_vals;

    while (1)
    {
        if (pp_handle != NULL)
        {
            ret_vals = _get_mmm_status();
            if (ret_vals == TRUE)
            {
                sound_energy_t tmp_sound_energy;

                change_ktp_status(g_pp_status.ktp_status);

                if ((g_pp_status.status == MMM_PP_ENGINE_PLAYING)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEIN)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEOUT)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEOUT_PAUSE))
                {
                    tmp_sound_energy.play_cur_energy = g_pp_status.adc_energy.adc_cur_energy;
                    tmp_sound_energy.play_cur_energy_rms = g_pp_status.adc_energy.adc_cur_energy_rms;
                    tmp_sound_energy.mic_cur_energy = g_pp_status.adc_energy.mic_cur_energy;
                    tmp_sound_energy.mic_cur_energy_rms = g_pp_status.adc_energy.mic_cur_energy_rms;
                    libc_memcpy(tmp_sound_energy.freq_point_energy, g_pp_status.freq_point_energy, sizeof(tmp_sound_energy.freq_point_energy));
                    com_update_sound_energy(&tmp_sound_energy);
                }
                else
                {
                    //�����������ֵ
                    com_update_sound_energy(NULL);
                }

                g_eg_status.lowpw_time = g_pp_status.adc_energy.adc_lowpw_time;

                if (g_pp_status.status == MMM_PP_ENGINE_ERROR)
                {
                    //��⵽������ʱû��������״̬����  TODO
                }
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();

            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    //record_module����
                    g_eg_status.record_sta = RECORD_STA_ERROR;
                }
            }
#endif
        }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
        fmeg_recording_check_status();
#endif

        //��ȡ��������Ϣ
        if (get_app_msg(THIS_APP_TYPE, &fmengine_msg) == TRUE)
        {
            result = fmengine_message_deal(&fmengine_msg);
            if (result == RESULT_APP_QUIT)
            {
                break;
            }
        }

        if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
        {
            change_engine_state (ENGINE_STATE_PAUSE);
        }
        else
        {
            change_engine_state (ENGINE_STATE_PLAYING);
        }

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    //�����������ֵ
    com_update_sound_energy(NULL);

    //�˳�Ӧ�ùرմ���
    if (g_eg_status.play_status == PlaySta)
    {
        if (_stop() == TRUE)
        {
            g_eg_status.play_status = PauseSta;
        }
    }

    /*��������Ҳ����FM ���淢MSG_FMENGINE_CLOSE  ��ϢҪ��FM �����˳�*/
    if (g_module_status != MODULE_STANDBY)
    {
        fm_module_standby();
    }

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    return result;
}


//����TTS����ǰ����ͣ����
app_result_e fmengine_tts_start(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    //����ͣ����
    if (g_eg_status.play_status == PlaySta)
    {
        ret_vals = _stop(); //��ͣ����
        if (ret_vals == TRUE)
        {
            g_eg_status.play_status = PauseSta;
            g_stop_by_tts_flag = TRUE;
        }
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif
    }

    fmengine_reply_msg(cur_msg, TRUE);

    return RESULT_NULL;
}

//TTS�����������ָ�����
app_result_e fmengine_tts_stop(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if (g_stop_by_tts_flag == TRUE)
    {
        if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
        {
            ret_vals = _play(); //����
            if (ret_vals == TRUE)
            {
                g_eg_status.play_status = PlaySta;
            }
        }

        g_stop_by_tts_flag = FALSE;
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() != MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_play();
        }
#endif
    }

    fmengine_reply_msg(cur_msg, TRUE);

    return RESULT_NULL;
}

app_result_e fmengine_enter_s2(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *) (cur_msg->msg.content.addr);

    //�����ֳ���׼������˯��ģʽ
    libc_printf("engine standby enter\n");

    fm_close(FM_STANDBY_MODE);

    //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
    fmengine_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //��˯��ģʽ�������ָ��ֳ�
    libc_printf("engine standby exit\n");

    if (fm_module_open_do(&g_init_para_backup) == FALSE)
    {
        libc_printf_warning("fm module reopen fail!!\n");
    }
    else
    {
        fm_set_freq(g_fmengine_status.FM_CurrentFreq);
        libc_printf("exit S2 set FM freq:%d\n", g_fmengine_status.FM_CurrentFreq);

        fm_mute(releaseMUTE);
    }

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_eg_status.record_sta == RECORD_STA_RECORDING) || (g_eg_status.record_sta == RECORD_STA_PAUSE))
    	&& (g_record_param.file_handle != NULL))
    {
        return TRUE; //�Ѿ�����
    }

    //�����ļ�
    common_err_ret = com_record_disk_init(disk);
    if (common_err_ret == REC_FS_ERR_NONE)
    {
        common_err_ret = com_record_create_file(FILE_FIRST, &g_record_param);
    }
    if (common_err_ret != REC_FS_ERR_NONE)
    {
        libc_printf_error("common_err_ret = %d\n", common_err_ret);
        com_record_disk_exit();
        return FALSE;
    }

    return TRUE;
}

bool _record_close_file(void)
{
    if (g_eg_status.record_sta != RECORD_STA_STOP)
    {
        com_record_close_file(FILE_FIRST);
        com_record_disk_exit();
    }

    return TRUE;
}

app_result_e fmengine_record_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_create_file();
        if (ret_vals == TRUE)
        {
            ret_vals = _record_start(1, RECORD_SOURCE_LR_MIX);
            if (ret_vals == TRUE)
            {
                g_eg_status.record_sta = RECORD_STA_RECORDING;
            }
        }
        else
        {
            ret_vals = FALSE;
        }

        if (ret_vals == FALSE)
        {
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }
    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

app_result_e fmengine_record_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_stop();
        _record_close_file();
        g_eg_status.record_sta = RECORD_STA_STOP;
    }

    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

#endif

