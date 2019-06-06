
/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*     brief    led驱动总接口定义;供驱动安装时使用;
*      <author>       <time>
*       Wekan       2015-3-26pm
*******************************************************************************/


#include "led_inner.h"

typedef void*(*led_op_func)(void*, void*, void*);

typedef struct
{
    led_op_func sdrv_led_module_install;
    led_op_func sdrv_led_module_uninstall;
    led_op_func sdrv_led_display;//display
    led_op_func sdrv_led_display_icon;//display icon
    led_op_func sdrv_clear_screen;//clear
    led_op_func sdrv_flash_screen;
    led_op_func sdrv_wake_up_LED_display;  //wake
    led_op_func sdrv_sleep_LED_display;    //sleep
    led_op_func sdrv_led_light_onoff; //led light
} led_driver_operations;



const led_driver_operations led_driver_op =
{
    (led_op_func) led_module_init_inner,
    (led_op_func) led_module_exit_inner,
    (led_op_func) led_display_inner, //display
    (led_op_func) led_display_icon_inner, //display icon
    (led_op_func) led_clear_screen_inner, //clear
    (led_op_func) led_flash_screen_inner ,//flush
    (led_op_func) led_wake_up_display_inner,  //wake
    (led_op_func) led_sleep_display_inner,    //sleep
    (led_op_func) led_light_onoff_inner, //led light
};

