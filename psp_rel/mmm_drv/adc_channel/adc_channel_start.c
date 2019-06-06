/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ADC通道启动接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "adc_channel.h"

bool adc_channel_feedback_data_func(short *p_data_adc_read, uint16 pcm_adc_buf_block)
{
    uint16 pcm_adc_buf_block_samples = pcm_adc_buf_block/(sizeof(short)*2);

    if (buffer_rw_write((buffer_rw_t *)&(g_adc_channel_info.adc_chan_buf_w), p_data_adc_read, pcm_adc_buf_block) == FALSE)
    {
        libc_printf_warning("adc chan buffer full!\n");
    }
    //计算能量
    adc_channel_calc_energy(&g_adc_channel_info, (short *)p_data_adc_read, pcm_adc_buf_block_samples);

    return TRUE;
}

#if (SUPPORT_MUTE_LOWPOWER == 1)
//fade_flag : 1-淡入，2-淡出
bool adc_channel_mic_lowpower_mute_flag(adc_channel_info_t *p_ac_info, bool *fade_flag)
{
    short lowpower_th;

    *fade_flag = 0;
    if (p_ac_info->lowpower_mute_flag != LOWPOWER_STA_NORMAL)
    {
        lowpower_th = MUTE_LOWPOWER_RL_TH_RMS;
    }
    else
    {
        lowpower_th = MUTE_LOWPOWER_AT_TH_RMS;
    }

    if (p_ac_info->adc_energy.mic_cur_energy_rms <= lowpower_th)
    {
        p_ac_info->highpower_continues = 0;

        if (p_ac_info->lowpower_continues < MUTE_LOWPOWER_AT_TIMES)
        {
            p_ac_info->lowpower_continues++;
        }
        if (p_ac_info->lowpower_continues >= MUTE_LOWPOWER_AT_TIMES)
        {
            if (p_ac_info->lowpower_mute_flag == LOWPOWER_STA_NORMAL)
            {
                p_ac_info->lowpower_mute_flag = LOWPOWER_STA_LOW;
                *fade_flag = MUTE_FADE_OUT;
                libc_printf("mute\n");
            }
        }
    }
    else
    {
        uint8 rl_times;

        p_ac_info->lowpower_continues = 0;

        if (p_ac_info->lowpower_mute_flag == LOWPOWER_STA_OFF)
        {
            rl_times = MUTE_LOWPOWER_RL_OFF_TIMES;
        }
        else
        {
            rl_times = MUTE_LOWPOWER_RL_TIMES;
        }

        if (p_ac_info->highpower_continues < rl_times)
        {
            p_ac_info->highpower_continues++;
        }
        if (p_ac_info->highpower_continues >= rl_times)
        {
            if (p_ac_info->lowpower_mute_flag != LOWPOWER_STA_NORMAL)
            {
                p_ac_info->lowpower_mute_flag = LOWPOWER_STA_NORMAL;
                *fade_flag = MUTE_FADE_IN;
                libc_printf("unmute\n");
            }
        }
    }

    if (p_ac_info->adc_energy.mic_cur_energy_rms < MUTE_LOWPOWER_OFF_TH_RMS)
    {
        if (p_ac_info->off_continues < MUTE_LOWPOWER_OFF_TIMES)
        {
            p_ac_info->off_continues++;
        }
        if (p_ac_info->off_continues >= MUTE_LOWPOWER_OFF_TIMES)
        {
            if (p_ac_info->lowpower_mute_flag != LOWPOWER_STA_OFF)
            {
                p_ac_info->lowpower_mute_flag = LOWPOWER_STA_OFF;
                libc_printf("off\n");
            }
        }
    }
    else
    {
        p_ac_info->off_continues = 0;
    }

    return p_ac_info->lowpower_mute_flag;
}

void adc_channel_mute_fadein(short *temp_buf, int32 points)
{
    int32 point_value;
    int32 i;

    for (i = 1; i <= points; i++)
    {
        point_value = temp_buf[i] * i / points;
        temp_buf[i] = (short) point_value;
    }
}

