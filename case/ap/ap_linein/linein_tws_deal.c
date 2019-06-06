/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������LINEINǰ̨Ӧ�� TWS ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_linein.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

btengine_info_t g_linein_bt_info;
uint8 last_tws_pair_status;
uint8 last_tws_dev_role;
uint8 last_phone_status;
uint8 g_tws_pos_switch_flag; //0��ʾδ�����л�״̬��1��ʾ�Ѳ�������������л������ȴ�7�����ڰ����л�
uint32 g_tws_pos_switch_time;
bool g_linein_tws_enable;
btplay_info_t g_btplay_info;
uint8 btplay_status_count;

void tws_spk_pos_tts_state_play(bool master_is_left)
{
    if (com_get_input_msg_type() == INPUT_MSG_KEY_SLAVE)
    {
        if (master_is_left == TRUE)
        {
            com_tts_state_play_tws_slave(TTS_TWS_RIGHT_BOX, 0);
        }
        else
        {
            com_tts_state_play_tws_slave(TTS_TWS_LEFT_BOX, 0);
        }
    }
    else
    {
        if (master_is_left == TRUE)
        {
            com_tts_state_play(TTS_TWS_LEFT_BOX, 0);
        }
        else
        {
            com_tts_state_play(TTS_TWS_RIGHT_BOX, 0);
        }
    }
}

app_result_e tws_dev_spk_pos_switch(void)
{
#if (TWS_POS_MODE != TWS_POS_MODE_ALWAYS_MASTER_LEFT)
#if (SUPPORT_LEFT_RIGHT_SWITCH == 1)
    msg_apps_t msg;

    if (g_linein_bt_info.tws_pair_status == TWS_PAIRED)
    {
        dev_spk_pos_e tmp_dev_spk_pos = get_tws_spk_pos();

        if (g_tws_pos_switch_flag == 0)
        {
            //�������������壬Ȼ��ȴ�6�����ڰ����л�
            if (tmp_dev_spk_pos == TWS_SPK_LEFT)
            {
                tws_spk_pos_tts_state_play(TRUE);
            }
            else if (tmp_dev_spk_pos == TWS_SPK_RIGHT)
            {
                tws_spk_pos_tts_state_play(FALSE);
            }

            g_tws_pos_switch_flag = 1;
            g_tws_pos_switch_time = sys_get_ab_timer();
        }
        else
        {
            //��ͣTWS����
            msg.type = MSG_BTPLAYEG_TWS_PAUSE_SYNC;
            send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);

            //�л��������������壬Ȼ��ȴ�6�����ڰ����л�
            if (tmp_dev_spk_pos == TWS_SPK_LEFT)
            {
                change_tws_spk_pos(TWS_SPK_RIGHT);
                com_tws_m2s_switch_pos_cmd(TWS_SPK_LEFT);

                //�л��������壬Ȼ�󲥱���Ȼ��ָ�����
                tws_spk_pos_tts_state_play(FALSE);
            }
            else if (tmp_dev_spk_pos == TWS_SPK_RIGHT)
            {
                change_tws_spk_pos(TWS_SPK_LEFT);
                com_tws_m2s_switch_pos_cmd(TWS_SPK_RIGHT);

                //��ͣ�²��ţ�Ȼ���л��������壬Ȼ�󲥱���Ȼ��ָ�����
                tws_spk_pos_tts_state_play(TRUE);
            }

            //�ָ�TWS����
            msg.type = MSG_BTPLAYEG_TWS_RESUME_SYNC;
            send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);

            g_tws_pos_switch_time = sys_get_ab_timer();
        }
    }
#endif
#endif
    return RESULT_NULL;
}

