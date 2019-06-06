/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用应用消息和按键消息处理。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

uint8 fast_play_flag = 0;

app_result_e a2dp_play_pause_deal(void);
app_result_e a2dp_next_deal(void);
app_result_e a2dp_prev_deal(void);
app_result_e key_ff_deal(void);
app_result_e key_fb_deal(void);
app_result_e key_bt_open_visible(void);
app_result_e key_ff_cancel_deal(void);
app_result_e key_fb_cancel_deal(void);
app_result_e hfp_outgo_call(void);

const key_map_t bt_tws_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },

    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },

    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FFOREWORD },
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FBACKWORD },

#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
    { { KEY_CONNECT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_OPEN_BT_VISIBLE },
#endif

    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_TWS_PAIR_LINK },
#if ((SUPPORT_RECORD_FUNC == 0) && (SUPPORT_BT_SIRI == 0))
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_HID_TAKE_PHOTO },
#endif

#ifdef DOUBLE_CLICK_KEY_TEST
    { { KEY_F1, 0, KEY_TYPE_DOUBLE_CLICK }, EVENT_CALL_LASTPHONE },
#endif

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

const key_map_t bt_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },

    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },

    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FFOREWORD },
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FBACKWORD },

#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
    { { KEY_CONNECT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_OPEN_BT_VISIBLE },
#endif

    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_CALL_LASTPHONE },
#if ((SUPPORT_RECORD_FUNC == 0) && (SUPPORT_BT_SIRI == 0))
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_HID_TAKE_PHOTO },
#endif

#ifdef DOUBLE_CLICK_KEY_TEST
    { { KEY_F1, 0, KEY_TYPE_DOUBLE_CLICK }, EVENT_CALL_LASTPHONE },
#endif

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

void btplay_send_avrcp_play_pause(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_PLAY_PAUSE_SYNC;
    msg.content.data[0] = 0x00;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
}

void avrcp_tts_state_play(uint8 tts_id, uint16 mode)
{
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_mode() != TWS_MODE_SINGLE) && (get_tws_role() == TWS_MASTER))
    {
        if (com_get_input_msg_type() == INPUT_MSG_KEY_SLAVE)
        {
            com_tts_state_play_tws_slave(tts_id, mode);
        }
        else
        {
            com_tts_state_play(tts_id, mode);
        }
    }
    else
#endif
    {
        com_tts_state_play(tts_id, mode);
    }
}

app_result_e a2dp_play_pause_deal(void)
{
    if ((g_btplay_bt_info.service_connect & AVRCP_CONNECTED_MY) != 0)
    {
        //消息类型(即消息名称)
        //这里只需要发送暂停或播放命令，不需要关心当前的播放状态
        //因为快速切换播放状态时，上层获取的状态有可能更新不及时
        btplay_send_avrcp_play_pause();

        sys_os_time_dly(10); //不要使用sys_mdelay，这种接口在低优先级任务不准确

        //为了播报更准确，在播报之间重新再获取一次引擎的状态
        com_get_btengine_status(&g_btplay_bt_info);

        if (g_btplay_bt_info.status == STATUS_A2DP_PAUSE)
        {
            avrcp_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
        }
        else
        {
            avrcp_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
        }
    }

    return RESULT_NULL;
}

app_result_e a2dp_next_deal(void)
{
    msg_apps_t msg;

    if ((g_btplay_bt_info.service_connect & AVRCP_CONNECTED_MY) != 0)
    {
        //消息类型(即消息名称)
        msg.type = MSG_BTENGINE_PLAY_NEXT_SYNC;
        //发送同步消息
        send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

        sys_os_time_dly(10); //不要使用sys_mdelay，这种接口在低优先级任务不准确

        avrcp_tts_state_play(TTS_NEXT_SONG, TTS_MODE_CONTROL_TTS_YES);
    }

    return RESULT_NULL;
}

