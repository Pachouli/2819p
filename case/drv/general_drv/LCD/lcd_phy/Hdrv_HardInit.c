/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏物理驱动实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "lcd_phy_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

extern int32 _rd_lcd_data_start;
extern int32 _rd_lcd_data_end;

void lcd_module_phy_init(void)
{
    //初始化LCD驱动全局变量
    libc_memset((void *)&_rd_lcd_data_start, 0x00, (uint32)&_rd_lcd_data_end - (uint32)&_rd_lcd_data_start);

    Hdrv_lcd_poweron();

    Hdrv_spi1_switch_cs(CS_LCD_MODULE);

    Hdrv_lcd_write_cmd(0xE2);          //Software Reset
    sys_mdelay(120);

    Hdrv_lcd_write_cmd(0xA2);          //Set Bias     0xA3 1/7bias,  0xA2 1/9 bias

    Hdrv_lcd_write_cmd(0xC0);
    Hdrv_lcd_write_cmd(0xA0);//stir=1左右镜像,stir=0正常显示
    Hdrv_lcd_write_cmd(0xA6);
    Hdrv_lcd_write_cmd(0xA4);

    Hdrv_lcd_write_cmd(0x24);          //Set internal rb/ra  0x24
    Hdrv_lcd_write_cmd(0x81);          //Set Contrast
    Hdrv_lcd_write_cmd(0x20);          //0x19    //0x25   //20

    Hdrv_lcd_write_cmd(0x2B);    //The Power Control Set
    sys_mdelay(1);
    Hdrv_lcd_write_cmd(0x2E);    //The Power Control Set
    sys_mdelay(1);
    Hdrv_lcd_write_cmd(0x2F);    //The Power Control Set

    Hdrv_lcd_write_cmd(0x2F);          //Power Control Set
    Hdrv_lcd_write_cmd(0x2F);
    Hdrv_lcd_write_cmd(0x2F);

    sys_mdelay(10);                //delay 40mS

    Hdrv_lcd_write_cmd(0x40);          //Set Display Start Line
    Hdrv_lcd_write_cmd(0xAF);          //Display ON

    Hdrv_UpdateScreen(NULL);
    Hdrv_StandbyScreen(TRUE);
}

void lcd_module_install_inner(void)
{
    lcd_module_phy_init();

    //默认切换到spinor
    Hdrv_spi1_switch_cs(CS_SPINOR);

#ifdef LCD_HDRV_TEST
    lcd_module_test_inner();
#endif

    Sdrv_Init(0);
}

void lcd_module_uninstall_inner(void)
{
}

#endif
