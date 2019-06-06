/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ��ǰ̨Ӧ����ѭ������ȡ����״̬������״̬����Ӧ״̬�仯����ȡӦ����Ϣ
 *       �Ͱ�����Ϣ����Ӧ��Ĭ������������MP3��ʽ��Ҳ�������ﲥ�š�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

extern app_result_e hfp_change_to_phone(void);

void btcall_get_engine_status(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTCALLEG_GET_STATUS_SYNC;
    msg.content.addr = &g_btcall_info;
    send_sync_msg(APP_ID_BTCALL_EG, &msg, NULL, 0);
}

bool _speech_play(bool oncall)
{
    msg_apps_t msg;
    btcall_speech_play_para_t tmp_btcall_para;

    if (g_speech_play_flag == TRUE)
    {
        return TRUE;
    }

    //��ʱ�ر�DRC
    com_set_drc_onoff(FALSE);

    //�л�Ϊͨ������PEQ����
    if (g_speech_peq_flag == FALSE)
    {
        com_set_speech_peq_para(TRUE);
        g_speech_peq_flag = TRUE;
    }

    tmp_btcall_para.p_asqt_config = NULL;
    tmp_btcall_para.p_asqt_dump_buf = NULL;
    tmp_btcall_para.oncall = oncall;
    tmp_btcall_para.att_flag = FALSE;

    //���ò���
#if (SUPPORT_ASQT_TEST == 1)
    if (com_get_stub_tools_type() == STUB_PC_TOOL_ASQT_MODE)
    {
        tmp_btcall_para.p_asqt_config = &g_asqt_configs;
        tmp_btcall_para.p_asqt_dump_buf = &g_asqt_dump_buf;
    }
#endif

    //��ʼ����
    msg.type = MSG_BTCALLEG_SPEECH_PLAY_SYNC;
    msg.content.addr = &tmp_btcall_para;
    send_sync_msg(APP_ID_BTCALL_EG, &msg, NULL, 0);

    g_speech_play_flag = TRUE;

    return TRUE;
}

bool _speech_stop(bool oncall)
{
    msg_apps_t msg;

    if (g_speech_play_flag == FALSE)
    {
        return TRUE;
    }

    //����Ϣ����
    msg.content.data[0] = oncall;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTCALLEG_SPEECH_STOP_SYNC;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_BTCALL_EG, &msg, NULL, 0);

    //�л��ز��ų���PEQ����
    if (g_speech_peq_flag == TRUE)
    {
        com_set_speech_peq_para(FALSE);
        g_speech_peq_flag = FALSE;
    }

    //��ʱ�ر�DRC-�ָ�DRC����
    com_set_drc_onoff(TRUE);

    g_speech_play_flag = FALSE;

    return TRUE;
}

