/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：led物理驱动模块相关数据结构，宏定义，接口声明等
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _LED_INTERFACE_H
#define _LED_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"

#define NUMBER1    0
#define NUMBER2    1
#define NUMBER3    2
#define NUMBER4    3

#define LCD_PLAY   0    //ICON "PLAY"
#define LCD_PAUSE  1    //ICON "PAUSE"
#define LCD_USB    2    //ICON "USB"
#define LCD_SD     3    //ICON "SD"
#define LCD_COL    4    //ICON ":"
#define LCD_AUX    5    //ICON "AUX"
#define LCD_FM     6    //ICON "FM"
#define LCD_P1     7    //ICON "."
#define ICON_TOTAL 8

#define LED_CLR_ALL  0 //清除全部数字和图标
#define LED_CLR_NUM  1 //仅清除数字

/*!
 * \brief
 *  lcd_cmd_e：led驱动命令类型枚举类型
 */
typedef enum
{
    LED_MODULE_INSTALL = 0,
    LED_MODULE_UNINSTALL,
    /*!显示LED数字或字母或短划线*/
    LED_DISPLAY,
    /*!显示图标*/
    LED_DISPLAY_ICON,
    /*!清除屏幕*/
    LED_CLEAR_SCREEN,
    /*!闪烁屏幕*/
    LED_FLASH_SCREEN,
    /*唤醒LED显示*/
    LED_WAKE_UP,
    /*关LED显示*/
    LED_SLEEP,
    /*开关LED背光，背光关闭仍然会更新驱动数据，但是不刷到LED数码管上*/
    LED_LIGHT_ONOFF,
} led_cmd_e;

#define LED_ENTRY_ADDR(x)          (LED_ENTRY_VA_START + (4 * x))

#define LED_MODULE_INSTALL_ENTRY   (*((uint32*)(LED_ENTRY_ADDR(LED_MODULE_INSTALL))))
#define LED_MODULE_UNINSTALL_ENTRY (*((uint32*)(LED_ENTRY_ADDR(LED_MODULE_UNINSTALL))))
#define LED_DISPLAY_ENTRY          (*((uint32*)(LED_ENTRY_ADDR(LED_DISPLAY))))
#define LED_DISPLAY_ICON_ENTRY     (*((uint32*)(LED_ENTRY_ADDR(LED_DISPLAY_ICON))))
#define LED_CLEAR_SCREEN_ENTRY     (*((uint32*)(LED_ENTRY_ADDR(LED_CLEAR_SCREEN))))
#define LED_FLASH_SCREEN_ENTRY     (*((uint32*)(LED_ENTRY_ADDR(LED_FLASH_SCREEN))))
#define LED_WAKE_UP_ENTRY          (*((uint32*)(LED_ENTRY_ADDR(LED_WAKE_UP))))
#define LED_SLEEP_ENTRY            (*((uint32*)(LED_ENTRY_ADDR(LED_SLEEP))))
#define LED_LIGHT_ONOFF_ENTRY      (*((uint32*)(LED_ENTRY_ADDR(LED_LIGHT_ONOFF))))

#if 0
extern int32 led_module_init_inner(void);
#endif
#define led_drv_module_install()       ((int(*)(void))LED_MODULE_INSTALL_ENTRY)()

#if 0
extern int32 led_module_exit_inner(void);
#endif
#define led_drv_module_uninstall()     ((int(*)(void))LED_MODULE_UNINSTALL_ENTRY)()

/*!
 * \brief显示LED
 * input: num_idx--0-3
 *        c--数据内容，ascii字符
 *        display--显示或不显示
 */
#if 0
extern int32 led_display_inner(uint8 num_idx, uint8 c, bool display);
#endif
#define led_drv_display(a,b,c)         ((int(*)(uint8, uint8, bool))LED_DISPLAY_ENTRY)((a),(b),(c))

/*!
 * \brief显示LED
 * input: icon_idx--0-7
 *        display--显示或不显示
 */
#if 0
extern int32 led_display_icon_inner(uint8 icon_idx, bool display);
#endif
#define led_drv_display_icon(a,b)      ((int(*)(uint8, bool))LED_DISPLAY_ICON_ENTRY)((a),(b))

/*!
 * \brief
 *   清除屏幕
 */
#if 0
extern int32 led_clear_screen_inner(uint8 clr_mode);
#endif
#define led_drv_clear_screen(a)        ((int(*)(uint8))LED_CLEAR_SCREEN_ENTRY)((a))

/*!
 * \brief
 *   闪烁屏幕
 *input: onoff--闪烁开关，取值：1表示开，显示原数据，0表示关，清除数据；
 */
#if 0
extern int32 led_flash_screen_inner(uint8 onoff);
#endif
#define led_drv_flash_screen(a)        ((int(*)(bool))LED_FLASH_SCREEN_ENTRY)((a))

#if 0
extern int32 led_wake_up_display_inner(void);
#endif
#define led_drv_wake_up_display()      ((int(*)(void))LED_WAKE_UP_ENTRY)()

#if 0
extern int32 led_sleep_display_inner(void);
#endif
#define led_drv_sleep_display()        ((int(*)(void))LED_SLEEP_ENTRY)()

#if 0
extern int32 led_light_onoff_inner(bool onoff);
#endif
#define led_drv_light_onoff(a)         ((int(*)(bool))LED_LIGHT_ONOFF_ENTRY)((a))

/*! \endcond */

#endif
