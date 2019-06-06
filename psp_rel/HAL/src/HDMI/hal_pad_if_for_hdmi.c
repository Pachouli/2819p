
//for HDMI I2C, input&output should be used at the same time

#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

extern int8 __pad_gpiox_anolog_digital_set(uint8 idx, pad_function_e pad_func);

void __hdmi_set_gpio_out_init(uint8 pad_gpio, uint8 level)
{
    uint32 GPIO_CTRL_X = 0;

    GPIO_CTRL_X = GPIO0_CTL + 4 * pad_gpio;

    *((REG32) (GPIO_CTRL_X)) &= (~GPIO0_CTL_MFP_MASK);
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_MFP_SHIFT);
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_GPIOOUTEN);
    //*((REG32) (GPIO_CTRL_X)) &= (~(1 << GPIO0_CTL_GPIOINEN));

    hal_set_gpio_out_level(pad_gpio, level);
}

void __hdmi_set_gpio_in_init(uint8 pad_gpio)
{
    uint32 GPIO_CTRL_X = 0;

    GPIO_CTRL_X = GPIO0_CTL + 4 * pad_gpio;

    *((REG32) (GPIO_CTRL_X)) &= (~GPIO0_CTL_MFP_MASK);
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_MFP_SHIFT);
    //*((REG32) (GPIO_CTRL_X)) &= (~(1 << GPIO0_CTL_GPIOOUTEN));
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_GPIOINEN);
}

int8 hal_hdmi_pad_gpio_op_template(uint8 pad_gpio, pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    int8 ret = 0;

    if (pad_op == PAD_OP_SET)
    {
        //�л�Ϊ digital ģʽ
        __pad_gpiox_anolog_digital_set(pad_gpio, pad_func);

        if (pad_func == PAD_FUNC_GPIOOUT)
        {
            __hdmi_set_gpio_out_init(pad_gpio, (uint8) (uint32) para);
        }
        else if (pad_func == PAD_FUNC_GPIOIN)
        {
            __hdmi_set_gpio_in_init(pad_gpio);
        }
    }

    return ret;
}

int8 hal_hdmi_set_pad_function(ic_pad_e pad_id, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    int8 ret = 0;

    if (pad_id >= PAD_ID_MAX)
    {
        g_p_sys_hal_cbks->hal_printf("<HDMI> set_pad_function pad_id error!\n");
        return -1;
    }

    if (pad_id >= PAD_GPIO0)
    {
        uint8 pad_gpio = pad_id - PAD_GPIO0;
        ret = hal_hdmi_pad_gpio_op_template(pad_gpio, PAD_OP_SET, pad_func, phy_para, para);
    }
    else
    {
        g_p_sys_hal_cbks->hal_printf("<HDMI> set_pad_function pad_id error! %d\n", pad_id);
    }

    if (ret < 0)
    {
        g_p_sys_hal_cbks->hal_printf("<HDMI> set_pad_function ret error! %d\n", ret);
    }

    return ret;
}

