
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

/************************************ IO BASIC OP **************************************/

void __set_gpio_phy(uint8 pad_gpio, pad_phy_para_t *phy_para)
{
    uint32 GPIO_CTRL_X = 0;

    if (phy_para == NULL)
    {
        return;
    }

    GPIO_CTRL_X = GPIO0_CTL + 4 * pad_gpio;

    if (phy_para->pad_pu_en == 0)
    {
        *((REG32) (GPIO_CTRL_X)) &= ~(1 << GPIO0_CTL_GPIO50KPUEN);
        *((REG32) (GPIO_CTRL_X)) &= ~(1 << GPIO0_CTL_10KPUEN);
    }
    else if (phy_para->pad_pu_en == 1)
    {
        *((REG32) GPIO_CTRL_X) |= (1 << GPIO0_CTL_GPIO50KPUEN);
        *((REG32) (GPIO_CTRL_X)) &= ~(1 << GPIO0_CTL_10KPUEN);
    }
    else
    {
        *((REG32) GPIO_CTRL_X) |= (1 << GPIO0_CTL_10KPUEN);
        *((REG32) (GPIO_CTRL_X)) &= ~(1 << GPIO0_CTL_GPIO50KPUEN);
    }

    if (phy_para->pad_pd_en == 1)
    {
        *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_GPIO100KPDEN);
    }
    else
    {
        *((REG32) (GPIO_CTRL_X)) &= ~(1 << GPIO0_CTL_GPIO100KPDEN);
    }

    if (phy_para->smit_en == 1)
    {
    	*((REG32) GPIO_CTRL_X) |= (1 << GPIO0_CTL_SMIT);
    }
    else
    {
    	*((REG32) GPIO_CTRL_X) &= ~(1 << GPIO0_CTL_SMIT);
    }

    if (phy_para->pad_driver <= 7)
    {
        *((REG32) (GPIO_CTRL_X)) &= ~GPIO0_CTL_PADDRV_MASK;
        *((REG32) (GPIO_CTRL_X)) |= ((phy_para->pad_driver) << GPIO0_CTL_PADDRV_SHIFT);
    }
}

void __set_sio_phy(uint8 pad_sio, pad_phy_para_t *phy_para)
{
    uint32 WIO_CTRL_X = 0;
    uint32 wio_ctrl_value;

    if (phy_para == NULL)
    {
        return;
    }

    if (pad_sio == 0)
    {
        WIO_CTRL_X = WIO0_CTL;
    }
    else
    {
        WIO_CTRL_X = WIO1_CTL;
    }
    wio_ctrl_value = *((REG32)(WIO_CTRL_X));

    if (phy_para->pad_pu_en == 1)
    {
        wio_ctrl_value |= (1 << WIO0_CTL_SIO500KPUEN);
    }
    else
    {
        wio_ctrl_value &= ~(1 << WIO0_CTL_SIO500KPUEN);
    }

    if (phy_para->pad_pd_en == 1)
    {
        wio_ctrl_value |= (1 << WIO0_CTL_SIO500KPDEN);
    }
    else
    {
        wio_ctrl_value &= ~(1 << WIO0_CTL_SIO500KPDEN);
    }

    if (phy_para->pad_driver <= 7)
    {
        wio_ctrl_value &= ~WIO0_CTL_PADDRV_MASK;
        wio_ctrl_value |= ((phy_para->pad_driver) << WIO0_CTL_PADDRV_SHIFT);
    }

    *((REG32)(WIO_CTRL_X)) = wio_ctrl_value;

    //RTCVDD寄存器写入需要等待一小点时间
    g_p_sys_hal_cbks->hal_udelay(150);
}

void __section__(".rcode") hal_set_gpio_out_level(uint8 pad_gpio, uint8 level)
{
    uint32 GPIO_BSRX;
    uint32 GPIO_BRRX;
    uint8 gpio_index = 0;

    GPIO_BSRX = (pad_gpio <= 31) ? (GPIO_BSR0) : (GPIO_BSR1);
    GPIO_BRRX = (pad_gpio <= 31) ? (GPIO_BRR0) : (GPIO_BRR1);
    gpio_index = (pad_gpio <= 31) ? (pad_gpio) : (pad_gpio - 32);

    if (level == 1)
    {
        *((REG32) (GPIO_BSRX)) = (1 << gpio_index); //单周期原子动作，不用关中断
    }
    else
    {
        *((REG32) (GPIO_BRRX)) = (1 << gpio_index); //单周期原子动作，不用关中断
    }
}

