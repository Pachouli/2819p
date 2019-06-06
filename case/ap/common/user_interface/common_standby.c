/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ����S2�͹���ģʽ�����Խ����Ƶģʽ���У���������������ӣ��������ڼ��
 *       ���ѵ���Ƶ������Ƿ���������ѣ��ù�����δ��ȫ��ͨ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"
#include "hal_pmu.h"

extern int enter_s2_func(uint32 low_time_ms)__FAR__;

static void s2_open_pa(void);

#define LOW_FREQ_INTERVAL_TIME 170 //��Ƶ���ʱ�䣬ÿ���һ��ʱ����лظ�Ƶ��⣬0��ʾһֱ�ڵ�Ƶ�������лظ�Ƶ
#define HIGH_FREQ_CHECK_TIME   10  //��Ƶ���ʱ��
static int high_freq_check(void);


/* �κ�Ӧ�ÿ���ʱ�����Խ���S2����ģʽ��ǰ̨���������������״̬��Ȼ�����͹���ģʽ��
 * ���У������м����DSP���Ѿ�ж�ص��������Ѿ�����8����û�����ݹ�����ǰ̨���ڿ���״̬
 *       ���Ѿ�����7����û�з����κ��¼�
 */
app_result_e com_standby(void)
{
    msg_apps_t msg;
    app_result_e result = RESULT_NULL;
    int ret_val = 0;
    os_event_t *standby_sem;
    uint32 irq_save;
    bool wait_key_flag = FALSE;
    bool wait_msg_flag = FALSE;
    uint8 print_mode;

#if (SUPPORT_UART_PRINT == 1)
    print_mode = (uint8) com_get_config_default_inner(SETTING_UART_PRINT_ENABLE);
#else
    print_mode = UART_PRINT_MODE_DISABLE;
#endif

    //step 1:����Ӧ�û��������������BT ENGINE����˯��״̬
    com_speaker_off_inner(SPK_SYS|SPK_POWER_OFF);

    //�ص�LED�����
#if (SUPPORT_LED_DISPLAY == 1)
    led_sleep_display();
#endif

#if (SUPPORT_IR_KEY == 1)
    if ((g_com_comval.support_ir_key == 1)
        && (com_get_config_default_inner(SETTING_ONOFF_POWEROFF_IR_WAKEUP) == 1))
    {
        //TODO ��Ҫ�л�Ϊ���IR��ⷽʽ
    }
#endif

    libc_printf_info("enter s2 mode\n");

    while (1)
    {
        if (libc_sem_init(&standby_sem, 0) == -1)
        {
            libc_printf("standby_sem\n");
            while (1);
        }

        msg.type = MSG_SYSTEM_ENTER_S2_SYNC;
        msg.content.addr = standby_sem;

        if (get_engine_type_inner() != ENGINE_NULL)
        {
            send_sync_msg_inner(APP_ID_THEENGINE, &msg, NULL, 0);
        }

        if (get_bt_state_inner() == BT_STATE_WORK)
        {
            send_sync_msg_inner(APP_ID_BTENGINE, &msg, NULL, 0);
        }
        com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
        irq_save = sys_local_irq_save();
        sys_drv_load_text1("common.drv");
        sys_drv_load_text1("hal.drv");

        if (print_mode != UART_PRINT_MODE_DISABLE)
        {
            sys_mdelay(2); //����Ϊ�˴�ӡ�ܹ�������ӡ��������2ms��ʱ
        }

        //����͹��Ĳ�ʵʱ��⻽�ѣ����Ѻ󷵻�
        ret_val = enter_s2_func(LOW_FREQ_INTERVAL_TIME);

        libc_printf("wake ret:%d\n", ret_val);

        sys_local_irq_restore(irq_save);

        //step 5:�������BT ENGINE�˳�˯��ģʽ���ָ�Ӧ�û���
        if (get_bt_state_inner() == BT_STATE_WORK)
        {
            libc_sem_post(standby_sem);
        }
        if (get_engine_type_inner() != ENGINE_NULL)
        {
            libc_sem_post(standby_sem);
        }
        libc_sem_destroy(&standby_sem);

        wake_ret_deal:

        if (ret_val == WAKE_BY_BT)
        {
            while (1)
            {
                com_get_btengine_status_inner(NULL);

                if (bt_info.bt_in_idle == FALSE)
                {
                    break;
                }
                else if (bt_info.low_power_mode != 0)
                {
                    break;
                }

                ret_val = high_freq_check();
                if (ret_val != 0)
                {
                    goto wake_ret_deal;
                }
            }

            if (bt_info.bt_in_idle == FALSE)
            {
                result = RESULT_NULL;
                break;
            }
        }
        else if (ret_val == WAKE_BY_LOWPOWER)
        {
            result = RESULT_JUST_POWER_OFF;
            break;
        }
        else if (ret_val == WAKE_BY_KEY)
        {
            wait_key_flag = TRUE;
            result = RESULT_NULL;
            break;
        }
        else if (ret_val == WAKE_BY_USBDC5V)
        {
            wait_msg_flag = TRUE;
            if (com_get_config_default_inner(SETTING_APP_SUPPORT_CHARGE_LOOP) == 1)
            {
                result = RESULT_ENTER_CHARGE_LOOP;
            }
            break;
        }
        else if (ret_val == WAKE_BY_RTCALARM)
        {
            result = RESULT_NULL;
            break;
        }
        else if (ret_val == WAKE_BY_HIGHFREQ)
        {
            uint8 i;

            for (i = 0; i < HIGH_FREQ_CHECK_TIME; i++)
            {
                ret_val = high_freq_check();
                if (ret_val != 0)
                {
                    goto wake_ret_deal;
                }
                sys_mdelay(1);
            }
        }
        else
        {
            break;
        }
    }

    hal_set_usb_detect(TRUE);

    standby_restart_all_app_timer_inner(APP_TIMER_GROUP_GUI);
    standby_restart_all_app_timer_inner(APP_TIMER_GROUP_COMMON);

    sys_counter_reset();

    //����LED�����
#if (SUPPORT_LED_DISPLAY == 1)
    led_wake_up_display();
#endif

    //��鰴��������
    if (wait_key_flag == TRUE)
    {
        input_gui_msg_t gui_msg;
        uint32 filter_key_timer = sys_get_ab_timer();
        while (1)
        {
            if (get_gui_msg(&gui_msg) == TRUE)
            {
                com_filter_key_hold();
                break;
            }
            if ((sys_get_ab_timer() - filter_key_timer) > 200)
            {
                break;
            }
        }
    }

    s2_open_pa();

    if (com_get_config_default_inner(SETTING_HARDWARE_EXTERN_PA_MUTE_ENABLE) == FALSE)
    {
        com_speaker_on_inner(SPK_SYS);
    }

    //���ϵͳ��Ϣ
    if (wait_msg_flag == TRUE)
    {
        private_msg_t private_msg;
        uint32 wait_msg_timer = sys_get_ab_timer();

        while (1)
        {
            if (get_app_msg(APP_TYPE_GUI, &private_msg) == TRUE)
            {
                libc_printf("standby msg:0x%x\n", private_msg.msg.type);
#if (SUPPORT_TIME_ALARM == 1)
                if (private_msg.msg.type == MSG_RTCALARM)
                {
                    result = RESULT_ENTER_ALARM;
                    break;
                }
#endif
            }

            if ((sys_get_ab_timer() - wait_msg_timer) > 1000)
            {
                break;
            }

            if ((get_cable_state() == CABLE_STATE_CABLE_IN)
                || (get_adaptor_state() == ADAPTOR_STATE_CABLE_IN))
            {
                libc_printf("wait msg quit @ %d\n", sys_get_ab_timer() - wait_msg_timer);
                break;
            }

            sys_os_time_dly(2);
        }
    }

    libc_printf_info("exit s2 mode\n");

    return result;
}

