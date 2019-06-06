/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用播放场景主循环，包括初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_play_init(void)
 * \radio 播放场景初始化
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       success
 * \retval       fail
 * \note
 */
/*******************************************************************************/
bool scene_play_init(void)
{
    msg_apps_t msg;
    msg_reply_t msg_reply;
    bool bret = TRUE;
    fmeg_init_para_t tmp_fmeg_init_para;

    tmp_fmeg_init_para.freq_init = 0;
    tmp_fmeg_init_para.threshold = g_radio_search_th;

    if (g_radio_config.band_mode == Band_Japan)
    {
        //步长
        tmp_fmeg_init_para.span = 100;
        //最低频率
        tmp_fmeg_init_para.freq_low = 76000;
        //最高频率
        tmp_fmeg_init_para.freq_high = 91000;
        //去加重时间常数
        tmp_fmeg_init_para.de_emphasis_tc = DE_EMPHASIS_TC_50US;
    }
    else if (g_radio_config.band_mode == Band_Europe)
    {
        //步长
        tmp_fmeg_init_para.span = 50;
        //最低频率
        tmp_fmeg_init_para.freq_low = 87500;
        //最高频率
        tmp_fmeg_init_para.freq_high = 108000;
        //去加重时间常数
        tmp_fmeg_init_para.de_emphasis_tc = DE_EMPHASIS_TC_50US;
    }
    else
    {
        //步长
        tmp_fmeg_init_para.span = 100;
        //最低频率
        tmp_fmeg_init_para.freq_low = 87500;
        //最高频率
        tmp_fmeg_init_para.freq_high = 108000;
        //去加重时间常数
        tmp_fmeg_init_para.de_emphasis_tc = DE_EMPHASIS_TC_50US; //中国
        //tmp_fmeg_init_para.de_emphasis_tc = DE_EMPHASIS_TC_75US; //美国
    }

    //初始化模组
    msg.content.addr = &tmp_fmeg_init_para;
    //初始化FM 模组
    msg.type = MSG_FMENGINE_INIT;
    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }

    return bret;
}

//快速预览模式会使用该接口来预览，代码硬拷贝到 com_fast_preview_ui.c 里
#if 0
void bt_show_welcome_ui(uint8 station_num, uint32 freq)
{
    led_clear_screen(CLR_ALL);
    led_display_icon(LCD_FM, TRUE);
    led_display_icon(LCD_PAUSE, FALSE);

    if ((station_num != 0) && (station_num != 0xff))
    {
        //CH
        led_display(NUMBER1, 'C', TRUE);
        led_display(NUMBER2, 'H', TRUE);
        //电台号
        led_display(NUMBER3, station_num/10, TRUE);
        led_display(NUMBER4, station_num%10, TRUE);
    }
    else
    {
        if (freq >= 100000)
        {
            led_display(NUMBER1, 1, TRUE);
        }
        else
        {
            led_display(NUMBER1, 0, FALSE);
        }
        freq = freq % 100000;
        led_display(NUMBER2, freq/10000, TRUE);
        freq = freq % 10000;
        led_display(NUMBER3, freq/1000, TRUE);
        led_display_icon(LCD_P1, TRUE);
        freq = freq % 1000;
        led_display(NUMBER4, freq/100, TRUE);
    }
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_playing(void)
 * \进入radio  播放场景界面
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e radio_scene_playing(bool first_flag)
{
    app_result_e loop_deal_ret = RESULT_NULL;
    uint8 num;
    bool enter_tts = FALSE;
    tts_play_ret_e tts_enter_ret;
    bool need_switch_ap = FALSE;
    bool need_flush_key = FALSE;

    g_fmeg_status.play_status = PlaySta;

    if (first_flag == TRUE)
    {
        //ESD RESET 不要播报TTS
        if ((g_p_global_var->bg_app_sta != BG_STATUS_BACK) && (g_p_esd_bk_info->reset_flag == 0))
        {
            enter_tts = TRUE;
        }
    }

    //ESD RESET 且断点状态为 StopSta/PauseSta 则不启动播放
    if ((g_p_esd_bk_info->reset_flag == 1)
        && ((g_p_esd_bk_info->play_status == StopSta) || (g_p_esd_bk_info->play_status == PauseSta)))
    {
        //ESD RESET前是暂停状态，不要 radio_resume
        g_fmeg_status.play_status = PauseSta;
        need_tts = FALSE;
    }
    else if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK)
        && ((g_p_global_var->play_status_before == StopSta) || (g_p_global_var->play_status_before == PauseSta)))
    {
        //打电话前是暂停状态，不要 radio_resume
        g_fmeg_status.play_status = PauseSta;
        need_tts = FALSE;
    }
    g_p_global_var->bg_app_sta = BG_STATUS_NULL;

    if (g_p_global_var->fp_switch_jump_welcome == FALSE)
    {
        if (first_flag == TRUE)
        {
            led_clear_screen(CLR_ALL);
            led_display_icon(LCD_FM, TRUE);
        }

        if (g_fmeg_status.play_status == PlaySta)
        {
            led_display_icon(LCD_PAUSE, FALSE);
            led_display_icon(LCD_PLAY, TRUE);
        }
        else
        {
            led_display_icon(LCD_PLAY, FALSE);
            led_display_icon(LCD_PAUSE, TRUE);
        }
    }

    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
    if (num != 0xff)
    {
        //切换后的频点是已保存频点
        g_radio_config.FMStatus.station = num;

        //电台号界面
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            radio_show_station_num(g_radio_config.FMStatus.station, TRUE);
        }
    }
    else
    {
        //显示频点电台
        if (g_p_global_var->fp_switch_jump_welcome == FALSE)
        {
            radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);
        }
    }

    g_p_global_var->fp_switch_jump_welcome = FALSE;

