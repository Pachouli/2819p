/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件TWS播放控制处理：主箱与从箱同步开始播放。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

int32 tws_start_music_decoder_sub_pre(music_bt_player_t *music_player)
{
    if (p_tws_info->dev_role != NORMAL_DEV)
    {
        //OUTPUT BUFFER 清0，以便先扔一两帧空白帧
        libc_memset((uint8 *) SBC_DEC_OUTPUT_BUFFER_ADDR, 0x00, SBC_DEC_OUTPUT_BLOCK_SIZE);
        send_pcm_to_pcm_buf_by_dac_fifo((short *) SBC_DEC_OUTPUT_BUFFER_ADDR, SBC_DEC_OUTPUT_BLOCK_SIZE/2, FALSE);

        p_tws_info->samples_diff_max = 0;
        p_tws_info->samples_diff_max_recent_count1 = 0;
        p_tws_info->samples_diff_max_recent1 = 0;
        p_tws_info->samples_diff_max_recent2 = (uint8)(-1);
    }

    return 0;
}

void __section__(".rcode") player_first_send_zero_data_to_pcm_buf(void)
{
    send_pcm_to_pcm_buf_by_dac_fifo_start();
    music_player_global->send_to_pcm_buf_count += SBC_DEC_OUTPUT_BLOCK_SIZE/4;
}

void __section__(".rcode") __tws_ture_start_play_handle(void)
{
    //获取DAC开始播放的时间
    music_player_global->tws_info.tws_get_bt_clk(&(music_player_global->tws_info.start_con_clock));

    //填充3帧空帧
    player_first_send_zero_data_to_pcm_buf();
    player_first_send_zero_data_to_pcm_buf();
    player_first_send_zero_data_to_pcm_buf();

    //让解码线程继续运行
    libc_sem_post(music_player_global->tws_info.start_dec_sem);
}

void __tws_ture_start_play_handle(void) __FAR__;

int32 tws_ture_start_play(music_bt_player_t *music_player, uint32 start_dac_con_clock)
{
    int32 ret = 0;

    if (start_dac_con_clock == 0)
    {
        p_tws_info->tws_set_bt_clk(0);
        p_tws_info->tws_reg_bt_clk_sync_cbk(NULL);

        return ret;
    }

    p_tws_info->tws_set_bt_clk(0);
    p_tws_info->tws_reg_bt_clk_sync_cbk(__tws_ture_start_play_handle);
    p_tws_info->tws_set_bt_clk(start_dac_con_clock);

    return ret;
}

int32 tws_set_start_play_m2s(music_bt_player_t *music_player, bt_clock_t *p_dl_clock)
{
    tws_sync_cmd_t tmp_tws_cmd;
    tws_m2s_start_play_para_t tmp_start_play_para;

    libc_memcpy(&(tmp_start_play_para.should_start_con_clock), p_dl_clock, sizeof(bt_clock_t));

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_PLAY;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_START_PLAY_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = sizeof(tws_m2s_start_play_para_t);
    libc_memcpy(tmp_tws_cmd.cmd_data, &tmp_start_play_para, sizeof(tws_m2s_start_play_para_t));

    if (buffer_rw_write(p_tws_info->p_tws_mmm_m2s_pipe, (uint8 *)&tmp_tws_cmd, sizeof(tws_sync_cmd_t)) == FALSE)
    {
        return -1;
    }
    else
    {
#ifdef BT_TWS_DEBUG_CMD_ID
        libc_printf("tws mmm send %d\n", tmp_tws_cmd.cmd_id);
#endif
    }

    return 0;
}

int32 tws_exec_start_play(music_bt_player_t *music_player, tws_m2s_start_play_para_t *p_start_play_para)
{
    if (p_tws_info->tws_need_start_sync_play == FALSE)
    {
        p_tws_info->tws_need_start_sync_play = TRUE;

        libc_memcpy(&(p_tws_info->should_start_con_clock), &(p_start_play_para->should_start_con_clock), sizeof(bt_clock_t));

        p_tws_info->slave_update_play_info_timer = sys_get_ab_timer();
    }

    return 0;
}

bool tws_set_and_wait_sync_play(music_bt_player_t *music_player, bt_clock_t *p_should_start_con_clock)
{
    bool ret = TRUE;
    bt_clock_t tmp_dl_clock;
    uint32 wait_start_dec_sem_timer;

    if ((p_tws_info->start_dec_sem == NULL) || (p_tws_info->start_dec_sem->os_event_cnt != 0))
    {
        libc_printf_warning("tws start_dec_sem invalid!!\n");
        return FALSE;
    }

    p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(&tmp_dl_clock);
    if (p_tws_info->tws_link_connect_flag == FALSE)
    {
        return FALSE;
    }

    libc_printf_info("set start dac play on [%d,0] at [%d,%d]\n", p_should_start_con_clock->bt_clk,
                tmp_dl_clock.bt_clk, tmp_dl_clock.bt_intraslot);

    if (tmp_dl_clock.bt_clk >= p_should_start_con_clock->bt_clk)
    {
        libc_printf_warning("should_start_con_clock too late!!\n");
        return FALSE;
    }

    tws_ture_start_play(music_player, p_should_start_con_clock->bt_clk);

    wait_start_dec_sem_timer = sys_get_ab_timer();
    while (libc_sem_trywait(p_tws_info->start_dec_sem) < 0)
    {
        if ((sys_get_ab_timer() - wait_start_dec_sem_timer) > 200)
        {
            ret = FALSE;
            p_tws_info->tws_get_bt_clk(&tmp_dl_clock);
            libc_printf("start_dec_sem at %d\n", tmp_dl_clock.bt_clk);
            break;
        }

        if (p_tws_info->dev_role == TWS_MASTER)
        {
            if (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN)
            {
                //SBC编码，并送到SBC Input Buffer
                if (sbc_encoder_frame_deal(music_player) < 0)
                {
                    libc_printf_error("sbc_encoder_frame_deal error!\n");
                }
            }
        }
    }

    tws_ture_start_play(music_player, 0);

    if (ret == TRUE)
    {
        libc_printf_info("start dac play at [%d,%d]\n", p_tws_info->start_con_clock.bt_clk,
                                                    p_tws_info->start_con_clock.bt_intraslot);
    }

    return ret;
}

#endif
