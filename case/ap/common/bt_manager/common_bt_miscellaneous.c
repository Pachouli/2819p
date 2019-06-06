/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������������ȡ����״̬�����������Թ�������AVDTP/SCO����ͨ·���ƽӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_bt_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \  ��ȡ������̨�����״̬
 * \param[in]    none
 * \param[out]   bg_info ����״ָ̬��
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 */
/*******************************************************************************/

void com_get_btengine_status_inner(btengine_info_t *p_bt_info)
{
    msg_apps_t msg;
    bool need_ignore_vol_sync;

    read_status_again:

    //����BT����״̬
    msg.type = MSG_BTENGINE_GET_STATUS_SYNC;
    msg.content.addr = &bt_info;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    //ESD RESETʵʱ�ϵ㱸��
    g_p_esd_bk_info->bt_info_state = bt_info.status;

    //��������δ������ͬ�����ֻ�֮ǰ�����ֻ�������ͬ������
    need_ignore_vol_sync = FALSE;
    if ((get_cur_app_id_inner() == APP_ID_BTPLAY) && (bt_info.vol_syn_flag != 0) && (bt_a2dp_info.a2dp_start_flag != 2))
    {
        need_ignore_vol_sync = TRUE;
    }
    if ((get_cur_app_id_inner() == APP_ID_BTCALL) && (bt_info.vol_syn_flag != 0) && (bt_hfp_info.hfp_start_flag != 2))
    {
        need_ignore_vol_sync = TRUE;
    }

    if (need_ignore_vol_sync == TRUE)
    {
        uint8 return_vol = 0;

        libc_printf("phone->btbox vol sync ignore!");

        if (bt_info.vol_syn_flag == 1)
        {
            return_vol = bt_info.remote_volume;
        }

        //����ͬ��������󣬸�֪bt engine�������
        msg.type = MSG_BTENGINE_VOL_UPDATE_FINISH_SYNC;
        msg.content.data[0] = return_vol;
        send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

        goto read_status_again;
    }

    if (p_bt_info != NULL)
    {
        libc_memcpy(p_bt_info, &bt_info, sizeof(btengine_info_t));
    }
}

/*  ����:�����豸�Ŀɼ��ԣ���������
 ����:visual_mode
 0-�豸���ɼ�����������
 1-�豸���ɼ���������
 2-�豸�ɼ�����������
 3-�豸�ɼ���������
 */
