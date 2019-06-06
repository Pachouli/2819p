
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

/************************************ IO BASIC OP **************************************/

extern void __set_gpio_phy(uint8 pad_gpio, pad_phy_para_t *phy_para);
extern void __set_sio_phy(uint8 pad_sio, pad_phy_para_t *phy_para);
extern void __set_gpio_out_init(uint8 pad_gpio, uint8 level);
extern void __set_gpio_in_init(uint8 pad_gpio);
extern void __set_sio_out_init(uint8 pad_sio, uint8 level);
extern void __set_sio_out_level(uint8 pad_sio, uint8 level);
extern void __set_sio_in_init(uint8 pad_sio);

/************************************ Defines **************************************/

typedef int8 (*cbk_pad_mfp_set)(pad_function_e pad_func, void *para);

typedef struct
{
    cbk_pad_mfp_set f_mfp_set;
} pad_io_template_f_t;

/************************************ PAD OP **************************************/

const int8 __pad_anolog_idx[PAD_GPIO_ID_MAX] =
{
    0, 0, 0,
    9, //IO3 LRADC9
    10, //IO4 LRADC10
    0, 0,
    1, //IO7 LRADC1
    0, 0, 0, 0,
    2, //IO12 LRADC2
    3, //IO13 LRADC3
    0, 0, 0, 0,
    2, //IO18 LRADC2
    3, //IO19 LRADC3
    4, //IO20 LRADC4
    5, //IO21 LRADC5
    6, //IO22 LRADC6
    7, //IO23 LRADC7
    8, //IO24 LRADC8
    9, //IO25 LRADC9
    10, //IO26 LRADC10
    4, //IO27 LRADC4
    5, //IO28 LRADC5
    6, //IO29 LRADC6
    7, //IO30 LRADC7
    8, //IO31 LRADC8
    0, 0, 0,
    4, //IO35 LRADC4
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    10, //IO48 LRADC10
    0, 0, 0,
};

int8 __pad_gpiox_anolog_digital_set(uint8 idx, pad_function_e pad_func)
{
    int8 ret = 0;
    uint32 GPIO_CTRL_X = 0;

    if (idx >= PAD_GPIO_ID_MAX)
    {
        return -1;
    }

    GPIO_CTRL_X = GPIO0_CTL + 4 * idx;

    if (pad_func == PAD_FUNC_ANOLOG)
    {
        *((REG32)(GPIO_CTRL_X)) |= (1 << GPIO3_CTL_AD_Select);

        ret = __pad_anolog_idx[idx];
    }
    else
    {
        *((REG32)(GPIO_CTRL_X)) &= ~(1 << GPIO3_CTL_AD_Select);
    }

    return ret;
}

int8 __pad_gpio0_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO0_CTL)) &= ~(0x0f);
        *((REG32) (GPIO0_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO0_MFP_PWM0)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio1_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO1_CTL)) &= ~(0x0f);
        *((REG32) (GPIO1_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO1_MFP_UART0_RX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO1_MFP_PWM1)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio2_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO2_CTL)) &= ~(0x0f);
        *((REG32) (GPIO2_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO2_MFP_UART0_TX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO2_MFP_I2C0_SDA)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio3_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO3_CTL)) &= ~(0x0f);
        *((REG32) (GPIO3_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO3_MFP_I2C0_SCL)
        {
            ret = 0;
        }
        else if (pad_func == GPIO3_MFP_PWM2)
        {
            ret = 2;
        }
    }

    return ret;
}

int8 __pad_gpio4_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO4_CTL)) &= ~(0x0f);
        *((REG32) (GPIO4_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO4_MFP_UART0_TX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO4_MFP_PWM3)
        {
            ret = 3;
        }
    }

    return ret;
}

int8 __pad_gpio5_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO5_CTL)) &= ~(0x0f);
        *((REG32) (GPIO5_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO5_MFP_UART0_RX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO5_MFP_I2C0_SDA)
        {
            ret = 0;
        }
        else if (pad_func == GPIO5_MFP_I2C1_SDA)
        {
            ret = 1;
        }
        else if (pad_func == GPIO5_MFP_PWM4)
        {
            ret = 4;
        }
    }

    return ret;
}

int8 __pad_gpio6_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO6_CTL)) &= ~(0x0f);
        *((REG32) (GPIO6_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO6_MFP_UART0_TX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO6_MFP_I2C0_SCL)
        {
            ret = 0;
        }
        else if (pad_func == GPIO6_MFP_I2C1_SCL)
        {
            ret = 1;
        }
        else if (pad_func == GPIO6_MFP_PWM5)
        {
            ret = 5;
        }
    }

    return ret;
}