void adc_channel_mute_fadeout(short *temp_buf, int32 points)
{
    int32 point_value;
    int32 i;

    for (i = points; i > 0; i--)
    {
        point_value = temp_buf[i] * i / points;
        temp_buf[i] = (short) point_value;
    }
}
#endif

//ADC+DMA中断触发，拷贝数据到Input Buffer中；0x01表示半满中断，0x02表示全满中断
void pcm_adc_dma_int_cbk(uint8 pending)
{
    short * p_data_adc_read;
    uint16 pcm_adc_buf_block = g_adc_channel_info.adc_chan_buf_set.pcm_adc_block_size;
    uint16 pcm_adc_buf_block_samples = pcm_adc_buf_block/(sizeof(short)*2);
    uint16 i;
    bool fade_flag;

    if ((pending & (DMA_IP_HF|DMA_IP_TC)) == (DMA_IP_HF|DMA_IP_TC))
    {
        libc_printf_warning("adc dma hf&tc int, miss int!\n");
    }

    if ((pending & DMA_IP_HF) != 0)
    {
        p_data_adc_read = (short *) (g_adc_channel_info.adc_chan_buf_set.adc_chan_buffer_1k);
    }
    else
    {
        p_data_adc_read = (short *) (g_adc_channel_info.adc_chan_buf_set.adc_chan_buffer_1k + pcm_adc_buf_block);
    }

    //MIC&AUX拆分为2路数据，MIC在原空间，AUX在临时空间
    if (g_adc_channel_info.adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
    {
        short *p_data_aux_read = (short *)g_adc_channel_info.mic_aux_temp_buffer_256;
        pcm_adc_buf_block /= 2;

        if (g_adc_channel_info.adc_chan_set.micin_input_mode == AIN_INPUT_MIC0_L)
        {
            for (i = 0; i < pcm_adc_buf_block/sizeof(short); i++)
            {
                *(p_data_adc_read+i) = *(p_data_adc_read + i*2);
                *(p_data_aux_read+i) = *(p_data_adc_read + i*2 + 1);
            }
        }
        else
        {
            for (i = 0; i < pcm_adc_buf_block/sizeof(short); i++)
            {
                *(p_data_aux_read+i) = *(p_data_adc_read + i*2);
                *(p_data_adc_read+i) = *(p_data_adc_read + i*2 + 1);
            }
        }
    }
    //单声道需要去掉1路无用数据
    else if (g_adc_channel_info.is_adc_dual_flag == MONO_ADC)
    {
        bool only_left;

        if (g_adc_channel_info.adc_chan_set.data_input_if == ADC_SOURCE_AUX)
        {
            if ((g_adc_channel_info.adc_chan_set.ain_input_mode == AIN_INPUT_L0_L1_MIX)
                || (g_adc_channel_info.adc_chan_set.ain_input_mode == AIN_INPUT_ONLY_L0)
                || (g_adc_channel_info.adc_chan_set.ain_input_mode == AIN_INPUT_ONLY_L1))
            {
                only_left = TRUE;
            }
            else
            {
                only_left = FALSE;
            }
        }
        else//ADC_SOURCE_MIC
        {
            if (g_adc_channel_info.adc_chan_set.micin_input_mode == AIN_INPUT_MIC0_L)
            {
                only_left = TRUE;
            }
            else
            {
                only_left = FALSE;
            }
        }

        pcm_adc_buf_block /= 2;
        if (only_left == TRUE)
        {
            for (i = 0; i < pcm_adc_buf_block/sizeof(short); i++)
            {
                *(p_data_adc_read+i) = *(p_data_adc_read + i*2);
            }
        }
        else
        {
            for (i = 0; i < pcm_adc_buf_block/sizeof(short); i++)
            {
                *(p_data_adc_read+i) = *(p_data_adc_read + i*2 + 1);
            }
        }
    }

    if (g_adc_channel_info.sample_rate_ok_flag == TRUE)
    {
        if (g_adc_channel_info.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
        {
            if ((g_adc_channel_info.need_resample == 0)
                || (g_adc_channel_info.start_buffer_data_flag == 1))
            {
                spdif_dvd_title_frame_deal(&g_adc_channel_info, p_data_adc_read, pcm_adc_buf_block);
            }
            else
            {
                g_adc_channel_info.start_buffer_data_delay++;
                if (g_adc_channel_info.start_buffer_data_delay >= 100)
                {
                    g_adc_channel_info.start_buffer_data_flag = 1;
                    g_adc_channel_info.start_buffer_data_delay = 0;
                }
            }
        }

        if (g_adc_channel_info.adc_chan_set.data_input_if == ADC_SOURCE_MIC)
        {
            //计算能量
            adc_channel_calc_mic_energy(&g_adc_channel_info, (short *)p_data_adc_read, pcm_adc_buf_block_samples);

#if (SUPPORT_MUTE_LOWPOWER == 1)
            adc_channel_mic_lowpower_mute_flag(&g_adc_channel_info, &fade_flag);
            if (g_adc_channel_info.lowpower_mute_flag != LOWPOWER_STA_NORMAL)
            {
                if (fade_flag == MUTE_FADE_OUT)
                {
                    adc_channel_mute_fadeout(p_data_adc_read, pcm_adc_buf_block/sizeof(short));
                }
                libc_memset((uint8 *)p_data_adc_read, 0x00, pcm_adc_buf_block);
            }
            else if (fade_flag == MUTE_FADE_IN)
            {
                adc_channel_mute_fadein(p_data_adc_read, pcm_adc_buf_block/sizeof(short));
            }
#endif

            if (buffer_rw_write((buffer_rw_t *)&(g_adc_channel_info.mic_chan_buf_w), (uint8 *)p_data_adc_read, pcm_adc_buf_block) == FALSE)
            {
                libc_printf_warning("mic chan buffer full!\n");
            }

        }
        else if (g_adc_channel_info.adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
        {
            //计算能量
            adc_channel_calc_energy(&g_adc_channel_info, (short *)g_adc_channel_info.mic_aux_temp_buffer_256, pcm_adc_buf_block_samples);
            adc_channel_calc_mic_energy(&g_adc_channel_info, (short *)p_data_adc_read, pcm_adc_buf_block_samples);

#if (SUPPORT_MUTE_LOWPOWER == 1)
            adc_channel_mic_lowpower_mute_flag(&g_adc_channel_info, &fade_flag);
            if (g_adc_channel_info.lowpower_mute_flag != LOWPOWER_STA_NORMAL)
            {
                if (fade_flag == MUTE_FADE_OUT)
                {
                    adc_channel_mute_fadeout(p_data_adc_read, pcm_adc_buf_block/sizeof(short));
                }
                libc_memset((uint8 *)p_data_adc_read, 0x00, pcm_adc_buf_block);
            }
            else if (fade_flag == MUTE_FADE_IN)
            {
                adc_channel_mute_fadein(p_data_adc_read, pcm_adc_buf_block/sizeof(short));
            }
#endif

            if (buffer_rw_write((buffer_rw_t *)&(g_adc_channel_info.mic_chan_buf_w), (uint8 *)p_data_adc_read, pcm_adc_buf_block) == FALSE)
            {
                libc_printf_warning("mic chan buffer full!\n");
            }
            if (buffer_rw_write((buffer_rw_t *)&(g_adc_channel_info.adc_chan_buf_w), g_adc_channel_info.mic_aux_temp_buffer_256, pcm_adc_buf_block) == FALSE)
            {
                libc_printf_warning("adc chan buffer full!\n");
            }
        }
        else
        {
            if (g_adc_channel_info.need_resample == 1)
            {
                if (g_adc_channel_info.start_buffer_data_flag == 1)
                {
                    spdif_resample_predeal(&g_adc_channel_info, p_data_adc_read, pcm_adc_buf_block);
                }
            }
            else
            {
                if (buffer_rw_write((buffer_rw_t *)&(g_adc_channel_info.adc_chan_buf_w), (uint8 *)p_data_adc_read, pcm_adc_buf_block) == FALSE)
                {
                    libc_printf_warning("adc chan buffer full!\n");
                }
                //计算能量
                adc_channel_calc_energy(&g_adc_channel_info, (short *)p_data_adc_read, pcm_adc_buf_block_samples);
            }
        }
    }

    if (g_adc_channel_info.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
    {
        spdif_sample_rate_deal(&g_adc_channel_info);
    }
}

int32 adc_channel_adc_open(adc_channel_info_t *p_ac_info)
{
    if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_AUX)
    {
        p_ac_info->sample_rate_ok_flag = TRUE;
        return adc_channel_data_input_if_aux(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_MIC)
    {
        p_ac_info->sample_rate_ok_flag = TRUE;
        return adc_channel_data_input_if_mic(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
    {
        p_ac_info->sample_rate_ok_flag = TRUE;
        return adc_channel_data_input_if_mic_aux(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_I2SRX)
    {
        p_ac_info->sample_rate_ok_flag = TRUE;
        return adc_channel_data_input_if_i2srx(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
    {
        p_ac_info->sample_rate_ok_flag = FALSE;
        p_ac_info->sample_rate_need_update = FALSE;

        p_ac_info->maybe_spdif_title_flag = TRUE;
        p_ac_info->zero_frame_count = 0;
        p_ac_info->need_fadein_flag = FALSE;
        p_ac_info->fadein_point_count = FADEIN_STEP;

        return adc_channel_data_input_if_spdifrx(p_ac_info);
    }
    else
    {
        libc_printf_error("data input interface error!\n");
    }

    return -1;
}

/* 启动音乐解码器 */
int32 adc_channel_start(adc_channel_info_t *p_ac_info)
{
    int32 ret = 0;

    //初始化Input Buffer管理器
    p_ac_info->adc_chan_buf_w.read_ptr = 0;
    p_ac_info->adc_chan_buf_w.write_ptr = 0;
    p_ac_info->adc_chan_buf_w.length = 0;
    p_ac_info->adc_chan_buf_w.raw_data = p_ac_info->adc_chan_buf_set.adc_chan_cache;
    p_ac_info->adc_chan_buf_w.capacity = p_ac_info->adc_chan_buf_set.adc_chan_cache_len;

    p_ac_info->mic_chan_buf_w.read_ptr = 0;
    p_ac_info->mic_chan_buf_w.write_ptr = 0;
    p_ac_info->mic_chan_buf_w.length = 0;
    p_ac_info->mic_chan_buf_w.raw_data = p_ac_info->adc_chan_buf_set.mic_chan_cache;
    p_ac_info->mic_chan_buf_w.capacity = p_ac_info->adc_chan_buf_set.mic_chan_cache_len;

    p_ac_info->mic_aux_temp_buffer_256 = p_ac_info->adc_chan_buf_set.mic_aux_temp_buffer_256;

    p_ac_info->pcm_adc_block_size = p_ac_info->adc_chan_buf_set.pcm_adc_block_size;

    //初始化AUDIO ADC/DAC/DMA，硬件开始工作
    if (p_ac_info->g_aout_init_flag == 0)
    {
        adc_channel_adc_open(p_ac_info);
        p_ac_info->g_aout_init_flag = 1;
    }

#if (SUPPORT_MUTE_LOWPOWER == 1)
    p_ac_info->lowpower_mute_flag = LOWPOWER_STA_OFF;
#endif

    //状态切换为 START
    p_ac_info->ac_status = ADC_CHANNEL_STA_START;

    return ret;
}

