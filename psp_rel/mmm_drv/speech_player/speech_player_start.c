/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件启动通话处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"
#include "sysdef.h"

uint8 __section__(".stack.ap_high_bt_call_stack_ptos")bt_call_stk_ptos[1];//动态数组
extern uint16 _BT_CALL_STK_SIZE;

#define DRV_NAME_SIZE 12
static const char sp_pre_name[DRV_NAME_SIZE] = "sp_pre.al";
static const char sp_post_name[DRV_NAME_SIZE] = "sp_post.al";

static const char dec_plc_name[DRV_NAME_SIZE] = "plc.al";
static const char dec_speech_name[DRV_NAME_SIZE] = "speech.al";//全双工
static const char dec_speech_half_name[DRV_NAME_SIZE] = "speechh.al";//半双工

static const uint8 aduiopp_name[DRV_NAME_SIZE] = "audiopp.al";

int32 creat_decode_thread(speech_player_t *speech_player)
{
    pthread_param_t player_thread_para;

    player_thread_para.start_rtn = (void *)player_loop;
    player_thread_para.arg = (void *) speech_player;
    player_thread_para.ptos = (OS_STK *) bt_call_stk_ptos;
    player_thread_para.stk_size = (uint32)&_BT_CALL_STK_SIZE;

    /* 创建中间件线程并设置属性 */
    if (libc_pthread_create(&player_thread_para, AP_BACK_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        speech_player = NULL;
        return -1;
    }

    return 0;
}

int32 start_speech_decoder_plc_init(speech_player_t *speech_player)
{
    int32 ret = 0;

    ret = hfp_plc_cmd(&(speech_player->plc_handle), HS_CMD_PLC_OPEN, speech_player->plc_deal_block/sizeof(short)/2); //unit is 2 samples
    if ((ret == -1) || (speech_player->plc_handle == NULL))
    {
        libc_printf_warning("plc open fail!!\n");
        ret = -1;
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

    if (index < FREQ_26M)
    {
        index = FREQ_26M;
    }

    return index;
}

void set_speech_decoder_clk(speech_player_t *speech_player)
{
    uint16 set_freq;

    if (speech_player->audiopp_handle == NULL)
    {
        return;
    }

    set_freq = speech_player->dae_freq + speech_player->decode_freq + speech_player->encode_freq;

    if (speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
    {
        if (speech_player->g_p_speech_aec_prms->half_mode == 1) //半双工
        {
            set_freq += 10; //补偿 Cache Miss
        }
        else //全双工
        {
            set_freq += 30; //补偿 Cache Miss
        }
    }

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (speech_player->resample_info.enable_resample == TRUE)
    {
        set_freq += 30;
    }
#endif

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_FAST == 1))
    if (speech_player->work_mode == SP_WORK_MODE_MIC)
    {
        set_freq += 222;
    }
#endif

    sys_adjust_clk(_get_freq_index(set_freq), SET_DECODE_CLK);
}

/* 启动音乐解码器 */
int32 start_speech_decoder(speech_player_t *speech_player)
{
    int32 ret = 0;
    bool need_install_hfp = FALSE;
    uint32 set_freq = 0;
    bool plc_al_half = FALSE;

    //申请预处理资源
    if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
    {
        ret = (int32) sys_drv_install(SP_PRE_GROUP, 0, sp_pre_name);
        if (ret == -1)
        {
            goto start_speech_decoder_fail_ret_0;
        }

        //打开预处理库
        ret = sp_pre_cmd(&(speech_player->sp_pre_handle), SP_PRE_CMD_OPEN, 0);
        if ((ret == -1) || (speech_player->sp_pre_handle == NULL))
        {
            libc_printf_warning("sp_pre open fail!!\n");
            goto start_speech_decoder_fail_ret_3;
        }

        speech_player->sco_data_predeal_buf = (short *) sys_malloc(speech_player->plc_deal_block);
        if (speech_player->sco_data_predeal_buf == NULL)
        {
            libc_printf("sco_data_predeal_buf\n");
            while(1);
        }
    }

    if (speech_player->speech_setting.use_sco_data_postdeal == TRUE)
    {
        ret = (int32) sys_drv_install(SP_POST_GROUP, 0, sp_post_name);
        if (ret == -1)
        {
            goto start_speech_decoder_fail_ret_0;
        }

        //打开预处理库
        ret = sp_post_cmd(&(speech_player->sp_post_handle), SP_POST_CMD_OPEN, 0);
        if ((ret == -1) || (speech_player->sp_post_handle == NULL))
        {
            libc_printf_warning("sp_post open fail!!\n");
            goto start_speech_decoder_fail_ret_3;
        }
    }

    if ((speech_player->g_p_speech_pre_agc_para->agc_enable == 1)
        || (speech_player->g_p_speech_post_agc_para->agc_enable == 1)
        || (speech_player->g_p_speech_aec_prms->aec_enable == 1))
    {
        need_install_hfp = TRUE;
    }

    if (speech_player->g_p_speech_plc_para->plc_enable == 1)
    {
        ret = (int32) sys_drv_install(PLC_GROUP, 0, dec_plc_name); //全双工
        if (ret == -1)
        {
            goto start_speech_decoder_fail_ret_0;
        }

        //打开PLC
        if (speech_player->g_p_speech_plc_para->plc_enable == 1)
        {
            ret = start_speech_decoder_plc_init(speech_player);
            if (ret == -1)
            {
                goto start_speech_decoder_fail_ret_1;
            }
        }
    }

    //加载语音通话算法库
    if (need_install_hfp == TRUE)
    {
        ret = (int32) sys_drv_install(SPEECH_GROUP, 0, dec_speech_name);
        if (ret == -1)
        {
            if ((speech_player->g_p_speech_aec_prms->half_mode == 1) //半双工
                || (speech_player->p_asqt_dump_buf != NULL)) //ASQT允许全双工配置跑半双工算法，这样避免死机
            {
                ret = (int32) sys_drv_install(SPEECH_GROUP, 0, dec_speech_half_name);
            }
        }
        if (ret == -1)
        {
            goto start_speech_decoder_fail_ret_0;
        }

        //打开预处理AGC
        if (speech_player->g_p_speech_pre_agc_para->agc_enable == 1)
        {
            ret = start_speech_decoder_agc_init(speech_player, speech_player->g_p_speech_pre_agc_para);
            if (ret == -1)
            {
                goto start_speech_decoder_fail_ret_1;
            }
        }

        //打开后处理AGC
        if (speech_player->g_p_speech_post_agc_para->agc_enable == 1)
        {
            ret = start_speech_decoder_agc_init(speech_player, speech_player->g_p_speech_post_agc_para);
            if (ret == -1)
            {
                goto start_speech_decoder_fail_ret_1;
            }
        }

        //打开AEC
        if (speech_player->g_p_speech_aec_prms->aec_enable == 1)
        {
            ret = start_speech_decoder_aec_init(speech_player, speech_player->g_p_speech_aec_prms);
            if (ret == -1)
            {
                goto start_speech_decoder_fail_ret_1;
            }
        }

        libc_printf_info("speech open!\n");
    }

    //加载数字音效库
    ret = (int32) sys_drv_install(SOUND_EFFECT_GROUP, 0, (void*)aduiopp_name);
    if (ret == -1)
    {
        goto start_speech_decoder_fail_ret_2;
    }
    //打开数字音效库
    ret = post_process_ops(&(speech_player->audiopp_handle), PP_CMD_OPEN, 0);
    if ((ret == -1) || (speech_player->audiopp_handle == NULL))
    {
        libc_printf_warning("audiopp open fail!!\n");
        goto start_speech_decoder_fail_ret_3;
    }

#if (SUPPORT_AUDIO_RESAMPLE == 1)

    set_resample_info(&speech_player->resample_info, speech_player->sample_rate);

    //打开重采样句柄
    if (speech_player->resample_info.enable_resample == TRUE)
    {
        speech_player->resample_info.resample_handle_left = pp_resample_open(speech_player->resample_info.convert_input_rate_hz, \
            speech_player->resample_info.convert_output_rate_hz, 0, RESAMPLE_TEMP_BUFF[0], RESAMPLE_TABLE_BUFF);
        if (speech_player->resample_info.resample_handle_left == NULL)
        {
            libc_printf_error("pp_resample_open left fail!\n");
            goto start_speech_decoder_fail_ret_4;
        }
    }

#endif

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
    if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta == MMM_RECORD_STOP))
    {
        ret = start_speech_record(speech_player);
    }