int8 __pad_gpio8_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO8_CTL)) &= ~(0x0f);
        *((REG32) (GPIO8_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO8_MFP_UART1_TX)
        {
            ret = 1;
        }
        else if (pad_func == GPIO8_MFP_I2C0_SDA)
        {
            ret = 0;
        }
        else if (pad_func == GPIO8_MFP_PWM6)
        {
            ret = 6;
        }
    }

    return ret;
}

int8 __pad_gpio9_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO9_CTL)) &= ~(0x0f);
        *((REG32) (GPIO9_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO9_MFP_UART1_RX)
        {
            ret = 1;
        }
        else if (pad_func == GPIO9_MFP_I2C0_SCL)
        {
            ret = 0;
        }
        else if (pad_func == GPIO9_MFP_PWM7)
        {
            ret = 7;
        }
    }

    return ret;
}

int8 __pad_gpio10_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO10_CTL)) &= ~(0x0f);
        *((REG32) (GPIO10_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO10_MFP_PWM8)
        {
            ret = 8;
        }
    }

    return ret;
}

int8 __pad_gpio11_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO11_CTL)) &= ~(0x0f);
        *((REG32) (GPIO11_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO11_MFP_PWM0)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio12_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO12_CTL)) &= ~(0x0f);
        *((REG32) (GPIO12_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO12_MFP_UART0_TX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO12_MFP_I2C1_SDA)
        {
            ret = 1;
        }
        else if (pad_func == GPIO12_MFP_PWM8)
        {
            ret = 8;
        }
    }

    return ret;
}

int8 __pad_gpio13_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO13_CTL)) &= ~(0x0f);
        *((REG32) (GPIO13_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO13_MFP_UART0_RX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO13_MFP_I2C1_SCL)
        {
            ret = 1;
        }
        else if (pad_func == GPIO13_MFP_PWM0)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio14_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO14_CTL)) &= ~(0x0f);
        *((REG32) (GPIO14_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO14_MFP_UART1_TX)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio15_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO15_CTL)) &= ~(0x0f);
        *((REG32) (GPIO15_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO15_MFP_UART1_RX)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio16_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO16_CTL)) &= ~(0x0f);
        *((REG32) (GPIO16_CTL)) |= (mfp_func_id);
    }

    return ret;
}

int8 __pad_gpio17_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO17_CTL)) &= ~(0x0f);
        *((REG32) (GPIO17_CTL)) |= (mfp_func_id);
    }

    return ret;
}

int8 __pad_gpio18_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO18_CTL)) &= ~(0x0f);
        *((REG32) (GPIO18_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO18_MFP_PWM0)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio19_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO19_CTL)) &= ~(0x0f);
        *((REG32) (GPIO19_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO19_MFP_PWM8)
        {
            ret = 8;
        }
    }

    return ret;
}

int8 __pad_gpio20_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO20_CTL)) &= ~(0x0f);
        *((REG32) (GPIO20_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO20_MFP_PWM1)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio21_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO21_CTL)) &= ~(0x0f);
        *((REG32) (GPIO21_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO21_MFP_PWM2)
        {
            ret = 2;
        }
    }

    return ret;
}

int8 __pad_gpio22_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO22_CTL)) &= ~(0x0f);
        *((REG32) (GPIO22_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO22_MFP_PWM3)
        {
            ret = 3;
        }
    }

    return ret;
}

int8 __pad_gpio23_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO23_CTL)) &= ~(0x0f);
        *((REG32) (GPIO23_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO23_MFP_PWM4)
        {
            ret = 4;
        }
    }

    return ret;
}

int8 __pad_gpio24_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO24_CTL)) &= ~(0x0f);
        *((REG32) (GPIO24_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO24_MFP_UART0_TX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO24_MFP_PWM5)
        {
            ret = 5;
        }
    }

    return ret;
}

int8 __pad_gpio25_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO25_CTL)) &= ~(0x0f);
        *((REG32) (GPIO25_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO25_MFP_UART0_RX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO25_MFP_PWM6)
        {
            ret = 6;
        }
    }

    return ret;
}

int8 __pad_gpio26_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO26_CTL)) &= ~(0x0f);
        *((REG32) (GPIO26_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO26_MFP_UART1_TX)
        {
            ret = 1;
        }
        else if (pad_func == GPIO26_MFP_PWM7)
        {
            ret = 7;
        }
    }

    return ret;
}

