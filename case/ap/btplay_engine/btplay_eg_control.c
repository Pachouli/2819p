/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�����Ӧ����ѭ������ȡ�м��״̬����Ӧ״̬�仯����ȡӦ����Ϣ����Ӧ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_btplay_engine.h"

void btplay_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))

record_status_e g_last_record_sta = RECORD_STA_STOP;

/*���µ�ǰ¼���ļ���С��vram*/
void btplay_engine_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void btplay_engine_recording_check_status(void)
{
    bool ret ;
    //����¼����Ϣ��vram
    if (g_btplay_info.record_sta == RECORD_STA_RECORDING)
    {
        btplay_engine_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_btplay_info.record_sta != RECORD_STA_RECORDING))
    {
        //��play��pause(¼����ͣʱ����Ҫǿ�Ƹ���ʱ��)
        libc_printf("record pause , update record file now\n");
        btplay_engine_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_btplay_info.record_sta;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_control_block(void)
 * \���湦�ܴ�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
app_result_e btplay_engine_control_block(void)
{
    private_msg_t cur_msg;
    app_result_e mengine_retval = RESULT_NULL;
    bool ret_vals;
    bool need_wait_player_stop = FALSE;
    bool need_app_quit = FALSE;
    app_result_e app_quit_result = RESULT_NULL;
    dev_role_e now_dev_role;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_btplay_data_source == SOURCE_LINEIN_ADC)
    {
        if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK) && (g_p_global_var->play_status_before != BTPLAY_PLAY))
        {
            //��绰ǰ����ͣ״̬����Ҫ _play
            g_filter_by_tws_mode_tdb = TRUE;
        }
        g_p_global_var->bg_app_sta = BG_STATUS_NULL;
    }

    now_dev_role = last_tws_role = get_tws_role();
    if (last_tws_role != NORMAL_DEV)
    {
        //g_filter_by_tws_clear_flag = TRUE;
        //g_filter_by_tws_flag = TRUE; //��ʱ����ֻ�д�绰������Ż�����ӳɹ�״̬����绰��������Ҫ����������
    }