#endif

    //AEC失效后恢复流程，先STOP，再START，解码线程没有被销毁
    if (speech_player->g_speech_reset_buffer_flag == FALSE)
    {
        //创建解码线程
        if (creat_decode_thread(speech_player) == -1)
        {
            ret = -1;
            goto start_speech_decoder_fail_ret_4;
        }
    }

    //设置数字音效参数，并启动淡入
    speech_player->fade_flag = 1;
    libc_printf_info("fadein:%d\n", FADE_IN_TIME_DEFAULT);
    speech_player->dae_update_flag = 1;
    update_PP_para_info(speech_player);

    speech_player->decode_freq = 0;
    if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
    {
        speech_player->decode_freq += 4;
    }
    if (speech_player->speech_setting.use_sco_data_postdeal == TRUE)
    {
        speech_player->decode_freq += 24;
    }

    //解码调频
    if (speech_player->p_asqt_dump_buf != NULL)
    {
        speech_player->decode_freq = 200;
    }
    else
    {
        if (speech_player->g_p_speech_plc_para->plc_enable == 1)
        {
            speech_player->decode_freq += 8;
        }

        if (speech_player->g_p_speech_pre_agc_para->agc_enable == 1)
        {
            speech_player->decode_freq += 4;
        }

        if (speech_player->g_p_speech_post_agc_para->agc_enable == 1)
        {
            speech_player->decode_freq += 4;
        }

        if (speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
        {
            if (speech_player->g_p_speech_aec_prms->half_mode == 1) //半双工
            {
                speech_player->decode_freq += 20;
            }
            else //全双工
            {
                speech_player->decode_freq += 60;
            }
        }
    }

    speech_player->decode_freq += 4; //playback

    set_speech_decoder_clk(speech_player);

    //状态切换为 READY
    speech_player->speech_status.status = MMM_SP_ENGINE_READY;
    speech_player->is_player_loop_enable = TRUE;
    speech_player->ready_start_timer = sys_get_ab_timer();

    //初始化AUDIO DAC，硬件开始工作；硬件中断会填充BUFFER，需要确保状态必须是 READY/PLAYING/ERROR
    mmm_sp_aout_open(speech_player);

    //设置缓冲区监控环境
    set_audio_aps (speech_player->audio_aps_info.slow_aps);
    speech_player->audio_aps = speech_player->audio_aps_info.slow_aps;
    speech_player->adjust_step_cnt = 0;

    speech_player->adjust_sample_counter = 0;
    speech_player->adjust_sample_high_counter = 0;

    speech_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;

    return ret;

    start_speech_decoder_fail_ret_4:

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    if (speech_player->resample_info.enable_resample == TRUE)
    {
        //关闭重采样句柄
        if (speech_player->resample_info.resample_handle_left != NULL)
        {
            pp_resample_close(speech_player->resample_info.resample_handle_left);
            speech_player->resample_info.resample_handle_left = NULL;
        }
    }
#endif

    //CLOSE音效库
    post_process_ops(speech_player->audiopp_handle, PP_CMD_CLOSE, 0);
    speech_player->audiopp_handle = NULL;

    start_speech_decoder_fail_ret_3:
    //卸载数字音效库
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    start_speech_decoder_fail_ret_2:
    start_speech_decoder_fail_ret_1:
    //卸载数字音效库
    sys_drv_uninstall (CODEC_GROUP);

    start_speech_decoder_fail_ret_0:

    return ret;
}

