/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用消息通信模块消息广播接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    以广播方式（也属于异步发送）发送应用私有消息。
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[in]    bc_mode 广播模式，0 只广播给前台应用  1 广播给所有应用
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 广播消息成功。
 * \retval           FALSE 广播消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 例子1：广播 MSG_POWER_OFF 关机消息
 msg_apps_t msg;

 msg.type = MSG_POWER_OFF;
 broadcast_msg_inner(&msg);
 * \endcode
 * \note
 * \li  广播应用消息最多只能附带4个字节的消息内容（建议采用双方约定code作为消息内容）。
 *******************************************************************************/
bool broadcast_msg_inner(msg_apps_t *msg, uint8 bc_mode)
{
    app_info_t *app;
    uint8 i;

    //广播该消息给所有应用
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
