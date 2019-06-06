/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件向引擎开放的命令API入口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

speech_player_t* speech_player_global;

uint8 __section__(".bss.pre_process_buf") PRE_PROCESS_BUF_ADDR[PRE_PROCESS_BUF_LEN];

uint8 __section__(".bss.ktd_input_buf") KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

uint8 __section__(".bss.mic_adc_buf") MIC_ADC_BUF_ADDR[MIC_ADC_BUF_LEN];
uint8 __section__(".bss.mic_input_buf") MIC_INPUT_BUF_ADDR[MIC_INPUT_BUF_LEN];
uint8 __section__(".bss.peq_temp_buf") PEQ_TEMP_BUFFER_ADDR[PEQ_TEMP_BUFFER_LEN];
uint8 __section__(".bss.predeal_buf") PRE_AGC_DEAL_BUF_ADDR[PRE_AGC_DEAL_BUF_LEN];
uint8 __section__(".bss.temp_buf") SPEECH_TEMP_BUF_ADDR[SPEECH_TEMP_BUF_LEN];
uint8 __section__(".bss.sp_post_temp_buf") SP_POST_BUFFER_ADDR[SP_POST_BUFFER_LEN];
uint8 __section__(".bss.sp_pre_temp_buf") SP_PRE_BUFFER_ADDR[SP_PRE_BUFFER_LEN];

uint8 __section__(".bss.mic_temp_buf") OK_MIC_TEMP_BUF[0x100];

const uint32 __section__(".rodata.entry") mmm_sp_cmd[] =
{
    (uint32) audio_mmm_sp_cmd,
};

/*! \cond MMM_SP_API */