//����TRUE��ʾ������TTS����������Ϣ�¼�����Ҫ��������״̬
bool tws_tts_display_deal(void)
{
    bool ret = FALSE;

    if (g_linein_bt_info.tws_pair_status != last_tws_pair_status)
    {
        if (g_linein_bt_info.tws_pair_status == TWS_PAIRED)
        {
            if (g_linein_bt_info.dev_role == TWS_MASTER)
            {
                com_tts_state_play(TTS_PAIR_SUCCEED, 0);
                ret = TRUE;
            }
        }
        else if (last_tws_pair_status == TWS_PAIRED)
        {
            com_tts_state_play(TTS_PAIR_DISCON, 0);
            ret = TRUE;
            if (last_tws_dev_role == TWS_SLAVE)
            {
                com_tts_state_play(TTS_WAIT_BT_CON, 0);
                ret = TRUE;
            }
        }

        last_tws_pair_status = g_linein_bt_info.tws_pair_status;
        last_tws_dev_role = g_linein_bt_info.dev_role;
    }

    if (g_tws_pos_switch_flag == 1)
    {
        if ((sys_get_ab_timer() - g_tws_pos_switch_time) >= 6000)
        {
            g_tws_pos_switch_flag = 0;
        }
    }

    //������TTS����������
    if (get_tws_role() == TWS_MASTER)
    {
        com_tts_state_play_tws_slave(-1, 0); //-1��ʾ�ӻ�������ȡtts_id����
    }

    return TRUE;
}

void btplay_tws_conform_mode(bool filter)
{
    if (get_engine_type() == ENGINE_BTPLAY)
    {
        msg_apps_t msg;
        msg.type = MSG_BTPLAYEG_TWS_CONFORM_MODE_SYNC;
        msg.content.data[0] = filter;
        send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);
    }
}

void btplay_tts_state_play(uint8 tts_id, uint16 mode)
{
    if (com_get_input_msg_type() == INPUT_MSG_KEY_SLAVE)
    {
        com_tts_state_play_tws_slave(tts_id, mode);
    }
    else
    {
        com_tts_state_play(tts_id, mode);
    }
}

app_result_e btplay_key_play(void)
{
    if (g_linein_status.play_status != PlaySta)
    {
        btplay_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
        com_speaker_on(SPK_PLAY);
        btplay_tws_conform_mode(FALSE);
        g_linein_status.play_status = PlaySta;
    }
    else
    {
        btplay_tws_conform_mode(TRUE);
        btplay_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
        g_linein_status.play_status = PauseSta;
    }

    return RESULT_NULL;
}

void _btplay_get_engine_status(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTPLAYEG_GET_STATUS_SYNC;
    msg.content.addr = &g_btplay_info;
    send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);
}

void _btplay_playing_check_status(void)
{
    //�ӳ�ͬ������״̬����ֹ����״̬һֱ�����沥��״̬��ƥ��
    _btplay_get_engine_status();
    if ((g_linein_status.play_status == PlaySta) && (g_btplay_info.status != BTPLAY_PLAY))
    {
        btplay_status_count++;
        if (btplay_status_count >= 50)
        {
            g_linein_status.play_status = PauseSta;
            libc_printf("auto switch to PauseSta\n");
            btplay_status_count = 0;
        }
    }
    else if ((g_linein_status.play_status != PlaySta) && (g_btplay_info.status == BTPLAY_PLAY))
    {
        btplay_status_count++;
        if (btplay_status_count >= 50)
        {
            g_linein_status.play_status = PlaySta;
            libc_printf("auto switch to PlaySta\n");
            btplay_status_count = 0;
        }
    }
    else
    {
        btplay_status_count = 0;
    }

    if ((g_last_linein_status == PlaySta) && (g_linein_status.play_status != PlaySta))
    {
        //��play��pause
        com_speaker_off(SPK_PLAY);
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PLAY, FALSE);
        led_display_icon(LCD_PAUSE, TRUE);
    }
    else if ((g_last_linein_status != PlaySta) && (g_linein_status.play_status == PlaySta))
    {
        //��pause��play
        com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

        led_display_icon(LCD_PAUSE, FALSE);
        led_display_icon(LCD_PLAY, TRUE);
    }

    g_last_linein_status = g_linein_status.play_status;
}

#endif
