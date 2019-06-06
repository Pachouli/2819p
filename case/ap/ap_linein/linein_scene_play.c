/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ����ǰ̨Ӧ�ò��ų�����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_linein.h"
#include "fm_interface.h"

void _kill_conflict_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //�鿴��ǰ�����Ƿ�Ϊmusic
    engine_type = get_engine_type();

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (engine_type == ENGINE_BTPLAY)
    {
        g_btplay_info.status = BTPLAY_STOP;
        btplay_status_count = 0;
    }
#endif

    if (engine_type != ENGINE_NULL)
    {
        //ע��radio��̨
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

bool _create_linein_engine(bool play)
{
    msg_apps_t msg;
    //����ֵ
    msg_reply_t temp_reply;
    bool ret_val = TRUE;

    //��Ϣ����
    msg.para[0] = APP_ID_LINEIN_EG;
    if (g_linein_play_mode == LINEIN_MODE_AUX)
    {
        msg.para[1] = play | (ENTER_AUX_CHAN<<4);
    }
#if (SUPPORT_RECORD_FUNC == 1)
    else if(g_linein_play_mode == LINEIN_MODE_MIC_C)
    {
        msg.para[1] = play | (ENTER_MIC_CHAN_C<<4);
    }
    else if(g_linein_play_mode == LINEIN_MODE_MIC_U)
    {
        msg.para[1] = play | (ENTER_MIC_CHAN_U<<4);
    }
#endif
    else if (g_linein_play_mode == LINEIN_MODE_SPDIF)
    {
        msg.para[1] = play | (ENTER_SPDIF_CHAN<<4);
    }
    else if (g_linein_play_mode == LINEIN_MODE_HDMI)
    {
        msg.para[1] = play | (ENTER_HDMI_CHAN<<4);
    }
    else if (g_linein_play_mode == LINEIN_MODE_ARC)
    {
        msg.para[1] = play | (ENTER_ARC_CHAN<<4);
    }
    else
    {
        msg.para[1] = play | (ENTER_I2SRX_CHAN<<4);
    }
    msg.content.addr = NULL;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    ret_val = send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0);

    return ret_val;
}

bool _create_btplay_engine(void)
{
    msg_apps_t msg;
    bool ret_val = TRUE;
    btplay_data_sub_source_e tmp_sub_source;

    if (g_linein_play_mode == LINEIN_MODE_SPDIF)
    {
        tmp_sub_source = SUB_SOURCE_SPDIF;
    }
    else if (g_linein_play_mode == LINEIN_MODE_I2SRX)
    {
        tmp_sub_source = SUB_SOURCE_I2SRX;
    }
    else if (g_linein_play_mode == LINEIN_MODE_HDMI)
    {
        tmp_sub_source = SUB_SOURCE_HDMI;
    }
    else
    {
        tmp_sub_source = SUB_SOURCE_AUX;
    }

    //����Ϣ����
    msg.para[0] = APP_ID_BTPLAY_EG;
    msg.para[1] = (SOURCE_LINEIN_ADC<<4) | (tmp_sub_source<<1) | TRUE;
    msg.content.addr = NULL;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    ret_val = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return ret_val;
}

//����Ԥ��ģʽ��ʹ�øýӿ���Ԥ��������Ӳ������ com_fast_preview_ui.c ��
void linein_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'L', TRUE);
    led_display(NUMBER2, 'I', TRUE);
    led_display(NUMBER3, 'N', TRUE);
    led_display(NUMBER4, 'E', TRUE);
    led_display_icon(LCD_AUX, TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}
void spdif_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'S', TRUE);
    led_display(NUMBER2, 'P', TRUE);
    led_display(NUMBER3, 'D', TRUE);
    led_display(NUMBER4, 'F', TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}
void hdmi_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'H', TRUE);
    led_display(NUMBER2, 'D', TRUE);
    led_display(NUMBER3, 'N', TRUE);
    led_display(NUMBER4, 'I', TRUE);

    led_display_icon(LCD_PAUSE, TRUE);
}
void arc_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 0, FALSE);
    led_display(NUMBER2, 'A', TRUE);
    led_display(NUMBER3, 'R', TRUE);
    led_display(NUMBER4, 'C', TRUE);

    led_display_icon(LCD_PAUSE, TRUE);
}
void record_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 0, FALSE);
    led_display(NUMBER2, 'R', TRUE);
    led_display(NUMBER3, 'E', TRUE);
    led_display(NUMBER4, 'C', TRUE);

    if (g_linein_play_mode == LINEIN_MODE_MIC_C)
    {
        led_display_icon(LCD_SD, TRUE);
    }
    else
    {
        led_display_icon(LCD_USB, TRUE);
    }
    led_display_icon(LCD_PAUSE, TRUE);
}

