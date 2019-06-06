/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件TWS播放主箱与从箱时间差监控及微调CLOCK；帧校验和校准。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

int32 tws_master_adjust_audio_aps(music_bt_player_t *music_player, uint8 aps)
{
    int32 ret = 0;
    bt_clock_t cur_dl_clock;
    uint32 bt_clock = 0;

    p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(&cur_dl_clock);
    if (p_tws_info->tws_link_connect_flag == FALSE)
    {
        return -1;
    }

    bt_clock = (cur_dl_clock.bt_clk & ~(0x03)) + TWS_ADJUST_APS_DELAY_CLOCK;

    if (tws_monitor_aps_adjust_sync(music_player, aps, bt_clock) == 1)
    {
        //需要与副箱一起同时调节Audio APS，设置一段时间后执行
        p_tws_info->need_master_set_audio_aps = TRUE;
        p_tws_info->master_set_audio_aps = aps;
        p_tws_info->master_set_audio_aps_bt_clock = bt_clock;
    }
    else
    {
        //命令发送失败，稍等50ms后再尝试
        ret = -1;
    }

    return ret;
}

int32 tws_slave_adjust_audio_aps_ing(music_bt_player_t *music_player)
{
    int32 ret = 0;
    bt_clock_t cur_dl_clock;

    p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(&cur_dl_clock);
    if (p_tws_info->tws_link_connect_flag == FALSE)
    {
        p_tws_info->slave_set_audio_aps_state = 0;
        p_tws_info->need_slave_set_audio_aps = FALSE;
        return -1;
    }

    if (p_tws_info->slave_set_audio_aps_para.restore_flag == TRUE)
    {
        if (p_tws_info->slave_set_audio_aps_state == 0)
        {
            p_tws_info->slave_set_audio_aps_restore = music_player->audio_aps;
            p_tws_info->slave_set_audio_aps_start_bt_clk = cur_dl_clock.bt_clk;
            set_audio_aps (p_tws_info->slave_set_audio_aps_para.aps);
            music_player->audio_aps = p_tws_info->slave_set_audio_aps_para.aps;
            p_tws_info->slave_set_audio_aps_state = 1;
#ifdef BT_TWS_DEBUG_INFO
            libc_printf("slave set aps fastslow 1st END at %d\n", cur_dl_clock.bt_clk);
#endif
        }
        else if (p_tws_info->slave_set_audio_aps_state == 1)
        {
            if ((cur_dl_clock.bt_clk - p_tws_info->slave_set_audio_aps_start_bt_clk) > p_tws_info->slave_set_audio_aps_para.clock_cnt)
            {
                set_audio_aps (p_tws_info->slave_set_audio_aps_restore);
                music_player->audio_aps = p_tws_info->slave_set_audio_aps_restore;
                p_tws_info->slave_set_audio_aps_state = 0;
                p_tws_info->need_slave_set_audio_aps = FALSE;
#ifdef BT_TWS_DEBUG_INFO
                libc_printf("slave set aps fastslow END at %d\n", cur_dl_clock.bt_clk);
#endif
            }
        }
    }
    else
    {
        if (cur_dl_clock.bt_clk >= p_tws_info->slave_set_audio_aps_para.bt_clock)
        {
            set_audio_aps (p_tws_info->slave_set_audio_aps_para.aps);
            music_player->audio_aps = p_tws_info->slave_set_audio_aps_para.aps;
            p_tws_info->need_slave_set_audio_aps = FALSE;
#ifdef BT_TWS_DEBUG_INFO
            libc_printf("slave set aps sync END at %d\n", cur_dl_clock.bt_clk);
#endif
        }
    }

    return ret;
}

