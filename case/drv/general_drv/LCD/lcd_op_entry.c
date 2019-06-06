/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏驱动API表。
 * 作者：cailizhen
 ********************************************************************************/


#include "lcd_inner.h"

typedef void*(*lcd_op_func)(void*, void*, void*);

typedef struct
{
    lcd_op_func sdrv_lcd_module_install;
    lcd_op_func sdrv_lcd_module_uninstall;
    lcd_op_func sdrv_lcd_update_screen;
    lcd_op_func sdrv_lcd_set_window;
    lcd_op_func sdrv_lcd_standby_screen;
    lcd_op_func sdrv_lcd_set_contrast;
    lcd_op_func sdrv_lcd_clear_screen;
    lcd_op_func sdrv_lcd_show_image;
    lcd_op_func sdrv_lcd_show_multi_string;
    lcd_op_func sdrv_lcd_show_string;
    lcd_op_func sdrv_lcd_set_text_pos;
    lcd_op_func sdrv_lcd_scroll_string;
    lcd_op_func sdrv_lcd_scroll_multi_string;
    lcd_op_func sdrv_lcd_scroll_string_scrolling;
	lcd_op_func sdrv_lcd_invert;
	lcd_op_func sdrv_lcd_set_option;
} lcd_driver_operations;


const lcd_driver_operations lcd_driver_op =
{
#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    (lcd_op_func) lcd_module_install_inner,
    (lcd_op_func) lcd_module_uninstall_inner,
    (lcd_op_func) Hdrv_UpdateScreen,
    (lcd_op_func) Hdrv_SetWindow,
    (lcd_op_func) Hdrv_StandbyScreen,
    (lcd_op_func) Hdrv_SetContrast,
    (lcd_op_func) Sdrv_ClearScreen,
    (lcd_op_func) Sdrv_PutSDImage,
    (lcd_op_func) Sdrv_PutMultiS,
    (lcd_op_func) Sdrv_PutS,
    (lcd_op_func) Sdrv_SetTextPos,
    (lcd_op_func) Sdrv_ScrollString,
    (lcd_op_func) Sdrv_ScrollMultiString,
    (lcd_op_func) Sdrv_ScrollStringScrolling,
    (lcd_op_func) Sdrv_Invert,
    (lcd_op_func) Sdrv_SetOption,
#else
    0,
#endif
};
