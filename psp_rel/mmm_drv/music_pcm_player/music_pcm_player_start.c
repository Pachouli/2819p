/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：AUX/FM/USB播歌中间件开始播放接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"
#include "sysdef.h"

uint8 __section__(".bss.pcm_adc_buffer") PCM_ADC_BUF_ADDR[PCM_ADC_BUF_LEN];

#define PCM_INPUT_BUF_BLOCK_COUNT    (12)
#define PCM_INPUT_BUF_LEN   (PCM_ADC_BUF_BLOCK * PCM_INPUT_BUF_BLOCK_COUNT)
uint8 __section__(".bss.input_buffer") PCM_INPUT_BUF_ADDR[PCM_INPUT_BUF_LEN];

uint8 __section__(".bss.usound_mic_adc") USOUND_MIC_ADC[PCM_ADC_BUF_BLOCK];
#define USOUND_MIC_CACHE_LEN  0x400
uint8 __section__(".bss.usound_mic_cache") USOUND_MIC_CACHE[USOUND_MIC_CACHE_LEN];

#if (SUPPORT_OK_MIC == 1)
uint8 __section__(".bss.mic_temp_buffer") ok_mic_temp_buffer[0x100];
uint8 __section__(".bss.mic_aux_temp_buffer") ok_mic_aux_temp_buffer[0x100];
#endif

#define ARC_PCM_INPUT_BUF_BLOCK_COUNT    (99)
#define ARC_PCM_INPUT_BUF_LEN   (PCM_ADC_BUF_BLOCK * ARC_PCM_INPUT_BUF_BLOCK_COUNT)
uint8 __section__(".bss.arc_input_buffer") ARC_PCM_INPUT_BUF_ADDR[ARC_PCM_INPUT_BUF_LEN];

uint8 __section__(".stack.ap_high_stack_ptos")ap_high_stk_ptos[1];//动态数组
extern uint16 _AP_HIGH_STK_SIZE;

#define DRV_NAME_SIZE 12
static const char aduiopp_name[DRV_NAME_SIZE] = "audiopp.al";
static const char adc_channel_name[DRV_NAME_SIZE] = "adc_chan.al";

int32 creat_decode_thread(music_pcm_player_t *music_player)
{
    pthread_param_t player_thread_para;

    player_thread_para.start_rtn = (void *)player_loop;
    player_thread_para.arg = (void *) music_player;
    player_thread_para.ptos = (OS_STK *) ap_high_stk_ptos;
    player_thread_para.stk_size = (uint32)&_AP_HIGH_STK_SIZE;

    /* 创建中间件线程并设置属性 */
    if (libc_pthread_create(&player_thread_para, AP_BACK_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        music_player = NULL;
        return -1;
    }

    return 0;
}

int32 start_music_decoder_load_audiopp(music_pcm_player_t *music_player)
{
    int32 ret = 0;

    //加载数字音效库
    ret = (int32) sys_drv_install(SOUND_EFFECT_GROUP, 0, aduiopp_name);
    if (ret == -1)
    {
        return -1;
    }

    //打开数字音效库
    ret = post_process_ops(&(music_player->audiopp_handle), PP_CMD_OPEN, 0);
    if ((ret == -1) || (music_player->audiopp_handle == NULL))
    {
        music_player->audiopp_handle = NULL;
        ret = -1;
        libc_printf_warning("audiopp open fail!\n");

        goto decoder_load_audiopp_fail_0;
    }

    return ret;

    decoder_load_audiopp_fail_0:

    //卸载数字音效库
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    return ret;
}

int32 start_music_decoder_load_adc_chan(music_pcm_player_t *music_player)
{
    adc_channel_buffer_setting_t adc_chan_buffer_set;
    adc_channel_resample_buffer_setting_t adc_chan_resample_buffer_set;
    int32 ret = 0;

    //打开ADC通道，设置参数，并启动ADC通道
    if (sys_drv_install(DRV_GROUP_CHANNEL, 0, adc_channel_name) == -1)
    {
        return -1;
    }

    ret = adc_channel_cmd(&(music_player->adc_channel_handle), ADC_CHANNEL_OPEN, NULL);
    if ((ret == -1) || (music_player->adc_channel_handle == NULL))
    {
        ret = -1;
        libc_printf_warning("audiopp open fail!\n");

        goto decoder_load_adc_chan_fail_0;
    }

    ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SETTING, &(music_player->music_setting.adc_chan_set));
    if (ret == -1)
    {
        goto decoder_load_adc_chan_fail_1;
    }

    libc_memset(&adc_chan_buffer_set, 0x00, sizeof(adc_chan_buffer_set));

    if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
    {
        adc_chan_buffer_set.adc_chan_buffer_1k = PCM_ADC_BUF_ADDR;
    }
#if (SUPPORT_OK_MIC == 1)
    else
    {
        adc_chan_buffer_set.adc_chan_buffer_1k = USOUND_MIC_ADC;
    }
#endif
    if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
    {
        adc_chan_buffer_set.pcm_adc_block_size = PCM_ADC_BUF_BLOCK; //DMA Block稍长一点，可以更稳定支持96KHz和88KHz采样
    }
    else
    {
        adc_chan_buffer_set.pcm_adc_block_size = PCM_ADC_BUF_BLOCK/2; //DMA Block短一点，延时可以短一点
    }
    if (music_player->music_setting.adc_chan_set.user_type == ADC_USER_HDMI_ARC)
    {
        //HDMI ARC应用模式下需要建立100ms级别的缓冲
        adc_chan_buffer_set.adc_chan_cache = ARC_PCM_INPUT_BUF_ADDR;
        adc_chan_buffer_set.adc_chan_cache_len = ARC_PCM_INPUT_BUF_LEN;
    }
    else
    {
        adc_chan_buffer_set.adc_chan_cache = PCM_INPUT_BUF_ADDR;
        adc_chan_buffer_set.adc_chan_cache_len = PCM_INPUT_BUF_LEN;
    }

    if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_MIC)
    {
        if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        {
            adc_chan_buffer_set.mic_chan_cache = PCM_INPUT_BUF_ADDR;
            adc_chan_buffer_set.mic_chan_cache_len = PCM_INPUT_BUF_LEN;
        }
#if (SUPPORT_OK_MIC == 1)
        else
        {
            adc_chan_buffer_set.mic_chan_cache = USOUND_MIC_CACHE;
            adc_chan_buffer_set.mic_chan_cache_len = USOUND_MIC_CACHE_LEN;
        }
#endif

    }
