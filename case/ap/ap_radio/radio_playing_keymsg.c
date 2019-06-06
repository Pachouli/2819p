/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用播放场景按键消息处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

/* radio 播放场景按键事件映射表*/
const key_map_t playing_key_map_list[] =
{
    /*! 短按NEXT 键下一频点事件*/
    { { KEY_CHADD, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! 短按PREV 键上一频点事件 */
    { { KEY_CHSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },

    /*! 长按NEXT 键向后半自动搜台事件*/
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_NEXT },
    /*! 长按PREV 键向前半自动搜台事件 */
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_PREV },

#if 1 //如果有专用CH+/CH-，则NEXT/PREV作为预设电台切换按键
    /*!短按NEXT 下一电台事件 */
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_PRESET },
    /*!短按NEXT 下一电台事件 */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_PRESET },
    /*! 短按KEY_NEXT_VOLADD 键下一频点事件*/
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_PRESET },
    /*! 短按KEY_PREV_VOLSUB 键上一频点事件 */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_PRESET },
#else
    /*! 短按NEXT 键下一频点事件*/
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! 短按PREV 键上一频点事件 */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },
    /*! 短按KEY_NEXT_VOLADD 键下一频点事件*/
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! 短按KEY_PREV_VOLSUB 键上一频点事件 */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },
#endif

    /*!短按play 暂停收听/恢复收听切换事件 */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PLAY_PAUSE },

    /*!短按F1 自动搜台事件 */
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_AUTO_SEARCH },

    /*! 数字按键调节 */
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP }, EVENT_NUMBER_INPUT },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