app_result_e a2dp_prev_deal(void)
{
    msg_apps_t msg;

    if ((g_btplay_bt_info.service_connect & AVRCP_CONNECTED_MY) != 0)
    {
        //消息类型(即消息名称)
        msg.type = MSG_BTENGINE_PLAY_PREV_SYNC;
        //发送同步消息
        send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

        sys_os_time_dly(10); //不要使用sys_mdelay，这种接口在低优先级任务不准确

        avrcp_tts_state_play(TTS_LAST_SONG, TTS_MODE_CONTROL_TTS_YES);
    }

    return RESULT_NULL;
}

app_result_e key_play_deal(void)
{
    if ((g_disp_status == STATUS_LINKED) || (g_disp_status == STATUS_A2DP_PLAY) || (g_disp_status == STATUS_A2DP_PAUSE))
    {
        return a2dp_play_pause_deal();
    }
    return RESULT_NULL;
}

app_result_e key_next_deal(void)
{
    if ((g_disp_status == STATUS_LINKED) || (g_disp_status == STATUS_A2DP_PLAY) || (g_disp_status == STATUS_A2DP_PAUSE))
    {
        return a2dp_next_deal();
    }

    return RESULT_NULL;
}

app_result_e key_prev_deal(void)
{
    if ((g_disp_status == STATUS_LINKED) || (g_disp_status == STATUS_A2DP_PLAY) || (g_disp_status == STATUS_A2DP_PAUSE))
    {
        return a2dp_prev_deal();
    }

    return RESULT_NULL;
}

app_result_e key_ff_deal(void)
{
    msg_apps_t msg;
    if (g_disp_status == STATUS_A2DP_PLAY)
    {
        if (fast_play_flag == 0)
        {
            //消息类型(即消息名称)
            msg.type = MSG_BTENGINE_FFWD_SYNC;
            //发送同步消息
            send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
            fast_play_flag = 1;
        }
    }
    //暂停状态下，过滤快进退消息
    else
    {
        com_filter_key_hold(); //过滤后续short up消息
    }

    return RESULT_NULL;
}

app_result_e key_fb_deal(void)
{
    msg_apps_t msg;
    if (g_disp_status == STATUS_A2DP_PLAY)
    {
        if (fast_play_flag == 0)
        {
            //消息类型(即消息名称)
            msg.type = MSG_BTENGINE_FBWD_SYNC;
            //发送同步消息
            send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
            fast_play_flag = 1;
        }
    }
    //暂停状态下，过滤快进退消息
    else
    {
        com_filter_key_hold(); //过滤后续short up消息
    }

    return RESULT_NULL;
}

