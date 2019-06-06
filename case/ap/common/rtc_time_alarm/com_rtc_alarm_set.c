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
    ALARM_SET_ONOFF = 0,
    ALARM_SET_HOUR,
    ALARM_SET_MIN,
    ALARM_SET_OK,
} alarm_set_status_e;

static alarm_info_t g_rtc_alarm_info;
static uint8  g_rtc_alarm_set_overtime;
static bool   g_need_update_alarm_display;
static bool   g_need_update_alarm_value;
static bool   g_hh_mm_display;
static alarm_set_status_e g_rtc_alarm_set_sta;
static uint8  g_number_input_keys; //在数字设置模式下，已输入几个数字按键，中间插入Next等按键会被清0
static uint8  g_alarm_setting_idx;
static time_t g_alarm_time_backup;

const key_map_t rtc_alarm_set_key_map_list[] =
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

void com_rtc_alarm_set_timer_handle(void)
{
    g_need_update_alarm_display = TRUE;
    g_rtc_alarm_set_overtime++;

    /*时钟闹钟场景不要关闭LED数码管背光*/
    sys_counter_reset();
}

void com_rtc_time_set_update_alarm_display(time_t *p_rtc_new_alarm)
{
    if (g_rtc_alarm_set_sta == ALARM_SET_ONOFF)
    {
        led_display_icon(LCD_COL,FALSE);
        led_display_icon(LCD_P1,FALSE);
        if (g_hh_mm_display == TRUE)
        {
            if (g_rtc_alarm_info.alarm_onoff == TRUE)
            {
                led_display(0,0,FALSE);
                led_display(1,'O',TRUE);
                led_display(2,'N',TRUE);
                led_display(3,0,FALSE);
            }
            else
            {
                led_display(0,0,FALSE);
                led_display(1,'O',TRUE);
                led_display(2,'F',TRUE);
                led_display(3,'F',TRUE);
            }
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
        led_display_icon(LCD_COL,TRUE);
        led_display_icon(LCD_P1,FALSE);
        if ((g_need_update_alarm_value == TRUE) || (g_rtc_alarm_set_sta == ALARM_SET_HOUR))
        {
            if (g_hh_mm_display == TRUE)
            {
                led_display(0,p_rtc_new_alarm->hour/10,TRUE);
                led_display(1,p_rtc_new_alarm->hour%10,TRUE);
            }
            else
            {
                led_display(0, 0,FALSE);
                led_display(1, 0,FALSE);
            }
        }
        if ((g_need_update_alarm_value == TRUE) || (g_rtc_alarm_set_sta == ALARM_SET_MIN))
        {
            if (g_hh_mm_display == TRUE)
            {
                led_display(2,p_rtc_new_alarm->minute/10,TRUE);
                led_display(3,p_rtc_new_alarm->minute%10,TRUE);
            }
            else
            {
                led_display(2, 0,FALSE);
                led_display(3, 0,FALSE);
            }
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
    g_need_update_alarm_value = FALSE;
}

void enter_new_alarm_setting(uint8 idx)
{
    com_rtc_get_alarm_info(idx, &g_rtc_alarm_info);
    libc_memcpy(&g_alarm_time_backup, &(g_rtc_alarm_info.alarm_time), sizeof(time_t));

    g_rtc_alarm_set_sta = ALARM_SET_ONOFF;
    g_need_update_alarm_display = FALSE;
    g_need_update_alarm_value = FALSE;
    g_hh_mm_display = FALSE;

    led_display_icon(LCD_COL,FALSE);
    led_display_icon(LCD_P1,FALSE);
    if (g_rtc_alarm_info.alarm_onoff == TRUE)
    {
        led_display(0,0,FALSE);
        led_display(1,'O',TRUE);
        led_display(2,'N',TRUE);
        led_display(3,0,FALSE);
    }
    else
    {
        led_display(0,0,FALSE);
        led_display(1,'O',TRUE);
        led_display(2,'F',TRUE);
        led_display(3,'F',TRUE);
    }

    g_rtc_alarm_set_overtime = 0;
}

app_result_e com_rtc_alarm_set_inner(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    int8 rtc_alarm_set_timer_id;

    //默认进入闹钟0
    g_alarm_setting_idx = 0;
    enter_new_alarm_setting(g_alarm_setting_idx);

    rtc_alarm_set_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SUB, APP_TIMER_ATTRB_CONTROL, \
        500, com_rtc_alarm_set_timer_handle);

    while (1)
    {
        if (g_need_update_alarm_display == TRUE)
        {
            com_rtc_time_set_update_alarm_display(&(g_rtc_alarm_info.alarm_time));

            g_need_update_alarm_display = FALSE;
        }

        if (g_rtc_alarm_set_overtime >= RTC_TIME_OVERTIME)
        {
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            g_rtc_alarm_set_overtime = 0;

            if (com_key_mapping(&gui_msg, &gui_event, rtc_alarm_set_key_map_list) == TRUE)
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
                        goto alarm_set_exit;
                    }
                    break;

                case EVENT_RTC_VALUE_ADD:
                    if (g_rtc_alarm_set_sta == ALARM_SET_ONOFF)
                    {
                        if (g_rtc_alarm_info.alarm_onoff == TRUE)
                        {
                            g_rtc_alarm_info.alarm_onoff = FALSE;
                        }
                        else
                        {
                            g_rtc_alarm_info.alarm_onoff = TRUE;
                        }
                    }
                    else if (g_rtc_alarm_set_sta == ALARM_SET_HOUR)
                    {
                        if (g_rtc_alarm_info.alarm_time.hour < 23)
                        {
                            g_rtc_alarm_info.alarm_time.hour++;
                        }
                        else
                        {
                            g_rtc_alarm_info.alarm_time.hour = 0;
                        }
                    }
                    else
                    {
                        if (g_rtc_alarm_info.alarm_time.minute < 59)
                        {
                            g_rtc_alarm_info.alarm_time.minute++;
                        }
                        else
                        {
                            g_rtc_alarm_info.alarm_time.minute = 1;
                        }
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_alarm_set_timer_id);
                    g_hh_mm_display = TRUE;
                    g_need_update_alarm_value = TRUE;
                    g_need_update_alarm_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_RTC_VALUE_SUB:
                    if (g_rtc_alarm_set_sta == ALARM_SET_ONOFF)
                    {
                        if (g_rtc_alarm_info.alarm_onoff == TRUE)
                        {
                            g_rtc_alarm_info.alarm_onoff = FALSE;
                        }
                        else
                        {
                            g_rtc_alarm_info.alarm_onoff = TRUE;
                        }
                    }
                    else if (g_rtc_alarm_set_sta == ALARM_SET_HOUR)
                    {
                        if (g_rtc_alarm_info.alarm_time.hour > 0)
                        {
                            g_rtc_alarm_info.alarm_time.hour--;
                        }
                        else
                        {
                            g_rtc_alarm_info.alarm_time.hour = 23;
                        }
                    }
                    else
                    {
                        if (g_rtc_alarm_info.alarm_time.minute > 0)
                        {
                            g_rtc_alarm_info.alarm_time.minute--;
                        }
                        else
                        {
                            g_rtc_alarm_info.alarm_time.minute = 59;
                        }
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_alarm_set_timer_id);
                    g_hh_mm_display = TRUE;
                    g_need_update_alarm_value = TRUE;
                    g_need_update_alarm_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_RTC_VALUE_CONFORM:
                    if (g_rtc_alarm_set_sta == ALARM_SET_ONOFF)
                    {
                        if (g_rtc_alarm_info.alarm_onoff == FALSE)
                        {
                            g_rtc_alarm_set_sta = ALARM_SET_OK;
                        }
                        else
                        {
                            g_rtc_alarm_set_sta = ALARM_SET_HOUR;
                        }
                    }
                    else if (g_rtc_alarm_set_sta == ALARM_SET_HOUR)
                    {
                        g_rtc_alarm_set_sta = ALARM_SET_MIN;
                    }
                    else
                    {
                        g_rtc_alarm_set_sta = ALARM_SET_OK;
                    }
                    restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_alarm_set_timer_id);
                    g_hh_mm_display = TRUE;
                    g_need_update_alarm_value = TRUE;
                    g_need_update_alarm_display = TRUE;
                    g_number_input_keys = 0;
                    break;

                case EVENT_NUMBER_INPUT:
                    if ((g_rtc_alarm_set_sta == ALARM_SET_HOUR) || (g_rtc_alarm_set_sta == ALARM_SET_MIN))
                    {
                        uint8 number_input = gui_msg.data.kmsg.val - KEY_NUM0;
                        bool number_input_finish = FALSE;

                        if (g_rtc_alarm_set_sta == ALARM_SET_HOUR)
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_alarm_info.alarm_time.hour = number_input;
                                g_number_input_keys = 1;
                            }
                            else
                            {
                                g_rtc_alarm_info.alarm_time.hour *= 10;
                                g_rtc_alarm_info.alarm_time.hour += number_input;
                                g_number_input_keys = 0;
                                number_input_finish = TRUE;
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if (g_rtc_alarm_info.alarm_time.hour > 23)
                                {
                                    g_rtc_alarm_info.alarm_time.hour = g_alarm_time_backup.hour;
                                }
                            }
                        }
                        else
                        {
                            if (g_number_input_keys == 0)
                            {
                                g_rtc_alarm_info.alarm_time.minute = number_input;
                                g_number_input_keys = 1;
                            }
                            else
                            {
                                g_rtc_alarm_info.alarm_time.minute *= 10;
                                g_rtc_alarm_info.alarm_time.minute += number_input;
                                g_number_input_keys = 0;
                                number_input_finish = TRUE;
                            }

                            //合法性检查
                            if (number_input_finish == TRUE)
                            {
                                if (g_rtc_alarm_info.alarm_time.minute > 59)
                                {
                                    g_rtc_alarm_info.alarm_time.minute = g_alarm_time_backup.minute;
                                }
                            }
                        }

                        restart_app_timer_inner(APP_TIMER_GROUP_COMMON, rtc_alarm_set_timer_id);
                        g_hh_mm_display = TRUE;
                        g_need_update_alarm_value = TRUE;
                        g_need_update_alarm_display = TRUE;
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

        if (g_rtc_alarm_set_sta == ALARM_SET_OK)
        {
            if (g_alarm_setting_idx >= (ALARM_RECORD_MAX - 1))
            {
                result = RESULT_BACK_RTC_TIME;
            }
            else
            {
                //保存当前闹钟，切换到下一个闹钟
                com_rtc_update_alarm_info(g_alarm_setting_idx, &g_rtc_alarm_info, ALARM_UPDATE_MODIFY);

                g_alarm_setting_idx++;
                enter_new_alarm_setting(g_alarm_setting_idx);
            }
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    alarm_set_exit:

    if (g_rtc_alarm_set_sta == ALARM_SET_OK)
    {
        com_rtc_update_alarm_info(g_alarm_setting_idx, &g_rtc_alarm_info, ALARM_UPDATE_MODIFY);
    }
    else
    {
        if (g_rtc_alarm_set_sta != ALARM_SET_ONOFF) //已确定闹钟开关设置，但未完成闹钟时间设置
        {
            libc_memcpy(&(g_rtc_alarm_info.alarm_time), &g_alarm_time_backup, sizeof(time_t));
            com_rtc_update_alarm_info(g_alarm_setting_idx, &g_rtc_alarm_info, ALARM_UPDATE_MODIFY);
        }
    }

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, rtc_alarm_set_timer_id);
    rtc_alarm_set_timer_id = -1;

    return result;
}