#if (SUPPORT_OK_MIC == 1)
    else if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
    {
        adc_chan_buffer_set.adc_chan_cache = PCM_INPUT_BUF_ADDR;
        adc_chan_buffer_set.adc_chan_cache_len = PCM_INPUT_BUF_LEN/2;
        adc_chan_buffer_set.mic_chan_cache = PCM_INPUT_BUF_ADDR + PCM_INPUT_BUF_LEN/2;
        adc_chan_buffer_set.mic_chan_cache_len = PCM_INPUT_BUF_LEN/2;
        adc_chan_buffer_set.mic_aux_temp_buffer_256 = ok_mic_aux_temp_buffer;
    }
#endif

    ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SET_BUFFER, &adc_chan_buffer_set);
    if (ret == -1)
    {
        goto decoder_load_adc_chan_fail_1;
    }

    adc_chan_resample_buffer_set.resample_left_ch_buff = RESAMPLE_LEFT_CHAN_BUFF;
    adc_chan_resample_buffer_set.resample_right_ch_buff = RESAMPLE_RIGHT_CHAN_BUFF;
    adc_chan_resample_buffer_set.resample_split_buff = RESAMPLE_LR_SPLIT_BUFF;
    adc_chan_resample_buffer_set.resample_left_ch_len = sizeof(RESAMPLE_LEFT_CHAN_BUFF);
    adc_chan_resample_buffer_set.resample_right_ch_len = sizeof(RESAMPLE_RIGHT_CHAN_BUFF);
    adc_chan_resample_buffer_set.ret_feedback_data_cbk = &(music_player->feedback_data_cbk);

    ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SET_RESAMPLE_BUFFER, &adc_chan_resample_buffer_set);
    if (ret == -1)
    {
        goto decoder_load_adc_chan_fail_1;
    }

    if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_MIC)
    {
        if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
        {
            ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_MIC_PIPE, &(music_player->p_input_buf_r));
            if (ret == -1)
            {
                goto decoder_load_adc_chan_fail_1;
            }
        }
#if (SUPPORT_OK_MIC == 1)
        else if (music_player->work_mode == PP_WORK_MODE_MIC)
        {
            ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_MIC_PIPE, &(music_player->p_micin_buf_r));
            if (ret == -1)
            {
                goto decoder_load_adc_chan_fail_1;
            }
        }
