/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������ϵͳ��Ϣ�Ͱ�����ϢĬ�ϴ���ӿڣ�Ӧ�ÿ����ȷַ�������Ϣ�������Ͳ���ִ��Ĭ�ϴ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    gui ����¼� �� ϵͳ��Ϣ������
 * \param[in]    msg_type ���������Ϣ���¼�������
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_NULL ���ؿս����AP�ɲ������κδ���
 * \retval           RESULT_REDRAW �����ػ�UI��APһ����Ҫ���� redraw ����UI
 * \retval           other ֱ�ӷ�����Ϣ���¼�������Ľ��������μ� app_result_e ����
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
 ����1���ڰ�����Ϣ���¼���ѭ���е��ã������ݰ����¼�
 input_gui_msg_t input_msg;
 msg_apps_type_e gui_event;
 bool ret;

 ret = get_gui_msg_inner(&input_msg);
 if(ret == TRUE)
 {
 if(com_key_mapping_inner(&input_msg, &gui_event, key_map_list) == TRUE)
 {
 switch(gui_event)
 {
 case EVENT_DO_SOMETHING:
 break;

 default:
 result = com_message_box_inner(gui_event);
 break;
 }
 }
 }
 else
 {
 gui ��Ϣ���¼���������ϣ����������Դ���Ӧ��˽����Ϣ��
 }

 ����2����Ӧ��˽����Ϣ�����У�����Ĭ��ϵͳ��Ϣ
 private_msg_t private_msg;
 msg_apps_type_e msg_type
 app_result_e result;

 if(get_app_msg_inner(&private_msg) == TRUE)
 {
 msg_type = private_msg.msg.type;
 switch(msg_type)
 {
 case XXX://����Ӧ��˽����Ϣ������һЩӦ����Ҫ���⴦���ϵͳ��Ϣ
 break;

 default:
 result = com_message_box_inner(msg_type);
 break;
 }
 }
 * \endcode
 * \note
 * \li  com_message_box_inner ����Ĭ�ϵķ�ʽ�� gui ����¼���ϵͳ��Ϣ ���д���
 * \li  ����û�Ҫ��ĳ�� gui ����¼���ϵͳ��Ϣ�������⴦����ôӦ���� gui �¼��ַ�
 *      ��Ӧ��˽����Ϣ�ַ��� case ��֧�����ȴ������
 * \li  tools �ӳ����²���ӦһЩ�л�Ӧ�õ��¼�����Ϣ������ԭǰ̨Ӧ����Ӧ��
 *******************************************************************************/

app_result_e com_poweroff_by_key(void)
{
    app_result_e result = RESULT_NULL;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
    {
        bool need_pr_master = TRUE;
        bool need_pr_slave = TRUE;

        //ֻ�ص���
        if (com_get_config_default_inner(SETTING_ONOFF_POWEROFF_TWS_PR_MODE) == 1)
        {
            if (com_get_input_msg_type() == INPUT_MSG_KEY_SLAVE)
            {
                need_pr_master = FALSE;
            }
            else
            {
                need_pr_slave = FALSE;
            }
        }

        if (need_pr_slave == TRUE)
        {
            com_tws_m2s_poweroff_cmd(TWS_POWEROFF_KEY);
        }

        if (need_pr_master == TRUE)
        {
            result = RESULT_POWER_OFF;
        }
    }
    else
#endif
    {
        result = RESULT_POWER_OFF;
    }

    return result;
}

