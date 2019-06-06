/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ����ģʽ����
 ********************************************************************************/

#include "app_mengine.h"

#ifdef DIR_NAME_SUPPORT
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_dirname(void* msg_ptr)
 * \��ȡĿ¼����Ϣ
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

 * \param[out]   msg_ptr->msg.content.addr //����ѭ����ʽ��ָ��
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_dirname(private_msg_t *msg_ptr)
{
    uint8 *dest_ptr = (uint8 *)msg_ptr->msg.content.addr;
    //dir name info
    libc_memcpy(dest_ptr, g_dir_name_buf, sizeof(g_dir_name_buf));

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_filename(void* msg_ptr)
 * \��ȡ�ļ�����Ϣ
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

 * \param[out]   msg_ptr->msg.content.addr //����ѭ����ʽ��ָ��
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_filename(private_msg_t *msg_ptr)
{
    music_file_name_t *dest_ptr = (music_file_name_t *) msg_ptr->reply->content;

    libc_memcpy(dest_ptr->fname_buffer, g_file_name, dest_ptr->fname_length);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fast_forward(void* msg_ptr)
 * \���
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
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_fast_forward(private_msg_t *msg_ptr)
{
    //���������� or ���
    if (mp_handle != NULL)
    {
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
        {
            _set_ffb(FAST_FORWARD);
            g_eg_status_p->play_status = PlayFast; //�޸�״̬
            g_eg_status_p->fast_status = FFPlaySta;
        }
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fast_backward(void* msg_ptr)
 * \����
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
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_fast_backward(private_msg_t *msg_ptr)
{
    //��ǰ���������� or ����
    if (mp_handle != NULL)
    {
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
        {
            _set_ffb(FAST_BACKWARD);
            g_eg_status_p->play_status = PlayFast; //�޸�״̬
            g_eg_status_p->fast_status = FBPlaySta;
        }
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_cancel_ffb(void* msg_ptr)
 * \ȡ�������
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
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_cancel_ffb(private_msg_t *msg_ptr)
{
    //��ǰ�ǿ����״̬
    if (mp_handle != NULL)
    {
        if (((g_eg_status_p->play_status == PlayFast) && (g_eg_status_p->fast_status != FBPlay_Null))
                || (g_eg_status_p->fast_status == FFPlayTailSta)
                || (g_eg_status_p->fast_status == FBPlayHeadSta))
        {
            if ((g_eg_status_p->fast_status == FFPlaySta) || (g_eg_status_p->fast_status == FBPlaySta))
            {
                _set_ffb(CANCLE_FAST_BACKWARD_FORWARD);
            }
            g_eg_status_p->play_status = PlaySta; //�޸�״̬
            g_eg_status_p->fast_status = FBPlay_Null;
        }
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

