/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用LED灯显示处理。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

void wait_link_display(void)
{
    if (display_flag == TRUE)
    {
        led_display(NUMBER2, 'b', 1);
        led_display(NUMBER3, 'L', 1);
        display_flag = FALSE;
    }
    else
    {
        led_display(NUMBER2, 0, FALSE);
        led_display(NUMBER3, 0, FALSE);
        display_flag = TRUE;
    }
}

//快速预览模式会使用该接口来预览，代码硬拷贝到 com_fast_preview_ui.c 里
void bt_show_welcome_ui(void)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);
    led_display(NUMBER2, 'b', TRUE);
    led_display(NUMBER3, 'L', TRUE);
}

void bt_ui_display(bool redraw_all)
{
    bool wait_link = FALSE;
    bool need_display_play_icon = FALSE;
    bool need_display_pause_icon = FALSE;

    if (g_p_global_var->fp_switch_jump_welcome == FALSE)
    {
        if (redraw_all == TRUE)
        {
            led_clear_screen(CLR_ALL);
            if (display_timer_id != -1)
            {
                kill_app_timer(THIS_APP_TIMER_GROUP, display_timer_id);
                display_timer_id = -1;
            }
        }
    }

    switch (g_disp_status)
    {
        case STATUS_NONE:
        libc_printf_info("STATUS_NONE\n");
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            bt_show_welcome_ui();
        }
        break;

        case STATUS_WAIT_PAIRED:
        libc_printf_info("STATUS_WAIT_PAIRED:%d\n", sys_get_ab_timer());
        com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_400MS, TK_SEL_PWM);

        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        wait_link = TRUE;
        break;

        case STATUS_LINKED:
        libc_printf_info("STATUS_LINKED:%d\n", sys_get_ab_timer());
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        break;

        case STATUS_A2DP_PLAY:
        libc_printf_info("STATUS_A2DP_PLAY\n");
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        need_display_play_icon = TRUE;
        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        break;

        case STATUS_A2DP_PAUSE:
        libc_printf_info("STATUS_A2DP_PAUSE\n");
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        need_display_pause_icon = TRUE;
        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        break;

        case STATUS_CALL_INCOMING:
        libc_printf_info("STATUS_CALL_INCOMING\n");
        break;

        case STATUS_CALL_OUTGOING:
        libc_printf_info("STATUS_CALL_OUTGOING\n");
        break;

        case STATUS_CALL_HFP:
        libc_printf_info("STATUS_CALL_HFP\n");
        break;

        case STATUS_CALL_PHONE:
        libc_printf_info("STATUS_CALL_PHONE\n");
        break;

        case STATUS_SIRI:
        libc_printf_info("STATUS_SIRI\n");
        break;

        default:
        break;
    }

    led_display_icon(LCD_PLAY, need_display_play_icon);
    led_display_icon(LCD_PAUSE, need_display_pause_icon);

    if (wait_link == TRUE)
    {
        if (display_timer_id == -1)
        {
            display_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN,\
                APP_TIMER_ATTRB_CONTROL, 500, (timer_proc) wait_link_display);
            display_flag = FALSE;
        }
    }
    else
    {
        if (display_timer_id != -1)
        {
            kill_app_timer(THIS_APP_TIMER_GROUP, display_timer_id);
            display_timer_id = -1;
        }
    }

    g_p_global_var->fp_switch_jump_welcome = FALSE;
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void bt_ui_display_tws(bool redraw_all)
{
    bool need_display_play_icon = FALSE;
    bool need_display_pause_icon = FALSE;

    if (redraw_all == TRUE)
    {
        led_clear_screen(CLR_ALL);
    }

    switch (g_btplay_info.status)
    {
        case BTPLAY_PLAY:
        libc_printf_info("TWS SLAVE PLAY\n");
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        need_display_play_icon = TRUE;
        break;

        case BTPLAY_PAUSE:
        case BTPLAY_STOP:
        libc_printf_info("TWS SLAVE PAUSE\n");
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        need_display_pause_icon = TRUE;
        break;

        default:
        break;
    }

    led_display_icon(LCD_PLAY, need_display_play_icon);
    led_display_icon(LCD_PAUSE, need_display_pause_icon);

    //销毁单箱或TWS主箱场景显示定时器
    if (display_timer_id != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, display_timer_id);
        display_timer_id = -1;
    }
}
#endif