int32 tws_master_wait_adjust_audio_aps_over(music_bt_player_t *music_player)
{
    int32 ret = 0;
    bt_clock_t cur_dl_clock;

    p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(&cur_dl_clock);
    if (p_tws_info->tws_link_connect_flag == FALSE)
    {
        p_tws_info->need_master_set_audio_aps = FALSE;
        p_tws_info->tws_slave_play_info.con_clock.bt_clk = 0; //清除slave记忆信息，需要等slave再上报一次后才会继续调节
        p_tws_info->slave_is_setting_audio_aps_m = FALSE;
        return -1;
    }

    if (p_tws_info->need_master_set_audio_aps == TRUE)
    {
        if (cur_dl_clock.bt_clk >= p_tws_info->master_set_audio_aps_bt_clock)
        {
            set_audio_aps (p_tws_info->master_set_audio_aps);
            music_player->audio_aps = p_tws_info->master_set_audio_aps;
            p_tws_info->need_master_set_audio_aps = FALSE;
            p_tws_info->tws_slave_play_info.con_clock.bt_clk = 0; //清除slave记忆信息，需要等slave再上报一次后才会继续调节
            p_tws_info->slave_is_setting_audio_aps_m = FALSE;
#ifdef BT_TWS_DEBUG_INFO
            libc_printf("master set aps sync END at %d\n", cur_dl_clock.bt_clk);
#endif
        }
    }
    else
    {
        if (cur_dl_clock.bt_clk >= p_tws_info->slave_must_set_audio_aps_ok_bt_clock_m)
        {
#ifdef BT_TWS_DEBUG_INFO
            libc_printf("master wait aps fastslow END at %d\n", cur_dl_clock.bt_clk);
#endif
            p_tws_info->tws_slave_play_info.con_clock.bt_clk = 0; //清除slave记忆信息，需要等slave再上报一次后才会继续调节
            p_tws_info->slave_is_setting_audio_aps_m = FALSE;
        }
    }

    return ret;
}

static uint32 __tws_calc_rel_time(bt_clock_t *p_base_dl_clock, bt_clock_t *p_cur_dl_clock)
{
    uint32 rel_time;

    rel_time = p_cur_dl_clock->bt_clk - p_base_dl_clock->bt_clk;
    rel_time = (rel_time * 312) + (rel_time / 2);
    if (p_cur_dl_clock->bt_intraslot >= p_base_dl_clock->bt_intraslot)
    {
        rel_time += (p_cur_dl_clock->bt_intraslot - p_base_dl_clock->bt_intraslot);
    }
    else
    {
        rel_time -= (p_base_dl_clock->bt_intraslot - p_cur_dl_clock->bt_intraslot);
    }

    return rel_time;
}

//原子操作获取DAC Samples和BT Clock
bool __section__(".rcode") tws_get_dac_samples_bt_clock(music_bt_player_t *music_player, uint32 *p_pcm_buf_remain, bt_clock_t *p_dl_clock)
{
    uint32 temp_flag;
    uint32 pcm_buf_remain;

    temp_flag = sys_local_irq_save();

    if (music_player->is_sending_to_pcm_buf_flag == TRUE)
    {
        pcm_buf_remain = (uint32) (-1);
    }
    else
    {
        pcm_buf_remain = get_pcmbuf_length();
    }

    if (pcm_buf_remain == (uint32) (-1))
    {
        sys_local_irq_restore(temp_flag);
        return FALSE;
    }

    pcm_buf_remain /= 2; //sample点数变为sample对数

    p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(p_dl_clock);
    if (p_tws_info->tws_link_connect_flag == FALSE)
    {
        sys_local_irq_restore(temp_flag);
        return FALSE; //Error 下一帧继续
    }

    *p_pcm_buf_remain = pcm_buf_remain;

    sys_local_irq_restore(temp_flag);

    return TRUE;
}

