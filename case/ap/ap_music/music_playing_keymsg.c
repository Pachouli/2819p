/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��  ǰ̨Ӧ�ð�����Ϣ����
 ********************************************************************************/

#include "app_music.h"

const key_map_t playing_key_map_list[] =
{
    /*! �̰���next�л���һ���¼� */
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    /*! �̰���prev�л���һ���¼� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    /*! ����next��� �¼� */
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FFOREWORD },
    /*! ����prev���� �¼� */
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FBACKWORD },
    /*! �̰���next�л���һ���¼� */
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    /*! �̰���prev�л���һ���¼� */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    /*!�̰�play����/��ͣ�¼� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },
    /*! ѭ��ģʽ�����¼� */
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_LOOPMODE_SET },
    /*! forder���¼� */
    { { KEY_FOLDER_ADD, 0, KEY_TYPE_SHORT_UP }, EVENT_FOLDER_ADD },
    /*! forder���¼� */
    { { KEY_FOLDER_SUB, 0, KEY_TYPE_SHORT_UP }, EVENT_FOLDER_SUB },

#if (SUPPORT_RECORD_FUNC == 1)
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ENTER_CARD_REC },
#endif

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

//�ֶ��л������󲥷���Ŀ��TTS
bool music_tts_play_song_seq(uint16 song_seq)
{
    uint8 tts_ids[10];
    uint8 section_cnt = 0;
    uint8 thousand, hundred, ten, one;
    bool number_bitmap = 0x00; //bit 3 ~ 0 �ֱ�Ϊǧ��ʮ��
    bool bret = TRUE;

    //�ر���Ŀ��TTS����
    if ((bool) com_get_config_default(MUSIC_ENABLE_SEQ_NUM_TTS) == FALSE)
    {
        return TRUE;
    }

    //������Ŀ��
    thousand = (uint8)(song_seq / 1000);
    hundred = (uint8)((song_seq % 1000) / 100);
    ten = (uint8)((song_seq % 100) / 10);
    one = (uint8)(song_seq % 10);

    //��
    tts_ids[section_cnt] = TTS_SECTION_NO;
    section_cnt++;
    //ǧλ������
    if (thousand > 0)
    {
        tts_ids[section_cnt] = TTS_SECTION_NUM_0 + thousand;
        section_cnt++;
        tts_ids[section_cnt] = TTS_SECTION_THOUSAND;
        section_cnt++;
        number_bitmap |= 0x08;
    }

    //��λ������
    if (hundred > 0)
    {
        tts_ids[section_cnt] = TTS_SECTION_NUM_0 + hundred;
        section_cnt++;
        tts_ids[section_cnt] = TTS_SECTION_HUNDRED;
        section_cnt++;
        number_bitmap |= 0x04;
    }
    else if ((number_bitmap & 0x08) != 0) //ǧλ����Ϊ0
    {
        tts_ids[section_cnt] = TTS_SECTION_NUM_0;
        section_cnt++;
    }
    else
    {
        //for QAC
    }

    //ʮλ������
    if (ten > 0)
    {
        tts_ids[section_cnt] = TTS_SECTION_NUM_0 + ten;
        section_cnt++;
        tts_ids[section_cnt] = TTS_SECTION_TEN;
        section_cnt++;
        number_bitmap |= 0x02;
    }
    else if ((number_bitmap & 0x04) != 0) //��λ����Ϊ0
    {
        tts_ids[section_cnt] = TTS_SECTION_NUM_0;
        section_cnt++;
    }
    else
    {
        //for QAC
    }

    //��λ������
    if (one > 0)
    {
        tts_ids[section_cnt] = TTS_SECTION_NUM_0 + one;
        section_cnt++;
    }

    //��
    tts_ids[section_cnt] = TTS_SECTION_SONG;
    section_cnt++;

    com_tts_state_play_list(tts_ids, section_cnt, 0);

    return bret;
}

