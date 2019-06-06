
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

void hal_set_pwm_init(uint8 idx, pwm_para_t *pwm_para)
{
    uint32 cpu_sr, temp;
    uint32 cmu_pwmxclk = CMU_PWM0CLK + (4*idx);
    uint32 pwmx_ctl = PWM0_CTL + (4*idx);

    cpu_sr = g_p_sys_hal_cbks->hal_int_close();
    *((REG32)(MRCR)) |= (1 << MRCR_PWM_RESET);
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_PWMCLKEN);
    g_p_sys_hal_cbks->hal_int_open(cpu_sr);

    temp = *((REG32)(pwmx_ctl));

    temp |= (1<<PWM0_CTL_PWM_EN);

    if (pwm_para->mode == PWM_MODE_BREATH)
    {
        if (pwm_para->breath_div == 0)
        {
            *((REG32)(cmu_pwmxclk)) = 3; //CK_32K divisor = 4
        }
        else if (pwm_para->breath_div == 1)
        {
            *((REG32)(cmu_pwmxclk)) = 7; //CK_32K divisor = 8
        }
        else if (pwm_para->breath_div == 2)
        {
            *((REG32)(cmu_pwmxclk)) = 11; //CK_32K divisor = 12
        }
        else
        {
            *((REG32)(cmu_pwmxclk)) = 15; //CK_32K divisor = 16
        }

        temp |= (1 << PWM0_CTL_Mode_SEL);

        temp &= (~PWM0_CTL_Q_MASK);
        temp |= (pwm_para->Q_value << PWM0_CTL_Q_SHIFT);

        temp &= (~PWM0_CTL_H_MASK);
        temp |= (pwm_para->H_time << PWM0_CTL_H_SHIFT);

        temp &= (~PWM0_CTL_L_MASK);
        temp |= (pwm_para->L_time << PWM0_CTL_L_SHIFT);

        *((REG32)(pwmx_ctl)) = temp;
    }
    else
    {
        if (pwm_para->period == PWM_PERIOD_200MS)
        {
            *((REG32)(cmu_pwmxclk)) = 24; //CK_32K divisor = 25
        }
        else if (pwm_para->period == PWM_PERIOD_400MS)
        {
            *((REG32)(cmu_pwmxclk)) = 49; //CK_32K divisor = 50
        }
        else if (pwm_para->period == PWM_PERIOD_1000MS)
        {
            *((REG32)(cmu_pwmxclk)) = 124; //CK_32K divisor = 125
        }
        else
        {
            *((REG32)(cmu_pwmxclk)) = 249; //CK_32K divisor = 250
        }

        temp &= ~(1 << PWM0_CTL_Mode_SEL);

        temp &= ~(1 << PWM0_CTL_POL_SEL);
        temp |= (pwm_para->polarity << PWM0_CTL_POL_SEL);

        temp &= ~PWM0_CTL_DUTY_MASK;
        temp |= (pwm_para->duty << PWM0_CTL_DUTY_SHIFT);

        *((REG32)(pwmx_ctl)) = temp;
    }
}

void hal_reset_pwm(uint8 idx)
{
    uint32 cpu_sr;
    uint32 pwmx_ctl = PWM0_CTL + (4*idx);

    *((REG32)(pwmx_ctl)) = 0;

//    cpu_sr = g_p_sys_hal_cbks->hal_int_close();
//    *((REG32)(MRCR)) &= ~(1 << MRCR_PWM_RESET);
//    *((REG32)(CMU_DEVCLKEN)) &= ~(1 << CMU_DEVCLKEN_PWMCLKEN);
//    g_p_sys_hal_cbks->hal_int_open(cpu_sr);
}
