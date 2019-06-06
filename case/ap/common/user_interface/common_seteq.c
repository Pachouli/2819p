/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：数字音效配置接口，图形EQ切换接口，图形EQ暂未支持。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 有效值个数 */
    uint16 total;
    /*! 有效值数组，可变长数组 */
    peq_band_t eq_data;
} peq_config_t;

typedef struct
{
    short threshold;
    short attack_time;
    short release_time;
} limiter_band_config_t;

typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 有效值个数 */
    uint16 total;
    /*! 有效值数组，可变长数组 */
    limiter_band_config_t limiter_data;
} limiter_config_t;

typedef struct
{
    uint16 temp1;
    uint16 total;
    drc_band_t drc_band_para;
} cfg_drc_band_config_t;

typedef struct
{
    uint16 detect_time;
    uint16 attack_time;
    uint16 release_time;
    int16  side_chain_threshold;
    int16  main_chain_target_gain;
} ducking_band_t;

typedef struct
{
    uint16 temp1;
    uint16 total;
    ducking_band_t ducking_para;
} ducking_config_t;

/*! 数字音效配置参数 */
dae_config_t *g_p_com_dae_cfg;
/*! 进入通话场景，备份播歌PEQ，切换为通话场景PEQ，从通话场景退出后需切换回播歌PEQ */
dae_config_t *p_dae_peq_backup;

void com_load_geq_config(dae_config_t *p_dae_cfg, com_eqtype_e eq_type)
{
    peq_config_t tmp_peq_config;
    uint8 i;

    //switch GEQ
    if (eq_type == EQ_DBB)
    {
        for (i = 0; i < EQ_NUM_BANDS; i++)
        {
            com_get_config_struct_inner((uint16)(DAE_GEQ1_BANK0_SETTING + i), (uint8 *) &tmp_peq_config, sizeof(peq_config_t));
            p_dae_cfg->music_eq_id[i] = tmp_peq_config.default_value;
            libc_memcpy(&(p_dae_cfg->peq_bands[i]), &(tmp_peq_config.eq_data), sizeof(peq_band_t));
        }
    }
    else
    {
        for (i = 0; i < EQ_NUM_BANDS; i++)
        {
            com_get_config_struct_inner((uint16)(DAE_PEQ_BANK0_SETTING + i), (uint8 *) &tmp_peq_config, sizeof(peq_config_t));
            p_dae_cfg->music_eq_id[i] = tmp_peq_config.default_value;
            libc_memcpy(&(p_dae_cfg->peq_bands[i]), &(tmp_peq_config.eq_data), sizeof(peq_band_t));
        }
    }
}

