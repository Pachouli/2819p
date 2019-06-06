/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用半自动搜台，搜到一个有效电台就会停下来。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool  bool scene_manual_search_init(void)
 * \radio 半自动搜台场景初始化
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
bool scene_manual_search_init(void)
{
    bool ret = TRUE;

    //取得当前播放频率
    radio_get_freq();

    //保存搜索起始频率
    startfreq = g_engine_status.FM_CurrentFreq;

    g_radio_config.FMStatus.freq = startfreq;

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_manual_search_exit(void)
 * \radio 半自动搜台场景退出
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note     需确定退出后的频点和电台号，便于刷新界面
 */
/*******************************************************************************/
bool scene_manual_search_exit(void)
{
    uint8 num;

    //获取当前频率值
    if (radio_get_freq() == TRUE)
    {
        //正常获取，播放当前频点
        g_radio_config.FMStatus.freq = g_engine_status.FM_CurrentFreq;

        if (g_radio_config.FMStatus.freq > max_freq[g_radio_config.band_mode])
        {
            //大于最大频点
            g_radio_config.FMStatus.freq = startfreq;
        }

        if (g_radio_config.FMStatus.freq < min_freq[g_radio_config.band_mode])
        {
            //小于最小频点(87-87.5之间的值)
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        //获取失败，播放半自动搜台前频点
        g_radio_config.FMStatus.freq = startfreq;
    }

    //判断当前频点是否是已保存频点
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //切换后的频点是未保存频点
    if (num == 0xff)
    {
        g_radio_config.FMStatus.station = 0;
    }
    else
    {
        //切换后的频点是已保存频点
        g_radio_config.FMStatus.station = num;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 init_start_freq(FM_SeekDir_e dir)
 * \半自动搜台初始化起始频点
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       步长space
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
uint8 init_start_freq(FM_SeekDir_e dir)
{
    uint8 space;
    //搜台步进
    space = 100;
    //欧洲频段,步长为50
    if (g_radio_config.band_mode == Band_Europe)
    {
        //欧洲波段或者在用户模式，步进调整为50K
        space = 50;
    }

    //设置初始判断频率
    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
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
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //最小频点切换到最大
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }
    return space;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_search(FM_SeekDir_e dir)
 * \软件半自动搜台处理
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   一次设置一个频点进行tune，并判断是否真台
 * \         找到一个真台就停止，边界回绕，搜索一轮。
 */
/*******************************************************************************/
app_result_e deal_soft_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space;
    uint8 num;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;

    space = init_start_freq(dir);

    while (1)
    {
        //判断当前频点是否是已保存频点
        //并更新电台号
        num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.station = num;
        }

        //界面刷新,每个循环都刷
        radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);

        //判断当前频率是否有效台

        if (radio_set_softseek(g_radio_config.FMStatus.freq, dir) == TRUE)
        {
            //过滤96MHZ 及 108MHZ 频点
            if ((g_radio_config.FMStatus.freq != 0x17700) && (g_radio_config.FMStatus.freq != 0x1a5e0))
            {
                //是有效电台，完成退出
                break;
            }
        }

        //退出条件
        if (dir == DIR_UP)
        {
            //边界回绕
            if (g_radio_config.FMStatus.freq >= max_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
            }
            //搜完一轮，未找到有效电台，退出
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //更新频点
                g_radio_config.FMStatus.freq += space;
            }
        }
        else //DIR_DOWN
        {
            //边界回绕
            if (g_radio_config.FMStatus.freq <= min_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
            }
            //搜完一轮，未找到有效电台，退出
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //更新频点
                g_radio_config.FMStatus.freq -= space;
            }
        }

        //处理系统消息和gui 消息
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &gui_event, cancel_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_SEARCH_IGNORE)
                {
                    //忽略一些按键DOWN消息
                    continue;
                }
                else
                {
                    //过滤后续按键消息
                    com_filter_key_hold();
                }
            }
            else
            {
                //保存按键消息
                message_redeal_gui_msg(&gui_msg); //备份缓冲消息
            }

            break;
        }
        else
        {
            //一次循环处理一种类型的消息
            ret = scene_play_sysmsg();
        }

        if (ret == RESULT_REDRAW)
        {
            ret = RESULT_NULL;
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_manual_search(FM_SeekDir_e dir)
 * \radio 播放界面半自动搜台场景
 * \param[in]    dir   半自动搜台方向
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note  进入半自动搜台场景时，引擎应该已安装，模组也已
 * \        初始化，同时界面也不需全部刷新，只更新与频率
 * \        相关项即可，包括频率图片，电台号，tunebar等
 */
/*******************************************************************************/
app_result_e radio_scene_manual_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;

    if (scene_manual_search_init() == FALSE)
    {
        //返回正常播放场景
        return ret;
    }

    //搜台时快闪
    com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);

    led_display_icon(LCD_PAUSE, FALSE);
    led_display_icon(LCD_PLAY, TRUE);

    change_app_state (APP_STATE_PLAYING);

    //处理软件半自动搜台
    ret = deal_soft_search(dir);

    change_app_state (APP_STATE_NO_PLAY);

    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    scene_manual_search_exit();

    //play freq
    need_tts = TRUE;

    //设置频点播放
    radio_set_freq(g_radio_config.FMStatus.freq);

    return ret;
}
