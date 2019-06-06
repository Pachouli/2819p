/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ϵͳ��Ϣ��˽����Ϣ����
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 mengine_message_done(void)
 * \������Ϣ����,����������յ���˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       Exit_App �˳�app
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_message_done_bank(private_msg_t *cur_msg)
{
    g_mengine_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
#if (SUPPORT_SOFT_KEYTONE == 1)
        /*���󲥷�������*/
        case MSG_SOFT_KEYTONE_PLAY_SYNC:
        {
            bool ret_val = FALSE;
            if (mp_handle != NULL)
            {
                if (mmm_mp_cmd(mp_handle, MMM_MP_KEYTONE_PLAY, (uint8 *)(cur_msg->msg.content.addr)) == 0)
                {
                    change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                    ret_val = TRUE;
                }
            }
            mengine_reply_msg(cur_msg, ret_val);
        }
        break;
#endif

        /*���ò������*/
        case MSG_SET_EQ_SYNC: //����eq +eq curve data
        mengine_eq_set(cur_msg);
        break;

        case MSG_MENGINE_SET_PLAYMODE_SYNC: //����ѭ��ģʽ+shuffle
        mengine_set_playmode(cur_msg);
        break;

        /*�����������*/
        case MSG_MENGINE_PLAY_SYNC: //��ʼ��������
        mengine_play(cur_msg);
        break;

        case MSG_MENGINE_STOP_SYNC: //ֹͣ��������
        mengine_stop(cur_msg);
        break;

        case MSG_MENGINE_PAUSE_SYNC: //��ͣ��������
        mengine_pause(cur_msg);
        break;

        case MSG_MENGINE_RESUME_SYNC: //�ָ���������(����ͣ������)
        mengine_resume(cur_msg);
        break;

        /*��������*/
        case MSG_MENGINE_FFWD_SYNC:
        mengine_fast_forward(cur_msg); //���
        break;

        case MSG_MENGINE_FBWD_SYNC:
        mengine_fast_backward(cur_msg); //����
        break;

        case MSG_MENGINE_CANCEL_FFB_SYNC:
        mengine_cancel_ffb(cur_msg); //ȡ�������
        break;

        /*ѡ���ļ�����*/ //���ò����ļ���·��location
        case MSG_MENGINE_SET_FILEPATH_SYNC:
        mengine_set_filepath(cur_msg);
        break;

        case MSG_MENGINE_PLAY_NEXT_SYNC: //������һ��
        mengine_play_next(cur_msg);
        break;

        case MSG_MENGINE_PLAY_PREV_SYNC: //������һ��
        mengine_play_prev(cur_msg);
        break;

        case MSG_MENGINE_FOLFER_NEXT_SYNC: //�л���һ��Ŀ¼
        mengine_play_folder_next(cur_msg);
        break;

        case MSG_MENGINE_FOLFER_PREV_SYNC: //�л���һ��Ŀ¼
        mengine_play_folder_prev(cur_msg);
        break;
        case MSG_MENGINE_GET_FILEINFO_SYNC: //��ȡ�ļ���ʽ��b/s����ʱ�����Ϣ
        mengine_get_fileinfo(cur_msg);
        break;

        /*��������*/
        case MSG_MENGINE_CLEAR_ERR_SYNC: //��������
        mengine_clear_error(cur_msg);
        break;

        case MSG_MENGINE_SET_SORTNUMBER_SYNC: //�����ļ����
        mengine_set_sortnumber(cur_msg);
        break;

        case MSG_MENGINE_GET_DISKNUMBER_SYNC: //��ȡ��ǰ�ļ����
        mengine_get_disknumber(cur_msg);
        break;

        case MSG_MENGINE_GET_DIRNAME_SYNC:
        #ifdef DIR_NAME_SUPPORT
        mengine_get_dirname(cur_msg);
#endif
        break;
        /*��ȡ�ļ���*/
        case MSG_MENGINE_GET_FILENAME_SYNC:
        mengine_get_filename(cur_msg);
        break;

        /*�����л�����̨Ӧ�ã�ͨ�����������֣�����Ӧ�ñ���״̬���Ա㷵��ʱ�ָ�״̬*/
        case MSG_READY_TO_BG_APP_SYNC:
        g_p_global_var->play_status_before = g_eg_status_p->play_status;
        mengine_reply_msg(cur_msg, TRUE);
        break;

        case MSG_APP_QUIT: //�˳�app
        //exit_msg_loop();
        g_mengine_result = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        g_mengine_result = mengine_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        g_mengine_result = mengine_tts_stop(cur_msg);
        break;

        case MSG_SYSTEM_ENTER_S2_SYNC:
        g_mengine_result = mengine_enter_s2(cur_msg);
        break;

        default:
        libc_printf_warning("music engine invalid msg!0x%x\n", cur_msg->msg.type);
        mengine_reply_msg(cur_msg, FALSE);
        break;
    }

    return g_mengine_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play(void* msg_ptr)
 * \��ʼ����
 * \param[in]    msg_ptr��Ϣָ��  para1
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
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play(private_msg_t *msg_ptr)
{
    bool ret_vals;

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    //���ò����ļ�
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{

    //�����ļ�����
    ret_vals = _play(PLAY_NORMAL); //����

    //}

    g_eg_playinfo_p->cur_file_switch |= 0x01;

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_stop(void* msg_ptr)
 * \ֹͣ����
 * \param[in]    msg_ptr��Ϣָ��  para1
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
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_stop(private_msg_t *msg_ptr)
{
    bool ret_vals;

    ret_vals = _stop(STOP_NORMAL); //ֹͣ����

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() != MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_play();
    }
#endif

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_pause(void* msg_ptr)
 * \��ͣ����
 * \param[in]    msg_ptr��Ϣָ��  para1
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
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_pause(private_msg_t *msg_ptr)
{
    bool ret_vals;

    //ret_vals = _stop(STOP_PAUSE);//��ͣ����

    ret_vals = _stop(STOP_NORMAL); //ֹͣ����

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() != MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_play();
    }
#endif

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_resume(void* msg_ptr)
 * \�ָ�����(����ͣ�ָ�)
 * \param[in]    msg_ptr��Ϣָ��  para1
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
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_resume(private_msg_t *msg_ptr)
{
    bool ret_vals;

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    //���¿�ʼ����, �����ļ�
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{

    //�����ļ�����
    ret_vals = _play(PLAY_RESUME); //����

    //}

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

//����EQ����
app_result_e mengine_eq_set(private_msg_t *msg_ptr)
{
    bool ret = TRUE;

    if (mp_handle != NULL)
    {
        ret = _set_eq();
    }

    mengine_reply_msg(msg_ptr, ret);

    return RESULT_NULL;
}

