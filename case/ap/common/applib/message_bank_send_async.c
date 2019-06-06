/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ����Ϣͨ��ģ�鷢���첽��Ϣ�ӿ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ���첽��ʽ����Ӧ��˽����Ϣ��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �����첽��Ϣ�ɹ���
 * \retval           FALSE �����첽��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1���첽������Ϣ������ȴ���������
 msg_apps_t msg;
 bool ret;

 msg.type = MSG_KILL_APP;
 msg.content.data[0] = APP_ID_MUSIC;
 ret = send_async_msg_inner(APP_ID_MANAGER, &msg);
 if(ret == FALSE)
 {
 ��������
 }
 * \endcode
 * \note
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã��첽��Ϣ�����˵�����Ϣ�����������ȡ�
 * \li  ˽����Ϣ�첽���ͷ�ʽ�����ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
 *******************************************************************************/
bool send_async_msg_inner(uint8 app_id, msg_apps_t *msg)
{
    const app_info_t *target_app_info;
    private_msg_t cur_send_pmsg;

    libc_printf_info("async msg:0x%x\n", msg->type);

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
    if (target_app_info == NULL)
    {
        app_noexist:
        libc_printf_warning("async msg app_noexist!!\n");
        return FALSE;
    }

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //������Ϣ��������
    if (sys_mq_send((uint32)(target_app_info->mq_id), (void *) &cur_send_pmsg) == -1)
    {
        libc_printf_warning("async msg mq_send fail!!\n");
        return FALSE;
    }

    return TRUE;
}

/*! \endcond */

