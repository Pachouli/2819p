/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ����ز���ͨ�����ԺͲ��Ų��ԡ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "music_common.h"
#include "ap_autotest_main_entry.h"
#include "bank_common/ap_autotest_common_entry.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
 * \ ��ȡ������ز�������
 * \param[in]    input_param    �������ָ��
 * \param[in]    msg_type       ��Ϣ����
 * \param[out]   output_param   �������ָ��
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    //�����������ĵ�ַ
    msg.content.addr = input_param;

    //��Ż�ȡ�����ĵ�ַ
    reply.content = output_param;

    //��Ϣ����(����Ϣ����)
    msg.type = msg_type;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}




/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \��ȡ��ǰ����״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t *play_status)
{
    bool bret;
    bret = music_send_msg(NULL, play_status, MSG_MENGINE_GET_STATUS_SYNC);
    return bret;
}



/******************************************************************************/
/*!
 * \par  Description:
 * \  ��������Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
app_result_e deal_gui_cardplay(input_gui_msg_t *gui_msg, mengine_status_t *cur_music_status)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    libc_printf("music key val:%d status:%d\n", gui_msg->data.kmsg.val, cur_music_status->play_status);
    switch (gui_msg->data.kmsg.val)
    {
        case KEY_PLAY:
        if ((cur_music_status->play_status == PauseSta) || (cur_music_status->play_status == StopSta))
        {
            com_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
            music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);
        }
        else if (cur_music_status->play_status == PlaySta)
        {
            //pause
            music_send_msg(NULL, NULL, MSG_MENGINE_PAUSE_SYNC);
            com_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
        }
        else
        {
            ; //nothing
        }
        break;

        case KEY_PREV:
        //����Ϣ����,���������İ�����Ϣ
        com_tts_state_play(TTS_LAST_SONG, TTS_MODE_CONTROL_TTS_YES);
        music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_PREV_SYNC);
        music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);
        break;

        case KEY_NEXT:
        //����Ϣ����,���������İ�����Ϣ
        com_tts_state_play(TTS_NEXT_SONG, TTS_MODE_CONTROL_TTS_YES);
        music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_NEXT_SYNC);
        music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);
        break;

        case KEY_NULL:
        break;

        default:
        app_ret = RESULT_APP_QUIT;
        break;
    }

    com_filter_key_hold();

    return app_ret;
}




/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(void)
 * \??????
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ??
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_scene_playing(uint8 disk_type)
{
    bool result = TRUE;
    uint32 bt_test_stamp;
    input_gui_msg_t gui_msg;

    //��ǰ����״̬��Ϣ
    mengine_status_t music_status;

    bt_test_stamp = sys_get_ab_timer();
    while (1)
    {
        //��ȡ״̬��Ϣ
        music_get_status(&music_status);

        if (TRUE == get_gui_msg(&gui_msg))
        {
            if(RESULT_APP_QUIT == deal_gui_cardplay(&gui_msg, &music_status))
            {
                break;
            }
        }

        //�����ȽϺ�ʱ,������ͣ״̬�²�ȥ�������
        if (disk_type == DISK_H)
        {
            if ((get_card_state() != CARD_STATE_CARD_NULL)
                && ((music_status.play_status == StopSta) || (music_status.play_status == PauseSta)))
            {
                if (sys_card_detect() == FALSE)
                {
                    result = FALSE;
                    break;
                }
            }
        }
        else
        {

        }

        //����1ms,�����񽻻�
        sys_os_time_dly(1);

        //���Ʋ���ʱ��,�������¼�ǿ���˳�
        if ((sys_get_ab_timer() - bt_test_stamp) > 10 * 1000)
        {
            break;
        }
    }

    return result;
}


int32 act_test_sdcard_play_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{   
    //��ǰ����״̬��Ϣ
    mengine_status_t music_status;

    //play tts
    com_tts_state_play(TTS_PLAY_SDCARD, 0);

    //����MUSIC����
    bool result = CREATE_APP_ENGINE(APP_ID_MENGINE, ENTER_MUSIC_START | DISK_H);
    if (result == TRUE)
    {
        //��ȡ״̬��Ϣ
        music_get_status(&music_status);

        if (EG_ERR_NONE != music_status.err_status)
        {
            //û�п���U��,��������û�пɲ��ŵ������ļ�
            result = FALSE;
        }
    }

    if (FALSE != result)
    {
        result = music_scene_playing(DISK_H);
    }

    KILL_CURRENT_APP_ENGINE();
    LOCAL_REPORT_RESULT_EXT("card play test", TESTID_SDCARD_TEST, result, test_mode, log_buffer, cur_log_len);

    return result;
}


int32 act_test_uhost_play_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    //��ǰ����״̬��Ϣ
    mengine_status_t music_status;

    //play tts
    com_tts_state_play(TTS_PLAY_UHOST, 0);

    //����MUSIC����
    bool result = CREATE_APP_ENGINE(APP_ID_MENGINE, ENTER_MUSIC_START | DISK_U);
    if (result == TRUE)
    {
        //��ȡ״̬��Ϣ
        music_get_status(&music_status);

        if (EG_ERR_NONE != music_status.err_status)
        {
            //û�п���U��,��������û�пɲ��ŵ������ļ�
            result = FALSE;
        }
    }

    if (FALSE != result)
    {
        result = music_scene_playing(DISK_U);
    }

    KILL_CURRENT_APP_ENGINE();
    LOCAL_REPORT_RESULT_EXT("uhost play test", TESTID_UHOST_TEST, result, test_mode, log_buffer, cur_log_len);

    return result;
}