#endif

    //�������AVDTP����
    if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
    {
        com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
    }

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
#endif

    g_btplay_info.status = BTPLAY_STOP;

    while (1)
    {
        //�м��״̬��ѯ������
        check_bp_status:
        if (bp_handle != NULL)
        {
            ret_vals = _get_mmm_status();
            if (ret_vals == TRUE)
            {
                sound_energy_t tmp_sound_energy;

                change_ktp_status(bp_status.ktp_status);

                if ((bp_status.status == MMM_BP_ENGINE_PLAYING)
                    || (bp_status.status == MMM_BP_ENGINE_FADEIN)
                    || (bp_status.status == MMM_BP_ENGINE_FADEOUT)
                    || (bp_status.status == MMM_BP_ENGINE_FADEOUT_PAUSE))
                {
                    tmp_sound_energy.play_cur_energy = bp_status.bt_cur_energy;
                    tmp_sound_energy.play_cur_energy_rms = bp_status.bt_cur_energy_rms;
                    tmp_sound_energy.mic_cur_energy = bp_status.mic_cur_energy;
                    tmp_sound_energy.mic_cur_energy_rms = bp_status.mic_cur_energy_rms;
                    libc_memcpy(tmp_sound_energy.freq_point_energy, bp_status.freq_point_energy, sizeof(tmp_sound_energy.freq_point_energy));
                    com_update_sound_energy(&tmp_sound_energy);
                }
                else
                {
                    //�����������ֵ
                    com_update_sound_energy(NULL);
                }

                //������ȡADC CHANNEL����������ʱ��
                g_btplay_info.adc_lowpw_time = bp_status.adc_energy.adc_lowpw_time;

                g_btplay_info.lowpw_time = bp_status.bt_lowpw_time;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
                if (g_bt_tws_enable == TRUE)
                {
                    //��⵽���䲥�Ž����������˳��м��������ֳ�
                    if ((need_wait_player_stop == TRUE)
                        || ((get_tws_mode() != TWS_MODE_SINGLE) && (get_tws_role() != NORMAL_DEV)
                            && ((bp_status.status == MMM_BP_ENGINE_PAUSE) || (bp_status.status == MMM_BP_ENGINE_ERROR)
                                || (bp_status.lost_pkg_too_more_flag == TRUE))))
                    {
#if (SUPPORT_TWS_EXCPT_RESET == 1)
                        if ((get_tws_role() == TWS_MASTER) && (btplay_engine_is_distance_near() == TRUE))
                        {
                            if (bp_status.status == MMM_BP_ENGINE_ERROR)
                            {
                                if (tws_play_error_count == 0)
                                {
                                    tws_play_first_error_timer = sys_get_ab_timer();
                                }
                                tws_play_error_count++;
                                tws_play_last_error_timer = sys_get_ab_timer();
                                libc_printf_warning("tws_play_error_count++ : %d\n", tws_play_error_count);
                            }

                            if (bp_status.lost_pkg_too_more_flag == TRUE)
                            {
                                if (tws_play_error_count == 0)
                                {
                                    tws_play_first_error_timer = sys_get_ab_timer();
                                }
                                tws_play_error_count++;
                                tws_play_last_error_timer = sys_get_ab_timer();
                                libc_printf_warning("lost_pkg_too_more_flag tws_play_error_count ++ : %d\n", tws_play_error_count);
                            }
                        }
#endif

                        libc_printf_info("btplay engine get_mmm_status pause or error to stop player!\n");
                        if (bp_handle != NULL)
                        {
                            _stop (TRUE);
                            g_btplay_info.status = BTPLAY_STOP;

                            btplay_engine_tws_clear_start();
                        }
                    }
                }
#endif

                if (bp_status.status == MMM_BP_ENGINE_ERROR)
                {
                    //��⵽������ʱû��������״̬����  TODO
                }
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();

            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    //record_module����
                    g_btplay_info.record_sta = RECORD_STA_ERROR;
                }
            }
#endif
        }
        need_wait_player_stop = FALSE;

        com_get_btengine_status(&btplay_bt_info);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
        btplay_engine_recording_check_status();
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO

#if (SUPPORT_TWS_EXCPT_RESET == 1)
#if 0
        if ((phone_rssi_value != btplay_bt_info.phone_rssi.rssi) || (tws_rssi_value != btplay_bt_info.tws_rssi.rssi))
        {
            libc_printf("rssi:%d,%d\n", btplay_bt_info.phone_rssi.rssi, btplay_bt_info.tws_rssi.rssi);

            phone_rssi_value = btplay_bt_info.phone_rssi.rssi;
            tws_rssi_value = btplay_bt_info.tws_rssi.rssi;
        }
#endif

        if (btplay_engine_is_distance_near() == FALSE)
        {
            if (tws_play_error_count > 0)
            {
                libc_printf("rssi weak, tws_play_error_count clear\n");
                tws_play_error_count = 0;
            }
        }

        if ((sys_get_ab_timer() - tws_play_last_error_timer) > CLEAR_ERROR_TIME)
        {
            if (tws_play_error_count > 0)
            {
                libc_printf("stable %d S, tws_play_error_count clear\n", CLEAR_ERROR_TIME/1000);
                tws_play_error_count = 0;
            }
        }
