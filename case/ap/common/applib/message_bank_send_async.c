/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用消息通信模块发送异步消息接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    以异步方式发送应用私有消息。
 * \param[in]    app_id 同步发送应用私有消息的目标应用ID
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 发送异步消息成功。
 * \retval           FALSE 发送异步消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 例子1：异步发送消息，无须等待立即返回
 msg_apps_t msg;
 bool ret;

 msg.type = MSG_KILL_APP;
 msg.content.data[0] = APP_ID_MUSIC;
 ret = send_async_msg_inner(APP_ID_MANAGER, &msg);
 if(ret == FALSE)
 {
 发生错误
 }
 * \endcode
 * \note
 * \li  返回错误原因包括：找不到接收应用，异步消息被过滤掉，消息队列已满，等。
 * \li  私有消息异步发送方式，最多只能附带4个字节的消息内容（建议采用双方约定code作为消息内容）。
 *******************************************************************************/
bool send_async_msg_inner(uint8 app_id, msg_apps_t *msg)
{
    const app_info_t *target_app_info;
    private_msg_t cur_send_pmsg;

    libc_printf_info("async msg:0x%x\n", msg->type);

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
    if (target_app_info == NULL)
    {
        app_noexist:
        libc_printf_warning("async msg app_noexist!!\n");
        return FALSE;
    }

    //填充私有消息结构体
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //发送消息到队列中
    if (sys_mq_send((uint32)(target_app_info->mq_id), (void *) &cur_send_pmsg) == -1)
    {
        libc_printf_warning("async msg mq_send fail!!\n");
        return FALSE;
    }

    return TRUE;
}

/*! \endcond */

