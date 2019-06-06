/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  解码线程创建、解码器启动功能实现
 ********************************************************************************/

#include "music_player_global.h"

uint8 __section__(".stack.ap_high_stack_ptos")ap_high_stk_ptos[1];//动态数组
extern uint16 _AP_HIGH_STK_SIZE;

uint8 __section__(".stack.ok_mic_stack_ptos")ok_mic_stk_ptos[1];//动态数组
extern uint16 _OK_MIC_STK_SIZE;

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

static const char adc_channel_name[DRV_NAME_SIZE] = "adc_chan.al";

uint8 __section__(".bss.decode_output_cache") decode_output_cache[0x2000];

uint8 __section__(".bss.pcm_adc_buffer") PCM_ADC_BUF_ADDR[PCM_ADC_BUF_LEN];

uint8 __section__(".bss.input_buffer") PCM_INPUT_BUF_ADDR[0x400];

uint8 __section__(".bss.OK_MIC_TEMP_BUFF") OK_MIC_TEMP_BUFF[0x100];

uint8 __section__(".bss.OK_MIC_DECODE_BUFF") OK_MIC_DECODE_BUFF[0x200];

int32 creat_decode_thread(music_player_t *music_player)
{
    pthread_param_t player_thread_para;/* music playrer线程上下文 */

    player_thread_para.start_rtn = (void *)player_loop;
    player_thread_para.arg = (void *) music_player;
    player_thread_para.ptos = (OS_STK *) ap_high_stk_ptos;
    player_thread_para.stk_size = (uint32)&_AP_HIGH_STK_SIZE;

    /* 创建中间件线程并设置属性 */
    if (libc_pthread_create(&player_thread_para, AP_BACK_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        return -1;
    }

    return 0;
}

#if (SUPPORT_OK_MIC == 1)
int32 creat_ok_mic_thread(music_player_t *music_player)
{
    pthread_param_t player_thread_para;/* music playrer线程上下文 */

    player_thread_para.start_rtn = (void *)ok_mic_loop;
    player_thread_para.arg = (void *) music_player;
    player_thread_para.ptos = (ok_mic_stk_ptos + (uint32)(&_OK_MIC_STK_SIZE));
    player_thread_para.stk_size = (uint32)&_OK_MIC_STK_SIZE;

    /* 创建中间件线程并设置属性 */
    if (libc_pthread_create(&player_thread_para, AP_BACK_HIGH_OK_MIC_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        return -1;
    }

    return 0;
}
#endif

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

    if (index < FREQ_26M)
    {
        index = FREQ_26M;
    }

    return index;
}

void set_music_decoder_clk(music_player_t *music_player)
{
    uint16 set_freq;

    if (music_player->audiopp_handle == NULL)
    {
        return;
    }

    set_freq = music_player->dae_freq + music_player->decode_freq;
    set_freq += 6; //补偿 Cache Miss

    if (music_player->fs_para.fs_type == FS_TYPE_VFS)
    {
        //插卡播歌或插U播歌，1536kbps或以上码流，预留最高30%读卡时间，防止有些卡读得很慢卡音，比如400KB/S
        if (music_player->music_info.music_type == WAV_TYPE)
        {
            set_freq += set_freq * 2 / 5;
        }
        //插卡播歌或插U播歌，1000kbps或以上码流，预留最高30%读卡时间，防止有些卡读得很慢卡音，比如400KB/S
        else if ((music_player->music_info.music_type == APE_TYPE)
            || (music_player->music_info.music_type == FLAC_TYPE))
        {
            set_freq += set_freq * 2 / 5;
        }
        //最高400kbps，预留12.5%读卡时间，防止有些卡读得很慢卡音，比如400KB/S
        else
        {
            set_freq += set_freq / 7;
        }
    }

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (music_player->resample_info.enable_resample == TRUE)
    {
        set_freq += 40;
    }
#endif

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_FAST == 1))
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        set_freq += 222;
    }
#endif

    sys_adjust_clk(_get_freq_index(set_freq), SET_DECODE_CLK);
}

