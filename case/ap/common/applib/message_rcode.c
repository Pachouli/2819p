/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用消息通信模块接收消息接口实现，支持应用消息临时缓冲；软定时器handle执行
 *       是挂载在这里的，前台应用还会执行蓝牙管理器LOOP，收到消息会回调钩子函数。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*!
 *  \brief
 *  g_buffer_app_msg 缓冲app消息
 */
static private_msg_t g_buffer_app_msg;
static private_msg_t g_buffer_hotplug_msg;
static bool g_only_get_hotplug_msg;
static bool g_last_is_hotplug_msg;

app_msg_dispatch_handle g_app_msg_dispatch_handle;

extern void com_app_msg_hook(private_msg_t *private_msg);

extern void com_bt_event_loop_inner(void);

#if (SUPPORT_FAST_PREVIEW == 1)
extern bool com_fast_preview_need_switch_ap(uint8 *func_id);
#endif

/*! \cond COMMON_API */

bool get_app_msg_hotplug(app_type_e type, private_msg_t *private_msg)
{
    bool ret_val;

    g_only_get_hotplug_msg = TRUE;

    ret_val = get_app_msg_inner(type, private_msg);

    g_only_get_hotplug_msg = FALSE;

    return ret_val;
}

void set_app_msg_dispatch_handle_inner(app_msg_dispatch_handle dispatch_handle)
{
    g_app_msg_dispatch_handle = dispatch_handle;
}

bool get_hotplug_msg(private_msg_t *private_msg)
{
    sys_msg_t sys_msg;
    if (sys_mq_receive(MQ_ID_HOTPLUG, (void *) &sys_msg) == 0)
    {
        g_last_is_hotplug_msg = TRUE;
        private_msg->msg.type = sys_msg.type;
        libc_printf_info("async msg:0x%x\n", sys_msg.type);
        com_app_msg_hook(private_msg);
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    接收应用私有消息（有些来自于接收系统消息时转发的）。
 * \param[in]    none
 * \param[out]   private_msg：返回接收到的应用私有消息
 * \return       bool
 * \retval           TRUE 接收应用私有消息成功。
 * \retval           FALSE 接收应用私有消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 例子1：应用私有消息处理
 private_msg_t private_msg;
 app_result_e result;
 bool ret;

 ret = get_app_msg_inner(&private_msg);
 if(ret == TRUE)
 {
 result = msg_callback(&private_msg);
 if(result == RESULT_REDRAW)
 {
 返回重绘
 }
 else
 {
 其他返回值处理
 }
 }
 * \endcode
 * \note
 *******************************************************************************/
bool get_app_msg_inner(app_type_e type, private_msg_t *private_msg)
{
    //front app
    if (type == APP_TYPE_GUI)
    {
        sys_msg_t sys_msg;

        //蓝牙管理器事件处理
        if (get_bt_state_inner() == BT_STATE_WORK)
        {
            com_bt_event_loop_inner();
        }

        //软定时器扫描
        handle_timers(APP_TIMER_GROUP_GUI);
        handle_timers(APP_TIMER_GROUP_COMMON);

        //优先处理热拔插消息
        if (g_buffer_hotplug_msg.msg.type != MSG_NULL) //缓冲消息，直接拷贝
        {
            libc_memcpy(private_msg, &g_buffer_hotplug_msg, sizeof(private_msg_t));
            g_buffer_hotplug_msg.msg.type = MSG_NULL;
            return TRUE;
        }

        if (get_hotplug_msg(private_msg) == TRUE)
        {
            return TRUE;
        }

        g_last_is_hotplug_msg = FALSE;

        if (g_only_get_hotplug_msg == FALSE)
        {
            uint8 func_index;

            if (g_buffer_app_msg.msg.type != MSG_NULL) //缓冲消息，直接拷贝
            {
                libc_memcpy(private_msg, &g_buffer_app_msg, sizeof(private_msg_t));
                g_buffer_app_msg.msg.type = MSG_NULL;
                return TRUE;
            }

#if (SUPPORT_FAST_PREVIEW == 1)
            if (com_fast_preview_need_switch_ap(&func_index) == TRUE)
            {
                msg_apps_t msg;
                msg.type = MSG_FAST_PREVIEW_SWITCH_AP;
                g_p_global_var->fp_switch_func_index = func_index;
                broadcast_msg_inner(&msg, BC_FRONT_APP);
            }
            else
#endif
            {
                //获取系统消息并广播
                if (sys_mq_receive(MQ_ID_SYS, (void *) &sys_msg) == 0)
                {
                    msg_apps_t msg;
                    msg.type = sys_msg.type;
                    broadcast_msg_inner(&msg, BC_FRONT_APP);
                }
            }

            //接收 应用私有消息
            if (sys_mq_receive(MQ_ID_DESK, (void *) private_msg) == 0)
            {
                com_app_msg_hook(private_msg);
                return TRUE;
            }
        }

        //消息队列已空，返回false
        return FALSE;
    }
    //engine app
    else if (type == APP_TYPE_CONSOLE)
    {
        //软定时器扫描
        handle_timers(APP_TIMER_GROUP_CONSOLE);

        //接收 应用私有消息
        if (sys_mq_receive(MQ_ID_EGN, (void *) private_msg) == 0)
        {
            return TRUE;
        }

        //消息队列已空，返回false
        return FALSE;
    }
    //bt engine
    else
    {
        //软定时器扫描
        handle_timers(APP_TIMER_GROUP_BTENGINE);

        //接收 应用私有消息
        if (sys_mq_receive(MQ_ID_BT, (void *) private_msg) == 0)
        {
            return TRUE;
        }

        //消息队列已空，返回false
        return FALSE;
    }
}

void message_redeal_app_msg_inner(private_msg_t *p_app_msg)
{
    if (p_app_msg == NULL)
    {
        libc_memset(&g_buffer_app_msg, 0x00, sizeof(private_msg_t));
        libc_memset(&g_buffer_hotplug_msg, 0x00, sizeof(private_msg_t));
    }
    else
    {
        if (g_last_is_hotplug_msg == TRUE)
        {
            libc_memcpy(&g_buffer_hotplug_msg, p_app_msg, sizeof(private_msg_t));
        }
        else
        {
            libc_memcpy(&g_buffer_app_msg, p_app_msg, sizeof(private_msg_t));
        }
    }
}

/*! \endcond */
