/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��  ǰ̨Ӧ�ò��ų�����ʼ�����˳�
 ********************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_open_engine(mengine_enter_mode_e start_mode)
 * \װ������,���������ڣ����ٰ�װ
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_open_engine(mengine_enter_mode_e start_mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //msg_reply_t temp_reply;

    //����Ϣ����
    msg.para[0] = APP_ID_MENGINE;
    if(DISK_C == g_file_path_info.file_path.dirlocation.disk)
    {
        msg.para[1] = (uint8) (start_mode);
    }
    else
    {
        msg.para[1] = (uint8) (start_mode | g_file_path_info.file_path.dirlocation.disk);
    }

    if (get_need_update_vm() == 1)
    {
        msg.para[1] |= 0x08; //bit3��ʾҪ����ϵ���Ϣ
    }
    msg.content.addr = NULL;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    if (bret == TRUE)
    {
        set_need_update_vm(0);
    }

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_close_engine(uint8 engine_id)
 * \�ر�����
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_close_engine(uint8 engine_id)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //ע����̨����̨engine_id
    msg.type = MSG_KILL_APP_SYNC; //MSG_KILL_APP;
    msg.para[0] = engine_id;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}

//����Ԥ��ģʽ��ʹ�øýӿ���Ԥ��������Ӳ������ com_fast_preview_ui.c ��
//disk_index: 0-SD��,1-Uhost,2-NOR
void music_show_welcome_ui(uint8 disk_index)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);

    if (disk_index == 0)
    {
        led_display_icon(LCD_SD, TRUE);
    }
    else if (disk_index == 1)
    {
        led_display_icon(LCD_USB, TRUE);
    }

    //��ʾ4��0
    led_display(NUMBER1, 0, TRUE);
    led_display(NUMBER2, 0, TRUE);
    led_display(NUMBER3, 0, TRUE);
    led_display(NUMBER4, 0, TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_init(void)
 * \���ų�����ʼ��
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_init(void)
{
    bool ret_val = TRUE;
    //for open engine param
    uint8 open_mode, tts_id;
    uint8 disk_index;
    app_result_e result = RESULT_NULL;
    bool need_switch_ap = FALSE;
    bool need_flush_key = FALSE;

    //from tools
    com_set_mute(FALSE); //�������

    if (get_engine_type() != ENGINE_NULL)
    {
        //ע��radio��̨
        music_close_engine(APP_ID_THEENGINE);
    }

    if (g_enter_mode == PARAM_MUSIC_CARD_REPLAY)
    {
        //tts_id = TTS_CARD_REPLAY;
        tts_id = TTS_REPLAY;
        open_mode = ENTER_RECORD_PLAY;

        disk_index = 0;
    }
    else if (g_enter_mode == PARAM_MUSIC_UHOST_REPLAY)
    {
        //tts_id = TTS_UHOST_REPLAY;
        tts_id = TTS_REPLAY;
        open_mode = ENTER_RECORD_PLAY;

        disk_index = 1;
    }
    else
    {
        if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
        {
            tts_id = TTS_PLAY_SDCARD;
            open_mode = ENTER_MUSIC_START;

            disk_index = 0;
        }
        else if (g_file_path_info.file_path.dirlocation.disk == DISK_U)
        {
            tts_id = TTS_PLAY_UHOST;
            open_mode = ENTER_MUSIC_START;

            disk_index = 1;
        }
        else //if (g_file_path_info.file_path.dirlocation.disk == DISK_C)
        {
            tts_id = TTS_PLAY_MDISK;
            open_mode = ENTER_MDISK_PLAY;
            disk_index = 2;
        }
    }

    if (g_p_global_var->fp_switch_jump_welcome == FALSE)
    {
        music_show_welcome_ui(disk_index);
    }
    g_p_global_var->fp_switch_jump_welcome = FALSE;

    //�Ӻ�̨Ӧ�÷��ػָ�ʱ���
    if (g_p_global_var->bg_app_sta == BG_STATUS_BACK)
    {
        g_cur_time = g_p_global_var->display_time;
    }

#if (SUPPORT_FAST_PREVIEW == 1)

    if (get_bt_state() == BT_STATE_WORK)
    {
        //�����֧�ֺ�̨���������з�����Ӧ�ö��� bt enging ��ͻ����Ҫɱ�� bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

    //�ȴ�����Ԥ��ģʽ�Ƿ��л�������Ӧ�ã�������л��Ļ�����������ת
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
            private_msg_t music_msg;
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
                if (get_app_msg(THIS_APP_TYPE, &music_msg) == TRUE)
                {
                    result = music_message_deal(&music_msg);
                    if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
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

    //ESD RESET ��Ҫ����TTS
    if ((g_p_global_var->bg_app_sta != BG_STATUS_BACK) && (g_p_esd_bk_info->reset_flag == 0))
    {
        //play tts
        com_tts_state_play(tts_id, 0);
    }

#else

    //ESD RESET ��Ҫ����TTS
    if ((g_p_global_var->bg_app_sta != BG_STATUS_BACK) && (g_p_esd_bk_info->reset_flag == 0))
    {
        //play tts
        com_tts_state_play(tts_id, 0);
    }

    if (get_bt_state() == BT_STATE_WORK)
    {
        //�����֧�ֺ�̨���������з�����Ӧ�ö��� bt enging ��ͻ����Ҫɱ�� bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

#endif

    com_speaker_on(SPK_PLAY);

    //����MUSIC����
    ret_val = music_open_engine(open_mode);

    if (ret_val == TRUE)
    {
        //check_file:
        //��ȡ״̬��Ϣ
        music_get_status(&g_music_status);

        //set volume
        com_reset_sound_volume(SET_VOL_BY_SYSTEM);

        if (g_music_status.err_status == EG_ERR_NO_FILE)
        {
            //û�п���U�̣���������û�пɲ��ŵ������ļ�����ʾ����
            ret_val = FALSE;
        }
    }

    if (ret_val == TRUE)
    {
        music_set_playmode(g_music_config.repeat_mode);
    }

    if (ret_val == FALSE)
    {
        result = RESULT_NEXT_FUNCTION;
    }

    fast_switch_ap_exit:

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_exit(void)
 * \�˳����ų���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_exit(void)
{
    if (get_engine_type() != ENGINE_NULL)
    {
        //ע��radio��̨
        music_close_engine(APP_ID_THEENGINE);
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(void)
 * \���ų�������
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_playing(void)
{
    app_result_e ret_vals;
    uint8 disk_in_play;

    //������ʼ�� ��ʼ���ļ�ѡ��������ȡUI����ʾ�����ݵ�
    ret_vals = _scene_play_init();

    if ((ret_vals != RESULT_NULL) && (ret_vals != RESULT_REDRAW))
    {
        goto ret_deal;
    }

    //�������������
    g_error_num = 0;

    //��ǰ������Ŀ��
    music_get_filepath(&g_file_path_info); //��ȡ�ļ�·��

    //��ʼ��ʾ��Ŀ��
    _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
    g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

    disk_in_play = g_file_path_info.file_path.dirlocation.disk;
    //���Ǵ�clock����,��һ���ǲ�����̡�mode���ȷ�ʽ����music,����Ҫ����ɨ�����

    //����ѭ��
    ret_vals = _playing_loop_deal();

    ret_deal:

    //���ó����˳�����
    _scene_play_exit();
    //deal result ret �����¼������,����ڽ�����һ���Ĵ����ɻص���һ�β���
    if (ret_vals == RESULT_NEXT_FUNCTION)
    {
        //other play mode need return to last play
        if ((g_enter_mode != PARAM_MUSIC_CARD_DIR) && (g_enter_mode != PARAM_MUSIC_UHOST_DIR) && (g_enter_mode != PARAM_MUSIC_RES_DIR))
        {
            ret_vals = RESULT_LASTPLAY;
        }
    }

    return ret_vals;
}