void _scene_linein_init_tts(void)
{
    //ESD RESET ��Ҫ����TTS
    if ((g_p_global_var->bg_app_sta != BG_STATUS_BACK) && (g_p_esd_bk_info->reset_flag == 0))
    {
        //������ʾ
        if (g_linein_play_mode == LINEIN_MODE_AUX)
        {
            com_tts_state_play(TTS_PLAY_LINEIN, 0);
        }
#if (SUPPORT_RECORD_FUNC == 1)
        else if (g_linein_play_mode == LINEIN_MODE_MIC_C)
        {
            //com_tts_state_play(TTS_CARD_RECORD, 0);
            com_tts_state_play(TTS_RECORD, 0);
        }
        else if (g_linein_play_mode == LINEIN_MODE_MIC_U)
        {
            //com_tts_state_play(TTS_UHOST_RECORD, 0);
            com_tts_state_play(TTS_RECORD, 0);
        }
#endif
        else if (g_linein_play_mode == LINEIN_MODE_SPDIF)
        {
            com_tts_state_play(TTS_PLAY_SPDIF, 0);
        }
        else if (g_linein_play_mode == LINEIN_MODE_HDMI || g_linein_play_mode == LINEIN_MODE_ARC)
        {
            com_tts_state_play(TTS_PLAY_HDMI, 0);
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_linein_init(void)
 * \linein������ʼ��
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      scene_linein_play.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_linein_init(void)
{
    bool ret_val = TRUE;
    bool need_switch_ap = FALSE;
    bool need_flush_key = FALSE;
    app_result_e result = RESULT_NULL;

    //ɱ����ͻ����
    _kill_conflict_engine();

    com_set_mute (FALSE); //�������

    if (g_linein_play_mode == LINEIN_MODE_AUX)
    {
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            linein_show_welcome_ui();
        }
    }
#if (SUPPORT_RECORD_FUNC == 1)
    else if ((g_linein_play_mode == LINEIN_MODE_MIC_C) || (g_linein_play_mode == LINEIN_MODE_MIC_U))
    {
        record_show_welcome_ui();
    }
#endif
    else if (g_linein_play_mode == LINEIN_MODE_SPDIF)
    {
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            spdif_show_welcome_ui();
        }
    }
    else if (g_linein_play_mode == LINEIN_MODE_HDMI)
    {
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            hdmi_show_welcome_ui();
        }
    }
    else if (g_linein_play_mode == LINEIN_MODE_ARC)
    {
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            arc_show_welcome_ui();
        }
    }

    g_p_global_var->fp_switch_jump_welcome = FALSE;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((g_linein_tws_enable == TRUE)
        && (g_p_global_var->bg_app_sta != BG_STATUS_BACK))
    {
        tws_dev_reconn_phone_policy_t param;
        param.cancel_cur_create_connection = 1;
        param.tws_paired_src_reconn_phone_enable = 0;
        com_btengine_connect_deal(CON_DEAL_SET_TWS_RECONN_PHONE_POLICY, &param);
    }
#endif

