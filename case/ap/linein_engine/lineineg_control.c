/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ��������Ӧ����ѭ����Ӧ����Ϣѭ������ȡ�м��״̬����Ӧ�仯��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_lineineg.h"

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
record_status_e g_last_record_sta = RECORD_STA_STOP;

/*���µ�ǰ¼���ļ���С��vram*/
void lineineg_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void lineineg_recording_check_status(void)
{
    //����¼����Ϣ��vram
    if (g_eg_status.record_sta == RECORD_STA_RECORDING)
    {
        lineineg_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_eg_status.record_sta != RECORD_STA_RECORDING))
    {
        //��play��pause(¼����ͣʱ����Ҫǿ�Ƹ���ʱ��)
        libc_printf("record pause, update record file now\n");
        lineineg_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_eg_status.record_sta;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void lineineg_control_block(void)
 * \���湦�ܴ�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
app_result_e lineineg_control_block(void)
{
    private_msg_t cur_msg_val;
    app_result_e mengine_retval = RESULT_NULL;
    bool ret_vals;
    bool no_data_flag = FALSE;
    uint32 no_data_timer = sys_get_ab_timer();

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
                    no_data_flag = FALSE;
                    no_data_timer = sys_get_ab_timer();
                }
                else
                {
                    //�����������ֵ
                    com_update_sound_energy(NULL);
                    no_data_flag = TRUE;
                }

                g_eg_status.lowpw_time = g_pp_status.adc_energy.adc_lowpw_time;

                if (g_pp_status.status == MMM_PP_ENGINE_ERROR)
                {
                    //��⵽������ʱû��������״̬����  TODO
                }
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();
            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    if (g_rm_status.err_no == MMM_MR_ERR_FILE_SIZE_OVER_LIMIT)
                    {
                        //¼���ļ���������¼���ļ�size���Զ�����¼���ļ�������ʼ��һ��¼��
                        libc_printf_info("reach the max size, record next\n");

                        if (lineineg_record_save_restart_do() == FALSE)
                        {
                            g_eg_status.record_sta = RECORD_STA_ERROR;
                        }
                    }
                    else
                    {
                        //record_module����
                        g_eg_status.record_sta = RECORD_STA_ERROR;
                    }
                }
            }
#endif
        }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
        lineineg_recording_check_status();
#endif

        //��ȡ��������ϸ
        if (get_app_msg(THIS_APP_TYPE, &cur_msg_val) == TRUE)
        {
            switch (cur_msg_val.msg.type)
            {
                case MSG_LINEINEG_GET_STATUS_SYNC: //��ȡ��ǰ����״̬
                lineineg_get_status(&cur_msg_val);
                break;

                default:
                mengine_retval = lineineg_message_done_bank(&cur_msg_val);
                break;
            }
        }

        if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
        {
            change_engine_state (ENGINE_STATE_PAUSE);
        }
        else
        {
            bool mute_equal_pause_flag = FALSE;

            if (g_lowenergy_poweroff_enable == TRUE)
            {
                if (g_eg_status.lowpw_time >= LOWENERGY_TIME_THRESHOLD)
                {
                    mute_equal_pause_flag = TRUE;
                }
                if ((no_data_flag == TRUE) && ((sys_get_ab_timer() - no_data_timer) >= LOWENERGY_TIME_THRESHOLD))
                {
                    mute_equal_pause_flag = TRUE;
                }
            }

            if (mute_equal_pause_flag == TRUE)
            {
                change_engine_state (ENGINE_STATE_PAUSE);
            }
            else
            {
                change_engine_state (ENGINE_STATE_PLAYING);
            }
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            //�˳�Ӧ�ùرմ���
            if (g_eg_status.play_status == PlaySta)
            {
                if (_stop() == TRUE)
                {
                    g_eg_status.play_status = PauseSta;
                }
            }
            break;
        }

        //ESD RESET ʵʱ�ϵ㱸��
        g_p_esd_bk_info->play_status = g_eg_status.play_status;

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    //�����������ֵ
    com_update_sound_energy(NULL);

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    return mengine_retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_reply_msg(private_msg_t* msg_ptr, bool ret_vals)
 * \�ظ�ͬ����Ϣ
 * \param[in]    void  msg_ptr ��Ϣ�ṹ��
 * \param[in]    bool  ret_vals ����¼������� TRUE �ɹ� FALSE ʧ��
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR ����
 * \ingroup      mengine_control.c
 * \note  �ظ���Ϣʱ������ret_vals�Ľ��������Ӧ��ɹ���ʧ��
 */
/*******************************************************************************/
void lineineg_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
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
 * \app_result_e lineineg_get_status(void* msg_ptr)
 * \��ȡ��ǰ����״̬
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //ͬ����־��1B
 *                 sem_id_t sem_id;           //�ź���ID��1B
 *                 msg_apps_t msg;          //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //��Ϣ���ͣ�2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   msg_ptr->msg.content.addr //���״̬��Ϣ
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e lineineg_get_status(private_msg_t *msg_ptr)
{
    //copy��ǰ����״̬����Ϣָ��ָ���������
    libc_memcpy(msg_ptr->reply->content, &g_eg_status, sizeof(lineineg_status_t));

    lineineg_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