//从config.bin加载DAE默认配置项
void com_load_dae_config(dae_config_t *p_dae_cfg)
{
    limiter_config_t tmp_limiter_config;
    cfg_drc_band_config_t temp_drc_band_config;
    ducking_config_t temp_ducking_config;
    uint8 i;

    p_dae_cfg->bypass = (uint8) com_get_config_default_inner(DAE_BYPASS_ENABLE);
    p_dae_cfg->precut = (int16) com_get_config_default_inner(DAE_PRECUT_RATIO);

    p_dae_cfg->peq_enable = (uint8) com_get_config_default_inner(DAE_PEQ_ENABLE);
    p_dae_cfg->peq_version = 1;
    if ((uint8) com_get_config_default_inner(OK_MIC_PEQ_ENABLE) == 1)
    {
        p_dae_cfg->kt_peq_band_num = (uint8) com_get_config_default_inner(OK_MIC_PEQ_BAND_NUM);
    }
    else
    {
        p_dae_cfg->kt_peq_band_num = 0;
    }
    com_load_geq_config(p_dae_cfg, g_com_env_var.eq_type);

    p_dae_cfg->limiter_enable = (uint8) com_get_config_default_inner(DAE_LIMITER_ENABLE);
    com_get_config_struct_inner(DAE_LIMITER_SETTING, (uint8 *) &tmp_limiter_config, sizeof(tmp_limiter_config));
    p_dae_cfg->limiter_threshold = tmp_limiter_config.limiter_data.threshold;
    p_dae_cfg->limiter_attack_time = tmp_limiter_config.limiter_data.attack_time;
    p_dae_cfg->limiter_release_time = tmp_limiter_config.limiter_data.release_time;

    p_dae_cfg->drc_enable = com_get_config_default_inner(DAE_DRC_ENABLE);
    p_dae_cfg->drc_crossover = com_get_config_default_inner(DAE_DRC_CROSSOVER);
    p_dae_cfg->pre_softvol_gain = (int16) com_get_config_default_inner(DAE_DRC_PRE_SOFTVOL_GAIN);
    p_dae_cfg->post_softvol_gain = (int16) com_get_config_default_inner(DAE_DRC_POST_SOFTVOL_GAIN);

    com_get_config_struct_inner(DAE_DRC_BAND1_SETTING, (uint8 *) &temp_drc_band_config, sizeof(cfg_drc_band_config_t));
    libc_memcpy(&(p_dae_cfg->drc_bands[0]), &(temp_drc_band_config.drc_band_para), sizeof(drc_band_t));
    com_get_config_struct_inner(DAE_DRC_BAND2_SETTING, (uint8 *) &temp_drc_band_config, sizeof(cfg_drc_band_config_t));
    libc_memcpy(&(p_dae_cfg->drc_bands[1]), &(temp_drc_band_config.drc_band_para), sizeof(drc_band_t));

    p_dae_cfg->drc_mode = DRC_MODE_NORMAL;

#if (SUPPORT_OK_MIC == 1)

    p_dae_cfg->kt_limiter_enable = (uint8) com_get_config_default_inner(OK_MIC_LIMITER_ENABLE);
    com_get_config_struct_inner(OK_MIC_LIMITER_SETTING, (uint8 *) &tmp_limiter_config, sizeof(tmp_limiter_config));
    p_dae_cfg->kt_limiter_threshold = tmp_limiter_config.limiter_data.threshold;
    p_dae_cfg->kt_limiter_attack_time = tmp_limiter_config.limiter_data.attack_time;
    p_dae_cfg->kt_limiter_release_time = tmp_limiter_config.limiter_data.release_time;

    p_dae_cfg->echo_enable = (uint8) com_get_config_default_inner(OK_MIC_ECHO_ENABLE);
    p_dae_cfg->echo_delay = (uint16) com_get_config_default_inner(OK_MIC_ECHO_DELAY);
    {
        echo_reverb_para_t tmp_echo_para;
        com_get_config_struct_inner(OK_MIC_ECHO_RATIO_ARRAY, (uint8 *) &tmp_echo_para, sizeof(echo_reverb_para_t));
        p_dae_cfg->echo_ratio = tmp_echo_para.default_value;

        p_dae_cfg->echo_ratio_index = tmp_echo_para.total-1;
        for (i = 0; i < tmp_echo_para.total; i++)
        {
            if (p_dae_cfg->echo_ratio == tmp_echo_para.value[i])
            {
                p_dae_cfg->echo_ratio_index = i;
                break;
            }
        }
    }
    p_dae_cfg->echo_lowpass_fc1 = (uint16) com_get_config_default_inner(OK_MIC_ECHO_LP_BAND0);
    p_dae_cfg->echo_lowpass_fc2 = (uint16) com_get_config_default_inner(OK_MIC_ECHO_LP_BAND1);

    p_dae_cfg->freq_shift_enable = (uint8) com_get_config_default_inner(OK_MIC_FREQ_SHIFT_ENABLE);

    p_dae_cfg->ducking_enable = FALSE;
    com_get_config_struct_inner(OK_MIC_DUCKING_PARA, (uint8 *) &temp_ducking_config, sizeof(ducking_config_t));
    p_dae_cfg->ducking_detect_time            = temp_ducking_config.ducking_para.detect_time;
    p_dae_cfg->ducking_attack_time            = temp_ducking_config.ducking_para.attack_time;
    p_dae_cfg->ducking_release_time           = temp_ducking_config.ducking_para.release_time;
    p_dae_cfg->ducking_side_chain_threshold   = (int16) temp_ducking_config.ducking_para.side_chain_threshold;
    p_dae_cfg->ducking_main_chain_target_gain = (int16) temp_ducking_config.ducking_para.main_chain_target_gain;

    p_dae_cfg->voice_removal_enable = FALSE;

    p_dae_cfg->mic_mix_mode_l_chan = (uint8) com_get_config_default_inner(OK_MIC_MIX_MODE_L_CHAN);
    p_dae_cfg->mic_mix_mode_r_chan = (uint8) com_get_config_default_inner(OK_MIC_MIX_MODE_R_CHAN);

    //如果产品有MIC热拔插检测，则默认MIC为拔出状态，可以MUTE掉OK MIC，等MIC插入后一小段时间再Release Mute
    //【默认】如果产品没有MIC热拔插检测，则默认为插入状态，且一直为插入状态
    change_ok_mic_state(TRUE);
    p_dae_cfg->ok_mic_mute = FALSE;
#endif

    p_dae_cfg->aset_mode = FALSE;
}

