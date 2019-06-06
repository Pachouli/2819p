/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用播放场景主循环，包括初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_show_frequency(uint32 freq, bool high_number_show)
 * \播放场景下的频率图片刷新
 * \param[in]     none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_show_frequency(uint32 freq, bool high_number_show)
{
    libc_printf_info("FM Freq : %d\n", freq);

    led_display_icon(LCD_COL, FALSE);

    if (high_number_show == TRUE)
    {
        led_display(NUMBER1, freq/100000, TRUE);
    }
    else
    {
        if (freq >= 100000)
        {
            led_display(NUMBER1, 1, TRUE);
        }
        else
        {
            led_display(NUMBER1, 0, FALSE);
        }
    }
    freq = freq % 100000;
    led_display(NUMBER2, freq/10000, TRUE);
    freq = freq % 10000;
    led_display(NUMBER3, freq/1000, TRUE);
    led_display_icon(LCD_P1, TRUE);
    freq = freq % 1000;
    led_display(NUMBER4, freq/100, TRUE);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_show_station_num(void)
 * \播放场景下的电台号刷新
 * \param[in]     none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note    调用此函数时，需确保g_radio_config.FMStatus.station
 */
/*******************************************************************************/
void radio_show_station_num(uint8 station_num, bool msg_show)
{
    libc_printf_info("FM CHAN : %d\n", station_num);

    led_display_icon(LCD_COL, FALSE);
    led_display_icon(LCD_P1, FALSE);
    //CH
    led_display(NUMBER1, 'C', TRUE);
    led_display(NUMBER2, 'H', TRUE);
    //电台号
    led_display(NUMBER3, station_num/10, TRUE);
    led_display(NUMBER4, station_num%10, TRUE);

    if (msg_show == TRUE)
    {
        led_enter_msg_show(3000);
    }
}

