
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

void __mpad_ejtag4_op(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    if ((uint8)(uint32)para == 0xff)
    {
        hal_debug_set_ejtag(0xff);
    }
    else
    {
        hal_debug_set_ejtag(3);
    }
}

void __mpad_btdebug_op(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    //Anolog优先级高于debug，需要确保关闭GPIO7的anolog模式
    hal_pmu_enable_lradc(1, FALSE);
    hal_set_pad_function(PAD_GPIO7, PAD_FUNC_GPIOIN, NULL, 0);
    hal_debug_set_btdebug((uint32)para);
}

typedef void (*__pad_op_func)(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para);

const __pad_op_func g_aud_dev_pad_op_func[PAD_GPIO0] =
{
    __mpad_ejtag4_op,  //MPAD_EJTAG4
    __mpad_btdebug_op, //MPAD_BTDEBUG
};


//TODO set_pad_function 可以返回硬件handle，比如PWM，这样应用层就无需关心PWM的ID等细节

int8 hal_set_pad_function(ic_pad_e pad_id, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    int8 ret = 0;

    if (pad_id >= PAD_ID_MAX)
    {
        g_p_sys_hal_cbks->hal_printf("<ERROR> set_pad_function pad_id error!\n");
        return -1;
    }

    if (pad_id >= PAD_GPIO0)
    {
        uint8 pad_gpio = pad_id - PAD_GPIO0;
        ret = hal_pad_gpio_op_template(pad_gpio, PAD_OP_SET, pad_func, phy_para, para);
    }
    else
    {
        (g_aud_dev_pad_op_func[pad_id])(PAD_OP_SET, pad_func, phy_para, para);
    }

    if (ret < 0)
    {
        g_p_sys_hal_cbks->hal_printf("<ERROR> set_pad_function ret error! %d\n", ret);
    }

    return ret;
}

void hal_ioctl_pad(ic_pad_e pad_id, pad_function_e pad_func, void *para)
{
    if (pad_id >= PAD_ID_MAX)
    {
        return;
    }

    if (pad_id >= PAD_GPIO0)
    {
        uint8 pad_gpio = pad_id - PAD_GPIO0;
        hal_pad_gpio_op_template(pad_gpio, PAD_OP_IOCTL, pad_func, NULL, para);
    }
    else
    {
        (g_aud_dev_pad_op_func[pad_id])(PAD_OP_IOCTL, pad_func, NULL, para);
    }

    return;
}
