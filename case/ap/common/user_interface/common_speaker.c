/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：外部功放和喇叭开关接口，支持临时MUTE外部功放。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

//返回设置前状态
bool is_speaker_on_inner(void)
{
#if (SUPPORT_EXTERN_PA == 1)
    if (g_com_comval.support_extern_pa == FALSE)
    {
        return TRUE;
    }

    return ((g_volume_infor.pa_main_on == TRUE) || (g_volume_infor.pa_sub_on_count > 0));
#else
    return TRUE;
#endif
}

//执行AB-D类功放选择
void com_switch_ab_d_do(void)
{
    if (g_volume_infor.pa_ab_d_flag == PA_SWITCH_D)
    {
#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
        set_pad_function(EXTERN_PA_AB_D_PIN, EXTERN_PA_AB_D_FUNC, NULL, (void *) (!(EXTERN_PA_AB_D_AB_LEVEL)));
#else
        set_pad_function(EXTERN_PA_AB_D_PIN, EXTERN_PA_AB_D_FUNC, NULL, (void *) (!(EXTERN_PA_AB_D_AB_LEVEL)));
#endif
    }
    else
    {
#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
        set_pad_function(EXTERN_PA_AB_D_PIN, EXTERN_PA_AB_D_FUNC, NULL, (void *) (EXTERN_PA_AB_D_AB_LEVEL));
#else
        set_pad_function(EXTERN_PA_AB_D_PIN, EXTERN_PA_AB_D_FUNC, NULL, (void *) (EXTERN_PA_AB_D_AB_LEVEL));
#endif
    }
}

//执行使能喇叭
void com_speaker_on_do(void)
{
#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
    set_pad_function(EXTERN_PA_ENABLE_PIN, EXTERN_PA_ENABLE_FUNC, NULL, (void *) EXTERN_PA_ENABLE_LEVEL);

    sys_os_time_dly(EXTERN_PA_ENABLE_DELAY/10);
#else
    set_pad_function(EXTERN_PA_ENABLE_PIN, EXTERN_PA_ENABLE_FUNC, NULL, (void *) EXTERN_PA_ENABLE_LEVEL);

#if (EXTERN_PA_AB_D_ENABLE == 1)
    com_switch_ab_d_do();
#endif

    sys_os_time_dly(EXTERN_PA_ENABLE_DELAY/10);
#endif
}

//执行关闭喇叭
void com_speaker_off_do(void)
{
#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
    set_pad_function(EXTERN_PA_ENABLE_PIN, EXTERN_PA_ENABLE_FUNC, NULL, (void *) (!(EXTERN_PA_ENABLE_LEVEL)));
#else
    set_pad_function(EXTERN_PA_ENABLE_PIN, EXTERN_PA_ENABLE_FUNC, NULL, (void *) (!(EXTERN_PA_ENABLE_LEVEL)));
    set_pad_function(EXTERN_PA_AB_D_PIN, EXTERN_PA_AB_D_FUNC, NULL, EXTERN_PA_AB_D_DISABLE_LEVEL);
#endif
}

#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
#if (USE_DCDC_CHIP == 1)
//使能DCDC升压
void com_speaker_en_dcdc_do(void)
{
    uint32 dcdc_en_timer = sys_get_ab_timer();
    bool pwm_en = FALSE;

    sys_os_sched_lock();

    while (1)
    {
        if (pwm_en == FALSE)
        {
            set_pad_function(DCDC_CHIP_ENABLE_PIN, DCDC_CHIP_ENABLE_FUNC, NULL, (void *) DCDC_CHIP_ENABLE_LEVEL);
            pwm_en = TRUE;
            sys_udelay(500);
        }
        else
        {
            set_pad_function(DCDC_CHIP_ENABLE_PIN, DCDC_CHIP_ENABLE_FUNC, NULL, (void *) (!DCDC_CHIP_ENABLE_LEVEL));
            pwm_en = FALSE;
            sys_udelay(200);
        }

        if ((sys_get_ab_timer() - dcdc_en_timer) > DCDC_CHIP_ENABLE_DELAY)
        {
            break;
        }
    }

    set_pad_function(DCDC_CHIP_ENABLE_PIN, DCDC_CHIP_ENABLE_FUNC, NULL, (void *) DCDC_CHIP_ENABLE_LEVEL);

    sys_os_sched_unlock();
}