/* 启动音乐解码器 */
int32 start_music_decoder(music_player_t *music_player)
{
    int32 ret = 0;

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    //设置重采样属性，会改变采样率，给 get_sample_rate_index 使用，所以必须放在 get_sample_rate_index 之前
    set_resample_info(&music_player->resample_info, music_player->music_info.sample_rate);
#endif

    if (get_sample_rate_index(music_player) == FALSE)
    {
        libc_printf_warning("unexpect sample rate! %d\n", music_player->music_info.sample_rate);
        music_player->music_status.err_no = MMM_MP_ERR_FILE_NOT_SUPPORT;
        goto start_music_decoder_exit_0;
    }

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        //打开MIC通道，设置参数，并启动ADC通道
        if ((int32) sys_drv_install(DRV_GROUP_CHANNEL, 0, adc_channel_name) == -1)
        {
            while (1);
        }
        else
        {
            adc_channel_buffer_setting_t adc_chan_buffer_set;

            adc_channel_cmd(&(music_player->adc_channel_handle), ADC_CHANNEL_OPEN, NULL);
            music_player->music_setting.adc_chan_set.default_samplerate = music_player->music_info.sample_rate;
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SETTING, &(music_player->music_setting.adc_chan_set));

            adc_chan_buffer_set.adc_chan_buffer_1k = PCM_ADC_BUF_ADDR;
            adc_chan_buffer_set.pcm_adc_block_size = PCM_ADC_BUF_BLOCK/2; //DMA Block短一点，延时可以短一点
            adc_chan_buffer_set.adc_chan_cache = 0;
            adc_chan_buffer_set.adc_chan_cache_len = 0;
            adc_chan_buffer_set.mic_chan_cache = PCM_INPUT_BUF_ADDR;
            adc_chan_buffer_set.mic_chan_cache_len = sizeof(PCM_INPUT_BUF_ADDR);
            adc_chan_buffer_set.mic_aux_temp_buffer_256 = 0;
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SET_BUFFER, &adc_chan_buffer_set);

            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_MIC_PIPE, &(music_player->p_mic_input_buf_r));
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_START, NULL);
        }
    }
#endif

    /* 开始播放才真正打开音频解码库 */
    ret = (int32) sys_drv_install(CODEC_GROUP, 0, (uint8 *) DRV_NAME_ARRAY[music_player->music_info.music_type - 1]);
    if (ret == -1)
    {
        music_player->music_status.err_no = MMM_MP_ERR_DECODER_LOAD;
        goto start_music_decoder_exit_0;
    }
    music_player->audiodec_type = music_player->music_info.music_type; //更新已加载的解码库信息

#if (SUPPORT_LOOP_PLAY == 1)
    if(music_player->music_loopplay_info.is_loop_play == TRUE)
    {
        music_player->music_loopplay_info.musicfile_actual_len = music_player->music_info.file_len;
        music_player->music_info.file_len = 0X7FFFFFFF;
        music_player->music_info.total_time = music_player->music_info.file_len/music_player->music_info.avg_bitrate*8;
    }
#endif

    ret = audio_decoder_cmd(&music_player->audiodec_handle, AD_CMD_OPEN, (uint32) & music_player->music_info);
    if ((ret < 0) || (music_player->audiodec_handle == NULL))
    {
        libc_printf_error("open dec_xxx.al fail!\n");
        music_player->music_status.err_no = MMM_MP_ERR_DECODER_ERROR;
        goto start_music_decoder_exit_1;
    }

    //加载数字音效库
    ret = (int32) sys_drv_install(SOUND_EFFECT_GROUP, 0, (void*)aduiopp_name);
    if (ret == -1)
    {
        music_player->music_status.err_no = MMM_MP_ERR_DECODER_LOAD;
        goto start_music_decoder_exit_2;
    }
    //打开数字音效库
    ret = post_process_ops(&(music_player->audiopp_handle), PP_CMD_OPEN, 0);
    if ((ret == -1) || (music_player->audiopp_handle == NULL))
    {
        libc_printf_error("audiopp open fail!\n");
        goto start_music_decoder_exit_3;
    }

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    //打开重采样句柄
    if (music_player->resample_info.enable_resample == TRUE)
    {
        music_player->resample_info.resample_handle_left = pp_resample_open(music_player->resample_info.convert_input_rate_hz, \
            music_player->resample_info.convert_output_rate_hz, 0, RESAMPLE_TEMP_BUFF[0], RESAMPLE_TABLE_BUFF);
        if (music_player->resample_info.resample_handle_left == NULL)
        {
            libc_printf_error("pp_resample_open left fail!\n");
            goto start_music_decoder_exit_4;
        }

        music_player->resample_info.resample_handle_right = pp_resample_open(music_player->resample_info.convert_input_rate_hz, \
            music_player->resample_info.convert_output_rate_hz, 1, RESAMPLE_TEMP_BUFF[1], RESAMPLE_TABLE_BUFF);
        if (music_player->resample_info.resample_handle_right == NULL)
        {
            libc_printf_error("pp_resample_open right fail!\n");
            goto start_music_decoder_exit_4;
        }
    }