//�ֶ��л�������ָ�����
bool music_switch_song_play(void)
{
    mengine_playinfo_t playinfo;

    //����л�������־
    music_get_playinfo(&playinfo);
    if(TRUE == music_play())
    {
        g_error_num = 0;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_mode(void)
 * \����mode������Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_mode(void)
{
    app_result_e switch_ret = RESULT_NULL;
    //int8 card_chcek = -1;

    if(g_enter_mode == PARAM_MUSIC_CARD_REPLAY)
    {
        //libc_printf("\r\n---- enter card music play ----\r\n");
        switch_ret = RESULT_MUSIC_CPLAY;
    }
    else if(g_enter_mode == PARAM_MUSIC_UHOST_REPLAY)
    {
        //libc_printf("\r\n---- enter card music play ----\r\n");
        switch_ret = RESULT_MUSIC_UPLAY;
    }
    else
    {
        //libc_printf("\r\n---- enter next function ----\r\n");
        switch_ret = RESULT_NEXT_FUNCTION;
    }

    return switch_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_folderadd(void)
 * \����folder+������Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_folderadd(void)
{
    //�л��ļ���
    //����ͬ����Ϣ
    music_send_msg(NULL, NULL, MSG_MENGINE_FOLFER_NEXT_SYNC);

    music_get_filepath (&g_file_path_info);

    //��ʾ��Ŀ��
    _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
    g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

    //�ֶ��и����������
    music_tts_play_song_seq(g_file_path_info.file_path.dirlocation.file_num);

    //�ֶ��и��ָ�����
    music_switch_song_play();

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_key_foldersub(void)
 * \����folder-������Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_foldersub(void)
{
    //�л��ļ���
    music_send_msg(NULL, NULL, MSG_MENGINE_FOLFER_PREV_SYNC);
    //Ϊ������ʹ��
    g_switch_prev = 1;

    music_get_filepath (&g_file_path_info);

    //��ʾ��Ŀ��
    _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
    g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

    //�ֶ��и����������
    music_tts_play_song_seq(g_file_path_info.file_path.dirlocation.file_num);
    //�ֶ��и��ָ�����
    music_switch_song_play();

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_gui(msg_gui_t *gui_msg)
 * \����gui��Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg)
{
    msg_apps_type_e cur_event;
    app_result_e key_result = RESULT_NULL;

    if ((g_music_status.play_status == PlayFast) || (g_music_status.fast_status == FFPlayTailSta)
            || (g_music_status.fast_status == FBPlayHeadSta))
    {
        if ((gui_msg->data.kmsg.type & KEY_TYPE_SHORT_UP) != 0)
        {
            //�����״̬�յ��κ�up��Ϣ��ת��Ϊ����������¼�
            cur_event = EVENT_PLAYING_NEXTSONG;
            goto event_deal;
        }
    }
    else
    {
        if (com_key_mapping(gui_msg, &cur_event, playing_key_map_list) == TRUE)
        {
            event_deal:
            //����gui�¼�
            switch (cur_event)
            {
                //������Ϣ���
                case EVENT_PLAYING_FFOREWORD:
                if ((g_music_status.play_status != PlayFast) && (g_music_status.play_status != PlaySta))
                {
                    //��ͣ״̬�£����˿������Ϣ
                    com_filter_key_hold(); //���˺���short up��Ϣ
                }
                else
                {
                    if (led_force_exit_msg_show() == TRUE)
                    {
                        led_clear_screen(CLR_MAIN);
                        last_minute = -1;
                        last_col = 0;
                    }
                    key_result = _scene_play_fast_deal(1);
                }
                break;
                //������Ϣ����
                case EVENT_PLAYING_FBACKWORD:
                if ((g_music_status.play_status != PlayFast) && (g_music_status.play_status != PlaySta))
                {
                    //��ͣ״̬�£����˿������Ϣ
                    com_filter_key_hold(); //���˺���short up��Ϣ
                }
                else
                {
                    if (led_force_exit_msg_show() == TRUE)
                    {
                        led_clear_screen(CLR_MAIN);
                        last_minute = -1;
                        last_col = 0;
                    }
                    key_result = _scene_play_fast_deal(0);
                }
                break;

                //������Ϣ��һ��
                case EVENT_PLAYING_PREVSONG:
                key_result = play_key_prev();
                break;

                //������Ϣ��һ��
                case EVENT_PLAYING_NEXTSONG:
                key_result = play_key_next();
                break;

                //��һ���ļ���
                case EVENT_FOLDER_ADD:
                key_result = play_key_folderadd();
                break;

                //��һ���ļ���
                case EVENT_FOLDER_SUB:
                key_result = play_key_foldersub();
                break;

                //������ͣ
                case EVENT_PLAYING_PAUSE:
                key_result = play_key_play();
                break;

                //loop mode set
                case EVENT_LOOPMODE_SET:
                key_result = play_key_repeat_next();
                break;

                case EVENT_ENTER_CARD_REC:
                com_filter_key_hold();
#if (SUPPORT_OK_MIC == 0)
                if ((g_enter_mode == PARAM_MUSIC_UHOST_DIR) || (g_enter_mode == PARAM_MUSIC_UHOST_REPLAY))
                {
                    cur_event = EVENT_ENTER_UHOST_REC;
                }
                key_result = com_message_box(cur_event);
#endif
                break;

                default:
                if (EVENT_MODE_SWITCH == cur_event)
                {
                    key_result = play_key_mode();
                    break;
                }
                key_result = com_message_box(cur_event);
                break;
            }
        }
    }

    return key_result;
}