#endif

        if (get_tws_role() == NORMAL_DEV)
        {
            if (g_filter_by_tws_mode_tdb == TRUE)
            {
                g_filter_by_tws_mode_tdb = FALSE;

                //�������AVDTP����
                if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
                {
                    com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
                }
            }
        }

        //����Ӧ��ģʽ��ȷ��������ΪTWS����������
        if (get_tws_mode() == TWS_MODE_SINGLE)
        {
            if ((g_filter_by_tws_forbidden_nest == 0) && (g_filter_by_tws_flag == TRUE))
            {
                g_tws_unlink_delay_unfilter_flag = FALSE;
                g_filter_by_tws_forbidden_nest = 0;
                g_filter_by_tws_clear_flag = FALSE;
                g_filter_by_tws_flag = FALSE;
                g_tws_s_clear_sta = S_CLEAR_NULL;
                g_tws_m_clear_sta = M_CLEAR_NULL;

                if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
                {
                    //�������AVDTP����
                    com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
                }
            }
        }
        //����Ӧ��ģʽ
        else
        {
            now_dev_role = get_tws_role();
            if ((now_dev_role != last_tws_role) || (g_tws_unlink_delay_unfilter_flag == TRUE))
            {
                need_wait_player_stop = btplay_engine_tws_role_change();
            }
            last_tws_role = now_dev_role;

            if (need_wait_player_stop == TRUE)
            {
                goto check_bp_status;
            }

            if (now_dev_role == TWS_MASTER)
            {
                btplay_engine_tws_clear_deal_m();
            }
            else if (now_dev_role == TWS_SLAVE)
            {
                btplay_engine_tws_clear_deal_s();
            }

            if (need_app_quit == TRUE)
            {
                if (g_filter_by_tws_clear_flag == FALSE)
                {
                    mengine_retval = app_quit_result;
                    break;
                }
                else
                {
                    sys_os_time_dly(1);
                    goto check_bp_status;
                }
            }
        }
#endif

        //��������ʱ���������Ƹ��м������������
        if ((bp_handle == NULL) && (g_filter_by_tts_flag == FALSE))
        {
            bool need_start = FALSE;

            if (g_btplay_data_source == SOURCE_BT_A2DP)
            {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
                if ((btplay_bt_info.avdtp_data_flag == TRUE) || (btplay_bt_info.avdtp_data_pending == TRUE))
                {
                    need_start = TRUE;
                }
#else
                if (btplay_bt_info.avdtp_data_flag == TRUE)
                {
                    need_start = TRUE;
                }
#endif
            }
            else
            {
                if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
                {
                    need_start = TRUE;
                }
            }

            if (need_start == TRUE)
            {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
                if (get_mmm_state() == MMM_STATE_KT_PLAYING)
                {
                    com_ok_mic_idle_stop();
                    com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
                }
#endif

                g_btplay_info.status = BTPLAY_PLAY;
                if (_play() == FALSE)
                {
                    libc_printf_error("_btplay_play fail!\n");
                    while (1)
                    {
                        ; //for QAC
                    }
                }
            }
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
            else if ((get_mmm_state() != MMM_STATE_KT_PLAYING)
                    && (btplay_bt_info.avdtp_data_flag == FALSE) && (btplay_bt_info.avdtp_data_pending == FALSE))
            {
                com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
                com_ok_mic_idle_play();
            }
#endif
        }

        if ((bp_handle != NULL) && (g_btplay_data_source == SOURCE_BT_A2DP)
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
            && (btplay_bt_info.avdtp_data_flag == FALSE) && (btplay_bt_info.avdtp_data_pending == FALSE))
#else
            && (btplay_bt_info.avdtp_data_flag == FALSE))
