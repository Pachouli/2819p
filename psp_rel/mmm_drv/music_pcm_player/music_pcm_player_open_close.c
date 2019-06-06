/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：AUX/FM/USB播歌中间件播放器打开和关闭。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    打开音频播放中间件
 * \param[in]   param  打开音乐播放中间件的参数，目前设置为空
 * \return       操作句柄
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *mmm_pp_open(void *param)
{
    music_pcm_player_t *music_player = music_player_global;

    sys_adjust_clk(FREQ_102M, SET_DECODE_CLK);

    /* 初始化settting */
    music_player->music_setting.default_samplerate = 44;
    music_player->music_setting.default_channels = 2;

    music_player->music_setting.aud_output_chan = AUD_OUTPUT_DUAL_CHAN;

    music_player->music_setting.fade_in_time = FADE_IN_TIME_DEFAULT;
    music_player->music_setting.fade_out_time = FADE_OUT_TIME_DEFAULT;

    music_player->fade_flag = 0;
    music_player->dae_update_flag = 0;
    music_player->buffer_need_reset = 1;
    music_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;

    music_player->work_mode = (pp_work_mode_e)(uint32) param;

    music_player->is_monitoring_flag = TRUE;
    music_player->is_idle_mode = FALSE;

    get_audio_aps_info(&(music_player->audio_aps_info));

    //OPEN后状态切换为STOPPED
    music_player->music_status.status = MMM_PP_ENGINE_STOPPED;

    music_player->record_status.write_pages = 0;
    music_player->record_status.status = RECORD_MODULE_STA_CLOSE;
    music_player->record_status.err_no = MMM_MR_ERR_OK;
    music_player->record_status.start_cluster = 0xffffffff;

    return (void *) music_player;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    关闭音乐播放中间件
 * \param[in]   handel  打开的音乐播放中间件操作句柄
 * \param[out]  param 关闭中间件所需保存数据的指针，目前设置为空
 * \return       the result
 * \retval           0 sucess
 * \retval           -1 failed
 *******************************************************************************/
int32 mmm_pp_close(music_pcm_player_t *music_player, void *param)
{
    //SPDIF播歌解锁频率
    if ((music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        && (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX))
    {
#if (SUPPORT_HDMI_MODULE == 0)
        sys_adjust_clk(FREQ_0M, SET_LOCK_CLK);
#endif
    }

    sys_adjust_clk(FREQ_0M, SET_DECODE_CLK);

    //CLOSE之后状态切换为NULL
    music_player->music_status.status = MMM_PP_ENGINE_NULL;

    return 0;
}