//return 0表示计算diff有效，2表示下一帧继续
static uint8 __tws_calc_dac_samples_diff(music_bt_player_t *music_player, int32 *p_may_dac_sample_diff)
{
    UINT64 con_clock_diff;
    uint32 dac_samples_diff;
    uint32 slave_may_dac_samples;
    uint32 slave_rel_time;  //相对于主箱开始DAC播放时间点
    uint32 slave_sended_dac_samples;
    uint32 master_rel_time; //相对于主箱开始DAC播放时间点
    uint32 master_sended_dac_samples;
    bt_clock_t cur_dl_clock;
    uint32 pcm_buf_remain;
    uint32 may_dac_sample_diff_abs;
    uint32 audio_clock_hz;
    int32 temp_dac_sample_diff;

    if ((tws_get_dac_samples_bt_clock(music_player, &pcm_buf_remain, &cur_dl_clock) == FALSE)
        || (cur_dl_clock.bt_clk < p_tws_info->tws_slave_play_info.con_clock.bt_clk))
    {
        return 2; //Error 下一帧继续
    }

    master_rel_time = __tws_calc_rel_time(&(p_tws_info->start_con_clock), &cur_dl_clock);
    slave_rel_time = __tws_calc_rel_time(&(p_tws_info->start_con_clock), &(p_tws_info->tws_slave_play_info.con_clock));
    con_clock_diff = __tws_calc_rel_time(&(p_tws_info->tws_slave_play_info.con_clock), &cur_dl_clock);

    master_sended_dac_samples = music_player->send_to_pcm_buf_count - pcm_buf_remain;
    slave_sended_dac_samples = p_tws_info->tws_slave_play_info.dac_samples;

    if (music_player->g_sbc_sample_rate == 44)
    {
        if (music_player->audio_aps == music_player->audio_aps_info.slow_aps)
        {
            audio_clock_hz = (uint32) (44100 + music_player->audio_aps_info.aps_freq_values[0][music_player->audio_aps_info.slow_aps]);
        }
        else
        {
            audio_clock_hz = (uint32) (44100 + music_player->audio_aps_info.aps_freq_values[0][music_player->audio_aps_info.fast_aps]);
        }
    }
    else
    {
        if (music_player->audio_aps == music_player->audio_aps_info.slow_aps)
        {
            audio_clock_hz = (uint32) (48000 + music_player->audio_aps_info.aps_freq_values[1][music_player->audio_aps_info.slow_aps]);
        }
        else
        {
            audio_clock_hz = (uint32) (48000 + music_player->audio_aps_info.aps_freq_values[1][music_player->audio_aps_info.fast_aps]);
        }
    }

    con_clock_diff = con_clock_diff * audio_clock_hz;
    dac_samples_diff = libc_math_udiv64_32(con_clock_diff, 1000000);

    slave_may_dac_samples = slave_sended_dac_samples + dac_samples_diff;

    temp_dac_sample_diff = *p_may_dac_sample_diff = (int32) master_sended_dac_samples - (int32) slave_may_dac_samples;

    //限制最大值为144,uint8有效值,-1作为其他用途；大于144即3ms我们认为是其他异常错误，不要调节clock
    if (*p_may_dac_sample_diff > 0)
    {
        if (*p_may_dac_sample_diff > 144)
        {
            *p_may_dac_sample_diff = 0;
            temp_dac_sample_diff = 144;
        }
        may_dac_sample_diff_abs = temp_dac_sample_diff;
    }
    else
    {
        if (*p_may_dac_sample_diff < -144)
        {
            *p_may_dac_sample_diff = 0;
            temp_dac_sample_diff = -144;
        }
        may_dac_sample_diff_abs = 0 - temp_dac_sample_diff;
    }

    p_tws_info->samples_diff_max_recent_count1++;
    if (p_tws_info->samples_diff_max_recent2 == (uint8)(-1))
    {
        if (p_tws_info->samples_diff_max_recent_count1 == TWS_DIFF_MAX_RECENT_COUNT/2)
        {
            p_tws_info->samples_diff_max_recent2 = 0;
        }
    }
    else
    {
        if (may_dac_sample_diff_abs > p_tws_info->samples_diff_max_recent2)
        {
            p_tws_info->samples_diff_max_recent2 = may_dac_sample_diff_abs;
        }
        if (p_tws_info->samples_diff_max_recent_count1 >= TWS_DIFF_MAX_RECENT_COUNT)
        {
            p_tws_info->samples_diff_max_recent1 = p_tws_info->samples_diff_max_recent2;
            p_tws_info->samples_diff_max_recent2 = 0;
            p_tws_info->samples_diff_max_recent_count1 = TWS_DIFF_MAX_RECENT_COUNT/2;
        }
    }
    if (may_dac_sample_diff_abs > p_tws_info->samples_diff_max_recent1)
    {
        p_tws_info->samples_diff_max_recent1 = may_dac_sample_diff_abs;
        libc_printf("tws max samples diff recent:%d\n", p_tws_info->samples_diff_max_recent1);
    }

    //主副箱时间差太大，且2S内纠正不过来，就置为错误状态，退出对箱播放
    if (may_dac_sample_diff_abs > TWS_SAMPLES_DIFF_MAX)
    {
        libc_printf("dac:%d,[m:%d @ %d];[s:%d,%d @ %d]\n", temp_dac_sample_diff, master_sended_dac_samples, master_rel_time,
                                                            slave_may_dac_samples, slave_sended_dac_samples, slave_rel_time);

        libc_printf_warning("tws samples diff too large, error!! %d\n", temp_dac_sample_diff);
        if (p_tws_info->samples_diff_too_large_timer == 0)
        {
            p_tws_info->samples_diff_too_large_timer = sys_get_ab_timer();
        }
        if ((sys_get_ab_timer() - p_tws_info->samples_diff_too_large_timer) > 2000)
        {
            libc_printf_warning("tws samples diff too large, error!! quit!!\n");
            music_player->music_status.status = MMM_BP_ENGINE_ERROR;
            return 2; //Error 退出对箱播放
        }
    }
    else
    {
        p_tws_info->samples_diff_too_large_timer = 0;
    }

    if (may_dac_sample_diff_abs > p_tws_info->samples_diff_max)
    {
        libc_printf_info("tws max samples diff:%d\n", temp_dac_sample_diff);

        p_tws_info->samples_diff_max = (uint8) may_dac_sample_diff_abs;
    }

#ifdef BT_TWS_DEBUG_INFO
    libc_printf("dac:%d,[m:%d @ %d];[s:%d,%d @ %d]\n", temp_dac_sample_diff, master_sended_dac_samples, master_rel_time,
                                                        slave_may_dac_samples, slave_sended_dac_samples, slave_rel_time);
#endif

    return 0;
}

