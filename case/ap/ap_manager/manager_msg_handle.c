/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù���������������Ӧ�õ���Ϣ�Ĵ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "manager.h"

void wait_ap_exit(void);
void kill_app(uint8 ap_id);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ϣ����
 * \param[in]    ˽����Ϣָ��
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void manager_msg_callback(private_msg_t *pri_msg)
{
    uint8 ap_name[APP_NAME_MAX];
    uint8 app_id;
    uint8 ap_param;
    bool app_exsit_flag = FALSE;
    app_info_t tmp_app_info;

    switch (pri_msg->msg.type)
    {
        //����ǰ̨Ӧ��
        case MSG_CREAT_APP:
        {
            app_id = pri_msg->msg.para[0];
            ap_param = pri_msg->msg.para[1];

            manager_get_app_name(ap_name, app_id);
            libc_printf_info("MSG_CREAT_APP : %s\n", ap_name);

            wait_ap_exit();

            sys_exece_ap(ap_name, 0, (void *)(uint32) ap_param);
        }
        break;

        //������̨Ӧ��
        case MSG_CREAT_APP_SYNC:
        {
            app_id = pri_msg->msg.para[0];

            manager_get_app_name(ap_name, app_id);

            libc_printf_info("MSG_CREAT_APP_SYNC : %s\n", ap_name);

            if (app_id == APP_ID_BTENGINE)
            {
                applib_get_app_info(APP_TYPE_BTENGINE, &tmp_app_info);
                if (tmp_app_info.used == 1)
                {
                    app_exsit_flag = TRUE;
                }
            }
            else
            {
                applib_get_app_info(APP_TYPE_CONSOLE, &tmp_app_info);
                if (tmp_app_info.used == 1)
                {
                    app_exsit_flag = TRUE;
                }
            }

            if (app_exsit_flag == FALSE)
            {
                int ret = 0;

                if (pri_msg->msg.content.addr == NULL)
                {
                    ap_param = pri_msg->msg.para[1];
                    ret = sys_exece_ap(ap_name, 0, (void *)(uint32) ap_param);
                }
                else
                {
                    ret = sys_exece_ap(ap_name, 0, (void *) pri_msg->msg.content.addr);
                }

                if (ret >= 0)
                {
                    pri_msg->reply->type = MSG_REPLY_SUCCESS;
                }
                else
                {
                    pri_msg->reply->type = MSG_REPLY_FAILED;
                }
                reply_sync_msg(pri_msg);
            }
            else
            //�����̨���ڲŴ���
            {
                libc_printf_warning("MSG_CREAT_APP_SYNC : %s, already exist!!\n", ap_name);

                pri_msg->reply->type = MSG_REPLY_SUCCESS;
                reply_sync_msg(pri_msg);
            }
        }
        break;

        case MSG_KILL_APP_SYNC:
        {
            app_id = pri_msg->msg.para[0];

            if (app_id == APP_ID_THEENGINE)
            {
                app_id = get_engine_appid_by_type(get_engine_type());
            }

            if (app_id == APP_ID_MAX)
            {
                libc_printf_warning("MSG_KILL_APP_SYNC engine NOT find!!\n");
                goto kill_over;
            }

            manager_get_app_name(ap_name, app_id);
            libc_printf_info("MSG_KILL_APP_SYNC : %s\n", ap_name);

            if (app_id == APP_ID_BTENGINE)
            {
                applib_get_app_info(APP_TYPE_BTENGINE, &tmp_app_info);
                if (tmp_app_info.used == 1)
                {
                    app_exsit_flag = TRUE;
                }
            }
            else
            {
                applib_get_app_info(APP_TYPE_CONSOLE, &tmp_app_info);
                if (tmp_app_info.used == 1)
                {
                    app_exsit_flag = TRUE;
                }
            }

            //�����̨���ڲŴ���
            if (app_exsit_flag == TRUE)
            {
                //ɱ����̨
                kill_app(app_id);
                pri_msg->reply->type = MSG_REPLY_SUCCESS;
                reply_sync_msg(pri_msg);
            }
            else //��̨�Ѿ���ɱ����ֱ�ӷ��سɹ�������ǰ̨�ղ�����Ӧ������
            {
                kill_over:
                pri_msg->reply->type = MSG_REPLY_SUCCESS;
                reply_sync_msg(pri_msg);
            }
        }
        break;

        default:
        libc_printf_warning("INVALID MANAGER MSG TYPE:0x%x\n", pri_msg->msg.type);
        break;
    }

}


/******************************************************************************/
/*!
 * \par  Description:
 *  �ȴ�ǰ����̨ap�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music
 * \note
 *******************************************************************************/
void wait_ap_exit(void)
{
    uint8 ap_ret;
    int32 waitpid_ret;

    while (1)
    {
        waitpid_ret = libc_waitpid(&ap_ret, 0);
        if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
        {
            break;
        }
    }

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ɱ��ap����
 * \param[in]    ap_id ap���
 * \param[out]   none
 * \return       none
 * \ingroup      music
 * \note
 *******************************************************************************/
void kill_app(uint8 ap_id)
{
    msg_apps_t async_msg;

    async_msg.type = MSG_APP_QUIT;
    send_async_msg(ap_id, &async_msg);

    wait_ap_exit();

    return;
}
