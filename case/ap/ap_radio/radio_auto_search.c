/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ǰ̨Ӧ���Զ���̨��ȫƵ������һ����߶�顣
 * ���ߣ�cailizhen
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
 * \�����ѱ����Ƶ��
 * \param[in]    input_freq param1 �ѵ���Ƶ��
 * \param[in]    cur_max param2 ��ǰ��������
 * \param[out]   none
 * \return   �洢�б�����
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

    //��̨����
    space = 100;
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲���, ��������Ϊ50K
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
 * \���ȫ�Զ���̨����
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   һ������һ��Ƶ�����tune�����ж��Ƿ���̨
 */
/*******************************************************************************/
app_result_e deal_soft_auto_seek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space, tab_num;
    uint32 seek_freq, start_freq, end_freq;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    bool audition_flag = FALSE; //�Ƿ�������ģʽ
    uint32 audition_start_time=0;

    //��̨����
    space = 100;
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲���, ��������Ϊ50K
        space = 50;
    }

#ifdef FM_THRESHOLD_TEST
    dir = DIR_UP;
#endif

    //��ȡ��ʼƵ��
    get_border_freq(&start_freq, &end_freq, dir);
    seek_freq = start_freq;

    //��̨����
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

                //�ָ��Զ���̨
                com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);

                //��ͣ����
                radio_pause();

                //mute
                radio_set_mute (SetMUTE);
            }

            //�����������жϵ�̨�б��Ƿ�����
            if (audition_flag == FALSE)
            {
                //��̨�б�����
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

            //�жϵ�ǰƵ���Ƿ���Ч̨
            while (1)
            {
                radio_set_throd(temp_threshold);

                if (radio_set_softseek(seek_freq, dir) == TRUE)
                {
                    //������Ч��̨
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
            //�жϵ�ǰƵ���Ƿ���Ч̨
            if (radio_set_softseek(seek_freq, dir) == TRUE)
            {
                //������Ч��̨
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

                //ˢ�µ�̨��
                radio_show_station_num(tab_num, FALSE);

                //��������ģʽ
                if (g_radio_seek_audition_enable == TRUE)
                {
                    audition_flag = TRUE;
                    audition_start_time = sys_get_ab_timer();

                    //������
                    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

                    //�ͷž���
                    radio_set_mute(releaseMUTE);

                    //��ʼ����
                    radio_resume();
                }
            }
        }

        if (audition_flag == FALSE)
        {
            //��̨�б�����
            if (tab_num >= MAX_STATION_COUNT)
            {
                break;
            }

            if (dir == DIR_UP)
            {
                if ((seek_freq + space) > end_freq)
                {
                    //����ɨ��,�ص����ų���
                    break;
                }
                else
                {
                    //����Ƶ��
                    seek_freq += space;
                }
            }
            else
            {

                if ((seek_freq - space) < end_freq)
                {
                    //����ɨ��,�ص����ų���
                    break;
                }
                else
                {
                    //����Ƶ��
                    seek_freq -= space;
                }
            }

            //ˢ��Ƶ��
            radio_show_frequency(seek_freq, FALSE);
        }

        //����ϵͳ��Ϣ��gui ��Ϣ
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            //�κΰ���,ȡ��ɨ��
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
            key_cancel_seek = TRUE;
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
        //����20ms �����񽻻�
        sys_os_time_dly(1);
    }

    if (audition_flag == TRUE)
    {
        //��ͣ����
        radio_pause();

        //mute
        radio_set_mute (SetMUTE);
    }

    //�����̨��
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
 * \ȫ�Զ���̨����������ʼ��
 * \param[in]    dir: ��̨����
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_auto_search_init(FM_SeekDir_e dir)
{
    //��յ�ǰ�˵����εĵ�̨�б�
    if (g_radio_config.band_mode == Band_China_US)
    {
        //��ͨ����
        g_radio_config.FMStatus.freq = MIN_US;
    }
    else if (g_radio_config.band_mode == Band_Japan)
    {
        //�ձ�����
        g_radio_config.FMStatus.freq = MIN_JP;
    }
    else
    {
        //ŷ�޲���
        g_radio_config.FMStatus.freq = MIN_EU;
    }
    //�����̨��������ǰ��̨��
    g_radio_config.FMStatus.station = 0;
    g_radio_config.FMStatus.station_count = 0;

    //��ջ����̨�б�
    libc_memset(g_radio_config.Auto_tab, 0, MAX_STATION_COUNT * 4);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_auto_search_exit(void)
 * \ȫ�Զ���̨�����˳�
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
        //���������ĵ�̨��ѡ���һ������
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
        //һ����̨Ҳδ��������ѡ��Ĭ��Ƶ�㲥��
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

    //��̨�����󱣴��̨�б�
    sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_auto_search(FM_SeekDir_e dir)
 * \ȫ�Զ���̨����
 * \param[in]    mode:  ��̨ģʽ( Ӳ����̨or  �����̨)
 * \param[in]    dir: ��̨����( ���ϻ�����)
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note ȫ�Զ���̨�������ǰ�˵����ζ�Ӧ���б�
 * \       �����յ�ǰ�˵����ν�����̨
 */
/*******************************************************************************/
app_result_e radio_scene_auto_search(FM_SeekDir_e dir)
{
    app_result_e result;

    /* ��ʼ��*/
    if (radio_auto_search_init(dir) == FALSE)
    {
        //��ʼ��ʧ�ܷ���
        return RESULT_ERROR;
    }

    //��̨ʱ����
    com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);

    led_display_icon(LCD_PAUSE, FALSE);
    led_display_icon(LCD_PLAY, TRUE);

    change_app_state (APP_STATE_PLAYING);

    //�������ȫ�Զ���̨
    result = deal_soft_auto_seek(dir);

    change_app_state (APP_STATE_NO_PLAY);

    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    radio_auto_search_exit();

    //play freq
    need_tts = TRUE;

    //����Ƶ�㲥��
    radio_set_freq(g_radio_config.FMStatus.freq);

    return result;
}
