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
    TIME_SET_HOUR = 0,
    TIME_SET_MIN,
    TIME_SET_OK,
} time_set_status_e;

static time_t g_rtc_new_time;
static uint8  g_rtc_time_set_overtime;
static bool   g_need_update_clock_display;
static bool   g_need_update_clock_value;
static bool   g_hh_mm_display;
static time_set_status_e g_rtc_time_set_sta;
static uint8  g_number_input_keys; //在数字设置模式下，已输入几个数字按键，中间插入Next等按键会被清0

const key_map_t rtc_time_set_key_map_list[] =
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

void com_rtc_time_set_timer_handle(void)
{
    g_need_update_clock_display = TRUE;
    g_rtc_time_set_overtime++;

    /*时钟闹钟场景不要关闭LED数码管背光*/
    sys_counter_reset();
}

void com_rtc_time_set_update_clock_display(time_t *p_rtc_new_time)
{
    if ((g_need_update_clock_value == TRUE) || (g_rtc_time_set_sta == TIME_SET_HOUR))
    {
        if (g_hh_mm_display == TRUE)
        {
            led_display(0,p_rtc_new_time->hour/10,TRUE);
            led_display(1,p_rtc_new_time->hour%10,TRUE);
        }
        else
        {
            led_display(0, 0,FALSE);
            led_display(1, 0,FALSE);
        }
    }
    if ((g_need_update_clock_value == TRUE) || (g_rtc_time_set_sta == TIME_SET_MIN))
    {
        if (g_hh_mm_display == TRUE)
        {
            led_display(2,p_rtc_new_time->minute/10,TRUE);
            led_display(3,p_rtc_new_time->minute%10,TRUE);
        }
        else
        {
            led_display(2, 0,FALSE);
            led_display(3, 0,FALSE);
        }
    }

    if (g_hh_mm_display == TRUE)
    {
        g_hh_mm_display = FALSE;
    }
    else
    {
        g_hh_mm_display = TRUE;
    }
    g_need_update_clock_value = FALSE;
}

app_result_e com_rtc_time_set_inner(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    int8 rtc_time_set_timer_id;
    time_t rtc_time_backup;

    g_rtc_time_set_sta = TIME_SET_HOUR;
    g_need_update_clock_display = FALSE;
    g_need_update_clock_value = FALSE;
    g_hh_mm_display = FALSE;

    rtc_get_time(&g_rtc_new_time);
    libc_memcpy(&rtc_time_backup, &g_rtc_new_time, sizeof(time_t));
    g_rtc_new_time.second = 0;

    led_display(0,g_rtc_new_time.hour/10,TRUE);
    led_display(1,g_rtc_new_time.hour%10,TRUE);
    led_display_icon(LCD_COL,TRUE);
    led_display_icon(LCD_P1,FALSE);
    led_display(2,g_rtc_new_time.minute/10,TRUE);
    led_display(3,g_rtc_new_time.minute%10,TRUE);

    g_rtc_time_set_overtime = 0;

    rtc_time_set_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SUB, APP_TIMER_ATTRB_CONTROL, \
        500, com_rtc_time_set_timer_handle);

    while (1)
    {
        if (g_need_update_clock_display == TRUE)
        {
            com_rtc_time_set_update_clock_display(&g_rtc_new_time);

            g_need_update_clock_display = FALSE;
        }

        if (g_rtc_time_set_overtime >= RTC_TIME_OVERTIME)
        {
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            g_rtc_time_set_overtime = 0;

            if (com_key_mapping(&gui_msg, &gui_event, rtc_time_set_key_map_list) == TRUE)
            {
                switch (gui_event)
                {
                case EVENT_SWITCH_RTC_SCENE:
                    result = RESULT_SWITCH_RTC_SCENE;
                    break;

                case EVENT_EXIT_RTC_TIME:
                    if (com_get_long_key_time_inner() >= 1000) //1S
                    {
                        com_filter_key_hold_inner();
                        goto time_set_exit;
                    }
                    break;

                case EVENT_RTC_VALUE_ADD:
                    if (g_rtc_time_set_sta == TIME_SET_HOUR)
                    {
                        if (g_rtc_new_time.hour < 23)
                        {
                            g_rtc_new_time.hour++;
                        }
                        else
                        {
                            g_rtc_new_time.hour = 0;
                        }
                    }
                    else
                    {
                        if (g_rtc_new_time.minute < 59)
                        {
                            g_rtc_new_time.minute++;
                        }
                        else
                        {
                            g_rtc_new_time.minute = 0;
                        }
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_time_set_timer_id);
                    g_hh_mm_display = TRUE;
                    g_need_update_clock_value = TRUE;
                    g_need_update_clock_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_RTC_VALUE_SUB:
                    if (g_rtc_time_set_sta == TIME_SET_HOUR)
                    {
                        if (g_rtc_new_time.hour > 0)
                        {
                            g_rtc_new_time.hour--;
                        }
                        else
                        {
                            g_rtc_new_time.hour = 23;
                        }
                    }
                    else
                    {
                        if (g_rtc_new_time.minute > 0)
                        {
                            g_rtc_new_time.minute--;
                        }
                        else
                        {
                            g_rtc_new_time.minute = 59;
                        }
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_time_set_timer_id);
                    g_hh_mm_display = TRUE;
                    g_need_update_clock_value = TRUE;
                    g_need_update_clock_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_RTC_VALUE_CONFORM:
                    if (g_rtc_time_set_sta == TIME_SET_HOUR)
                    {
                        g_rtc_time_set_sta = TIME_SET_MIN;
                    }
                    else
                    {
                        g_rtc_time_set_sta = TIME_SET_OK;
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_time_set_timer_id);
                    g_hh_mm_display = TRUE;
                    g_need_update_clock_value = TRUE;
                    g_need_update_clock_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_NUMBER_INPUT:
                    {
                        uint8 number_input = gui_msg.data.kmsg.val - KEY_NUM0;
                        bool number_input_finish = FALSE;

                        if (g_rtc_time_set_sta == TIME_SET_HOUR)
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_new_time.hour = number_input;
                                g_number_input_keys = 1;
                            }
                            else
                            {
                                g_rtc_new_time.hour *= 10;
                                g_rtc_new_time.hour += number_input;
                                g_number_input_keys = 0;
                                number_input_finish = TRUE;
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if (g_rtc_new_time.hour > 23)
                                {
                                    g_rtc_new_time.hour = rtc_time_backup.hour;
                                }
                            }
                        }
                        else
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_new_time.minute = number_input;
                                g_number_input_keys = 1;
                            }
                            else
                            {
                                g_rtc_new_time.minute *= 10;
                                g_rtc_new_time.minute += number_input;
                                g_number_input_keys = 0;
                                number_input_finish = TRUE;
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if (g_rtc_new_time.minute > 59)
                                {
                                    g_rtc_new_time.minute = rtc_time_backup.minute;
                                }
                            }
                        }

                        restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_time_set_timer_id);
                        g_hh_mm_display = TRUE;
                        g_need_update_clock_value = TRUE;
                        g_need_update_clock_display = TRUE;
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

        if (g_rtc_time_set_sta == TIME_SET_OK)
        {
            rtc_set_time(&g_rtc_new_time);
            result = RESULT_BACK_RTC_TIME;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    time_set_exit:

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, rtc_time_set_timer_id);
    rtc_time_set_timer_id = -1;

    return result;
}

