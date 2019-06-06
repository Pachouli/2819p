/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*     brief    led显示，供AP调用
*      <author>       <time>
*       Wekan       2015-3-26pm
*******************************************************************************/


#include "led_inner.h"

/******************************************************************************
* \par  Description: 闪烁屏幕
* \param[in]   onoff --1表示显示原数据，0表示清除原数据
* \param[out]  none
* \return      none
* \note:闪烁必须首先调用关闭，然后调用显示原数据
*   <author>    <time>
*    Wekan   2015-3-26pm
*******************************************************************************/

int32 led_flash_screen_inner(uint8 onoff)
{
#if (SUPPORT_LED_DISPLAY == 1)
    if (0 == onoff)
    {
        uint32 seg_disp_data[2];
        seg_disp_data[0] = 0;
        seg_disp_data[1] = 0;

        if (led_light_flag == TRUE)
        {
            hal_led_hardware_display(seg_disp_data);
        }
    }
    else
    {
        if (led_light_flag == TRUE)
        {
            hal_led_hardware_display((uint32*) cur_disval);
        }
    }

    return 0;
#elif (SUPPORT_LED_DISPLAY == 2)
    if (0 == onoff)
    {
        icon_map = 0;
        libc_memset(digits,' ',4);
        sim_led_hardware_display();
    }
    else
    {
        icon_map = bak_icon_map;
        libc_memcpy(digits,bak_digits,4);
        sim_led_hardware_display();
    }
    return 0;
#endif
}
