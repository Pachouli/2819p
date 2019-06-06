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

#define ONE_CYCLE_INST_COUNT  240 //��ѭ����תָ����

int __section__(".TEXT.S2") enter_s2_func(uint32 low_time_ms)
{
    int wake_ret = 0;
    int low_cycles = 32 * low_time_ms / ONE_CYCLE_INST_COUNT;

    low_power_reg_bak_t low_power_reg_bak_var;
    //step 2:����͹�������ģʽ
    s2_pmu_enter_low_power_mode(&low_power_reg_bak_var);

    while (1)
    {
        //��������
        if (s2_intc_is_irq_pending(IRQ_BT_BASEBAND) == TRUE)
        {
            wake_ret = WAKE_BY_BT;
            break;
        }

        //���ӻ���
        if (s2_intc_is_irq_pending(IRQ_RTC) == TRUE)
        {
            wake_ret = WAKE_BY_RTCALARM;
            break;
        }

        //onoff key����
        if (hal_pmu_read_onoff_key() != 0)
        {
            wake_ret = WAKE_BY_KEY;
            break;
        }

        //��������
        if (s2_intc_is_irq_pending(IRQ_BT_BASEBAND) == TRUE)
        {
            wake_ret = WAKE_BY_BT;
            break;
        }

        //USB DC5V ���
#if (SUPPORT_WIO_WAKEUP == 1)
        {
            uint8 wio_level = !DC5V_DETECT_LEVEL;

            if (DC5V_DETECT_PIN == PAD_WIO0)
            {
                wio_level = hal_gpio_read_wio(0);
            }
            else if (DC5V_DETECT_PIN == PAD_WIO1)
            {
                wio_level = hal_gpio_read_wio(1);
            }

            if (wio_level == DC5V_DETECT_LEVEL)
            {
                wake_ret = WAKE_BY_USBDC5V;
                break;
            }
        }
#endif

        //��ʱ�л�����Ƶ��������LRADC��IR��SD���Ȼ���
        if (low_time_ms > 0)
        {
            low_cycles--;
            if (low_cycles == 0)
            {
                wake_ret = WAKE_BY_HIGHFREQ;
                break;
            }
        }

        //�忴�Ź�
        clear_watch_dog();
    }

    //step 4:�˳��͹�������ģʽ
    s2_pmu_exit_low_power_mode(&low_power_reg_bak_var);

    return wake_ret;
}