app_result_e com_message_box_inner(msg_apps_type_e msg_type)
{
    app_result_e result = RESULT_NULL;

    switch (msg_type)
    {
        /************************** ��ݼ����� ******************************/
        case EVENT_MODE_SWITCH:
        {
            uint8 next_func_id;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (com_get_input_msg_type_inner() == INPUT_MSG_KEY_SLAVE)
            {
                if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
                {
                    com_tws_m2s_switch_next_app_cmd();
                }
            }
            else
#endif
            {
                next_func_id = com_ap_switch_ask_next_func_inner(RESULT_NEXT_FUNCTION);
                if (next_func_id == get_cur_func_id())
                {
                    libc_printf_info("NO next APP to switch!!\n");
                    return RESULT_NULL;
                }
                else
                {
                    return RESULT_NEXT_FUNCTION;
                }
            }
        }
        break;

        case EVENT_FORCE_WAITPAIRED:
        if (com_get_long_key_time_inner() >= 1000) //1S
        {
            if (get_bt_state_inner() == BT_STATE_WORK)
            {
                com_filter_key_hold_inner(); //���˺���short up��Ϣ
                com_btengine_connect_deal_inner(CON_DEAL_FORCE_DISCONNECT, NULL);
            }
        }
        break;

        case EVENT_SWITCH_CONNECT:
        if (com_get_long_key_time_inner() >= 1000) //1S
        {
            if (get_bt_state_inner() == BT_STATE_WORK)
            {
                com_filter_key_hold_inner(); //���˺���short up��Ϣ
                com_btengine_connect_deal_inner(CON_DEAL_DISC_CONN_SWITCH, NULL);
            }
        }
        break;

        /*! MUTE �����л� */
        case EVENT_SWITCH_MUTE:
        if (get_mute_enable() == TRUE)
        {
            //ȡ������״̬������������
            set_mute_enable(FALSE);
            com_reset_sound_volume_inner(SET_VOL_BY_SYSTEM);
        }
        else
        {
            //��������Ϊ0������Ϊ����״̬
            uint8 volume_backup = com_get_sound_volume_inner();
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, 0);
            set_mute_enable(TRUE);
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, volume_backup);
        }
        break;

        /*! ���� VOL+ /VOL- /NEXT LONG /PREV LONG�����������ڽ����¼� */
        case EVENT_VOLUME_ADD:
        com_volume_add_inner(SET_VOL_BY_KEYSHORT);
        break;

        case EVENT_VOLUME_ADD_HOLD:
        case EVENT_VOLUME_ADD_NEXT:
        com_volume_add_inner(SET_VOL_BY_KEYHOLD);
        com_filter_key_up_inner();
        break;

        case EVENT_VOLUME_SUB:
        com_volume_sub_inner(SET_VOL_BY_KEYSHORT);
        break;

        case EVENT_VOLUME_SUB_HOLD:
        case EVENT_VOLUME_SUB_PREV:
        com_volume_sub_inner(SET_VOL_BY_KEYHOLD);
        com_filter_key_up_inner();
        break;

        case EVENT_EQ_NEXT:
        #if 0
        com_switch_geq_inner(SETEQ_NEXT, 0, 0);
        #else
        if (g_com_env_var.eq_type == EQ_NOR)
        {
            com_switch_geq_inner(SETEQ_TYPE, EQ_DBB, 0);
        }
        else
        {
            com_switch_geq_inner(SETEQ_TYPE, EQ_NOR, 0);
        }
        #endif
        break;

        /*! ���� PLAY �������ӹػ��¼� */
        case EVENT_ENTER_SHUTOFF_PLAY_SEC:
        if (com_get_long_key_time_inner() >= 2000) //2S
        {
            com_filter_key_hold_inner();

            result = com_poweroff_by_key();
        }
        break;

        /*! ���� PLAY ���ػ��¼� */
        case EVENT_ENTER_SHUTOFF_PLAY:
        /*! �̰� POWER ���ػ��¼� */
        case EVENT_ENTER_SHUTOFF:
        {
            com_filter_key_hold_inner();

            result = com_poweroff_by_key();
        }
        break;

        /*! TTS�����л��¼� */
        case EVENT_TTS_LANGUAGE_SWITCH:
        com_filter_key_hold_inner();
        com_tts_state_play_control_inner(TTS_CTRL_LANGUAGE_SWITCH);
        break;
        /*! TTS�����л��¼� */
        case EVENT_TTS_ENABLE_SWITCH:
        com_tts_state_play_control_inner(TTS_CTRL_ENABLE_SWITCH);
        break;

        case EVENT_ENTER_CARD_REC:
