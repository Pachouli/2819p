
/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*     brief    清除屏幕，供AP调用
*      <author>       <time>
*       Wekan       2015-3-26pm
*******************************************************************************/


#include "led_inner.h"


/******************************************************************************
* \par  Description: 灭掉所有segment
* \param[in]   none
* \param[out]  none
* \return      none
*   <author>    <time>
*    Wekan   2015-3-26pm
*******************************************************************************/

int32 led_clear_screen_inner(uint8 clr_mode)
{
#if (SUPPORT_LED_DISPLAY == 1)
    uint8 i = 0;
    //clear data buffer can't use sys interface
    if (clr_mode == LED_CLR_ALL)
    {
	    for(i = 0; i < sizeof(cur_disval); i++)
	    {
	        cur_disval[i] = 0;
	    }
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			led_display_inner(NUMBER1+i, 0, FALSE);
		}
	}
    //send data to led segment
    if (led_light_flag == TRUE)
    {
        hal_led_hardware_display((uint32*) cur_disval);
    }
    return 0;
#elif (SUPPORT_LED_DISPLAY == 2)
    if (clr_mode == LED_CLR_ALL)
    {
	    bak_icon_map = icon_map = 0;
	    libc_memset(digits,' ',4);
	    libc_memset(bak_digits,' ',4);
	}
	else
	{
	    libc_memset(digits,' ',4);
	    libc_memset(bak_digits,' ',4);
	}
    sim_led_hardware_display();
    return 0;
#endif
}

int32 led_light_onoff_inner(bool onoff)
{
    if ((onoff == FALSE) && (led_light_flag == TRUE))
    {
#if (SUPPORT_LED_DISPLAY == 1)
        uint32 seg_disp_data[2];
        seg_disp_data[0] = 0;
        seg_disp_data[1] = 0;
        hal_led_hardware_display((uint32*) seg_disp_data);
#else
        led_clear_screen_inner(LED_CLR_ALL);
#endif
        led_light_flag = FALSE;
    }
    else if ((onoff == TRUE) && (led_light_flag == FALSE))
    {
        led_light_flag = TRUE;
#if (SUPPORT_LED_DISPLAY == 1)
        hal_led_hardware_display((uint32*) cur_disval);
#endif
    }

    return 0;
}

