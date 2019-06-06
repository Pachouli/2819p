/*!
 * \file      led_light.c
 * \brief     LED 灯控制接口
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#include "upgrade_app.h"

#include <../HAL/include/hal_datatype_inner.h>


#if (SUPPORT_LED_LIGHT == 1)

#define LED_LIGHT_PIN_GPIO  PAD_GPIO20
#define LED_LIGHT_PIN_FUNC  GPIO20_MFP_GPIO20


/* LED 灯控制
 */
void led_light_ctrl(bool on_off)
{
    uint_t  gpio_no = LED_LIGHT_PIN_GPIO - PAD_GPIO0;
    uint_t  mfp_sel = LED_LIGHT_PIN_FUNC - PAD_FUNC_MFP;
    
    uint_t  reg = (gpio_no < 32) ? GPIO_ODAT0 : GPIO_ODAT1;
    uint_t  bit = (gpio_no < 32) ? gpio_no : (gpio_no - 32);
    
    u32_t   gpio_ctl;

    if (on_off)
        __reg32(reg) |= (1 << bit);
    else
        __reg32(reg) &= ~(1 << bit);

    reg = (GPIO0_CTL + gpio_no * 4);
    
    gpio_ctl = __reg32(reg);

    gpio_ctl &= ~(
        (1 << GPIO0_CTL_GPIOINEN)  |
        (GPIO0_CTL_MFP_MASK));

    gpio_ctl |= (
        (1 << GPIO0_CTL_GPIOOUTEN) |
        (mfp_sel << GPIO0_CTL_MFP_SHIFT));

    __reg32(reg) = gpio_ctl;
}

#endif  // SUPPORT_LED_LIGHT


