/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м���������򿪺͹رա�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ƶ�����м��
 * \param[in]   param  �����ֲ����м���Ĳ�����Ŀǰ����Ϊ��
 * \return       �������
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *mmm_bp_open(void *param)
{
    music_bt_player_t *music_player = music_player_global;

    sys_adjust_clk(FREQ_102M, SET_DECODE_CLK);

    /* ��ʼ��settting */
    music_player->music_setting.default_samplerate = 44;
    music_player->music_setting.default_channels = 2;

    music_player->music_setting.aud_output_chan = AUD_OUTPUT_DUAL_CHAN;

    music_player->music_setting.fade_in_time = FADE_IN_TIME_DEFAULT;
    music_player->music_setting.fade_out_time = FADE_OUT_TIME_DEFAULT;

    music_player->music_setting.start_delay_time = MY_BTPLAY_DELAY_TIME_MAX;

    music_player->fade_flag = 0;
    music_player->dae_update_flag = 0;
    music_player->buffer_need_reset = 1;
    music_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;

    music_player->buffer_time_length = (uint16) - 1;
    music_player->buffer_time_min = (uint16) - 1;
    music_player->frame_count = 0;
    music_player->last_frame_timestamp = 0;
    music_player->cur_timestamp = 0;

    //��ʼ��Input Buffer������
    music_player->input_buf = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_BTPLAYPIPE);
    if (music_player->input_buf == NULL)
    {
        while (1);
    }

    music_player->work_mode = (bp_work_mode_e)(uint32) param;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

    p_tws_info = &(music_player->tws_info);

    p_tws_info->dev_role = NORMAL_DEV;
    p_tws_info->dev_spk = TWS_SPK_STEREO;

    if ((music_player->work_mode == BP_WORK_MODE_TWS) || (music_player->work_mode == BP_WORK_MODE_TWS_MIC))
    {
        p_tws_info->p_tws_mmm_m2s_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_MMM_M2S);
        if (p_tws_info->p_tws_mmm_m2s_pipe == NULL)
        {
            while (1);
        }

        p_tws_info->p_tws_mmm_m2s_play_sync_cmd = (tws_sync_cmd_t *) sys_shm_mount(SHARE_MEM_ID_TWS_MMM_M2S_PLAY);
        if (p_tws_info->p_tws_mmm_m2s_play_sync_cmd == NULL)
        {
            while (1);
        }
        libc_memset(p_tws_info->p_tws_mmm_m2s_play_sync_cmd, 0x00, sizeof(tws_sync_cmd_t));

        p_tws_info->p_tws_mmm_s2m_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_MMM_S2M);
        if (p_tws_info->p_tws_mmm_s2m_pipe == NULL)
        {
            while (1);
        }

        if (p_tws_info->p_tws_mmm_s2m_pipe->length != 0)
        {
            buffer_rw_flush(p_tws_info->p_tws_mmm_s2m_pipe);
        }
    }
#endif

    get_audio_aps_info(&(music_player->audio_aps_info));

    //OPEN��״̬�л�ΪSTOPPED
    music_player->music_status.status = MMM_BP_ENGINE_STOPPED;

    return (void *) music_player;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ر����ֲ����м��
 * \param[in]   handel  �򿪵����ֲ����м���������
 * \param[out]  param �ر��м�����豣�����ݵ�ָ�룬Ŀǰ����Ϊ��
 * \return       the result
 * \retval           0 sucess
 * \retval           -1 failed
 *******************************************************************************/
int32 mmm_bp_close(music_bt_player_t *music_player, void *param)
{
#if (SUPPORT_SOFT_KEYTONE == 1)
    if (music_player->kt_play_enable == TRUE)
    {
       keytone_stop(music_player);
    }
#endif

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    //SPDIF�������Ƶ��
    if (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN)
    {
        if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
        {
#if (SUPPORT_HDMI_MODULE == 0)
            sys_adjust_clk(FREQ_0M, SET_LOCK_CLK);
#endif
        }
    }
#endif

    sys_adjust_clk(FREQ_0M, SET_DECODE_CLK);

    //CLOSE֮��״̬�л�ΪNULL
    music_player->music_status.status = MMM_BP_ENGINE_NULL;

    return 0;
}
