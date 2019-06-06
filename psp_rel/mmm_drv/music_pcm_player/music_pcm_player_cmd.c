/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：AUX/FM/USB播歌中间件向引擎开放命令API入口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"

music_pcm_player_t* music_player_global;

freq_point_config_t g_freq_point_cfg;

uint8 __section__(".bss.ktd_input_buffer") KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];
uint8 __section__(".bss.pcm_dec_buf") PCM_DEC_BUF_ADDR[PCM_DEC_BUF_BLOCK];

//重采样输入输出缓冲
short __section__(".bss.RESAMPLE_IO_BUFFER") RESAMPLE_INPUT_BUFFER[128];
short __section__(".bss.RESAMPLE_IO_BUFFER") RESAMPLE_OUTPUT_BUFFER[2][64];

//创建左右声道重采样句柄所需的缓冲
short __section__(".bss.RESAMPLE_TEMP_BUFF") RESAMPLE_TEMP_BUFF[2][835];
int   __section__(".bss.RESAMPLE_TABLE_BUFF") RESAMPLE_TABLE_BUFF[312];

//左右声道分离缓冲区
uint8 __section__(".bss.RESAMPLE_L_CHAN_BUFF") RESAMPLE_LEFT_CHAN_BUFF[0x800];
uint8 __section__(".bss.RESAMPLE_R_CHAN_BUFF") RESAMPLE_RIGHT_CHAN_BUFF[0x800];
uint8 __section__(".bss.RESAMPLE_LR_SPLIT_BUFF") RESAMPLE_LR_SPLIT_BUFF[0x100];

const uint32 __section__(".rodata.entry") mmm_pp_cmd[] =
{
    (uint32) audio_mmm_pp_cmd,
};

/*! \cond MMM_PP_API */

