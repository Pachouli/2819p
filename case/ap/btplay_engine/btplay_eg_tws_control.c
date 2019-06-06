/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌引擎应用主循环，获取中间件状态并响应状态变化，获取应用消息并响应。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_btplay_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO
bool tws_send_sync_cmd(uint8 dev_role, uint8 cmd_id)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret = TRUE;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_EG;
    tmp_tws_cmd.cmd_id = cmd_id;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 0;

    if (dev_role == TWS_MASTER)
    {
        ret = write_tws_cmd_to_buffer_rw(p_tws_engine_m2s_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));
    }
    else
    {
        ret = write_tws_cmd_to_buffer_rw(p_tws_engine_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));
    }

    if (ret == TRUE)
    {
        libc_printf("tws eg send:%d\n", cmd_id);
    }

    return ret;
}

bool tws_recv_sync_cmd(uint8 dev_role, uint8 *p_cmd_id)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret = TRUE;

    if (dev_role == TWS_MASTER)
    {
        ret = read_tws_cmd_from_buffer_rw(p_tws_engine_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));
    }
    else
    {
        ret = read_tws_cmd_from_buffer_rw(p_tws_engine_m2s_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));
    }

    if (ret == TRUE)
    {
        *p_cmd_id = tmp_tws_cmd.cmd_id;
        libc_printf("tws eg recv:%d\n", *p_cmd_id);
    }

    return ret;
}