#endif
        {
            _stop (FALSE);
            g_btplay_info.status = BTPLAY_STOP;
        }

        //��ȡ��������ϸ
        if (get_app_msg(THIS_APP_TYPE, &cur_msg) == TRUE)
        {
            switch (cur_msg.msg.type)
            {
                /*��ȡBTPLAY����״̬*/
                case MSG_BTPLAYEG_GET_STATUS_SYNC:
#ifdef ENABLE_TRUE_WIRELESS_STEREO
#if (SUPPORT_TWS_EXCPT_RESET == 1)
                //��������޷�������ʼTWS���裬��Ϊ������δ֪�쳣����ҪRESET
                if ((tws_play_error_count >= REPORT_EXCPT_ERROR_COUNT)
                    && ((sys_get_ab_timer() - tws_play_first_error_timer) >= REPORT_EXCPT_TIME))
                {
                    libc_printf_error("BTPLAY_EXCPT!!\n");
                    g_btplay_info.status = BTPLAY_EXCPT;
                }
#endif
#endif
                libc_memcpy((btplay_info_t *)(cur_msg.msg.content.addr), &g_btplay_info, sizeof(btplay_info_t));
                btplay_engine_reply_msg(&cur_msg, TRUE);
                break;

#if (SUPPORT_SOFT_KEYTONE == 1)
                /*���󲥷�������*/
                case MSG_SOFT_KEYTONE_PLAY_SYNC:
                {
                    bool ret_val = FALSE;
                    if (bp_handle != NULL)
                    {
                        if (mmm_bp_cmd(bp_handle, MMM_BP_KEYTONE_PLAY, (uint8 *)(cur_msg.msg.content.addr)) == 0)
                        {
                            change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                            ret_val = TRUE;
                        }
                    }
                    btplay_engine_reply_msg(&cur_msg, ret_val);
                }
                break;
#endif

                /*���ò������*/
                case MSG_SET_EQ_SYNC: //����eq +eq curve data
                mengine_retval = btplay_engine_eq_set(&cur_msg);
                break;

                /*�����л�����̨Ӧ�ã�ͨ�����������֣�����Ӧ�ñ���״̬���Ա㷵��ʱ�ָ�״̬*/
                case MSG_READY_TO_BG_APP_SYNC:
                //������Ϊ��ͳһ����btplay�ǲ���Ҫ���������־��
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                if (g_btplay_data_source == SOURCE_LINEIN_ADC)
                {
                    if (g_filter_by_tws_mode_tdb == FALSE)
                    {
                        g_p_global_var->play_status_before = BTPLAY_PLAY;
                    }
                    else
                    {
                        g_p_global_var->play_status_before = BTPLAY_STOP;
                    }
                }
                else
#endif
                {
                    g_p_global_var->play_status_before = g_btplay_info.status;
                }
                btplay_engine_reply_msg(&cur_msg, TRUE);
                break;

                case MSG_APP_QUIT: //�˳�app
                //exit_msg_loop();
                mengine_retval = RESULT_APP_QUIT;
                break;

                case MSG_TTS_START_SYNC: //׼������TTS����
                mengine_retval = btplay_engine_tts_start(&cur_msg);
                break;

                case MSG_TTS_STOP_SYNC: //TTS�������
                mengine_retval = btplay_engine_tts_stop(&cur_msg);
                break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
                case MSG_REC_START_SYNC:
                mengine_retval = btplay_engine_record_start(&cur_msg);
                break;

                case MSG_REC_STOP_SYNC:
                mengine_retval = btplay_engine_record_stop(&cur_msg);
                break;
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
                case MSG_BTPLAYEG_TWS_PAUSE_SYNC:
                mengine_retval = btplay_engine_tws_pause(&cur_msg);
                break;

                case MSG_BTPLAYEG_TWS_RESUME_SYNC:
                mengine_retval = btplay_engine_tws_resume(&cur_msg);
                break;

                case MSG_BTPLAYEG_TWS_CONFORM_MODE_SYNC:
                mengine_retval = btplay_engine_tws_conform_mode(&cur_msg);
                break;
#endif

                case MSG_SYSTEM_ENTER_S2_SYNC:
                mengine_retval = btplay_engine_enter_s2(&cur_msg);
                break;

                default:
                libc_printf_warning("btplay engine invalid msg!0x%x\n", cur_msg.msg.type);
                btplay_engine_reply_msg(&cur_msg, FALSE);
                break;
            }
        }

        //�����Ƿ��ڽ�����²���״̬
        if (bp_handle == NULL)
        {
            change_engine_state (ENGINE_STATE_PAUSE);
        }
        else
        {
            if ((g_lowenergy_poweroff_enable == TRUE) && (g_btplay_info.adc_lowpw_time >= LOWENERGY_TIME_THRESHOLD))
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
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if ((get_tws_mode() != TWS_MODE_SINGLE) && (get_tws_role() != NORMAL_DEV))
            {
                g_tws_keep_filter_flag = TRUE;
                need_wait_player_stop = TRUE;
                need_app_quit = TRUE;
                app_quit_result = mengine_retval;
                mengine_retval = RESULT_NULL;
            }
            else
#endif
            {
                break;
            }
        }

        //ESD RESETʵʱ�ϵ㱸��
        g_p_esd_bk_info->play_status = g_btplay_info.status;

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    //�����������ֵ
    com_update_sound_energy(NULL);

    //ֹͣ�����м����ж���м��
    if (bp_handle != NULL)
    {
        _stop (TRUE);
    }

    g_btplay_info.status = BTPLAY_STOP;

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    //����͹���AVDTP����
    com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);

    return mengine_retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_reply_msg(void* msg_ptr, bool ret_vals)
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
void btplay_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
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