#endif

    //音乐播放断点续播定位
    if (music_player->music_decode_info.music_bp_info.bp_time_offset < 0)
    {
        if ((music_player->music_decode_info.music_bp_info.bp_time_offset + music_player->music_info.total_time) < 0)
        {
            music_player->music_decode_info.music_bp_info.bp_time_offset = -music_player->music_info.total_time / 2;
        }
    }
    ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_SEEK, (uint32)
            & music_player->music_decode_info);
    if (ret < 0)
    {
        music_player->music_status.err_no = MMM_MP_ERR_DECODER_ERROR;
        goto start_music_decoder_exit_5;
    }

    ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_RESET, (uint32) NULL);
    if (ret < 0)
    {
        music_player->music_status.err_no = MMM_MP_ERR_DECODER_ERROR;
        goto start_music_decoder_exit_5;
    }

#if (SUPPORT_OK_MIC == 1)
    //加载OK MIC线程
    if (music_player->work_mode == MP_WORK_MODE_MIC)
    {
        music_player->decode_output_cache_rw.raw_data = decode_output_cache;
        music_player->decode_output_cache_rw.capacity = sizeof(decode_output_cache);

        //加载解码线程
        if (creat_ok_mic_thread(music_player) == -1)
        {
            music_player->music_status.err_no = MMM_MP_ERR_DECODER_LOAD;
            ret = -1;
            goto start_music_decoder_exit_5;
        }
        music_player->is_ok_mic_loop_enable = TRUE;
        music_player->ok_mic_wait_decode_first = TRUE; //待解码第一帧后再开始
    }
#endif

    //加载解码线程
    if (creat_decode_thread(music_player) == -1)
    {
        music_player->music_status.err_no = MMM_MP_ERR_DECODER_LOAD;
        ret = -1;
        goto start_music_decoder_exit_5;
    }
    music_player->is_player_loop_enable = TRUE;

    //设置数字音效参数，并启动淡入
    music_player->fade_flag = 1;
    music_player->dae_update_flag = 1;
    update_PP_para_info(music_player);
    music_player->music_status.status = MMM_MP_ENGINE_FADEIN;
    libc_printf_info("fadein:%d\n", music_player->music_setting.fade_in_time);

    //使能Audio
    mmm_mp_aout_open(music_player, NULL);
    music_player->g_aout_init_flag = TRUE;

    //设置解码频率
    if (music_player->music_info.music_type == MP3_TYPE)
    {
        music_player->decode_freq = 45;
    }
    else if (music_player->music_info.music_type == WMA_TYPE)
    {
        music_player->decode_freq = 75;
    }
    else if (music_player->music_info.music_type == WAV_TYPE)
    {
        music_player->decode_freq = 20;
    }
    else if (music_player->music_info.music_type == APE_TYPE)
    {
        music_player->decode_freq = 75;
    }
    else if (music_player->music_info.music_type == FLAC_TYPE)
    {
        music_player->decode_freq = 60; //FLAC原本解码运算量不高，但是由于每帧解码结果有16KB，而我们的PCM_BUF只有8KB，
                                        //有一半时间浪费在等待PCM_BUF完全缓冲了解码结果，所以必须提速保证在一半时间内解码完成
    }

    set_music_decoder_clk(music_player);

    return ret;

    start_music_decoder_exit_5:

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    //关闭重采样句柄
    if (music_player->resample_info.enable_resample == TRUE)
    {
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

    start_music_decoder_exit_4:
    //CLOSE音效库
    post_process_ops(music_player->audiopp_handle, PP_CMD_CLOSE, 0);
    music_player->audiopp_handle = NULL;

    start_music_decoder_exit_3:
    //卸载数字音效库
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    start_music_decoder_exit_2:
    //CLOSE解码库
    audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CLOSE, (uint32) NULL);
    music_player->audiodec_handle = NULL;

    start_music_decoder_exit_1:
    //卸载解码库
    if (music_player->audiodec_type != 0)
    {
        sys_drv_uninstall (CODEC_GROUP);
        music_player->audiodec_type = 0;
    }

    start_music_decoder_exit_0:

    return ret;
}
