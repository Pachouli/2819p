/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ�����������غͱ���ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��config.bin�ж�ȡĬ������
 * \param[in]    setting_comval ����������Ϣ�ṹ��ָ��
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 *******************************************************************************/
void com_get_default_setting(comval_t *setting_comval)
{
    //Ӧ������
    setting_comval->support_cardplay = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_CARDPLAY);
    setting_comval->support_linein = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_LINEIN);
    setting_comval->support_fmradio = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_FMRADIO);
    setting_comval->support_bt_inback = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_BT_INBACK);
    setting_comval->support_btplay = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_BTPLAY);
    setting_comval->support_btcall = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_BTCALL);
#if (SUPPORT_PHONE_CHARGING == 1)
    setting_comval->support_iphonecharge = (uint8) com_get_config_default_inner(SETTING_HARDWARE_SUPPORT_IPHONE_CHARGE);
#else
    setting_comval->support_iphonecharge = 0;
#endif
    if (setting_comval->support_iphonecharge == 1)
    {
        setting_comval->support_usbbox = 0;
        setting_comval->support_usbmsc = 0;
    }
    else
    {
        setting_comval->support_usbbox = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_USBBOX);
        setting_comval->support_usbmsc = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_USBMSC);
        //ǿ��ʹ�ܶ�����ģʽ
        if (g_p_esd_bk_info->force_enable_udisk == 1)
        {
        	setting_comval->support_usbmsc = 1;
        }
    }
    setting_comval->support_uhostplay = (uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_UHOSTPLAY);
#if (SUPPORT_CARD_HARDWARE == 1)
    setting_comval->hard_support_card = (uint8) com_get_config_default_inner(SETTING_HARDWARE_SUPPORT_CARD);
#else
    setting_comval->hard_support_card = 0;
#endif

#if (SUPPORT_LED_LIGHT != 0)
    setting_comval->support_led_light = (uint8) com_get_config_default_inner(SETTING_DISPLAY_SUPPORT_LED);
#else
    setting_comval->support_led_light = 0;
#endif

#if (SUPPORT_LED_DISPLAY != 0)
    setting_comval->support_led_segment = (uint8) com_get_config_default_inner(SETTING_DISPLAY_SUPPORT_LED_SEGMENT);
#else
    setting_comval->support_led_segment = 0;
#endif

#if (SUPPORT_IR_KEY != 0)
    setting_comval->support_ir_key = (uint8) com_get_config_default_inner(SETTING_IR_KEY_ENABLE);
#else
    setting_comval->support_ir_key = 0;
#endif

#if (SUPPORT_EXTERN_PA != 0)
    setting_comval->support_extern_pa = 1;
#else
    setting_comval->support_extern_pa = 0;
#endif

#if (SUPPORT_LINEIN_DETECT != DETECT_NO_SUPPORT)
    setting_comval->support_aux_detect = 1;
#else
    setting_comval->support_aux_detect = 0;
#endif

#if (SUPPORT_DC5V_DETECT != DETECT_NO_SUPPORT)
    setting_comval->support_dc5v_detect = 1;
#else
    setting_comval->support_dc5v_detect = 0;
#endif

    setting_comval->is_udev_det_base_dc5v_det = 0;
#if (UDEV_DET_BASE_DC5V_DET == 1)
    if ((setting_comval->support_dc5v_detect == 1) && (setting_comval->support_uhostplay == 1))
    {
        setting_comval->is_udev_det_base_dc5v_det = 1;
    }
#endif

#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_LRADC)
    setting_comval->support_bat_lradc = 1;
#else
    setting_comval->support_bat_lradc = 0;
#endif
}

void com_get_default_env_var(env_var_t *p_env_var)
{
    //��ǰ����
    g_com_env_var.main_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_DEFAULT_MAIN_VOLUME);
    g_com_env_var.call_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_DEFAULT_CALL_VOLUME);

    //CASE ���Զ��� Graphics EQ ����
    g_com_env_var.eq_enable = (uint8) com_get_config_default_inner(DAE_GRAPHICS_EQ_ENABLE);
    g_com_env_var.eq_type = (uint8) com_get_config_default_inner(DAE_GRAPHICS_EQ_TYPE);

    g_com_env_var.dev_role = NORMAL_DEV;
    g_com_env_var.spk_pos = TWS_SPK_STEREO;

#if (SUPPORT_FM_TX == 1)
    g_com_env_var.fm_tx_freq = 875;
#endif

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    g_com_env_var.contrast = 10;
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���� sys_comval ָ�룬�Ա� common ����ϵͳ�����
 * \param[in]    comval ȫ��ϵͳ��������ָ��
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ǰ̨Ӧ���ڽ���ʱ������øýӿڣ����� common ģ�齫�޷��������С�
 *******************************************************************************/
void com_load_sys_comval(void)
{
    //��ȡϵͳ����������Ϣ
    com_get_default_setting(&g_com_comval);

    //��ȡcommon VM����������Ϣ
    if (sys_vm_read(&g_com_env_var, VM_AP_ENV_VAR, sizeof(env_var_t)) < 0)
    {
        g_com_env_var.magic = 0xffff;
    }
    //��ʼ��ϵͳ��VM����
    if (g_com_env_var.magic != VRAM_MAGIC(VM_AP_ENV_VAR))
    {
        g_com_env_var.magic = VRAM_MAGIC(VM_AP_ENV_VAR);
        com_get_default_env_var(&g_com_env_var);
        sys_vm_write(&g_com_env_var, VM_AP_ENV_VAR, sizeof(env_var_t));
    }
}

void com_save_sys_comval_inner(void)
{
    g_com_env_var.main_volume = g_volume_infor.main_volume;
    g_com_env_var.call_volume = g_volume_infor.call_volume;

    sys_vm_write(&g_com_env_var, VM_AP_ENV_VAR, sizeof(env_var_t));
}

void com_restore_sys_comval_by_reset_inner(void)
{
    //�ָ�������EQ���͵�
    com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_p_esd_bk_info->volume_value);
    com_set_sound_volume_mode_inner(VOLUME_MODE_CALL, VOLUME_GAIN_CALL);
    com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_p_esd_bk_info->volume_value_call);
    com_set_sound_volume_mode_inner(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);
}

/*! \endcond */
