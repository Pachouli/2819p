/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ�LINEIN ͨ�����ԺͲ��Ų��ԡ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "linein_common.h"
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
bool linein_send_msg(void *input_param, void *output_param, uint32 msg_type)
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
    bret = send_sync_msg(APP_ID_LINEIN_EG, &msg, &reply, 0);

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
bool linein_get_status(lineineg_status_t *play_status)
{
    bool bret;
    bret = linein_send_msg(NULL, play_status, MSG_LINEINEG_GET_STATUS_SYNC);
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
app_result_e deal_gui_auxplay(input_gui_msg_t *gui_msg)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    switch (gui_msg->data.kmsg.val)
    {
        case KEY_PLAY:
        case KEY_PREV:
        case KEY_NEXT:
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
bool linein_scene_playing(void)
{
    bool result = TRUE;
    uint32 bt_test_stamp;
    input_gui_msg_t gui_msg;
    
    //��ǰ����״̬��Ϣ
    lineineg_status_t linein_status;

    bt_test_stamp = sys_get_ab_timer();
    while (1)
    {
        //��ȡ״̬��Ϣ
        linein_get_status(&linein_status);

        if (TRUE == get_gui_msg(&gui_msg))
        {
            if(RESULT_APP_QUIT == deal_gui_auxplay(&gui_msg))
            {
                break;
            }
        }

        //����1ms,�����񽻻�
        sys_os_time_dly(1);

        //���Ʋ���ʱ��,�������¼�ǿ���˳�
        if ((sys_get_ab_timer() - bt_test_stamp) > 5 * 1000)
        {
            break;
        }
    }

    return result;
}


int32 act_test_linein_play_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    bool result;

    //play tts
    com_tts_state_play(TTS_PLAY_LINEIN, 0);

    //linein�����auxģʽ����
    bool play = TRUE;
    uint8 para = play | (ENTER_AUX_CHAN<<4);

    //����MUSIC����
    result = CREATE_APP_ENGINE(APP_ID_LINEIN_EG, para);

    if (FALSE != result)
    {
        result = linein_scene_playing();
    }

    KILL_CURRENT_APP_ENGINE();
    LOCAL_REPORT_RESULT_EXT("linein test:", TESTID_LINEIN_TEST, result, test_mode, log_buffer, cur_log_len);

    return result;
}





