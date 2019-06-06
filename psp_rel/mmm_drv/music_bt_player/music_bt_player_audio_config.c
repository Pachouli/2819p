/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件音频相关接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"

void init_pcm_buf(uint32 half_empty_thres, uint32 half_full_thres)
{
    pcmbuf_setting_t pcmbuf_sett;

    pcmbuf_sett.irq_cbk = bt_pcm_buf_int_cbk;
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
    sys_wait_dma_chan_tc (DMA_CHAN_DMA2, WAIT_DMA_TC_ALWAYS);

    wait_dacfifo_empty();
}

void send_pcm_to_pcm_buf_by_dac_fifo(short *pcm_buffer, uint32 sample_cnt, bool start)
{
    dma_chan_setting_t dma_sett;

    libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
    dma_sett.src_mem_addr = (uint8 *) pcm_buffer;
    dma_sett.frame_len = sample_cnt;
    dma_sett.src_type = DMA_SRC_MEM;
    dma_sett.dst_type = DMA_DST_DAC_I2STX;
    dma_sett.start = start;
    sys_set_dma_chan(DMA_CHAN_DMA2, &dma_sett);

    wait_dma_complete();
}

void __section__(".rcode") send_pcm_to_pcm_buf_by_dac_fifo_start(void)
{
    sys_start_dma_chan(DMA_CHAN_DMA2, 0);
    sys_wait_dma_chan_tc(DMA_CHAN_DMA2, WAIT_DMA_TC_ALWAYS);
    wait_dacfifo_empty();
}

int32 mmm_bp_aout_open(music_bt_player_t *music_player)
{
    dac_setting_t dac_sett;

    libc_memset(&dac_sett, 0x00, sizeof(dac_setting_t));
    dac_sett.sample_rate = music_player->music_setting.default_samplerate;
    dac_sett.aps = music_player->audio_aps_info.normal_aps;

    if (music_player->music_setting.aud_output_chan == AUD_OUTPUT_MIX_LEFT_CHAN)
    {
        dac_sett.dacr_mute = TRUE;
    }
    if (music_player->music_setting.aud_output_chan == AUD_OUTPUT_MIX_RIGHT_CHAN)
    {
        dac_sett.dacl_mute = TRUE;
    }
    enable_dac(&dac_sett);

    sys_reset_dma_chan (DMA_CHAN_DMA2);

    return 0;
}

int32 mmm_bp_aout_close(music_bt_player_t *music_player)
{
    reset_pcmbuf();

    disable_dac();

    return 0;
}

