/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ����ǰ̨Ӧ�ò��ų���Ӧ����Ϣ�Ͱ�����Ϣ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_linein.h"

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
 * \bool linein_get_status(lineineg_status_t* play_status)
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
 * \bool linein_pause(void)
 * \��ͣ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool linein_pause(void)
{
    bool bret;
    bret = linein_send_msg(NULL, NULL, MSG_LINEINEG_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool linein_resume(void)
 * \�ָ���������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool linein_resume(void)
{
    bool bret;
    com_speaker_on(SPK_PLAY);
    bret = linein_send_msg(NULL, NULL, MSG_LINEINEG_RESUME_SYNC);
    return bret;
}

#if (SUPPORT_RECORD_FUNC == 1)
bool linein_save_record_restart(void)
{
    bool bret;
    com_speaker_on(SPK_PLAY);
    bret = linein_send_msg(NULL, NULL, MSG_REC_SAVE_RESTART_NEXT_SYNC);
    return bret;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e linein_message_deal(private_msg_t *msg)
 * \����music��˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    msg��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //ͬ����־��1B
 *                 sem_id_t sem_id;           //�ź���ID��1B
 *                 msg_apps_t msg;          //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //��Ϣ���ͣ�2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��
 * \param[out]   none
 * \return       app_result_e the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control_sub.c
 * \note
 */
/*******************************************************************************/
app_result_e linein_message_deal(private_msg_t *msg)
{
    app_result_e result = RESULT_NULL;

    switch (msg->msg.type)
    {
        //linein�γ�
        case MSG_LINEIN_OUT:
        if (g_linein_play_mode == LINEIN_MODE_AUX)
        {
            result = RESULT_NEXT_FUNCTION;
        }
        break;

#if (SUPPORT_RECORD_FUNC == 1)
        case MSG_UH_OUT:
        if (g_linein_play_mode == LINEIN_MODE_MIC_U)
        {
            result = RESULT_NEXT_FUNCTION;
        }
        else
        {
            result = com_message_box(msg->msg.type);
        }
        break;

        case MSG_SD_OUT:
        if (g_linein_play_mode == LINEIN_MODE_MIC_C)
        {
            result = RESULT_NEXT_FUNCTION;
        }
        else
        {
            result = com_message_box(msg->msg.type);
        }
        break;
#endif
        default:
        result = com_message_box(msg->msg.type);
        break;
    }
    return result;
}

