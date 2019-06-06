/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件音频相关接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

#define ADC_CHAN_DMA_CHAN  DMA_CHAN_DMA0
#define DAC_CHAN_DMA_CHAN  DMA_CHAN_DMA2

void init_pcm_buf(uint32 half_empty_thres, uint32 half_full_thres)
{
    pcmbuf_setting_t pcmbuf_sett;

    pcmbuf_sett.irq_cbk = speech_pcm_buf_int_cbk;
    pcmbuf_sett.half_empty_thres = half_empty_thres;
    pcmbuf_sett.half_full_thres = half_full_thres;
    pcmbuf_sett.he_ie = TRUE;
    pcmbuf_sett.ep_ie = TRUE;
    pcmbuf_sett.hf_ie = TRUE;
    pcmbuf_sett.fu_ie = FALSE;

    set_pcmbuf(&pcmbuf_sett);
}

void wait_dma_complete(void)
{
    sys_wait_dma_chan_tc (DAC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

    wait_dacfifo_empty();
}

void send_pcm_to_pcm_buf_by_dac_fifo(short *pcm_buffer, uint32 sample_cnt)
{
    dma_chan_setting_t dma_sett;

    libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
    dma_sett.src_mem_addr = (uint8 *) pcm_buffer;
    dma_sett.frame_len = sample_cnt;
    dma_sett.src_type = DMA_SRC_MEM;
    dma_sett.dst_type = DMA_DST_DAC_I2STX;
    dma_sett.start = TRUE;
    sys_set_dma_chan(DAC_CHAN_DMA_CHAN, &dma_sett);

    wait_dma_complete();
}

int32 mmm_sp_aout_open(speech_player_t *speech_player)
{
    dma_chan_setting_t dma_sett;
    dac_setting_t dac_sett;
    ain_setting_t ain_sett;
    adc_setting_t adc_sett;

    ain_sett.ain_input_mode = AIN_INPUT_MIC0_L;
    ain_sett.op_gain.mic_op_gain = speech_player->speech_setting.mic_op_gain;
    ain_sett.ain_pa_mode = FALSE;
    enable_ain(AIN_SOURCE_MIC1, &ain_sett);

    adc_sett.sample_rate = speech_player->sample_rate;
    adc_sett.gain = speech_player->speech_setting.mic_adc_gain;
    enable_adc(AIN_SOURCE_MIC1, &adc_sett);

    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
    dma_sett.dst_mem_addr = (uint8 *) MIC_ADC_BUF_ADDR;
    dma_sett.frame_len = (MY_PCM_ADC_BLOCK_SIZE*2) / sizeof(short);
    dma_sett.irq_cbk = speech_adc_dma_int_cbk;
    dma_sett.src_type = DMA_SRC_ADC;
    dma_sett.dst_type = DMA_DST_MEM;
    dma_sett.hf_ie = TRUE;
    dma_sett.tc_ie = TRUE;
    dma_sett.reload = TRUE;
    dma_sett.start = TRUE;
    sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett);

    libc_memset(&dac_sett, 0x00, sizeof(dac_setting_t));

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (speech_player->resample_info.enable_resample == TRUE)
    {
        dac_sett.sample_rate = speech_player->resample_info.convert_output_rate_hz / 1000;
    }
    else
#endif
    {
        dac_sett.sample_rate = speech_player->sample_rate;
    }
    dac_sett.aps = speech_player->audio_aps_info.normal_aps;

    if (speech_player->speech_setting.aud_output_chan == AUD_OUTPUT_MIX_LEFT_CHAN)
    {
        dac_sett.dacr_mute = TRUE;
    }
    if (speech_player->speech_setting.aud_output_chan == AUD_OUTPUT_MIX_RIGHT_CHAN)
    {
        dac_sett.dacl_mute = TRUE;
    }

    enable_dac(&dac_sett);

    sys_reset_dma_chan (DAC_CHAN_DMA_CHAN);

    return 0;
}

int32 mmm_sp_aout_close(speech_player_t *speech_player)
{
    sys_reset_dma_chan (DAC_CHAN_DMA_CHAN);
    sys_reset_dma_chan (ADC_CHAN_DMA_CHAN);

    reset_pcmbuf();

    disable_ain (AIN_SOURCE_MIC1);
    disable_adc(AIN_SOURCE_MIC1);
    disable_dac();

    return 0;
}

