/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Ӧ�ü�⿪��ѡ�ȷ��������һ��Ӧ�á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "ap_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  �ػ�����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void config_power_off(void)
{
    wakeup_en_para_t temp_wakeup_en_para;
    uint8 release_cnt;

    libc_printf_info("POWER OFF START @%d\n", sys_get_ab_timer());

    com_speaker_off(SPK_SYS|SPK_POWER_OFF);

    disable_pa();

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);

    //���ESD RESET��ʶ
    g_p_esd_bk_info->flag = 0;

    libc_printf_info("POWER OFF END @%d\n", sys_get_ab_timer());
    sys_mdelay(10); //delay 10ms�Ա��ܹ���ӡ����

    //�ϵ�ػ������ٷ���
    libc_memset(&temp_wakeup_en_para, 0x00, sizeof(temp_wakeup_en_para));
    temp_wakeup_en_para.onoff_short_en = 1;
    temp_wakeup_en_para.onoff_long_en = 1;
    temp_wakeup_en_para.onoff_long_time = ONOFF_KEY_LPT_2S;
#if (SUPPORT_WIO_WAKEUP == 1)
    if (com_get_config_default(SETTING_APP_SUPPORT_CHARGE_LOOP) == 1)
    {
        temp_wakeup_en_para.wio0_en = 1;
        temp_wakeup_en_para.wio0_dir = WIO_WAKE_DIR_HIGH;
    }
#endif
#if (SUPPORT_TIME_ALARM == 1)

#if (SUPPORT_EXT_RTC == 0)
    temp_wakeup_en_para.alarm_en = 1;
#else
    if (EXT_RTC_INT_PIN == PAD_WIO0)
    {
        temp_wakeup_en_para.wio0_en = 1;
        temp_wakeup_en_para.wio0_dir = WIO_WAKE_DIR_HIGH;
    }
    else
    {
        temp_wakeup_en_para.wio1_en = 1;
        temp_wakeup_en_para.wio1_dir = WIO_WAKE_DIR_HIGH;
    }
#endif

#endif

    //���û��ѷ�ʽ�Ͳ���
    pmu_set_wake_up(&temp_wakeup_en_para);

    release_cnt = 0;
    while (1)
    {
        if (hal_pmu_read_onoff_key() == FALSE)
        {
            release_cnt++;
            if (release_cnt >= 3)
            {
                break;
            }
        }
        else
        {
            release_cnt = 0;
            libc_printf("wait onoff hit release\n");
        }

        sys_os_time_dly(1);
    }

    sys_mdelay(10); //Ϊ��DMA UART��ӡ�ܹ���ȫ��ӡ��������ʱ

    //�ϵ�
    pmu_power_off();
}