//sample_cnt>0表示主箱偏快，将副箱CLOCK调快一点；<0表示主箱偏慢，将副箱CLOCK调慢一点
static void __tws_slave_aps_adjust_fast_slow(music_bt_player_t *music_player, int32 sample_cnt)
{
    tws_m2s_set_audio_aps_para_t tmp_set_audio_aps_para;
    uint8 slave_adjust;
    uint32 bt_clock_cnt;
    uint8 s_aps_idx, d_aps_idx;
    int8  aps_freq_diff;

    if (music_player->audio_aps == music_player->audio_aps_info.slow_aps)
    {
        if (sample_cnt > 0)
        {//副箱调快一点
            slave_adjust = music_player->audio_aps_info.slow_tws_faster_aps;
            bt_clock_cnt = sample_cnt;
            s_aps_idx = music_player->audio_aps_info.slow_aps;
            d_aps_idx = music_player->audio_aps_info.slow_tws_faster_aps;
            if (music_player->g_sbc_sample_rate == 44)
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[0][d_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[0][s_aps_idx];
            }
            else
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[1][d_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[1][s_aps_idx];
            }
            bt_clock_cnt = (bt_clock_cnt * 1000000)/aps_freq_diff/312;
            bt_clock_cnt &= ~(0x03);
        }
        else
        {//副箱调慢一点
            slave_adjust = music_player->audio_aps_info.slow_tws_slower_aps;;
            bt_clock_cnt = 0 - sample_cnt;
            s_aps_idx = music_player->audio_aps_info.slow_aps;
            d_aps_idx = music_player->audio_aps_info.slow_tws_slower_aps;
            if (music_player->g_sbc_sample_rate == 44)
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[0][s_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[0][d_aps_idx];
            }
            else
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[1][s_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[1][d_aps_idx];
            }
            bt_clock_cnt = (bt_clock_cnt * 1000000)/aps_freq_diff/312;
            bt_clock_cnt &= ~(0x03);
        }
    }
    else
    {
        if (sample_cnt > 0)
        {//副箱调快一点
            slave_adjust = music_player->audio_aps_info.fast_tws_faster_aps;;
            bt_clock_cnt = sample_cnt;
            s_aps_idx = music_player->audio_aps_info.fast_aps;
            d_aps_idx = music_player->audio_aps_info.fast_tws_faster_aps;
            if (music_player->g_sbc_sample_rate == 44)
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[0][d_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[0][s_aps_idx];
            }
            else
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[1][d_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[1][s_aps_idx];
            }
            bt_clock_cnt = (bt_clock_cnt * 1000000)/aps_freq_diff/312;
            bt_clock_cnt &= ~(0x03);
        }
        else
        {//副箱调慢一点
            slave_adjust = music_player->audio_aps_info.fast_tws_slower_aps;
            bt_clock_cnt = 0 - sample_cnt;
            s_aps_idx = music_player->audio_aps_info.fast_aps;
            d_aps_idx = music_player->audio_aps_info.fast_tws_slower_aps;
            if (music_player->g_sbc_sample_rate == 44)
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[0][s_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[0][d_aps_idx];
            }
            else
            {
                aps_freq_diff = music_player->audio_aps_info.aps_freq_values[1][s_aps_idx] \
                        - music_player->audio_aps_info.aps_freq_values[1][d_aps_idx];
            }
            bt_clock_cnt = (bt_clock_cnt * 1000000)/aps_freq_diff/312;
            bt_clock_cnt &= ~(0x03);
        }
    }

#ifdef BT_TWS_DEBUG_INFO
    libc_printf("set audio aps fastslow:%d\n", bt_clock_cnt);
#endif

    tmp_set_audio_aps_para.bt_clock = 0;
    tmp_set_audio_aps_para.clock_cnt = bt_clock_cnt;
    tmp_set_audio_aps_para.aps = slave_adjust;
    tmp_set_audio_aps_para.restore_flag = TRUE;

    if (tws_update_audio_aps_m2s(music_player, &tmp_set_audio_aps_para) == 0)
    {
        bt_clock_t cur_dl_clock;
        p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(&cur_dl_clock);
        if (p_tws_info->tws_link_connect_flag == TRUE)
        {
            p_tws_info->slave_must_set_audio_aps_ok_bt_clock_m = cur_dl_clock.bt_clk
                + tmp_set_audio_aps_para.clock_cnt + TWS_ADJUST_APS_DELAY_CLOCK;
            p_tws_info->slave_is_setting_audio_aps_m = TRUE;
        }
    }
}

