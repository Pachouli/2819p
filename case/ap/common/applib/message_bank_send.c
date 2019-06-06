/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ����Ϣͨ��ģ�鷢��ͬ����Ϣ�ͻظ���Ϣ�ӿ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib_inner.h"

#define  SYNC_MSG_TIMEOUT  3000 //30������ʱ����λ10ms��timeout=0��ǿ��Ϊ��ֵ��

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ͬ����ʽ����Ӧ��˽����Ϣ��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[in]    reply ָ��ͬ����ϢӦ��ṹ��
 * \param[in]    timeout ��ʱ����ʱ�䣬0 ��ʾһֱ�ȴ�����ʱ����  ��λ10ms
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ����ͬ����Ϣ�ɹ���
 * \retval           FALSE ����ͬ����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1������ʽ����ͬ����Ϣ��ֱ���Է����� reply_sync_msg_inner Ӧ��
 msg_apps_t msg;
 msg_reply_t reply;
 bool ret;

 msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
 reply.content = (void *)file_path;
 ret = send_sync_msg_inner(APP_ID_MANAGER, &msg, &reply, 0);
 if(ret == TRUE)
 {
 if(reply.type == MSG_REPLY_SUCCESS)
 {
 ��ȷ���أ���ȡ�������ļ�·��
 }
 else
 {
 ���󷵻�
 }
 }
 else
 {
 ��������
 }
 * \endcode
 * \note
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã�����ͬ����Ϣ���Լ���ͬ����Ϣ�����˵�����Ϣ�����������ȡ�
 * \li  ע����Ϣ���ݻ����� msg.content.addr �� Ӧ�𻺳��� reply.content �Ŀռ䶼�ǿ�����Ϣ���ͷ��ġ�
 *      ���ڷ���ͬ����Ϣ���ȴ����أ�������� send_sync_msg_inner �Ĵ������� control bank�Σ���ô�����
 *      ���������Կ���bank data�С�
 *******************************************************************************/
bool send_sync_msg_inner(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    const app_info_t *target_app_info;
    msg_reply_t temp_reply;
    os_event_t *sem; //ͬ����Ϣͬ���ź���ָ��
    bool retvalue = TRUE;

    if (timeout == 0)
    {
        timeout = SYNC_MSG_TIMEOUT; //������ͬ����Ϣû����Ӧ������
    }

    //ͨ������IDת��Ϊ����APP ID
    if (app_id == APP_ID_THEENGINE)
    {
        app_id = get_engine_appid_by_type_inner(get_engine_type_inner());
        if (app_id == APP_ID_MAX)
        {
            goto app_noexist;
        }
    }

    //����Ŀ��Ӧ��
    target_app_info = applib_get_app_info_by_app_id(app_id);
    if (NULL == target_app_info)
    {
        app_noexist:
        libc_printf_warning("sync msg app_noexist!!0x%x\n", msg->type);
        return FALSE;
    }

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    if (libc_sem_init(&sem, 0) == -1)
    {
        libc_printf("sync msg\n");
        while (1);
    }
    cur_send_pmsg.sem = sem;
    if (reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //������Ϣ��������
    if (sys_mq_send((uint32)(target_app_info->mq_id), (void *) &cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_printf_warning("sync msg mq_send err!!0x%x\n", msg->type);
        return FALSE;
    }

    //�ȴ�ͬ����Ϣ��Ӧ
    if (libc_sem_wait(sem, timeout) < 0)
    {
        libc_printf("sync msg 0x%x\n", msg->type);
        while (1);
        retvalue = FALSE;
    }

    libc_sem_destroy(&sem);

    return retvalue;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ��ͬ����Ϣ������Ҫ�ͷ��ź�������
 * \param[in]    private_msg ָ����յ���Ӧ��˽����Ϣ����ͬ����ʽ���ͣ�
 * \param[out]   none
 * \return       bool
 * \retval           ����Ӧ��ͬ����Ϣ�Ƿ�ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1��Ӧ��ͬ����Ϣ MSG_MENGINE_GET_FILEPATH_SYNC
 libc_memcpy((file_path_info_t *)private_msg.reply.content.addr, &tmp_file_path, sizeof(file_path_info_t));
 private_msg.reply.type = MSG_REPLY_SUCCESS;
 reply_sync_msg_inner(&private_msg);
 * \endcode
 * \note
 *******************************************************************************/

bool reply_sync_msg_inner(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}

/*! \endcond */