#if (SUPPORT_FAST_PREVIEW == 1)

    if (get_bt_state() == BT_STATE_WORK)
    {
        //�����֧�ֺ�̨���������з�����Ӧ�ö��� bt enging ��ͻ����Ҫɱ�� bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

    //�ȴ�����Ԥ��ģʽ�Ƿ��л�������Ӧ�ã�������л��Ļ�����������ת
    while (1)
    {
        fast_preview_status_e fp_sta;
        fp_sta = com_fast_preview_get_status();
        if (fp_sta != FAST_PREVIEW_STA_NULL)
        {
            if (fp_sta == FAST_PREVIEW_STA_RUNNING)
            {
                need_flush_key = TRUE;
            }
            sys_os_time_dly(1);
        }
        else
        {
            private_msg_t linein_msg;
            input_gui_msg_t input_msg;
            uint8 i;

            if (need_flush_key == TRUE)
            {
                while (get_gui_msg(&input_msg) == TRUE)
                {
                    ; //flush key msg
                }
            }

            for (i = 0; i < 5; i++)
            {
                if (get_app_msg(THIS_APP_TYPE, &linein_msg) == TRUE)
                {
                    result = linein_message_deal(&linein_msg);
                    if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
                    {
                        need_switch_ap = TRUE;
                        break;
                    }
                }
                else
                {
                    break;
                }

                sys_os_time_dly(1);
            }

            break;
        }
    }

    if (need_switch_ap == TRUE)
    {
        goto fast_switch_ap_exit;
    }

    _scene_linein_init_tts();

#else

    _scene_linein_init_tts();

    if (get_bt_state() == BT_STATE_WORK)
    {
        //�����֧�ֺ�̨���������з�����Ӧ�ö��� bt enging ��ͻ����Ҫɱ�� bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

#endif

    //AUX��FMʹ����ͬ��AIN�ڣ���Ҫȷ��FM�������AUX�ź�
    if (com_get_config_default(LINEIN_INPUT_PIN_USE) == com_get_config_default(RADIO_INPUT_PIN_USE))
    {
        //FM����IDLE״̬��AOUT�迹�ϴ󣬶�AUX��Ƶָ��Ӱ���С
        common_fmdrv_standby(FM_IDLE_MODE);
    }

    //AUX->PAֱͨ��ʽ�����ε���ʼ��ʱ����
#if (SUPPORT_AUX_AA_STEREO == 1)
    if ((uint8) com_get_config_default(LINEIN_CHANNEL_SEL) == AUX_MODE_AA_STEREO)
    {
        com_aux_pa_init();
    }
#endif

    g_linein_tws_flag = FALSE;
    //���������Ҹ����Ѵ���BTPLAY״̬����ô�ͻ��л���LINEIN TWSģʽ������ʹ���LINEIN��ͨģʽ
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_linein_tws_enable == TRUE)
    {
        bool linein_tws_keep_stop = FALSE;

        if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK) && (g_p_global_var->play_status_before != BTPLAY_PLAY))
        {
            linein_tws_keep_stop = TRUE;
        }

        if (get_bt_state() == BT_STATE_NONE)
        {
            com_creat_bt_engine(0);
        }
        com_get_btengine_status(&g_linein_bt_info);
        last_tws_pair_status = g_linein_bt_info.tws_pair_status;
        last_tws_dev_role = g_linein_bt_info.dev_role;
        last_phone_status = g_linein_bt_info.status;

        if ((g_linein_bt_info.dev_role == TWS_MASTER) && (g_p_global_var->slave_btplay_flag == TRUE))
        {
            com_tws_m2s_switch_tws_mode_cmd(TWS_MODE_AUX_TWS);
            if (get_tws_mode() == TWS_MODE_AUX_TWS)
            {
                g_linein_tws_flag = TRUE;
                ret_val = _create_btplay_engine();
                if (linein_tws_keep_stop == TRUE)
                {
                    btplay_tws_conform_mode(TRUE);
                    g_linein_status.play_status = PauseSta;
                }
                else
                {
                    btplay_tws_conform_mode(FALSE);
                    g_linein_status.play_status = PlaySta;
                }
            }
        }
    }
#endif

    if (g_linein_tws_flag == FALSE)
    {
        ret_val = _create_linein_engine(TRUE);
    }

    if (ret_val == TRUE)
    {
        //set volume
        com_reset_sound_volume(SET_VOL_BY_SYSTEM);
    }

    if (ret_val == FALSE)
    {
        result = RESULT_NEXT_FUNCTION;
    }

    fast_switch_ap_exit:

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_linein_exit(void)
 * \�˳�linein����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_linein_exit(void)
{
    //ɱ��LINEIN����
    _kill_conflict_engine();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(void)
 * \linein��������
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      scene_linein_play.c
 * \note
 */
/*******************************************************************************/

app_result_e scene_linein_play(void)
{
    app_result_e ret_vals;
    uint8 drc_mode_backup;

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_LINEIN);

    //������ʼ��
    ret_vals = _scene_linein_init();

    if ((ret_vals == RESULT_NULL) || (ret_vals == RESULT_REDRAW))
    {
#if (SUPPORT_ASET_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            if (g_linein_play_mode == LINEIN_MODE_AUX)
            {
                //����AUXģʽ
                aset_switch_aux_mode(1);
            }
        }
#endif

        if (g_linein_play_mode == LINEIN_MODE_AUX)
        {
            drc_mode_backup = com_switch_drc_mode(DRC_MODE_AUX);
        }

        //����ѭ��
        ret_vals = _linein_loop_deal();

        if (g_linein_play_mode == LINEIN_MODE_AUX)
        {
            com_switch_drc_mode(drc_mode_backup);
        }

#if (SUPPORT_ASET_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            if (g_linein_play_mode == LINEIN_MODE_AUX)
            {
                //�˳�AUXģʽ
                aset_switch_aux_mode(0);
            }
        }
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        //�ǹػ����µ��л�Ӧ��
        if ((ret_vals != RESULT_POWER_OFF) && (ret_vals != RESULT_LOW_POWER))
        {
            if (get_tws_mode() == TWS_MODE_AUX_TWS)
            {
                _kill_conflict_engine();
                com_tws_m2s_switch_tws_mode_cmd(TWS_MODE_SINGLE);
            }
        }
#endif

        //���ó����˳�����
        _scene_linein_exit();
    }

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);

    //ɱ����ͻ����
    _kill_conflict_engine();

    //AUX��FMʹ����ͬ��AIN�ڣ���Ҫȷ��FM�������AUX�ź�
    if (com_get_config_default(LINEIN_INPUT_PIN_USE) == com_get_config_default(RADIO_INPUT_PIN_USE))
    {
        //FM����Standby״̬���Խ��͹���
        common_fmdrv_standby(FM_STANDBY_MODE);
    }

    return ret_vals;
}

