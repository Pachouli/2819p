/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ADC通道音频配置接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "adc_channel.h"

#define ADC_CHAN_DMA_CHAN  DMA_CHAN_DMA0

static void adc_dma_sett(adc_channel_info_t *p_ac_info, uint8 dma_src)
{
    dma_chan_setting_t dma_sett;

    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
    dma_sett.dst_mem_addr = p_ac_info->adc_chan_buf_set.adc_chan_buffer_1k;
    dma_sett.frame_len = (p_ac_info->adc_chan_buf_set.pcm_adc_block_size*2) / sizeof(short);
    dma_sett.irq_cbk = pcm_adc_dma_int_cbk;
    dma_sett.src_type = dma_src;
    dma_sett.dst_type = DMA_DST_MEM;
    dma_sett.hf_ie = TRUE;
    dma_sett.tc_ie = TRUE;
    dma_sett.reload = TRUE;
    dma_sett.start = TRUE;
    sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett);
}

int32 adc_channel_data_input_if_aux(adc_channel_info_t *p_ac_info)
{
    ain_setting_t ain_sett;
    adc_setting_t adc_sett;

    p_ac_info->is_adc_dual_flag = DUAL_ADC;
    if ((p_ac_info->adc_chan_set.ain_input_mode != AIN_INPUT_L0_R0_DUAL)
        && (p_ac_info->adc_chan_set.ain_input_mode != AIN_INPUT_L1_R1_DUAL))
    {
        p_ac_info->is_adc_dual_flag = MONO_ADC;
    }

    ain_sett.op_gain.ain_op_gain = p_ac_info->adc_chan_set.ain_op_gain;
    ain_sett.ain_pa_mode = FALSE;
    ain_sett.ain_input_mode = p_ac_info->adc_chan_set.ain_input_mode;
    enable_ain(AIN_SOURCE_AUX, &ain_sett);

    adc_sett.sample_rate = p_ac_info->adc_chan_set.default_samplerate;
    adc_sett.gain = p_ac_info->adc_chan_set.ain_adc_gain;
    enable_adc(AIN_SOURCE_AUX, &adc_sett);

    adc_dma_sett(p_ac_info, DMA_SRC_ADC);

    return 0;
}

int32 adc_channel_data_input_if_aux_exit(adc_channel_info_t *p_ac_info)
{
    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    disable_adc (AIN_SOURCE_AUX);
    disable_ain(AIN_SOURCE_AUX);

    return 0;
}

int32 adc_channel_data_input_if_mic(adc_channel_info_t *p_ac_info)
{
    ain_setting_t ain_sett;
    adc_setting_t adc_sett;

    p_ac_info->is_adc_dual_flag = MONO_ADC;

    ain_sett.op_gain.ain_op_gain = p_ac_info->adc_chan_set.micin_op_gain;
    ain_sett.ain_pa_mode = FALSE;
    ain_sett.ain_input_mode = p_ac_info->adc_chan_set.micin_input_mode;
    enable_ain(AIN_SOURCE_MIC1, &ain_sett);

    adc_sett.sample_rate = p_ac_info->adc_chan_set.default_samplerate;
    adc_sett.gain = p_ac_info->adc_chan_set.micin_adc_gain;
    enable_adc(AIN_SOURCE_MIC1, &adc_sett);

    adc_dma_sett(p_ac_info, DMA_SRC_ADC);

    return 0;
}

int32 adc_channel_data_input_if_mic_exit(adc_channel_info_t *p_ac_info)
{
    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    disable_adc (AIN_SOURCE_MIC1);
    disable_ain(AIN_SOURCE_MIC1);

    return 0;
}

int32 adc_channel_data_input_if_mic_aux(adc_channel_info_t *p_ac_info)
{
    ain_setting_t ain_sett;
    adc_setting_t adc_sett;

    p_ac_info->is_adc_dual_flag = DUAL_ADC;

    ain_sett.op_gain.ain_op_gain = p_ac_info->adc_chan_set.micin_op_gain;
    ain_sett.ain_pa_mode = FALSE;
    ain_sett.ain_input_mode = p_ac_info->adc_chan_set.micin_input_mode;
    enable_ain(AIN_SOURCE_MIC1, &ain_sett);

    adc_sett.sample_rate = p_ac_info->adc_chan_set.default_samplerate;
    adc_sett.gain = p_ac_info->adc_chan_set.micin_adc_gain;
    enable_adc(AIN_SOURCE_MIC1, &adc_sett);

    ain_sett.op_gain.ain_op_gain = p_ac_info->adc_chan_set.ain_op_gain;
    ain_sett.ain_pa_mode = FALSE;
    ain_sett.ain_input_mode = p_ac_info->adc_chan_set.ain_input_mode;
    enable_ain(AIN_SOURCE_AUX, &ain_sett);

    adc_sett.sample_rate = p_ac_info->adc_chan_set.default_samplerate;
    adc_sett.gain = p_ac_info->adc_chan_set.micin_adc_gain;
    enable_adc(AIN_SOURCE_AUX, &adc_sett);

    adc_dma_sett(p_ac_info, DMA_SRC_ADC);

    return 0;
}

int32 adc_channel_data_input_if_mic_aux_exit(adc_channel_info_t *p_ac_info)
{
    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    disable_adc (AIN_SOURCE_MIC1);
    disable_ain(AIN_SOURCE_MIC1);
    disable_adc (AIN_SOURCE_AUX);
    disable_ain(AIN_SOURCE_AUX);

    return 0;
}

int32 adc_channel_data_input_if_i2srx(adc_channel_info_t *p_ac_info)
{
    i2srx_setting_t i2srx_sett;

    p_ac_info->is_adc_dual_flag = DUAL_ADC;

    i2srx_sett.sample_rate = p_ac_info->adc_chan_set.default_samplerate;
    i2srx_sett.i2srx_work_mode = p_ac_info->adc_chan_set.i2srx_work_mode;
    i2srx_sett.i2srx_slave_mclk_use_inner = p_ac_info->adc_chan_set.i2srx_slave_mclk_use_inner;
    i2srx_sett.i2srx_width = p_ac_info->adc_chan_set.i2srx_width;
    i2srx_sett.i2srx_data_mode = p_ac_info->adc_chan_set.i2srx_data_mode;
    enable_i2srx(&i2srx_sett);

    adc_dma_sett(p_ac_info, DMA_SRC_I2S_RX);

    return 0;
}

int32 adc_channel_data_input_if_i2srx_exit(adc_channel_info_t *p_ac_info)
{
    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    disable_i2srx();

    return 0;
}

int32 adc_channel_data_input_if_spdifrx(adc_channel_info_t *p_ac_info)
{
    p_ac_info->is_adc_dual_flag = DUAL_ADC;

    enable_spdifrx();

    adc_dma_sett(p_ac_info, DMA_SRC_SPDIF_RX);

    return 0;
}

int32 adc_channel_data_input_if_spdifrx_exit(adc_channel_info_t *p_ac_info)
{
    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    disable_spdifrx();

    return 0;
}
