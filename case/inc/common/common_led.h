/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：分立 LED 显示功能相关的宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_LED_H_
#define _COMMON_LED_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "common_led_id.h"

typedef enum
{
	LED_SCENE_MAIN = 0,
    LED_SCENE_SUB  = 1,
	LED_SCENE_MAX,
} led_scene_e;

#define TK_SEL_PWM   0 //使用PWM闪灯
#define TK_SEL_GPIO  1 //使用硬件定时器驱动GPIO闪灯

#if 0
extern void com_led_twinkle_init_inner(led_id_e led_id, led_scene_e led_scene, pwm_period_e period, uint8 pwm_gpio_sel);
#endif
#define com_led_twinkle_init(a,b,c,d)    ((void(*)(led_id_e , led_scene_e , pwm_period_e , uint8 ))COM_COM_LED_TWINKLE_INIT)((a),(b),(c),(d))

#if 0
extern void com_led_light_on_inner(led_id_e led_id, led_scene_e led_scene);
#endif
#define com_led_light_on(a,b)            ((void(*)(led_id_e , led_scene_e))COM_COM_LED_LIGHT_ON)((a),(b))

#if 0
extern void com_led_light_off_inner(led_id_e led_id, led_scene_e led_scene);
#endif
#define com_led_light_off(a,b)           ((void(*)(led_id_e , led_scene_e))COM_COM_LED_LIGHT_OFF)((a),(b))

#if 0
extern void com_led_light_dummy_inner(led_id_e led_id, led_scene_e led_scene);
#endif
#define com_led_light_dummy(a,b)         ((void(*)(led_id_e , led_scene_e))COM_COM_LED_LIGHT_DUMMY)((a),(b))

#if 0
extern void com_led_breath_inner(led_id_e led_id, led_scene_e led_scene);
#endif
#define com_led_breath(a,b)              ((void(*)(led_id_e , led_scene_e))COM_COM_LED_BREATH)((a),(b))

#if 0
extern void com_led_display(uint8 num_idx, uint8 c, bool display);
#endif
#define led_display(a,b,c)               ((void(*)(uint8, uint8, bool))COM_COM_LED_DISPLAY)((a),(b),(c))

#if 0
extern void com_led_display_icon(uint8 icon_idx, bool display);
#endif
#define led_display_icon(a,b)            ((void(*)(uint8, bool))COM_COM_LED_DISPLAY_ICON)((a),(b))

#define CLR_ALL  0 //清除全部数字和图标
#define CLR_MAIN 1 //仅清除数字和:号.号图标
#if 0
extern void com_led_clear_screen(uint8 clr_mode);
#endif
#define led_clear_screen(a)              ((void(*)(uint8))COM_COM_LED_CLEAR_SCREEN)((a))

#if 0
extern void com_led_flash_screen(uint8 onoff);
#endif
#define led_flash_screen(a)              ((void(*)(bool))COM_COM_LED_FLASH_SCREEN)((a))

#if 0
extern void com_led_enter_msg_show(uint16 disp_time);
#endif
#define led_enter_msg_show(a)            ((void(*)(uint16))COM_COM_LED_ENTER_MSG_SHOW)((a))

#if 0
extern bool com_led_force_exit_msg_show(void);
#endif
#define led_force_exit_msg_show()        ((bool(*)(void))COM_COM_LED_FORCE_EXIT_MSG_SHOW)()

#if 0
extern void com_led_twinkle_icon(uint8 icon_idx, bool start, uint16 period_ms);
#endif
#define led_twinkle_icon(a,b,c)          ((void(*)(uint8, bool, uint16))COM_COM_LED_TWINKLE_ICON)((a),(b),(c))

#endif
