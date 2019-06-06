/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌前台应用  前台应用获取并处理引擎播放信息
 ********************************************************************************/


#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* info_ptr)
 * \获取播放信息当前播放时间和切歌信息
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
bool music_get_playinfo(mengine_playinfo_t *info_ptr)
{
    bool bret = TRUE;
    //发送同步消息
    bret = music_send_msg(NULL, info_ptr, MSG_MENGINE_GET_PLAYINFO_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_sysmsg(void)
 * \处理系统消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_sysmsg(void)
{
    //get message
    private_msg_t music_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(THIS_APP_TYPE, &music_msg) == TRUE)
    {
        msg_result = music_message_deal(&music_msg);
    }
    return msg_result;
}

void _playing_show_time(uint32 show_time)
{
    uint8 minute, second;

    show_time = show_time % 3600; //小时无法显示，去掉
    minute = show_time/60;
    second = show_time%60;

    if (minute != last_minute)
    {
        led_display(NUMBER1, minute/10, TRUE);
        led_display(NUMBER2, minute%10, TRUE);
    }
    if (last_col == 0)
    {
        led_display_icon(LCD_COL, TRUE);
    }

    led_display(NUMBER3, second/10, TRUE);
    led_display(NUMBER4, second%10, TRUE);

    last_minute = minute;
    last_col = 1;
}

void _playing_show_seq(uint32 song_seq)
{
    uint8 n;

    led_force_exit_msg_show();
    led_clear_screen(CLR_MAIN);

    for (n = 0; n < 3; n++)
    {
        led_display(NUMBER4 - n, (song_seq % 10), TRUE);
        song_seq /= 10;
    }
    if (song_seq > 9)
    {
        //大于9999首歌曲显示F
        led_display(NUMBER1, 'F', TRUE);
    }
    else
    {
        led_display(NUMBER1, song_seq, TRUE);
    }

    led_enter_msg_show(3000);
}

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
//LCD点阵屏显示文件名字
void _playing_show_filename(uint8 init)
{
    region_t str_region;

    str_region.x = 0;
    str_region.y = 16;
    str_region.width = 128;
    str_region.height = 16;
    if (init == TRUE)
    {
        lcd_clear_screen(&str_region);
        g_fname_scroll_flag = lcd_scroll_string(g_fname_buffer, &str_region);
        libc_printf("g_fname_scroll_flag:%d\n", g_fname_scroll_flag);
        lcd_update_screen(&str_region);
    }
    else
    {
        lcd_scroll_string_scrolling(1);
    }
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void _playing_check_status(void)
 * \从引擎获取当前状态信息 判断是否出错 是否切换歌曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       app_result_e
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _playing_check_status(void)
{
    //mengine_playinfo_t playinfo;

    app_result_e sta_ret = RESULT_NULL;

    music_get_status (&g_music_status); //获取状态信息

    if (((last_play_status == PlaySta) || (last_play_status == PlayFast))
            && ((g_music_status.play_status != PlaySta) && (g_music_status.play_status != PlayFast)))
    {
        //从play到pause
        com_speaker_off(SPK_PLAY);
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PLAY, FALSE);
        led_display_icon(LCD_PAUSE, TRUE);
    }
    else if (((last_play_status != PlaySta) && (last_play_status != PlayFast))
            && ((g_music_status.play_status == PlaySta) || (g_music_status.play_status == PlayFast)))
    {
        //从pause到play
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PAUSE, FALSE);
        led_display_icon(LCD_PLAY, TRUE);
    }
    last_play_status = g_music_status.play_status;

    //正常播放,没有错误
    if (g_music_status.err_status == EG_ERR_NONE)
    {
        //连续错误歌曲数
        g_error_num = 0;

        //清除向前切换标识
        g_switch_prev = 0;

        //获取播放信息
        music_get_playinfo (&g_play_info);

        //只有在播放或快进退时才需要更新时间
        if ((g_music_status.play_status == PlaySta) || (g_music_status.play_status == PlayFast))
        {
            g_cur_time = g_play_info.cur_time / 1000; //ms -> s

            if (g_cur_time != g_prev_time)
            {
                g_prev_time = g_cur_time;

                _playing_show_time(g_cur_time);

                libc_printf_info("play time:%d\n", g_cur_time);
            }
        }

        //是否切歌
        if ((g_play_info.cur_file_switch & 0x01) != 0)
        {
            music_get_filepath (&g_file_path_info);
            libc_printf_info("file index:%d\n", g_file_path_info.file_path.dirlocation.file_num);
            libc_printf_info("file total:%d\n", g_file_path_info.file_path.dirlocation.file_total);

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
            {
                music_file_name_t tmp_music_file_name;
                //获取文件名字
                tmp_music_file_name.fname_buffer = g_fname_buffer;
                tmp_music_file_name.fname_length = 32;
                music_get_filename(&tmp_music_file_name);
                libc_printf("filename:");
                libc_printhex(g_fname_buffer, 32, 0);
                _playing_show_filename(TRUE);
            }
#endif

            if (g_file_path_info.file_path.dirlocation.file_num != g_manul_switch_song_seq)
            {
                _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
            }
            g_manul_switch_song_seq = 0;

            last_minute = -1;
        }

        //是否恢复播歌
        if ((g_play_info.cur_file_switch & 0x03) != 0)
        {
            music_get_fileinfo (&g_music_fileinfo);
            g_total_time = (uint32) g_music_fileinfo.total_time / 1000; //ms -> s
            libc_printf_info("total time:%d\n", g_total_time);
        }

        if((0 == g_file_path_info.file_path.dirlocation.file_total) && (g_file_path_info.file_path.dirlocation.disk == DISK_C))
        {
            sta_ret = RESULT_NEXT_FUNCTION;
        }
    }
    else
    {
        sta_ret = play_eror_deal(g_music_status.err_status);
        if (sta_ret == RESULT_NEXT_FUNCTION)
        {
            //清除错误号
            music_clear_error();
        }
        else
        {
            if (sta_ret == RESULT_NULL)
            {
                music_get_filepath (&g_file_path_info);
            }
        }
    }

    return sta_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _playing_loop_deal(void)
 * \播放场景循环处理
 * \param[in]    para  para1
 * \param[in]    style para2
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
#if (SUPPORT_SPI_LCD_DISPLAY == 1)
void scroll_string_timer_handle(void)
{
    g_scroll_flag = TRUE;
}
#endif

app_result_e _playing_loop_deal(void)
{
    input_gui_msg_t gui_msg;
    app_result_e ret_vals = RESULT_NULL;
    bool need_redraw = FALSE;
    int8 scroll_string_timer_id;
    uint32 test_count = 0;

    //注册应用消息分发函数
    set_app_msg_dispatch_handle(music_message_deal);

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    scroll_string_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 100, scroll_string_timer_handle);
#endif

#if (SUPPORT_SPI_LCD_DISPLAY)
#if 0
    //文件浏览测试接口
    fsel_enum_dir_func_test(DISK_H);
#endif
#endif

    while (1)
    {
        if (need_redraw == TRUE)
        {
            led_clear_screen(CLR_MAIN);
            last_minute = -1;
            last_col = 0;
            _playing_show_time(g_cur_time);
#if (SUPPORT_SPI_LCD_DISPLAY == 1)
            lcd_clear_screen(NULL);
            _playing_show_filename(TRUE);
#endif
            need_redraw = FALSE;
        }

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
        if (g_fname_scroll_flag == TRUE)
        {
            if (g_scroll_flag == TRUE)
            {
                _playing_show_filename(FALSE);
                g_scroll_flag = FALSE;
            }
        }
#endif

        //从引擎获取当前状态信息
        ret_vals = _playing_check_status();

        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
        {
            break;
        }

        //由于卡检测可能是比较费时的动作，所以如果正在卡播放就不进行卡检测，只有暂停状态下才会进行卡检测
#if (SUPPORT_CARD_DETECT == DETECT_BY_CMD)
        if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
        {
            if ((get_card_state() != CARD_STATE_CARD_NULL)
                && ((g_music_status.play_status == StopSta) || (g_music_status.play_status == PauseSta)))
            {
                if (sys_card_detect() == FALSE)
                {
                    uint16 msg = MSG_SYS_SD_OUT;
                    sys_mq_send(MQ_ID_HOTPLUG, &msg);
                }
            }
        }
#endif

#if (SUPPORT_SPI_LCD_DISPLAY)
#if 0
        test_count++;
        if (test_count == 500)
        {
            select_media_file_play_test(DISK_H);
        }
#endif
#endif

        //获取和处理GUI消息、私有消息
        if ((get_gui_msg(&gui_msg) == TRUE))
        {
            ret_vals = _scene_play_guimsg(&gui_msg);
        }
        else
        {
            //一次循环处理一种类型的消息
            ret_vals = _scene_play_sysmsg();
        }

        if (ret_vals == RESULT_REDRAW)
        {
            ret_vals = RESULT_NULL;
            need_redraw = TRUE;
        }

        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
        {
            break;
        }
        else
        {
            if (g_file_path_info.file_path.dirlocation.disk == DISK_U)
            {
                if (get_uhost_state() != UHOST_IN)
                {
                    ret_vals = RESULT_NEXT_FUNCTION;
                    break;
                }
            }
            else if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
            {
                if (get_card_state() != CARD_STATE_CARD_IN)
                {
                    ret_vals = RESULT_NEXT_FUNCTION;
                    break;
                }
            }
        }

#if (SUPPORT_ASET_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            aset_test_loop_deal();
        }
#endif

        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    } //end of while

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    kill_app_timer(THIS_APP_TIMER_GROUP, scroll_string_timer_id);
    scroll_string_timer_id = -1;
#endif

    return ret_vals;
}