//����EQ����
app_result_e btplay_engine_eq_set(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (bp_handle != NULL)
    {
        ret_vals = _eq_set();
    }

    btplay_engine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_start(void* msg_ptr)
 * \��ʼ��������,���û���׼��
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

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //���øýӿں�Ҫ��֤����ʹ�� 0x37000 ���ϵĿռ䣬ֱ������ mengine_tts_stop Ϊֹ
 */
/*******************************************************************************/
app_result_e btplay_engine_tts_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (bp_handle != NULL)
    {
        //��ͣ����
        ret_vals = _stop (TRUE);
        g_btplay_info.status = BTPLAY_STOP;
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif

        //����͹���AVDTP����
        com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
    }

    g_filter_by_tts_flag = TRUE;

    btplay_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_stop(void* msg_ptr)
 * \�����������,�ɻָ�����
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

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //���øýӿڣ���ʾ 0x37000 ���Ͽռ������ȫ��musicӦ��֧��
 */
/*******************************************************************************/
app_result_e btplay_engine_tts_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    g_filter_by_tts_flag = FALSE;

    if ((g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
    {
        //�������AVDTP����
        com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
    }

    //����Ҫ������ָ�_play������ѭ����⵽���ݻ��Զ�_play

    btplay_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

app_result_e btplay_engine_enter_s2(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *) (cur_msg->msg.content.addr);

    //�����ֳ���׼������˯��ģʽ
    libc_printf("engine standby enter\n");

    //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
    btplay_engine_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //��˯��ģʽ�������ָ��ֳ�
    libc_printf("engine standby exit\n");

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_btplay_info.record_sta == RECORD_STA_RECORDING) || (g_btplay_info.record_sta == RECORD_STA_PAUSE))
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
    if (g_btplay_info.record_sta != RECORD_STA_STOP)
    {
        com_record_close_file(FILE_FIRST);
        com_record_disk_exit();
    }

    return TRUE;
}

app_result_e btplay_engine_record_start(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if ((g_btplay_info.status == BTPLAY_STOP) || (g_btplay_info.status == BTPLAY_PAUSE))
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
                g_btplay_info.record_sta = RECORD_STA_RECORDING;
            }
        }
        else
        {
            ret_vals = FALSE;
        }

        if (ret_vals == FALSE)
        {
            g_btplay_info.record_sta = RECORD_STA_ERROR;
        }
    }
    btplay_engine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

app_result_e btplay_engine_record_stop(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if ((g_btplay_info.status == BTPLAY_STOP) || (g_btplay_info.status == BTPLAY_PAUSE))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_stop();
        _record_close_file();
        g_btplay_info.record_sta = RECORD_STA_STOP;
    }

    btplay_engine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

#endif
