/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ������Ӧ����ѭ������ѯ�м��״̬����Ӧ״̬�仯����ȡӦ����Ϣ����Ӧ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_btcall_engine.h"

void btcall_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))

record_status_e g_last_record_sta = RECORD_STA_STOP;

/*���µ�ǰ¼���ļ���С��vram*/
void btcall_engine_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void btcall_engine_recording_check_status(void)
{
    bool ret ;
    //����¼����Ϣ��vram
    if (g_btcall_info.record_sta == RECORD_STA_RECORDING)
    {
        btcall_engine_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_btcall_info.record_sta != RECORD_STA_RECORDING))
    {
        //��play��pause(¼����ͣʱ����Ҫǿ�Ƹ���ʱ��)
        libc_printf("record pause , update record file now\n");
        btcall_engine_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_btcall_info.record_sta;
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
app_result_e btcall_engine_control_block(void)
{
    private_msg_t cur_msg;
    app_result_e mengine_retval = RESULT_NULL;
    bool ret_vals;
    bool need_set_hfp_connect = FALSE;
    uint8 last_hfp_status;

    g_btcall_info.status = BTCALL_STOP;

    com_get_btengine_status(&btcall_bt_info);
    last_hfp_status = btcall_bt_info.status;
    if (last_hfp_status == STATUS_CALL_HFP)
    {
        need_set_hfp_connect = TRUE;
    }

    while (1)
    {
        //�м��״̬��ѯ������
        if (sp_handle != NULL)
        {
            ret_vals = _get_mmm_status();
            if (ret_vals == TRUE)
            {
                sound_energy_t tmp_sound_energy;
                change_ktp_status(sp_status.ktp_status);

                tmp_sound_energy.play_cur_energy = sp_status.play_cur_energy;
                tmp_sound_energy.play_cur_energy_rms = sp_status.play_cur_energy_rms;
                tmp_sound_energy.mic_cur_energy = sp_status.mic_cur_energy;
                tmp_sound_energy.mic_cur_energy_rms = sp_status.mic_cur_energy_rms;
                com_update_sound_energy(&tmp_sound_energy);

                if (sp_status.status == MMM_SP_ENGINE_ERROR)
                {
                    //��⵽������ʱû��������״̬����  TODO
                }

            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();

            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    //record_module����
                    g_btcall_info.record_sta = RECORD_STA_ERROR;
                }
            }
#endif
        }

        com_get_btengine_status(&btcall_bt_info);

        if ((last_hfp_status != STATUS_CALL_HFP) && (btcall_bt_info.status == STATUS_CALL_HFP))
        {
            need_set_hfp_connect = TRUE;
        }
        last_hfp_status = btcall_bt_info.status;

        if ((sp_handle != NULL) && (need_set_hfp_connect == TRUE))
        {
            if (mmm_sp_cmd(sp_handle, MMM_SP_SET_HFP_CONNECTED, 0) == 0)
            {//���óɹ�
                need_set_hfp_connect = FALSE;
            }
        }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
        btcall_engine_recording_check_status();
#endif

        //��ȡ��������ϸ
        if (get_app_msg(THIS_APP_TYPE, &cur_msg) == TRUE)
        {
            switch (cur_msg.msg.type)
            {
                /*��ȡBTCALL����״̬*/
                case MSG_BTCALLEG_GET_STATUS_SYNC:
                {
                    libc_memcpy((btcall_info_t *)(cur_msg.msg.content.addr), &g_btcall_info, sizeof(btcall_info_t));
                    btcall_engine_reply_msg(&cur_msg, TRUE);
                }
                break;

#if (SUPPORT_SOFT_KEYTONE == 1)
                /*���󲥷�������*/
                case MSG_SOFT_KEYTONE_PLAY_SYNC:
                {
                    bool ret_val = FALSE;
                    if (sp_handle != NULL)
                    {
                        if (mmm_sp_cmd(sp_handle, MMM_SP_KEYTONE_PLAY, (uint8 *)(cur_msg.msg.content.addr)) == 0)
                        {
                            change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                            ret_val = TRUE;
                        }
                    }
                    btcall_engine_reply_msg(&cur_msg, ret_val);
                }
                break;
#endif

                /*���ò������*/
                case MSG_SET_EQ_SYNC: //����eq +eq curve data
                if (sp_handle != NULL)
                {
                    mmm_sp_cmd(sp_handle, MMM_SP_SET_EQ, 0);
                }
                btcall_engine_reply_msg(&cur_msg, TRUE);
                break;

                case MSG_BTCALLEG_SPEECH_PLAY_SYNC:
                {
                    btcall_speech_play_para_t *p_play_para = (btcall_speech_play_para_t *)(cur_msg.msg.content.addr);

                    libc_memcpy(&g_play_para_backup, p_play_para, sizeof(btcall_speech_play_para_t));

                    g_btcall_info.status = BTCALL_PLAY;
                    ret_vals = _play(g_play_para_backup.oncall, g_play_para_backup.att_flag, \
                            g_play_para_backup.p_asqt_config, g_play_para_backup.p_asqt_dump_buf);

                    btcall_engine_reply_msg(&cur_msg, ret_vals);
                }
                break;

                case MSG_BTCALLEG_SPEECH_STOP_SYNC:
                {
                    bool oncall = cur_msg.msg.content.data[0];

                    g_btcall_info.status = BTCALL_STOP;
                    ret_vals = _stop(oncall);
                    need_set_hfp_connect = FALSE;

                    btcall_engine_reply_msg(&cur_msg, ret_vals);
                }
                break;

                case MSG_APP_QUIT: //�˳�app
                //exit_msg_loop();
                mengine_retval = RESULT_APP_QUIT;
                break;

                case MSG_TTS_START_SYNC: //׼������TTS����
                mengine_retval = btcall_engine_tts_start(&cur_msg);
                break;

                case MSG_TTS_STOP_SYNC: //TTS�������
                mengine_retval = btcall_engine_tts_stop(&cur_msg);
                break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
                case MSG_REC_START_SYNC:
                mengine_retval = btcall_engine_record_start(&cur_msg);
                break;

                case MSG_REC_STOP_SYNC:
                mengine_retval = btcall_engine_record_stop(&cur_msg);
                break;
#endif

                default:
                libc_printf_warning("btcall engine invalid msg!0x%x\n", cur_msg.msg.type);
                btcall_engine_reply_msg(&cur_msg, FALSE);
                break;
            }
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            break;
        }

        //ESD RESETʵʱ�ϵ㱸��
        g_p_esd_bk_info->play_status = 0; //��0ȷ�����������������

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    //�����������ֵ
    com_update_sound_energy(NULL);

    g_btcall_info.status = BTCALL_STOP;

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
void btcall_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
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
app_result_e btcall_engine_tts_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (sp_handle != NULL)
    {
        //����ͣ����
        ret_vals = _stop(g_play_para_backup.oncall);
        if (ret_vals == TRUE)
        {
            g_stop_by_tts_flag = TRUE;
        }
    }

    btcall_engine_reply_msg(msg_ptr, ret_vals);
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
app_result_e btcall_engine_tts_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_stop_by_tts_flag == TRUE)
    {
        //�ָ�����
        ret_vals = _play(g_play_para_backup.oncall, g_play_para_backup.att_flag, \
                g_play_para_backup.p_asqt_config, g_play_para_backup.p_asqt_dump_buf);

        g_stop_by_tts_flag = FALSE;
    }

    btcall_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_btcall_info.record_sta == RECORD_STA_RECORDING) || (g_btcall_info.record_sta == RECORD_STA_PAUSE))
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
    if (g_btcall_info.record_sta != RECORD_STA_STOP)
    {
        com_record_close_file(FILE_FIRST);
        com_record_disk_exit();
    }

    return TRUE;
}

app_result_e btcall_engine_record_start(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if (g_btcall_info.status == BTCALL_STOP)
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
                g_btcall_info.record_sta = RECORD_STA_RECORDING;
            }
        }
        else
        {
            ret_vals = FALSE;
        }

        if (ret_vals == FALSE)
        {
            g_btcall_info.record_sta = RECORD_STA_ERROR;
        }
    }
    btcall_engine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

app_result_e btcall_engine_record_stop(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if (g_btcall_info.status == BTCALL_STOP)
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_stop();
        _record_close_file();
        g_btcall_info.record_sta = RECORD_STA_STOP;
    }

    btcall_engine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

#endif