app_result_e key_bt_open_visible(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_OPEN_BT_VISIBLE_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e key_ff_cancel_deal(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_FFWD_CANCEL_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    fast_play_flag = 0;

    return RESULT_NULL;
}

app_result_e key_fb_cancel_deal(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_FBWD_CANCEL_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    fast_play_flag = 0;

    return RESULT_NULL;
}

app_result_e spp_create_connect(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SPP_CREATE_CONNECTION_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}


app_result_e spp_discon_connect(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SPP_DISCON_CONNECTION_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}



app_result_e hfp_outgo_call(void)
{
    msg_apps_t msg;

    if ((g_btplay_bt_info.service_connect & HFP_CONNECTED) != 0)
    {
        //消息类型(即消息名称)
        msg.type = MSG_BTENGINE_OUTGOING_CALL_SYNC;
        //发送同步消息
        send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
    }

    return RESULT_NULL;
}

app_result_e deal_gui_bt(input_gui_msg_t *gui_msg)
{
    app_result_e app_ret;
    msg_apps_type_e gui_event;
    const key_map_t *p_key_map_list;

    app_ret = RESULT_NULL;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_bt_tws_enable == TRUE)
    {
        p_key_map_list = bt_tws_key_map_list;
    }
    else
#endif
    {
        p_key_map_list = bt_key_map_list;
    }

    if (com_key_mapping(gui_msg, &gui_event, p_key_map_list) == TRUE)
    {
        switch (gui_event)
        {
            case EVENT_PLAYING_NEXTSONG:
            if (fast_play_flag == 1)
            {
                app_ret = key_ff_cancel_deal();
            }
            else
            {
                app_ret = key_next_deal();
            }
            break;

            case EVENT_PLAYING_PREVSONG:
            if (fast_play_flag == 1)
            {
                app_ret = key_fb_cancel_deal();
            }
            else
            {
                app_ret = key_prev_deal();
            }
            break;

            case EVENT_PLAYING_PAUSE:
            app_ret = key_play_deal();
            break;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
            case EVENT_TWS_PAIR_LINK:
            {
                if (g_bt_tws_enable == TRUE)
                {
                    if (g_btplay_bt_info.tws_pair_status != TWS_PAIRED)
                    {
#if (SUPPORT_TWS_CONNECT_WHEN_PHONE_CONNECTED == 0)
                        if ((g_btplay_bt_info.status == STATUS_LINKED)
                            || (g_btplay_bt_info.status == STATUS_A2DP_PLAY)
                            || (g_btplay_bt_info.status == STATUS_A2DP_PAUSE))
                        {
                            //与手机建立连接后，不允许主动发起组队
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
                com_filter_key_hold(); //过滤后续short up消息

                if (get_tws_mode() != TWS_MODE_SINGLE)
                {
                    tws_dev_spk_pos_switch();
                }
            }
            break;
#endif

            case EVENT_CALL_LASTPHONE:
            #ifdef bt_auto_test
            if (g_btplay_bt_info.status==STATUS_WAIT_PAIRED)
            {
                com_btengine_connect_deal(CON_DEAL_CLR_PAIRED_LIST, NULL);
                app_ret = RESULT_NULL;
            }
            else
            #endif
            {
                if (((BQB_TEST_ENABLE|BQB_SPP_CREATE_CONNECT) & com_get_config_default(BTSTACK_BQB_TEST_ENALBE)) == (BQB_TEST_ENABLE|BQB_SPP_CREATE_CONNECT))
                {
                    spp_create_connect();
                    break;
                }
                app_ret = hfp_outgo_call();
            }
            break;

            case EVENT_HID_TAKE_PHOTO:
            com_filter_key_hold();
            if (((BQB_TEST_ENABLE|BQB_SPP_CREATE_CONNECT) & com_get_config_default(BTSTACK_BQB_TEST_ENALBE)) == (BQB_TEST_ENABLE|BQB_SPP_CREATE_CONNECT))
            {
                spp_discon_connect();
                app_ret = RESULT_NULL;
                break;
            }
            if (com_get_config_default(BTMANAGER_ENABLE_HID) == 1)
            {
                if (com_get_config_default(SETTING_APP_SUPPORT_TAKEPHOTO) == 1)
                {
                    app_ret = RESULT_ENTER_PHOTO;
                }
                else
                {
                    com_btengine_hid_take_photo();
                }
            }
            break;

            case EVENT_PLAYING_FFOREWORD:
            app_ret = key_ff_deal();
            break;

            case EVENT_PLAYING_FBACKWORD:
            app_ret = key_fb_deal();
            break;

            case EVENT_OPEN_BT_VISIBLE:
            app_ret = key_bt_open_visible();
            break;

            default:
            app_ret = com_message_box(gui_event);
            if (app_ret == RESULT_REDRAW)
            {
                g_need_draw = TRUE;
                app_ret = RESULT_NULL;
            }
            break;
        }
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int bt_message_deal(private_msg_t *msg)
 * \Record 获取私有消息的处理
 * \param[in]    private_msg_t *msg
 * \param[out]   none
 * \return               app_result_e
 * \retval
 * \ingroup      bluetooth_message_handle.c
 * \note
 */
/*******************************************************************************/
app_result_e bt_message_deal(private_msg_t *msg)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    switch (msg->msg.type)
    {
        default:
        app_ret = com_message_box(msg->msg.type);
        break;
    }

    return app_ret;
}

