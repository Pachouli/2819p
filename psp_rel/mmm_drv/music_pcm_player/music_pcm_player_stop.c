/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：AUX/FM/USB播歌中间件停止播放接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"
#include "sysdef.h"

#define DRV_NAME_SIZE 12
static const char aduiopp_name[DRV_NAME_SIZE] = "audiopp.al";

void destroy_decode_thread(music_pcm_player_t *music_player)
{
    music_player->thread_terminate_flag = TRUE;

    while (music_player->thread_terminate_flag == TRUE)
    {
        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }
}

int32 stop_music_decoder_fadeout_start(music_pcm_player_t *music_player, bool pause)
{
    music_player->fade_flag = 2;
    libc_printf_info("fadeout:%d\n", music_player->music_setting.fade_out_time);
    update_PP_para_info(music_player);

    if (pause == TRUE)
    {
        music_player->music_status.status = MMM_PP_ENGINE_FADEOUT_PAUSE;
    }
    else
    {
        music_player->music_status.status = MMM_PP_ENGINE_FADEOUT;
    }

    return 0;
}

int32 stop_music_decoder_fadeout_wait_cp(music_pcm_player_t *music_player)
{
    uint32 fade_out_timeout;

#ifdef PCM_MMM_DEBUG_INFO
    libc_printf("wait pcm buf empty\n");
#endif

    fade_out_timeout = sys_get_ab_timer();

    //等待PCM_BUF的淡出音频全部播放完毕
    while ((music_player->g_pp_pcm_buf_pending & 0x02) == 0)
    {
        //通过检测PCM BUF中采样点数来判断是否为空
        if (get_pcmbuf_length() == 0)
        {
            break;
        }

        //如果500ms还结束不了，应该是除了问题
        if ((sys_get_ab_timer() - fade_out_timeout) > 500)
        {
            libc_printf_warning("fadeout overtime!!\n");
            break;
        }
    }

#ifdef PCM_MMM_DEBUG_INFO
    libc_printf("pcm buf empty\n");
#endif

    return 0;
}

int32 stop_music_decoder_fadeout(music_pcm_player_t *music_player)
{
    int32 ret = 0;

    //启动淡出
    stop_music_decoder_fadeout_start(music_player, TRUE);

    while (1)
    {
        if ((music_player->music_status.status == MMM_PP_ENGINE_PAUSE)
            || (music_player->music_status.status == MMM_PP_ENGINE_ERROR))
        {
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

            ret = stop_music_decoder_fadeout_wait_cp(music_player);

            break;
        }

        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }

    return ret;
}

/* 重置音乐解码器 */
int32 stop_music_decoder(music_pcm_player_t *music_player)
{
    int32 ret = 0;

    //请求停止播放，等待淡出结束后销毁解码线程
    if (music_player->music_status.status == MMM_PP_ENGINE_PLAYING)
    {
        //启动淡出并等待淡出结束
        ret = stop_music_decoder_fadeout(music_player);
    }

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

    //断言：至此状态应该为READY或PAUSE或ERROR
    if ((music_player->music_status.status != MMM_PP_ENGINE_READY)
        && (music_player->music_status.status != MMM_PP_ENGINE_PAUSE)
        && (music_player->music_status.status != MMM_PP_ENGINE_ERROR))
    {
        libc_printf_error("STOP CMD status NO READY&PAUSE&ERROR\n");
        while (1);
    }

    //解码线程不再需要运行，销毁解码线程
    music_player->is_player_loop_enable = FALSE;
    destroy_decode_thread(music_player);

    //停止AUDIO 硬件工作；销毁解码信号量等；
    if (music_player->music_status.status == MMM_PP_ENGINE_READY)
    {
        music_player->music_status.status = MMM_PP_ENGINE_PAUSE; //即将退出，强制为PAUSE状态
    }
    stop_music_decoder_sub(music_player);

    //状态修改为 MMM_PP_ENGINE_STOPPED，STOP流程结束
    music_player->music_status.status = MMM_PP_ENGINE_STOPPED;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    //停止解码后退出录音
    if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta != MMM_RECORD_STOP))
    {
        ret = stop_music_record(music_player);
    }
#endif

    return ret;
}

//中间出现断音也会调用该接口
int32 stop_music_decoder_sub(music_pcm_player_t *music_player)
{
    int32 ret = 0;

#if (SUPPORT_SOFT_KEYTONE == 1)
    if (music_player->kt_play_enable == TRUE)
    {
        keytone_stop(music_player);
    }
#endif

    //清空 buffer
    if (music_player->p_input_buf_r != NULL)
    {
        buffer_rw_flush(music_player->p_input_buf_r);
    }

    if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
    {
        //关闭重采样句柄
        if (music_player->resample_handle_left != NULL)
        {
            pp_resample_close(music_player->resample_handle_left);
            music_player->resample_handle_left = NULL;
        }
        if (music_player->resample_handle_right != NULL)
        {
            pp_resample_close(music_player->resample_handle_right);
            music_player->resample_handle_right = NULL;
            libc_printf("close resample module\n");
        }
    }

    if (music_player->adc_channel_handle != NULL)
    {
        //关闭ADC通道
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_STOP, NULL);
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_CLOSE, NULL);
        sys_drv_uninstall(DRV_GROUP_CHANNEL);
        music_player->adc_channel_handle = NULL;
    }

    //停止AUDIO 硬件工作
    if (music_player->g_aout_init_flag == 1)
    {
        mmm_pp_aout_close(music_player);
        music_player->g_aout_init_flag = 0;
    }

    //销毁解码线程
    if (music_player->pcm_dec_sem != NULL)
    {
        libc_sem_post(music_player->pcm_dec_sem);
        libc_sem_destroy(&(music_player->pcm_dec_sem));
        music_player->pcm_dec_sem = NULL;
    }

    music_player->g_pp_pcm_buf_int_en_flag = FALSE;
    music_player->g_pp_pcm_buf_pending = 0;

    if (music_player->audiopp_handle != NULL)
    {
        //关闭数字音效库
        ret = post_process_ops(music_player->audiopp_handle, PP_CMD_CLOSE, 0);
        libc_memset(&g_PP_para_info, 0x00, sizeof(g_PP_para_info)); //关闭音效库要同时将全局变量复位
        music_player->audiopp_handle = NULL;
        if (ret != 0)
        {
            libc_printf_warning("PP_CMD_CLOSE fail!!\n");
        }

        //卸载数字音效库
        sys_drv_uninstall(SOUND_EFFECT_GROUP);
    }

    if (music_player->music_status.status == MMM_PP_ENGINE_READY)
    {
        ret = start_music_decoder_load_audiopp(music_player);//重新加载音效库以便支持切换采样率
        if (ret != -1)
        {
#if (SUPPORT_OK_MIC == 0)
            if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
#endif
            {
                ret = start_music_decoder_load_adc_chan(music_player);

                if ((music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
                    && (music_player->need_resample == 1))
                {
                    ret = start_resample_module(music_player);
                }
            }
        }
    }

    return ret;
}
