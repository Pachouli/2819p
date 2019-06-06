

/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *       Wekan       2015-3-26pm
 *******************************************************************************/


#include "led_inner.h"

bool led_sleep_falg = TRUE;

void led_module_set_mfp(void)
{
#if (SUPPORT_LED_DISPLAY == 1)
    pad_phy_para_t phy_para;
    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
#ifdef ENABLE_GPIO_LED_DISPLAY
    phy_para.pad_driver = 1; //驱动能力由IO口决定
    set_pad_function(LED_7SEG_SEG0_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG1_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG2_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG3_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG4_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG5_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG6_PIN, PAD_FUNC_GPIOOUT, &phy_para, 0);
#else
    phy_para.pad_driver = 7; //驱动能力调到最高，最终亮度由LED稳压源控制
    set_pad_function(LED_7SEG_SEG0_PIN, LED_7SEG_SEG0_FUNC, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG1_PIN, LED_7SEG_SEG1_FUNC, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG2_PIN, LED_7SEG_SEG2_FUNC, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG3_PIN, LED_7SEG_SEG3_FUNC, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG4_PIN, LED_7SEG_SEG4_FUNC, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG5_PIN, LED_7SEG_SEG5_FUNC, &phy_para, 0);
    set_pad_function(LED_7SEG_SEG6_PIN, LED_7SEG_SEG6_FUNC, &phy_para, 0);
#endif
#endif
}

/******************************************************************************
 * \par  Description: LED初始化装载
 * \param[in]   none
 * \param[out]  none
 * \return      初始化是否成功
 * \retval      1: 初始化成功
 * \retval      0: 初始化成功
 *   <author>    <time>
 *    Wekan   2015-3-26pm
 *******************************************************************************/

void led_module_hardware_init(void)
{
#ifdef ENABLE_GPIO_LED_DISPLAY
    led_io_phy_para_t temp_phy_para;
    temp_phy_para.common_mode = COMMON_CATHODE_MODE; //暂时只支持 COMMON_CATHODE_MODE 模式
    temp_phy_para.light_level = 1; //4ma
    temp_phy_para.led_pin_num = 7; //暂时只支持7PIN LED数码管
    temp_phy_para.led_io_pin[0] = LED_7SEG_SEG0_PIN;
    temp_phy_para.led_io_pin[1] = LED_7SEG_SEG1_PIN;
    temp_phy_para.led_io_pin[2] = LED_7SEG_SEG2_PIN;
    temp_phy_para.led_io_pin[3] = LED_7SEG_SEG3_PIN;
    temp_phy_para.led_io_pin[4] = LED_7SEG_SEG4_PIN;
    temp_phy_para.led_io_pin[5] = LED_7SEG_SEG5_PIN;
    temp_phy_para.led_io_pin[6] = LED_7SEG_SEG6_PIN;
    hal_led_hardware_init(&temp_phy_para);
#else
    led_phy_para_t temp_phy_para;

    temp_phy_para.common_mode = COMMON_CATHODE_MODE;
    temp_phy_para.light_level = LIGHT_LEVEL_3MA;
    hal_led_hardware_init(&temp_phy_para);
#endif
}

int32 led_module_init_inner(void)
{
    led_module_set_mfp();
    led_module_hardware_init();
    led_sleep_falg = FALSE;//转为非休眠状态
    led_light_flag = TRUE;
    return 0;
}


/******************************************************************************
 * \par  Description:LED卸载
 * \param[in]   none
 * \param[out]  none
 * \return      卸载是否成功
 * \retval      1: 初始化成功
 * \retval      0: 初始化成功
 *   <author>    <time>
 *    Wekan   2015-3-26pm
 *******************************************************************************/
int32 led_module_exit_inner(void)
{
    hal_led_hardware_exit();
    return 0;
}

/******************************************************************************
* \par  Description: 返回唤醒前的状态
* \param[in]   参数没有意义
* \param[out]  none
* \return      FALSE休眠(被唤醒)，TRUE显示(无需唤醒)
*   <author>    <time>
*    Wekan   2015-3-26pm
*******************************************************************************/

int32 led_wake_up_display_inner(void)
{
    if(led_sleep_falg != 0)
    {
        led_module_hardware_init();
        led_sleep_falg = FALSE;//转为非休眠状态

        if (led_light_flag == TRUE)
        {
            hal_led_hardware_display((uint32*) cur_disval);
        }

        return 0;
    }
    return 1;
}

/******************************************************************************
* \par  Description: 使led显示进入休眠状态
* \param[in]   参数没有意义
* \param[out]  none
* \return
*   <author>    <time>
*    Wekan   2015-3-26pm
*******************************************************************************/

int32 led_sleep_display_inner(void)
{
    led_sleep_falg = TRUE;
    hal_led_hardware_exit();
    return 0;
}
