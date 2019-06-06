/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏物理驱动实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "lcd_phy_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

#define CS_SPINOR      0
#define CS_LCD_MODULE  1

//dev_id 0-spinor,1-lcd
void Hdrv_spi1_switch_cs(uint8 dev_id)
{
    pad_phy_para_t phy_para;

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 1;
    phy_para.pad_driver = 3;

    if (dev_id == CS_SPINOR)
    {
        set_pad_function(PAD_GPIO20, PAD_FUNC_GPIOOUT, NULL, 1);
        set_pad_function(PAD_GPIO12, GPIO12_MFP_SPI1_SS, &phy_para, 0);
    }
    else
    {
        set_pad_function(PAD_GPIO12, PAD_FUNC_GPIOOUT, NULL, 1);
        set_pad_function(PAD_GPIO20, GPIO20_MFP_SPI1_SS, &phy_para, 0);
    }
}

void Hdrv_lcd_write_cmd(uint8 cmd)
{
    uint8 tmp[1];

    spi1_set_cs(NULL, 0);

    set_pad_function(PAD_GPIO25, PAD_FUNC_GPIOOUT, NULL, 0); //RS,0-write cmd,1-write data
    tmp[0] = cmd;
    spi1_write_data(NULL, tmp, 1, 0);

    spi1_set_cs(NULL, 1);
}

void Hdrv_lcd_write_data(uint8 dat)
{
    uint8 tmp[1];

    spi1_set_cs(NULL, 0);

    set_pad_function(PAD_GPIO25, PAD_FUNC_GPIOOUT, NULL, 1); //RS,0-write cmd,1-write data
    tmp[0] = dat;
    spi1_write_data(NULL, tmp, 1, 0);

    spi1_set_cs(NULL, 1);
}

void Hdrv_lcd_poweron(void)
{
    //RESET
    set_pad_function(SPI_LCD_MOD_RST_PIN, SPI_LCD_MOD_RST_FUNC, NULL, 1); //RST
    sys_mdelay(1);
    set_pad_function(SPI_LCD_MOD_RST_PIN, SPI_LCD_MOD_RST_FUNC, NULL, 0); //RST
    sys_udelay(10);
    set_pad_function(SPI_LCD_MOD_RST_PIN, SPI_LCD_MOD_RST_FUNC, NULL, 1); //RST
    sys_mdelay(10);

    set_pad_function(SPI_LCD_MOD_RS_SEL_PIN, SPI_LCD_MOD_RS_SEL_FUNC, NULL, 1); //RS,0-write cmd,1-write data

    set_pad_function(SPI_LCD_MOD_BKLIGHT_PIN, SPI_LCD_MOD_BKLIGHT_FUNC, NULL, 0); //背光熄灭
}

void Hdrv_lcd_switch_light(bool on)
{
    if (on == TRUE)
    {
        set_pad_function(SPI_LCD_MOD_BKLIGHT_PIN, SPI_LCD_MOD_BKLIGHT_FUNC, NULL, 1); //背光亮
    }
    else
    {
        set_pad_function(SPI_LCD_MOD_BKLIGHT_PIN, SPI_LCD_MOD_BKLIGHT_FUNC, NULL, 0); //背光熄灭
    }
}

#endif
