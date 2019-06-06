/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LED数码管显示接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#if (SUPPORT_LED_DISPLAY != 0)
static bool led_msg_show_flag = FALSE;
static int8 led_msg_show_timer = -1;
static int8 led_segment_twinkle_timer = -1;
static bool led_segment_twinkle_on = FALSE;
static uint8 led_segment_twinkle_icon_idx;
#endif

void led_module_install(void)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    led_drv_module_install();
#endif
}

void led_module_uninstall(void)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    led_drv_module_uninstall();
#endif
}

void com_led_display(uint8 num_idx, uint8 c, bool display)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
        return;
    }

    //子场景下不允许更新数字区
    if (led_msg_show_flag == TRUE)
    {
        return;
    }

    led_drv_display(num_idx, c, display);
#endif
}

void com_led_display_icon(uint8 icon_idx, bool display)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
        return;
    }

    //子场景下不允许更新:号图标和.号图标
    if (led_msg_show_flag == TRUE)
    {
        if ((icon_idx == LCD_COL) || (icon_idx == LCD_P1))
        {
            return;
        }
    }

    //闪烁的图标设置常亮或常灭需要取消
    if ((led_segment_twinkle_timer != -1) && (icon_idx == led_segment_twinkle_icon_idx))
    {
        com_led_twinkle_icon(0, FALSE, 0);
    }

    led_drv_display_icon(icon_idx, display);
#endif
}

void com_led_clear_screen(uint8 clr_mode)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
        return;
    }

    if (led_msg_show_flag == TRUE)
    {
        uint8 i;

        if (clr_mode == CLR_ALL)
        {
            //清除除了:号和.号图标之外的图标，即主信息区不能被清除，
            //需要保持子场景显示延时时长以确保用户能够准确读到显示信息
            for (i = 0; i < ICON_TOTAL; i++)
            {
                if ((i == LCD_COL) || (i == LCD_P1))
                {
                    continue;
                }

                com_led_display_icon(i, FALSE);
            }
        }
    }
    else
    {
        if (clr_mode == CLR_ALL)
        {
            //闪烁的图标清屏时需要取消
            if (led_segment_twinkle_timer != -1)
            {
                com_led_twinkle_icon(0, FALSE, 0);
            }

            led_drv_clear_screen(LED_CLR_ALL);
        }
        else
        {
            led_drv_clear_screen(LED_CLR_NUM);
            com_led_display_icon(LCD_COL, FALSE);
            com_led_display_icon(LCD_P1, FALSE);
        }
    }
#endif
}

void com_led_flash_screen(bool onoff)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
        return;
    }

    if (led_msg_show_flag == TRUE)
    {
        return;
    }

    led_drv_flash_screen(onoff);
#endif
}

void led_wake_up_display(void)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    led_drv_wake_up_display();
#endif
}

void led_sleep_display(void)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    led_drv_sleep_display();
#endif
}

void led_light_onoff(bool onoff)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
        return;
    }

    led_drv_light_onoff(onoff);
#endif
}

#if (SUPPORT_LED_DISPLAY != 0)
void led_msg_show_timeout_handle(void)
{
    uint16 msg;

    msg = MSG_LED_REDRAW;
    sys_mq_send(MQ_ID_SYS, &msg);

    led_msg_show_flag = FALSE;
    led_msg_show_timer = -1;
}
#endif

void com_led_enter_msg_show(uint16 disp_time)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (disp_time != 0)
    {
        if (led_msg_show_timer != -1)
        {
            kill_app_timer_inner(APP_TIMER_GROUP_COMMON, led_msg_show_timer);
            led_msg_show_timer = -1;
        }

        led_msg_show_timer = set_single_shot_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SYS, \
            APP_TIMER_ATTRB_CONTROL, disp_time, led_msg_show_timeout_handle);

        led_msg_show_flag = TRUE;
    }
#endif
}

bool com_led_force_exit_msg_show(void)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (led_msg_show_flag == TRUE)
    {
        if (led_msg_show_timer != -1)
        {
            kill_app_timer_inner(APP_TIMER_GROUP_COMMON, led_msg_show_timer);
            led_msg_show_timer = -1;
        }

        led_msg_show_flag = FALSE;
        return TRUE;
    }
#endif
    return FALSE;
}

#if (SUPPORT_LED_DISPLAY != 0)
void com_led_segment_twinkle_handle_led0(void)
{
    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (g_app_info_state.ui_display_forbidden == TRUE)
    {
        return;
    }

    if (led_segment_twinkle_on == TRUE)
    {
        led_drv_display_icon(led_segment_twinkle_icon_idx, FALSE);
        led_segment_twinkle_on = FALSE;
    }
    else
    {
        led_drv_display_icon(led_segment_twinkle_icon_idx, TRUE);
        led_segment_twinkle_on = TRUE;
    }
}
#endif

void com_led_twinkle_icon(uint8 icon_idx, bool start, uint16 period_ms)
{
#if (SUPPORT_LED_DISPLAY != 0)
    if (led_segment_twinkle_timer != -1)
    {
        sys_del_irq_timer1(led_segment_twinkle_timer);
        led_segment_twinkle_timer = -1;
    }

    if (g_com_comval.support_led_segment == 0)
    {
        return;
    }

    if (start == TRUE)
    {
        if (period_ms < 20)
        {
            period_ms = 20;
        }
        led_segment_twinkle_on = FALSE;
        led_segment_twinkle_icon_idx = icon_idx;

        if (g_app_info_state.ui_display_forbidden == FALSE)
        {
            com_led_segment_twinkle_handle_led0();
        }

        led_segment_twinkle_timer = sys_set_irq_timer1(com_led_segment_twinkle_handle_led0, period_ms/20);
    }
#endif
}
