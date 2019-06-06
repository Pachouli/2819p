/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ǰ̨Ӧ�ò��ų���������Ϣ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_radio.h"

/* radio ���ų��������¼�ӳ���*/
const key_map_t playing_key_map_list[] =
{
    /*! �̰�NEXT ����һƵ���¼�*/
    { { KEY_CHADD, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! �̰�PREV ����һƵ���¼� */
    { { KEY_CHSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },

    /*! ����NEXT �������Զ���̨�¼�*/
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_NEXT },
    /*! ����PREV ����ǰ���Զ���̨�¼� */
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_PREV },

#if 1 //�����ר��CH+/CH-����NEXT/PREV��ΪԤ���̨�л�����
    /*!�̰�NEXT ��һ��̨�¼� */
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_PRESET },
    /*!�̰�NEXT ��һ��̨�¼� */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_PRESET },
    /*! �̰�KEY_NEXT_VOLADD ����һƵ���¼�*/
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_PRESET },
    /*! �̰�KEY_PREV_VOLSUB ����һƵ���¼� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_PRESET },
#else
    /*! �̰�NEXT ����һƵ���¼�*/
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! �̰�PREV ����һƵ���¼� */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },
    /*! �̰�KEY_NEXT_VOLADD ����һƵ���¼�*/
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! �̰�KEY_PREV_VOLSUB ����һƵ���¼� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },
#endif

    /*!�̰�play ��ͣ����/�ָ������л��¼� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PLAY_PAUSE },

    /*!�̰�F1 �Զ���̨�¼� */
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_AUTO_SEARCH },

    /*! ���ְ������� */
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP }, EVENT_NUMBER_INPUT },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

void set_freq(uint32 freq)
{
    uint8 num;

    //�ж��л����Ƶ���Ƿ��ڵ�ǰ�����ŵ�Ԥ���̨����
    num = whether_in_list(freq, pfreq);

    //�л����Ƶ����δ����Ƶ��
    if (num == 0xff)
    {
        //û����Ԥ���̨����
        g_radio_config.FMStatus.station = 0;
    }
    else
    {
        //�л����Ƶ�����ѱ���Ƶ��
        g_radio_config.FMStatus.station = num;

        //��̨�Ž���
        radio_show_station_num(g_radio_config.FMStatus.station, TRUE);
    }

    //ˢ��Ƶ�������ʾ
    radio_show_frequency(freq, FALSE);

    //����Ƶ�㲥��
    radio_set_freq(freq);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e change_freq(FM_SeekDir_e dir)
 * \�л�����һ������һ��Ƶ��
 * \param[in]    dir �л�����
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

    //����
    space = 100;

    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ��Ƶ��,  ��50kΪ����
        space = 50;
    }

    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
            //�����л�Ƶ��
            g_radio_config.FMStatus.freq += space;
        }
        else
        {
            //���Ƶ���л�����С
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
        {
            //�����л�
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //��СƵ���л������
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
 * \�л�����һ���ѱ����̨
 * \param[in]    direction �л���̨����0��ʾ�л�����һ����̨��1��ʾ�л�����һ����̨
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

    //ѭ������
    uint8 i, cur_num;
    //��̨Ƶ��ֵ
    uint32 station_freq;

    //Ԥ���б������ѱ����̨
    if (g_radio_config.FMStatus.station_count > 0)
    {
        if (g_radio_config.FMStatus.station == 0)
        {
            //��ǰ����Ԥ���б��е�Ƶ���ϣ��л�����һ��Ƶ��
            g_radio_config.FMStatus.station = 1;
        }
        else
        {
            if (direction == SELECT_NEXT_DIRECTION)
            {
                if (g_radio_config.FMStatus.station < g_radio_config.FMStatus.station_count)
                {
                    //�л���̨��
                    g_radio_config.FMStatus.station++;
                }
                else
                {
                    //���ص�һ��
                    g_radio_config.FMStatus.station = 1;
                }
            }
            else
            {
                if (g_radio_config.FMStatus.station > 1)
                {
                    //�л���̨��
                    g_radio_config.FMStatus.station--;
                }
                else
                {
                    //���ص�һ��
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
                //�ҵ���Ч̨���˳�
                break;
            }
            else
            {
                //���²���
                cur_num++;
                if (cur_num > g_radio_config.FMStatus.station_count)
                {
                    cur_num = 1;
                }
            }
        }
        //�ҵ���Ч��̨
        g_radio_config.FMStatus.station = cur_num;
        //ͨ����̨�Ż�ȡƵ��ֵ
        g_radio_config.FMStatus.freq = *(pfreq + g_radio_config.FMStatus.station - 1);

        //ˢ�µ�̨��
        radio_show_station_num(g_radio_config.FMStatus.station, TRUE);
    }
    else
    {
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
    }

    //ˢ��Ƶ�������ʾ
    radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);

    need_tts = TRUE;

    //����Ƶ�㲥��(û�б����̨Ҳ��Ҫ��������Ƶ��)
    radio_set_freq(g_radio_config.FMStatus.freq);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  play_next_station(void)
 * \�л�����һ���ѱ����̨
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
        //��ͣ����
        radio_pause();
    }

    ret = select_station(direction);

    if (need_tts == TRUE)
    {
        //play current freq
        radio_freq_tts_play(g_radio_config.FMStatus.freq);

        need_tts = FALSE;
    }

    //��ʼ����
    radio_resume();

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_play_search_deal(FM_SeekDir_e search_dir)
 * \�����Զ���̨������Ϣ
 * \param[in]    search_dir  para1 ���Զ���̨����
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up��Ϣ
 * \retval       false ���������
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
 * \����next��һ��Ƶ����Ϣ
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

    //mute�������л���̨����
    radio_set_mute (SetMUTE);

    ret_next = change_freq(DIR_UP);

    //�ͷž���
    radio_set_mute(releaseMUTE);

    //��ʼ����
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
 * \����prev��һ��Ƶ����Ϣ
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

    //mute�������л���̨����
    radio_set_mute (SetMUTE);

    ret_prev = change_freq(DIR_DOWN);

    //�ͷž���
    radio_set_mute(releaseMUTE);

    //��ʼ����
    if ((g_fmeg_status.play_status == StopSta) || (g_fmeg_status.play_status == PauseSta))
    {
        radio_resume();
    }

    return ret_prev;
}

