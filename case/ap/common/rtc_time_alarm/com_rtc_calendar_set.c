/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：实现RTC时钟显示功能。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

typedef enum
{
    CALENDAR_SET_YEAR = 0,
    CALENDAR_SET_MONTH,
    CALENDAR_SET_DAY,
    CALENDAR_SET_OK,
} calendar_set_status_e;

static date_t g_rtc_new_date;
static uint8  g_rtc_calendar_set_overtime;
static bool   g_need_update_date_display;
static bool   g_need_update_date_value;
static bool   g_yyyy_mm_dd_display;
static calendar_set_status_e g_rtc_calendar_set_sta;
static uint8  g_number_input_keys; //在数字设置模式下，已输入几个数字按键，中间插入Next等按键会被清0

const key_map_t rtc_calendar_set_key_map_list[] =
{
    /*! 模式切换事件：时钟显示->闹钟设置->时钟设置->万年历设置 */
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_SWITCH_RTC_SCENE },
    /*! 长按 MODE 2秒钟返回原主场景 */
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_EXIT_RTC_TIME_HOLD },

    /*! 以下按键返回原主场景 */
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, MSG_NULL },

    /*! 短按PLAY确定数值 */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RTC_VALUE_CONFORM },

    /*! 调节加 */
    { { KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RTC_VALUE_ADD },
    { { KEY_VADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RTC_VALUE_ADD },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RTC_VALUE_ADD },

    /*! 调节减 */
    { { KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RTC_VALUE_SUB },
    { { KEY_VSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RTC_VALUE_SUB },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RTC_VALUE_SUB },

    /*! 数字按键调节 */
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP }, EVENT_NUMBER_INPUT },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL },
};

const uint8 day_table[12] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static bool is_leap_year(uint16 year)
{
    return ((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0));
}

static uint8 get_days_max(date_t *p_date)
{
    uint8 days_max;

    days_max = day_table[p_date->month - 1];
    if ((is_leap_year(p_date->year) == TRUE) && (p_date->month == 2))
    {
        days_max = 29;
    }

    return days_max;
}

void com_rtc_calendar_set_timer_handle(void)
{
    g_need_update_date_display = TRUE;
    g_rtc_calendar_set_overtime++;

    /*时钟闹钟场景不要关闭LED数码管背光*/
    sys_counter_reset();
}

void com_rtc_time_set_update_date_display(date_t *p_rtc_new_date)
{
    if (g_rtc_calendar_set_sta == CALENDAR_SET_YEAR)
    {
        led_display_icon(LCD_COL,FALSE);
        led_display_icon(LCD_P1,FALSE);
        if (g_yyyy_mm_dd_display == TRUE)
        {
            led_display(0,p_rtc_new_date->year/1000,TRUE);
            led_display(1,p_rtc_new_date->year%1000/100,TRUE);
            led_display(2,p_rtc_new_date->year%100/10,TRUE);
            led_display(3,p_rtc_new_date->year%10,TRUE);
        }
        else
        {
            led_display(0, 0,FALSE);
            led_display(1, 0,FALSE);
            led_display(2, 0,FALSE);
            led_display(3, 0,FALSE);
        }
    }
    else
    {
        led_display_icon(LCD_COL,FALSE);
        led_display_icon(LCD_P1,FALSE);
        if ((g_need_update_date_value == TRUE) || (g_rtc_calendar_set_sta == CALENDAR_SET_MONTH))
        {
            if (g_yyyy_mm_dd_display == TRUE)
            {
                led_display(0,p_rtc_new_date->month/10,TRUE);
                led_display(1,p_rtc_new_date->month%10,TRUE);
            }
            else
            {
                led_display(0, 0,FALSE);
                led_display(1, 0,FALSE);
            }
        }
        if ((g_need_update_date_value == TRUE) || (g_rtc_calendar_set_sta == CALENDAR_SET_DAY))
        {
            if (g_yyyy_mm_dd_display == TRUE)
            {
                led_display(2,p_rtc_new_date->day/10,TRUE);
                led_display(3,p_rtc_new_date->day%10,TRUE);
            }
            else
            {
                led_display(2, 0,FALSE);
                led_display(3, 0,FALSE);
            }
        }
    }

    if (g_yyyy_mm_dd_display == TRUE)
    {
        g_yyyy_mm_dd_display = FALSE;
    }
    else
    {
        g_yyyy_mm_dd_display = TRUE;
    }
    g_need_update_date_value = FALSE;
}

