/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：分立LED灯显示接口，支持闪烁模式和呼吸灯模式。
 * 作者：cailizhen
 ********************************************************************************/

#define USE_LED_PIN_CFG

#include "common_ui_inner.h"

//LED显示管理
#if (SUPPORT_LED_LIGHT == 1)
static led_scene_info_t com_led_scene_info[LED_ID_MAX];
static const uint16 pwm_period_to_dutygrade[] = {pwm_duty_grade_2000,pwm_duty_grade_4000,pwm_duty_grade_10000,pwm_duty_grade_20000};
#endif

void led_var_init(void)
{
#if (SUPPORT_LED_LIGHT == 1)
    int i;

    for (i = 0; i < LED_ID_MAX; i++)
    {
        com_led_scene_info[i].scene_status[LED_SCENE_MAIN].led_status = LED_STATUS_NULL;
        com_led_scene_info[i].scene_status[LED_SCENE_SUB].led_status = LED_STATUS_NULL;
        com_led_scene_info[i].led_timer_id = -1;
        com_led_scene_info[i].led_sim_breath_on = FALSE;
    }
#endif
}

#if (SUPPORT_LED_LIGHT == 1)
static void _led_light_off_main(led_id_e led_id)
{
    if (com_led_scene_info[led_id].led_timer_id != -1)
    {
        sys_del_irq_timer1(com_led_scene_info[led_id].led_timer_id);
        com_led_scene_info[led_id].led_timer_id = -1;
    }

    if ((com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status == LED_STATUS_DUTY)
        || (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status == LED_STATUS_BREATH))
    {
        hal_disable_pwm(com_led_pin_cfg[led_id].pwm_idx);
    }

    set_pad_function(com_led_pin_cfg[led_id].pin, PAD_FUNC_GPIOOUT, NULL, (void *) 0);
}

void com_led_twinkle_handle_led0(void)
{
    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if (com_led_scene_info[0].led_gpio_on == TRUE)
    {
        ioctl_pad(com_led_pin_cfg[0].pin, PAD_FUNC_GPIOOUT, (void *) 0);
        com_led_scene_info[0].led_gpio_on = FALSE;
    }
    else
    {
        ioctl_pad(com_led_pin_cfg[0].pin, PAD_FUNC_GPIOOUT, (void *) 1);
        com_led_scene_info[0].led_gpio_on = TRUE;
    }
}

void com_led_twinkle_handle_led1(void)
{
    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if (com_led_scene_info[1].led_gpio_on == TRUE)
    {
        ioctl_pad(com_led_pin_cfg[1].pin, PAD_FUNC_GPIOOUT, (void *) 0);
        com_led_scene_info[1].led_gpio_on = FALSE;
    }
    else
    {
        ioctl_pad(com_led_pin_cfg[1].pin, PAD_FUNC_GPIOOUT, (void *) 1);
        com_led_scene_info[1].led_gpio_on = TRUE;
    }
}

void com_led_twinkle_handle_led2(void)
{
    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if (com_led_scene_info[2].led_gpio_on == TRUE)
    {
        ioctl_pad(com_led_pin_cfg[2].pin, PAD_FUNC_GPIOOUT, (void *) 0);
        com_led_scene_info[2].led_gpio_on = FALSE;
    }
    else
    {
        ioctl_pad(com_led_pin_cfg[2].pin, PAD_FUNC_GPIOOUT, (void *) 1);
        com_led_scene_info[2].led_gpio_on = TRUE;
    }
}
#endif

