/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  解码线程销毁、解码器停止功能实现
 ********************************************************************************/

#include "music_player_global.h"

#define DRV_NAME_SIZE 12
static const uint8 DRV_NAME_ARRAY[][DRV_NAME_SIZE] =
{
    "dec_mp3.al",
    "dec_wma.al",
    "dec_wav.al",
#if (SUPPORT_APE_TYPE == 1)
    "dec_ape.al",
#endif
#if (SUPPORT_FLAC_TYPE == 1)
    "dec_flac.al",
#endif
};
static const uint8 aduiopp_name[DRV_NAME_SIZE] = "audiopp.al";


void destroy_decode_thread(music_player_t *music_player)
{
    music_player->thread_terminate_flag = TRUE;
    while (music_player->thread_terminate_flag == TRUE)
    {
        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }
}

#if (SUPPORT_OK_MIC == 1)
void destroy_ok_mic_thread(music_player_t *music_player)
{
    music_player->ok_mic_thread_terminate_flag = TRUE;
    while (music_player->ok_mic_thread_terminate_flag == TRUE)
    {
        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }
}
#endif

int32 stop_music_decoder_wait_pcmbuf_empty(music_player_t *music_player)
{
    uint32 fade_out_timeout;

#if (SUPPORT_SOFT_KEYTONE == 1)
    //等待按键音播放完成
    while ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
    {
        libc_printf("wait keytone stop!\n");
        keytone_playend_deal(music_player);
    }
    if (music_player->kt_play_enable == TRUE)
    {
        keytone_stop(music_player);
    }
#endif

#ifdef MUSIC_MMM_DEBUG_INFO
    libc_printf("wait pcm buf empty\n");
#endif

    fade_out_timeout = sys_get_ab_timer();
    //等待PCM_BUF的淡出音频全部播放完毕
    while ((music_player->g_mp_pcm_buf_pending & 0x02) == 0)
    {
        //通过检测PCM BUF中采样点数来判断是否为空
        if (get_pcmbuf_length() == 0)
        {
            break;
        }

        //如果500ms还结束不了，应该是除了问题
        if ((sys_get_ab_timer() - fade_out_timeout) > 500)
        {
            libc_printf_warning("fadeout overtime!\n");
            break;
        }
    }

    //PCM_BUF末尾必须是zero点，否则下次播放声音会有杂音
    if ((music_player->g_mp_pcm_buf_pending & 0x02) != 0)
    {
        short zero_samples[4];
        libc_memset(zero_samples, 0x00, sizeof(short)*4);
        send_pcm_to_pcm_buf_by_dac_fifo(zero_samples, 4);
        sys_mdelay(1);
    }

#ifdef MUSIC_MMM_DEBUG_INFO
    libc_printf("pcm buf empty\n");
#endif
    return 0;
}

int32 stop_music_decoder_fadeout(music_player_t *music_player)
{
    int32 ret = 0;

    //启动淡出
    music_player->fade_flag = 2;
    update_PP_para_info(music_player);
    music_player->music_status.status = MMM_MP_ENGINE_FADEOUT;

    libc_printf_info("fadeout:%d\n", music_player->music_setting.fade_out_time);

    while (1)
    {
        if ((music_player->music_status.status == MMM_MP_ENGINE_PAUSED)
            || (music_player->music_status.status == MMM_MP_ENGINE_PLAYEND)
            || (music_player->music_status.status == MMM_MP_ENGINE_ERROR))
        {
            stop_music_decoder_wait_pcmbuf_empty(music_player);

            break;
        }

        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }

    return ret;
}

/* 重置音乐解码器 */
int32 stop_music_decoder(music_player_t *music_player)
{
    int32 ret = 0;

    //请求停止播放，等待淡出结束后销毁解码线程
    if (music_player->music_status.status == MMM_MP_ENGINE_PLAYING)
    {
        ret = stop_music_decoder_fadeout(music_player);
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    //等待按键音播放完成
    if (music_player->g_aout_init_flag == TRUE)
    {
        while ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
        {
            libc_printf("wait keytone stop!\n");
            keytone_playend_deal(music_player);
        }
    }
    if (music_player->kt_play_enable == TRUE)
    {
        keytone_stop(music_player);
    }
#endif

    //断言：至此状态应该为PLAYEND或PAUSE或ERROR
    if ((music_player->music_status.status != MMM_MP_ENGINE_PLAYEND)
        && (music_player->music_status.status != MMM_MP_ENGINE_PAUSED)
        && (music_player->music_status.status != MMM_MP_ENGINE_ERROR))
    {
        libc_printf_error("STOP CMD status NO PAUSE&ERROR!\n");
        while (1);
    }

    //解码线程不再需要运行，销毁解码线程
    music_player->is_player_loop_enable = FALSE;
    destroy_decode_thread(music_player);

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        music_player->is_ok_mic_loop_enable = FALSE;
        destroy_ok_mic_thread(music_player);
    }
#endif

    //停止AUDIO 硬件工作
    if (music_player->g_aout_init_flag == TRUE)
    {
        mmm_mp_aout_close(music_player, NULL);
        music_player->g_aout_init_flag = FALSE;
    }

    //销毁解码信号量
    if (music_player_global->music_pcmbuf_sem != NULL)
    {
        libc_sem_post(music_player_global->music_pcmbuf_sem);
        libc_sem_destroy(&(music_player_global->music_pcmbuf_sem));
        music_player_global->music_pcmbuf_sem = NULL;
    }
    music_player_global->g_mp_pcm_buf_int_en_flag = FALSE;
    music_player_global->g_mp_pcm_buf_pending = 0;

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (music_player->resample_info.enable_resample == TRUE)
    {
        //关闭重采样句柄
        if (music_player->resample_info.resample_handle_left != NULL)
        {
            pp_resample_close(music_player->resample_info.resample_handle_left);
            music_player->resample_info.resample_handle_left = NULL;
        }
        if (music_player->resample_info.resample_handle_right != NULL)
        {
            pp_resample_close(music_player->resample_info.resample_handle_right);
            music_player->resample_info.resample_handle_right = NULL;
        }
    }
#endif

    //关闭音效库
    ret = post_process_ops(music_player->audiopp_handle, PP_CMD_CLOSE, 0);
    music_player->audiopp_handle = NULL;
    if (ret != 0)
    {
        libc_printf_error("PP_CMD_CLOSE fail!\n");
    }

    //卸载数字音效库
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    //CLOSE解码库
    ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CLOSE, (uint32) NULL);
    music_player->audiodec_handle = NULL;
    if (ret < 0)
    {
        libc_printf_error("AD_CMD_CLOSE fail!\n");
    }

    //卸载解码库
    if (music_player->audiodec_type != 0)
    {
        sys_drv_uninstall (CODEC_GROUP);
        music_player->audiodec_type = 0;
    }

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        //关闭ADC通道
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_STOP, NULL);
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_CLOSE, NULL);
        sys_drv_uninstall(DRV_GROUP_CHANNEL);
    }
#endif

    //状态修改为 MMM_BP_ENGINE_FILESET，STOP流程结束
    music_player->music_status.status = MMM_MP_ENGINE_FILESET;

    //实时播放控制的信息需要清零
    music_player->music_ffrev_flag = 0;
    music_player->music_seek_flag = 0;

    return ret;
}

