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

int32 tws_stop_music_decoder_sub(music_bt_player_t *music_player, bool stop)
{
    if (p_tws_info->dev_role != NORMAL_DEV)
    {
        bt_clock_t cur_dl_clock;

        p_tws_info->tws_need_start_sync_play = FALSE;
        p_tws_info->send_to_dac_anolog_count = 0;
        music_player->send_to_pcm_buf_count = 0;
        music_player->is_sending_to_pcm_buf_flag = FALSE;

        libc_memset(&(p_tws_info->tws_slave_play_info), 0x00, sizeof(tws_play_info_t));
        libc_memset(&(p_tws_info->tws_slave_ip_frame_info), 0x00, sizeof(tws_ip_frame_info_t));
        libc_memset(&(p_tws_info->tws_play_info), 0x00, sizeof(tws_play_info_t));
        libc_memset(&(p_tws_info->tws_ip_frame_info), 0x00, sizeof(tws_ip_frame_info_t));

        if (p_tws_info->tws_get_bt_clk(&cur_dl_clock) == TRUE)
        {
            libc_printf("tws player stop at : %d\n", cur_dl_clock.bt_clk);
        }

        buffer_rw_flush(p_tws_info->p_tws_mmm_m2s_pipe);
        buffer_rw_flush(p_tws_info->p_tws_mmm_s2m_pipe);
    }

    return 0;
}

#endif
