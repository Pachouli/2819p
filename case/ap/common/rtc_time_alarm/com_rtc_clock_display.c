/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：实现RTC时钟显示功能。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

static time_t g_rtc_time;
static bool   g_need_update_time;
static bool   g_ampm_display_flag;
static bool   g_hh_mm_col_display;
static uint8  g_rtc_time_disaplay_overtime;

const key_map_t rtc_clock_key_map_list[] =
{
    /*! 模式切换事件：时钟显示->闹钟设置->时钟设置->万年历设置 */
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_SWITCH_RTC_SCENE },
    /*! 长按 MODE 2秒钟返回原主场景 */
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_EXIT_RTC_TIME_HOLD },

    /*! 以下按键返回原主场景 */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_VADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD | KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_VSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD | KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD | KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD | KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP }, EVENT_EXIT_RTC_TIME },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL },
};

void com_rtc_clock_display_timer_handle(void)
{
    g_need_update_time = TRUE;
    g_rtc_time_disaplay_overtime++;

    /*时钟闹钟场景不要关闭LED数码管背光*/
    sys_counter_reset();
}

static uint8 get_ampm_hour(uint8 hour)
{
    uint8 ampm_hour;

    if (hour == 0)
    {
        ampm_hour = 12;
    }
    else if (hour <= 12)
    {
        ampm_hour = hour;
    }
    else
    {
        ampm_hour = hour - 12;
    }

    return ampm_hour;
}

app_result_e com_rtc_clock_display_inner(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    time_t rtc_time_backup;
    int8 rtc_clock_display_timer_id;

    rtc_get_time(&g_rtc_time);
    libc_memcpy(&rtc_time_backup, &g_rtc_time, sizeof(time_t));

    g_ampm_display_flag = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_TIME_AMPM_ENABLE);

    com_led_force_exit_msg_show();

    if (g_ampm_display_flag == TRUE)
    {
        uint8 ampm_hour = get_ampm_hour(g_rtc_time.hour);
        led_display(0,ampm_hour/10,TRUE);
        led_display(1,ampm_hour%10,TRUE);
    }
    else
    {
        led_display(0,g_rtc_time.hour/10,TRUE);
        led_display(1,g_rtc_time.hour%10,TRUE);
    }
    led_display_icon(LCD_COL,TRUE);
    led_display_icon(LCD_P1,FALSE);
    led_display(2,g_rtc_time.minute/10,TRUE);
    led_display(3,g_rtc_time.minute%10,TRUE);

    g_rtc_time_disaplay_overtime = 0;
    g_need_update_time = FALSE;
    g_hh_mm_col_display = FALSE;

    rtc_clock_display_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SUB, APP_TIMER_ATTRB_CONTROL, \
        500, com_rtc_clock_display_timer_handle);

    while (1)
    {
        if (g_need_update_time == TRUE)
        {
            rtc_get_time(&g_rtc_time);

            if (g_rtc_time.hour != rtc_time_backup.hour)
            {
                if (g_ampm_display_flag == TRUE)
                {
                    uint8 ampm_hour = get_ampm_hour(g_rtc_time.hour);
                    led_display(0,ampm_hour/10,TRUE);
                    led_display(1,ampm_hour%10,TRUE);
                }
                else
                {
                    led_display(0,g_rtc_time.hour/10,TRUE);
                    led_display(1,g_rtc_time.hour%10,TRUE);
                }
                rtc_time_backup.hour = g_rtc_time.hour;
            }
            led_display_icon(LCD_COL, g_hh_mm_col_display);
            if (g_rtc_time.minute != rtc_time_backup.minute)
            {
                led_display(2,g_rtc_time.minute/10,TRUE);
                led_display(3,g_rtc_time.minute%10,TRUE);
                rtc_time_backup.minute = g_rtc_time.minute;
            }

            if (g_hh_mm_col_display == TRUE)
            {
                g_hh_mm_col_display = FALSE;
            }
            else
            {
                g_hh_mm_col_display = TRUE;
            }
            g_need_update_time = FALSE;
        }

        if (g_rtc_time_disaplay_overtime >= RTC_TIME_OVERTIME)
        {
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            g_rtc_time_disaplay_overtime = 0;

            if (com_key_mapping(&gui_msg, &gui_event, rtc_clock_key_map_list) == TRUE)
            {
                switch (gui_event)
                {
                case EVENT_SWITCH_RTC_SCENE:
                    result = RESULT_SWITCH_RTC_SCENE;
                    break;

                case EVENT_EXIT_RTC_TIME_HOLD:
                    if (com_get_long_key_time_inner() >= 1000) //1S
                    {
                        com_filter_key_hold_inner(); //过滤后续short up消息
                        goto clock_display_exit;
                    }
                    break;

                case EVENT_EXIT_RTC_TIME:
                    com_filter_key_hold_inner(); //过滤后续short up消息
                    goto clock_display_exit;

                default:
                    result = com_message_box(gui_event);
                    break;
                }
            }
        }
        else
        {
            if (g_app_msg_dispatch_handle != NULL)
            {
                if (get_app_msg(APP_TYPE_GUI, &pri_msg) == TRUE)
                {
                    result = g_app_msg_dispatch_handle(&pri_msg);
                }
            }
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    clock_display_exit:

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, rtc_clock_display_timer_id);
    rtc_clock_display_timer_id = -1;

    return result;
}

app_result_e com_rtc_time_entry_inner(void)
{
    app_result_e result = RESULT_NULL;
    rtc_scene_e cur_rtc_scene = RTC_TIME_DISPLAY;

    change_app_timer_tag(APP_TIMER_TAG_SUB);

    while (1)
    {
        switch (cur_rtc_scene)
        {
        case RTC_TIME_DISPLAY:
            result = com_rtc_clock_display_inner();
            break;

        case RTC_ALARM_SET:
            result = com_rtc_alarm_set_inner();
            break;

        case RTC_TIME_SET:
            result = com_rtc_time_set_inner();
            break;

        case RTC_CALENDAR_SET:
            result = com_rtc_calendar_set_inner();
            break;

        default:
            result = RESULT_NULL;
            break;
        }

        if (result == RESULT_BACK_RTC_TIME)
        {
            cur_rtc_scene = RTC_TIME_DISPLAY;
        }
        else if (result == RESULT_SWITCH_RTC_SCENE)
        {
            cur_rtc_scene++;
            if (cur_rtc_scene >= RTC_SCENE_MAX)
            {
                cur_rtc_scene = 0;
            }
        }
        else
        {
            break;
        }
    }

    change_app_timer_tag(APP_TIMER_TAG_MAIN);

    if (result == RESULT_NULL)
    {
        result = RESULT_REDRAW;
    }

    return result;
}

