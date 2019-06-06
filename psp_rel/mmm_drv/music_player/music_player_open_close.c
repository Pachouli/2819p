/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز����м��  �м���򿪡��رչ���ʵ��
 ********************************************************************************/

#include "music_player_global.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ƶ�����м��
 * \param[in]   param  �����ֲ����м���Ĳ�����Ŀǰ����Ϊ��
 * \return       �������
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *mmm_mp_open(void *param)
{
    music_player_t *music_player = music_player_global;

    sys_adjust_clk(FREQ_102M, SET_DECODE_CLK);

    music_player->music_decode_info.aout = &music_player->aout;

    /* ��ʼ��settting */
    music_player->music_setting.aout_setting.default_samplerate = 44;
    music_player->music_setting.aout_setting.default_channels = 2;

    music_player->music_setting.aud_output_chan = AUD_OUTPUT_DUAL_CHAN;

    music_player->music_setting.fade_in_time = FADE_IN_TIME_DEFAULT;
    music_player->music_setting.fade_out_time = FADE_OUT_TIME_DEFAULT;

    music_player->music_setting.ffb_step = 1000;
    music_player->music_setting.ffb_delay = 100;
    music_player->music_setting.ffb_play_time = 200;

    music_player->storage_io.read = mmm_mp_read;
    music_player->storage_io.seek = mmm_mp_seek;
    music_player->storage_io.tell = mmm_mp_tell;
    music_player->storage_io.write = NULL;
    music_player->storage_io.file_type = 0;
    music_player->storage_io.handel = NULL;

    music_player->music_info.storage_io = &(music_player->storage_io);

    format_check_temp_buf = (uint8 *) (FORMAT_CHECK_TEMP_BUF_ADDR);

    get_audio_aps_info(&(music_player->audio_aps_info));

    music_player->buffer_need_reset = 1;
    music_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;

    music_player->work_mode = (mp_work_mode_e)(uint32) param;

    //OPEN��״̬�л�ΪSTOPPED
    music_player->music_status.status = MMM_MP_ENGINE_STOPPED;
    piAudioparam_global = (AUDIOPARAM *)sys_malloc(sizeof(AUDIOPARAM));
    if(piAudioparam_global == NULL)
    {
        libc_printf("piAudioparam_global\n");
        while(1);
    }
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
int32 mmm_mp_close(music_player_t *music_player, void *param)
{
#if (SUPPORT_SOFT_KEYTONE == 1)
    if (music_player->kt_play_enable == TRUE)
    {
        keytone_stop(music_player);
    }
#endif

    sys_adjust_clk(FREQ_0M, SET_DECODE_CLK);

    if(piAudioparam_global != NULL)
    {
        sys_free(piAudioparam_global);
    }
    //CLOSE֮��״̬�л�ΪNULL
    music_player->music_status.status = MMM_MP_ENGINE_NULL;

    return 0;
}

