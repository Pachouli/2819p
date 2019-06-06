/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件向引擎提供的命令API入口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"

music_bt_player_t* music_player_global;

freq_point_config_t g_freq_point_cfg;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
music_bt_player_tws_info_t *p_tws_info;
#endif

uint8 __section__(".bss.dec_temp_buffer") SBC_DEC_TEMP_BUFFER_ADDR[SBC_DEC_TEMP_BUFFER_LEN];
uint8 __section__(".bss.dec_output_buffer") SBC_DEC_OUTPUT_BUFFER_ADDR[SBC_DEC_OUTPUT_BLOCK_SIZE];
uint8 __section__(".bss.enc_output_buffer") SBC_ENC_OUTPUT_BUFFER_ADDR[SBC_ENC_OUTPUT_BLOCK_SIZE];
uint8 __section__(".bss.ktd_input_buffer") KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

const uint32 __section__(".rodata.entry") mmm_bp_cmd[] =
{
    (uint32) audio_mmm_bp_cmd,
};

/*! \cond MMM_BP_API */

int32 audio_mmm_bp_cmd(void *handel, mmm_bp_cmd_t cmd, uint32 args)
{
    music_bt_player_t *music_player = NULL;
    int32 ret = 0;

    if (cmd != MMM_BP_OPEN)
    {
        music_player = (music_bt_player_t *) handel;
    }

    if ((music_player_global->music_status.status == MMM_BP_ENGINE_NULL) && (cmd != MMM_BP_OPEN))
    {
        ret = -1;
        return ret;
    }

    sys_os_sched_lock();

    switch (cmd)
    {
        case MMM_BP_GET_STATUS:
        libc_memcpy((void *) args, &music_player->music_status, sizeof(mmm_bp_status_t));
        break;

        case MMM_BP_OPEN:
        music_player = mmm_bp_open((void*)args);
        if (music_player == NULL)
        {
            *((music_bt_player_t **) handel) = NULL;
            ret = -1;
            break;
        }
        *((music_bt_player_t **) handel) = music_player;
        break;

        case MMM_BP_CLOSE:
        //非法状态
        if (music_player->music_status.status != MMM_BP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        ret = mmm_bp_close(handel, NULL);
        break;

        case MMM_BP_SET_MUSIC_SETTING:
        //非法状态
        if (music_player->music_status.status != MMM_BP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        libc_memcpy(&(music_player->music_setting), (music_bt_setting_t *) args, sizeof(music_bt_setting_t));

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
        //SPDIF播歌锁频为222M，采样率可支持到最高96KHz
        if (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN)
        {
            if (music_player->music_setting.adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
            {
                sys_adjust_clk(FREQ_222M, SET_LOCK_CLK);
            }
        }
#endif

        music_player->playing_setting_ok = TRUE;
        break;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
        case MMM_BP_SET_TWS_MODE:
        {
            music_bt_tws_setting_t *p_tws_sett;
            //非法状态
            if (music_player->music_status.status != MMM_BP_ENGINE_STOPPED)
            {
                ret = -1;
                break;
            }

            p_tws_sett = (music_bt_tws_setting_t*)(args);
            p_tws_info->dev_role = p_tws_sett->dev_role;
            p_tws_info->dev_spk = p_tws_sett->dev_spk;
            p_tws_info->tws_get_bt_clk = p_tws_sett->tws_get_bt_clk;
            p_tws_info->tws_set_bt_clk = p_tws_sett->tws_set_bt_clk;
            p_tws_info->tws_reg_bt_clk_sync_cbk = p_tws_sett->tws_reg_bt_clk_sync_cbk;
            p_tws_info->tws_has_avdtp_pkg_lost = p_tws_sett->tws_has_avdtp_pkg_lost;
            p_tws_info->tws_link_connect_flag = TRUE;
        }
        break;

        case MMM_BP_SET_SEND_FRAME_CBK:
        {
            //非法状态
            if (music_player->music_status.status != MMM_BP_ENGINE_STOPPED)
            {
                ret = -1;
                break;
            }

            music_player->p_f_send_frame = (f_send_frame)args;
        }
        break;
#endif

        case MMM_BP_PLAY:
        //非法状态
        if (music_player->music_status.status != MMM_BP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        //如果未经过设置不允许PLAY
        if (music_player->playing_setting_ok == FALSE)
        {
            libc_printf_warning("BT PLAYER NO setting!\n");
            ret = -1;
            break;
        }

        ret = start_music_decoder(music_player);
        break;

        case MMM_BP_STOP:
        //非法状态
        if (music_player->music_status.status == MMM_BP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        //如果当前处于淡入/淡出状态，必须先等淡入/淡出结束
        while ((music_player->music_status.status == MMM_BP_ENGINE_FADEIN)
            || (music_player->music_status.status == MMM_BP_ENGINE_FADEOUT)
            || (music_player->music_status.status == MMM_BP_ENGINE_FADEOUT_PAUSE))
        {
#ifdef BT_MMM_DEBUG_INFO
            libc_printf("STOP CMD wait fade over!\n");
#endif
            sys_os_sched_unlock();
            sys_os_time_dly(1);
            sys_os_sched_lock();
        }

        ret = stop_music_decoder(music_player);
        break;

        case MMM_BP_SET_EQ:
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);
        set_music_decoder_clk(music_player);
        break;

        case MMM_BP_SET_FREQ_POINT:
        libc_memcpy(&g_freq_point_cfg, (freq_point_config_t *) args, sizeof(freq_point_config_t));
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);
        set_music_decoder_clk(music_player);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1))
        case MMM_BP_RECORDING_SET_FS:
        libc_memcpy(&(music_player->record_fs), (record_module_fs_para_t *) args, sizeof(record_module_fs_para_t));
        break;

        case MMM_BP_RECORDING_SET_ENCODE:
        libc_memcpy(&(music_player->encode_para), (record_module_encode_para_t *) args, sizeof(record_module_encode_para_t));
        break;

        case MMM_BP_RECORDING_START:
        if ((music_player->music_status.status == MMM_BP_ENGINE_FADEIN)
            || (music_player->music_status.status == MMM_BP_ENGINE_PLAYING))
        {
            music_player->record_enable = TRUE;
            music_player->encode_freq = RECORD_FREQ_MIPS;
            set_music_decoder_clk(music_player);
            ret = start_music_record(music_player);
        }
        else if (music_player->music_status.status == MMM_BP_ENGINE_STOPPED)
        {
            music_player->record_enable = TRUE;
            music_player->encode_freq = RECORD_FREQ_MIPS;
        }
        else
        {
            libc_printf("BP Status Error!\n");
            ret = -1;
        }
        break;

        case MMM_BP_RECORDING_STOP:
        if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta != MMM_RECORD_STOP))
        {
            ret = stop_music_record(music_player);
            music_player->record_enable = FALSE;
            music_player->encode_freq = 0;
            set_music_decoder_clk(music_player);
        }
        break;

        case MMM_BP_RECORDING_GET_STATUS:
        {
            libc_memcpy((void *) args, &(music_player->record_status), sizeof(record_module_status_t));
        }
        break;
#endif

#if (SUPPORT_SOFT_KEYTONE == 1)
        case MMM_BP_KEYTONE_PLAY:
        //非法状态
        if ((music_player->music_status.status != MMM_BP_ENGINE_FADEIN)
            && (music_player->music_status.status != MMM_BP_ENGINE_PLAYING))
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
        libc_printf_warning("MMM_BP cmd not support:%d\n", cmd);
        ret = -1;
        break;
    }

    sys_os_sched_unlock();

    return ret;
}

int32 audio_rcode_init(void)
{
    music_player_global = sys_malloc(sizeof(music_bt_player_t));
    if (music_player_global == NULL)
    {
        libc_printf("mmm_bt_player\n");
        return -1;
    }
    else
    {
        libc_memset(music_player_global, 0, sizeof(music_bt_player_t));
        music_player_global->music_status.status = MMM_BP_ENGINE_NULL;
        return 0;
    }
}

int32 audio_rcode_exit(void)
{
    sys_free(music_player_global);
    music_player_global = NULL;

    return 0;
}

module_init(audio_rcode_init)
module_exit(audio_rcode_exit)

/*! \endcond */

