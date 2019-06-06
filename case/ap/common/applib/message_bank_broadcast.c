/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ����Ϣͨ��ģ����Ϣ�㲥�ӿ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    �Թ㲥��ʽ��Ҳ�����첽���ͣ�����Ӧ��˽����Ϣ��
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[in]    bc_mode �㲥ģʽ��0 ֻ�㲥��ǰ̨Ӧ��  1 �㲥������Ӧ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �㲥��Ϣ�ɹ���
 * \retval           FALSE �㲥��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1���㲥 MSG_POWER_OFF �ػ���Ϣ
 msg_apps_t msg;

 msg.type = MSG_POWER_OFF;
 broadcast_msg_inner(&msg);
 * \endcode
 * \note
 * \li  �㲥Ӧ����Ϣ���ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
 *******************************************************************************/
bool broadcast_msg_inner(msg_apps_t *msg, uint8 bc_mode)
{
    app_info_t *app;
    uint8 i;

    //�㲥����Ϣ������Ӧ��
    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = applib_get_app_info_by_type(i);
        if (bc_mode == BC_FRONT_APP)
        {
            if ((app->used == 1) && (app->app_type == APP_TYPE_GUI))
            {
                send_async_msg_inner(app->app_id, msg);
            }
        }
        else
        {
            if (app->used == 1)
            {
                send_async_msg_inner(app->app_id, msg);
            }
        }
    }

    return TRUE;
}

/*! \endcond */