void set_freq(uint32 freq)
{
    uint8 num;

    //判断切换后的频点是否在当前所播放的预设电台表中
    num = whether_in_list(freq, pfreq);

    //切换后的频点是未保存频点
    if (num == 0xff)
    {
        //没有在预设电台表中
        g_radio_config.FMStatus.station = 0;
    }
    else
    {
        //切换后的频点是已保存频点
        g_radio_config.FMStatus.station = num;

        //电台号界面
        radio_show_station_num(g_radio_config.FMStatus.station, TRUE);
    }

    //刷新频率相关显示
    radio_show_frequency(freq, FALSE);

    //更新频点播放
    radio_set_freq(freq);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e change_freq(FM_SeekDir_e dir)
 * \切换到上一个或下一个频率
 * \param[in]    dir 切换方向
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e change_freq(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_IGNORE;
    uint8 space;

    //步长
    space = 100;

    if (g_radio_config.band_mode == Band_Europe)
    {
        //欧洲频段,  以50k为步进
        space = 50;
    }

    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
            //向上切换频率
            g_radio_config.FMStatus.freq += space;
        }
        else
        {
            //最大频点切换到最小
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
        {
            //向下切换
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //最小频点切换到最大
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }

    set_freq(g_radio_config.FMStatus.freq);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  select_station(uint8 direction)
 * \切换到下一个已保存电台
 * \param[in]    direction 切换电台方向，0表示切换到上一个电台，1表示切换到下一个电台
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e select_station(uint8 direction)
{
    app_result_e ret = RESULT_IGNORE;

    //循环查找
    uint8 i, cur_num;
    //电台频率值
    uint32 station_freq;

    //预设列表中有已保存电台
    if (g_radio_config.FMStatus.station_count > 0)
    {
        if (g_radio_config.FMStatus.station == 0)
        {
            //当前不在预设列表中的频点上，切换到第一个频点
            g_radio_config.FMStatus.station = 1;
        }
        else
        {
            if (direction == SELECT_NEXT_DIRECTION)
            {
                if (g_radio_config.FMStatus.station < g_radio_config.FMStatus.station_count)
                {
                    //切换电台号
                    g_radio_config.FMStatus.station++;
                }
                else
                {
                    //最后回第一项
                    g_radio_config.FMStatus.station = 1;
                }
            }
            else
            {
                if (g_radio_config.FMStatus.station > 1)
                {
                    //切换电台号
                    g_radio_config.FMStatus.station--;
                }
                else
                {
                    //最后回第一项
                    g_radio_config.FMStatus.station = g_radio_config.FMStatus.station_count;
                }
            }
        }
        cur_num = g_radio_config.FMStatus.station;
        for (i = 0; i < g_radio_config.FMStatus.station_count; i++)
        {
            station_freq = *(pfreq + cur_num - 1);
            if ((station_freq >= min_freq[g_radio_config.band_mode]) && (station_freq
                    <= max_freq[g_radio_config.band_mode]))
            {
                //找到有效台，退出
                break;
            }
            else
            {
                //向下查找
                cur_num++;
                if (cur_num > g_radio_config.FMStatus.station_count)
                {
                    cur_num = 1;
                }
            }
        }
        //找到有效电台
        g_radio_config.FMStatus.station = cur_num;
        //通过电台号获取频率值
        g_radio_config.FMStatus.freq = *(pfreq + g_radio_config.FMStatus.station - 1);

        //刷新电台号
        radio_show_station_num(g_radio_config.FMStatus.station, TRUE);
    }
    else
    {
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
    }

    //刷新频率相关显示
    radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);

    need_tts = TRUE;

    //更新频点播放(没有保存电台也需要重新设置频点)
    radio_set_freq(g_radio_config.FMStatus.freq);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  play_next_station(void)
 * \切换到下一个已保存电台
 * \param[in]    void
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e play_next_station(uint8 direction)
{
    app_result_e ret;

    if (g_fmeg_status.play_status == PlaySta)
    {
        //暂停收听
        radio_pause();
    }

    ret = select_station(direction);

    if (need_tts == TRUE)
    {
        //play current freq
        radio_freq_tts_play(g_radio_config.FMStatus.freq);

        need_tts = FALSE;
    }

    //开始收听
    radio_resume();

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_play_search_deal(FM_SeekDir_e search_dir)
 * \处半自动搜台按键消息
 * \param[in]    search_dir  para1 半自动搜台方向
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up消息
 * \retval       false 继续快进退
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e radio_scene_play_search_deal(FM_SeekDir_e search_dir)
{
    app_result_e deal_retval;

    deal_retval = radio_scene_manual_search(search_dir);

    return deal_retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_play_next_freq(void)
 * \处理next下一个频点消息
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      radio_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e radio_play_next_freq(void)
{
    app_result_e ret_next;

    //mute，避免切换电台杂音
    radio_set_mute (SetMUTE);

    ret_next = change_freq(DIR_UP);

    //释放静音
    radio_set_mute(releaseMUTE);

    //开始收听
    if ((g_fmeg_status.play_status == StopSta) || (g_fmeg_status.play_status == PauseSta))
    {
        radio_resume();
    }

    return ret_next;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_play_prev_freq(void)
 * \处理prev上一个频点消息
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      radio_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e radio_play_prev_freq(void)
{
    app_result_e ret_prev;

    //mute，避免切换电台杂音
    radio_set_mute (SetMUTE);

    ret_prev = change_freq(DIR_DOWN);

    //释放静音
    radio_set_mute(releaseMUTE);

    //开始收听
    if ((g_fmeg_status.play_status == StopSta) || (g_fmeg_status.play_status == PauseSta))
    {
        radio_resume();
    }

    return ret_prev;
}

app_result_e radio_play_number_input_freq(uint32 freq)
{
    //mute，避免切换电台杂音
    radio_set_mute (SetMUTE);

    g_radio_config.FMStatus.freq = freq;
    set_freq(freq);

    //释放静音
    radio_set_mute(releaseMUTE);

    //开始收听
    if ((g_fmeg_status.play_status == StopSta) || (g_fmeg_status.play_status == PauseSta))
    {
        radio_resume();
    }

    return RESULT_NULL;
}

//暂停收听/恢复收听
app_result_e radio_play_pause_station(void)
{
    if ((g_fmeg_status.play_status == PauseSta) || (g_fmeg_status.play_status == StopSta))
    {
        com_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
        radio_resume();
    }
    else//PlaySta
    {
        //pause
        radio_pause();
        com_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e scene_play_guimsg(input_gui_msg_t* cur_msg)
 * \radio 播放场景gui消息处理
 * \param[in]    cur_event  按键触发的消息事件
 * \param[in]    cur_msg  当前消息,用于数字选台
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e scene_play_guimsg(input_gui_msg_t *cur_msg)
{
    app_result_e result = RESULT_NULL;
    msg_apps_type_e cur_event;
    bool keep_number_input;

    if (com_key_mapping(cur_msg, &cur_event, playing_key_map_list) == TRUE)
    {
        keep_number_input = FALSE;

        switch (cur_event)
        {
            //切换到下一个预设电台
            case EVENT_RADIO_NEXT_PRESET:
            led_force_exit_msg_show();
            result = play_next_station(SELECT_NEXT_DIRECTION);
            break;

            //切换到上一个预设电台
            case EVENT_RADIO_PREV_PRESET:
            led_force_exit_msg_show();
            result = play_next_station(SELECT_PREV_DIRECTION);
            break;

            //切换到下一个频率
            case EVENT_RADIO_NEXT_FREQ:
            led_force_exit_msg_show();
            result = radio_play_next_freq();
            break;

            //切换到上一个频率
            case EVENT_RADIO_PREV_FREQ:
            led_force_exit_msg_show();
            result = radio_play_prev_freq();
            break;

            //向后半自动搜台
            case EVENT_RADIO_SEEK_NEXT:
            com_filter_key_hold(); //过滤后续按键消息
            led_force_exit_msg_show();
            result = RESULT_RADIO_MANUAL_UP_SEARCH;
            break;

            //向前半自动搜台
            case EVENT_RADIO_SEEK_PREV:
            com_filter_key_hold(); //过滤后续按键消息
            led_force_exit_msg_show();
            result = RESULT_RADIO_MANUAL_DOWN_SEARCH;
            break;

            //自动搜台事件处理
            case EVENT_RADIO_AUTO_SEARCH:
            led_force_exit_msg_show();
            result = RESULT_RADIO_AUTO_SEARCH;
            break;

            //暂停收听/恢复收听事件处理
            case EVENT_RADIO_PLAY_PAUSE:
            led_force_exit_msg_show();
            result = radio_play_pause_station();
            break;

            case EVENT_NUMBER_INPUT:
            {
                uint8 number_input = cur_msg->data.kmsg.val - KEY_NUM0;
                bool number_input_finish = FALSE;
                uint32 start_freq, end_freq;

                keep_number_input = TRUE;
                g_number_input_timer = sys_get_ab_timer();

                led_force_exit_msg_show();

                get_border_freq(&start_freq, &end_freq, DIR_UP);

                if (g_number_input_keys == 0)
                {
                    g_number_input_freq = number_input;
                    g_number_input_high = number_input;
                    g_number_input_keys++;
                }
                else
                {
                    g_number_input_freq *= 10;
                    g_number_input_freq += number_input;
                    g_number_input_keys++;
                    if ((g_number_input_high != 1) && (g_number_input_keys == 3))
                    {
                        g_number_input_keys = 0;
                        number_input_finish = TRUE;
                    }
                    else if ((g_number_input_high == 1) && (g_number_input_keys == 4))
                    {
                        g_number_input_keys = 0;
                        number_input_finish = TRUE;
                    }
                }

                //合法性检查
                if (number_input_finish == TRUE)
                {
                    if ((g_number_input_freq*100 >= start_freq) && (g_number_input_freq*100 <= end_freq))
                    {
                        //合法频点，切换频点
                        radio_play_number_input_freq(g_number_input_freq*100);
                    }
                    else
                    {
                        //非法频点，恢复原来频点显示
                        radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);
                    }
                }
                else
                {
                    radio_show_frequency(g_number_input_freq*100, (g_number_input_high == 1));
                }
            }
            break;

            default:
            result = com_message_box(cur_event);
            break;
        }

        //如果不是按数字按键，则退出数字按键输入模式
        if (keep_number_input == FALSE)
        {
            //先恢复原来频点显示
            radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);
            g_number_input_keys = 0;
        }
    }

    return result;
}

