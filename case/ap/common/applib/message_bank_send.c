/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用消息通信模块发送同步消息和回复消息接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

#define  SYNC_MSG_TIMEOUT  3000 //30秒钟延时，单位10ms；timeout=0会强制为该值；

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    以同步方式发送应用私有消息。
 * \param[in]    app_id 同步发送应用私有消息的目标应用ID
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[in]    reply 指向同步消息应答结构体
 * \param[in]    timeout 超时发送时间，0 表示一直等待不超时返回  单位10ms
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 发送同步消息成功。
 * \retval           FALSE 发送同步消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 例子1：阻塞式发送同步消息，直到对方调用 reply_sync_msg_inner 应答
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
 正确返回，获取并处理文件路径
 }
 else
 {
 错误返回
 }
 }
 else
 {
 其他错误
 }
 * \endcode
 * \note
 * \li  返回错误原因包括：找不到接收应用，发送同步消息给自己，同步消息被过滤掉，消息队列已满，等。
 * \li  注意消息内容缓冲区 msg.content.addr 和 应答缓冲区 reply.content 的空间都是开在消息发送方的。
 *      由于发送同步消息后会等待返回，如果调用 send_sync_msg_inner 的代码是在 control bank段，那么上面的
 *      缓冲区可以开在bank data中。
 *******************************************************************************/
bool send_sync_msg_inner(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    const app_info_t *target_app_info;
    msg_reply_t temp_reply;
    os_event_t *sem; //同步消息同步信号量指针
    bool retvalue = TRUE;

    if (timeout == 0)
    {
        timeout = SYNC_MSG_TIMEOUT; //避免因同步消息没有响应而死机
    }

    //通用引擎ID转换为具体APP ID
    if (app_id == APP_ID_THEENGINE)
    {
        app_id = get_engine_appid_by_type_inner(get_engine_type_inner());
        if (app_id == APP_ID_MAX)
        {
            goto app_noexist;
        }
    }

    //查找目标应用
    target_app_info = applib_get_app_info_by_app_id(app_id);
    if (NULL == target_app_info)
    {
        app_noexist:
        libc_printf_warning("sync msg app_noexist!!0x%x\n", msg->type);
        return FALSE;
    }

    //填充私有消息结构体
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

    //发送消息到队列中
    if (sys_mq_send((uint32)(target_app_info->mq_id), (void *) &cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_printf_warning("sync msg mq_send err!!0x%x\n", msg->type);
        return FALSE;
    }

    //等待同步消息回应
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
 *    应答同步消息，仅需要释放信号量即可
 * \param[in]    private_msg 指向接收到的应用私有消息（以同步方式发送）
 * \param[out]   none
 * \return       bool
 * \retval           返回应答同步消息是否成功，true表示成功，false表示失败
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 例子1：应答同步消息 MSG_MENGINE_GET_FILEPATH_SYNC
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

