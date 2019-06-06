
/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>           
 *       Wekan       2015-3-26pm            
 *******************************************************************************/

#ifndef _LED_INNER_H
#define _LED_INNER_H

#include "psp_includes.h"
#include "led_interface.h"

//#define NUMBER1_DISP_1 //数码1只能显示数字1

extern bool led_light_flag;
extern uint8 cur_disval[8];
extern bool led_sleep_falg;

extern int32 led_module_init_inner(void);
extern int32 led_module_exit_inner(void);
extern int32 led_wake_up_display_inner(void);
extern int32 led_sleep_display_inner(void);
extern int32 led_clear_screen_inner(uint8 clr_mode);
extern int32 led_flash_screen_inner(uint8 onoff);
extern int32 led_light_onoff_inner(bool onoff);

extern int32 led_display_inner(uint8 num_idx, uint8 c, bool display);
extern int32 led_display_icon_inner(uint8 icon_idx, bool display);

#if (SUPPORT_LED_DISPLAY == 2)
extern uint8 icon_map;
extern uint8 digits[4];
extern uint8 bak_icon_map;
extern uint8 bak_digits[4];
extern void sim_led_hardware_display(void);
#endif

#endif //_LCD_INNER_H