#if (SUPPORT_RECORD_FUNC == 1)
        {
            result = RESULT_ENTER_CARD_REC;
        }
#endif
        break;

        case EVENT_ENTER_UHOST_REC:
#if (SUPPORT_RECORD_FUNC == 1)
        {
            result = RESULT_ENTER_UHOST_REC;
        }
#endif
        break;

        case EVENT_ENTER_RTC_TIME:
        if (com_get_long_key_time_inner() >= 1000) //1S
        {
            com_filter_key_hold_inner();
#if (SUPPORT_TIME_ALARM == 1)
            if ((get_cur_func_id() != APP_FUNC_UDISK) && (get_cur_func_id() != APP_FUNC_BTCALL))
            {
                result = com_rtc_time_entry_inner();
            }
#endif
        }
        break;

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
        case EVENT_ENTER_MENU:
        {
            result = ui_set_contrast();
        }
        break;
#endif

        case EVENT_SIRI_START:
        com_filter_key_hold_inner();
        com_btengine_hfp_siri_start();
        break;

        //����¼���طţ������ǰ����¼���ͷ��ص�ǰ¼�����̻طţ����������U�̻ط�
        case EVENT_ENTER_REPLAY:
        if (get_uhost_state_inner() == UHOST_IN)
        {
            result = RESULT_ENTER_UHOST_REPLAY;
        }
        else if (get_card_state_inner() == CARD_STATE_CARD_IN)
        {
            result = RESULT_ENTER_CARD_REPLAY;
        }
        break;

#if (SUPPORT_OK_MIC == 1)
        /*! ��Ͳ���ȿ��� */
        case EVENT_DUCKING_SWITCH:
        {
            com_set_dae_ducking_switch();
        }
        break;

        /*! ԭ���������� */
        case EVENT_REMOVAL_SWITCH:
        {
            com_set_dae_voice_removal_switch();
        }
        break;

#endif

        /*! ¼������ */
        case EVENT_RECORD_SWITCH:
        case EVENT_RECORD_PLAY_PAUSE:
        {
            bool need_record_switch = FALSE;
            com_filter_key_hold();
#if (SUPPORT_RECORD_FUNC == 1)

            //˵����MIC¼���Ƚϸ��ӣ���LINEINǰ̨���ȴ���

#if (SUPPORT_BTPLAY_RECORD_FUNC == 1)
            if (get_cur_func_id() == APP_FUNC_BTPLAY)
            {
                need_record_switch = TRUE;
            }
#endif

#if (SUPPORT_BTCALL_RECORD_FUNC == 1)
            if (get_cur_func_id() == APP_FUNC_BTCALL)
            {
                need_record_switch = TRUE;
            }
#endif

#if (SUPPORT_LINEIN_RECORD_FUNC == 1)
            if (get_cur_func_id() == APP_FUNC_PLAYLINEIN)
            {
                need_record_switch = TRUE;
            }
#endif

#if (SUPPORT_RADIO_RECORD_FUNC == 1)
            if (get_cur_func_id() == APP_FUNC_RADIO)
            {
                need_record_switch = TRUE;
            }
#endif

#if (SUPPORT_USOUND_RECORD_FUNC == 1)
            if (get_cur_func_id() == APP_FUNC_USOUND)
            {
                need_record_switch = TRUE;
            }
#endif

            if (need_record_switch == TRUE)
            {
                com_record_switch_record();
            }
#endif
        }
        break;

