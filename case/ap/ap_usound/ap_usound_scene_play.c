/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：
 ********************************************************************************/
#include "ap_usound.h"

int sys_clk_backup;

app_result_e udevice_msg_callback(private_msg_t *pri_msg);

bool create_usound_engine(void)
{
    msg_apps_t msg;
    bool bret = TRUE;

    //消息内容
    msg.para[0] = APP_ID_USOUND_EG;
    msg.para[1] = com_get_sound_volume_pc();
    msg.content.addr = NULL;
    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;

    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}

static bool udevice_close_engine(void)
{
    msg_apps_t msg;

    if (get_engine_type() != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return TRUE;
}

//快速预览模式会使用该接口来预览，代码硬拷贝到 com_fast_preview_ui.c 里
void usound_show_welcome_ui(void)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    //LED数码管显示
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'U', TRUE);
    led_display(NUMBER2, 'S', TRUE);
    led_display(NUMBER3, 'B', TRUE);
}

app_result_e scene_usound_init(void)
{
    bool bret = TRUE;
    bool need_switch_ap = FALSE;
    bool need_flush_key = FALSE;
    app_result_e result = RESULT_NULL;

    //杀死冲突引擎
    udevice_close_engine();

    com_set_mute(FALSE); //解除静音

    //set volume
    com_reset_sound_volume(SET_VOL_BY_SYSTEM);

    if (g_p_global_var->fp_switch_jump_welcome == FALSE)
    {
        usound_show_welcome_ui();
    }
    g_p_global_var->fp_switch_jump_welcome = FALSE;

#if (SUPPORT_FAST_PREVIEW == 1)

    if (get_bt_state() == BT_STATE_WORK)
    {
        bool need_kill_btengine = TRUE;

        if (need_kill_btengine == TRUE)
        {
            com_close_bt_engine();
        }
    }

    //等待快速预览模式是否切换到其他应用，如果有切换的话，就马上跳转
    while (1)
    {
        fast_preview_status_e fp_sta;
        fp_sta = com_fast_preview_get_status();
        if (fp_sta != FAST_PREVIEW_STA_NULL)
        {
            if (fp_sta == FAST_PREVIEW_STA_RUNNING)
            {
                need_flush_key = TRUE;
            }
            sys_os_time_dly(1);
        }
        else
        {
            private_msg_t pri_msg;
            input_gui_msg_t input_msg;
            uint8 i;

            if (need_flush_key == TRUE)
            {
                while (get_gui_msg(&input_msg) == TRUE)
                {
                    ; //flush key msg
                }
            }

            for (i = 0; i < 5; i++)
            {
                if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
                {
                    result = udevice_msg_callback(&pri_msg);
                    if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
                    {
                        need_switch_ap = TRUE;
                        break;
                    }
                }
                else
                {
                    break;
                }

                sys_os_time_dly(1);
            }

            break;
        }
    }

    if (need_switch_ap == TRUE)
    {
        goto fast_switch_ap_exit;
    }

    //ESD RESET 不要播报TTS
    if (g_p_esd_bk_info->reset_flag == 0)
    {
        //语音提示
        com_tts_state_play(TTS_ENTER_USBBOX, 0);
    }

#else

    //ESD RESET 不要播报TTS
    if (g_p_esd_bk_info->reset_flag == 0)
    {
        //语音提示
        com_tts_state_play(TTS_ENTER_USBBOX, 0);
    }

    if (get_bt_state() == BT_STATE_WORK)
    {
        bool need_kill_btengine = TRUE;

        if (need_kill_btengine == TRUE)
        {
            com_close_bt_engine();
        }
    }

#endif

    if (g_volume_sync >= 2)
    {
        com_set_usbbox_volume_ctrl(USBBOX_VOL_CTRL_ENTER);
    }

    sys_clk_backup = com_adjust_sys_clk(FREQ_108M, TRUE);

    bret = create_usound_engine();

    if (bret == FALSE)
    {
        result = RESULT_NEXT_FUNCTION;
    }

    fast_switch_ap_exit:

    return result;
}

bool scene_usound_exit(void)
{
    udevice_close_engine();

    com_adjust_sys_clk(sys_clk_backup, FALSE);

    if (g_volume_sync >= 2)
    {
        com_set_usbbox_volume_ctrl(USBBOX_VOL_CTRL_EXIT);
    }

    return TRUE;
}

app_result_e udevice_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;

    /*同步消息处理*/
    switch (pri_msg->msg.type)
    {
        case MSG_USB_UNSTICK:
        ret_result = RESULT_NEXT_FUNCTION;
        break;

        default:
        ret_result = com_message_box(pri_msg->msg.type);
        break;
    }

    return ret_result;
}

void _playing_check_status(void)
{
    usound_get_play_status(&g_cur_play_status); //获取状态信息

    if ((g_last_usbbox_status == PlaySta) && (g_cur_play_status.play_status != PlaySta))
    {
        //从play到pause
        com_speaker_off(SPK_PLAY);
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PLAY, FALSE);
        led_display_icon(LCD_PAUSE, TRUE);
    }
    else if ((g_last_usbbox_status != PlaySta) && (g_cur_play_status.play_status == PlaySta))
    {
        //从pause到play
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);
        com_speaker_on(SPK_PLAY);

        led_display_icon(LCD_PAUSE, FALSE);
        led_display_icon(LCD_PLAY, TRUE);
    }

    g_last_usbbox_status = g_cur_play_status.play_status;
}

app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    private_msg_t pri_msg;
    bool need_redraw = FALSE;

    //注册应用消息分发函数
    set_app_msg_dispatch_handle(udevice_msg_callback);

    while (1)
    {
        if (need_redraw == TRUE)
        {
            led_clear_screen(CLR_MAIN);
            led_display(NUMBER1, 'U', TRUE);
            led_display(NUMBER2, 'S', TRUE);
            led_display(NUMBER3, 'B', TRUE);

            need_redraw = FALSE;
        }

        _playing_check_status();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
        com_record_update_record_status(g_cur_play_status.record_sta);
#endif

        /*
         2:PC可调节，可同步至小机,小机调节不可同步至PC;
         3:PC和小机均可互相调节并同步
         */
        if (g_volume_sync >= 2)
        {
            pc_set_volume_sync();
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            result = usound_msg_deal(&gui_msg);
        }
        else
        {
            if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
            {
                result = udevice_msg_callback(&pri_msg);
            }
        }

        if (result == RESULT_REDRAW)
        {
            result = RESULT_NULL;
            need_redraw = TRUE;
        }

        if ((result != RESULT_NULL) && (result != RESULT_USB_SOUND) && (result != RESULT_IGNORE))
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    return result;
}

app_result_e scene_usound_play(void)
{
    app_result_e result;

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);
    com_speaker_on(SPK_PLAY);
    //场景初始化
    result = scene_usound_init();
    if ((result == RESULT_NULL) || (result == RESULT_REDRAW))
    {
        //场景循环
        result = get_message_loop();
        //调用场景退出函数
    }
    scene_usound_exit();
    return result;
}

