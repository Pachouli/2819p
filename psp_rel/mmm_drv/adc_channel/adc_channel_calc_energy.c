/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ADC通道计算能量接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "adc_channel.h"

void adc_channel_calc_energy(adc_channel_info_t *p_ac_info, short *frame_buf, uint16 samples_cnt)
{
    bool is_stereo = TRUE;
    if ((p_ac_info->is_adc_dual_flag == MONO_ADC) || (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX))
    {
        is_stereo = FALSE;
    }

    //能量计算
    energy_calc(frame_buf, samples_cnt, is_stereo, &(p_ac_info->adc_energy.adc_cur_energy), &(p_ac_info->adc_energy.adc_cur_energy_rms));

    //低能量持续时间统计
    if ((p_ac_info->adc_chan_set.adc_lowpw_th_max != 0) || (p_ac_info->adc_chan_set.adc_lowpw_th_rms != 0))
    {
        bool max_match_flag = TRUE;
        bool rms_match_flag = TRUE;

        if (p_ac_info->adc_chan_set.adc_lowpw_th_max != 0)
        {
            if (p_ac_info->adc_energy.adc_cur_energy > p_ac_info->adc_chan_set.adc_lowpw_th_max)
            {
                max_match_flag = FALSE;
            }
        }

        if (p_ac_info->adc_chan_set.adc_lowpw_th_rms != 0)
        {
            if (p_ac_info->adc_energy.adc_cur_energy_rms > p_ac_info->adc_chan_set.adc_lowpw_th_rms)
            {
                rms_match_flag = FALSE;
            }
        }

        if ((max_match_flag == TRUE) && (rms_match_flag == TRUE))
        {
            p_ac_info->adc_lowpw_frame++;
        }
        else
        {
            p_ac_info->adc_lowpw_frame = 0;
        }

        p_ac_info->adc_energy.adc_lowpw_time = p_ac_info->adc_lowpw_frame * samples_cnt / p_ac_info->adc_chan_set.default_samplerate;
    }
}

void adc_channel_calc_mic_energy(adc_channel_info_t *p_ac_info, short *frame_buf, uint16 samples_cnt)
{
    //能量计算
    energy_calc(frame_buf, samples_cnt, FALSE, &(p_ac_info->adc_energy.mic_cur_energy), &(p_ac_info->adc_energy.mic_cur_energy_rms));
}