#if (SUPPORT_FAST_PREVIEW == 1)
    //更新预览信息
    g_radio_config.preview_station_num = num;
    g_radio_config.preview_freq = g_radio_config.FMStatus.freq;
    com_fast_preview_update_ap_welcome_ui(APP_FUNC_RADIO, &g_radio_config);
#endif

    if (first_flag == TRUE)
    {
        //初始化后FM处于静音状态
        g_engine_status.FM_Mute_Status = SetMUTE;

        //设置音量
        com_reset_sound_volume(SET_VOL_BY_SYSTEM);

#if (SUPPORT_FAST_PREVIEW == 1)

        if (get_bt_state() == BT_STATE_WORK)
        {
            //如果不支持后台蓝牙，所有非蓝牙应用都与 bt enging 冲突，需要杀死 bt engine
#ifdef FM_THRESHOLD_TEST //FM门限值测试需要使用蓝牙的空间
            if (1)
#else
            if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
#endif
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
                private_msg_t play_msg;
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
                    if (get_app_msg(THIS_APP_TYPE, &play_msg) == TRUE)
                    {
                        loop_deal_ret = radio_app_msg_callback(&play_msg);
                        if ((loop_deal_ret != RESULT_NULL) && (loop_deal_ret != RESULT_REDRAW))
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

        if (enter_tts == TRUE)
        {
            //语音提示
            tts_enter_ret = com_tts_state_play(TTS_ENTER_RADIO, 0);
        }

#else

        if (enter_tts == TRUE)
        {
            //语音提示
            tts_enter_ret = com_tts_state_play(TTS_ENTER_RADIO, 0);
        }

        if (get_bt_state() == BT_STATE_WORK)
        {
            //如果不支持后台蓝牙，所有非蓝牙应用都与 bt enging 冲突，需要杀死 bt engine
#ifdef FM_THRESHOLD_TEST //FM门限值测试需要使用蓝牙的空间
            if (1)
#else
            if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
#endif
            {
                com_close_bt_engine();
            }
        }

#endif

        _create_engine();

        if (scene_play_init() == FALSE)
        {
            return RESULT_NEXT_FUNCTION;
        }

        radio_get_freq();

        //for SPP, 同步电台频点，必须在第一次com_tts_state_play之前做，否则APK会获取到错误的频点
        if (g_radio_config.FMStatus.freq != g_engine_status.FM_CurrentFreq)
        {
            //need_reset_freq = TRUE;
            //选择正在播放进入, 不重设频点, 避免出现一下短暂杂音
            radio_set_freq(g_radio_config.FMStatus.freq); //not release mute because need tts is true
        }
        g_engine_status.FM_CurrentFreq = g_radio_config.FMStatus.freq; //使用radio_get_freq会获取到错误的频点
    }

    if ((first_flag == TRUE) && (enter_tts == TRUE))
    {
        if ((tts_enter_ret == TTS_PLAY_RET_BY_KEY) || (tts_enter_ret == TTS_PLAY_RET_BY_SYS))
        {
            //如果播报“收音机”被终止了，那么FM频点就不再播报了
            need_tts = FALSE;
        }
    }

    if (need_tts == TRUE)
    {
        //play current freq
        radio_freq_tts_play(g_radio_config.FMStatus.freq);

        need_tts = FALSE;
    }

    //释放静音
    radio_set_mute(releaseMUTE);

    if (g_fmeg_status.play_status == PlaySta)
    {
        //从pause到play
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        //开始收听
        radio_resume();
    }

    loop_deal_ret = _playing_loop_deal();

    if (g_fmeg_status.play_status == PlaySta)
    {
        //暂停收听
        radio_pause();

        //从play到pause
        com_speaker_off(SPK_PLAY);
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        g_last_fmeg_status = StopSta;
    }

    //mute
    radio_set_mute (SetMUTE);

    fast_switch_ap_exit:

    return loop_deal_ret;
}

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
void _playing_check_status(void)
{
    radio_get_play_status (&g_fmeg_status); //获取状态信息

    if ((g_last_fmeg_status == PlaySta) && (g_fmeg_status.play_status != PlaySta))
    {
        //从play到pause
        com_speaker_off(SPK_PLAY);
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PLAY, FALSE);
        led_display_icon(LCD_PAUSE, TRUE);
    }
    else if ((g_last_fmeg_status != PlaySta) && (g_fmeg_status.play_status == PlaySta))
    {
        //从pause到play
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PAUSE, FALSE);
        led_display_icon(LCD_PLAY, TRUE);
    }

    g_last_fmeg_status = g_fmeg_status.play_status;

    //ESD RESET 实时断点备份
    g_p_esd_bk_info->play_status = g_fmeg_status.play_status;
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
app_result_e _playing_loop_deal(void)
{
    app_result_e ret = RESULT_NULL;
    input_gui_msg_t gui_msg;
    bool need_redraw = FALSE;

    //radio 播放场景循环
    while (1)
    {
        //从引擎获取当前状态信息
        _playing_check_status();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
        com_record_update_record_status(g_fmeg_status.record_sta);
#endif

        if (need_redraw == TRUE)
        {
            led_clear_screen(CLR_MAIN);
            //显示频点电台
            radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);

            need_redraw = FALSE;
        }

        //获取和处理GUI消息、私有消息
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            ret = scene_play_guimsg(&gui_msg);
        }
        else
        {
            //一次循环处理一种类型的消息
            ret = scene_play_sysmsg();
        }

        if (ret == RESULT_NULL)
        {
            //数字按键输入模式8S超时
            if ((g_number_input_keys > 0) && ((sys_get_ab_timer() - g_number_input_timer) > 8000))
            {
                g_number_input_keys = 0;
                ret = RESULT_REDRAW;
            }
        }

        if (ret == RESULT_REDRAW)
        {
            need_redraw = TRUE;
            ret = RESULT_NULL;
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }

#if (SUPPORT_ASET_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            aset_test_loop_deal();
        }
#endif

        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }

    return ret;
}