int32 audio_mmm_sp_cmd(void *handel, mmm_sp_cmd_t cmd, uint32 args)
{
    speech_player_t *speech_player = NULL;
    int32 ret = 0;

    if (cmd != MMM_SP_OPEN)
    {
        speech_player = (speech_player_t *) handel;
    }

    if ((speech_player_global->speech_status.status == MMM_SP_ENGINE_NULL) && (cmd != MMM_SP_OPEN))
    {
        ret = -1;
        return ret;
    }

    sys_os_sched_lock();

    switch (cmd)
    {
        case MMM_SP_GET_STATUS:
        libc_memcpy((void *) args, &speech_player->speech_status, sizeof(mmm_sp_status_t));
        break;

        case MMM_SP_OPEN:
        speech_player = mmm_sp_open(args);
        if (speech_player == NULL)
        {
            *((speech_player_t **) handel) = NULL;
            ret = -1;
            break;
        }
        *((speech_player_t **) handel) = speech_player;
        break;

        case MMM_SP_CLOSE:
        //非法状态
        if (speech_player->speech_status.status != MMM_SP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        ret = mmm_sp_close(handel, NULL);
        break;

        case MMM_SP_SET_SPEECH_SETTING_EXT:
        {
            speech_setting_ext_t * p_speech_sett_ext;

            //非法状态
            if (speech_player->speech_status.status != MMM_SP_ENGINE_STOPPED)
            {
                ret = -1;
                break;
            }

            p_speech_sett_ext = (speech_setting_ext_t *) args;

            libc_memcpy(&(speech_player->speech_setting), &(p_speech_sett_ext->speech_setting), sizeof(speech_setting_t));
            if (speech_player->speech_setting.default_channels == SPEECH_CHAN_RIGHT_LEFT)
            {
                if (speech_player->speech_setting.right_weak != 0)
                {
                     speech_player->weak_result = libc_math_exp_fixed(0 - (int16)(speech_player->speech_setting.right_weak*10));
                }
                else if (speech_player->speech_setting.left_weak != 0)
                {
                     speech_player->weak_result = libc_math_exp_fixed(0 - (int16)(speech_player->speech_setting.left_weak*10));
                }
            }
            speech_player->playing_setting_speech_ok = TRUE;


            libc_memcpy(speech_player->g_p_speech_plc_para, &(p_speech_sett_ext->plc_para), sizeof(plc_para_t));
            speech_player->playing_setting_plc_ok = TRUE;

            libc_memcpy(speech_player->g_p_speech_pre_agc_para, &(p_speech_sett_ext->pre_agc_para), sizeof(agc_para_t));
            speech_player->playing_setting_preagc_ok = TRUE;

            libc_memcpy(speech_player->g_p_speech_post_agc_para, &(p_speech_sett_ext->post_agc_para), sizeof(agc_para_t));
            speech_player->playing_setting_postagc_ok = TRUE;

            libc_memcpy(speech_player->g_p_speech_aec_prms, &(p_speech_sett_ext->aec_prms), sizeof(actionsaec_prms_t));
            speech_player->playing_setting_aec_ok = TRUE;

            if (speech_player->speech_setting.sco_codec_type == 0)
            {
                speech_player->sample_rate = 8;
                speech_player->aec_deal_block = 256; //post agc deal block
                speech_player->codec_deal_block = 60;
                speech_player->plc_deal_block = 120;
                if (speech_player->g_p_speech_pre_agc_para->agc_enable == 0)
                {
                    speech_player->pre_agc_deal_block = 128;
                }
                else
                {
                    speech_player->pre_agc_deal_block = 256;
                }
            }
            else
            {
                speech_player->sample_rate = 16;
                speech_player->aec_deal_block = 512; //post agc deal block
                speech_player->codec_deal_block = 60;
                speech_player->plc_deal_block = 240;
                speech_player->pre_agc_deal_block = 256;
                speech_player->pre_high_thres = PRE_PROCESS_BUF_HIGH_THRES_DEFAULT*32;
                speech_player->pre_mean_thres = PRE_PROCESS_BUF_MEAN_THRES_DEFAULT*32;
            }
            speech_player->pcmbuf_fill_block = speech_player->sample_rate * 2 * 2; //2ms
            speech_player->pre_high_thres = PRE_PROCESS_BUF_HIGH_THRES_DEFAULT*speech_player->sample_rate*2;
            speech_player->pre_mean_thres = PRE_PROCESS_BUF_MEAN_THRES_DEFAULT*speech_player->sample_rate*2;
        }
        break;

        case MMM_SP_SET_ASQT_DUMP_BUF:
        //非法状态
        if (speech_player->speech_status.status != MMM_SP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        speech_player->p_asqt_dump_buf = (asqt_dump_buf_t *) args;
        speech_player->pre_high_thres = 64*speech_player->sample_rate*2;
        speech_player->pre_mean_thres = 64*speech_player->sample_rate*2;

        //ASQT 离线调试模式下请求SCO IN
        if ((speech_player->p_asqt_dump_buf != NULL) && (speech_player->p_asqt_dump_buf->sco_in_flag == TRUE))
        {
            speech_player->sco_input_buf_asqt = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_SCO_INPIPE_ASQT);
            if (speech_player->sco_input_buf_asqt == NULL)
            {
                while (1);
            }
            libc_printf("malloc sco in asqt:%d\n", speech_player->plc_deal_block*2);
            speech_player->soc_input_buf_asqt_tempbuf = (uint8 *) sys_malloc(speech_player->plc_deal_block*2);
        }

        break;

        case MMM_SP_PLAY:
        //非法状态
        if (speech_player->speech_status.status != MMM_SP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        if ((speech_player->playing_setting_speech_ok == FALSE)
            || (speech_player->playing_setting_plc_ok == FALSE)
            || (speech_player->playing_setting_aec_ok == FALSE)
            || (speech_player->playing_setting_preagc_ok == FALSE)
            || (speech_player->playing_setting_postagc_ok == FALSE))
        {
            libc_printf_warning("SPEECH PLAYER NO setting!\n");
            ret = -1;
            break;
        }

        ret = start_speech_decoder(speech_player);
        break;

        case MMM_SP_STOP:
        //非法状态
        if (speech_player->speech_status.status == MMM_SP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        //如果当前处于等待状态和淡入状态，必须等到播放起来才允许STOP
        while ((speech_player->speech_status.status == MMM_SP_ENGINE_READY)
            || (speech_player->g_speech_fadein_flag == 1))
        {
#ifdef MUSIC_MMM_DEBUG_INFO
            libc_printf("STOP CMD wait fade over!\n");
#endif
            sys_os_sched_unlock();
            sys_os_time_dly(1);
            sys_os_sched_lock();
        }

        //强制取消未执行的Reset
        speech_player->g_speech_reset_buffer_flag = FALSE;
        ret = stop_speech_decoder(speech_player);
        break;

        case MMM_SP_SET_EQ:
        speech_player->dae_update_flag = 1;
        update_PP_para_info(speech_player);
        set_speech_decoder_clk(speech_player);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
        case MMM_SP_RECORDING_SET_FS:
        libc_memcpy(&(speech_player->record_fs), (record_module_fs_para_t *) args, sizeof(record_module_fs_para_t));
        break;

        case MMM_SP_RECORDING_SET_ENCODE:
        libc_memcpy(&(speech_player->encode_para), (record_module_encode_para_t *) args, sizeof(record_module_encode_para_t));
        break;

        case MMM_SP_RECORDING_START:
        if ((speech_player->speech_status.status == MMM_SP_ENGINE_READY)
            || (speech_player->speech_status.status == MMM_SP_ENGINE_PLAYING))
        {
            speech_player->record_enable = TRUE;
            speech_player->encode_freq = RECORD_FREQ_MIPS;
            set_speech_decoder_clk(speech_player);
            ret = start_speech_record(speech_player);
        }
        else if (speech_player->speech_status.status == MMM_SP_ENGINE_STOPPED)
        {
            speech_player->record_enable = TRUE;
            speech_player->encode_freq = RECORD_FREQ_MIPS;
        }
        else
        {
            libc_printf("BP Status Error!\n");
            ret = -1;
        }
        break;

        case MMM_SP_RECORDING_STOP:
        if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta != MMM_RECORD_STOP))
        {
            ret = stop_speech_record(speech_player);
            speech_player->record_enable = FALSE;
            speech_player->encode_freq = 0;
            set_speech_decoder_clk(speech_player);
        }
        break;

        case MMM_SP_RECORDING_GET_STATUS:
        {
            libc_memcpy((void *) args, &(speech_player->record_status), sizeof(record_module_status_t));
        }
        break;
#endif

#if (SUPPORT_SOFT_KEYTONE == 1)
        case MMM_SP_KEYTONE_PLAY:
        //非法状态
        if ((speech_player->speech_status.status != MMM_SP_ENGINE_READY)
            && (speech_player->speech_status.status != MMM_SP_ENGINE_PLAYING))
        {
            ret = -1;
            break;
        }
        //如果当前还有软按键音在播放，就忽略本次软按键音播放请求
        if (speech_player->speech_status.ktp_status == MMM_KTP_ENGINE_PLAYING)
        {
            libc_printf_warning("keytone is playing, ignore!\n");
            ret = -1;
            break;
        }

        ret = keytone_start(speech_player, args);
        break;
#endif

        case MMM_SP_SET_HFP_CONNECTED:
        if (speech_player->speech_status.status != MMM_SP_ENGINE_PLAYING)
        {
            ret = -1;
            break;
        }

        if (speech_player->set_hfp_connected_flag == 0)
        {
            //全双工AEC
            if ((speech_player->g_p_speech_aec_prms->aec_enable == TRUE)
                && (speech_player->g_p_speech_aec_prms->half_mode == 0))
            {
                speech_player->g_speech_reset_buffer_flag = TRUE;
            }
            speech_player->set_hfp_connected_flag = 1;
        }
        break;

        default:
        libc_printf_warning("MMM_SP cmd not support:%d\n", cmd);
        ret = -1;
        break;
    }

    sys_os_sched_unlock();

    return ret;
}


int32 audio_rcode_init(void)
{
    speech_player_global = sys_malloc(sizeof(speech_player_t));
    if (speech_player_global == NULL)
    {
        libc_printf("mmm_sp_player\n");
        return -1;
    }
    else
    {
        libc_memset(speech_player_global, 0x00, sizeof(speech_player_t));
        speech_player_global->speech_status.status = MMM_SP_ENGINE_NULL;
        return 0;
    }
}

int32 audio_rcode_exit(void)
{
    sys_free(speech_player_global);
    speech_player_global = NULL;

    return 0;
}

module_init (audio_rcode_init)
module_exit(audio_rcode_exit)

/*! \endcond */

