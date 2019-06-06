/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ǰ̨Ӧ�ð��Զ���̨���ѵ�һ����Ч��̨�ͻ�ͣ������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool  bool scene_manual_search_init(void)
 * \radio ���Զ���̨������ʼ��
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

    //ȡ�õ�ǰ����Ƶ��
    radio_get_freq();

    //����������ʼƵ��
    startfreq = g_engine_status.FM_CurrentFreq;

    g_radio_config.FMStatus.freq = startfreq;

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_manual_search_exit(void)
 * \radio ���Զ���̨�����˳�
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note     ��ȷ���˳����Ƶ��͵�̨�ţ�����ˢ�½���
 */
/*******************************************************************************/
bool scene_manual_search_exit(void)
{
    uint8 num;

    //��ȡ��ǰƵ��ֵ
    if (radio_get_freq() == TRUE)
    {
        //������ȡ�����ŵ�ǰƵ��
        g_radio_config.FMStatus.freq = g_engine_status.FM_CurrentFreq;

        if (g_radio_config.FMStatus.freq > max_freq[g_radio_config.band_mode])
        {
            //�������Ƶ��
            g_radio_config.FMStatus.freq = startfreq;
        }

        if (g_radio_config.FMStatus.freq < min_freq[g_radio_config.band_mode])
        {
            //С����СƵ��(87-87.5֮���ֵ)
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        //��ȡʧ�ܣ����Ű��Զ���̨ǰƵ��
        g_radio_config.FMStatus.freq = startfreq;
    }

    //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //�л����Ƶ����δ����Ƶ��
    if (num == 0xff)
    {
        g_radio_config.FMStatus.station = 0;
    }
    else
    {
        //�л����Ƶ�����ѱ���Ƶ��
        g_radio_config.FMStatus.station = num;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 init_start_freq(FM_SeekDir_e dir)
 * \���Զ���̨��ʼ����ʼƵ��
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       ����space
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
uint8 init_start_freq(FM_SeekDir_e dir)
{
    uint8 space;
    //��̨����
    space = 100;
    //ŷ��Ƶ��,����Ϊ50
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲��λ������û�ģʽ����������Ϊ50K
        space = 50;
    }

    //���ó�ʼ�ж�Ƶ��
    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
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
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //��СƵ���л������
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }
    return space;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_search(FM_SeekDir_e dir)
 * \������Զ���̨����
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   һ������һ��Ƶ�����tune�����ж��Ƿ���̨
 * \         �ҵ�һ����̨��ֹͣ���߽���ƣ�����һ�֡�
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
        //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
        //�����µ�̨��
        num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.station = num;
        }

        //����ˢ��,ÿ��ѭ����ˢ
        radio_show_frequency(g_radio_config.FMStatus.freq, FALSE);

        //�жϵ�ǰƵ���Ƿ���Ч̨

        if (radio_set_softseek(g_radio_config.FMStatus.freq, dir) == TRUE)
        {
            //����96MHZ �� 108MHZ Ƶ��
            if ((g_radio_config.FMStatus.freq != 0x17700) && (g_radio_config.FMStatus.freq != 0x1a5e0))
            {
                //����Ч��̨������˳�
                break;
            }
        }

        //�˳�����
        if (dir == DIR_UP)
        {
            //�߽����
            if (g_radio_config.FMStatus.freq >= max_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
            }
            //����һ�֣�δ�ҵ���Ч��̨���˳�
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //����Ƶ��
                g_radio_config.FMStatus.freq += space;
            }
        }
        else //DIR_DOWN
        {
            //�߽����
            if (g_radio_config.FMStatus.freq <= min_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
            }
            //����һ�֣�δ�ҵ���Ч��̨���˳�
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //����Ƶ��
                g_radio_config.FMStatus.freq -= space;
            }
        }

        //����ϵͳ��Ϣ��gui ��Ϣ
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &gui_event, cancel_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_SEARCH_IGNORE)
                {
                    //����һЩ����DOWN��Ϣ
                    continue;
                }
                else
                {
                    //���˺���������Ϣ
                    com_filter_key_hold();
                }
            }
            else
            {
                //���水����Ϣ
                message_redeal_gui_msg(&gui_msg); //���ݻ�����Ϣ
            }

            break;
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
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
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_manual_search(FM_SeekDir_e dir)
 * \radio ���Ž�����Զ���̨����
 * \param[in]    dir   ���Զ���̨����
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note  ������Զ���̨����ʱ������Ӧ���Ѱ�װ��ģ��Ҳ��
 * \        ��ʼ����ͬʱ����Ҳ����ȫ��ˢ�£�ֻ������Ƶ��
 * \        �����ɣ�����Ƶ��ͼƬ����̨�ţ�tunebar��
 */
/*******************************************************************************/
app_result_e radio_scene_manual_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;

    if (scene_manual_search_init() == FALSE)
    {
        //�����������ų���
        return ret;
    }

    //��̨ʱ����
    com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);

    led_display_icon(LCD_PAUSE, FALSE);
    led_display_icon(LCD_PLAY, TRUE);

    change_app_state (APP_STATE_PLAYING);

    //����������Զ���̨
    ret = deal_soft_search(dir);

    change_app_state (APP_STATE_NO_PLAY);

    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    scene_manual_search_exit();

    //play freq
    need_tts = TRUE;

    //����Ƶ�㲥��
    radio_set_freq(g_radio_config.FMStatus.freq);

    return ret;
}
