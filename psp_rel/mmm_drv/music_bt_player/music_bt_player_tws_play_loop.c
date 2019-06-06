/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件TWS播放控制处理：主箱与从箱交互命令处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

bool tws_cmd_and_event_deal(music_bt_player_t *music_player)
{
    bool ret = FALSE;
    tws_sync_cmd_t temp_tws_sync_cmd;

    if (p_tws_info->dev_role == TWS_MASTER)
    {
        while (buffer_rw_read(p_tws_info->p_tws_mmm_s2m_pipe, (uint8 *)&temp_tws_sync_cmd, sizeof(tws_sync_cmd_t)) == TRUE)
        {
#ifdef BT_TWS_DEBUG_CMD_ID
            libc_printf("tws mmm recv %d\n", temp_tws_sync_cmd.cmd_id);
#endif
            ret = TRUE;
            if (temp_tws_sync_cmd.cmd_id == TWS_SYNC_S2M_PLAY_INFO_EV)
            {
                tws_exec_update_play_info(music_player, (tws_play_info_t *)(temp_tws_sync_cmd.cmd_data));
            }
            else if (temp_tws_sync_cmd.cmd_id == TWS_SYNC_S2M_IP_FRAME_INFO_EV)
            {
                if (tws_exec_update_ip_frame_info(music_player, (tws_ip_frame_info_t *)(temp_tws_sync_cmd.cmd_data)) < 0)
                {
                    return FALSE;
                }
            }
        }
    }
    else if (p_tws_info->dev_role == TWS_SLAVE)
    {
        while (buffer_rw_read(p_tws_info->p_tws_mmm_m2s_pipe, (uint8 *)&temp_tws_sync_cmd, sizeof(tws_sync_cmd_t)) == TRUE)
        {
#ifdef BT_TWS_DEBUG_CMD_ID
            libc_printf("tws mmm recv %d\n", temp_tws_sync_cmd.cmd_id);
#endif
            ret = TRUE;
            if (temp_tws_sync_cmd.cmd_id == TWS_SYNC_M2S_START_PLAY_CMD)
            {
                tws_exec_start_play(music_player, (tws_m2s_start_play_para_t *)(temp_tws_sync_cmd.cmd_data));
            }
            else if (temp_tws_sync_cmd.cmd_id == TWS_SYNC_M2S_SET_AUDIO_APS_CMD)
            {
                tws_exec_update_audio_aps(music_player, (tws_m2s_set_audio_aps_para_t *)(temp_tws_sync_cmd.cmd_data));
            }
        }
    }

    return ret;
}

int32 tws_update_play_info_s2m(music_bt_player_t *music_player)
{
    int32 ret = 0;
    uint32 pcm_buf_remain;
    tws_sync_cmd_t tmp_tws_cmd;

    if (p_tws_info->tws_need_start_sync_play == FALSE)
    {
        return 0;
    }

    //副箱需要调整Audio APS，则暂停上报播放信息，并且要在调整结束后才开始计时
    if (p_tws_info->need_slave_set_audio_aps == TRUE)
    {
        p_tws_info->slave_update_play_info_timer = sys_get_ab_timer();
        return 0;
    }

    if ((sys_get_ab_timer() - p_tws_info->slave_update_play_info_timer) > TWS_SLAVE_UPDATE_PLAY_INFO_TIMER)
    {
        if (tws_get_dac_samples_bt_clock(music_player, &pcm_buf_remain, &(p_tws_info->tws_play_info.con_clock)) == FALSE)
        {
            libc_printf_warning("get_pcmbuf_length fail!\n");
            return -1;
        }

        p_tws_info->tws_play_info.dac_samples = music_player->send_to_pcm_buf_count - pcm_buf_remain;

        //libc_printf("tim:[%d,%d],[%d,%d]\n", p_tws_info->send_to_pcm_buf_count, pcm_buf_remain,
        //p_tws_info->tws_play_info.con_clock.bt_clk, p_tws_info->tws_play_info.con_clock.bt_intraslot);

        tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_PLAY;
        tmp_tws_cmd.cmd_id = TWS_SYNC_S2M_PLAY_INFO_EV;
        tmp_tws_cmd.cmd_para = 0;
        tmp_tws_cmd.cmd_len = sizeof(tws_play_info_t);
        libc_memcpy(tmp_tws_cmd.cmd_data, &(p_tws_info->tws_play_info), sizeof(tws_play_info_t));

        if (buffer_rw_write(p_tws_info->p_tws_mmm_s2m_pipe, (uint8 *)&tmp_tws_cmd, sizeof(tws_sync_cmd_t)) == FALSE)
        {
            ret = -1;
        }
        else
        {
#ifdef BT_TWS_DEBUG_CMD_ID
            libc_printf("tws mmm send %d\n", tmp_tws_cmd.cmd_id);
#endif	
        }

        p_tws_info->slave_update_play_info_timer = sys_get_ab_timer();
    }

    return ret;
}

int32 tws_exec_update_play_info(music_bt_player_t *music_player, tws_play_info_t *p_tws_play_info)
{
    int32 ret = 0;

    if (music_player->music_status.status != MMM_BP_ENGINE_PLAYING)
    {
        libc_printf_warning("update_play_info at NOT PLAYING!\n");
        return 0;
    }

    libc_memcpy(&(p_tws_info->tws_slave_play_info), p_tws_play_info, sizeof(tws_play_info_t));

    return ret;
}

#endif
