/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：实现RTC时钟显示功能。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_alarm.h"

static now_alarm_detail_t g_now_alarm_detail;
static bool g_need_update_time;
static bool g_hh_mm_col_display;
static ring_type_e g_alarm_ring_type;
static mengine_status_t alarm_play_status;

const key_map_t rtc_alarm_rint_key_map_list[] =
{
    /*! 关闭闹钟 */
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_ALARM_CLOSE },
    /*! 关闭闹钟 */
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_CLOSE },

    /*! 以下按键SNOOZE闹钟 */
    { { KEY_F1, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_ALARM_SNOOZE },
    { { KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },
    { { KEY_VADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },
    { { KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },
    { { KEY_VSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ALARM_SNOOZE },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL },
};

void * alarm_ring_play(void)
{
    libc_printf_info("alarm ring play\n");

    return com_sd_mp3_player_play_mp3("alarm.mp3");
}

bool alarm_ring_stop(void * alarm_ring_handle)
{
    libc_printf_info("alarm ring stop\n");

    com_sd_mp3_player_stop(alarm_ring_handle);

    return TRUE;
}

bool alarm_ring_is_over(void * alarm_ring_handle)
{
    return com_sd_mp3_player_is_stop(alarm_ring_handle);
}

void _kill_conflict_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //查看当前引擎是否为music
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        //注销radio后台
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

bool alarm_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;

    //存放输入参数的地址
    msg.content.addr = input_param;

    //存放获取参数的地址
    reply.content = output_param;

    //消息类型(即消息名称)
    msg.type = msg_type;

    //发送同步消息
    bret = send_sync_msg(APP_ID_THEENGINE, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

bool check_disk_alarm_ring_valid(uint8 disk_type)
{
    msg_apps_t msg;
    bool bret = TRUE;

    //无消息内容
    msg.para[0] = APP_ID_MENGINE;
    msg.para[1] = (uint8) (ENTER_ALARM_PLAY | disk_type);
    msg.content.addr = NULL;

    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    //获取是否能够找到合法歌曲播放
    bret = alarm_send_msg(NULL, &alarm_play_status, MSG_MENGINE_GET_STATUS_SYNC);
    if (bret == TRUE)
    {
        if (alarm_play_status.err_status != EG_ERR_NONE)
        {
            bret = FALSE;
        }
        else
        {
            uint8 playmode = FSEL_MODE_LOOPONEDIR;
            alarm_send_msg(&playmode, NULL, MSG_MENGINE_SET_PLAYMODE_SYNC);
        }
    }

    if (bret == FALSE)
    {
        _kill_conflict_engine();
    }

    return bret;
}

bool check_alarm_overtime(time_t *p_cur_time)
{
    uint32 start_second, cur_second;

    start_second = g_now_alarm_detail.start_time.minute * 60;
    start_second += g_now_alarm_detail.start_time.second;

    cur_second = p_cur_time->minute * 60;
    cur_second += p_cur_time->second;

    if (cur_second < start_second)
    {
        cur_second += 3600; //越过小时
    }

    if ((cur_second - start_second) >= (g_now_alarm_detail.overtime * 60))
    {
        return TRUE;
    }

    return FALSE;
}

void alarm_clock_display_timer_handle(void)
{
    g_need_update_time = TRUE;
}

app_result_e alarm_app_msg_deal(private_msg_t *pri_msg)
{
    app_result_e result = RESULT_NULL;

    switch (pri_msg->msg.type)
    {
    /*! 闹钟起闹不响应设备插入消息 */
    case MSG_SD_IN:
    case MSG_UH_IN:
    case MSG_LINEIN_IN:
    case MSG_USB_STICK:
        break;

    default:
        result = com_message_box(pri_msg->msg.type);
    }

    return result;
}

app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    void * alarm_ring_handle = NULL;
    time_t temp_time;
    bool alarm_snooze_flag = FALSE;
    bool alarm_overtime_flag = FALSE;
    int8 alarm_clock_display_timer_id;
    uint8 backup_volume;
    bool bret = TRUE;
    uint8 i;

    libc_printf_info("ENTER ALARM RING!\n");

    //确保杀死其他引擎
    _kill_conflict_engine();

    com_set_mute (FALSE); //解除静音

    com_rtc_get_alarm_detail(&g_now_alarm_detail);
    libc_printf("alarm detail:\n");
    libc_printhex(&g_now_alarm_detail, sizeof(g_now_alarm_detail), 0);

    //设置闹钟闹铃音量
    backup_volume = com_get_sound_volume();
    com_set_sound_volume(SET_VOL_BY_SYSTEM, g_now_alarm_detail.volume);

    alarm_clock_display_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, \
        500, alarm_clock_display_timer_handle);

    //从S3回到S1状态，1S内读取RTC日历时间可能无效
    for (i = 0; i < 30; i++)
    {
        rtc_get_time(&temp_time);
        if ((temp_time.hour != 0) || (temp_time.minute != 0) || (temp_time.second != 0))
        {
            break;
        }
        sys_os_time_dly(5);
    }
    libc_printf("ALARM TIME:%02d:%02d:%02d\n", temp_time.hour, temp_time.minute, temp_time.second);

    led_clear_screen(CLR_ALL);
    led_display(0,temp_time.hour/10,TRUE);
    led_display(1,temp_time.hour%10,TRUE);
    led_display_icon(LCD_COL,TRUE);
    led_display_icon(LCD_PLAY,TRUE);
    led_display(2,temp_time.minute/10,TRUE);
    led_display(3,temp_time.minute%10,TRUE);
    g_need_update_time = FALSE;
    g_hh_mm_col_display = FALSE;

    if (get_bt_state() == BT_STATE_WORK)
    {
        //如果不支持后台蓝牙，所有非蓝牙应用都与 bt enging 冲突，需要杀死 bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

    g_alarm_ring_type = RING_TYPE_BUILTIN;
    if (get_uhost_state() == UHOST_IN)
    {
        set_cur_func_id(APP_FUNC_ALARM_UHOST);
        if (check_disk_alarm_ring_valid(DISK_U) == TRUE)
        {
            g_alarm_ring_type = RING_TYPE_UHOST;
        }
        else
        {
            set_cur_func_id(APP_FUNC_ALARM);
        }
    }
    if (g_alarm_ring_type == RING_TYPE_BUILTIN)
    {
        if (get_card_state() == CARD_STATE_CARD_IN)
        {
            set_cur_func_id(APP_FUNC_ALARM_SDCARD);
            if (check_disk_alarm_ring_valid(DISK_H) == TRUE)
            {
                g_alarm_ring_type = RING_TYPE_SDCARD;
            }
            else
            {
                set_cur_func_id(APP_FUNC_ALARM);
            }
        }
    }

    if (g_alarm_ring_type == RING_TYPE_SDCARD)
    {
        led_display_icon(LCD_SD,TRUE);
    }
    else if (g_alarm_ring_type == RING_TYPE_UHOST)
    {
        led_display_icon(LCD_USB,TRUE);
    }

    while (1)
    {
        if (g_need_update_time == TRUE)
        {
            if (g_hh_mm_col_display == FALSE)
            {
                led_clear_screen(CLR_MAIN);

                g_hh_mm_col_display = TRUE;
            }
            else
            {
                rtc_get_time(&temp_time);

                led_display(0,temp_time.hour/10,TRUE);
                led_display(1,temp_time.hour%10,TRUE);
                led_display_icon(LCD_COL,TRUE);
                led_display(2,temp_time.minute/10,TRUE);
                led_display(3,temp_time.minute%10,TRUE);

                g_hh_mm_col_display = FALSE;
            }

            g_need_update_time = FALSE;

            //闹钟起闹需要始终保持LED数码管显示时间
            {
                uint16 msg_type = MSG_KEEP_LED_DISPLAY;
                sys_mq_send(MQ_ID_SYS, &msg_type);
            }
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &gui_event, rtc_alarm_rint_key_map_list) == TRUE)
            {
                switch (gui_event)
                {
                case EVENT_ALARM_CLOSE:
                    com_filter_key_hold();
                    goto alarm_ring_exit;

                case EVENT_ALARM_SNOOZE:
                    com_filter_key_hold();
                    alarm_snooze_flag = TRUE;
                    goto alarm_ring_exit;

                default:
                    result = com_message_box(gui_event);
                    break;
                }
            }
        }
        else
        {
            if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
            {
                result = alarm_app_msg_deal(&pri_msg);
            }
        }

        if (check_alarm_overtime(&temp_time) == TRUE)
        {
            alarm_overtime_flag = TRUE;
            break;
        }

        if (g_alarm_ring_type == RING_TYPE_BUILTIN)
        {
            if (alarm_ring_handle == NULL)
            {
                alarm_ring_handle = alarm_ring_play();
                if (alarm_ring_handle == NULL)
                {
                    libc_printf_error("com_rtc_alarm_ring fail!\n");
                    while (1);
                }
            }
            else
            {
                if (alarm_ring_is_over(alarm_ring_handle) == TRUE)
                {
                    alarm_ring_stop(alarm_ring_handle);
                    alarm_ring_handle = NULL;
                }
            }
        }
        else
        {
            bret = alarm_send_msg(NULL, &alarm_play_status, MSG_MENGINE_GET_STATUS_SYNC);
            if (bret == TRUE)
            {
                if (alarm_play_status.err_status != EG_ERR_NONE)
                {
                    bret = FALSE;
                }
            }
            if (bret == FALSE)
            {
                break;
            }
        }

        if (result == RESULT_REDRAW)
        {
            result = RESULT_NULL;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        sys_os_time_dly(1);
    }

    alarm_ring_exit:

    if (alarm_overtime_flag == TRUE)
    {
        libc_printf_info("ALARM OVERTIME!\n");
        com_rtc_update_alarm(ALARM_UPDATE_OVERTIME);
    }
    else if (alarm_snooze_flag == TRUE)
    {
        libc_printf_info("ALARM SNOOZE!\n");
        com_rtc_update_alarm(ALARM_UPDATE_SNOOZE);
    }
    else
    {
        libc_printf_info("ALARM CLOSE!\n");
        com_rtc_update_alarm(ALARM_UPDATE_CLOSE);
    }

    if (g_alarm_ring_type == RING_TYPE_BUILTIN)
    {
        if (alarm_ring_handle != NULL)
        {
            alarm_ring_stop(alarm_ring_handle);
        }
    }
    else
    {
        //杀死music引擎
        _kill_conflict_engine();
    }

    kill_app_timer(THIS_APP_TIMER_GROUP, alarm_clock_display_timer_id);
    alarm_clock_display_timer_id = -1;

    //设置闹钟闹铃音量
    com_set_sound_volume(SET_VOL_BY_SYSTEM, backup_volume);

    return result;
}
