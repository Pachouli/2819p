/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TWS 副箱发送给主箱的事件处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

tws_slave_ui_info_t g_tws_slave_ui_info;

bool com_tws_s2m_update_bat(uint8 slave_bat_val)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_S2M_UI_UPDATE_BAT_EV;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = slave_bat_val;

    ret_val = write_tws_cmd_to_buffer_rw_inner(g_p_tws_ui_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));

    if (ret_val == TRUE)
    {
        libc_printf_info("update bat:%d\n", slave_bat_val);
    }

    return ret_val;
}

bool com_tws_s2m_update_key(uint32 key_data)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_S2M_UI_UPDATE_KEY_EV;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = sizeof(uint32);
    *(uint32 *)tmp_tws_cmd.cmd_data = key_data;

    ret_val = write_tws_cmd_to_buffer_rw_inner(g_p_tws_ui_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));

    if (ret_val == TRUE)
    {
        libc_printf("update key:0x%x\n", key_data);
    }

    return ret_val;
}

bool com_tws_s2m_tts_play_req_inner(uint8 tts_id)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_S2M_UI_PLAY_TTS_REQ;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = tts_id;

    ret_val = write_tws_cmd_to_buffer_rw_inner(g_p_tws_ui_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));

    if (ret_val == TRUE)
    {
        libc_printf_info("tts play req:%d\n", tts_id);
    }

    return ret_val;
}

bool com_tws_s2m_update_btplay_inner(bool enter)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret_val;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_UI;
    tmp_tws_cmd.cmd_id = TWS_SYNC_S2M_UI_UPDATE_BTPLAY_EV;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = 1;
    tmp_tws_cmd.cmd_data[0] = enter;

    ret_val = write_tws_cmd_to_buffer_rw_inner(g_p_tws_ui_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t));

    if (ret_val == TRUE)
    {
        libc_printf_info("update btplay:%d\n", enter);
    }

    return ret_val;
}

//更新BTPLAY模式
//如果是退出BTPLAY模式，那么需要等待解码停下来并切换为单箱应用模式，才可以退出
bool com_tws_slave_update_btplay_inner(bool is_btplay)
{
    uint32 update_btplay_timer = sys_get_ab_timer();

    g_tws_slave_ui_info.is_btplay = is_btplay;
    g_tws_slave_ui_info.btplay_update = 1;

    if (is_btplay == TRUE)
    {
        return TRUE;
    }

    while (1)
    {
        com_bt_event_loop_inner();

        //等待变成单箱应用模式
        if (get_tws_mode_inner() == TWS_MODE_SINGLE)
        {
            break;
        }

        if ((sys_get_ab_timer() - update_btplay_timer) > 500)
        {
            libc_printf_warning("slave update btplay overtime!\n");
            break;
        }

        sys_os_time_dly(1);
    }

    return TRUE;
}

bool com_tws_slave_send_event_loop_deal(void)
{
    if ((g_tws_slave_ui_info.btplay_update != 0)
        && ((sys_get_ab_timer() - g_tws_slave_ui_info.btplay_timer) >= 500))
    {
        if (com_tws_s2m_update_btplay_inner(g_tws_slave_ui_info.is_btplay) == TRUE)
        {
            g_tws_slave_ui_info.btplay_update = 2;
            g_tws_slave_ui_info.btplay_timer = sys_get_ab_timer();
        }
    }

    if (g_tws_slave_ui_info.bat_val_update == TRUE)
    {
        if (com_tws_s2m_update_bat(g_tws_slave_ui_info.bat_val) == TRUE)
        {
            g_tws_slave_ui_info.bat_val_update = FALSE;
        }
    }

    if (g_tws_slave_ui_info.has_key == TRUE)
    {
        if (com_tws_s2m_update_key(g_tws_slave_ui_info.key_data) == TRUE)
        {
            g_tws_slave_ui_info.has_key = FALSE;
        }
    }

    return TRUE;
}

bool com_tws_sync_cmd_event_deal_m(tws_sync_cmd_t *p_tws_cmd)
{
    //上报副箱是否在BTPLAY事件
    if (p_tws_cmd->cmd_id == TWS_SYNC_S2M_UI_UPDATE_BTPLAY_EV)
    {
        bool is_btplay;

        is_btplay = p_tws_cmd->cmd_data[0];
        libc_printf_info("recv update btplay:%d\n", is_btplay);

        g_p_global_var->slave_btplay_flag = is_btplay;
        g_p_global_var->update_btplay_flag = TRUE;

        com_tws_m2s_ack_update_btplay_inner();
    }
    //上报电池电量
    else if (p_tws_cmd->cmd_id == TWS_SYNC_S2M_UI_UPDATE_BAT_EV)
    {
        uint8 slave_bat_val;
        uint8 min_bat_val;

        slave_bat_val = p_tws_cmd->cmd_data[0];
        libc_printf_info("recv bat val:%d\n", slave_bat_val);

        g_app_info_state.slave_bat_value = slave_bat_val;
        min_bat_val = g_app_info_state.bat_value;
        if (g_app_info_state.slave_bat_value < g_app_info_state.bat_value)
        {
            min_bat_val = g_app_info_state.slave_bat_value;
        }

        if (g_volume_infor.phy_limit_current != g_volume_infor.phy_limit_tbl[min_bat_val])
        {
            com_set_volume_limit_inner(g_volume_infor.phy_limit_tbl[min_bat_val]);
            com_tws_m2s_set_vol_limit_cmd(g_volume_infor.phy_limit_tbl[min_bat_val]);
        }
    }
    //上报按键动作
    else if (p_tws_cmd->cmd_id == TWS_SYNC_S2M_UI_UPDATE_KEY_EV)
    {
        uint32 key_data;

        key_data = *(uint32 *)p_tws_cmd->cmd_data;

        if ((int) sys_mq_send(MQ_ID_GUI_TEST, (void *) &key_data) == 0)
        {

        }

        libc_printf("recv slave key!0x%x\n", key_data);
    }
    //请求播报TTS
    else if (p_tws_cmd->cmd_id == TWS_SYNC_S2M_UI_PLAY_TTS_REQ)
    {
        if (g_tts_play_infor.slave_tts_count < 3)
        {
            uint8 tts_id = p_tws_cmd->cmd_data[0];
            g_tts_play_infor.slave_tts_ids[g_tts_play_infor.slave_tts_count] = tts_id;
            g_tts_play_infor.slave_tts_count++;

            libc_printf_info("recv tts play req:%d\n", tts_id);
        }
    }
    //invalid event
    else
    {
        libc_printf_warning("recv invalid sync cmd!%d\n", p_tws_cmd->cmd_id);
    }

    return TRUE;
}

bool com_tws_sync_cmd_loop_deal_m(void)
{
    tws_sync_cmd_t tmp_tws_cmd;
    bool ret_val = FALSE;

    if (read_tws_cmd_from_buffer_rw_inner(g_p_tws_ui_s2m_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t)) == TRUE)
    {
        ret_val = com_tws_sync_cmd_event_deal_m(&tmp_tws_cmd);
    }

    return ret_val;
}

#endif