app_result_e com_rtc_calendar_set_inner(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    int8 rtc_calendar_set_timer_id;
    date_t rtc_date_backup;

    g_rtc_calendar_set_sta = CALENDAR_SET_YEAR;
    g_need_update_date_display = FALSE;
    g_need_update_date_value = FALSE;
    g_yyyy_mm_dd_display = FALSE;

    rtc_get_date(&g_rtc_new_date);
    libc_memcpy(&rtc_date_backup, &g_rtc_new_date, sizeof(date_t));

    led_display(0,g_rtc_new_date.year/1000,TRUE);
    led_display(1,g_rtc_new_date.year%1000/100,TRUE);
    led_display_icon(LCD_COL,FALSE);
    led_display_icon(LCD_P1,FALSE);
    led_display(2,g_rtc_new_date.year%100/10,TRUE);
    led_display(3,g_rtc_new_date.year%10,TRUE);

    g_rtc_calendar_set_overtime = 0;

    rtc_calendar_set_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SUB, APP_TIMER_ATTRB_CONTROL, \
        500, com_rtc_calendar_set_timer_handle);

    while (1)
    {
        if (g_need_update_date_display == TRUE)
        {
            com_rtc_time_set_update_date_display(&g_rtc_new_date);

            g_need_update_date_display = FALSE;
        }

        if (g_rtc_calendar_set_overtime >= RTC_TIME_OVERTIME)
        {
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            g_rtc_calendar_set_overtime = 0;

            if (com_key_mapping(&gui_msg, &gui_event, rtc_calendar_set_key_map_list) == TRUE)
            {
                switch (gui_event)
                {
                case EVENT_SWITCH_RTC_SCENE:
                    result = RESULT_SWITCH_RTC_SCENE;
                    break;

                case EVENT_EXIT_RTC_TIME_HOLD:
                    if (com_get_long_key_time_inner() >= 1000) //1S
                    {
                        com_filter_key_hold_inner();
                        goto calendar_set_exit;
                    }
                    break;

                case EVENT_RTC_VALUE_ADD:
                    if (g_rtc_calendar_set_sta == CALENDAR_SET_YEAR)
                    {
                        if ((g_rtc_new_date.year > 2099) || (g_rtc_new_date.year < 2000))
                        {
                            g_rtc_new_date.year = 2099;
                        }

                        if (g_rtc_new_date.year < 2099)
                        {
                            g_rtc_new_date.year++;
                        }
                        else
                        {
                            g_rtc_new_date.year = 2000;
                        }
                    }
                    else if (g_rtc_calendar_set_sta == CALENDAR_SET_MONTH)
                    {
                        if (g_rtc_new_date.month < 12)
                        {
                            g_rtc_new_date.month++;
                        }
                        else
                        {
                            g_rtc_new_date.month = 1;
                        }
                    }
                    else
                    {
                        if (g_rtc_new_date.day < get_days_max(&g_rtc_new_date))
                        {
                            g_rtc_new_date.day++;
                        }
                        else
                        {
                            g_rtc_new_date.day = 1;
                        }
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_calendar_set_timer_id);
                    g_yyyy_mm_dd_display = TRUE;
                    g_need_update_date_value = TRUE;
                    g_need_update_date_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_RTC_VALUE_SUB:
                    if (g_rtc_calendar_set_sta == CALENDAR_SET_YEAR)
                    {
                        if ((g_rtc_new_date.year > 2099) || (g_rtc_new_date.year < 2000))
                        {
                            g_rtc_new_date.year = 2000;
                        }

                        if (g_rtc_new_date.year > 2000)
                        {
                            g_rtc_new_date.year--;
                        }
                        else
                        {
                            g_rtc_new_date.year = 2099;
                        }
                    }
                    else if (g_rtc_calendar_set_sta == CALENDAR_SET_MONTH)
                    {
                        if (g_rtc_new_date.month > 1)
                        {
                            g_rtc_new_date.month--;
                        }
                        else
                        {
                            g_rtc_new_date.month = 12;
                        }
                    }
                    else
                    {
                        if (g_rtc_new_date.day > 1)
                        {
                            g_rtc_new_date.day--;
                        }
                        else
                        {
                            g_rtc_new_date.day = get_days_max(&g_rtc_new_date);
                        }
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_calendar_set_timer_id);
                    g_yyyy_mm_dd_display = TRUE;
                    g_need_update_date_value = TRUE;
                    g_need_update_date_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_RTC_VALUE_CONFORM:
                    if (g_rtc_calendar_set_sta == CALENDAR_SET_YEAR)
                    {
                        if ((g_rtc_new_date.year > 2099) || (g_rtc_new_date.year < 2000))
                        {
                            g_rtc_new_date.year = rtc_date_backup.year;
                        }
                        else
                        {
                            if (g_rtc_new_date.day > get_days_max(&g_rtc_new_date))
                            {
                                g_rtc_new_date.day = get_days_max(&g_rtc_new_date);
                            }
                            g_rtc_calendar_set_sta = CALENDAR_SET_MONTH;
                        }
                    }
                    else if (g_rtc_calendar_set_sta == CALENDAR_SET_MONTH)
                    {
                        if (g_rtc_new_date.month == 0)
                        {
                            g_rtc_new_date.month = rtc_date_backup.month;
                        }
                        else
                        {
                            if (g_rtc_new_date.day > get_days_max(&g_rtc_new_date))
                            {
                                g_rtc_new_date.day = get_days_max(&g_rtc_new_date);
                            }
                            g_rtc_calendar_set_sta = CALENDAR_SET_DAY;
                        }
                    }
                    else
                    {
                        if (g_rtc_new_date.day == 0)
                        {
                            g_rtc_new_date.day = rtc_date_backup.day;
                        }
                        else
                        {
                            g_rtc_calendar_set_sta = CALENDAR_SET_OK;
                        }
                    }

                    //更新备份日历
                    libc_memcpy(&rtc_date_backup, &g_rtc_new_date, sizeof(date_t));

                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_calendar_set_timer_id);
                    g_yyyy_mm_dd_display = TRUE;
                    g_need_update_date_value = TRUE;
                    g_need_update_date_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_NUMBER_INPUT:
                    {
                        uint8 number_input = gui_msg.data.kmsg.val - KEY_NUM0;
                        bool number_input_finish = FALSE;

                        if (g_rtc_calendar_set_sta == CALENDAR_SET_YEAR)
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_new_date.year = number_input;
                                g_number_input_keys++;
                            }
                            else
                            {
                                g_rtc_new_date.year *= 10;
                                g_rtc_new_date.year += number_input;
                                g_number_input_keys++;
                                if (g_number_input_keys == 4)
                                {
                                    g_number_input_keys = 0;
                                    number_input_finish = TRUE;
                                }
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if ((g_rtc_new_date.year > 2099) || (g_rtc_new_date.year < 2000))
                                {
                                    g_rtc_new_date.year = rtc_date_backup.year;
                                }
                            }
                        }
                        else if (g_rtc_calendar_set_sta == CALENDAR_SET_MONTH)
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_new_date.month = number_input;
                                g_number_input_keys = 1;
                            }
                            else
                            {
                                g_rtc_new_date.month *= 10;
                                g_rtc_new_date.month += number_input;
                                g_number_input_keys = 0;
                                number_input_finish = TRUE;
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if ((g_rtc_new_date.month == 0) || (g_rtc_new_date.month > 12))
                                {
                                    g_rtc_new_date.month = rtc_date_backup.month;
                                }
                            }
                        }
                        else
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_new_date.day = number_input;
                                g_number_input_keys = 1;
                            }
                            else
                            {
                                g_rtc_new_date.day *= 10;
                                g_rtc_new_date.day += number_input;
                                g_number_input_keys = 0;
                                number_input_finish = TRUE;
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if ((g_rtc_new_date.day == 0)
                                    || (g_rtc_new_date.day > get_days_max(&g_rtc_new_date)))
                                {
                                    g_rtc_new_date.day = rtc_date_backup.day;
                                }
                            }
                        }

                        restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_calendar_set_timer_id);
                        g_yyyy_mm_dd_display = TRUE;
                        g_need_update_date_value = TRUE;
                        g_need_update_date_display = TRUE;
                    }
                    break;

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

        if (g_rtc_calendar_set_sta == CALENDAR_SET_OK)
        {
            rtc_set_date(&g_rtc_new_date);
            result = RESULT_BACK_RTC_TIME;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    calendar_set_exit:

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, rtc_calendar_set_timer_id);
    rtc_calendar_set_timer_id = -1;

    return result;
}

