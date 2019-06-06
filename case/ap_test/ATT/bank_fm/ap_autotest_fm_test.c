/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ�LINEIN ͨ�����ԺͲ��Ų��ԡ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "fm_common.h"
#include "ap_autotest_main_entry.h"
#include "bank_common/ap_autotest_common_entry.h"

//��ǰ����״̬��Ϣ
//fmeg_status_t g_fmeg_status;


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
bool radio_send_msg(void *input_param, void *output_param, uint32 msg_type)
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
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &reply, 0);

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
bool radio_get_status(fmeg_status_t *play_status)
{
    bool bret;
    bret = radio_send_msg(NULL, play_status, MSG_FMENGINE_GET_PLAY_STATUS);
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
app_result_e deal_gui_radioplay(input_gui_msg_t *gui_msg)
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


bool scene_play_init(uint32 freq, uint8 threshold)
{
    bool bret = TRUE;
    fmeg_init_para_t tmp_fmeg_init_para;

    tmp_fmeg_init_para.freq_init = freq;
    tmp_fmeg_init_para.threshold = threshold;
    //����
    tmp_fmeg_init_para.span = 100;
    //���Ƶ��
    tmp_fmeg_init_para.freq_low = 87500;
    //���Ƶ��
    tmp_fmeg_init_para.freq_high = 108000;
    //ȥ����ʱ�䳣��
    tmp_fmeg_init_para.de_emphasis_tc = DE_EMPHASIS_TC_50US; //�й�

    bret = radio_send_msg(&tmp_fmeg_init_para, NULL, MSG_FMENGINE_INIT);

    return bret;
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
bool radio_scene_playing(void)
{
    bool result = TRUE;
    uint32 bt_test_stamp;
    input_gui_msg_t gui_msg;

    //��ǰ����״̬��Ϣ
    fmeg_status_t fmeg_status;

    bt_test_stamp = sys_get_ab_timer();
    while (1)
    {
        //��ȡ״̬��Ϣ
        radio_get_status(&fmeg_status);

        if (TRUE == get_gui_msg(&gui_msg))
        {
            if(RESULT_APP_QUIT == deal_gui_radioplay(&gui_msg))
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


int32 act_test_fm_play_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    bool result;
    int sys_clk_backup;
    fmplay_test_arg_t *p_fmplay_test_arg = (fmplay_test_arg_t *) arg_buffer;

    //play tts
    com_tts_state_play(TTS_ENTER_RADIO, 0);

    sys_clk_backup = com_adjust_sys_clk(FREQ_4M, TRUE);

    //�л�ΪAB�๦�ţ��Խ��͸��ţ�����FM����Ч��
    com_speaker_ab_d_switch(PA_SWITCH_AB);

    //����MUSIC����
    result = CREATE_APP_ENGINE(APP_ID_FMENGINE, FM_EG_ENTER_NORMAL);

    if (FALSE != result)
    {
        result = scene_play_init(p_fmplay_test_arg->freq, p_fmplay_test_arg->threshold);
        if (FALSE != result)
        {
            result = radio_send_msg(&(p_fmplay_test_arg->threshold), NULL, MSG_FMENGINE_SETTHRED);
            if (FALSE != result)
            {
                result = radio_send_msg(&(p_fmplay_test_arg->freq), NULL, MSG_FMENGINE_SET_SOFTSEEK);
                if (FALSE != result)
                {
                    FM_MUTE_e mode = releaseMUTE;
                    radio_send_msg(&mode, NULL, MSG_FMENGINE_SETMUTE);
                    radio_send_msg(NULL, NULL, MSG_FMENGINE_RESUME_SYNC);

                    result = radio_scene_playing();
                }
                else
                {
                    libc_printf("fm seek fail!", 0, 0);
                }
            }
        }
        else
        {
            libc_printf("fm module init fail!", 0, 0);
        }
    }

    KILL_CURRENT_APP_ENGINE();

    LOCAL_REPORT_RESULT_EXT("fm test:", TESTID_FM_TEST, result, test_mode, log_buffer, cur_log_len);

    //�л�ΪD�๦�ţ��Խ��͹���
    com_speaker_ab_d_switch(PA_SWITCH_D);

    com_adjust_sys_clk(sys_clk_backup, FALSE);

    return result;
}