app_result_e radio_play_number_input_freq(uint32 freq)
{
    //mute�������л���̨����
    radio_set_mute (SetMUTE);

    g_radio_config.FMStatus.freq = freq;
    set_freq(freq);

    //�ͷž���
    radio_set_mute(releaseMUTE);

    //��ʼ����
    if ((g_fmeg_status.play_status == StopSta) || (g_fmeg_status.play_status == PauseSta))
    {
        radio_resume();
    }

    return RESULT_NULL;
}

//��ͣ����/�ָ�����
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
 * \radio ���ų���gui��Ϣ����
 * \param[in]    cur_event  ������������Ϣ�¼�
 * \param[in]    cur_msg  ��ǰ��Ϣ,��������ѡ̨
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
            //�л�����һ��Ԥ���̨
            case EVENT_RADIO_NEXT_PRESET:
            led_force_exit_msg_show();
            result = play_next_station(SELECT_NEXT_DIRECTION);
            break;

            //�л�����һ��Ԥ���̨
            case EVENT_RADIO_PREV_PRESET:
            led_force_exit_msg_show();
            result = play_next_station(SELECT_PREV_DIRECTION);
            break;

            //�л�����һ��Ƶ��
            case EVENT_RADIO_NEXT_FREQ:
            led_force_exit_msg_show();
            result = radio_play_next_freq();
            break;

            //�л�����һ��Ƶ��
            case EVENT_RADIO_PREV_FREQ:
            led_force_exit_msg_show();
            result = radio_play_prev_freq();
            break;

            //�����Զ���̨
            case EVENT_RADIO_SEEK_NEXT:
            com_filter_key_hold(); //���˺���������Ϣ
            led_force_exit_msg_show();
            result = RESULT_RADIO_MANUAL_UP_SEARCH;
            break;

            //��ǰ���Զ���̨
            case EVENT_RADIO_SEEK_PREV:
            com_filter_key_hold(); //���˺���������Ϣ
            led_force_exit_msg_show();
            result = RESULT_RADIO_MANUAL_DOWN_SEARCH;
            break;

            //�Զ���̨�¼�����
            case EVENT_RADIO_AUTO_SEARCH:
            led_force_exit_msg_show();
            result = RESULT_RADIO_AUTO_SEARCH;
            break;

            //��ͣ����/�ָ������¼�����
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

                //�Ϸ��Լ��
                if (number_input_finish == TRUE)
                {
                    if ((g_number_input_freq*100 >= start_freq) && (g_number_input_freq*100 <= end_freq))
                    {
                        //�Ϸ�Ƶ�㣬�л�Ƶ��
                        radio_play_number_input_freq(g_number_input_freq*100);
                    }
                    else
                    {
                        //�Ƿ�Ƶ�㣬�ָ�ԭ��Ƶ����ʾ
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

        //������ǰ����ְ��������˳����ְ�������ģʽ
        if (keep_number_input == FALSE)
        {
            //�Ȼָ�ԭ��Ƶ����ʾ
            radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);
            g_number_input_keys = 0;
        }
    }

    return result;
}