int8 __pad_gpio27_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO27_CTL)) &= ~(0x0f);
        *((REG32) (GPIO27_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO27_MFP_UART1_RX)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio28_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO28_CTL)) &= ~(0x0f);
        *((REG32) (GPIO28_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO28_MFP_I2C1_SDA)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio29_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO29_CTL)) &= ~(0x0f);
        *((REG32) (GPIO29_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO29_MFP_I2C1_SCL)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio30_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO30_CTL)) &= ~(0x0f);
        *((REG32) (GPIO30_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO30_MFP_I2C0_SDA)
        {
            ret = 0;
        }
        else if (pad_func == GPIO30_MFP_UART0_RX)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio31_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO31_CTL)) &= ~(0x0f);
        *((REG32) (GPIO31_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO31_MFP_I2C0_SCL)
        {
            ret = 0;
        }
        else if (pad_func == GPIO31_MFP_UART0_TX)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio32_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO32_CTL)) &= ~(0x0f);
        *((REG32) (GPIO32_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO32_MFP_PWM0)
        {
            ret = 0;
        }
    }

    return ret;
}

int8 __pad_gpio33_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO33_CTL)) &= ~(0x0f);
        *((REG32) (GPIO33_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO33_MFP_PWM1)
        {
            ret = 1;
        }
    }

    return ret;
}

int8 __pad_gpio34_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO34_CTL)) &= ~(0x0f);
        *((REG32) (GPIO34_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO34_MFP_PWM2)
        {
            ret = 2;
        }
    }

    return ret;
}

int8 __pad_gpio35_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO35_CTL)) &= ~(0x0f);
        *((REG32) (GPIO35_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO35_MFP_PWM3)
        {
            ret = 3;
        }
    }

    return ret;
}

int8 __pad_gpio36_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO36_CTL)) &= ~(0x0f);
        *((REG32) (GPIO36_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO36_MFP_I2C0_SCL)
        {
            ret = 0;
        }
        else if (pad_func == GPIO36_MFP_UART1_TX)
        {
            ret = 1;
        }
        else if (pad_func == GPIO36_MFP_PWM4)
        {
            ret = 4;
        }
    }

    return ret;
}

int8 __pad_gpio37_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO37_CTL)) &= ~(0x0f);
        *((REG32) (GPIO37_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO37_MFP_I2C0_SDA)
        {
            ret = 0;
        }
        else if (pad_func == GPIO37_MFP_UART1_RX)
        {
            ret = 1;
        }
        else if (pad_func == GPIO37_MFP_PWM5)
        {
            ret = 5;
        }
    }

    return ret;
}

int8 __pad_gpio48_mfp_set(pad_function_e pad_func, void *para)
{
    int8 ret = 0;

    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO48_CTL)) &= ~(0x0f);
        *((REG32) (GPIO48_CTL)) |= (mfp_func_id);

        if (pad_func == GPIO48_MFP_UART0_TX)
        {
            ret = 0;
        }
        else if (pad_func == GPIO48_MFP_UART1_TX)
        {
            ret = 1;
        }
        else if (pad_func == GPIO48_MFP_PWM8)
        {
            ret = 8;
        }
    }

    return 0;
}

int8 __pad_gpio49_mfp_set(pad_function_e pad_func, void *para)
{
    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO49_CTL)) &= ~(0x0f);
        *((REG32) (GPIO49_CTL)) |= (mfp_func_id);
    }

    return 0;
}

int8 __pad_gpio50_mfp_set(pad_function_e pad_func, void *para)
{
    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO50_CTL)) &= ~(0x0f);
        *((REG32) (GPIO50_CTL)) |= (mfp_func_id);
    }

    return 0;
}

int8 __pad_gpio51_mfp_set(pad_function_e pad_func, void *para)
{
    if (pad_func >= PAD_FUNC_MFP)
    {
        uint8 mfp_func_id = pad_func - PAD_FUNC_MFP;

        *((REG32) (GPIO51_CTL)) &= ~(0x0f);
        *((REG32) (GPIO51_CTL)) |= (mfp_func_id);
    }

    return 0;
}


