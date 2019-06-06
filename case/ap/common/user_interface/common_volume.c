/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音量处理接口，包括调节音量，切换多种音量模式等；支持电池电量低限制音量功能。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#define VOL_PHY_LIMIT_DISABLE  0
#define VOL_PHY_LIMIT_LIGHT    1
#define VOL_PHY_LIMIT_MODERATE 2
#define VOL_PHY_LIMIT_HEAVY    3
#define VOL_PHY_LIMIT_USER_DEF 4

static const int16 volume_phy_limit_tbl_light[BATTERY_GRADE_MAX + 1]    = {-50, -50, -30,-20,-10,  0,0,0,0,0};
static const int16 volume_phy_limit_tbl_moderate[BATTERY_GRADE_MAX + 1] = {-80, -80, -60,-40,-25,-10,0,0,0,0};
static const int16 volume_phy_limit_tbl_heavy[BATTERY_GRADE_MAX + 1]    = {-100,-100,-80,-60,-40,-20,0,0,0,0};

typedef struct
{
    uint16 temp1;
    uint16 total;
    int16 precut;
    int16 volume;
} drc_volume_cfg_t;

typedef struct
{
    uint16 active; //此参数没用到，只是保持与config.bin中的数据结构一致
    uint16 total; //按键总数
    uint16 limit_val[BATTERY_GRADE_MAX + 1]; //按键定义数组
} volume_limit_var_t;

volume_infor_t g_volume_infor;

#if (SUPPORT_HDMI_MODULE == 1)
static uint8 __sys_vol_2_cec_vol(uint8 sys_vol)
{
    return (sys_vol*CEC_VOLUME_MAX/VOLUME_VALUE_MAX);
}
#endif

void volume_var_init(void)
{
    volume_limit_var_t tmp_vol_limit_var;
    uint8 volume_phy_limit_grade;
    uint8 i;

    g_volume_infor.main_gain = (int16) com_get_config_default_inner(SETTING_SOUND_MAIN_VOLUME_GAIN);
    g_volume_infor.call_gain = (int16) com_get_config_default_inner(BTCALL_SPEAKER_VOLUME_GAIN);
    g_volume_infor.linein_gain = (int16) com_get_config_default_inner(LINEIN_VOLUME_GAIN);
    g_volume_infor.radio_gain = (int16) com_get_config_default_inner(RADIO_VOLUME_GAIN);
    g_volume_infor.gain_mode = VOLUME_GAIN_MAIN;
    g_volume_infor.gain_current = g_volume_infor.main_gain;

    volume_phy_limit_grade = (int16) com_get_config_default_inner(SETTING_SOUND_VOLUME_PHY_LIMIT_GRADE);
    if (volume_phy_limit_grade == VOL_PHY_LIMIT_DISABLE)
    {
        libc_memset(g_volume_infor.phy_limit_tbl, 0x00, sizeof(g_volume_infor.phy_limit_tbl));
    }
    else if (volume_phy_limit_grade == VOL_PHY_LIMIT_LIGHT)
    {
        libc_memcpy(g_volume_infor.phy_limit_tbl, volume_phy_limit_tbl_light, sizeof(g_volume_infor.phy_limit_tbl));
    }
    else if (volume_phy_limit_grade == VOL_PHY_LIMIT_MODERATE)
    {
        libc_memcpy(g_volume_infor.phy_limit_tbl, volume_phy_limit_tbl_moderate, sizeof(g_volume_infor.phy_limit_tbl));
    }
    else if (volume_phy_limit_grade == VOL_PHY_LIMIT_HEAVY)
    {
        libc_memcpy(g_volume_infor.phy_limit_tbl, volume_phy_limit_tbl_heavy, sizeof(g_volume_infor.phy_limit_tbl));
    }
    else
    {
        com_get_config_struct_inner(SETTING_SOUND_VOLUME_PHY_LIMIT_TBL, (uint8 *) &tmp_vol_limit_var, sizeof(volume_limit_var_t));
        for (i = 0; i < (BATTERY_GRADE_MAX + 1); i++)
        {
            g_volume_infor.phy_limit_tbl[i] = (int16) tmp_vol_limit_var.limit_val[i];
        }
    }

    g_volume_infor.phy_current = 0;
    g_volume_infor.phy_limit_current = g_volume_infor.phy_limit_tbl[g_app_info_state.bat_value];
    if (g_volume_infor.phy_limit_current < 0)
    {
        libc_printf_info("phy_limit_current:%d\n", g_volume_infor.phy_limit_current);
    }

    g_volume_infor.volume_mode = VOLUME_MODE_MAIN;
    g_volume_infor.volume_current_max = VOLUME_VALUE_MAX;

    if ((uint8) com_get_config_default_inner(SETTING_SOUND_VOLUME_RESET_POWERON) == TRUE)
    {
        g_volume_infor.main_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_DEFAULT_MAIN_VOLUME);
        g_volume_infor.call_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_DEFAULT_CALL_VOLUME);
    }
    else
    {
        uint8 min_volume;
        uint8 max_volume;
        uint8 cur_volume;

        g_volume_infor.main_volume = g_com_env_var.main_volume;
        g_volume_infor.call_volume = g_com_env_var.call_volume;

        min_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_MIN_VOLUME_POWERON);
        max_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_MAX_VOLUME_POWERON);
        cur_volume = g_volume_infor.main_volume;
        if (cur_volume < min_volume)
        {
            cur_volume = min_volume;
        }
        if (cur_volume > max_volume)
        {
            cur_volume = max_volume;
        }
        g_volume_infor.main_volume = cur_volume;
    }

    g_volume_infor.volume_current = g_volume_infor.main_volume;