int32 audio_mmm_pp_cmd(void *handel, mmm_pp_cmd_t cmd, uint32 args)
{
    music_pcm_player_t *music_player = NULL;
    int32 ret = 0;

    if (cmd != MMM_PP_OPEN)
    {
        music_player = (music_pcm_player_t *) handel;
    }

    if ((music_player_global->music_status.status == MMM_PP_ENGINE_NULL) && (cmd != MMM_PP_OPEN))
    {
        ret = -1;
        return ret;
    }

    sys_os_sched_lock();

    switch (cmd)
    {
        case MMM_PP_GET_STATUS:
        libc_memcpy((void *) args, &music_player->music_status, sizeof(mmm_pp_status_t));
        break;

        case MMM_PP_OPEN:
        music_player = mmm_pp_open(args);
        if (music_player == NULL)
        {
            *((music_pcm_player_t **) handel) = NULL;
            ret = -1;
            break;
        }
        *((music_pcm_player_t **) handel) = music_player;
        break;

        case MMM_PP_CLOSE:
        //非法状态
        if (music_player->music_status.status != MMM_PP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        ret = mmm_pp_close(handel, NULL);
        break;

        case MMM_PP_SET_MUSIC_SETTING:
        //非法状态
        if (music_player->music_status.status != MMM_PP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        libc_memcpy(&(music_player->music_setting), (music_pcm_setting_t *) args, sizeof(music_pcm_setting_t));

        //设置立体声/单声道标志
        music_player->is_adc_dual_flag = TRUE;
        if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        {
            if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_AUX)
            {
                if ((music_player->music_setting.adc_chan_set.ain_input_mode != AIN_INPUT_L0_R0_DUAL)
                    && (music_player->music_setting.adc_chan_set.ain_input_mode != AIN_INPUT_L1_R1_DUAL))
                {
                    music_player->is_adc_dual_flag = FALSE;
                }
            }
            else if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_MIC)
            {
                music_player->is_adc_dual_flag = FALSE;
            }
            else if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
            {
                music_player->is_adc_dual_flag = FALSE;
            }
        }

        if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        {
            music_player->music_setting.adc_chan_set.user_type = ADC_USER_LINEIN;
            if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_HDMIARC)
            {
                music_player->music_setting.adc_chan_set.user_type = ADC_USER_HDMI_ARC;
                music_player->music_setting.adc_chan_set.data_input_if = ADC_SOURCE_SPDIFRX;
                libc_printf_info("HDMI ARC NOT SUPPORT BT INBACK!!\n");
            }
        }

        //是否需要微调频率
        music_player->need_adjust_aps = FALSE;
        music_player->adjust_block_thres = MY_ADJUST_LENGTH_THRES;
        if (music_player->music_setting.pcm_source == PCM_SOURCE_USB)
        {
            music_player->need_adjust_aps = TRUE;
        }
        if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        {
            if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
            {
                music_player->need_adjust_aps = TRUE;
            }
            else if ((music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_I2SRX)
                && (music_player->music_setting.adc_chan_set.i2srx_work_mode == I2SRX_WORK_SLAVE))
            {
                music_player->need_adjust_aps = TRUE;
            }
        }

        //限制 start_delay_time
        if (music_player->music_setting.adc_chan_set.user_type == ADC_USER_HDMI_ARC)
        {
            if (music_player->music_setting.start_delay_time > 250)
            {
                music_player->music_setting.start_delay_time = 250;
            }
            if (music_player->music_setting.start_delay_time <= 20)
            {
                music_player->music_setting.start_delay_time = 0;
            }
            else
            {
                music_player->music_setting.start_delay_time -= 8;
            }
        }

        //SPDIF播歌锁频为222M，采样率可支持到最高96KHz
        if ((music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
            && (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX))
        {
            sys_adjust_clk(FREQ_222M, SET_LOCK_CLK);
        }

        music_player->playing_setting_ok = TRUE;
        break;

        //监听开关
        case MMM_PP_CLOSE_MONITORING:
        {
            bool close_monitoring = (bool)(uint32)args;
            if (close_monitoring == TRUE)
            {
                music_player->is_monitoring_flag = FALSE;
            }
            else
            {
                music_player->is_monitoring_flag = TRUE;
            }
        }
        break;

        //设置空闲模式
        case MMM_PP_SET_IDLE_MODE:
        {
            bool is_idle_mode = (bool)(uint32)args;

            music_player->is_idle_mode = is_idle_mode;
        }
        break;

        case MMM_PP_PLAY:
        //非法状态
        if (music_player->music_status.status != MMM_PP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        //如果未经过设置不允许PLAY
        if (music_player->playing_setting_ok == FALSE)
        {
            libc_printf_warning("PCM PLAYER NO setting!\n");
            ret = -1;
            break;
        }

        ret = start_music_decoder(music_player);

#if (SUPPORT_SOFT_KEYTONE == 1)
        if (music_player->work_mode == PP_WORK_KEYTONE)
        {
            if (ret < 0)
            {

            }
            else
            {
                ret = keytone_start(music_player, args);
            }
        }
#endif
        break;

        case MMM_PP_STOP:
        //非法状态
        if (music_player->music_status.status == MMM_PP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        //如果当前处于淡入/淡出状态，必须先等淡入/淡出结束
        while ((music_player->music_status.status == MMM_PP_ENGINE_FADEIN)
            || (music_player->music_status.status == MMM_PP_ENGINE_FADEOUT))
        {
#ifdef PCM_MMM_DEBUG_INFO
            libc_printf("STOP CMD wait fade over!\n");
#endif
            sys_os_sched_unlock();
            sys_os_time_dly(1);
            sys_os_sched_lock();
        }

        ret = stop_music_decoder(music_player);
        break;

        case MMM_PP_SET_EQ:
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);
        set_music_decoder_clk(music_player);
        break;

        case MMM_PP_SET_FREQ_POINT:
        libc_memcpy(&g_freq_point_cfg, (freq_point_config_t *) args, sizeof(freq_point_config_t));
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);
        set_music_decoder_clk(music_player);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
        case MMM_PP_RECORDING_SET_FS:
        libc_memcpy(&(music_player->record_fs), (record_module_fs_para_t *) args, sizeof(record_module_fs_para_t));
        break;

        case MMM_PP_RECORDING_SET_ENCODE:
        libc_memcpy(&(music_player->encode_para), (record_module_encode_para_t *) args, sizeof(record_module_encode_para_t));
        break;

        case MMM_PP_RECORDING_START:
        if ((music_player->music_status.status == MMM_PP_ENGINE_FADEIN)
            || (music_player->music_status.status == MMM_PP_ENGINE_PLAYING))
        {
            music_player->record_enable = TRUE;
            music_player->encode_freq = RECORD_FREQ_MIPS;
            set_music_decoder_clk(music_player);
            ret = start_music_record(music_player);
        }
        else if (music_player->music_status.status == MMM_PP_ENGINE_STOPPED)
        {
            music_player->record_enable = TRUE;
            music_player->encode_freq = RECORD_FREQ_MIPS;
        }
        else
        {
            libc_printf("PP Status Error!\n");
            ret = -1;
        }
        break;

        case MMM_PP_RECORDING_STOP:
        if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta != MMM_RECORD_STOP))
        {
            ret = stop_music_record(music_player);
            music_player->record_enable = FALSE;
            music_player->encode_freq = 0;
            set_music_decoder_clk(music_player);
        }
        break;

        case MMM_PP_RECORDING_GET_STATUS:
        {
            libc_memcpy((void *) args, &(music_player->record_status), sizeof(record_module_status_t));
        }
        break;
#endif

#if (SUPPORT_SOFT_KEYTONE == 1)
        case MMM_PP_KEYTONE_PLAY:
        //非法状态
        if ((music_player->music_status.status != MMM_PP_ENGINE_FADEIN)
            && (music_player->music_status.status != MMM_PP_ENGINE_PLAYING))
        {
            ret = -1;
            break;
        }
        //如果当前还有软按键音在播放，就忽略本次软按键音播放请求
        if (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING)
        {
            libc_printf_warning("keytone is playing, ignore!\n");
            ret = -1;
            break;
        }

        ret = keytone_start(music_player, args);
        break;
#endif

        default:
        libc_printf_warning("MMM_PP cmd not support:%d\n", cmd);
        ret = -1;
        break;
    }

    sys_os_sched_unlock();

    return ret;
}

int32 audio_rcode_init(void)
{
    music_player_global = sys_malloc(sizeof(music_pcm_player_t));
    if (music_player_global == NULL)
    {
        libc_printf("mmm_pcm_player\n");
        return -1;
    }
    else
    {
        libc_memset(music_player_global, 0, sizeof(music_pcm_player_t));
        music_player_global->music_status.status = MMM_PP_ENGINE_NULL;
        return 0;
    }
}

int32 audio_rcode_exit(void)
{
    sys_free(music_player_global);
    music_player_global->music_status.status = MMM_PP_ENGINE_NULL;

    return 0;
}

module_init (audio_rcode_init)
module_exit(audio_rcode_exit)

/*! \endcond */