app_result_e btplay_engine_tws_pause(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (bp_handle != NULL)
    {
        //暂停播放
        ret_vals = _stop (TRUE);
        g_btplay_info.status = BTPLAY_STOP;
    }
    else
    {
        //清除和过滤AVDTP数据
        com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
    }

    g_filter_by_tws_forbidden_nest++;
    g_filter_by_tws_flag = TRUE;

    btplay_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

app_result_e btplay_engine_tws_resume(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_filter_by_tws_forbidden_nest > 0)
    {
        g_filter_by_tws_forbidden_nest--;
    }
    else
    {
        libc_printf_warning("g_filter_by_tws_forbidden_nest ERROR!\n");
    }

    if ((g_filter_by_tws_forbidden_nest == 0) && (g_filter_by_tws_clear_flag == FALSE))
    {
        g_filter_by_tws_flag = FALSE;
    }

    if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
    {
        //解除过滤AVDTP数据
        com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
    }

    //不需要在这里恢复_play，在主循环检测到数据会自动_play

    btplay_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

app_result_e btplay_engine_tws_conform_mode(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;
    bool filter_by_tws_mode_tdb = msg_ptr->msg.content.data[0];

    if (filter_by_tws_mode_tdb == FALSE)
    {
        if (g_filter_by_tws_mode_tdb == TRUE)
        {
            g_filter_by_tws_mode_tdb = FALSE;

            //解除过滤AVDTP数据
            if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
            {
                com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
            }
        }
    }
    else
    {
        if (g_filter_by_tws_mode_tdb == FALSE)
        {
            g_filter_by_tws_mode_tdb = TRUE;

            if (bp_handle != NULL)
            {
                //暂停播放
                ret_vals = _stop (TRUE);
                g_btplay_info.status = BTPLAY_STOP;
                //防止快速按键恢复播放，这样会出现副箱尚未完成退出而主箱已经继续传数给副箱的问题
                btplay_engine_tws_clear_start();
            }
            else
            {
                //清除和过滤AVDTP数据
                com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
            }
        }
    }

    btplay_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

int32 btplay_engine_tws_clear_start(void)
{
    //清除和过滤AVDTP数据
    com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
    g_filter_by_tws_clear_flag = TRUE;
    g_filter_by_tws_flag = TRUE;
    g_tws_start_clear_timer = sys_get_ab_timer();
    g_tws_clear_send_rpt = 0;

    if (get_tws_role() == TWS_MASTER)
    {
        g_tws_m_clear_sta = M_CLEAR_NULL;
    }
    else if (get_tws_role() == TWS_SLAVE)
    {
        g_tws_s_clear_send_req_first = TRUE;
        g_tws_s_clear_sta = S_CLEAR_NULL;
    }

    return 0;
}

bool btplay_engine_tws_role_change(void)
{
    bool need_wait_player_stop = FALSE;
    dev_role_e now_dev_role = get_tws_role();

    if ((last_tws_role == NORMAL_DEV) && (now_dev_role != NORMAL_DEV))
    {
        //清除和过滤AVDTP数据
        com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
        g_filter_by_tws_clear_flag = TRUE;
        g_filter_by_tws_flag = TRUE;
        g_tws_start_clear_timer = sys_get_ab_timer();
        g_tws_clear_send_rpt = 0;

        if (now_dev_role == TWS_MASTER)
        {
            g_tws_m_clear_sta = M_CLEAR_NULL;

            need_wait_player_stop = TRUE;
        }
        else
        {
            g_tws_s_clear_send_req_first = TRUE;
            g_tws_s_clear_sta = S_CLEAR_NULL;
        }
    }
    else if ((last_tws_role != NORMAL_DEV) && (now_dev_role == NORMAL_DEV))
    {
        btplay_eg_tws_flush_pipe();
        g_tws_unlink_delay_unfilter_flag = TRUE;
        g_tws_unlink_delay_unfilter_time = sys_get_ab_timer();
    }

    if ((g_tws_unlink_delay_unfilter_flag == TRUE)
        && ((sys_get_ab_timer() - g_tws_unlink_delay_unfilter_time) >= 200))
    {
        g_tws_unlink_delay_unfilter_flag = FALSE;

        g_filter_by_tws_forbidden_nest = 0;
        g_filter_by_tws_clear_flag = FALSE;
        g_filter_by_tws_flag = FALSE;
        g_tws_s_clear_sta = S_CLEAR_NULL;
        g_tws_m_clear_sta = M_CLEAR_NULL;

        if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
        {
            //解除过滤AVDTP数据
            com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
        }
    }

    return need_wait_player_stop;
}

int32 btplay_engine_tws_clear_deal_m(void)
{
    uint8 recv_cmd_id;
    bool recv_cmd_flag;
    bool clear_over_flag = FALSE;

    recv_cmd_flag = tws_recv_sync_cmd(TWS_MASTER, &recv_cmd_id);
    if ((recv_cmd_flag == TRUE) && (g_filter_by_tws_clear_flag == FALSE))
    {
        if (recv_cmd_id == TWS_SYNC_S2M_EG_WAIT_CLEAR_REQ)
        {
            btplay_engine_tws_clear_start();
        }
    }

    if (g_filter_by_tws_clear_flag == TRUE)
    {
        if (recv_cmd_flag == FALSE)
        {
            if ((sys_get_ab_timer() - g_tws_start_clear_timer) > TWS_CLEAR_OVER_TIME)
            {
                clear_over_flag = TRUE;
                goto _m_clear_over;
            }
        }
        else
        {
            g_tws_start_clear_timer = sys_get_ab_timer();
            g_tws_clear_send_rpt = 0;
        }

        if (recv_cmd_flag == TRUE)
        {
            if ((g_tws_m_clear_sta == M_CLEAR_NULL) && (recv_cmd_id == TWS_SYNC_S2M_EG_WAIT_CLEAR_REQ))
            {
                //收到副箱清理现场的请求
                g_tws_m_clear_sta = M_CLEAR_RECV_REQ;
                if (tws_send_sync_cmd(TWS_MASTER, TWS_SYNC_M2S_EG_CLEAR_RSP) == TRUE)
                {
                    g_tws_m_clear_sta = M_CLEAR_SEND_RSP;
                }
            }

            if ((g_tws_m_clear_sta == M_CLEAR_SEND_RSP) && (recv_cmd_id == TWS_SYNC_S2M_EG_CLEAR_CFM))
            {
                g_tws_m_clear_sta = M_CLEAR_RECV_CFM;
                clear_over_flag = TRUE;
                goto _m_clear_over;
            }
        }

        //发送回应有可能会失败，所以必须放在这里确保每次进来都能去发送
        if ((g_tws_m_clear_sta == M_CLEAR_RECV_REQ) || (g_tws_m_clear_sta == M_CLEAR_SEND_RSP))
        {
            if ((sys_get_ab_timer() - g_tws_start_clear_timer) > (100*g_tws_clear_send_rpt))
            {
                if (tws_send_sync_cmd(TWS_MASTER, TWS_SYNC_M2S_EG_CLEAR_RSP) == TRUE)
                {
                    g_tws_m_clear_sta = M_CLEAR_SEND_RSP;
                }
                g_tws_clear_send_rpt++;
            }
        }

        _m_clear_over:
        if (clear_over_flag == TRUE)
        {
            g_tws_s_clear_send_req_first = FALSE;
            g_filter_by_tws_clear_flag = FALSE;
            if (g_filter_by_tws_forbidden_nest == 0)
            {
                g_filter_by_tws_flag = FALSE;
            }

            flush_tws_cmd_buffer_rw(p_tws_mmm_m2s_pipe);
            flush_tws_cmd_buffer_rw(p_tws_mmm_s2m_pipe);

            if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
            {
                //至此，主箱可以解除过滤AVDTP数据
                if (g_tws_keep_filter_flag == FALSE)
                {
                    com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
                }
            }
        }
    }

    return 0;
}

int32 btplay_engine_tws_clear_deal_s(void)
{
    uint8 recv_cmd_id;
    bool recv_cmd_flag;
    bool clear_over_flag = FALSE;

    if (g_filter_by_tws_clear_flag == TRUE)
    {
        recv_cmd_flag = tws_recv_sync_cmd(TWS_SLAVE, &recv_cmd_id);
        if (recv_cmd_flag == FALSE)
        {
            if ((sys_get_ab_timer() - g_tws_start_clear_timer) > TWS_CLEAR_OVER_TIME)
            {
                clear_over_flag = TRUE;
                goto _s_clear_over;
            }
        }
        else
        {
            g_tws_start_clear_timer = sys_get_ab_timer();
            g_tws_clear_send_rpt = 0;
        }

        if (recv_cmd_flag == TRUE)
        {
            if ((g_tws_s_clear_sta == S_CLEAR_SEND_REQ) && (recv_cmd_id == TWS_SYNC_M2S_EG_CLEAR_RSP))
            {
                g_tws_s_clear_sta = S_CLEAR_RECV_RSP;
            }
        }

        if ((g_tws_s_clear_sta == S_CLEAR_NULL) || (g_tws_s_clear_sta == S_CLEAR_SEND_REQ))
        {
            if (g_tws_s_clear_send_req_first == TRUE)
            {
                if (tws_send_sync_cmd(TWS_SLAVE, TWS_SYNC_S2M_EG_WAIT_CLEAR_REQ) == TRUE)
                {
                    g_tws_s_clear_sta = S_CLEAR_SEND_REQ;
                }
                g_tws_s_clear_send_req_first = FALSE;
            }
            else
            {
                if ((sys_get_ab_timer() - g_tws_start_clear_timer) > (100*g_tws_clear_send_rpt))
                {
                    if (tws_send_sync_cmd(TWS_SLAVE, TWS_SYNC_S2M_EG_WAIT_CLEAR_REQ) == TRUE)
                    {
                        g_tws_s_clear_sta = S_CLEAR_SEND_REQ;
                    }
                    g_tws_clear_send_rpt++;
                }
            }
        }

        if (g_tws_s_clear_sta == S_CLEAR_RECV_RSP)
        {
            if (tws_send_sync_cmd(TWS_SLAVE, TWS_SYNC_S2M_EG_CLEAR_CFM) == TRUE)
            {
                g_tws_s_clear_sta = S_CLEAR_SEND_CFM;
                clear_over_flag = TRUE;
                goto _s_clear_over;
            }
        }

        _s_clear_over:
        if (clear_over_flag == TRUE)
        {
            g_filter_by_tws_clear_flag = FALSE;
            if (g_filter_by_tws_forbidden_nest == 0)
            {
                g_filter_by_tws_flag = FALSE;
            }

            flush_tws_cmd_buffer_rw(p_tws_mmm_m2s_pipe);
            flush_tws_cmd_buffer_rw(p_tws_mmm_s2m_pipe);

            if ((g_filter_by_tts_flag == FALSE) && (g_filter_by_tws_flag == FALSE) && (g_filter_by_tws_mode_tdb == FALSE))
            {
                //至此，副箱可以解除过滤AVDTP数据
                if (g_tws_keep_filter_flag == FALSE)
                {
                    com_btengine_set_play_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
                }
            }
        }
    }
    else
    {
        recv_cmd_flag = tws_recv_sync_cmd(TWS_SLAVE, &recv_cmd_id);
        //到这里还能够收到主箱的RSP，可能是因为主箱还没有收到副箱的CFM，所以这里再确认
        if ((recv_cmd_flag == TRUE) && (recv_cmd_id == TWS_SYNC_M2S_EG_CLEAR_RSP))
        {
            tws_send_sync_cmd(TWS_SLAVE, TWS_SYNC_S2M_EG_CLEAR_CFM);
        }
    }

    return 0;
}

//FALSE表示未知或很远，TRUE表示较近
bool btplay_engine_is_distance_near(void)
{
    if (get_tws_mode() != TWS_MODE_SINGLE)
    {
        if (g_btplay_data_source == SOURCE_BT_A2DP)
        {
            if ((btplay_bt_info.phone_rssi.rssi >= -10) && (btplay_bt_info.tws_rssi.rssi >= -10))
            {
                return TRUE;
            }
        }
        else
        {
            if (btplay_bt_info.tws_rssi.rssi >= -10)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

#endif