//主箱调节CLOCK，同步副箱调节CLOCK
//return 0表示不需要调节副箱CLOCK，1表示需要调节副箱CLOCK，2表示待定下个循环继续判断
uint8 tws_monitor_aps_adjust_sync(music_bt_player_t *music_player, uint8 aps, uint32 bt_clock)
{
    tws_m2s_set_audio_aps_para_t tmp_set_audio_aps_para;

    tmp_set_audio_aps_para.bt_clock = bt_clock;
    tmp_set_audio_aps_para.clock_cnt = 0;
    tmp_set_audio_aps_para.aps = aps;
    tmp_set_audio_aps_para.restore_flag = FALSE;

#ifdef BT_TWS_DEBUG_INFO
    libc_printf("set audio aps sync:%d\n", bt_clock);
#endif

    if (tws_update_audio_aps_m2s(music_player, &tmp_set_audio_aps_para) == 0)
    {
        p_tws_info->slave_is_setting_audio_aps_m = TRUE;
        return 1;
    }

    return 2;//本次调节Audio APS取消
}

//因主副箱时间差偏大而调节
//return 0表示不需要调节副箱CLOCK，1表示需要调节副箱CLOCK，2表示待定下个循环继续判断
uint8 tws_monitor_aps_adjust_fastslow(music_bt_player_t *music_player)
{
    //计算主箱和从箱速率偏差
    int32 may_dac_sample_diff;
    uint8 ret = 0;

    //尚未收到副箱更新的play_info，无需调整副箱CLOCK
    if (p_tws_info->tws_slave_play_info.con_clock.bt_clk == 0)
    {
        return 0;
    }

    ret = __tws_calc_dac_samples_diff(music_player, &may_dac_sample_diff);

    if (ret == 2)
    {
        return ret;
    }

    //如果主箱比从箱多DAC多于2个采样点，那么就会让副箱调快Audio APS以对齐
    //如果主箱比从箱少DAC多于2个采样点，那么就会让副箱调慢Audio APS以对齐
    if ((may_dac_sample_diff > 2) || (may_dac_sample_diff < -2))
    {
        __tws_slave_aps_adjust_fast_slow(music_player, may_dac_sample_diff);
        ret = 1; //表示主箱与副箱尚未对齐，不能调整主箱CLOCK
    }

    return ret;
}

