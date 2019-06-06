/*!
 * \file      seg_led.c
 * \brief     数码管显示接口
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#include "upgrade_app.h"
#include "upgrade_seg_led.h"

#include <../HAL/include/hal_datatype_inner.h>


#if (SUPPORT_LED_DISPLAY == 1)

enum
{
    _1A = 0x00, _1B, _1C, _1D, _1E, _1F, _1G,
    _2A = 0x10, _2B, _2C, _2D, _2E, _2F, _2G,
    _3A = 0x20, _3B, _3C, _3D, _3E, _3F, _3G,
    _4A = 0x30, _4B, _4C, _4D, _4E, _4F, _4G,
    _K1 = 0x40, _K2, _K3, _K4, _K5, _K6, _K7, _K8,
    _NA = 0xFF,
};


const u8_t  seg_led_map_table[7][7] =
{
    /* BYTE_6 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ BYTE_0 */
    { _NA, _4A, _K8, _NA, _K6, _NA, _NA, },  // BIT_6
    { _4B, _NA, _4F, _4D, _3E, _K3, _3D, },  // BIT_5
    { _4G, _4C, _NA, _3A, _3G, _2C, _1D, },  // BIT_4
    { _NA, _4E, _3C, _NA, _3F, _2G, _1C, },  // BIT_3
    { _K7, _K2, _3B, _K5, _NA, _2F, _1G, },  // BIT_2
    { _NA, _2D, _2E, _2B, _2A, _NA, _1F, },  // BIT_1
    { _NA, _K1, _K4, _1E, _1B, _1A, _NA, },  // BIT_0
};


typedef struct
{
    u8_t  gpio_no;
    u8_t  mfp_sel;
    
} seg_led_pin_t;


const seg_led_pin_t  seg_led_pins[] =
{
    { LED_7SEG_SEG0_PIN - PAD_GPIO0, LED_7SEG_SEG0_FUNC - PAD_FUNC_MFP, },
    { LED_7SEG_SEG1_PIN - PAD_GPIO0, LED_7SEG_SEG1_FUNC - PAD_FUNC_MFP, },
    { LED_7SEG_SEG2_PIN - PAD_GPIO0, LED_7SEG_SEG2_FUNC - PAD_FUNC_MFP, },
    { LED_7SEG_SEG3_PIN - PAD_GPIO0, LED_7SEG_SEG3_FUNC - PAD_FUNC_MFP, },
    { LED_7SEG_SEG4_PIN - PAD_GPIO0, LED_7SEG_SEG4_FUNC - PAD_FUNC_MFP, },
    { LED_7SEG_SEG5_PIN - PAD_GPIO0, LED_7SEG_SEG5_FUNC - PAD_FUNC_MFP, },
	{ LED_7SEG_SEG6_PIN - PAD_GPIO0, LED_7SEG_SEG6_FUNC - PAD_FUNC_MFP, },
};


void seg_led_setup_pins(void)
{
    int  i;

    for (i = 0; i < 7; i++)
    {
        u32_t  reg = GPIO0_CTL + seg_led_pins[i].gpio_no * 4;
        
        u32_t  gpio_ctl = __reg32(reg);

        gpio_ctl &= ~(
            (1 << GPIO0_CTL_GPIOINEN)  |
            (1 << GPIO0_CTL_GPIOOUTEN) |
            (GPIO0_CTL_MFP_MASK));

        gpio_ctl |= (seg_led_pins[i].mfp_sel << GPIO0_CTL_MFP_SHIFT);

        __reg32(reg) = gpio_ctl;
    }
}


void seg_led_update(u32_t* buf)
{
    __reg32(SEG_DISP_DATA0) = buf[0];
    __reg32(SEG_DISP_DATA1) = buf[1];
    
    __reg32(SEG_DISP_CTL) |= (1 << SEG_DISP_CTL_REFRSH);

    while (__reg32(SEG_DISP_CTL) & (1 << SEG_DISP_CTL_REFRSH))
        ;
}


/* 数字显示
 */
const u8_t  seg_led_number[] =
{
    SEG_LED_CHAR_0,
    SEG_LED_CHAR_1,
    SEG_LED_CHAR_2,
    SEG_LED_CHAR_3,
    SEG_LED_CHAR_4,
    SEG_LED_CHAR_5,
    SEG_LED_CHAR_6,
    SEG_LED_CHAR_7,
    SEG_LED_CHAR_8,
    SEG_LED_CHAR_9,
};


/* 数码管显示
 */
void seg_led_display(u8_t* data)
{
    u32_t  disp_buf[2];
    
    u8_t*  pbuf = (u8_t*)disp_buf;

    int    byte_index;
    int    bit_index;

    disp_buf[0] = 0;
    disp_buf[1] = 0;

    for (byte_index = 0; byte_index < 7; byte_index++)
    {
        for (bit_index = 0; bit_index < 7; bit_index++)
        {
            u8_t  map_val = seg_led_map_table[6 - bit_index][6 - byte_index];

            if (map_val == _NA)
                continue;

            if (data[map_val >> 4] & (1 << (map_val & 0xF)))
            {
                pbuf[byte_index] |= (1 << bit_index);
            }
        }
    }

    seg_led_setup_pins();

    seg_led_update(disp_buf);
}

#endif  // SUPPORT_LED_DISPLAY


