/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：公共系统消息和按键消息默认处理接口，应用可以先分发处理消息，这样就不会执行默认处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    gui 快捷事件 和 系统消息处理函数
 * \param[in]    msg_type 待处理的消息（事件）类型
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_NULL 返回空结果，AP可不进行任何处理
 * \retval           RESULT_REDRAW 返回重绘UI，AP一般需要重新 redraw 所有UI
 * \retval           other 直接返回消息（事件）处理的结果，具体参见 app_result_e 定义
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
 例子1：在按键消息（事件）循环中调用，处理快捷按键事件
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
 gui 消息（事件）处理完毕，接下来可以处理应用私有消息了
 }

 例子2：在应用私有消息处理中，处理默认系统消息
 private_msg_t private_msg;
 msg_apps_type_e msg_type
 app_result_e result;

 if(get_app_msg_inner(&private_msg) == TRUE)
 {
 msg_type = private_msg.msg.type;
 switch(msg_type)
 {
 case XXX://处理应用私有消息，或者一些应用需要特殊处理的系统消息
 break;

 default:
 result = com_message_box_inner(msg_type);
 break;
 }
 }
 * \endcode
 * \note
 * \li  com_message_box_inner 是用默认的方式对 gui 快捷事件和系统消息 进行处理。
 * \li  如果用户要对某个 gui 快捷事件或系统消息进行特殊处理，那么应该在 gui 事件分发
 *      或应用私有消息分发的 case 分支中抢先处理掉。
 * \li  tools 子场景下不响应一些切换应用的事件和消息，返回原前台应用响应。
 *******************************************************************************/

app_result_e com_poweroff_by_key(void)
{
    app_result_e result = RESULT_NULL;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
    {
        bool need_pr_master = TRUE;
        bool need_pr_slave = TRUE;

        //只关单箱
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
        /************************** 快捷键处理 ******************************/
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
                com_filter_key_hold_inner(); //过滤后续short up消息
                com_btengine_connect_deal_inner(CON_DEAL_FORCE_DISCONNECT, NULL);
            }
        }
        break;

        case EVENT_SWITCH_CONNECT:
        if (com_get_long_key_time_inner() >= 1000) //1S
        {
            if (get_bt_state_inner() == BT_STATE_WORK)
            {
                com_filter_key_hold_inner(); //过滤后续short up消息
                com_btengine_connect_deal_inner(CON_DEAL_DISC_CONN_SWITCH, NULL);
            }
        }
        break;

        /*! MUTE 开关切换 */
        case EVENT_SWITCH_MUTE:
        if (get_mute_enable() == TRUE)
        {
            //取消静音状态，并重设音量
            set_mute_enable(FALSE);
            com_reset_sound_volume_inner(SET_VOL_BY_SYSTEM);
        }
        else
        {
            //设置音量为0，并置为静音状态
            uint8 volume_backup = com_get_sound_volume_inner();
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, 0);
            set_mute_enable(TRUE);
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, volume_backup);
        }
        break;

        /*! 按下 VOL+ /VOL- /NEXT LONG /PREV LONG进入音量调节界面事件 */
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

        /*! 长按 PLAY 键几秒钟关机事件 */
        case EVENT_ENTER_SHUTOFF_PLAY_SEC:
        if (com_get_long_key_time_inner() >= 2000) //2S
        {
            com_filter_key_hold_inner();

            result = com_poweroff_by_key();
        }
        break;

        /*! 长按 PLAY 键关机事件 */
        case EVENT_ENTER_SHUTOFF_PLAY:
        /*! 短按 POWER 键关机事件 */
        case EVENT_ENTER_SHUTOFF:
        {
            com_filter_key_hold_inner();

            result = com_poweroff_by_key();
        }
        break;

        /*! TTS语言切换事件 */
        case EVENT_TTS_LANGUAGE_SWITCH:
        com_filter_key_hold_inner();
        com_tts_state_play_control_inner(TTS_CTRL_LANGUAGE_SWITCH);
        break;
        /*! TTS开关切换事件 */
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

        //进入录音回放，如果当前正在录音就返回当前录音磁盘回放，否则就优先U盘回放
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
        /*! 话筒优先开关 */
        case EVENT_DUCKING_SWITCH:
        {
            com_set_dae_ducking_switch();
        }
        break;

        /*! 原音消除开关 */
        case EVENT_REMOVAL_SWITCH:
        {
            com_set_dae_voice_removal_switch();
        }
        break;

#endif

        /*! 录音开关 */
        case EVENT_RECORD_SWITCH:
        case EVENT_RECORD_PLAY_PAUSE:
        {
            bool need_record_switch = FALSE;
            com_filter_key_hold();
#if (SUPPORT_RECORD_FUNC == 1)

            //说明：MIC录音比较复杂，在LINEIN前台优先处理

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

        /************************** 系统消息处理 ******************************/
        /* 系统低电消息 */
        case MSG_LOW_POWER:
        {
            result = RESULT_LOW_POWER;
        }
        break;

        /* 系统进入S2消息 */
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
                //提示“电量低，请充电！”
                com_tts_state_play_inner(TTS_BAT_LOW_CHARGE, 0);
            }
        }
        break;

        /*! 关机消息 */
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

        /* 卡插入消息，进入music card播放模式 */
        case MSG_SD_IN:
        clear_fs_vram_info_inner();
        if (g_com_comval.support_cardplay == 1)
        {
            /*卡插入不再清除vm信息 支持插卡断点播放*/
            //set_need_update_vm_inner(1);
            //进入MUSIC应用
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
                //手机USB数据线插入，不要切换到U盘播歌
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

        /* USB线插入消息 */
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

        /* LINEIN线插入 */
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

        //切换到hfp前台
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

            if ((get_cur_app_id() != APP_ID_BTCALL) //防止接收两次MSG_SWITCH_FRONT_APP消息
                && (bt_conflict_flag == FALSE)) //防止切换到不支持蓝牙的应用还发生应用切换
            {
                msg_apps_t send_msg;

                //先发送消息通知即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态
                if (get_engine_type_inner() != ENGINE_NULL)
                {
                    send_msg.type = MSG_READY_TO_BG_APP_SYNC;
                    send_sync_msg_inner(APP_ID_THEENGINE, &send_msg, NULL, 0);
                }

                //切换到打电话应用
                result = RESULT_BLUETEETH_HFP;
            }
        }
        break;

        //闹钟响
        case MSG_RTCALARM:
        {
#if (SUPPORT_TIME_ALARM == 1)
            msg_apps_t send_msg;

            //先发送消息通知即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态
            if (get_engine_type_inner() != ENGINE_NULL)
            {
                send_msg.type = MSG_READY_TO_BG_APP_SYNC;
                send_sync_msg_inner(APP_ID_THEENGINE, &send_msg, NULL, 0);
            }

            //切换到闹钟起闹应用
            result = RESULT_ENTER_ALARM;
#endif
        }
        break;

        //刷新LED数码管数字区
        case MSG_LED_REDRAW:
        {
            result = RESULT_REDRAW;
        }
        break;

#if (SUPPORT_FAST_PREVIEW == 1)
        //快速预览切换应用消息
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
