/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ǰ̨Ӧ�ù�������ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_radio.h"

//cancel search
const key_map_t cancel_key_map_list[] =
{
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_NEXT, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_PREV, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_PLAY, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_IGNORE }, //����PLAY DOWN���ݲ��˳�
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_SEARCH_EXIT }, //����PLAY��ػ�
    { { KEY_MODE, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_IGNORE }, //����MODE DOWN���ݲ��˳�
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEARCH_EXIT }, //�̰�Mode���л�Ӧ��
    { { KEY_F1, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_VADD, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_VSUB, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_NUMBER, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },

    /*! ������־ */
    { { KEY_NULL_NO_SHORTCUT, 0, KEY_TYPE_NULL }, MSG_NULL }
};

/******************************************************************************/
/*!
 * \par  Description:
 * \RadioUI  ��ȡ˽����Ϣ�Ĵ���
 * \param[in]    msg  ˽����Ϣ�ṹ
 * \param[out]   none
 * \return  app_result_e �����Ϣ
 * \note RadioUI �Ĵ���ĸ���˽����Ϣ�Ļص�����
 *******************************************************************************/
app_result_e radio_app_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;

    ret_result = com_message_box(pri_msg->msg.type);

    return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  scene_play_sysmsg(void)
 * \���ų���ϵͳ��Ϣ��˽����Ϣ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e scene_play_sysmsg(void)
{
    private_msg_t play_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(THIS_APP_TYPE, &play_msg) == TRUE)
    {
        msg_result = radio_app_msg_callback(&play_msg);
    }
    return msg_result;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 whether_in_list( uint16  freq, uint16 *p_freq)
 * \�ж�ָ��Ƶ�ʵĵ�̨�Ƿ��ڵ�ǰ�ı����б���
 * \param[in]    freq
 * \param[in]    p_freq
 * \param[out]   none
 * \return       ��ǰƵ����ָ�����б��з��ص�̨�ţ����򷵻�0xff
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
uint8 whether_in_list(uint32 freq, uint32 *p_freq)
{
    uint8 ret;
    uint32 *ptr;
    uint8 num;
    uint32 tmp;

    /* ��ǰ���ε�̨�б�*/
    ptr = p_freq;

    for (num = 0; num < MAX_STATION_COUNT; num++)
    {
        tmp = *(ptr + num);
        if (tmp == freq)
        {
            //���б������ҵ�
            break;
        }
    }

    //���б���δ�ҵ�
    if (num == MAX_STATION_COUNT)
    {
        ret = 0xff;
    }
    else
    {
        ret = num + 1;
    }

    return ret;
}

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
    bret = send_sync_msg(APP_ID_THEENGINE, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_freq(uint32 freq)
 * \ͬ������Ƶ�ʲ���
 * \param[in]    freq  ���õ�Ƶ��ֵ( AP ��)
 * \param[out]   none
 * \return
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_freq(uint32 freq)
{
    bool bret = TRUE;

    bret = radio_send_msg(&freq, NULL, MSG_FMENGINE_SETFREQ_SYNC);

    //for SPP, ͬ����̨Ƶ��
    radio_get_freq();

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_mute(FM_MUTE_e mode)
 * \���þ�����������
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_mute(FM_MUTE_e mode)
{
    bool bret = TRUE;

    g_engine_status.FM_Mute_Status = mode;

    bret = radio_send_msg(&mode, NULL, MSG_FMENGINE_SETMUTE);

    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_throd(uint8 val)
 * \������̨����
 * \param[in]    val ��̨����ֵ
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_throd(uint8 val)
{
    bool bret = TRUE;

    bret = radio_send_msg(&val, NULL, MSG_FMENGINE_SETTHRED);

    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_softseek(uint32 freq)
 * \���������̨Ƶ��
 * \param[in]    freq, dir
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note   �����̨:   �����ǰƵ��Ϊ��̨������TRUE
 */
/*******************************************************************************/
bool radio_set_softseek(uint32 freq, uint8 dir)
{
    bool bret = TRUE;

    bret = radio_send_msg(&freq, NULL, MSG_FMENGINE_SET_SOFTSEEK);

    //for SPP, ͬ����̨Ƶ��
    radio_get_freq();

    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_freq(void)
 * \��ȡ��ǰƵ����Ϣ
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_freq(void)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, &g_engine_status.FM_CurrentFreq, MSG_FMENGINE_GET_FREQ);

    return bret;
}

//���ŵ�̨
bool radio_resume(void)
{
    bool bret = TRUE;

    com_speaker_on(SPK_PLAY);

    bret = radio_send_msg(NULL, NULL, MSG_FMENGINE_RESUME_SYNC);

    return bret;
}

//��ͣ��̨
bool radio_pause(void)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, NULL, MSG_FMENGINE_PAUSE_SYNC);

    return bret;
}

//��ȡ����״̬
bool radio_get_play_status(fmeg_status_t *p_fmeg_status)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, p_fmeg_status, MSG_FMENGINE_GET_PLAY_STATUS);

    return bret;
}

//��ȡFM��̨״̬
bool radio_get_fm_status(FM_Drv_Status_t *p_fm_drv_status)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, p_fm_drv_status, MSG_FMENGINE_GET_FM_STATUS);

    return bret;
}

/*! \endcond */