bool _callring_play(void)
{
    char *cr_mp3_name =
    { "callring.mp3" };

    libc_printf_info("callrin play\n");

    callring_handle = com_sd_mp3_player_play_mp3(cr_mp3_name);
    if (callring_handle == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

bool _callring_stop(void)
{
    libc_printf_info("callrin stop\n");

    com_sd_mp3_player_stop(callring_handle);

    callring_handle = NULL;

    return TRUE;
}

bool _callring_is_over(void)
{
    return com_sd_mp3_player_is_stop(callring_handle);
}

app_result_e bthfp_msg_deal(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    private_msg_t pri_msg;

    if (get_gui_msg(&gui_msg) == TRUE)
    {
        result = deal_gui_bt(&gui_msg);
    }
    else
    {
        if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
        {
            result = bt_message_deal(&pri_msg);
        }
    }

    return result;
}

//ͬ������Զ�˵������������
void update_volume_syn(void)
{
    msg_apps_t msg;
    uint8 tmp_vol;
    uint8 return_vol=0;
    if (g_btcall_bt_info.vol_syn_flag == 0)
    {
        return;
    }
    if (g_btcall_bt_info.vol_syn_flag == 1)
    {
        libc_printf_info("remote_volume:%d\n", g_btcall_bt_info.remote_volume);
        return_vol=g_btcall_bt_info.remote_volume;

        tmp_vol = (g_btcall_bt_info.remote_volume * VOLUME_VALUE_CALL_MAX / HFP_VOLUME_MAX);
        tmp_vol = (tmp_vol > VOLUME_VALUE_CALL_MAX) ? VOLUME_VALUE_CALL_MAX : tmp_vol;
        com_volume_set(SET_VOL_BY_REMOTE, tmp_vol);
    }
    else if (g_btcall_bt_info.vol_syn_flag == 2)
    {
        com_volume_add(SET_VOL_BY_REMOTE);
    }
    else
    {
        com_volume_sub(SET_VOL_BY_REMOTE);
    }
    //����ͬ��������󣬸�֪bt engine�������
    msg.type = MSG_BTENGINE_VOL_UPDATE_FINISH_SYNC;
    msg.content.data[0]=return_vol;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
}

//�����ֻ����͹���������
void play_phone_ring(void)
{
    if (g_cr_status == CR_STATUS_PLAY)
    {
        _callring_stop();
        g_cr_status = CR_STATUS_STOP;
    }
    if (g_speech_play_flag == FALSE)
    {
        ignore_default_ring_flag = FALSE;
        _speech_play (TRUE);
    }
}
//����Ĭ������
void play_default_ring(void)
{
#if (CALLIN_DEFAULT_RING_SUPPORT == 1)

    if (ignore_default_ring_flag == TRUE)
    {
        return;
    }
    if ((g_disp_status == STATUS_CALL_INCOMING) || (g_disp_status == STATUS_CALL_OUTGOING))
    {
        if (g_cr_status == CR_STATUS_STOP)
        {
            if (callring_handle == NULL)
            {
                _callring_play();
                g_cr_status = CR_STATUS_PLAY;
            }
        }
        else if (g_cr_status == CR_STATUS_PLAY)
        {
            if (_callring_is_over() == TRUE)
            {
                _callring_stop();
                g_cr_status = CR_STATUS_BREAK;
                g_cr_timer = sys_get_ab_timer();
            }
        }
        else
        {
            if ((sys_get_ab_timer() - g_cr_timer) > (callring_space_time * 100))
            {
                libc_printf_info("CR_STATUS_STOP\n");
                g_cr_status = CR_STATUS_STOP;
            }
        }
    }

#endif
}

//���ŵ绰������/����
void play_phonebook(void)
{
    if (g_pb_need_init == TRUE)
    {
        //��ȡ�������ݣ���������ƣ������֧�ֵ绰��Э�飬��ֻ�к��룩
        if ((btcall_pb_tts_init() == TRUE) && ((g_pb_result == PB_NUM_VALID) || (g_pb_result == PB_NAME_VALID)))
        {
            g_pb_play_status = PB_STATUS_GET_CONTENT;
            g_pb_need_init = FALSE;
        }
        else
        {
            g_pb_break_timer = sys_get_ab_timer();
            g_pb_play_status = PB_STATUS_BREAK;
        }
    }

    //�ȴ����ʱ��
    if (g_pb_play_status == PB_STATUS_BREAK)
    {
        if ((sys_get_ab_timer() - g_pb_break_timer) >= (callring_pb_space_time * 1000))
        {
            g_pb_play_status = PB_STATUS_GET_CONTENT;
        }
    }

    //��ȡ�������ݣ��Ҳ��ڲ���Ĭ�������������Ϳ��������������/���Ʋ���
    if (g_pb_play_status == PB_STATUS_GET_CONTENT)
    {
        //�Ҷϵ绰���л�������ֻ�
        if (g_speech_play_flag == TRUE)
        {
            _speech_stop (TRUE);
        }

        btcall_pb_tts_play();
        g_pb_play_status = PB_STATUS_PLAY;
    }
    else if (g_pb_play_status == PB_STATUS_PLAY)
    {
        if (btcall_pb_tts_is_over() == TRUE)
        {
            btcall_pb_tts_stop();
            g_pb_break_timer = sys_get_ab_timer();
            g_pb_play_status = PB_STATUS_BREAK;
        }
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    btcall_status_e btcall_last_status;
    bool need_draw = TRUE;
    bool need_free_asqt = FALSE;
    uint32 sco_no_data_timer = sys_get_ab_timer();

    callring_wait_sco_flag = TRUE;
    g_wait_sco_timer = sys_get_ab_timer();
    ignore_default_ring_flag = FALSE;

    //��ȡ������һ��״̬����֤ g_btcall_bt_info ��Ч
    com_get_btengine_status(&g_btcall_bt_info);

    //��ȡ������һ��״̬��ȷ�� g_btcall_info ��Ч
    btcall_get_engine_status();
    btcall_last_status = g_btcall_info.status;

#ifdef bt_auto_test
    get_statis_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 12000, (timer_proc) deal_random_test);
#endif

    //ע��Ӧ����Ϣ�ַ�����
    set_app_msg_dispatch_handle(bt_message_deal);

#if (SUPPORT_ASQT_TEST == 1)
    if (com_get_stub_tools_type() == STUB_PC_TOOL_ASQT_MODE)
    {
        if (sys_shm_creat(SHARE_MEM_ID_SCO_INPIPE_ASQT, &g_sco_in_cache, sizeof(buffer_rw_t)) == -1)
        {
            while (1);
        }
        need_free_asqt = TRUE;
    }
#endif

    while (1)
    {
#if (SUPPORT_ASQT_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASQT_MODE)
        {
            asqt_loop_deal();
        }
        else
#endif
        {
            //����״̬����
            com_get_btengine_status(&g_btcall_bt_info);

            update_volume_syn();
            g_disp_status = g_btcall_bt_info.status;

            if ((g_disp_status != STATUS_CALL_INCOMING)
                && (g_disp_status != STATUS_CALL_OUTGOING)
                && (g_disp_status != STATUS_CALL_HFP)
                && (g_disp_status != STATUS_CALL_PHONE)
                && (g_disp_status != STATUS_SIRI))
            {
                result = RESULT_LASTPLAY;
                break;
            }

            //�ս���ͨ��������Ҫ�ȴ�2���ӣ�֮��������޽���SCO��·�������Ƿ񲥷�������������
            if (callring_wait_sco_flag == TRUE)
            {
                if ((sys_get_ab_timer() - g_wait_sco_timer) >= 2000)
                {
                    callring_wait_sco_flag = FALSE;
                }
            }

            //����״̬��������������
            if ((g_disp_status == STATUS_CALL_INCOMING)
                && (callin_hint_mode != CALLIN_HINT_MODE_ONLY_RING)
                && (g_cr_status == CR_STATUS_STOP))
            {
                play_phonebook();
            }
            else if (g_disp_status != STATUS_CALL_INCOMING)
            {
                if (g_pb_play_status == PB_STATUS_PLAY)
                {
                    btcall_pb_tts_stop();
                    g_pb_play_status = PB_STATUS_STOP;
                    g_pb_need_init = TRUE;
                }
            }

            //�Ѿ�������sco��·
            if (g_btcall_bt_info.sco_created_flag == TRUE)
            {
                if ((callin_hint_mode == CALLIN_HINT_MODE_ONLY_RING)
                    || (g_disp_status != STATUS_CALL_INCOMING)
                    || ((callin_hint_mode != CALLIN_HINT_MODE_ONLY_RING) && (g_pb_play_status == PB_STATUS_BREAK)))
                {
                    play_phone_ring();
                }

                if (((g_disp_status == STATUS_CALL_HFP) || (g_disp_status == STATUS_SIRI))
                    && (g_btcall_bt_info.sco_info.sco_trans_status == 0))
                {
                    if ((sys_get_ab_timer() - sco_no_data_timer) >= 1000)
                    {
                        libc_printf_warning("SCO NO DATA, MAYBE SCO LINK ERROR, RELEASE SCO!\n");

                        if (g_disp_status == STATUS_CALL_HFP)
                        {
                            //���ڽ���ʱ�л���PHONE����
                            libc_printf_info("SCO NO DATA, HFP -> PHONE\n");
                            hfp_change_to_phone();
                        }
                        else if (g_disp_status == STATUS_SIRI)
                        {
                            libc_printf_info("SCO NO DATA, END SIRI\n");
                            hfp_siri_deal(FALSE);
                            sys_os_time_dly(200); //�ȴ�2S��������SIRI
                            hfp_siri_deal(TRUE);
                        }

                        sco_no_data_timer = sys_get_ab_timer();
                    }
                }
                else
                {
                    sco_no_data_timer = sys_get_ab_timer();
                }
            }
            else
            {
                sco_no_data_timer = sys_get_ab_timer();

                //�Ҷϵ绰���л�������ֻ�
                if (g_speech_play_flag == TRUE)
                {
                    _speech_stop (TRUE);

                    //����������ȥ������в����ֻ��������ڽ����绰ʱ����һ����ʱ��sco��·��Ͽ���
                    //�������ʱ��β���ȥ����Ĭ������
                    if ((g_disp_status == STATUS_CALL_INCOMING) || (g_disp_status == STATUS_CALL_OUTGOING))
                    {
                        ignore_default_ring_flag = TRUE;
                    }
                }

                if ((callin_hint_mode == CALLIN_HINT_MODE_ONLY_RING)
                    || ((callin_hint_mode == CALLIN_HINT_MODE_RING_PB) && (g_pb_play_status == PB_STATUS_BREAK)))
                {
                    if (callring_wait_sco_flag == FALSE)
                    {
                        play_default_ring();
                    }
                }
            }

            //����״̬����
            btcall_get_engine_status();
            if (g_btcall_info.status != btcall_last_status)
            {
                btcall_last_status = g_btcall_info.status;
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
            com_record_update_record_status(g_btcall_info.record_sta);
#endif

            if ((need_draw == TRUE) || (g_disp_status != last_disp_status))
            {
                if (need_draw == TRUE)
                {
                    led_clear_screen(CLR_MAIN);
                }
                bt_ui_display();
                last_disp_status = g_disp_status;
                need_draw = FALSE;
            }
        }

        result = bthfp_msg_deal();

        if (result == RESULT_REDRAW)
        {
            result = RESULT_NULL;
            need_draw = TRUE;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        //����10ms�����������
#if (SUPPORT_ASQT_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASQT_MODE)
        {
            ; //ASQTģʽ�²���Ҫ�ͷ�ʱ��Ƭ
        }
        else
#endif
        {
            sys_os_time_dly(1);
        }
    }

#if (SUPPORT_ASQT_TEST == 1)
    if (need_free_asqt == TRUE)
    {
        sys_shm_destroy (SHARE_MEM_ID_SCO_INPIPE_ASQT);
    }
#endif

    if (g_disp_status == STATUS_CALL_HFP)
    {
        //���ڽ���ʱ��next/prev�л���PHONE����
        if (g_chf_cag_exch_enable == 1)
        {
            hfp_change_to_phone();
        }
    }

    if (g_pb_play_status == PB_STATUS_PLAY)
    {
        btcall_pb_tts_stop();
        g_pb_play_status = PB_STATUS_STOP;
        g_pb_need_init = TRUE;
    }

    if (g_cr_status == CR_STATUS_PLAY)
    {
        _callring_stop();
        g_cr_status = CR_STATUS_STOP;
    }

    if (g_speech_play_flag == TRUE)
    {
        _speech_stop (TRUE);
    }
#ifdef bt_auto_test
    if (get_statis_timer_id != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, get_statis_timer_id);
        get_statis_timer_id = -1;
    }
#endif
    return result;
}

