/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  音效参数解析
 ********************************************************************************/


#include "music_player_global.h"

#define AUDIOPP_CHAN_BUF_LEN        (0x400) //POST_PROCESS_FRAME_SAMPLE*4, POST_PROCESS_FRAME_SAMPLE 可取 64/128/256
static uint8 __section__(".bss.audiopp_lrch_buffer") g_audiopp_lrch_buffer[AUDIOPP_CHAN_BUF_LEN*2];
static uint8 __section__(".bss.audiopp_mic_ch_buffer") g_audiopp_mic_ch_buffer[AUDIOPP_CHAN_BUF_LEN];

static uint8 __section__(".bss.echo_buf0") g_audiopp_echo_buf0[ECHO_BUF_SIZE];
static uint8 __section__(".bss.echo_buf1") g_audiopp_echo_buf1[ECHO_BUF_SIZE];
static uint8 __section__(".bss.echo_buf2") g_audiopp_echo_buf2[ECHO_BUF_SIZE];
static uint8 __section__(".bss.echo_buf3") g_audiopp_echo_buf3[ECHO_BUF_SIZE];
static uint8 __section__(".bss.echo_buf4") g_audiopp_echo_buf4[ECHO_BUF_SIZE];
static uint8 __section__(".bss.freq_shift_buf") g_audiopp_freq_shift_buf[FREQ_SHIFT_BUF_SIZE];

int32 _post_process_update_ops(PP_para_info_t * p_info)
{
    return post_process_ops(music_player_global->audiopp_handle, PP_CMD_SET_DAE_PARA, (unsigned int)p_info);
}

void update_PP_para_info(music_player_t *music_player)
{
    dae_config_t *p_dae_cfg;

    if (music_player->audiopp_handle == NULL)
    {
        return;
    }

    p_dae_cfg = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (p_dae_cfg == NULL)
    {
        return;
    }
    music_player->post_process_detail.p_dae_cfg = p_dae_cfg;
    music_player->ok_mic_mute_flag = p_dae_cfg->ok_mic_mute;

    music_player->post_process_detail.update_ops = _post_process_update_ops;
    music_player->post_process_detail.p_pp_p_info = &g_PP_para_info;
    music_player->post_process_detail.p_fp_config = &g_freq_point_cfg;
    music_player->post_process_detail.lch_buffer = g_audiopp_lrch_buffer;
    music_player->post_process_detail.rch_buffer = g_audiopp_lrch_buffer + AUDIOPP_CHAN_BUF_LEN;

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        music_player->post_process_detail.mic_lch_buffer = g_audiopp_mic_ch_buffer;
        if (music_player->buffer_need_reset == 1)
        {
            libc_memset(g_audiopp_echo_buf0, 0x00, sizeof(g_audiopp_echo_buf0));
            libc_memset(g_audiopp_echo_buf1, 0x00, sizeof(g_audiopp_echo_buf1));
            libc_memset(g_audiopp_echo_buf2, 0x00, sizeof(g_audiopp_echo_buf2));
            libc_memset(g_audiopp_echo_buf3, 0x00, sizeof(g_audiopp_echo_buf3));
            libc_memset(g_audiopp_echo_buf4, 0x00, sizeof(g_audiopp_echo_buf4));
            libc_memset(g_audiopp_freq_shift_buf, 0x00, sizeof(g_audiopp_freq_shift_buf));

            music_player->buffer_need_reset = 0;
        }
        music_player->post_process_detail.echo_buf[0] = g_audiopp_echo_buf0;
        music_player->post_process_detail.echo_buf[1] = g_audiopp_echo_buf1;
        music_player->post_process_detail.echo_buf[2] = g_audiopp_echo_buf2;
        music_player->post_process_detail.echo_buf[3] = g_audiopp_echo_buf3;
        music_player->post_process_detail.echo_buf[4] = g_audiopp_echo_buf4;
        music_player->post_process_detail.freq_shift_buf = g_audiopp_freq_shift_buf;
    }
#endif

    music_player->post_process_detail.frame_sample = MY_POST_PROCESS_FRAME_SAMPLE;
    music_player->post_process_detail.sample_rate_khz = music_player->music_info.sample_rate;
    music_player->post_process_detail.aud_output_chan = music_player->music_setting.aud_output_chan;
    music_player->post_process_detail.dae_update_flag = music_player->dae_update_flag;
    music_player->post_process_detail.fade_flag = music_player->fade_flag;
    music_player->post_process_detail.fade_in_time = music_player->music_setting.fade_in_time;
    music_player->post_process_detail.fade_out_time = music_player->music_setting.fade_out_time;
#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        music_player->post_process_detail.mic_mode = MIC_MODE_OK_MIC;
    }
#endif
    update_pp_para_info(&(music_player->post_process_detail));

    music_player->fade_flag = 0;
    music_player->dae_update_flag = 0;
    music_player->dae_freq = music_player->post_process_detail.ret_dae_freq;
}