int32 tws_update_audio_aps_m2s(music_bt_player_t *music_player, tws_m2s_set_audio_aps_para_t *p_set_audio_aps_para)
{
    tws_sync_cmd_t tmp_tws_cmd;

    tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_PLAY;
    tmp_tws_cmd.cmd_id = TWS_SYNC_M2S_SET_AUDIO_APS_CMD;
    tmp_tws_cmd.cmd_para = 0;
    tmp_tws_cmd.cmd_len = sizeof(tws_m2s_set_audio_aps_para_t);
    libc_memcpy(tmp_tws_cmd.cmd_data, p_set_audio_aps_para, sizeof(tws_m2s_set_audio_aps_para_t));

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

int32 tws_exec_update_audio_aps(music_bt_player_t *music_player, tws_m2s_set_audio_aps_para_t *p_set_audio_aps_para)
{
    //设置标志，然后在play_loop中调节Audio APS
    p_tws_info->need_slave_set_audio_aps = TRUE;
    p_tws_info->slave_set_audio_aps_state = 0;
    libc_memcpy(&(p_tws_info->slave_set_audio_aps_para), p_set_audio_aps_para, sizeof(tws_m2s_set_audio_aps_para_t));

#ifdef BT_TWS_DEBUG_INFO
    libc_printf("set_audio_aps:%d, bf:%d\n", p_set_audio_aps_para->aps, music_player->buffer_time_length);
#endif

    return 0;
}

uint16 tws_frame_check_sum_deal(uint8 *frame_data, uint16 frame_len)
{
    uint16 frame_checksum = 0;
    uint16 i;

    for (i = 0; i < frame_len; i++)
    {
        frame_checksum += frame_data[i];
    }

    p_tws_info->ip_frame_id_count %= (TWS_IP_FRAME_MAX_COUNT*10);
    if ((p_tws_info->ip_frame_id_count % TWS_IP_FRAME_MAX_COUNT) == 0)
    {
        p_tws_info->ip_frame_cks = frame_checksum;
        p_tws_info->ip_frame_id_cks = p_tws_info->ip_frame_id_count;
        p_tws_info->need_update_ip_frame_cks = TRUE;
    }
    p_tws_info->ip_frame_id_count++;

    return frame_checksum;
}

int32 tws_update_ip_frame_info_s2m(music_bt_player_t *music_player)
{
    int32 ret = 0;
    uint16 tmp_time_length = music_player->buffer_time_length;
    tws_sync_cmd_t tmp_tws_cmd;
    bool need_update = FALSE;

    if ((p_tws_info->tws_need_start_sync_play == FALSE) && (p_tws_info->tws_ip_frame_info.ip_buf_rtime != tmp_time_length))
    {
        uint16 check_sum, i;
        uint8 *p_raw_data;
        buffer_rw_t temp_input_buf;
        uint32 temp_flag;

        temp_flag = sys_local_intc_save(0x01);
        libc_memcpy(&temp_input_buf, music_player->input_buf, sizeof(buffer_rw_t));
        sys_local_intc_restore(temp_flag);

        p_tws_info->tws_ip_frame_info.ip_buf_rtime = tmp_time_length;

        if ((temp_input_buf.read_ptr == 0) && (temp_input_buf.length <= temp_input_buf.capacity))
        {
            p_tws_info->tws_ip_frame_info.ip_frame_id = (uint16)(-1);
            p_tws_info->tws_ip_frame_info.ip_frame_len = temp_input_buf.length;
            p_raw_data = (uint8 *)(temp_input_buf.raw_data);
            check_sum = 0;
            for (i = 0; i < p_tws_info->tws_ip_frame_info.ip_frame_len; i++)
            {
                check_sum += p_raw_data[i];
            }
            p_tws_info->tws_ip_frame_info.ip_frame_cks = check_sum;
        }
        else
        {
            libc_printf_warning("slave input buffer maybe error!!\n");
        }

        need_update = TRUE;
    }
    else
    {
        if (p_tws_info->need_update_ip_frame_cks == TRUE)
        {
            p_tws_info->tws_ip_frame_info.ip_buf_rtime = tmp_time_length;
            p_tws_info->tws_ip_frame_info.ip_frame_id = p_tws_info->ip_frame_id_cks;
            p_tws_info->tws_ip_frame_info.ip_frame_cks = p_tws_info->ip_frame_cks;
            p_tws_info->need_update_ip_frame_cks = FALSE;

            need_update = TRUE;
        }
    }

    if ((p_tws_info->tws_ip_frame_info.ip_frame_lost == FALSE) && (p_tws_info->tws_slave_frame_lost_flag == TRUE))
    {
        need_update = TRUE;
        p_tws_info->tws_ip_frame_info.ip_frame_lost = p_tws_info->tws_slave_frame_lost_flag;
    }

    if (need_update == TRUE)
    {
        tmp_tws_cmd.cmd_group = TWS_SYNC_GROUP_PLAY;
        tmp_tws_cmd.cmd_id = TWS_SYNC_S2M_IP_FRAME_INFO_EV;
        tmp_tws_cmd.cmd_para = 0;
        tmp_tws_cmd.cmd_len = sizeof(tws_ip_frame_info_t);
        libc_memcpy(tmp_tws_cmd.cmd_data, &(p_tws_info->tws_ip_frame_info), sizeof(tws_ip_frame_info_t));

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
    }

    return ret;
}

int32 tws_exec_update_ip_frame_info(music_bt_player_t *music_player, tws_ip_frame_info_t *p_tws_ip_frame_info)
{
    int32 ret = 0;

    libc_memcpy(&(p_tws_info->tws_slave_ip_frame_info), p_tws_ip_frame_info, sizeof(tws_ip_frame_info_t));

    if (p_tws_ip_frame_info->ip_frame_lost == TRUE)
    {
        libc_printf_error("slave frame lost!\n");
        music_player->music_status.status = MMM_BP_ENGINE_ERROR;
        return -1;
    }

    if (p_tws_info->tws_need_start_sync_play == FALSE)
    {
        if (p_tws_ip_frame_info->ip_frame_id == (uint16)(-1))
        {
            uint16 check_sum, i;
            uint16 check_sum_len;
            uint8 *p_raw_data;
            buffer_rw_t temp_input_buf;
            uint32 temp_flag;

            temp_flag = sys_local_intc_save(0x01);
            libc_memcpy(&temp_input_buf, music_player->input_buf, sizeof(buffer_rw_t));
            sys_local_intc_restore(temp_flag);

            if ((temp_input_buf.read_ptr == 0) && (temp_input_buf.length <= temp_input_buf.capacity))
            {
                check_sum_len = p_tws_ip_frame_info->ip_frame_len;
                p_raw_data = (uint8 *)(temp_input_buf.raw_data);
                check_sum = 0;
                for (i = 0; i < check_sum_len; i++)
                {
                    check_sum += p_raw_data[i];
                }
                if (p_tws_ip_frame_info->ip_frame_cks != check_sum)
                {
                    libc_printf_error("ip frame check sum fail!!%d,%d\n", check_sum_len,temp_input_buf.length);
                    music_player->music_status.status = MMM_BP_ENGINE_ERROR;
                    return -1;
                }
            }
        }

        if (p_tws_ip_frame_info->ip_buf_rtime >= (music_player->music_setting.start_delay_time - 120))
        {
            bt_clock_t cur_dl_clock;
            uint8 i;

            for (i = 0; i < 5; i++)
            {
                p_tws_info->tws_link_connect_flag = p_tws_info->tws_get_bt_clk(&cur_dl_clock);
                if (p_tws_info->tws_link_connect_flag == FALSE)
                {
                    break;
                }
                cur_dl_clock.bt_clk = (cur_dl_clock.bt_clk & ~(0x03)) + TWS_START_PLAY_DELAY_CLOCK;
                if (cur_dl_clock.bt_clk < 4)
                {
                    cur_dl_clock.bt_clk = 4;
                }
                cur_dl_clock.bt_intraslot = 0;

                ret = tws_set_start_play_m2s(music_player, &cur_dl_clock);
                if (ret == 0)
                {
                    break;
                }
                else
                {
                    libc_printf_warning("tws_set_start_play_m2s fail!\n");
                }

                sys_os_time_dly(1);
            }

            if (ret == 0)
            {
                libc_memcpy(&(p_tws_info->should_start_con_clock), &cur_dl_clock, sizeof(bt_clock_t));
                p_tws_info->tws_need_start_sync_play = TRUE;
            }
        }
    }
    else
    {
        if (p_tws_ip_frame_info->ip_frame_id != (uint16)(-1))
        {
            if (p_tws_ip_frame_info->ip_frame_id == p_tws_info->ip_frame_id_cks)
            {
                if (p_tws_ip_frame_info->ip_frame_cks != p_tws_info->ip_frame_cks)
                {
                    libc_printf_error("ip frame check sum fail!!%d,%d,at %d\n", p_tws_ip_frame_info->ip_frame_cks,
                        p_tws_info->ip_frame_cks, p_tws_ip_frame_info->ip_frame_id);
                    music_player->music_status.status = MMM_BP_ENGINE_ERROR;
                    return -1;
                }
            }
        }
    }

    return ret;
}

#endif