bool com_btengine_set_visual_mode_inner(uint8 visual_mode)
{
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_MGR_SET_VISUALMODE_SYNC;
    msg.content.data[0] = visual_mode;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_connect_deal_inner(uint8 mode, void *para)
{
    msg_apps_t msg;
    bool need_send_msg = FALSE;

    if (get_bt_state_inner() == BT_STATE_NONE)
    {
        return TRUE;
    }

    com_get_btengine_status_inner(NULL);

    //�Ͽ��������������ʹ���豸���棬ֻ��BTPLAYӦ������Ч
    if (mode == CON_DEAL_DISC_CONN_SWITCH)
    {
        if ((bt_info.status >= STATUS_LINKED) && (bt_info.status <= STATUS_SIRI))
        {
            mode = CON_DEAL_FORCE_DISCONNECT;
        }
        else
        {
            mode = CON_DEAL_CONNECT_LAST;
        }
    }

    if (mode == CON_DEAL_FORCE_DISCONNECT)
    {
        if ((bt_info.status >= STATUS_LINKED) && (bt_info.status <= STATUS_SIRI))
        {
            msg.type = MSG_BTENGINE_FORCE_WAITPAIRED_SYNC;
            msg.content.data[0] = 0x00;
            need_send_msg = TRUE;
        }
        else
        {
            return TRUE;
        }
    }
    else if (mode == CON_DEAL_GET_PIARED_LIST)
    {
        msg.type = MSG_BTENGINE_GET_PAIRED_LIST_SYNC;
        msg.content.addr = para;
        need_send_msg = TRUE;
    }
    else if (mode == CON_DEAL_CLR_PAIRED_LIST)
    {
        msg.type = MSG_BTENGINE_CLR_PAIRED_LIST_SYNC;
        msg.content.data[0]=CLEAR_ALL_DEV_PAIRED_INFO;
        need_send_msg = TRUE;
    }
    else if (mode == CON_DEAL_CONNECT_LAST)
    {
        msg.type = MSG_BTENGINE_SET_CONNECT_LAST_SYNC;
        need_send_msg = TRUE;
    }
    else if (mode == CON_DEAL_CONNECT_ADDR)
    {
        msg.type = MSG_BTENGINE_SET_CONNECT_ADDR_SYNC;
        msg.content.addr = para;
        need_send_msg = TRUE;
    }
    else if (mode == CON_DEAL_TWS_LINK)
    {
        msg.type = MSG_BTENGINE_TWS_LINK_SYNC;
        msg.content.data[0] = (uint8) (uint32) para;
        need_send_msg = TRUE;
    }
    else if (mode == CON_DEAL_TWS_UNLINK)
    {
        msg.type = MSG_BTENGINE_TWS_FORCE_UNLINK_SYNC;
        need_send_msg = TRUE;
    }
    else if (mode == CON_DEAL_SET_TWS_RECONN_PHONE_POLICY)
    {
        msg.type = MSG_BTENGINE_SET_TWS_DEV_RECONNECT_PHONE_POLICY_SYNC;
        msg.content.addr = para;
        need_send_msg = TRUE;
    }

    //����ͬ����Ϣ
    if (need_send_msg == TRUE)
    {
        send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
    }

    return TRUE;
}

bool com_btengine_set_sbc_play_inner(bool enter)
{
    msg_apps_t msg;

    if (enter == TRUE)
    {
        //�����������������Ƹ��ѿ�ʼ����
        msg.type = MSG_BTENGINE_ENTER_SBC_PLAY_SYNC;
    }
    else
    {
        //�����������������Ƹ���ֹͣ����
        msg.type = MSG_BTENGINE_EXIT_SBC_PLAY_SYNC;
    }
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_set_play_pipe_filter_inner(uint8 filter_opt)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_PLAY_PIPE_CTRL_SYNC;
    msg.content.data[0] = filter_opt;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_set_hfp_call_inner(bool enter)
{
    msg_apps_t msg;

    if (enter == TRUE)
    {
        msg.type = MSG_BTENGINE_ENTER_HFP_CALL_SYNC;
    }
    else
    {
        msg.type = MSG_BTENGINE_EXIT_HFP_CALL_SYNC;
    }
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_set_sco_pipe_filter_inner(uint8 filter_opt)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_SCO_PIPE_CTRL_SYNC;
    msg.content.data[0] = filter_opt;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_hid_take_photo_inner(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_HID_TAKE_PHOTO_SYNC;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_force_disable_sniff_inner(bool force_disable)
{
    msg_apps_t msg;

    if (force_disable == TRUE)
    {
        msg.type = MSG_BTENGINE_DISABLE_ENTER_BT_SNIFF_SYNC;
    }
    else
    {
        msg.type = MSG_BTENGINE_ENABLE_ENTER_BT_SNIFF_SYNC;
    }
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_switch_avdtp_source(avdtp_data_source_e source)
{
    msg_apps_t msg;

    if (get_bt_state_inner() == BT_STATE_NONE)
    {
        return TRUE;
    }

    msg.type = MSG_BTENGINE_SWITCH_AVDTP_SOURCE_SYNC;
    msg.content.data[0] = source;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool com_btengine_hfp_siri_start(void)
{
    msg_apps_t msg;

    if (get_bt_state_inner() == BT_STATE_NONE)
    {
        return TRUE;
    }

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_SET_SIRI_SYNC;
    msg.content.data[0] = BTENGINE_MSG_SIRI_START;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO

bool com_btengine_tws_get_bt_cbk_inner(bt_clock_cbk_t *p_bt_clock_cbk)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;

    msg.type = MSG_BTENGINE_TWS_GET_BT_CBK_SYNC;
    msg.content.addr = p_bt_clock_cbk;
    send_sync_msg(APP_ID_BTENGINE, &msg, &temp_reply, 0);

    if (temp_reply.type == MSG_REPLY_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool com_btengine_tws_get_send_frame_cbk_inner(f_send_frame *pp_f_send_frame)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;

    msg.type = MSG_BTENGINE_TWS_GET_SEND_FRAME_CBK_SYNC;
    msg.content.addr = pp_f_send_frame;
    send_sync_msg(APP_ID_BTENGINE, &msg, &temp_reply, 0);

    if (temp_reply.type == MSG_REPLY_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif

