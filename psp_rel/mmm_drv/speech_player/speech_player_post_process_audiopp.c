/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件  音效参数解析
 ********************************************************************************/

#include "speech_player_global.h"

#define AUDIOPP_CHAN_BUF_LEN        (0x100) //POST_PROCESS_FRAME_SAMPLE*4, POST_PROCESS_FRAME_SAMPLE 可取 64/128/256，由于采样率低，选择64
uint8 __section__(".bss.audiopp_lrch_buffer") g_audiopp_lrch_buffer[AUDIOPP_CHAN_BUF_LEN*2]; //music l/r chan
uint8 __section__(".bss.audiopp_mic_lrch_buffer") g_audiopp_mic_lrch_buffer[AUDIOPP_CHAN_BUF_LEN]; //mic chan

static uint8 __section__(".bss.echo_buf0") g_audiopp_echo_buf0[ECHO_BUF_SIZE];
static uint8 __section__(".bss.echo_buf1") g_audiopp_echo_buf1[0xC00];
static uint8 __section__(".bss.freq_shift_buf") g_audiopp_freq_shift_buf[FREQ_SHIFT_BUF_SIZE];

PP_para_info_t g_PP_para_info;

int32 _post_process_update_ops(PP_para_info_t * p_info)
{
    return post_process_ops(speech_player_global->audiopp_handle, PP_CMD_SET_DAE_PARA, p_info);
}

void update_PP_para_info(speech_player_t *speech_player)
{
    dae_config_t *p_dae_cfg;

    if (speech_player->audiopp_handle == NULL)
    {
        return;
    }

    p_dae_cfg = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (p_dae_cfg == NULL)
    {
        return;
    }
    speech_player->post_process_detail.p_dae_cfg = p_dae_cfg;
    speech_player->ok_mic_mute_flag = p_dae_cfg->ok_mic_mute;

    if (speech_player->fade_flag == 1)
    {
        speech_player->g_speech_fadein_flag = 1;
    }

    speech_player->post_process_detail.update_ops = _post_process_update_ops;
    speech_player->post_process_detail.p_pp_p_info = &g_PP_para_info;
    speech_player->post_process_detail.p_fp_config = NULL;
    speech_player->post_process_detail.lch_buffer = g_audiopp_lrch_buffer;
    speech_player->post_process_detail.rch_buffer = g_audiopp_lrch_buffer + AUDIOPP_CHAN_BUF_LEN;
    speech_player->post_process_detail.mic_lch_buffer = g_audiopp_mic_lrch_buffer;

#if (SUPPORT_OK_MIC == 1)
    if (speech_player->work_mode == SP_WORK_MODE_MIC)
    {
        if (speech_player->buffer_need_reset == 1)
        {
            libc_memset(g_audiopp_echo_buf0, 0x00, sizeof(g_audiopp_echo_buf0));
            libc_memset(g_audiopp_echo_buf1, 0x00, sizeof(g_audiopp_echo_buf1));
            libc_memset(g_audiopp_freq_shift_buf, 0x00, sizeof(g_audiopp_freq_shift_buf));

            speech_player->buffer_need_reset = 0;
        }
        speech_player->post_process_detail.echo_buf[0] = g_audiopp_echo_buf0;
        speech_player->post_process_detail.echo_buf[1] = g_audiopp_echo_buf1;
        speech_player->post_process_detail.freq_shift_buf = g_audiopp_freq_shift_buf;
    }
#endif

    speech_player->post_process_detail.frame_sample = MY_POST_PROCESS_FRAME_SAMPLE;
    speech_player->post_process_detail.sample_rate_khz = speech_player->sample_rate;
    speech_player->post_process_detail.aud_output_chan = AUD_OUTPUT_LEFT_CHAN;
    speech_player->post_process_detail.dae_update_flag = speech_player->dae_update_flag;
    speech_player->post_process_detail.fade_flag = speech_player->fade_flag;
    speech_player->post_process_detail.fade_in_time = FADE_IN_TIME_DEFAULT;
    speech_player->post_process_detail.fade_out_time = 0;

#if (SUPPORT_OK_MIC == 1)
    if (speech_player->work_mode == SP_WORK_MODE_MIC)
    {
        speech_player->post_process_detail.mic_mode = MIC_MODE_OK_MIC;
    }
    else
#endif
    {
        speech_player->post_process_detail.mic_mode = MIC_MODE_BASIC;
    }

    update_pp_para_info(&(speech_player->post_process_detail));

    speech_player->fade_flag = 0;
    speech_player->dae_update_flag = 0;
    speech_player->dae_freq = speech_player->post_process_detail.ret_dae_freq;
}

void update_PP_enable(speech_player_t *speech_player, bool music_pp_enable, bool mic_pp_enable)
{
    g_PP_para_info.music_common_para.pp_enable = music_pp_enable;
    g_PP_para_info.mic_common_para.pp_enable = mic_pp_enable;

    post_process_ops(speech_player->audiopp_handle, PP_CMD_SET_DAE_PARA, (uint32) &g_PP_para_info);
}
