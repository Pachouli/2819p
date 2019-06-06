/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开机/关机应用入口，开机流程，关机流程；要考虑ESD重启的情况。
 * 作者：cailizhen
 ********************************************************************************/

#include "ap_config.h"

const key_map_t key_config_list[] =
{
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_MODE_SWITCH },
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_MODE_SWITCH },

    /*! 结束标志 */
    { { KEY_NULL_NO_SHORTCUT, 0, KEY_TYPE_NULL }, MSG_NULL },
};

app_result_e config_charge_loop(uint8 mode)
{
    input_gui_msg_t gui_msg;
    private_msg_t pri_msg;
    msg_apps_type_e cur_event;
    app_result_e result = RESULT_NULL;
    uint32 usb_cable_plugout_count = 0;

    libc_printf_info("Enter charge loop : %d\n", mode);

#if (SUPPORT_HDMI_MODULE == 1)
    sys_adjust_clk(FREQ_0M, SET_LOCK_CLK);
    hdmi_power_config(FALSE);
#endif

    com_speaker_off(SPK_SYS|SPK_POWER_OFF);

    //关闭TTS播报和按键音
    com_tts_state_play_control(TTS_CTRL_IGNORE);
    keytone_set_on_off (FALSE);
    change_app_state (APP_STATE_PLAYING);

    sys_adjust_clk(SYS_CLK_ON_IDLE, SET_SYS_CLK);

    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'C', TRUE);
    led_display(NUMBER2, 'H', TRUE);
    led_display(NUMBER3, 'A', TRUE);
    led_display(NUMBER4, 'R', TRUE);

#if (SUPPORT_FAST_PREVIEW == 1)
    key_set_fast_preview_cbk(NULL);
#endif

    while(1)
    {
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &cur_event, key_config_list) == TRUE)
            {
                switch (cur_event)
                {
                    case EVENT_MODE_SWITCH:
                    goto poweron_here;

                    default:
                    break;
                }
            }
        }
        else
        {
            if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
            {
                switch(pri_msg.msg.type)
                {
#if (SUPPORT_HDMI_MODULE == 1)
                    case MSG_HDMI_POWER_ON:
                    libc_printf_info("MSG_HDMI_POWER_ON\n");
                    result = RESULT_LASTPLAY;
                    goto poweron_here;

                    case MSG_HDMI_POWER_HD1:
                    result = RESULT_ENTER_HDMI;
                    break;

                    case MSG_HDMI_POWER_HD2:
                    result = RESULT_ENTER_HDMI1;
                    break;

                    case MSG_HDMI_POWER_HD3:
                    result = RESULT_ENTER_HDMI2;
                    break;

                    case MSG_HDMI_POWER_ARC:
                    result = RESULT_ENTER_HDMI_ARC;
                    break;
#endif

                    case MSG_LOW_POWER:
                    result = RESULT_LOW_POWER;
                    break;

                    case MSG_USB_STICK:
                    if (mode == CHARGE_MODE_ON)
                    {
                        result = RESULT_USB_TRANS;
                    }
                    break;

                    case MSG_FULL_CHARGE:
                    //在这里添加充电指示灯控制
                    break;

                    case MSG_RTCALARM:
#if (SUPPORT_TIME_ALARM == 1)
                    result = RESULT_ENTER_ALARM;
#endif
                    break;

                    default:
                    break;
                }
            }
        }

        if ((get_cable_state() == CABLE_STATE_CABLE_NULL) && (get_adaptor_state() == ADAPTOR_STATE_CABLE_NULL))
        {
            usb_cable_plugout_count++;
            if (usb_cable_plugout_count > 20)
            {
                result = RESULT_POWER_OFF;
            }
        }
        else
        {
            usb_cable_plugout_count = 0;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        sys_os_time_dly(1);
    }

    poweron_here:

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    led_clear_screen(CLR_ALL);

    sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);

    change_app_state (APP_STATE_NO_PLAY);
    //恢复TTS播报和按键音
    com_tts_state_play_control(TTS_CTRL_CANCEL_IGNORE);
    keytone_set_on_off(TRUE);

    if (com_get_config_default(SETTING_HARDWARE_EXTERN_PA_MUTE_ENABLE) == FALSE)
    {
        com_speaker_on(SPK_SYS);
    }

    libc_printf_info("charge loop quit : 0x%x\n", result);

    return result;
}