void com_led_twinkle_init_inner(led_id_e led_id, led_scene_e led_scene, pwm_period_e period, uint8 pwm_gpio_sel)
{
#if (SUPPORT_LED_LIGHT == 1)
    sim_pwm_info_t pwmInfo;
    sim_pwm_nor_info_t *pni;
    pwm_para_t pwm_para;
    bool update_flag = TRUE;

    if (g_com_comval.support_led_light == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if ((led_scene == LED_SCENE_MAIN) && (com_led_scene_info[led_id].scene_status[LED_SCENE_SUB].led_status != LED_STATUS_NULL))
    {
        update_flag = FALSE;
    }

    if ((led_scene == LED_SCENE_SUB) && (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status != LED_STATUS_NULL))
    {
        _led_light_off_main(led_id);
    }

    //如果某个PIN不支持PWM，则只能使用GPIOOUT方式点亮LED
    if (pwm_gpio_sel == TK_SEL_PWM)
    {
        //如果不支持硬件PWM
        if (com_led_pin_cfg[led_id].pwm_mfp == 0xff)
        {
            pwm_gpio_sel = TK_SEL_GPIO;
        }
    }

    if (update_flag == TRUE)
    {
        if (com_led_scene_info[led_id].led_timer_id != -1)
        {
            sys_del_irq_timer1(com_led_scene_info[led_id].led_timer_id);
            com_led_scene_info[led_id].led_timer_id = -1;
        }

        if ((com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_DUTY)
                || (com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_BREATH))
        {
            hal_disable_pwm(com_led_pin_cfg[led_id].pwm_idx);
        }
    }

    if (pwm_gpio_sel == TK_SEL_PWM)
    {
        if (update_flag == TRUE)
        {
            pwm_para.mode = PWM_MODE_NORMAL;
            pwm_para.polarity = PWM_HIGH_ACTIVE;
            pwm_para.duty = 127;
            pwm_para.period = period;
            set_pad_function(com_led_pin_cfg[led_id].pin, com_led_pin_cfg[led_id].pwm_mfp, NULL, 0);
            hal_set_pwm(com_led_pin_cfg[led_id].pwm_idx, &pwm_para);
        }
        com_led_scene_info[led_id].scene_status[led_scene].led_status = LED_STATUS_DUTY;
    }
    else
    {
        if (update_flag == TRUE)
        {
            int tn;

            if (period == PWM_PERIOD_200MS)
            {
                tn = 10;
            }
            else if (period == PWM_PERIOD_400MS)
            {
                tn = 20;
            }
            else if (period == PWM_PERIOD_1000MS)
            {
                tn = 50;
            }
            else
            {
                tn = 100;
            }

            set_pad_function(com_led_pin_cfg[led_id].pin, PAD_FUNC_GPIOOUT, NULL, (void *) 1);
            com_led_scene_info[led_id].led_gpio_on = TRUE;

            if (led_id == 0)
            {
                com_led_scene_info[led_id].led_timer_id = sys_set_irq_timer1(com_led_twinkle_handle_led0, tn);
            }
            else if (led_id == 1)
            {
                com_led_scene_info[led_id].led_timer_id = sys_set_irq_timer1(com_led_twinkle_handle_led1, tn);
            }
            else if (led_id == 2)
            {
                com_led_scene_info[led_id].led_timer_id = sys_set_irq_timer1(com_led_twinkle_handle_led2, tn);
            }
            else
            {
                libc_printf_error("please realize function : com_led_twinkle_handle_led%d\n", led_id);
                while (1);
            }
        }
        com_led_scene_info[led_id].scene_status[led_scene].led_status = LED_STATUS_DUTY_GPIO;
    }
    com_led_scene_info[led_id].scene_status[led_scene].led_para = period;
#endif
}

void com_led_light_on_inner(led_id_e led_id, led_scene_e led_scene)
{
#if (SUPPORT_LED_LIGHT == 1)
    bool update_flag = TRUE;

    if (g_com_comval.support_led_light == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if ((led_scene == LED_SCENE_MAIN) && (com_led_scene_info[led_id].scene_status[LED_SCENE_SUB].led_status != LED_STATUS_NULL))
    {
        update_flag = FALSE;
    }

    if ((led_scene == LED_SCENE_SUB) && (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status != LED_STATUS_NULL))
    {
        _led_light_off_main(led_id);
    }

    if (update_flag == TRUE)
    {
        if (com_led_scene_info[led_id].led_timer_id != -1)
        {
            sys_del_irq_timer1(com_led_scene_info[led_id].led_timer_id);
            com_led_scene_info[led_id].led_timer_id = -1;
        }

        if ((com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_DUTY)
                || (com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_BREATH))
        {
            hal_disable_pwm(com_led_pin_cfg[led_id].pwm_idx);
        }

        set_pad_function(com_led_pin_cfg[led_id].pin, PAD_FUNC_GPIOOUT, NULL, (void *) 1);
    }

    com_led_scene_info[led_id].scene_status[led_scene].led_status = LED_STATUS_ON;
#endif
}

void com_led_light_off_inner(led_id_e led_id, led_scene_e led_scene)
{
#if (SUPPORT_LED_LIGHT == 1)
    bool update_flag = TRUE;
    bool led_off_flag = FALSE;

    if (g_com_comval.support_led_light == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if ((led_scene == LED_SCENE_MAIN) && (com_led_scene_info[led_id].scene_status[LED_SCENE_SUB].led_status != LED_STATUS_NULL))
    {
        update_flag = FALSE;
    }

    if (update_flag == TRUE)
    {
        if (com_led_scene_info[led_id].led_timer_id != -1)
        {
            sys_del_irq_timer1(com_led_scene_info[led_id].led_timer_id);
            com_led_scene_info[led_id].led_timer_id = -1;
        }

        if ((com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_DUTY)
            || (com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_BREATH))
        {
            hal_disable_pwm(com_led_pin_cfg[led_id].pwm_idx);
        }

        led_off_flag = TRUE;
    }

    com_led_scene_info[led_id].scene_status[led_scene].led_status = LED_STATUS_NULL;

    if ((led_scene == LED_SCENE_SUB) && (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status != LED_STATUS_NULL))
    {
        led_off_flag = FALSE;
        switch (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status)
        {
        case LED_STATUS_ON:
            com_led_light_on_inner(led_id, LED_SCENE_MAIN);
            break;

        case LED_STATUS_DUTY:
            com_led_twinkle_init_inner(led_id, LED_SCENE_MAIN, com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_para, TK_SEL_PWM);
            break;

        case LED_STATUS_DUTY_GPIO:
            com_led_twinkle_init_inner(led_id, LED_SCENE_MAIN, com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_para, TK_SEL_GPIO);
            break;

        case LED_STATUS_BREATH:
            com_led_breath_inner(led_id, LED_SCENE_MAIN);
            break;

        default:
            led_off_flag = TRUE;
            break;
        }
    }

    if (led_off_flag == TRUE)
    {
        set_pad_function(com_led_pin_cfg[led_id].pin, PAD_FUNC_GPIOOUT, NULL, (void *) 0);
    }
#endif
}

void com_led_light_dummy_inner(led_id_e led_id, led_scene_e led_scene)
{
#if (SUPPORT_LED_LIGHT == 1)
    bool update_flag = TRUE;

    if (g_com_comval.support_led_light == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    if ((led_scene == LED_SCENE_MAIN) && (com_led_scene_info[led_id].scene_status[LED_SCENE_SUB].led_status != LED_STATUS_NULL))
    {
        update_flag = FALSE;
    }

    if ((led_scene == LED_SCENE_SUB) && (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status != LED_STATUS_NULL))
    {
        _led_light_off_main(led_id);
    }

    if (update_flag == TRUE)
    {
        if (com_led_scene_info[led_id].led_timer_id != -1)
        {
            sys_del_irq_timer1(com_led_scene_info[led_id].led_timer_id);
            com_led_scene_info[led_id].led_timer_id = -1;
        }

        if ((com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_DUTY)
            || (com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_BREATH))
        {
            hal_disable_pwm(com_led_pin_cfg[led_id].pwm_idx);
        }

        set_pad_function(com_led_pin_cfg[led_id].pin, PAD_FUNC_GPIOOUT, NULL, (void *) 0);
    }

    com_led_scene_info[led_id].scene_status[led_scene].led_status = LED_STATUS_DUMMY;
#endif
}

void com_led_breath_inner(led_id_e led_id, led_scene_e led_scene)
{
#if (SUPPORT_LED_LIGHT == 1)
    pwm_para_t pwm_para;
    sim_pwm_info_t pwmInfo;
    sim_pwm_bre_info_t *pbi;
    bool update_flag = TRUE;

    if (g_com_comval.support_led_light == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
    	return;
    }

    //如果某个PIN不支持PWM，则只能使用GPIOOUT方式点亮LED
    if (com_led_pin_cfg[led_id].pwm_mfp == 0xff)
    {
        com_led_twinkle_init_inner(led_id, led_scene, PWM_PERIOD_1000MS, TK_SEL_GPIO);
        return;
    }

    if ((led_scene == LED_SCENE_MAIN) && (com_led_scene_info[led_id].scene_status[LED_SCENE_SUB].led_status != LED_STATUS_NULL))
    {
        update_flag = FALSE;
    }

    if ((led_scene == LED_SCENE_SUB) && (com_led_scene_info[led_id].scene_status[LED_SCENE_MAIN].led_status != LED_STATUS_NULL))
    {
        _led_light_off_main(led_id);
    }

    if (update_flag == TRUE)
    {
        if (com_led_scene_info[led_id].led_timer_id != -1)
        {
            sys_del_irq_timer1(com_led_scene_info[led_id].led_timer_id);
            com_led_scene_info[led_id].led_timer_id = -1;
        }

        if ((com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_DUTY)
                || (com_led_scene_info[led_id].scene_status[led_scene].led_status == LED_STATUS_BREATH))
        {
            hal_disable_pwm(com_led_pin_cfg[led_id].pwm_idx);
        }

        //支持硬件PWM
        pwm_para.mode = PWM_MODE_BREATH;
        pwm_para.breath_div = 1; //0 ~ 3 分别对应0.5s,1s,1.5s,2s
        pwm_para.Q_value = 3; //0 ~ 3 分别对应 0.25x ~ 1x
        pwm_para.H_time = 128; //128 for 1x, max 255 for 2x
        pwm_para.L_time = 128; //128 for 1x, max 255 for 2x
        set_pad_function(com_led_pin_cfg[led_id].pin, com_led_pin_cfg[led_id].pwm_mfp, NULL, 0);
        hal_set_pwm(com_led_pin_cfg[led_id].pwm_idx, &pwm_para);
    }

    com_led_scene_info[led_id].scene_status[led_scene].led_status = LED_STATUS_BREATH;
#endif
}
