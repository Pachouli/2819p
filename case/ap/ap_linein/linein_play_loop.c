/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ����ǰ̨Ӧ�ò��ų�����ѭ������ȡ����״̬����Ӧ״̬�仯����ȡӦ����Ϣ
 *       �Ͱ�����Ϣ����Ӧ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_linein.h"

const key_map_t linein_key_map_list[] =
{
    /*!�̰�play�¼� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_LINEIN_PLAY },
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_TWS_PAIR_LINK },
#endif

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

#if (SUPPORT_RECORD_FUNC == 1)

void reset_rec_cur_time(void)
{
    rec_time_cur = 0;
    rec_time_bak = 0;
    rec_time_display = 0;
}

void _recording_show_time(uint32 show_time)
{
    //�������ʾʱ��
    uint8 minute, second;

    show_time %= 3600; //Сʱ�޷���ʾ��ȥ��
    minute = show_time/60;
    second = show_time%60;

    led_display(NUMBER1, minute/10, TRUE);
    led_display(NUMBER2, minute%10, TRUE);
    led_display_icon(LCD_COL, TRUE);
    led_display(NUMBER3, second/10, TRUE);
    led_display(NUMBER4, second%10, TRUE);
}

void display_cur_rec_time(bool force)
{
    uint32 tmp_time = rec_time_bak + rec_time_cur;
    bool need_display = FALSE;

    if (force == TRUE)
    {
        need_display = TRUE;
    }

    if ((tmp_time/1000) != rec_time_display)
    {
        need_display = TRUE;
        rec_time_display = (tmp_time/1000);
        libc_printf("REC TIME:%d\n", rec_time_display);
    }

    if (need_display == TRUE)
    {
        //�������ʾʱ��
        _recording_show_time(rec_time_display);
    }
}

#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  _scene_linein_sysmsg(void)
 * \linein����ϵͳ��Ϣ��˽����Ϣ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e _scene_linein_sysmsg(void)
{
    //get message
    private_msg_t linein_msg;
    app_result_e msg_result = RESULT_IGNORE;
    if (get_app_msg(THIS_APP_TYPE, &linein_msg) == TRUE)
    {
        msg_result = linein_message_deal(&linein_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_linein_guimsg(msg_gui_t *gui_msg)
 * \����gui��Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_linein_deal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_linein_guimsg(input_gui_msg_t *gui_msg)
{
    msg_apps_type_e cur_event;
    app_result_e key_result = RESULT_NULL;

    if (com_key_mapping(gui_msg, &cur_event, linein_key_map_list) == TRUE)
    {
        //����gui�¼�
        switch (cur_event)
        {
            //play
            case EVENT_LINEIN_PLAY:
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (g_linein_tws_flag == TRUE)
            {
                key_result = btplay_key_play();
            }
            else
#endif
            {
                key_result = linein_key_play();
            }
            break;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
            case EVENT_TWS_PAIR_LINK:
            {
                if (g_linein_tws_enable == TRUE)
                {
                    if (g_linein_bt_info.tws_pair_status != TWS_PAIRED)
                    {
#if (SUPPORT_TWS_CONNECT_WHEN_PHONE_CONNECTED == 0)
                        if ((g_linein_bt_info.status == STATUS_LINKED)
                            || (g_linein_bt_info.status == STATUS_A2DP_PLAY)
                            || (g_linein_bt_info.status == STATUS_A2DP_PAUSE))
                        {
                            //���ֻ��������Ӻ󣬲����������������
                        }
                        else
#endif
                        {
                            com_tts_state_play(TTS_WAIT_BT_PAIR, 0);
                            com_btengine_connect_deal(CON_DEAL_TWS_LINK, TWS_LINK_SRC);
                        }
                    }
                    else
                    {
                        com_btengine_connect_deal(CON_DEAL_TWS_UNLINK, 0);
                    }
                }
            }
            break;

            case EVENT_TWS_SWITCH_POS:
            if (com_get_long_key_time() >= 1000) //1S
            {
                com_filter_key_hold(); //���˺���short up��Ϣ

                if (g_linein_tws_enable == TRUE)
                {
                    tws_dev_spk_pos_switch();
                }
            }
            break;
#endif

#if (SUPPORT_RECORD_FUNC == 1)

            case EVENT_RECORD_PLAY_PAUSE:
            {
                if ((g_linein_play_mode == LINEIN_MODE_MIC_C) || (g_linein_play_mode == LINEIN_MODE_MIC_U))
                {
                    com_filter_key_hold();

                    linein_save_record_restart();
                    reset_rec_cur_time();
                }
                else
                {
                    key_result = com_message_box(cur_event);
                }
            }
            break;

            case EVENT_MODE_SWITCH :
            {
                if (g_linein_play_mode == LINEIN_MODE_MIC_C)
                {
                    key_result = RESULT_ENTER_CARD_REPLAY;
                }
                else if (g_linein_play_mode == LINEIN_MODE_MIC_U)
                {
                    key_result = RESULT_ENTER_UHOST_REPLAY;
                }
                else
                {
                    key_result = com_message_box(cur_event);
                }
            }
            break;
#endif

            default:
            key_result = com_message_box(cur_event);
            break;
        }
    }

    return key_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e linein_key_ignore(void)
 * \�����ڵ�ǰ�����²���Ӧ�İ�����Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_linein_deal.c
 * \note
 */
/*******************************************************************************/
app_result_e linein_key_play(void)
{
    if ((g_linein_status.play_status == PauseSta) || (g_linein_status.play_status == StopSta))
    {
        com_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
        linein_resume();
    }
    else//PlaySta
    {
        //pause
        linein_pause();
        com_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _playing_check_status(void)
 * \�������ȡ��ǰ״̬��Ϣ �ж��Ƿ���� �Ƿ��л�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       app_result_e
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
void _playing_check_status(void)
{
    linein_get_status (&g_linein_status); //��ȡ״̬��Ϣ

    if ((g_last_linein_status == PlaySta) && (g_linein_status.play_status != PlaySta))
    {
        //��play��pause
        com_speaker_off(SPK_PLAY);
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PLAY, FALSE);
        led_display_icon(LCD_PAUSE, TRUE);
    }
    else if ((g_last_linein_status != PlaySta) && (g_linein_status.play_status == PlaySta))
    {
        //��pause��play
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PAUSE, FALSE);
        led_display_icon(LCD_PLAY, TRUE);
    }

    g_last_linein_status = g_linein_status.play_status;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _linein_loop_deal(void)
 * \line in����ѭ������
 * \param[in]    para  para1
 * \param[in]    style para2
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_linein_deal.c
 * \note
 */
/*******************************************************************************/
app_result_e _linein_loop_deal(void)
{
    input_gui_msg_t gui_msg;
    app_result_e ret_vals = RESULT_NULL;
    bool need_redraw = FALSE;
    bool need_replay_after_tts = FALSE;
    record_status_e last_record_sta = RECORD_STA_STOP;

    //ע��Ӧ����Ϣ�ַ�����
    set_app_msg_dispatch_handle(linein_message_deal);

    while (1)
    {
        if (need_redraw == TRUE)
        {
            led_clear_screen(CLR_MAIN);
            if ((g_linein_play_mode == LINEIN_MODE_MIC_C) || (g_linein_play_mode == LINEIN_MODE_MIC_U))
            {
#if (SUPPORT_RECORD_FUNC == 1)
                display_cur_rec_time(TRUE);
#endif
            }
            else if (g_linein_play_mode == LINEIN_MODE_AUX)
            {
                led_display(NUMBER1, 'L', TRUE);
                led_display(NUMBER2, 'I', TRUE);
                led_display(NUMBER3, 'N', TRUE);
                led_display(NUMBER4, 'E', TRUE);
            }
            else if (g_linein_play_mode == LINEIN_MODE_HDMI )
            {
                led_display(NUMBER1, 'H', TRUE);
                led_display(NUMBER2, 'D', TRUE);
                led_display(NUMBER3, 'N', TRUE);
                led_display(NUMBER4, 'I', TRUE);
            }
            else if (g_linein_play_mode == LINEIN_MODE_ARC )
            {
                led_display(NUMBER1, 0, FALSE);
                led_display(NUMBER2, 'A', TRUE);
                led_display(NUMBER3, 'R', TRUE);
                led_display(NUMBER4, 'C', TRUE);
            }
            else
            {
                led_display(NUMBER1, 'S', TRUE);
                led_display(NUMBER2, 'P', TRUE);
                led_display(NUMBER3, 'D', TRUE);
                led_display(NUMBER4, 'F', TRUE);
            }

            need_redraw = FALSE;
        }

        //�������ȡ��ǰ״̬��Ϣ
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (g_linein_tws_flag == TRUE)
        {
            _btplay_playing_check_status();
        }
        else
#endif
        {
            _playing_check_status();

#if (SUPPORT_RECORD_FUNC == 1)
            if ((g_linein_play_mode == LINEIN_MODE_MIC_C) || (g_linein_play_mode == LINEIN_MODE_MIC_U))
            {
                //¼������󷵻�Ĭ��Ӧ��
                if (g_linein_status.record_sta == RECORD_STA_ERROR)
                {
                    ret_vals = RESULT_NEXT_FUNCTION;
                    break;
                }

                if (g_linein_status.record_sta == RECORD_STA_RECORDING)
                {
                    //����ʱ�䵽 rec_time_cur ��ˢ����ʾ
                    rec_time_cur = g_linein_status.rec_time;
                    display_cur_rec_time(FALSE);
                }
            }

            com_record_update_record_status(g_linein_status.record_sta);
            if ((last_record_sta != RECORD_STA_PAUSE) && (g_linein_status.record_sta == RECORD_STA_PAUSE))
            {
                //����ʱ��
                rec_time_bak += rec_time_cur;
                rec_time_cur = 0;
            }
            last_record_sta = g_linein_status.record_sta;
#endif
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (g_linein_tws_enable == TRUE)
        {
            bool tts_play_flag;
            uint16 tts_play_id;

            com_get_btengine_status(&g_linein_bt_info);

            //��������������ɻָ���δ֪�쳣����ҪRESET����ģ��
            if (g_btplay_info.status == BTPLAY_EXCPT)
            {
                //Step 1:ж��������������
                _kill_conflict_engine();
                //Step 2:ж����������
                com_close_bt_engine();
                //Step 3:����װ����������
                com_creat_bt_engine(0);
                //Step 4:��ȡһ������״̬
                com_get_btengine_status(&g_linein_bt_info);
                //Step 5:��װ��LINEIN����
                _create_linein_engine(TRUE);
                need_replay_after_tts = FALSE;
                g_linein_tws_flag = FALSE;
            }

            tts_play_flag = FALSE;

            if ((g_linein_bt_info.tws_pair_status == TWS_PAIRED) && (g_linein_bt_info.dev_role == TWS_MASTER))
            {
                if ((last_phone_status == STATUS_WAIT_PAIRED)
                    && ((g_linein_bt_info.status == STATUS_LINKED)
                        || (g_linein_bt_info.status == STATUS_A2DP_PLAY)
                        || (g_linein_bt_info.status == STATUS_A2DP_PAUSE)))
                {
                    //�������ӳɹ�
                    tts_play_id = TTS_BT_CON_OK;
                    tts_play_flag = TRUE;
                }
                else if (((last_phone_status == STATUS_LINKED)
                        || (last_phone_status == STATUS_A2DP_PLAY)
                        || (last_phone_status == STATUS_A2DP_PAUSE))
                    && (g_linein_bt_info.status == STATUS_WAIT_PAIRED))
                {
                    //�����Ͽ�
                    tts_play_id = TTS_BT_DISCON;
                    tts_play_flag = TRUE;
                }
            }
            last_phone_status = g_linein_bt_info.status; //�����Ƿ���Ӷ�һֱ����

            if (g_linein_status.play_status == PlaySta)
            {
                if (g_linein_tws_flag == TRUE)
                {
                    if (((g_linein_bt_info.tws_pair_status != TWS_PAIRED) && (last_tws_pair_status == TWS_PAIRED))
                        || (tts_play_flag == TRUE))
                    {
                        //��LINEIN����ģʽ�¶Ͽ������������������������ͣ���裬
                        //��������TTS��Ų���ָ�LINEIN�������һ�����������������е��
                        btplay_tws_conform_mode(TRUE);

                        need_replay_after_tts = TRUE;
                    }
                }
                else
                {
                    //NOTE!! ��������������Ҫ����TTS����ô��δ���Ӧ�����ε�
                    #if 1
                    if (((g_linein_bt_info.tws_pair_status == TWS_PAIRED) && (last_tws_pair_status != TWS_PAIRED))
                        && (g_linein_bt_info.dev_role == TWS_MASTER))
                    {
                        //��LINEIN����ģʽ�¸��������������LINEIN������ͣ���裬
                        //��������TTS��Ų���ָ�LINEIN�������һ�����������������е��
                        linein_pause();

                        need_replay_after_tts = TRUE;
                    }
                    #endif
                }
            }

            //�������ʱ�Ქ���ֻ�����/�Ͽ���ʾ��
            if (tts_play_flag == TRUE)
            {
                com_tts_state_play(tts_play_id, 0);
            }

            //ǰ����ͣ���裬���ﲥ��TTS���������ָ����ţ����������̱����������
            if ((tts_play_flag == TRUE) || (tws_tts_display_deal() == TRUE))
            {
                //��ʾ������TTS����������Ϣ�¼�����Ҫ��������״̬
                com_get_btengine_status(&g_linein_bt_info);
            }

            //���䴦�����Ӧ��ģʽ�͵���Ӧ��ģʽ�л�
            if (g_linein_bt_info.dev_role == TWS_MASTER)
            {
                if (get_tws_mode() == TWS_MODE_SINGLE)
                {
                    if (g_p_global_var->slave_btplay_flag == TRUE)
                    {
                        //�л���TWS_MODE_AUX_TWS
                        switch_2_aux_tws:
                        _kill_conflict_engine();
                        com_tws_m2s_switch_tws_mode_cmd(TWS_MODE_AUX_TWS);
                        if (get_tws_mode() == TWS_MODE_AUX_TWS)
                        {
                            _create_btplay_engine();
                            if ((need_replay_after_tts == TRUE) || (g_linein_status.play_status == PlaySta))
                            {
                                btplay_tws_conform_mode(FALSE);
                            }
                            need_replay_after_tts = FALSE;
                            g_linein_tws_flag = TRUE;
                        }
                    }
                    else if ((g_linein_tws_flag == TRUE) || (need_replay_after_tts == TRUE))
                    {
                        goto switch_2_single_mode;
                    }
                }
                else if (get_tws_mode() == TWS_MODE_AUX_TWS)
                {
                    if (g_p_global_var->slave_btplay_flag == FALSE)
                    {
                        //�л���TWS_MODE_SINGLE
                        switch_2_single_mode:
                        btplay_tws_conform_mode(TRUE);
                        _kill_conflict_engine();
                        if (g_linein_bt_info.dev_role == TWS_MASTER)
                        {
                            com_tws_m2s_switch_tws_mode_cmd(TWS_MODE_SINGLE);
                        }
                        if (get_tws_mode() == TWS_MODE_SINGLE)
                        {
                            bool play_flag = FALSE;
                            if ((need_replay_after_tts == TRUE) || (g_linein_status.play_status == PlaySta))
                            {
                                play_flag = TRUE;
                            }
                            _create_linein_engine(play_flag);
                            need_replay_after_tts = FALSE;
                            g_linein_tws_flag = FALSE;
                        }
                    }
                    else if ((g_linein_tws_flag == FALSE) || (need_replay_after_tts == TRUE))
                    {
                        goto switch_2_aux_tws;
                    }
                }
            }
            else
            {
                if (g_linein_tws_flag == TRUE)
                {
                    goto switch_2_single_mode;
                }
            }
        }
#endif

        //��ȡ�ʹ���GUI��Ϣ��˽����Ϣ
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            ret_vals = _scene_linein_guimsg(&gui_msg);
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret_vals = _scene_linein_sysmsg();
        }

        if (ret_vals == RESULT_REDRAW)
        {
            need_redraw = TRUE;
            ret_vals = RESULT_NULL;
        }

        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
        {
            break;
        }
        else
        {
            if (g_linein_play_mode == LINEIN_MODE_AUX)
            {
                if (get_linein_state() != LINEIN_STATE_LINEIN_IN)
                {
                    ret_vals = RESULT_NEXT_FUNCTION;
                    break;
                }
            }
        }

#if (SUPPORT_ASET_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            aset_test_loop_deal();
        }
#endif

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    } //end of while

    return ret_vals;
}

