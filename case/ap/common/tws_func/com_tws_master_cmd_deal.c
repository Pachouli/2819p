/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TWS 主箱发送命令处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

extern sys_counter_t g_sys_counter;

static bool __tws_m2s_send_cmd_sync(tws_sync_cmd_t *p_cmd, uint32 timeout, tws_sync_cmd_t *p_cmd_reply)
{
    uint32 send_cmd_timout;
    uint8  send_cmd_state; //0表示初始化，1表示已经发送同步命令，2表示已经收到答复
    bool ret_val = TRUE;

    send_cmd_timout = sys_get_ab_timer();
    send_cmd_state = 0;

    while (1)
    {
        if (send_cmd_state == 0)
        {
            ret_val = write_tws_cmd_to_buffer_rw_inner(g_p_tws_ui_m2s_pipe, p_cmd, sizeof(tws_sync_cmd_t));
            if (ret_val == TRUE)
            {
                send_cmd_state = 1;
            }
        }

        if (send_cmd_state == 1)
        {
            if (read_tws_cmd_from_buffer_rw_inner(g_p_tws_ui_s2m_pipe, p_cmd_reply, sizeof(tws_sync_cmd_t)) == TRUE)
            {
                if (p_cmd_reply->cmd_id == TWS_SYNC_S2M_UI_SYNC_REPLY)
                {
                    send_cmd_state = 2;
                    if (p_cmd_reply->cmd_para == TWS_SYNC_REPLY_NACK)
                    {
                        ret_val = FALSE;
                    }
                }
                else
                {
                    com_tws_sync_cmd_event_deal_m(p_cmd_reply);
                }
            }
        }

        if (send_cmd_state == 2)
        {
            break;
        }

        if (timeout > 0)
        {
            if ((sys_get_ab_timer() - send_cmd_timout) > timeout)
            {
                libc_printf_warning("master send cmd sync timeout!0x%x\n", p_cmd->cmd_id);
                ret_val = FALSE;
                break;
            }
        }

        sys_os_time_dly(1);
    }

    return ret_val;
}

bool com_tws_s2m_cmd_sync_reply(tws_sync_reply_e ack, void *para, uint8 para_len)
{
    tws_sync_cmd_t reply_cmd;
    bool ret_val;
    uint8 i;

    reply_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    reply_cmd.cmd_id = TWS_SYNC_S2M_UI_SYNC_REPLY;
    reply_cmd.cmd_para = ack;
    reply_cmd.cmd_len = para_len;

    if ((para != NULL) && (para_len > 0))
    {
        libc_memcpy(reply_cmd.cmd_data, para, para_len);
    }

    for (i = 0; i < 10; i++)
    {
        ret_val = write_tws_cmd_to_buffer_rw_inner(g_p_tws_ui_s2m_pipe, &reply_cmd, sizeof(tws_sync_cmd_t));
        if (ret_val == TRUE)
        {
            break;
        }

        sys_os_time_dly(1);
    }

    if (i == 10)
    {
        libc_printf_warning("slave cmd sync reply timeout!\n");
    }

    return ret_val;
}

bool com_tws_m2s_set_vol_val_cmd(uint8 vol)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_SET_VOL_VAL_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = vol;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf("master set vol:%d\n", vol);
    }

    return ret_val;
}

bool com_tws_m2s_set_vol_limit_cmd(int16 vol_limit)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_SET_VOL_LIMIT_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = sizeof(int16);
    libc_memcpy(tmp_tws_cmd.cmd_data, &vol_limit, sizeof(int16));

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf("master set vol limit:%d\n", vol_limit);
    }

    return ret_val;
}

bool com_tws_m2s_switch_pos_cmd_inner(dev_spk_pos_e pos)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_SWITCH_POS_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = pos;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf_info("master switch pos:%d\n", pos);
    }

    return ret_val;
}

bool com_tws_m2s_switch_tws_mode_cmd_inner(tws_mode_e tws_mode)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    libc_printf_info("master switch tws mode start:%d\n", tws_mode);

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_SWITCH_TWS_MODE_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = tws_mode;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        change_tws_mode_inner(tws_mode);
        if (tws_mode == TWS_MODE_AUX_TWS)
        {
            com_btengine_switch_avdtp_source(AVDTP_SOURCE_LINEIN_ADC);
        }
        else
        {
            com_btengine_switch_avdtp_source(AVDTP_SOURCE_BT_A2DP);
        }
        if (tws_mode == TWS_MODE_SINGLE)
        {
            com_tws_mode_exit(TWS_MASTER);
        }
        else
        {
            com_tws_mode_init(TWS_MASTER);
        }
        libc_printf_info("master switch tws mode finish:%d\n", tws_mode);
    }

    return ret_val;
}

bool com_tws_m2s_ack_update_btplay_inner(void)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_ACK_UPDATE_BTPLAY;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 0;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf_info("master ack update btplay\n");
    }

    return ret_val;
}

bool com_tws_m2s_poweroff_cmd(uint8 poweroff_mode)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_POWEROFF_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = poweroff_mode;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf_info("master poweroff\n");
    }

    return ret_val;
}