#if (SUPPORT_HDMI_MODULE == 1)
        case MSG_HDMI_VOL_ADD:
        if (com_hdmi_is_hdmi_func(get_cur_func_id()) == TRUE)
        {
            com_volume_add_inner(SET_VOL_BY_KEYSHORT);
        }
        break;

        case MSG_HDMI_VOL_SUB:
        if (com_hdmi_is_hdmi_func(get_cur_func_id()) == TRUE)
        {
            com_volume_sub_inner(SET_VOL_BY_KEYSHORT);
        }
        break;

        case MSG_HDMI_ARC_IN:
        {
            if (get_cur_func_id() != APP_FUNC_HDMI_ARC)
            {
#if (SUPPORT_FAST_PREVIEW == 1)
                com_fast_preview_next_app_inner(RESULT_ENTER_HDMI_ARC);
#else
                result = RESULT_ENTER_HDMI_ARC;
#endif
            }
        }
        break;

        //HDMI 0/HDMI IN
        case MSG_HDMI_HDMI0_IN:
        {
            if (get_cur_func_id() != APP_FUNC_HDMI_HDMI)
            {
#if (SUPPORT_FAST_PREVIEW == 1)
                com_fast_preview_next_app_inner(RESULT_ENTER_HDMI);
#else
                result = RESULT_ENTER_HDMI;
#endif
            }
        }
        break;

        case MSG_HDMI_HDMI1_IN:
        {
            if (get_cur_func_id() != APP_FUNC_HDMI_HDMI1)
            {
#if (SUPPORT_FAST_PREVIEW == 1)
                com_fast_preview_next_app_inner(RESULT_ENTER_HDMI1);
#else
                result = RESULT_ENTER_HDMI1;
#endif
            }
        }
        break;

        case MSG_HDMI_HDMI2_IN:
        {
            if (get_cur_func_id() != APP_FUNC_HDMI_HDMI2)
            {
#if (SUPPORT_FAST_PREVIEW == 1)
                com_fast_preview_next_app_inner(RESULT_ENTER_HDMI2);
#else
                result = RESULT_ENTER_HDMI2;
#endif
            }
        }
        break;

        case MSG_HDMI_MUTE_ON:
        com_hdmi_cec_mute(TRUE, TRUE);
        break;
        case MSG_HDMI_MUTE_OFF:
        com_hdmi_cec_mute(TRUE, FALSE);
        break;

        case MSG_HDMI_MUTE_ON_S:
        com_hdmi_cec_mute(FALSE, TRUE);
        break;
        case MSG_HDMI_MUTE_OFF_S:
        com_hdmi_cec_mute(FALSE, FALSE);
        break;
#endif

        /************************** ϵͳ��Ϣ���� ******************************/
        /* ϵͳ�͵���Ϣ */
        case MSG_LOW_POWER:
        {
            result = RESULT_LOW_POWER;
        }
        break;

        /* ϵͳ����S2��Ϣ */
        case MSG_SYSTEM_ENTER_S2:
        {
            result = com_standby();
        }
        break;

        case MSG_LOW_BATTERY:
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_SLAVE))
            {
                com_tws_s2m_tts_play_req_inner(TTS_BAT_LOW_CHARGE);
            }
            else
#endif
            {
                //��ʾ�������ͣ����磡��
                com_tts_state_play_inner(TTS_BAT_LOW_CHARGE, 0);
            }
        }
        break;

        /*! �ػ���Ϣ */
        case MSG_POWER_OFF:
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
            {
                com_tws_m2s_poweroff_cmd(TWS_POWEROFF_SYS);
            }
#endif

            result = RESULT_POWER_OFF;
        }
        break;

        /* ��������Ϣ������music card����ģʽ */
        case MSG_SD_IN:
        clear_fs_vram_info_inner();
        if (g_com_comval.support_cardplay == 1)
        {
            /*�����벻�����vm��Ϣ ֧�ֲ忨�ϵ㲥��*/
            //set_need_update_vm_inner(1);
            //����MUSICӦ��
#if (SUPPORT_FAST_PREVIEW == 1)
            com_fast_preview_next_app_inner(RESULT_MUSIC_CPLAY);
#else
            result = RESULT_MUSIC_CPLAY;
#endif
        }
        break;

        case MSG_UH_IN:
        clear_fs_vram_info_inner();
        if (g_com_comval.support_uhostplay == 1)
        {
#if (SUPPORT_PHONE_CHARGING == 1)
            if ((g_com_comval.support_iphonecharge == 1) && (get_uhost_type_inner() != UDEVICE_TYPE))
            {
                //�ֻ�USB�����߲��룬��Ҫ�л���U�̲���
            }
            else
#endif
            {
#if (SUPPORT_FAST_PREVIEW == 1)
                com_fast_preview_next_app_inner(RESULT_MUSIC_UPLAY);
#else
                result = RESULT_MUSIC_UPLAY;
#endif
            }
        }
        break;

        /* USB�߲�����Ϣ */
        case MSG_USB_STICK:
        if (g_com_comval.support_usbbox == 1)
        {
#if (SUPPORT_FAST_PREVIEW == 1)
            com_fast_preview_next_app_inner(RESULT_USB_SOUND);
#else
            result = RESULT_USB_SOUND;
#endif
        }
        break;

        /* LINEIN�߲��� */
        case MSG_LINEIN_IN:
        if (g_com_comval.support_linein == 1)
        {
#if (SUPPORT_FAST_PREVIEW == 1)
            com_fast_preview_next_app_inner(RESULT_ENTER_LINEIN);
#else
            result = RESULT_ENTER_LINEIN;
#endif
        }
        break;

        //�л���hfpǰ̨
        case MSG_SWITCH_FRONT_APP:
        {
            bool bt_conflict_flag = FALSE;

            if (g_com_comval.support_bt_inback == 1)
            {
                if ((get_cur_app_id() == APP_ID_USOUND)
                    || (get_cur_app_id() == APP_ID_CONFIG))
                {
                    bt_conflict_flag = TRUE;
                }
            }
            else
            {
                if ((get_cur_app_id() != APP_ID_BTPLAY) && (get_cur_app_id() != APP_ID_BTCALL))
                {
                    bt_conflict_flag = TRUE;
                }
            }

            if ((get_cur_app_id() != APP_ID_BTCALL) //��ֹ��������MSG_SWITCH_FRONT_APP��Ϣ
                && (bt_conflict_flag == FALSE)) //��ֹ�л�����֧��������Ӧ�û�����Ӧ���л�
            {
                msg_apps_t send_msg;

                //�ȷ�����Ϣ֪ͨ�����л�����̨Ӧ�ã�ͨ�����������֣�����Ӧ�ñ���״̬���Ա㷵��ʱ�ָ�״̬
                if (get_engine_type_inner() != ENGINE_NULL)
                {
                    send_msg.type = MSG_READY_TO_BG_APP_SYNC;
                    send_sync_msg_inner(APP_ID_THEENGINE, &send_msg, NULL, 0);
                }

                //�л�����绰Ӧ��
                result = RESULT_BLUETEETH_HFP;
            }
        }
        break;

        //������
        case MSG_RTCALARM:
        {
#if (SUPPORT_TIME_ALARM == 1)
            msg_apps_t send_msg;

            //�ȷ�����Ϣ֪ͨ�����л�����̨Ӧ�ã�ͨ�����������֣�����Ӧ�ñ���״̬���Ա㷵��ʱ�ָ�״̬
            if (get_engine_type_inner() != ENGINE_NULL)
            {
                send_msg.type = MSG_READY_TO_BG_APP_SYNC;
                send_sync_msg_inner(APP_ID_THEENGINE, &send_msg, NULL, 0);
            }

            //�л�����������Ӧ��
            result = RESULT_ENTER_ALARM;
#endif
        }
        break;

        //ˢ��LED�����������
        case MSG_LED_REDRAW:
        {
            result = RESULT_REDRAW;
        }
        break;

#if (SUPPORT_FAST_PREVIEW == 1)
        //����Ԥ���л�Ӧ����Ϣ
        case MSG_FAST_PREVIEW_SWITCH_AP:
        {
            result = RESULT_FAST_PREVIEW_SWITCH_AP;
        }
        break;
#endif

        default:
        break;
    }

    return result;
}

/*! \endcond */
