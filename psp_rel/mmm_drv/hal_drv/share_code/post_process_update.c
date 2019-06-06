/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：音效参数解析和设置
 ********************************************************************************/

#include "audio_post_process.h"
#include "post_process.h"

#define SR_MAX   9

static const uint8 sample_rate_tbl_KHZ[SR_MAX] =
    { 48, 32, 24, 16, 12, 8, 44, 22, 11 };
static const uint16 sample_rate_tbl_1HZ[SR_MAX] =
    { 48000, 32000, 24000, 16000, 12000, 8000, 44100, 22050, 11025 };

void update_PP_para_info_inner(post_process_detail_t *p_pp_detail)
{
    dae_config_t *p_dae_cfg = (dae_config_t *)(p_pp_detail->p_dae_cfg);
    PP_para_info_t *p_PP_para_info = (PP_para_info_t *)(p_pp_detail->p_pp_p_info);
    uint16 dae_freq = 0;
    uint8 i;

    for (i = 0; i < SR_MAX; i++)
    {
        if (sample_rate_tbl_KHZ[i] == p_pp_detail->sample_rate_khz)
        {
            break;
        }
    }

    if (i == SR_MAX)
    {
        libc_printf_warning("invalid dae sample rate! %d\n", p_pp_detail->sample_rate_khz);
        while (1);
    }

    p_PP_para_info->music_common_para.sample_rate = sample_rate_tbl_1HZ[i];
    p_PP_para_info->music_common_para.block_size = p_pp_detail->frame_sample;
    p_PP_para_info->music_common_para.channels = 2;

    p_PP_para_info->music_common_para.pp_buf_addr[0] = (int)p_pp_detail->lch_buffer;
    p_PP_para_info->music_common_para.pp_buf_addr[1] = (int)p_pp_detail->rch_buffer;

    if (p_pp_detail->aud_output_chan == AUD_OUTPUT_DUAL_CHAN)
    {
        p_PP_para_info->music_common_para.output_channel_config = OUTPUT_CHANNEL_NORMAL;
    }
    else if (p_pp_detail->aud_output_chan == AUD_OUTPUT_SWITCH_DAUL_CHAN)
    {
        p_PP_para_info->music_common_para.output_channel_config = OUTPUT_CHANNEL_L_R_SWITCH;
    }
    else if ((p_pp_detail->aud_output_chan == AUD_OUTPUT_MIX_LEFT_CHAN)
            || (p_pp_detail->aud_output_chan == AUD_OUTPUT_MIX_RIGHT_CHAN)
            || (p_pp_detail->aud_output_chan == AUD_OUTPUT_MIX_DAUL_CHAN))
    {
        p_PP_para_info->music_common_para.output_channel_config = OUTPUT_CHANNEL_L_R_MERGE;
        p_PP_para_info->music_common_para.channels = 1;
    }
    else if (p_pp_detail->aud_output_chan == AUD_OUTPUT_LEFT_CHAN)
    {
        p_PP_para_info->music_common_para.output_channel_config = OUTPUT_CHANNEL_L_R_ALL_L;
        p_PP_para_info->music_common_para.channels = 1;
    }
    else//AUD_OUTPUT_RIGHT_CHAN
    {
        p_PP_para_info->music_common_para.output_channel_config = OUTPUT_CHANNEL_L_R_ALL_R;
        p_PP_para_info->music_common_para.channels = 1;
    }

    p_PP_para_info->music_common_para.fade_in_time_ms = p_pp_detail->fade_in_time;
    p_PP_para_info->music_common_para.fade_out_time_ms = p_pp_detail->fade_out_time;
    p_PP_para_info->music_common_para.fade_flag = p_pp_detail->fade_flag;
    p_pp_detail->fade_flag = 0;

    p_PP_para_info->music_common_para.pre_soft_volume = 0 -(short)(p_dae_cfg->precut);
    p_PP_para_info->music_common_para.soft_volume = p_dae_cfg->soft_volume;
    p_PP_para_info->mic_common_para.soft_volume = p_dae_cfg->kt_soft_volume;

    if (p_pp_detail->mic_mode != MIC_MODE_DISABLE)
    {
        p_PP_para_info->mic_common_para.pp_enable = 1;
        p_PP_para_info->mic_common_para.bypass = p_dae_cfg->bypass;
        p_PP_para_info->mic_common_para.sample_rate = sample_rate_tbl_1HZ[i];
        p_PP_para_info->mic_common_para.output_channel_config = OUTPUT_CHANNEL_NORMAL;
        p_PP_para_info->mic_common_para.channels = 1;
        p_PP_para_info->mic_common_para.pp_buf_addr[0] = p_pp_detail->mic_lch_buffer;
        p_PP_para_info->mic_common_para.pp_buf_addr[1] = p_pp_detail->mic_rch_buffer;
        p_PP_para_info->mic_common_para.fade_in_time_ms = p_pp_detail->fade_in_time;
        p_PP_para_info->mic_common_para.fade_out_time_ms = p_pp_detail->fade_out_time;
        p_PP_para_info->mic_common_para.fade_flag = p_pp_detail->fade_flag;

        //通话场景下默认MIX
        p_PP_para_info->mic_mix_config_l = KT_MIX_MODE_MIX;
        p_PP_para_info->mic_mix_config_r = KT_MIX_MODE_MIX;

        if (p_pp_detail->mic_mode == MIC_MODE_OK_MIC)
        {
            p_PP_para_info->mic_mix_config_l = p_dae_cfg->mic_mix_mode_l_chan;
            p_PP_para_info->mic_mix_config_r = p_dae_cfg->mic_mix_mode_r_chan;

            p_PP_para_info->echo_para.echo_enable = p_dae_cfg->echo_enable;
            p_PP_para_info->echo_para.echo_delay = p_dae_cfg->echo_delay;
            p_PP_para_info->echo_para.echo_decay = p_dae_cfg->echo_ratio;
            p_PP_para_info->echo_para.echo_lowpass_fc1 = p_dae_cfg->echo_lowpass_fc1;
            p_PP_para_info->echo_para.echo_lowpass_fc2 = p_dae_cfg->echo_lowpass_fc2;

            p_PP_para_info->afc_para.freq_shift_enable = p_dae_cfg->freq_shift_enable;

            p_PP_para_info->voice_removal_para.voice_removal_enable = p_dae_cfg->voice_removal_enable;

            p_PP_para_info->ducking_effect_para.Ducking_enable = p_dae_cfg->ducking_enable;
            p_PP_para_info->ducking_effect_para.rms_detection_time = p_dae_cfg->ducking_detect_time;
            p_PP_para_info->ducking_effect_para.attack_time = p_dae_cfg->ducking_attack_time;
            p_PP_para_info->ducking_effect_para.release_time = p_dae_cfg->ducking_release_time;
            p_PP_para_info->ducking_effect_para.side_chain_threshold = p_dae_cfg->ducking_side_chain_threshold/2; //0.1dB -> 0.2dB
            p_PP_para_info->ducking_effect_para.main_chain_target_gain = p_dae_cfg->ducking_main_chain_target_gain;

            p_PP_para_info->echo_buf[0] = p_pp_detail->echo_buf[0];
            p_PP_para_info->echo_buf[1] = p_pp_detail->echo_buf[1];
            p_PP_para_info->echo_buf[2] = p_pp_detail->echo_buf[2];
            p_PP_para_info->echo_buf[3] = p_pp_detail->echo_buf[3];
            p_PP_para_info->echo_buf[4] = p_pp_detail->echo_buf[4];
            p_PP_para_info->echo_buf[5] = p_pp_detail->echo_buf[5];
            p_PP_para_info->freq_shift_buf = p_pp_detail->freq_shift_buf;
        }
    }
    else
    {
        p_PP_para_info->mic_common_para.pp_enable = FALSE;
    }

    if (p_pp_detail->dae_update_flag == 1)
    {
        bool last_peq_enable = (p_PP_para_info->peq_para.music_peq_enable || p_PP_para_info->peq_para.mic_peq_enable);
        bool cur_peq_enable = FALSE;

        p_PP_para_info->music_common_para.pp_enable = TRUE;

        if (p_pp_detail->idle_mode == TRUE)
        {
            p_PP_para_info->music_common_para.bypass = TRUE;
        }
        else
        {
            p_PP_para_info->music_common_para.bypass = p_dae_cfg->bypass;
        }

        p_PP_para_info->peq_para.music_peq_enable = p_dae_cfg->peq_enable;
        p_PP_para_info->peq_para.mic_peq_band_num = p_dae_cfg->kt_peq_band_num;
        p_PP_para_info->peq_para.mic_peq_enable = FALSE;
        if ((p_dae_cfg->peq_enable == TRUE) && (p_dae_cfg->kt_peq_band_num > 0))
        {
            p_PP_para_info->peq_para.mic_peq_enable = TRUE;
        }

        cur_peq_enable = (p_PP_para_info->peq_para.music_peq_enable || p_PP_para_info->peq_para.mic_peq_enable);
        if ((last_peq_enable == FALSE) && (cur_peq_enable == TRUE))
        {
            p_PP_para_info->peq_para.peq_change_flag = 1;
        }
        if (p_dae_cfg->aset_mode == TRUE)
        {
            p_PP_para_info->peq_para.peq_change_flag = 1;
            p_PP_para_info->DRC_para.DRC_change_flag = 1;
        }

        if (p_PP_para_info->peq_para.peq_change_flag == 0)
        {
            if (libc_memcmp(p_PP_para_info->peq_para.band_settings, p_dae_cfg->peq_bands, sizeof(p_PP_para_info->peq_para.band_settings)) != 0)
            {
                p_PP_para_info->peq_para.peq_change_flag = 1;
            }
            if (p_dae_cfg->peq_version != 0)
            {
                for (i = 0; i < EQ_NUM_BANDS; i++)
                {
                    if (p_PP_para_info->peq_para.music_eq_id[i] != p_dae_cfg->music_eq_id[i])
                    {
                        p_PP_para_info->peq_para.peq_change_flag = 1;
                        break;
                    }
                }
            }
        }

        if (p_dae_cfg->peq_version == 0)
        {
            for (i = 0; i < EQ_NUM_BANDS; i++)
            {
                p_PP_para_info->peq_para.music_eq_id[i] = 1;
            }
        }
        else
        {
            for (i = 0; i < EQ_NUM_BANDS; i++)
            {
                p_PP_para_info->peq_para.music_eq_id[i] = p_dae_cfg->music_eq_id[i];
            }
        }

        libc_memcpy(p_PP_para_info->peq_para.band_settings, p_dae_cfg->peq_bands, sizeof(p_PP_para_info->peq_para.band_settings));

        p_PP_para_info->music_limiter_para.enable = p_dae_cfg->limiter_enable;
        p_PP_para_info->music_limiter_para.Limiter_threshold = p_dae_cfg->limiter_threshold;
        p_PP_para_info->music_limiter_para.Limiter_attack_time = p_dae_cfg->limiter_attack_time;
        p_PP_para_info->music_limiter_para.Limiter_release_time = p_dae_cfg->limiter_release_time;

        p_PP_para_info->mic_limiter_para.enable = p_dae_cfg->kt_limiter_enable;
        p_PP_para_info->mic_limiter_para.Limiter_threshold = p_dae_cfg->kt_limiter_threshold;
        p_PP_para_info->mic_limiter_para.Limiter_attack_time = p_dae_cfg->kt_limiter_attack_time;
        p_PP_para_info->mic_limiter_para.Limiter_release_time = p_dae_cfg->kt_limiter_release_time;

        if (p_PP_para_info->DRC_para.DRC_change_flag == 0)
        {
            if ((p_PP_para_info->DRC_para.DRC_enable != p_dae_cfg->drc_enable)
                || (p_PP_para_info->DRC_para.DRC_crossover != p_dae_cfg->drc_crossover)
                || (libc_memcmp(&p_PP_para_info->DRC_para.DRC_bands, p_dae_cfg->drc_bands, sizeof(p_PP_para_info->DRC_para.DRC_bands)) != 0))
            {
                p_PP_para_info->DRC_para.DRC_change_flag = 1;
            }
        }

        p_PP_para_info->DRC_para.DRC_enable = p_dae_cfg->drc_enable;
        p_PP_para_info->DRC_para.DRC_crossover = p_dae_cfg->drc_crossover;
        libc_memcpy(p_PP_para_info->DRC_para.DRC_bands, p_dae_cfg->drc_bands, sizeof(p_PP_para_info->DRC_para.DRC_bands));

        if (p_pp_detail->p_fp_config != NULL)
        {
            p_PP_para_info->freq_display_para.freq_display_enable = p_pp_detail->p_fp_config->freq_point_enable;
            p_PP_para_info->freq_display_para.freq_point_num = p_pp_detail->p_fp_config->freq_point_num;
            libc_memcpy(p_PP_para_info->freq_display_para.freq_point, p_pp_detail->p_fp_config->freq_point, sizeof(p_PP_para_info->freq_display_para.freq_point));
        }
        else
        {
            p_PP_para_info->freq_display_para.freq_display_enable = 0;
        }

        p_PP_para_info->noise_gate_para.noise_gate_enable = p_pp_detail->ng_enable;
        p_PP_para_info->noise_gate_para.noise_gate_threshold = NG_THRESHOLD;
        p_PP_para_info->noise_gate_para.noise_gate_attack_time = NG_ATTACK_TIME;
        p_PP_para_info->noise_gate_para.noise_gate_release_time = NG_RELEASE_TIME;
        p_PP_para_info->noise_gate_para.noise_gate_ratio = NG_RATIO;
        p_PP_para_info->noise_gate_para.set0_threshold = NG_SET0_THRESHOLD;

        if (p_dae_cfg->aset_mode == TRUE)
        {
            dae_freq = 200;
        }
        else
        {
            dae_freq = 2;
            if (p_PP_para_info->music_common_para.bypass == 0)
            {
                if (p_PP_para_info->peq_para.music_peq_enable == 1)
                {
                    uint8 band_en_num = 0;

                    for (i = 0; i < (EQ_NUM_BANDS - p_PP_para_info->peq_para.mic_peq_band_num); i++)
                    {
                        if (p_PP_para_info->peq_para.band_settings[i].gain != 0)
                        {
                            band_en_num++;
                        }
                    }

                    dae_freq += 8;
                    dae_freq += (2*band_en_num); //PEQ每段运算量2MHz
                }
                if (p_PP_para_info->music_limiter_para.enable == 1)
                {
                    dae_freq += 8;
                }

                if (p_PP_para_info->noise_gate_para.noise_gate_enable == 1)
                {
                    dae_freq += 30;
                }

                if (p_PP_para_info->music_common_para.channels == 1)
                {
                    dae_freq = dae_freq / 2;
                }

                if (p_PP_para_info->DRC_para.DRC_enable == 1)
                {
                    dae_freq += 59;
                }

                if ((p_PP_para_info->freq_display_para.freq_display_enable == TRUE) && (p_PP_para_info->freq_display_para.freq_point_num > 0))
                {
                    dae_freq += (p_PP_para_info->freq_display_para.freq_point_num*12)/10;
                }

                if (p_PP_para_info->voice_removal_para.voice_removal_enable == 1)
                {
                    dae_freq += 28;
                }
            }

            if (p_PP_para_info->mic_common_para.bypass == 0)
            {
                if ((p_PP_para_info->peq_para.mic_peq_enable == 1) && (p_PP_para_info->peq_para.mic_peq_band_num > 0))
                {
                    uint8 band_en_num = 0;

                    for (i = (EQ_NUM_BANDS - p_PP_para_info->peq_para.mic_peq_band_num); i < EQ_NUM_BANDS; i++)
                    {
                        if (p_PP_para_info->peq_para.band_settings[i].gain != 0)
                        {
                            band_en_num++;
                        }
                    }

                    dae_freq += band_en_num; //PEQ每段运算量1MHz
                }

                if (p_PP_para_info->mic_limiter_para.enable == 1)
                {
                    dae_freq += 4;
                }

                if (p_PP_para_info->echo_para.echo_enable == 1)
                {
                    dae_freq += 15;
                }

                if (p_PP_para_info->afc_para.freq_shift_enable == 1)
                {
                    dae_freq += 38;
                }
            }

            if (p_pp_detail->sample_rate_khz == 48)
            {
                dae_freq += dae_freq/10; //48KHz采样率运算量增大1/10
            }
        }

        p_pp_detail->ret_dae_freq = dae_freq;
    }
    p_pp_detail->dae_update_flag = 0;

    if ((p_PP_para_info->music_common_para.bypass == 0) && (p_PP_para_info->DRC_para.DRC_enable == 1))
    {
        p_PP_para_info->music_common_para.pre_soft_volume += p_dae_cfg->pre_softvol_gain;
        p_PP_para_info->music_common_para.pre_soft_volume += p_dae_cfg->drc_precut;
        p_PP_para_info->music_common_para.soft_volume += p_dae_cfg->post_softvol_gain;
    }

    (p_pp_detail->update_ops)(p_PP_para_info);

    //以下标志设置一次即可
    p_PP_para_info->music_common_para.fade_flag = 0;
    p_PP_para_info->mic_common_para.fade_flag = 0;
    p_PP_para_info->peq_para.peq_change_flag = 0;
    p_PP_para_info->DRC_para.DRC_change_flag = 0;
}
