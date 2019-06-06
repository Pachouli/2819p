/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌前台应用  按键功能处理
 ********************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_fast_keyup(void)
 * \处理快进退的消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up消息
 * \retval       false 继续快进退
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_fast_keyup(void)
{
    input_gui_msg_t ui_msg;

    bool key_ret = FALSE;

    //任何按键的抬起键都会退出快进退
    while (get_gui_msg(&ui_msg) == TRUE)
    {
        if ((ui_msg.data.kmsg.type & KEY_TYPE_SHORT_UP) == 0)
        {
            continue;
        }
        else
        {
            key_ret = TRUE;
            break;
        }
    }
    return key_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_play_fast_deal(uint8 ffb)
 * \处理快进退按键消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up消息
 * \retval       false 继续快进退
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_fast_deal(uint8 ffb)
{
    app_result_e deal_retval;

    deal_retval = play_key_playfast(ffb);

    return deal_retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_prev(void* param)
 * \处理prev 按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_prev(void)
{
    //清消息队列，不处理过多的按键消息
    sys_mq_flush (MQ_ID_GUI);
    //快进退时取消快进退
    if ((g_music_status.play_status == PlayFast) || (g_music_status.fast_status == FFPlayTailSta)
            || (g_music_status.fast_status == FBPlayHeadSta))
    {
        music_cancel_ffb();
    }
    else
    {
        //为出错处理使用
        g_switch_prev = 1;

        music_play_prev();
        g_error_num = 0;
        music_get_filepath (&g_file_path_info);

        //显示曲目号
        _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
        g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

        //手动切歌后语音播报
        music_tts_play_song_seq(g_file_path_info.file_path.dirlocation.file_num);
        //手动切歌后恢复播放
        music_switch_song_play();
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_next(void)
 * \处理next按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_next(void)
{
    //清消息队列，不处理过多的按键消息
    sys_mq_flush (MQ_ID_GUI);
    //停快进快退
    if ((g_music_status.play_status == PlayFast) || (g_music_status.fast_status == FFPlayTailSta)
            || (g_music_status.fast_status == FBPlayHeadSta))
    {
        music_cancel_ffb();
    }
    else
    {
        music_play_next();
        g_error_num = 0;
        music_get_filepath (&g_file_path_info);

        //显示曲目号
        _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
        g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

        //手动切歌后语音播报
        music_tts_play_song_seq(g_file_path_info.file_path.dirlocation.file_num);
        //手动切歌后恢复播放
        music_switch_song_play();
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_playfast(uint8 direct)
 * \处理prev long按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_playfast(uint8 direct)
{
    if (g_music_status.play_status != PlayFast)
    {
        if (g_music_status.play_status == PlaySta) //播放时才允许快进退
        {
            //设置快进退状态
            if ((direct == 1) && (g_music_status.fast_status == FBPlay_Null))
            {
                music_fast_forward();
            }
            else if ((direct == 0) && (g_music_status.fast_status == FBPlay_Null))
            {
                music_fast_backward();
            }
        }
    }
    else
    {
        //收到up消息，退出快进退
        if (_scene_play_fast_keyup() == TRUE)
        {
            music_cancel_ffb();
        }
    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_play(void)
 * \处理next按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_play(void)
{
    if ((g_music_status.play_status == PauseSta) || (g_music_status.play_status == StopSta))
    {
        com_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
        music_resume();
    }
    else if (g_music_status.play_status == PlaySta)
    {
        //pause
        music_pause();
        com_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
    }
    else
    {
        ; //nothing
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_repeat_next(void)
 * \处理repeat按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
void _show_loopmode(uint32 loopmode)
{
    led_force_exit_msg_show();
    led_clear_screen(CLR_MAIN);

    led_display(NUMBER2, 'L', TRUE);
    led_display(NUMBER3, 'P', TRUE);
    led_display(NUMBER4, loopmode, TRUE);

    led_enter_msg_show(3000);
}
app_result_e play_key_repeat_next(void)
{
    //设置loop模式
    //fsel_mode_e loop_type;
    uint8 loopmode_index;

    switch (g_music_config.repeat_mode)
    {
        case FSEL_MODE_LOOPALL:
        //next one is loop one
        g_music_config.repeat_mode = FSEL_MODE_LOOPONE;
        loopmode_index = 2;
        libc_printf_info("repeat mode : LOOPONE\n", 0, 0);
        break;

        case FSEL_MODE_LOOPONE:
        //next one is random
        g_music_config.repeat_mode = FSEL_MODE_NORMAL;
        loopmode_index = 3;
        libc_printf_info("repeat mode : NORMAL\n", 0, 0);
        break;

        case FSEL_MODE_NORMAL:
        //next one is loop all
        g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        loopmode_index = 1;
        libc_printf_info("repeat mode : LOOPALL\n", 0, 0);
        break;

        default:
        g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        loopmode_index = 1;
        libc_printf_info("repeat mode : LOOPALL\n", 0, 0);
        break;
    }
    music_set_playmode(g_music_config.repeat_mode);

    _show_loopmode(loopmode_index);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_repeat_prev(void)
 * \处理repeat按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_repeat_prev(void)
{
    //设置loop模式

    switch (g_music_config.repeat_mode)
    {
        case FSEL_MODE_LOOPALL:
        //last one is random
        g_music_config.repeat_mode = FSEL_MODE_RANOM;
        break;

        case FSEL_MODE_LOOPONE:
        //last one is loop all
        g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        break;

        case FSEL_MODE_RANOM:
        //last one is loop one
        g_music_config.repeat_mode = FSEL_MODE_LOOPONE;
        break;
        default:
        g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        break;
    }
    music_set_playmode(g_music_config.repeat_mode);

    return RESULT_NULL;
}

