/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��  �������ܴ���
 ********************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_fast_keyup(void)
 * \�������˵���Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up��Ϣ
 * \retval       false ���������
 * \ingroup      music_playing_keydeal.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_fast_keyup(void)
{
    input_gui_msg_t ui_msg;

    bool key_ret = FALSE;

    //�κΰ�����̧��������˳������
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
 * \�������˰�����Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up��Ϣ
 * \retval       false ���������
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
 * \����prev ������Ϣ
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
    //����Ϣ���У����������İ�����Ϣ
    sys_mq_flush (MQ_ID_GUI);
    //�����ʱȡ�������
    if ((g_music_status.play_status == PlayFast) || (g_music_status.fast_status == FFPlayTailSta)
            || (g_music_status.fast_status == FBPlayHeadSta))
    {
        music_cancel_ffb();
    }
    else
    {
        //Ϊ������ʹ��
        g_switch_prev = 1;

        music_play_prev();
        g_error_num = 0;
        music_get_filepath (&g_file_path_info);

        //��ʾ��Ŀ��
        _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
        g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

        //�ֶ��и����������
        music_tts_play_song_seq(g_file_path_info.file_path.dirlocation.file_num);
        //�ֶ��и��ָ�����
        music_switch_song_play();
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_next(void)
 * \����next������Ϣ
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
    //����Ϣ���У����������İ�����Ϣ
    sys_mq_flush (MQ_ID_GUI);
    //ͣ�������
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

        //��ʾ��Ŀ��
        _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
        g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

        //�ֶ��и����������
        music_tts_play_song_seq(g_file_path_info.file_path.dirlocation.file_num);
        //�ֶ��и��ָ�����
        music_switch_song_play();
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_playfast(uint8 direct)
 * \����prev long������Ϣ
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
        if (g_music_status.play_status == PlaySta) //����ʱ����������
        {
            //���ÿ����״̬
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
        //�յ�up��Ϣ���˳������
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
 * \����next������Ϣ
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
 * \����repeat������Ϣ
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
    //����loopģʽ
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
 * \����repeat������Ϣ
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
    //����loopģʽ

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