static void s2_open_pa(void)
{
    pa_setting_t pa_sett;
    aud_output_channel_e aud_output_chan;

    //��PA��PA�����󳣿�
    pa_sett.pa_swing = (pa_swing_mode_e) com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_PA_SWING);
    aud_output_chan = (aud_output_channel_e) com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);

    pa_sett.direct_drive_enable = com_get_config_default(SETTING_HARDWARE_EARPHONE_DDV_ENABLE);
    pa_sett.pal_output = TRUE;
    pa_sett.par_output = TRUE;
    if ((aud_output_chan == AUD_OUTPUT_MIX_LEFT_CHAN) || (aud_output_chan == AUD_OUTPUT_LEFT_CHAN))
    {
        pa_sett.par_output = FALSE;
    }
    if ((aud_output_chan == AUD_OUTPUT_MIX_RIGHT_CHAN) || (aud_output_chan == AUD_OUTPUT_RIGHT_CHAN))
    {
        pa_sett.pal_output = FALSE;
    }

    //�Ƿ�ʹ�ö��������ʹ�ö����������PAʱ��Ҫ��PA����ʱ�ϳ����ر�PAʱ��Ҫ��PA�ŵ��ʱҲ�ϳ�
    pa_sett.antipop = (bool) com_get_config_default(SETTING_HARDWARE_EARPHONE_ENABLE);

    enable_pa(&pa_sett);

    com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_volume_infor.volume_current);
}

static int high_freq_check(void)
{
    int wake_ret = 0;
    private_msg_t app_msg;

#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_BATADC)
    //�͵��˳��͹���ģʽ
    if (hal_pmu_read_batadc() < BAT_ADC_LOW_POWER)
    {
        wake_ret = WAKE_BY_LOWPOWER;
    }
#else
    //�͵��˳��͹���ģʽ
    if (hal_pmu_read_lradc(PAD_KEY_LRADCX_IDX) < LRADC_LOW_POWER)
    {
        wake_ret = WAKE_BY_LOWPOWER;
    }
#endif

    //����������
    if (hal_pmu_read_lradc(1) < LINEIN_ADCKEY_DET_ADC_VAL_MIN)
    {
        wake_ret = WAKE_BY_KEY;
    }

    //����ң�ػ���
#if (SUPPORT_IR_KEY == 1)
    if ((g_com_comval.support_ir_key == 1)
        && (com_get_config_default_inner(SETTING_ONOFF_POWEROFF_IR_WAKEUP) == 1))
    {
        //TODO ʹ�����IR��ⷽʽ�����ж��Ƿ�ǰprotocol��IR�ź�
    }
#endif

    //����Ƿ���ϵͳ��Ϣ����ϵͳ��ϢҲҪ�˳��������������졢USB�����߲��롢USB��������߲����
    if (get_hotplug_msg(&app_msg) == TRUE)
    {
        message_redeal_app_msg_inner(&app_msg); //���ݻ�����Ϣ
        wake_ret = WAKE_BY_RTCALARM;
    }

    return wake_ret;
}