#endif
    }
    else
    {
        ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_PIPE, &(music_player->p_input_buf_r));
        if (ret == -1)
        {
            goto decoder_load_adc_chan_fail_1;
        }
    }

    if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
    {
        ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_MIC_PIPE, &(music_player->p_micin_buf_r));
        if (ret == -1)
        {
            goto decoder_load_adc_chan_fail_1;
        }
    }

    ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_RESAMPLE_LCH_PIPE, &(music_player->p_resample_lch_r));
    if (ret == -1)
    {
        goto decoder_load_adc_chan_fail_1;
    }

    ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_RESAMPLE_RCH_PIPE, &(music_player->p_resample_rch_r));
    if (ret == -1)
    {
        goto decoder_load_adc_chan_fail_1;
    }

    ret = adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_START, NULL);
    if (ret == -1)
    {
        goto decoder_load_adc_chan_fail_1;
    }

    if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
    {
        uint32 wait_samplerate_timer = sys_get_ab_timer();
        get_sample_rate_ret_t get_sample_rate_ret;

        music_player->need_update_samplerate = TRUE;
        while (1)
        {
            if ((sys_get_ab_timer() - wait_samplerate_timer) > 1000)
            {
                libc_printf("mmm get samplerate overtime!\n");
                break;
            }

            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_SAMPLERATE, &get_sample_rate_ret);
            if (get_sample_rate_ret.sample_rate_ok == TRUE)
            {
                music_player->music_setting.default_samplerate = get_sample_rate_ret.sample_rate_khz;
                music_player->need_update_samplerate = FALSE;

                music_player->need_resample = get_sample_rate_ret.need_resample;
                music_player->resample_src_khz = get_sample_rate_ret.resample_src_khz;
                music_player->resample_dest_khz = get_sample_rate_ret.sample_rate_khz;
                libc_printf("need_resample:%d,%d->%d\n", music_player->need_resample,
                        music_player->resample_src_khz, music_player->resample_dest_khz);

                if ((music_player->music_setting.adc_chan_set.user_type == ADC_USER_HDMI_ARC)
                    && (music_player->music_setting.start_delay_time != 0))
                {
                    music_player->adjust_block_thres = music_player->music_setting.start_delay_time
                        * music_player->music_setting.default_samplerate * 4;
                    if ((music_player->adjust_block_thres % PCM_ADC_BUF_BLOCK) != 0)
                    {
                        music_player->adjust_block_thres = (music_player->adjust_block_thres / PCM_ADC_BUF_BLOCK) * PCM_ADC_BUF_BLOCK;
                        music_player->adjust_block_thres += PCM_ADC_BUF_BLOCK;
                    }
                }

                break;
            }
        }
    }

    return ret;

    decoder_load_adc_chan_fail_1:
    adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_CLOSE, NULL);

    decoder_load_adc_chan_fail_0:

    //卸载ADC CHANNEL
    sys_drv_uninstall(DRV_GROUP_CHANNEL);

    return ret;
}

int32 start_resample_module(music_pcm_player_t *music_player)
{
    int32 ret = 0;
    int src_hz;
    int dest_hz;

    if (music_player->resample_src_khz == 96)
    {
        src_hz = 96000;
    }
    else
    {
        src_hz = 88200;
    }

    if (music_player->resample_dest_khz == 48)
    {
        dest_hz = 48000;
    }
    else
    {
        dest_hz = 44100;
    }

    //打开重采样句柄
    music_player->resample_handle_left = pp_resample_open(src_hz, dest_hz,\
            0, RESAMPLE_TEMP_BUFF[0], RESAMPLE_TABLE_BUFF);
    if (music_player->resample_handle_left == NULL)
    {
        libc_printf_error("pp_resample_open left fail!\n");
        ret = -1;
        return ret;
    }

    music_player->resample_handle_right = pp_resample_open(src_hz, dest_hz,\
            1, RESAMPLE_TEMP_BUFF[1], RESAMPLE_TABLE_BUFF);
    if (music_player->resample_handle_right == NULL)
    {
        libc_printf_error("pp_resample_open right fail!\n");
        ret = -1;
        return ret;
    }

    libc_printf("open resample module : %x,%x\n", music_player->resample_handle_left, music_player->resample_handle_right);

    libc_printf("resample start buffer data\n");
    ret = adc_channel_cmd(&(music_player->adc_channel_handle), ADC_CHANNEL_RESAMPLE_START_BUF_DATA, NULL);

    return ret;
}

