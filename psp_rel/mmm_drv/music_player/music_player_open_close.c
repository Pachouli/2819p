/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  中间件打开、关闭功能实现
 ********************************************************************************/

#include "music_player_global.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    打开音频播放中间件
 * \param[in]   param  打开音乐播放中间件的参数，目前设置为空
 * \return       操作句柄
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *mmm_mp_open(void *param)
{
    music_player_t *music_player = music_player_global;

    sys_adjust_clk(FREQ_102M, SET_DECODE_CLK);

    music_player->music_decode_info.aout = &music_player->aout;

    /* 初始化settting */
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

    //OPEN后状态切换为STOPPED
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
 *    关闭音乐播放中间件
 * \param[in]   handel  打开的音乐播放中间件操作句柄
 * \param[out]  param 关闭中间件所需保存数据的指针，目前设置为空
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
    //CLOSE之后状态切换为NULL
    music_player->music_status.status = MMM_MP_ENGINE_NULL;

    return 0;
}