const pad_io_template_f_t g_pad_io_template_f[54] =
{
    { __pad_gpio0_mfp_set },
    { __pad_gpio1_mfp_set },
    { __pad_gpio2_mfp_set },
    { __pad_gpio3_mfp_set },
    { __pad_gpio4_mfp_set },
    { __pad_gpio5_mfp_set },
    { __pad_gpio6_mfp_set },
    { NULL },
    { __pad_gpio8_mfp_set },
    { __pad_gpio9_mfp_set },
    { __pad_gpio10_mfp_set },
    { __pad_gpio11_mfp_set },
    { __pad_gpio12_mfp_set },
    { __pad_gpio13_mfp_set },
    { __pad_gpio14_mfp_set },
    { __pad_gpio15_mfp_set },
    { __pad_gpio16_mfp_set },
    { __pad_gpio17_mfp_set },
    { __pad_gpio18_mfp_set },
    { __pad_gpio19_mfp_set },
    { __pad_gpio20_mfp_set },
    { __pad_gpio21_mfp_set },
    { __pad_gpio22_mfp_set },
    { __pad_gpio23_mfp_set },
    { __pad_gpio24_mfp_set },
    { __pad_gpio25_mfp_set },
    { __pad_gpio26_mfp_set },
    { __pad_gpio27_mfp_set },
    { __pad_gpio28_mfp_set },
    { __pad_gpio29_mfp_set },
    { __pad_gpio30_mfp_set },
    { __pad_gpio31_mfp_set },
    { __pad_gpio32_mfp_set },
    { __pad_gpio33_mfp_set },
    { __pad_gpio34_mfp_set },
    { __pad_gpio35_mfp_set },
    { __pad_gpio36_mfp_set },
    { __pad_gpio37_mfp_set },
    { NULL }, //PAD_VRO,   //GPIO38
    { NULL }, //PAD_VRO_S, //GPIO39
    { NULL }, //PAD_AUX0L, //GPIO40
    { NULL }, //PAD_AUX0R, //GPIO41
    { NULL }, //PAD_AOUTL, //GPIO42
    { NULL }, //PAD_AOUTR, //GPIO43
    { NULL }, //PAD_AUX1L, //GPIO44
    { NULL }, //PAD_AUX1R, //GPIO45
    { NULL }, //PAD_MIC0,  //GPIO46
    { NULL }, //PAD_MIC1,  //GPIO47
    { __pad_gpio48_mfp_set },
    { __pad_gpio49_mfp_set },
    { __pad_gpio50_mfp_set },
    { __pad_gpio51_mfp_set },
    { NULL },
    { NULL },
};

int8 hal_pad_gpio_op_template(uint8 pad_gpio, pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    int8 ret = 0;

    if (pad_op == PAD_OP_SET)
    {
        if (pad_func == PAD_FUNC_ANOLOG)
        {
            ret = __pad_gpiox_anolog_digital_set(pad_gpio, pad_func);
        }
        else
        {
            //�л�Ϊ digital ģʽ
            __pad_gpiox_anolog_digital_set(pad_gpio, pad_func);

            if (pad_func == PAD_FUNC_GPIOOUT)
            {
                __set_gpio_out_init(pad_gpio, (uint8) (uint32) para);
            }
            else if (pad_func == PAD_FUNC_GPIOIN)
            {
                __set_gpio_in_init(pad_gpio);
            }
            else if (pad_func == PAD_FUNC_SIOOUT)
            {
                __set_sio_out_init(pad_gpio-52, (uint8) (uint32) para);
            }
            else if (pad_func == PAD_FUNC_SIOIN)
            {
                __set_sio_in_init(pad_gpio-52);
            }
            else if (g_pad_io_template_f[pad_gpio].f_mfp_set != NULL)
            {
                ret = g_pad_io_template_f[pad_gpio].f_mfp_set(pad_func, para);
            }

            if (pad_gpio >= 52)
            {
                __set_sio_phy(pad_gpio-52, phy_para);
            }
            else
            {
                __set_gpio_phy(pad_gpio, phy_para);
            }
        }
    }
    else if (pad_op == PAD_OP_IOCTL)
    {
        if (pad_func == PAD_FUNC_GPIOOUT)
        {
            hal_set_gpio_out_level(pad_gpio, (uint8) (uint32) para);
        }
        else if (pad_func == PAD_FUNC_GPIOIN)
        {
            *(uint8 *) (para) = hal_get_gpio_in_level(pad_gpio);
        }
        else if (pad_func == PAD_FUNC_SIOOUT)
        {
            __set_sio_out_level(pad_gpio-52, (uint8) (uint32) para);
        }
        else if (pad_func == PAD_FUNC_SIOIN)
        {
            *(uint8 *) (para) = hal_get_sio_in_level(pad_gpio-52);
        }
    }

    return ret;
}