//关闭DCDC升压
void com_speaker_dis_dcdc_do(void)
{
    set_pad_function(DCDC_CHIP_ENABLE_PIN, DCDC_CHIP_ENABLE_FUNC, NULL, (void *) (!(DCDC_CHIP_ENABLE_LEVEL)));
}
#endif
#endif

//打开外部PA+喇叭
void com_speaker_on_inner(uint8 mode)
{
#if (SUPPORT_EXTERN_PA == 1)
    if (g_com_comval.support_extern_pa == FALSE)
    {
        return;
    }

    if ((mode & SPK_EN_DCDC) != 0)
    {
#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
#if (USE_DCDC_CHIP == 1)
        //使能DC升压使用软件PWM需要锁掉度，最好放在开机过程中进行
        if (g_volume_infor.dcdc_enable == FALSE)
        {
            com_speaker_en_dcdc_do();
            g_volume_infor.dcdc_enable = TRUE;
        }
#endif
#endif
        return;
    }

    if ((g_volume_infor.pa_main_on == FALSE) && (g_volume_infor.pa_sub_on_count == 0))
    {
        libc_printf_info("pa on\n");

#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
#if (USE_DCDC_CHIP == 1)
        //使能DC升压使用软件PWM需要锁掉度，最好放在开机过程中进行
        if (g_volume_infor.dcdc_enable == FALSE)
        {
            com_speaker_en_dcdc_do();
            g_volume_infor.dcdc_enable = TRUE;
        }
#endif
#endif

        com_speaker_on_do();
    }

    if ((mode & SPK_MODE_OPTION) == SPK_PLAY)
    {
        libc_printf_info("SPK_PLAY on\n");
        g_volume_infor.pa_main_on = TRUE;
    }
    else
    {
        libc_printf_info("SPK_SYS on\n");
        g_volume_infor.pa_sub_on_count++;
    }
#endif
}

//关闭外部PA+喇叭
void com_speaker_off_inner(uint8 mode)
{
#if (SUPPORT_EXTERN_PA == 1)
    bool need_off = FALSE;

    if (g_com_comval.support_extern_pa == FALSE)
    {
        return;
    }

    if ((mode & SPK_MODE_OPTION) == SPK_PLAY)
    {
        libc_printf_info("SPK_PLAY off\n");
        if (g_volume_infor.pa_main_on == TRUE)
        {
            g_volume_infor.pa_main_on = FALSE;
        }

        if (g_volume_infor.pa_sub_on_count == 0)
        {
            need_off = TRUE;
        }
    }
    else
    {
        libc_printf_info("SPK_SYS off\n");
        if (g_volume_infor.pa_sub_on_count > 0)
        {
            g_volume_infor.pa_sub_on_count--;
        }

        if ((g_volume_infor.pa_main_on == FALSE) && (g_volume_infor.pa_sub_on_count == 0))
        {
            need_off = TRUE;
        }
    }

    if (need_off == TRUE)
    {
        com_speaker_off_do();

#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
#if (USE_DCDC_CHIP == 1)
        if ((g_volume_infor.dcdc_enable == TRUE) && ((mode & SPK_POWER_OFF) != 0))
        {
            com_speaker_dis_dcdc_do();
            g_volume_infor.dcdc_enable = FALSE;
        }
#endif
#endif

        sys_os_time_dly(1);

        libc_printf_info("pa off\n");
    }
#endif
}

//外部PA AB/D类功放选择
void com_speaker_ab_d_switch_inner(uint8 ab_d)
{
#if (SUPPORT_EXTERN_PA == 1)
#if (EXTERN_PA_AB_D_ENABLE == 1)
    if (g_com_comval.support_extern_pa == FALSE)
    {
        return;
    }

    g_volume_infor.pa_ab_d_flag = ab_d;

    if ((g_volume_infor.pa_main_on == FALSE) && (g_volume_infor.pa_sub_on_count == 0))
    {
        return;
    }

    com_switch_ab_d_do();

#endif
#endif
}
