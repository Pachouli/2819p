/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ��ǰ̨Ӧ�ð�����Ϣ��Ӧ����Ϣ�����ݲ�֧������ͨ����SIRI���������֡�
 * ���ߣ�cailizhen
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
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_CALL_LASTPHONE }, //����ͨ��
    { { KEY_F1, 0, KEY_TYPE_LONG }, EVENT_LOOPMODE_SET }, //����ͨ��
#endif

    /*! ������־ */
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
            /*ͨ�������в�����ӦӦ���л�*/
            case EVENT_MODE_SWITCH:
                break;

            case EVENT_PLAYING_NEXTSONG:
            case EVENT_PLAYING_PREVSONG:
            if (g_disp_status == STATUS_CALL_INCOMING)
            {
                //����ʱ��next/prev�ܾ�����
                hfp_reject_call();
            }
            else if (g_disp_status == STATUS_CALL_HFP)
            {
                //���ڽ���ʱ��next/prev�л���PHONE����
                if (g_chf_cag_exch_enable == 1)
                {
                    hfp_change_to_phone();
                }
            }
            else if (g_disp_status == STATUS_CALL_PHONE)
            {
                //���ڽ���ʱ��next/prev�л����������
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
                    //����ʱ��play����
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
                        //���ʱ/���ڽ���ʱ��play�Ҷ�
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
            com_filter_key_hold(); //���˺���short up��Ϣ
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
 * \Record ��ȡ˽����Ϣ�Ĵ���
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
        //��绰�����º����豸������Ϣ
        break;

        default: //Ĭ��ϵͳ��Ϣ��messageBox����
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

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_ACCEPT_CALL_SYNC;
    //����ͬ����Ϣ
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

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_MGR_SETSYN_SYNC;
    //����ͬ����Ϣ
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

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_3WAY_HANDLE_SYNC;
    msg.content.data[0] = mode;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e hfp_siri_deal(bool start)
{
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_SET_SIRI_SYNC;
    if (start == TRUE)
    {
        msg.content.data[0] = BTENGINE_MSG_SIRI_START;
    }
    else
    {
        msg.content.data[0] = BTENGINE_MSG_SIRI_END;
    }
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}