//该接口仅仅在CONFIG开机时调用一次
void com_init_dae_config(dae_config_t *p_dae_cfg)
{
    dae_config_t *p_dae_cfg_shm;

    p_dae_cfg_shm = sys_malloc(sizeof(dae_config_t));
    if (p_dae_cfg_shm == NULL)
    {
        libc_printf("p_dae_cfg_shm\n");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    p_dae_cfg->aset_mode = FALSE;

    libc_memcpy(p_dae_cfg_shm, p_dae_cfg, sizeof(dae_config_t));

    if (sys_shm_creat(SHARE_MEM_ID_DAECFG, p_dae_cfg_shm, sizeof(dae_config_t)) == -1)
    {
        while (1);
    }
}

//更新DAE参数到共享内存中
void com_update_dae_config(dae_config_t *p_dae_cfg)
{
    dae_config_t *p_dae_cfg_shm;
    msg_apps_t seteq_msg;

    p_dae_cfg->aset_mode = FALSE;
#if (SUPPORT_ASET_TEST == 1)
    if (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASET_EQ_MODE)
    {
        p_dae_cfg->aset_mode = TRUE;
    }
#endif
#if (SUPPORT_ASQT_TEST == 1)
    if (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASQT_MODE)
    {
        p_dae_cfg->aset_mode = TRUE;
    }
#endif

    p_dae_cfg_shm = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (p_dae_cfg_shm != NULL)
    {
        libc_memcpy(p_dae_cfg_shm, p_dae_cfg, sizeof(dae_config_t));
    }
    else
    {
        libc_printf_warning("SHARE_MEM_ID_DAECFG not exist!!\n");
    }

    //发送消息给引擎
    if (get_mmm_state_inner() == MMM_STATE_KT_PLAYING)
    {
#if (SUPPORT_OK_MIC == 1)
        //按键音通道中间，直接发给按键音通道中间件即可
        com_ok_mic_idle_eq_set();
#endif
    }
    else if (get_mmm_state_inner() == MMM_STATE_TTS_PLAYING)
    {
        //TTS播放中间件，不需要调节DAE参数
    }
    else
    {
        if (get_engine_type_inner() != ENGINE_NULL)
        {
            seteq_msg.type = MSG_SET_EQ_SYNC;
            send_sync_msg_inner(APP_ID_THEENGINE, &seteq_msg, NULL, 0);
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置EQ。
 * \param[in]    mode 设置EQ模式，0表示指定EQ，1表示向下切EQ，2表示向上切EQ
 * \param[in]    eq_type 指定EQ，只有mode为0时有效
 * \param[in]    eq_para 用户EQ参数，只有设置用户EQ有效
 *                       NULL表示使用系统当前的用户EQ参数，非NULL表示更新用户EQ参数
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 设置EQ成功
 * \retval           FALSE 设置EQ失败
 * \note
 *******************************************************************************/
bool com_switch_geq_inner(uint8 mode, com_eqtype_e eq_type, uint8 *eq_para)
{
    switch (mode)
    {
        case SETEQ_NEXT:
        if (g_com_env_var.eq_type >= EQ_USR_DEF)
        {
            g_com_env_var.eq_type = EQ_NOR;
        }
        else
        {
            g_com_env_var.eq_type++;
        }
        break;

        case SETEQ_PREV:
        if (g_com_env_var.eq_type == EQ_NOR)
        {
            g_com_env_var.eq_type = EQ_USR_DEF;
        }
        else
        {
            g_com_env_var.eq_type--;
        }
        break;

        case SETEQ_TYPE:
        g_com_env_var.eq_type = eq_type;
        break;

        default:
        break;
    }

    libc_printf("switch geq:%d\n", g_com_env_var.eq_type);

    //load GEQ config
    com_load_geq_config(g_p_com_dae_cfg, g_com_env_var.eq_type);

    //更新参数到共享内存
    com_update_dae_config(g_p_com_dae_cfg);

    return TRUE;
}

//动态开关DAE
bool com_set_dae_onoff_inner(bool onoff)
{
    if (onoff == FALSE)
    {
        g_p_com_dae_cfg->bypass = TRUE;
        g_p_com_dae_cfg->precut = 0;
    }
    else
    {
        g_p_com_dae_cfg->bypass = (uint8) com_get_config_default_inner(DAE_BYPASS_ENABLE);
        g_p_com_dae_cfg->precut = (int16) com_get_config_default_inner(DAE_PRECUT_RATIO);
    }

    com_update_dae_config(g_p_com_dae_cfg);

    return TRUE;
}

//动态开关DRC
bool com_set_drc_onoff_inner(bool onoff)
{
    if (onoff == FALSE)
    {
        g_p_com_dae_cfg->drc_enable = FALSE;
    }
    else
    {
        g_p_com_dae_cfg->drc_enable = com_get_config_default_inner(DAE_DRC_ENABLE);
    }

    com_update_dae_config(g_p_com_dae_cfg);

    //开关DRC需要重置音量
    com_reset_sound_volume_inner(SET_VOL_BY_SYSTEM);

    return TRUE;
}

//切换普通模式和AUX模式的DRC参数
//0-普通模式DRC, 1-AUX模式DRC
//返回当前模式，以便在临时设置模式后恢复模式
uint8 com_switch_drc_mode_inner(uint8 mode)
{
    cfg_drc_band_config_t temp_drc_band_config;
    bool need_switch = FALSE;
    uint8 backup_mode = g_p_com_dae_cfg->drc_mode;

    if (mode == backup_mode)
    {
        return backup_mode;
    }

#if (SUPPORT_ASET_TEST == 1)
    if (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASET_EQ_MODE)
    {
        need_switch = TRUE;
    }
#endif
    if (g_p_com_dae_cfg->drc_enable == TRUE)
    {
        need_switch = TRUE;
    }

    if (need_switch == FALSE)
    {
        return backup_mode;
    }

    if (mode == DRC_MODE_AUX)
    {
        g_p_com_dae_cfg->drc_crossover = com_get_config_default_inner(DAE_AUX_DRC_CROSSOVER);
        g_p_com_dae_cfg->pre_softvol_gain = (int16) com_get_config_default_inner(DAE_AUX_DRC_PRE_SOFTVOL_GAIN);
        g_p_com_dae_cfg->post_softvol_gain = (int16) com_get_config_default_inner(DAE_AUX_DRC_POST_SOFTVOL_GAIN);

        com_get_config_struct_inner(DAE_AUX_DRC_BAND1_SETTING, (uint8 *) &temp_drc_band_config, sizeof(cfg_drc_band_config_t));
        libc_memcpy(&(g_p_com_dae_cfg->drc_bands[0]), &(temp_drc_band_config.drc_band_para), sizeof(drc_band_t));
        com_get_config_struct_inner(DAE_AUX_DRC_BAND2_SETTING, (uint8 *) &temp_drc_band_config, sizeof(cfg_drc_band_config_t));
        libc_memcpy(&(g_p_com_dae_cfg->drc_bands[1]), &(temp_drc_band_config.drc_band_para), sizeof(drc_band_t));
    }
    else
    {
        g_p_com_dae_cfg->drc_crossover = com_get_config_default_inner(DAE_DRC_CROSSOVER);
        g_p_com_dae_cfg->pre_softvol_gain = (int16) com_get_config_default_inner(DAE_DRC_PRE_SOFTVOL_GAIN);
        g_p_com_dae_cfg->post_softvol_gain = (int16) com_get_config_default_inner(DAE_DRC_POST_SOFTVOL_GAIN);

        com_get_config_struct_inner(DAE_DRC_BAND1_SETTING, (uint8 *) &temp_drc_band_config, sizeof(cfg_drc_band_config_t));
        libc_memcpy(&(g_p_com_dae_cfg->drc_bands[0]), &(temp_drc_band_config.drc_band_para), sizeof(drc_band_t));
        com_get_config_struct_inner(DAE_DRC_BAND2_SETTING, (uint8 *) &temp_drc_band_config, sizeof(cfg_drc_band_config_t));
        libc_memcpy(&(g_p_com_dae_cfg->drc_bands[1]), &(temp_drc_band_config.drc_band_para), sizeof(drc_band_t));
    }

    g_p_com_dae_cfg->drc_mode = mode;

    com_update_dae_config(g_p_com_dae_cfg);

    return backup_mode;
}

//设置前置软音量
bool com_set_drc_precut_inner(int16 precut)
{
    g_p_com_dae_cfg->drc_precut = precut;
    com_update_dae_config(g_p_com_dae_cfg);
    return TRUE;
}

//通话场景PEQ参数设置
bool com_set_speech_peq_para_inner(bool in)
{
    if (in == TRUE)
    {//进入通话场景
        peq_config_t tmp_peq_config;
        uint8 i;

        p_dae_peq_backup = (dae_config_t *) sys_malloc(sizeof(dae_config_t));
        if (p_dae_peq_backup == NULL)
        {
            libc_printf("p_dae_peq_backup\n");
            return FALSE;
        }

        //首先把DAE PEQ参数备份起来
        libc_memcpy(p_dae_peq_backup, g_p_com_dae_cfg, sizeof(dae_config_t));

        //设置通话PEQ参数
        g_p_com_dae_cfg->bypass = (uint8) com_get_config_default_inner(BTCALL_BYPASS_ENABLE);
        g_p_com_dae_cfg->precut = 0;
        g_p_com_dae_cfg->peq_enable = (uint8) com_get_config_default_inner(BTCALL_PEQ_ENABLE);
        g_p_com_dae_cfg->kt_peq_band_num = (uint8) com_get_config_default_inner(BTCALL_PEQ_BAND_NUM_POSTREAT);
        for (i = 0; i < EQ_NUM_BANDS; i++)
        {
            com_get_config_struct_inner((uint16)(BTCALL_PEQ_BANK0_SETTING + i), (uint8 *) &tmp_peq_config, sizeof(peq_config_t));
            g_p_com_dae_cfg->music_eq_id[i] = tmp_peq_config.default_value;
            libc_memcpy(&(g_p_com_dae_cfg->peq_bands[i]), &(tmp_peq_config.eq_data), sizeof(peq_band_t));
        }

        //更新参数到共享内存
        com_update_dae_config(g_p_com_dae_cfg);

        return TRUE;
    }
    else
    {//退出通话场景
        //首先恢复DAE PEQ参数
        g_p_com_dae_cfg->bypass = p_dae_peq_backup->bypass;
        g_p_com_dae_cfg->precut = p_dae_peq_backup->precut;
        g_p_com_dae_cfg->peq_enable = p_dae_peq_backup->peq_enable;
        g_p_com_dae_cfg->kt_peq_band_num = p_dae_peq_backup->kt_peq_band_num;
        libc_memcpy(g_p_com_dae_cfg->peq_bands, p_dae_peq_backup->peq_bands, sizeof(peq_band_t)*EQ_NUM_BANDS);
        libc_memcpy(g_p_com_dae_cfg->music_eq_id, p_dae_peq_backup->music_eq_id, sizeof(g_p_com_dae_cfg->music_eq_id));

        sys_free(p_dae_peq_backup);
        p_dae_peq_backup = NULL;

        //更新参数到共享内存
        com_update_dae_config(g_p_com_dae_cfg);

        return TRUE;
    }
}

bool com_update_speech_peq_para_inner(bool peq_enable, peq_band_t * peq_array)
{
    g_p_com_dae_cfg->peq_enable = peq_enable;
    libc_memcpy(g_p_com_dae_cfg->peq_bands, peq_array, sizeof(peq_band_t)*EQ_NUM_BANDS);

    //更新参数到共享内存
    com_update_dae_config(g_p_com_dae_cfg);

    return TRUE;
}

#if (SUPPORT_OK_MIC == 1)

//混响深度提高
bool com_set_dae_reverb_depth_add(void)
{
    if (g_p_com_dae_cfg->echo_enable == 1)
    {
        echo_reverb_para_t tmp_echo_para;
        com_get_config_struct_inner(OK_MIC_ECHO_RATIO_ARRAY, (uint8 *) &tmp_echo_para, sizeof(echo_reverb_para_t));

        if (g_p_com_dae_cfg->echo_ratio_index < (tmp_echo_para.total-1))
        {
            g_p_com_dae_cfg->echo_ratio_index++;
            g_p_com_dae_cfg->echo_ratio = tmp_echo_para.value[g_p_com_dae_cfg->echo_ratio_index];
            libc_printf_info("echo_ratio:%d\n", g_p_com_dae_cfg->echo_ratio);
            com_update_dae_config(g_p_com_dae_cfg);
        }
        else
        {
            //com_tts_state_play(TTS_PARATIPS, TTS_MODE_KEY_NORECV | TTS_MODE_SYS_NORECV);
            //message_key_flush_inner();
        }
    }

    return TRUE;
}

//混响深度降低
bool com_set_dae_reverb_depth_sub(void)
{
    if (g_p_com_dae_cfg->echo_enable == 1)
    {
        echo_reverb_para_t tmp_echo_para;
        com_get_config_struct_inner(OK_MIC_ECHO_RATIO_ARRAY, (uint8 *) &tmp_echo_para, sizeof(echo_reverb_para_t));

        if (g_p_com_dae_cfg->echo_ratio_index > 0)
        {
            g_p_com_dae_cfg->echo_ratio_index--;
            g_p_com_dae_cfg->echo_ratio = tmp_echo_para.value[g_p_com_dae_cfg->echo_ratio_index];
            libc_printf_info("echo_ratio:%d\n", g_p_com_dae_cfg->echo_ratio);
            com_update_dae_config(g_p_com_dae_cfg);
        }
        else
        {
            //com_tts_state_play(TTS_PARATIPS, TTS_MODE_KEY_NORECV | TTS_MODE_SYS_NORECV);
            //message_key_flush_inner();
        }
    }

    return TRUE;
}

//混响深度设置
bool com_set_dae_reverb_depth_set(uint8 ratio_index)
{
    if (g_p_com_dae_cfg->echo_enable == 1)
    {
        echo_reverb_para_t tmp_echo_para;
        com_get_config_struct_inner(OK_MIC_ECHO_RATIO_ARRAY, (uint8 *) &tmp_echo_para, sizeof(echo_reverb_para_t));

        if (ratio_index > (tmp_echo_para.total-1))
        {
            ratio_index = (tmp_echo_para.total-1);
        }

        g_p_com_dae_cfg->echo_ratio_index = ratio_index;
        g_p_com_dae_cfg->echo_ratio = tmp_echo_para.value[g_p_com_dae_cfg->echo_ratio_index];
        libc_printf_info("echo_ratio:%d\n", g_p_com_dae_cfg->echo_ratio);
        com_update_dae_config(g_p_com_dae_cfg);
    }

    return TRUE;
}

/*
 * 14 个 PEQ 点，由 OK_MIC_PEQ_BAND_NUM 划分，前面(14-OK_MIC_PEQ_BAND_NUM)给伴奏，后面OK_MIC_PEQ_BAND_NUM给MIC
 * 所以，MIC增强从下标13开始用4个点做增强用；伴奏增强预留前面2个点做高通滤波，接着4个点做增强用
*/

//MIC-低音增强
void vr_adc_mic_low_enhance_cbk(int16 p1_gain, int16 p2_gain)
{
    g_p_com_dae_cfg->peq_bands[10].gain = p1_gain;
    g_p_com_dae_cfg->peq_bands[11].gain = p2_gain;
    com_update_dae_config(g_p_com_dae_cfg);
}
//MIC-高音增强
void vr_adc_mic_high_enhance_cbk(int16 p1_gain, int16 p2_gain)
{
    g_p_com_dae_cfg->peq_bands[12].gain = p1_gain;
    g_p_com_dae_cfg->peq_bands[13].gain = p2_gain;
    com_update_dae_config(g_p_com_dae_cfg);
}
//伴奏-低音增强
void vr_adc_play_low_enhance_cbk(int16 p1_gain, int16 p2_gain)
{
    g_p_com_dae_cfg->peq_bands[2].gain = p1_gain;
    g_p_com_dae_cfg->peq_bands[3].gain = p2_gain;
    com_update_dae_config(g_p_com_dae_cfg);
}
//伴奏-高音增强
void vr_adc_play_high_enhance_cbk(int16 p1_gain, int16 p2_gain)
{
    g_p_com_dae_cfg->peq_bands[4].gain = p1_gain;
    g_p_com_dae_cfg->peq_bands[5].gain = p2_gain;
    com_update_dae_config(g_p_com_dae_cfg);
}

bool com_set_dae_ducking_switch(void)
{
    if ((uint8) com_get_config_default_inner(OK_MIC_DUCKING_ENABLE) == FALSE)
    {
        return FALSE;
    }

    if (g_p_com_dae_cfg->ducking_enable == FALSE)
    {
        g_p_com_dae_cfg->ducking_enable = TRUE;
        //com_tts_state_play_inner(TTS_DUCKING_ON, 0);
    }
    else
    {
        g_p_com_dae_cfg->ducking_enable = FALSE;
        //com_tts_state_play_inner(TTS_DUCKING_OFF, 0);
    }
    libc_printf_info("ducking enable:%d\n", g_p_com_dae_cfg->ducking_enable);
    com_update_dae_config(g_p_com_dae_cfg);

    return g_p_com_dae_cfg->ducking_enable;
}

bool com_set_dae_voice_removal_switch(void)
{
    if ((uint8) com_get_config_default_inner(PLAY_VOICE_REMOVAL_ENABLE) == FALSE)
    {
        return FALSE;
    }

    if (g_p_com_dae_cfg->voice_removal_enable == FALSE)
    {
        g_p_com_dae_cfg->voice_removal_enable = TRUE;
        //com_tts_state_play_inner(TTS_VR_ON, 0);
    }
    else
    {
        g_p_com_dae_cfg->voice_removal_enable = FALSE;
        //com_tts_state_play_inner(TTS_VR_OFF, 0);
    }
    libc_printf_info("voice removal enable:%d\n", g_p_com_dae_cfg->voice_removal_enable);
    com_update_dae_config(g_p_com_dae_cfg);

    return g_p_com_dae_cfg->voice_removal_enable;
}

bool com_set_dae_ok_mic_mute(bool mute)
{
    g_p_com_dae_cfg->ok_mic_mute = mute;
    libc_printf_info("ok mic mute:%d\n", mute);
    com_update_dae_config(g_p_com_dae_cfg);

    return TRUE;
}

#endif