bool com_tws_m2s_switch_next_app_cmd(void)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_SWITCH_NEXT_APP_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 0;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf_info("allow slave switch next app\n");
    }

    return ret_val;
}

bool com_tws_m2s_tts_play_req(uint8 tts_id)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_TTS_PLAY_REQ;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = tts_id;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        libc_printf_info("master tts play:%d\n", tts_id);
    }

    return ret_val;
}

bool com_tws_m2s_tts_status_req(tws_slave_tts_state_e *tts_sta)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_sync_cmd_t tmp_cmd_reply;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_UI_TTS_STA_REQ;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 0;

    ret_val = __tws_m2s_send_cmd_sync(&tmp_tws_cmd, 500, &tmp_cmd_reply);
    if (ret_val == TRUE)
    {
        *tts_sta = (tws_slave_tts_state_e)(tmp_cmd_reply.cmd_data[0]);
        //libc_printf("master tts sta:%d\n", *tts_sta);
    }
    else
    {
        *tts_sta = TWS_SLAVE_STA_UNKNOWN;
    }

    return ret_val;
}

bool com_tws_s2m_tts_status_rsp(tws_slave_tts_state_e tts_sta)
{
    return com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, &tts_sta, 1);
}

bool com_tws_sync_cmd_loop_deal_s(void)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret_val = FALSE;

    if (read_tws_cmd_from_buffer_rw_inner(g_p_tws_ui_m2s_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t)) == TRUE)
    {
        if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_SET_VOL_VAL_CMD)
        {
            uint8 vol_val = tmp_tws_cmd.cmd_data[0];

            libc_printf("recv set vol:%d\n", vol_val);

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);

            if (g_tts_play_infor.tts_playing == 1)
            {
                g_tts_play_infor.tts_slave_volume_delay_flag = 1;
                g_tts_play_infor.slave_volume_delay = vol_val;
                libc_printf("delay\n");
            }
            else
            {
                com_volume_set_inner(SET_VOL_BY_SYSTEM, vol_val);
            }
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_SET_VOL_LIMIT_CMD)
        {
            int16 vol_limit;

            libc_memcpy(&vol_limit, tmp_tws_cmd.cmd_data, sizeof(int16));

            libc_printf("recv set vol limit:%d\n", vol_limit);

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);

            if (g_tts_play_infor.tts_playing == 1)
            {
                g_tts_play_infor.tts_slave_limit_delay_flag = 1;
                g_tts_play_infor.slave_limit_delay = vol_limit;
                libc_printf("delay\n");
            }
            else
            {
                com_set_volume_limit_inner(vol_limit);
            }
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_POWEROFF_CMD)
        {
            uint16 msg_type;
            uint8 poweroff_mode = tmp_tws_cmd.cmd_data[0];

            libc_printf_info("recv poweroff\n");

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);

            if (poweroff_mode == TWS_POWEROFF_SYS)
            {
                msg_type = MSG_POWER_OFF;
            }
            else if (poweroff_mode == TWS_POWEROFF_KEY)
            {
                msg_type = EVENT_ENTER_SHUTOFF_PLAY;
            }
            else
            {
                msg_type = MSG_SYSTEM_ENTER_S2;
            }

            sys_mq_send(MQ_ID_SYS, &msg_type);
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_SWITCH_POS_CMD)
        {
            dev_spk_pos_e tws_spk = (dev_spk_pos_e)(tmp_tws_cmd.cmd_data[0]);

            libc_printf_info("recv switch pos:%d\n", tws_spk);

            change_tws_spk_pos_inner(tws_spk);

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_SWITCH_TWS_MODE_CMD)
        {
            tws_mode_e tws_mode = (tws_mode_e)(tmp_tws_cmd.cmd_data[0]);

            libc_printf_info("recv switch tws mode:%d\n", tws_mode);

            change_tws_mode_inner(tws_mode);
            if (tws_mode == TWS_MODE_SINGLE)
            {
                com_tws_mode_exit(TWS_SLAVE);
            }
            else
            {
                com_tws_mode_init(TWS_SLAVE);
            }

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_ACK_UPDATE_BTPLAY)
        {
            if (g_tws_slave_ui_info.btplay_update == 2)
            {
                g_tws_slave_ui_info.btplay_update = 0;
            }
            libc_printf_info("recv ack update btplay\n");
            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_TTS_PLAY_REQ)
        {
            uint8 tts_id = tmp_tws_cmd.cmd_data[0];

            libc_printf_info("recv tts play req:%d\n", tts_id);

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);

            com_tws_tts_play_slave(tts_id, 0);
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_TTS_STA_REQ)
        {
            tws_slave_tts_state_e tts_sta = TWS_SLAVE_STA_STOP;
            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, &tts_sta, 1);
        }
        else if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_SWITCH_NEXT_APP_CMD)
        {
            uint16 msg_type;

            libc_printf_info("recv switch next app\n");

            com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, NULL, 0);
            sys_os_time_dly(10); //会导致TWS连接断开，延时100ms确保主箱收到reply

            msg_type = EVENT_MODE_SWITCH;

            sys_mq_send(MQ_ID_SYS, &msg_type);
        }
    }

    return ret_val;
}

#endif
