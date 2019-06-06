/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话前台应用按键消息和应用消息处理；暂不支持三方通话和SIRI等语音助手。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

app_result_e hfp_accept_call(void);
app_result_e hfp_reject_call(void);
app_result_e hfp_hang_up_call(void);
app_result_e hfp_change_to_phone(void);
app_result_e hfp_change_to_hfp(void);
app_result_e hfp_3way_handle(uint8 mode);
app_result_e hfp_siri_deal(bool start);

const key_map_t bt_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },

#if (SUPPORT_RECORD_FUNC == 0)
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_CALL_LASTPHONE }, //三方通话
    { { KEY_F1, 0, KEY_TYPE_LONG }, EVENT_LOOPMODE_SET }, //三方通话
#endif

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

app_result_e deal_gui_bt(input_gui_msg_t *gui_msg)
{
    app_result_e app_ret;
    msg_apps_type_e gui_event;

    app_ret = RESULT_NULL;

    if (com_key_mapping(gui_msg, &gui_event, bt_key_map_list) == TRUE)
    {
        switch (gui_event)
        {
            /*通话过程中不能响应应用切换*/
            case EVENT_MODE_SWITCH:
                break;

            case EVENT_PLAYING_NEXTSONG:
            case EVENT_PLAYING_PREVSONG:
            if (g_disp_status == STATUS_CALL_INCOMING)
            {
                //来电时按next/prev拒绝接听
                hfp_reject_call();
            }
            else if (g_disp_status == STATUS_CALL_HFP)
            {
                //正在接听时按next/prev切换到PHONE接听
                if (g_chf_cag_exch_enable == 1)
                {
                    hfp_change_to_phone();
                }
            }
            else if (g_disp_status == STATUS_CALL_PHONE)
            {
                //正在接听时按next/prev切换到音箱接听
                if (g_chf_cag_exch_enable == 1)
                {
                    hfp_change_to_hfp();
                }
            }
            else
            {
                //nothing for QAC
            }
            break;

            case EVENT_PLAYING_PAUSE:
            if ((g_btcall_bt_info.three_way_status == THREE_WAY_NEW_CALL_IN) ||
                    (g_btcall_bt_info.three_way_status == THREE_WAY_HOLD_EXIST))
            {
                hfp_3way_handle(HFP_3WAY_SWAP_CALL);
            }
            else
            {
                if (g_disp_status == STATUS_CALL_INCOMING)
                {
                    //来电时按play接听
                    hfp_accept_call();
                }
                else
                {
                    if (g_disp_status == STATUS_SIRI)
                    {
                        hfp_siri_deal(FALSE);
                    }
                    else
                    {
                        //打出时/正在接听时按play挂断
                        hfp_hang_up_call();
                    }
                }
            }
            break;

            case EVENT_CALL_LASTPHONE:
            if ((g_btcall_bt_info.three_way_status == THREE_WAY_NEW_CALL_IN) ||
                    (g_btcall_bt_info.three_way_status == THREE_WAY_HOLD_EXIST))
            {
                hfp_3way_handle(HFP_3WAY_RELEASE_HELD);
            }
            break;

            case EVENT_LOOPMODE_SET:
            com_filter_key_hold(); //过滤后续short up消息
            if ((g_btcall_bt_info.three_way_status == THREE_WAY_NEW_CALL_IN) ||
                    (g_btcall_bt_info.three_way_status == THREE_WAY_HOLD_EXIST))
            {
                hfp_3way_handle(HFP_3WAY_RELEASE_ACTIVE);
            }
            break;

            default:
            app_ret = com_message_box(gui_event);
            if ((app_ret != RESULT_NULL) && (app_ret != RESULT_REDRAW))
            {
                hfp_change_to_phone();
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
    app_result_e app_ret = RESULT_NULL;

    switch (msg->msg.type)
    {
        case MSG_SD_IN:
        case MSG_UH_IN:
        case MSG_LINEIN_IN:
        case MSG_USB_STICK:
        //打电话场景下忽略设备插入消息
        break;

        default: //默认系统消息，messageBox处理
        app_ret = com_message_box(msg->msg.type);
        if ((app_ret != RESULT_NULL) && (app_ret != RESULT_REDRAW))
        {
            if (app_ret == RESULT_ENTER_ALARM)
            {
                libc_printf_info("BTCALL MODE ALARM AUTO SNOOZE!\n");
                com_rtc_update_alarm(ALARM_UPDATE_SNOOZE);
                app_ret = RESULT_NULL;
            }
            else
            {
                hfp_change_to_phone();
            }
        }
        break;
    }

    return app_ret;
}

app_result_e hfp_accept_call(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_ACCEPT_CALL_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e hfp_reject_call(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_REJECT_CALL_SYNC;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e hfp_hang_up_call(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_HANDUP_CALL_SYNC;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e hfp_change_to_phone(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_MGR_RELEASESYN_SYNC;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e hfp_change_to_hfp(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_MGR_SETSYN_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

/*
 mode:0--release the held call
 1--release the current call
 2--held/active call swap
 3--transform held to active
 */
app_result_e hfp_3way_handle(uint8 mode)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_3WAY_HANDLE_SYNC;
    msg.content.data[0] = mode;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e hfp_siri_deal(bool start)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SET_SIRI_SYNC;
    if (start == TRUE)
    {
        msg.content.data[0] = BTENGINE_MSG_SIRI_START;
    }
    else
    {
        msg.content.data[0] = BTENGINE_MSG_SIRI_END;
    }
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}