/* 启动音乐解码器 */
int32 start_music_decoder(music_pcm_player_t *music_player)
{
    int32 ret = 0;

    //加载数字音效库
    ret = start_music_decoder_load_audiopp(music_player);
    if (ret == -1)
    {
        goto start_music_decoder_fail_ret_0;
    }

    //创建解码线程
    if (creat_decode_thread(music_player) == -1)
    {
        ret = -1;
        goto start_music_decoder_fail_ret_0;
    }

    if (music_player->music_setting.pcm_source == PCM_SOURCE_AUX)
    {
        ret = start_music_decoder_load_adc_chan(music_player);
        if (ret == -1)
        {
            goto start_music_decoder_fail_ret_0;
        }
    }
    else
    {
#if (SUPPORT_OK_MIC == 1)
        if (music_player->work_mode == PP_WORK_MODE_MIC)
        {
            ret = start_music_decoder_load_adc_chan(music_player);
            if (ret == -1)
            {
                goto start_music_decoder_fail_ret_0;
            }
        }
#endif

        music_player->p_input_buf_r = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_USBPCMPIPE);
        if (music_player->p_input_buf_r == NULL)
        {
            while (1);
        }
    }

    if ((music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
        && (music_player->need_resample == 1))
    {
        if (start_resample_module(music_player) < 0)
        {
            goto start_music_decoder_fail_ret_0;
        }
    }

    //状态切换为 READY
    music_player->music_status.status = MMM_PP_ENGINE_READY;
#if (SUPPORT_OK_MIC == 1)
    music_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;
#endif
    music_player->is_player_loop_enable = TRUE;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    //启动播放后自动启动录音
    if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta == MMM_RECORD_STOP))
    {
        ret = start_music_record(music_player);
    }
#endif

    return ret;

    start_music_decoder_fail_ret_0:

    if (music_player->audiopp_handle != NULL)
    {
        post_process_ops(music_player->audiopp_handle, PP_CMD_CLOSE, 0);
        music_player->audiopp_handle = NULL;
        //卸载数字音效库
        sys_drv_uninstall (SOUND_EFFECT_GROUP);
    }

    return ret;
}

static const uint16 freq_valus[] =
{
    0, 4, 8, 14, 26, 30, 36, 42, 48, 54, 60, 66,72, 78, 84, 90, 96, 102,
    108, 114, 120, 126, 132, 138, 144, 150, 156, 162 ,168,
    174, 180, 186, 192,198, 204, 210, 216, 222,
};
static uint8 _get_freq_index(uint32 freq_value)
{
    uint8 index;

    for (index = 0; index < sizeof(freq_valus)/sizeof(uint16); index++)
    {
        if (freq_value <= freq_valus[index])
        {
            break;
        }
    }

    if (index < FREQ_14M)
    {
        index = FREQ_14M;
    }

    return index;
}

void set_music_decoder_clk(music_pcm_player_t *music_player)
{
    uint16 set_freq;

    if (music_player->audiopp_handle == NULL)
    {
        return;
    }

    set_freq = music_player->dae_freq + music_player->decode_freq + music_player->encode_freq;

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_FAST == 1))
    if (music_player->work_mode == PP_WORK_MODE_MIC)
    {
        set_freq += 222;
    }
#endif

    sys_adjust_clk(_get_freq_index(set_freq), SET_DECODE_CLK);
}

//采集到足够数据从才从READY状态切换为FADEIN状态，这时才真正运行起来
int32 start_music_decoder_sub(music_pcm_player_t *music_player)
{
    //断言：至此状态应该为READY
    if (music_player->music_status.status != MMM_PP_ENGINE_READY)
    {
        libc_printf_error("START SUB status NO READY\n");
        while (1);
    }

    //初始化AUDIO DAC，硬件开始工作
    if (music_player->g_aout_init_flag == 0)
    {
        mmm_pp_aout_open(music_player);
        music_player->g_aout_init_flag = 1;
    }

    //设置数字音效参数，并启动淡入
    music_player->fade_flag = 1;
    music_player->dae_update_flag = 1;
    update_PP_para_info(music_player);
    music_player->music_status.status = MMM_PP_ENGINE_FADEIN;

    libc_printf_info("fadein:%d\n", music_player->music_setting.fade_in_time);

    music_player->decode_freq = 4; //解码

    set_music_decoder_clk(music_player);

    //设置缓冲区监控环境
    if (music_player->need_adjust_aps == TRUE)
    {
        set_audio_aps(music_player->audio_aps_info.fast_aps);
        music_player->audio_aps = music_player->audio_aps_info.fast_aps;
        music_player->adjust_step_cnt = 0;
        music_player->adjust_sample_counter = 0;
        music_player->adjust_sample_high_counter = 0;

        music_player->usound_max_block = music_player->p_input_buf_r->length / PCM_DEC_BUF_BLOCK;
        music_player->usound_min_block = music_player->usound_max_block;
        libc_printf_info("start block:%d\n", music_player->usound_max_block);
    }

    return 0;
}