void __set_gpio_out_init(uint8 pad_gpio, uint8 level)
{
    uint32 GPIO_CTRL_X = 0;

    GPIO_CTRL_X = GPIO0_CTL + 4 * pad_gpio;

    *((REG32) (GPIO_CTRL_X)) &= (~GPIO0_CTL_MFP_MASK);
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_MFP_SHIFT);
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_GPIOOUTEN);
    *((REG32) (GPIO_CTRL_X)) &= (~(1 << GPIO0_CTL_GPIOINEN));

    hal_set_gpio_out_level(pad_gpio, level);
}

void __set_gpio_in_init(uint8 pad_gpio)
{
    uint32 GPIO_CTRL_X = 0;

    GPIO_CTRL_X = GPIO0_CTL + 4 * pad_gpio;

    *((REG32) (GPIO_CTRL_X)) &= (~GPIO0_CTL_MFP_MASK);
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_MFP_SHIFT);
    *((REG32) (GPIO_CTRL_X)) &= (~(1 << GPIO0_CTL_GPIOOUTEN));
    *((REG32) (GPIO_CTRL_X)) |= (1 << GPIO0_CTL_GPIOINEN);
}

uint8 __section__(".rcode") hal_get_gpio_in_level(uint8 pad_gpio)
{
    uint32 GPIO_INDATA_X = 0;
    uint8 gpio_index = 0;

    GPIO_INDATA_X = (pad_gpio <= 31) ? (GPIO_IDAT0) : (GPIO_IDAT1);
    gpio_index = (pad_gpio <= 31) ? (pad_gpio) : (pad_gpio - 32);

    return ((*((REG32) (GPIO_INDATA_X)) & (1 << gpio_index)) != 0);
}

void __set_sio_out_level(uint8 pad_sio, uint8 level)
{
    uint32 WIO_CTRL_X = 0;
    uint32 wio_ctrl_value;

    if (pad_sio == 0)
    {
        WIO_CTRL_X = WIO0_CTL;
    }
    else
    {
        WIO_CTRL_X = WIO1_CTL;
    }
    wio_ctrl_value = *((REG32)(WIO_CTRL_X));

    if (level == 1)
    {
        wio_ctrl_value |= (1 << WIO0_CTL_SIODAT);
    }
    else
    {
        wio_ctrl_value &= (~(1 << WIO0_CTL_SIODAT));
    }

    *((REG32)(WIO_CTRL_X)) = wio_ctrl_value;

    //RTCVDD寄存器写入需要等待一小点时间
    g_p_sys_hal_cbks->hal_udelay(150);
}

void __set_sio_out_init(uint8 pad_sio, uint8 level)
{
    uint32 WIO_CTRL_X = 0;
    uint32 wio_ctrl_value;

    if (pad_sio == 0)
    {
        WIO_CTRL_X = WIO0_CTL;
    }
    else
    {
        WIO_CTRL_X = WIO1_CTL;
    }
    wio_ctrl_value = *((REG32)(WIO_CTRL_X));

    wio_ctrl_value |= (1 << WIO0_CTL_SIOOUTEN);
    wio_ctrl_value &= (~(1 << WIO0_CTL_SIOINEN));

    *((REG32)(WIO_CTRL_X)) = wio_ctrl_value;

    //RTCVDD寄存器写入需要等待一小点时间
    g_p_sys_hal_cbks->hal_udelay(150);

    __set_sio_out_level(pad_sio, level);
}



void __set_sio_in_init(uint8 pad_sio)
{
    uint32 WIO_CTRL_X = 0;
    uint32 wio_ctrl_value;

    if (pad_sio == 0)
    {
        WIO_CTRL_X = WIO0_CTL;
    }
    else
    {
        WIO_CTRL_X = WIO1_CTL;
    }
    wio_ctrl_value = *((REG32)(WIO_CTRL_X));

    wio_ctrl_value &= (~(1 << WIO0_CTL_SIOOUTEN));
    wio_ctrl_value |= (1 << WIO0_CTL_SIOINEN);

    *((REG32)(WIO_CTRL_X)) = wio_ctrl_value;

    //RTCVDD寄存器写入需要等待一小点时间
    g_p_sys_hal_cbks->hal_udelay(150);
}

uint8 __section__(".rcode") hal_get_sio_in_level(uint8 pad_sio)
{
    uint32 WIO_CTRL_X = 0;

    if (pad_sio == 0)
    {
        WIO_CTRL_X = WIO0_CTL;
    }
    else
    {
        WIO_CTRL_X = WIO1_CTL;
    }

    return ((*((REG32)(WIO_CTRL_X)) & (1 << WIO0_CTL_SIODAT)) != 0);
}

