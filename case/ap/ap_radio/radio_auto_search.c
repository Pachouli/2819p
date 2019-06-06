/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用自动搜台，全频段搜索一遍或者多遍。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

const uint32 border_freq[][2] =
{
    //dir_down,dir_up
    { MAX_US, MIN_US },
    { MAX_JP, MIN_JP },
    { MAX_EU, MIN_EU },
};

bool key_cancel_seek = FALSE;

void get_border_freq(uint32 *start_freq, uint32 *end_freq, FM_SeekDir_e dir)
{
    *start_freq = border_freq[g_radio_config.band_mode][dir];
    *end_freq = border_freq[g_radio_config.band_mode][1-dir];
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 fliter_exist_freq(uint32 input_freq,uint8 cur_max)
 * \过滤已保存的频点
 * \param[in]    input_freq param1 搜到的频点
 * \param[in]    cur_max param2 当前的最大序号
 * \param[out]   none
 * \return   存储列表的序号
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
uint8 fliter_exist_freq(uint32 input_freq, uint8 cur_max)
{
    uint8 number;
    for (number = 0; number < cur_max; number++)
    {
        if (input_freq == g_radio_config.Auto_tab[(number)])
        {
            break;
        }
    }
    return number;
}

#ifdef FM_THRESHOLD_TEST
void fm_threshold_test_sort_print(void)
{
    int i, j, k, totol_step, space;
    uint32 start_freq, end_freq;
    uint16 thres_sames[MAX_TH_VALUE-MIN_TH_VALUE+1];

    get_border_freq(&start_freq, &end_freq, DIR_UP);

    //搜台步进
    space = 100;
    if (g_radio_config.band_mode == Band_Europe)
    {
        //欧洲波段, 步进调整为50K
        space = 50;
    }
    totol_step = (end_freq-start_freq)/space + 1;

    libc_memset(thres_sames, 0, sizeof(thres_sames));

    for (i = 0; i <= (MAX_TH_VALUE - MIN_TH_VALUE); i++)
    {
        for (j = 0; j < totol_step; j++)
        {
            if (fm_threshold_values[j] == (MAX_TH_VALUE-i))
            {
                thres_sames[i]++;
            }
        }
    }

    for (i = 0; i <= (MAX_TH_VALUE - MIN_TH_VALUE); i++)
    {
        if (thres_sames[i] > 0)
        {
            libc_printf("TH=%d has %d:", (MAX_TH_VALUE-i), thres_sames[i]);
            k = 0;
            for (j = 0; j < totol_step; j++)
            {
                if (fm_threshold_values[j] == (MAX_TH_VALUE-i))
                {
                    k++;
                    if (k < thres_sames[i])
                    {
                        libc_printf("%d,", start_freq+j*space);
                    }
                    else
                    {
                        libc_printf("%d\n", start_freq+j*space);
                    }
                }
            }
        }
    }
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_auto_seek(FM_SeekDir_e direction)
 * \软件全自动搜台处理
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   一次设置一个频点进行tune，并判断是否真台
 */
/*******************************************************************************/
app_result_e deal_soft_auto_seek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space, tab_num;
    uint32 seek_freq, start_freq, end_freq;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    bool audition_flag = FALSE; //是否处于试听模式
    uint32 audition_start_time=0;

    //搜台步进
    space = 100;
    if (g_radio_config.band_mode == Band_Europe)
    {
        //欧洲波段, 步进调整为50K
        space = 50;
    }

#ifdef FM_THRESHOLD_TEST
    dir = DIR_UP;
#endif

    //获取起始频点
    get_border_freq(&start_freq, &end_freq, dir);
    seek_freq = start_freq;

    //电台个数
    tab_num = 0;
    key_cancel_seek = FALSE;

    radio_set_throd(g_radio_search_th);

    while (1)
    {
        if (audition_flag == TRUE)
        {
            if ((sys_get_ab_timer() - audition_start_time) > ((uint32)g_radio_seek_audition_time*1000))
            {
                audition_flag = FALSE;

                //恢复自动搜台
                com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);

                //暂停收听
                radio_pause();

                //mute
                radio_set_mute (SetMUTE);
            }

            //试听结束会判断电台列表是否已满
            if (audition_flag == FALSE)
            {
                //电台列表已满
                if (tab_num >= MAX_STATION_COUNT)
                {
                    break;
                }
            }
        }
        else
        {
            bool new_station = FALSE;

#ifdef FM_THRESHOLD_TEST
            uint16 temp_threshold = MAX_TH_VALUE;

            //判断当前频率是否有效台
            while (1)
            {
                radio_set_throd(temp_threshold);

                if (radio_set_softseek(seek_freq, dir) == TRUE)
                {
                    //保存有效电台
                    if (temp_threshold >= g_radio_search_th)
                    {
                        uint8 i = fliter_exist_freq(seek_freq, tab_num);
                        if (i == tab_num)
                        {
                            new_station = TRUE;
                        }
                    }
                    fm_threshold_values[(seek_freq-start_freq)/space] = (int16) temp_threshold;
                    libc_printf("TH : %d\n", temp_threshold);
                    break;
                }

                if (temp_threshold == MIN_TH_VALUE)
                {
                    fm_threshold_values[(seek_freq-start_freq)/space] = -1;
                    libc_printf("TH : Fail!\n", temp_threshold);
                    break;
                }
                else
                {
                    temp_threshold--;
                }
            }
#else
            //判断当前频率是否有效台
            if (radio_set_softseek(seek_freq, dir) == TRUE)
            {
                //保存有效电台
                uint8 i = fliter_exist_freq(seek_freq, tab_num);
                if (i == tab_num)
                {
                    new_station = TRUE;
                }
            }
#endif

            if (new_station == TRUE)
            {
                g_radio_config.Auto_tab[tab_num] = seek_freq;
                tab_num++;

                //刷新电台号
                radio_show_station_num(tab_num, FALSE);

                //进入试听模式
                if (g_radio_seek_audition_enable == TRUE)
                {
                    audition_flag = TRUE;
                    audition_start_time = sys_get_ab_timer();

                    //打开试听
                    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

                    //释放静音
                    radio_set_mute(releaseMUTE);

                    //开始收听
                    radio_resume();
                }
            }
        }

        if (audition_flag == FALSE)
        {
            //电台列表已满
            if (tab_num >= MAX_STATION_COUNT)
            {
                break;
            }

            if (dir == DIR_UP)
            {
                if ((seek_freq + space) > end_freq)
                {
                    //结束扫描,回到播放场景
                    break;
                }
                else
                {
                    //更新频点
                    seek_freq += space;
                }
            }
            else
            {

                if ((seek_freq - space) < end_freq)
                {
                    //结束扫描,回到播放场景
                    break;
                }
                else
                {
                    //更新频点
                    seek_freq -= space;
                }
            }

            //刷新频点
            radio_show_frequency(seek_freq, FALSE);
        }

        //处理系统消息和gui 消息
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            //任何按键,取消扫描
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
            key_cancel_seek = TRUE;
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
        //挂起20ms 多任务交互
        sys_os_time_dly(1);
    }

    if (audition_flag == TRUE)
    {
        //暂停收听
        radio_pause();

        //mute
        radio_set_mute (SetMUTE);
    }

    //保存电台数
    g_radio_config.FMStatus.station_count = tab_num;

#ifdef FM_THRESHOLD_TEST
    fm_threshold_test_sort_print();
#endif

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_auto_search_init(FM_SeekDir_e dir)
 * \全自动搜台场景参数初始化
 * \param[in]    dir: 搜台方向
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_auto_search_init(FM_SeekDir_e dir)
{
    //清空当前菜单波段的电台列表
    if (g_radio_config.band_mode == Band_China_US)
    {
        //普通波段
        g_radio_config.FMStatus.freq = MIN_US;
    }
    else if (g_radio_config.band_mode == Band_Japan)
    {
        //日本波段
        g_radio_config.FMStatus.freq = MIN_JP;
    }
    else
    {
        //欧洲波段
        g_radio_config.FMStatus.freq = MIN_EU;
    }
    //清除电台个数、当前电台号
    g_radio_config.FMStatus.station = 0;
    g_radio_config.FMStatus.station_count = 0;

    //清空缓存电台列表
    libc_memset(g_radio_config.Auto_tab, 0, MAX_STATION_COUNT * 4);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_auto_search_exit(void)
 * \全自动搜台场景退出
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_auto_search_exit(void)
{
    uint32 play_freq;

    if (g_radio_config.FMStatus.station_count != 0)
    {
        //有搜索到的电台，选择第一个播放
        if (key_cancel_seek == TRUE)
        {
            g_radio_config.FMStatus.station = g_radio_config.FMStatus.station_count;
        }
        else
        {
            g_radio_config.FMStatus.station = 1;
        }
        g_radio_config.FMStatus.freq = g_radio_config.Auto_tab[g_radio_config.FMStatus.station - 1];
    }
    else
    {
        //一个电台也未搜索到，选择默认频点播放
        g_radio_config.FMStatus.station = 0;
        if (g_radio_config.band_mode == Band_Japan)
        {
            play_freq = MIN_JP;
        }
        else
        {
            play_freq = MIN_US;
        }
        g_radio_config.FMStatus.freq = play_freq;
    }

    //搜台结束后保存电台列表
    sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_auto_search(FM_SeekDir_e dir)
 * \全自动搜台场景
 * \param[in]    mode:  搜台模式( 硬件搜台or  软件搜台)
 * \param[in]    dir: 搜台方向( 向上或向下)
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note 全自动搜台，清除当前菜单波段对应的列表
 * \       并按照当前菜单波段进行搜台
 */
/*******************************************************************************/
app_result_e radio_scene_auto_search(FM_SeekDir_e dir)
{
    app_result_e result;

    /* 初始化*/
    if (radio_auto_search_init(dir) == FALSE)
    {
        //初始化失败返回
        return RESULT_ERROR;
    }

    //搜台时快闪
    com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);

    led_display_icon(LCD_PAUSE, FALSE);
    led_display_icon(LCD_PLAY, TRUE);

    change_app_state (APP_STATE_PLAYING);

    //处理软件全自动搜台
    result = deal_soft_auto_seek(dir);

    change_app_state (APP_STATE_NO_PLAY);

    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    radio_auto_search_exit();

    //play freq
    need_tts = TRUE;

    //设置频点播放
    radio_set_freq(g_radio_config.FMStatus.freq);

    return result;
}