#if (SUPPORT_OK_MIC == 1)
    g_volume_infor.mic_soft_volume_gain = (int16) com_get_config_default_inner(OK_MIC_SOFT_VOLUME_GAIN);
#if (SUPPORT_VR_ADCKEY == 1)
    com_set_mic_volume(0); //MIC音量初始静音，等待读取调节器的音量值
#else
    com_set_mic_volume(VOLUME_VALUE_MAX);
#endif
#endif

    //正常开机设置默认值
    if (g_p_esd_bk_info->flag != ESD_BK_FLAG)
    {
        g_p_esd_bk_info->volume_value = g_volume_infor.main_volume;
        g_p_esd_bk_info->volume_value_call = g_volume_infor.call_volume;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前声音音量。
 * \param[in]    none
 * \param[out]   none
 * \return       uint8 当前音量值，0xff表示无效
 * \note
 *******************************************************************************/
uint8 com_get_sound_volume_inner(void)
{
    return g_volume_infor.volume_current;
}

//main volume -> pc volume
uint8 com_get_sound_volume_pc_inner(void)
{
    int32 current_pc_db;
    uint8 volume_pc;

    current_pc_db = g_aud_vol_p1_db_table[g_volume_infor.volume_current];

    for (volume_pc = 0; volume_pc <= 100; volume_pc++)
    {
        if (g_aud_vol_p1_db_pc_vol_table[volume_pc] >= current_pc_db)
        {
            break;
        }
    }

    return volume_pc;
}

//pc volume -> main volume
uint8 com_get_sound_volume_from_pc(uint8 volume_pc)
{
    int16 pc_volume_gain;
    int16 main_volume_gain;
    uint8 main_volume;

    pc_volume_gain = g_aud_vol_p1_db_pc_vol_table[volume_pc];

    if ((g_p_com_dae_cfg->bypass == 0) && (g_p_com_dae_cfg->drc_enable == 1))
    {
        drc_volume_cfg_t drc_volume_cfg;

        for (main_volume = VOLUME_VALUE_MAX; main_volume > 0; main_volume--)
        {
            com_get_config_struct_inner(DAE_DRC_VOL_GRADE_31 + (VOLUME_VALUE_MAX - main_volume),
                (uint8 *) &drc_volume_cfg, sizeof(drc_volume_cfg_t));

            main_volume_gain = drc_volume_cfg.precut + drc_volume_cfg.volume;

            if (main_volume_gain <= pc_volume_gain)
            {
                break;
            }
        }
    }
    else
    {
        for (main_volume = VOLUME_VALUE_MAX; main_volume > 0; main_volume--)
        {
            main_volume_gain = g_aud_vol_p1_db_table[main_volume];

            if (main_volume_gain <= pc_volume_gain)
            {
                break;
            }
        }
    }

    return main_volume;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    重置当前声音音量。
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 重置音量成功
 * \retval           FALSE 重置音量失败
 * \note
 *******************************************************************************/
bool com_reset_sound_volume_inner(uint8 mode)
{
    return com_set_sound_volume_inner(mode, g_volume_infor.volume_current);
}

//设置物理音量，根据 phy_current、gain_current、phy_limit_current
void com_set_phy_volume(void)
{
    int16 set_volume_db = g_volume_infor.phy_current;

    if (g_volume_infor.gain_current > 0)//正增益，以增大输出功率为优先
    {
        if (set_volume_db > g_volume_infor.phy_limit_current)
        {
            libc_printf_info("set volume %d > volume limit %d\n", set_volume_db, g_volume_infor.phy_limit_current);
            set_volume_db = g_volume_infor.phy_limit_current;
        }

        set_volume_db += g_volume_infor.gain_current;
    }
    else//负增益，以限制输出功率为优先
    {
        set_volume_db += g_volume_infor.gain_current;

        if (set_volume_db > g_volume_infor.phy_limit_current)
        {
            libc_printf_info("set volume %d > volume limit %d\n", set_volume_db, g_volume_infor.phy_limit_current);
            set_volume_db = g_volume_infor.phy_limit_current;
        }
    }

    //OK MIC应用模式使用软音量，DAC音量固定为0dB
#if (SUPPORT_OK_MIC == 1)
    g_p_com_dae_cfg->soft_volume = set_volume_db;
    com_update_dae_config(g_p_com_dae_cfg);
    set_audio_volume(0);
#else
    set_audio_volume(set_volume_db);
#endif

    libc_printf_info("set audio volume %d\n", set_volume_db);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置声音音量。
 * \param[in]    volume 设置音量值
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 设置音量成功
 * \retval           FALSE 设置音量失败
 * \note
 *******************************************************************************/
bool com_set_sound_volume_inner(uint8 mode, uint8 volume)
{
    int16 set_volume_db;
    bool bret = TRUE;

    if (mode != SET_VOL_BY_PC_VOL)
    {
        //保存当前音量值
        g_volume_infor.volume_current = volume;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
        {
            //TTS过程中临时调节音量不要同步给副箱
            if (g_tts_play_infor.tts_playing == 0)
            {
                if (g_volume_infor.tws_sync_volume_flag == TWS_SYNC_VOL_NULL)
                {
                    if (volume != g_volume_infor.tws_sync_volume_value)
                    {
                        com_tws_m2s_set_vol_val_cmd(volume);
                        g_volume_infor.tws_sync_volume_value = volume;
                    }
                    g_volume_infor.tws_sync_volume_flag = TWS_SYNC_VOL_RIGHT_NOW;
                    g_volume_infor.tws_sync_volume_delay_timer = sys_get_ab_timer();
                }
                else if (g_volume_infor.tws_sync_volume_flag == TWS_SYNC_VOL_RIGHT_NOW)
                {
                    g_volume_infor.tws_sync_volume_delay = volume;
                    g_volume_infor.tws_sync_volume_flag = TWS_SYNC_VOL_DELAY;
                }
                else
                {
                    g_volume_infor.tws_sync_volume_delay = volume;
                }
            }
        }
#endif

        //ESD RESET实时备份音量；开机时不要更新，这样才不会把备份音量覆盖了
        if (get_cur_app_id_inner() != APP_ID_CONFIG)
        {
            if (g_volume_infor.volume_mode == VOLUME_MODE_MAIN)
            {
                g_p_esd_bk_info->volume_value = g_volume_infor.main_volume = g_volume_infor.volume_current;
            }
            else
            {
                g_p_esd_bk_info->volume_value_call = g_volume_infor.call_volume = g_volume_infor.volume_current;
            }
        }
    }

    //只有非静音时才需要调整音量
    if (get_mute_enable() == FALSE)
    {
        int16 kt_set_volume_db, kt_max_volume;

        if (mode != SET_VOL_BY_PC_VOL)
        {
            if (g_volume_infor.volume_mode == VOLUME_MODE_MAIN)
            {
                if ((g_p_com_dae_cfg->bypass == 0) && (g_p_com_dae_cfg->drc_enable == 1))
                {
                    drc_volume_cfg_t drc_volume_cfg;

                    if (volume > VOLUME_VALUE_MAX)
                    {
                        volume = VOLUME_VALUE_MAX;
                    }

                    com_get_config_struct_inner(DAE_DRC_VOL_GRADE_31 + (VOLUME_VALUE_MAX - volume),
                        (uint8 *) &drc_volume_cfg, sizeof(drc_volume_cfg_t));

                    //更新软音量
                    com_set_drc_precut_inner(drc_volume_cfg.precut);

                    set_volume_db = drc_volume_cfg.volume;

                    libc_printf_info("set drc volume [%d,%d]\n", drc_volume_cfg.precut, set_volume_db);
                }
                else
                {
                    set_volume_db = g_aud_vol_p1_db_table[volume];
                }
            }
            else
            {
                if (volume > VOLUME_VALUE_CALL_MAX)
                {
                    libc_printf_warning("invalid call volume!!\n");
                    volume = VOLUME_VALUE_CALL_MAX;
                }
                set_volume_db = g_aud_vol_p1_db_call_table[volume];
            }
        }
        else //vol [0 ~ 100], 100 for 0dB, 50 for -6dB
        {
            set_volume_db = g_aud_vol_p1_db_pc_vol_table[volume];

            if ((g_p_com_dae_cfg->bypass == 0) && (g_p_com_dae_cfg->drc_enable == 1))
            {
                uint8 main_volume;
                drc_volume_cfg_t drc_volume_cfg;
                int16 diff_volume;

                main_volume = com_get_sound_volume_from_pc(volume);

                com_get_config_struct_inner(DAE_DRC_VOL_GRADE_31 + (VOLUME_VALUE_MAX - main_volume),
                    (uint8 *) &drc_volume_cfg, sizeof(drc_volume_cfg_t));

                //更新软音量
                diff_volume = set_volume_db - (drc_volume_cfg.precut + drc_volume_cfg.volume);
                drc_volume_cfg.precut += diff_volume/2;
                drc_volume_cfg.volume += diff_volume - (diff_volume/2);
                com_set_drc_precut_inner(drc_volume_cfg.precut);

                set_volume_db = drc_volume_cfg.volume;

                libc_printf_info("set drc volume [%d,%d]\n", drc_volume_cfg.precut, set_volume_db);
            }
            g_volume_infor.usbbox_volume = volume;
        }

        kt_set_volume_db = set_volume_db;
        g_volume_infor.phy_current = set_volume_db;

        //设置物理音量
        com_set_phy_volume();

        //只更新音箱音量，忽略音量同步，或者没有蓝牙引擎时直接返回
        if ((mode == SET_VOL_BY_REMOTE) || (mode == SET_VOL_BY_PC_VOL) || (mode == SET_VOL_BY_SYSTEM)
            || (get_bt_state_inner() == BT_STATE_NONE))
        {
            return bret;
        }
        //同步音箱端的音量到远端设备(手机或ipad等)
        com_syn_vol_to_phone(volume);
    }

    return bret;
}

#if (SUPPORT_OK_MIC == 1)
//OK MIC独立调节MIC音量
void com_set_mic_volume(uint8 volume)
{
    int16 set_volume_db;

    if (volume > VOLUME_VALUE_MAX)
    {
        volume = VOLUME_VALUE_MAX;
    }

    g_volume_infor.mic_volume_current = volume;

    set_volume_db = g_aud_vol_p1_db_table[volume];
    set_volume_db += g_volume_infor.mic_soft_volume_gain;

    g_p_com_dae_cfg->kt_soft_volume = set_volume_db;

    libc_printf("mic volume set:%d\n", set_volume_db);

    com_update_dae_config(g_p_com_dae_cfg);
}
#endif

//TTS后恢复USB音箱音量
void com_set_usbbox_volume_ctrl_inner(uint8 ctrl_mode)
{
    if (ctrl_mode == USBBOX_VOL_CTRL_ENTER)
    {
        g_volume_infor.usbbox_volume_flag = TRUE;
    }
    else if (ctrl_mode == USBBOX_VOL_CTRL_EXIT)
    {
        g_volume_infor.usbbox_volume_flag = FALSE;
    }
    else
    {
        if (g_volume_infor.usbbox_volume_flag == TRUE)
        {
            com_set_sound_volume_inner(SET_VOL_BY_PC_VOL, g_volume_infor.usbbox_volume);
        }
    }
}

//设置音量模式和音量增益模式
void com_set_sound_volume_mode_inner(uint8 volume_mode, uint8 gain_mode)
{
    uint8 set_mode = SET_VOL_BY_SYSTEM;

    g_volume_infor.gain_mode = gain_mode;
    if (g_volume_infor.gain_mode == VOLUME_GAIN_CALL)
    {
        g_volume_infor.gain_current = g_volume_infor.call_gain;
    }
    else if (g_volume_infor.gain_mode == VOLUME_GAIN_LINEIN)
    {
        g_volume_infor.gain_current = g_volume_infor.linein_gain;
    }
    else if (g_volume_infor.gain_mode == VOLUME_GAIN_RADIO)
    {
        g_volume_infor.gain_current = g_volume_infor.radio_gain;
    }
    else
    {
        g_volume_infor.gain_current = g_volume_infor.main_gain;
    }

    if ((g_volume_infor.volume_mode == VOLUME_MODE_CALL) && (volume_mode == VOLUME_MODE_MAIN))
    {
        g_volume_infor.volume_mode = VOLUME_MODE_MAIN;
        g_volume_infor.volume_current_max = VOLUME_VALUE_MAX;
        g_volume_infor.call_volume = g_volume_infor.volume_current;
        g_volume_infor.volume_current = g_volume_infor.main_volume;
        set_mode = SET_VOL_BY_SWITCH;
    }
    else if ((g_volume_infor.volume_mode == VOLUME_MODE_MAIN) && (volume_mode == VOLUME_MODE_CALL))
    {
        g_volume_infor.volume_mode = VOLUME_MODE_CALL;
        g_volume_infor.volume_current_max = VOLUME_VALUE_CALL_MAX;
        g_volume_infor.main_volume = g_volume_infor.volume_current;
        g_volume_infor.volume_current = g_volume_infor.call_volume;
        set_mode = SET_VOL_BY_SWITCH;
    }

    com_set_sound_volume_inner(set_mode, g_volume_infor.volume_current);
}

//获取音量模式和音量增益模式
uint8 com_get_sound_volume_mode(void)
{
    return g_volume_infor.volume_mode;
}
uint8 com_get_sound_volume_gain_mode(void)
{
    return g_volume_infor.gain_mode;
}

//设置音量增益值
void com_set_sound_volume_gain_inner(int16 gain)
{
    g_volume_infor.gain_current = gain;
    if (g_volume_infor.gain_mode == VOLUME_GAIN_MAIN)
    {
        g_volume_infor.main_gain = g_volume_infor.gain_current;
    }
    else if (g_volume_infor.gain_mode == VOLUME_GAIN_CALL)
    {
        g_volume_infor.call_gain = g_volume_infor.gain_current;
    }
    else if (g_volume_infor.gain_mode == VOLUME_GAIN_LINEIN)
    {
        g_volume_infor.linein_gain = g_volume_infor.gain_current;
    }
    else if (g_volume_infor.gain_mode == VOLUME_GAIN_RADIO)
    {
        g_volume_infor.radio_gain = g_volume_infor.gain_current;
    }

    com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_volume_infor.volume_current);
}

void com_volume_value_display(uint8 vol)
{
    led_display_icon(LCD_COL, FALSE);
    led_display_icon(LCD_P1, FALSE);
    led_display(NUMBER1, 'U', TRUE);
    led_display(NUMBER2, 0, FALSE);
    led_display(NUMBER3, vol/10, TRUE);
    led_display(NUMBER4, vol%10, TRUE);
}

static void _volume_sound_tips(void)
{
#if (SUPPORT_SOFT_KEYTONE == 1)
    keytone_play_inner (KT_ID_KEYTONE);
#else
    keytone_play_inner (KT_MODE_KEYTONE);
#endif
}

void com_volume_sub_inner(uint8 mode)
{
    if (get_mute_enable() == TRUE)
    {
        //取消静音状态
        com_set_mute_inner(FALSE);
        return;
    }

    if (g_volume_infor.volume_current > 0)
    {
        g_volume_infor.volume_current--;
        libc_printf_info("volume:%d\n", g_volume_infor.volume_current);
        com_set_sound_volume_inner(mode, g_volume_infor.volume_current);

#if (SUPPORT_HDMI_MODULE == 1)
        hdmi_set_volume(__sys_vol_2_cec_vol(g_volume_infor.volume_current));
#endif
    }

#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment != 0)
    {
        com_led_force_exit_msg_show();
        com_volume_value_display(g_volume_infor.volume_current);
        led_enter_msg_show(3000);
    }
#endif

    if (g_volume_infor.volume_current == 0)
    {
        _volume_sound_tips();
    }
}

void com_volume_add_inner(uint8 mode)
{
    if (get_mute_enable() == TRUE)
    {
        //取消静音状态
        com_set_mute_inner(FALSE);
        return;
    }

    if (g_volume_infor.volume_current < g_volume_infor.volume_current_max)
    {
        g_volume_infor.volume_current++;
        libc_printf_info("volume:%d\n", g_volume_infor.volume_current);
        com_set_sound_volume_inner(mode, g_volume_infor.volume_current);

#if (SUPPORT_HDMI_MODULE == 1)
        hdmi_set_volume(__sys_vol_2_cec_vol(g_volume_infor.volume_current));
#endif
    }

#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment != 0)
    {
        com_led_force_exit_msg_show();
        com_volume_value_display(g_volume_infor.volume_current);
        led_enter_msg_show(3000);
    }
#endif

    if (g_volume_infor.volume_current >= g_volume_infor.volume_current_max)
    {
        _volume_sound_tips();
    }
}

void com_volume_set_inner(uint8 mode, uint8 set_vol)
{
    if (set_vol <= g_volume_infor.volume_current_max)
    {
        g_volume_infor.volume_current = set_vol;
        libc_printf_info("volume:%d\n", g_volume_infor.volume_current);
        com_set_sound_volume_inner(mode, g_volume_infor.volume_current);

#if (SUPPORT_LED_DISPLAY != 0)
        if (g_com_comval.support_led_segment != 0)
        {
            com_led_force_exit_msg_show();
            com_volume_value_display(g_volume_infor.volume_current);
            led_enter_msg_show(3000);
        }
#endif

        if (g_volume_infor.volume_current >= g_volume_infor.volume_current_max)
        {
            _volume_sound_tips();
        }

        if (g_volume_infor.volume_current == 0)
        {
            _volume_sound_tips();
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    启动或关闭静音。
 * \param[in]    mute TRUE表示静音，FALSE表示释放静音
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 设置静音成功。
 * \retval           FALSE 设置静音失败。
 * \ingroup      common_func
 * \note
 *******************************************************************************/
bool com_set_mute_inner(bool mute)
{
    if (mute == TRUE)
    {
        if (get_mute_enable() == FALSE)
        {
            //设置音量为0，并置为静音状态
            uint8 volume_backup = com_get_sound_volume_inner();
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, 0);
            set_mute_enable(TRUE);

            libc_printf_info("mute\n");

#if (SUPPORT_HDMI_MODULE == 1)
            if (com_hdmi_is_hdmi_func(get_cur_func_id()) == TRUE)
            {
                hdmi_set_mute(TRUE);
                set_cec_mute_enable(TRUE);
            }
#endif

            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, volume_backup);
        }
    }
    else
    {
        if (get_mute_enable() == TRUE)
        {
            libc_printf_info("release mute\n");

            //取消静音状态，并重设音量
            set_mute_enable(FALSE);

#if (SUPPORT_HDMI_MODULE == 1)
            if (com_hdmi_is_hdmi_func(get_cur_func_id()) == TRUE)
            {
                hdmi_set_mute(FALSE);
                set_cec_mute_enable(FALSE);
            }
#endif

            com_reset_sound_volume_inner(SET_VOL_BY_SYSTEM);
        }
    }

    return TRUE;
}

//切换mute与release mute状态
bool com_set_mute_toggle_inner(void)
{
    if (get_mute_enable() == TRUE)
    {
        com_set_mute_inner(FALSE);
    }
    else
    {
        com_set_mute_inner(TRUE);
    }

    return TRUE;
}

#if (SUPPORT_HDMI_MODULE == 1)
bool com_hdmi_cec_mute(bool flag, bool mute)
{
    libc_printf_info("mute %s,%d\n", (mute ? "on":"off"), flag);

    if(flag == TRUE)
    {
        com_set_mute_inner(mute);
    }
    set_cec_mute_enable(mute);

    return TRUE;
}
#endif

//设置音量限制，在电池电量发生变化时进行更新
void com_set_volume_limit_inner(int16 vol_limit)
{
    g_volume_infor.phy_limit_current = vol_limit;

    if (g_volume_infor.phy_limit_current < 0)
    {
        libc_printf_info("set phy volume limit:%d\n", g_volume_infor.phy_limit_current);
    }

    //重新设置物理音量
    com_set_phy_volume();
}
