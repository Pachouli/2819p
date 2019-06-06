/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������� LED IDö�����͡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_LED_ID_H_
#define _COMMON_LED_ID_H_

#include "psp_includes.h"
#include "key_interface.h"

typedef enum
{
    LED_ID_STATUS  = 0,
    LED_ID_MAX,
} led_id_e;

#define MFP_MODE_NOT_PWM   0xFF //��ʾ��PIN����ʹ��PWM����������Ϊ��PINû��PWM���ܣ���������ΪPWM������PINռ����

typedef struct
{
    ic_pad_e pin;
    uint8    pwm_mfp; /* 0xff ��ʾ��PIN����ʹ��PWM����������Ϊ��PINû��PWM���ܣ���������ΪPWM������PINռ���� */
    uint8    pwm_idx; /* PWMX : PWM0 ~ PWM8 */
    uint8    reserve;
} led_pin_cfg_t;

#ifdef USE_LED_PIN_CFG
static const led_pin_cfg_t com_led_pin_cfg[LED_ID_MAX] =
{
    //��PWM���ܵ�PIN������
    //PWM0 : PAD_GPIO0/GPIO11/GPIO13/GPIO18/GPIO32
    //PWM1 : PAD_GPIO1/GPIO20/GPIO33
    //PWM2 : PAD_GPIO3/GPIO21/GPIO34
    //PWM3 : PAD_GPIO4/GPIO22/GPIO35
    //PWM4 : PAD_GPIO5/GPIO23/GPIO36
    //PWM5 : PAD_GPIO6/GPIO24/GPIO37
    //PWM6 : PAD_GPIO8/GPIO25
    //PWM7 : PAD_GPIO9/GPIO26
    //PWM8 : PAD_GPIO10/GPIO12/GPIO19/GPIO48
    {PAD_GPIO20, GPIO20_MFP_PWM1, 1, 0},
};
#endif

#endif
