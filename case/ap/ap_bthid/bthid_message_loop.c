/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙拍照前台应用主循环，获取按键和应用消息并响应。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bthid.h"

uint8 isLimitedDiscoverableMode = 0;
int8 exit_limited_discoverable_mode_timer_id = -1;

app_result_e limited_discover_deal(void);
app_result_e disconnect_hid_deal(void);

const key_map_t bt_key_map_list[] =
{
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },

    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_HID_TAKE_PHOTO },

    // FOR HID BQB   set in Limited Discoverable Mode
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP}, EVENT_HID_BQB_PREV },
    // FOR HID BQB   disconnect hid
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP}, EVENT_HID_BQB_NEXT },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

app_result_e deal_gui_bt(input_gui_msg_t *gui_msg)
{
    app_result_e app_ret;
    msg_apps_type_e gui_event;

    app_ret = RESULT_NULL;

    if (com_key_mapping(gui_msg, &gui_event, bt_key_map_list) == TRUE)
    {
        switch (gui_event)
        {
            case EVENT_PLAYING_PAUSE:
            com_btengine_hid_take_photo();
            break;

            case EVENT_HID_TAKE_PHOTO:
            com_filter_key_hold();
            app_ret = RESULT_LASTPLAY;
            break;

            case EVENT_HID_BQB_PREV:
            if ((com_get_config_default(BTSTACK_BQB_TEST_ENALBE) & 0x01) == 0x01)
            {
                app_ret = limited_discover_deal();
            }
            break;

            case EVENT_HID_BQB_NEXT:
            if ((com_get_config_default(BTSTACK_BQB_TEST_ENALBE) & 0x01) == 0x01)
            {
                app_ret = disconnect_hid_deal();
            }
            break;

            default:
            app_ret = com_message_box(gui_event);
            if (app_ret == RESULT_REDRAW)
            {
                g_need_draw = TRUE;
                app_ret = RESULT_NULL;
            }
            break;
        }
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int bt_message_deal(private_msg_t *msg)
 * \Record 获取私有消息的处理
 * \param[in]    private_msg_t *msg
 * \param[out]   none
 * \return               app_result_e
 * \retval
 * \ingroup      bluetooth_message_handle.c
 * \note
 */
/*******************************************************************************/
app_result_e bt_message_deal(private_msg_t *msg)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    switch (msg->msg.type)
    {
        default:
        app_ret = com_message_box(msg->msg.type);
        break;
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  根据后台蓝牙的状态进行处理
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
app_result_e bt_status_deal(void)
{
    app_result_e result = RESULT_NULL;

    com_get_btengine_status(&g_bthid_bt_info);
    g_disp_status = g_bthid_bt_info.status;

    return result;
}

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

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    private_msg_t pri_msg;

    //获取并更新一次状态，保证 g_bthid_bt_info 有效
    com_get_btengine_status(&g_bthid_bt_info);

    //注册应用消息分发函数
    set_app_msg_dispatch_handle(bt_message_deal);

    while (1)
    {
        result = bt_status_deal();
        if (result != RESULT_NULL)
        {
            break;
        }

        if ((g_need_draw == TRUE) || (g_disp_status != last_disp_status))
        {
            if (g_need_draw == TRUE)
            {
                led_clear_screen(CLR_ALL);
                if (display_timer_id != -1)
                {
                    kill_app_timer(THIS_APP_TIMER_GROUP, display_timer_id);
                    display_timer_id = -1;
                }
            }

            switch (g_disp_status)
            {
                case STATUS_NONE:
                libc_printf_info("STATUS_NONE\n");
                com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

                led_clear_screen(CLR_ALL);
                led_display(NUMBER2, 'b', TRUE);
                led_display(NUMBER3, 'L', TRUE);
                break;

                case STATUS_WAIT_PAIRED:
                libc_printf_info("STATUS_WAIT_PAIRED\n");
                com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_400MS, TK_SEL_PWM);

                led_display(NUMBER2, 'b', TRUE);
                led_display(NUMBER3, 'L', TRUE);

                if (display_timer_id == -1)
                {
                    display_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN,\
                        APP_TIMER_ATTRB_CONTROL, 500, (timer_proc) wait_link_display);
                    display_flag = FALSE;
                }

                if (last_disp_status == STATUS_NONE)
                {
                    //语音播报等待连接
                    com_tts_state_play(TTS_WAIT_BT_CON, 0);
                }
                else
                {
                    //语音播报蓝牙断开&等待连接
                    com_tts_state_play(TTS_BT_DISCON, 0);
                    com_tts_state_play(TTS_WAIT_BT_CON, 0);
                }
                break;

                case STATUS_LINKED:
                libc_printf_info("STATUS_LINKED\n");
                com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

                led_display(NUMBER2, 'b', TRUE);
                led_display(NUMBER3, 'L', TRUE);

                if (display_timer_id != -1)
                {
                    kill_app_timer(THIS_APP_TIMER_GROUP, display_timer_id);
                    display_timer_id = -1;
                }

                //蓝牙连接成功
                com_tts_state_play(TTS_BT_CON_OK, 0);
                break;

                default:
                break;
            }

            last_disp_status = g_disp_status;
            g_need_draw = FALSE;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            result = deal_gui_bt(&gui_msg);
        }
        else
        {
            if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
            {
                result = bt_message_deal(&pri_msg);
            }
        }

        if (result == RESULT_REDRAW)
        {
            result = RESULT_NULL;
            g_need_draw = TRUE;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    return result;
}

#if 0
static void _delay(void)
{
    int i,j;
    for (i=0; i<0x1000; i++);
}
#endif

void exit_limited_discoverable_mode(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SET_TO_GENERAL_DISCOVERABLE_MODE_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    isLimitedDiscoverableMode = 0;
    exit_limited_discoverable_mode_timer_id = -1;
}

app_result_e limited_discover_deal(void)
{
    msg_apps_t msg;

    isLimitedDiscoverableMode = 1;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SET_TO_LIMITED_DISCOVERABLE_MODE_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    if (exit_limited_discoverable_mode_timer_id == -1)
    {
        exit_limited_discoverable_mode_timer_id = set_single_shot_app_timer(THIS_APP_TIMER_GROUP,
                APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 1000 * 30, (timer_proc) exit_limited_discoverable_mode);
    }
    else
    {
        restart_app_timer(THIS_APP_TIMER_GROUP, exit_limited_discoverable_mode_timer_id);
    }

    return RESULT_NULL;
}

app_result_e disconnect_hid_deal(void)
{
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_HID_DISCONNECT_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

