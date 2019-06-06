/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：系统快捷键定义及按键消息映射接口实现；按键映射以应用自定义按键映射表为优先，
 *       应用也可以指定不映射系统快捷键。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! \cond COMMON_API */

/*!
 * \brief
 *  key_shortcut_list：快捷键映射表
 */
const key_map_t key_shortcut_list[] =
{
    /*! 长按 PLAY 按键关机 */
    { { KEY_PLAY, 0, KEY_TYPE_LONG }, EVENT_ENTER_SHUTOFF_PLAY },
    /*! 长按 PLAY 按键2秒钟关机 */
    //{ { KEY_PLAY, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ENTER_SHUTOFF_PLAY_SEC },
    /*! 短按 POWER 按键关机 */
    { { KEY_POWER, 0, KEY_TYPE_SHORT_UP }, EVENT_ENTER_SHUTOFF },

    /*! 模式切换事件 */
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_MODE_SWITCH },

    /*! 长按 MODE 2秒钟进入RTC时钟显示 */
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ENTER_RTC_TIME },

    /*! 长按 Connect 2秒钟切换连接：断开连接或回连最后一个设备 */
    { { KEY_CONNECT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_SWITCH_CONNECT },

    /*! 短按/按住 VADD 按键转换为 EVENT_VOLUME_ADD 事件 */
    { { KEY_VADD, 0, KEY_TYPE_DOWN }, EVENT_VOLUME_ADD },
    { { KEY_VADD, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_ADD_HOLD },
    /*! 短按/按住 VSUB 按键转换为 EVENT_VOLUME_SUB 事件 */
    { { KEY_VSUB, 0, KEY_TYPE_DOWN }, EVENT_VOLUME_SUB },
    { { KEY_VSUB, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_SUB_HOLD },
    /*! 按住 NEXT_VOLADD 按键转换为 EVENT_VOLUME_ADD_NEXT 事件 */
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_ADD_NEXT },
    /*! 按住 PREV_VOLSUB 按键转换为 EVENT_VOLUME_SUB_PREV 事件 */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_SUB_PREV },

    /*! 短按 Mute 按键转换为 EVENT_SWITCH_MUTE 事件 */
    { { KEY_MUTE, 0, KEY_TYPE_SHORT_UP }, EVENT_SWITCH_MUTE },

    /*! 短按 EQ 按键转换为 EVENT_EQ_NEXT */
    /*{ { KEY_EQ, 0, KEY_TYPE_SHORT_UP }, EVENT_EQ_NEXT },*/

    /* 录音快捷键放在前面，表示录音优先SIRI */
#if (SUPPORT_RECORD_FUNC == 1)
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RECORD_PLAY_PAUSE },
#endif

#if (SUPPORT_BT_SIRI == 1)
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_SIRI_START },
#endif

    /*! 短按REPLAY键切换到录音回放模式 */
    { { KEY_REPLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_ENTER_REPLAY },

    /*! 话筒优先开关 */
    { { KEY_DUCKING_SWITCH, 0, KEY_TYPE_SHORT_UP }, EVENT_DUCKING_SWITCH },
    /*! 原音消除开关 */
    { { KEY_REMOVAL_SWITCH, 0, KEY_TYPE_SHORT_UP }, EVENT_REMOVAL_SWITCH },
    /*! 录音开关 */
    { { KEY_RECORD_SWITCH, 0, KEY_TYPE_SHORT_UP }, EVENT_RECORD_SWITCH },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 *    按键消息映射，将 gui 消息映射为事件，以支持不同按键输入对应统一业务处理。
 * \param[in]    input_msg 要进行映射的 gui 消息
 * \param[in]    key_map_list 进行映射的按键映射表
 * \param[out]   gui_event 存储映射后事件
 * \return       bool
 * \retval           TRUE 映射得到有效 gui 事件。
 * \retval           FALSE 没有映射得到有效 gui 事件，按键消息被过滤掉了。
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 例子1：gui 消息（事件）循环
 input_gui_msg_t input_msg;
 msg_apps_type_e gui_event;
 bool ret;

 ret = get_gui_msg_inner(&input_msg);
 if(ret == TRUE)
 {
 if(com_key_mapping_inner(&input_msg, &gui_event, key_map_list) == TRUE)
 {
 switch(gui_event)
 {
 case EVENT_DO_SOMETHING:
 break;

 default:
 result = com_message_box_inner(gui_event);
 break;
 }
 }
 }
 else
 {
 gui 消息（事件）处理完毕，接下来可以处理应用私有消息了
 }
 * \endcode
 * \note
 * \li  按键映射接口中，会用两个映射表进行映射，一个是接口内默认的快捷键映射表，
 *      一个是用户自定义的按键映射表，后者优先级比较高。
 * \li  如果用户不想响应某个快捷键，或者想把某个快捷键另作别用，那么可以在自定义
 *      的按键映射表中添加该按键的映射项。
 *******************************************************************************/
bool com_key_mapping_inner(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event, const key_map_t *key_map_list)
{
    const key_map_t *this_key_map;
    uint16 key_type;
    bool use_shortcut = TRUE;
    bool key_val_match;

    key_type = input_msg->data.kmsg.type;

    this_key_map = key_map_list;
    for (;; this_key_map++)
    {
        /* 达到用户最后一个映射，结束按键映射 */
        if (KEY_NULL == this_key_map->key_event.val)
        {
            break;
        }
        if (KEY_NULL_NO_SHORTCUT == this_key_map->key_event.val)
        {
            //不使用快捷键映射表，用于嵌套子场景中
            use_shortcut = FALSE;
            break;
        }

        key_val_match = FALSE;
        if (this_key_map->key_event.val == KEY_NUMBER)
        {
            if ((input_msg->data.kmsg.val >= KEY_NUM0) && (input_msg->data.kmsg.val <= KEY_NUM9))
            {
                key_val_match = TRUE;
            }
        }
        else
        {
            if (this_key_map->key_event.val == input_msg->data.kmsg.val)
            {
                key_val_match = TRUE;
            }
        }

        /* 找到需要映射的按键, 进行映射 */
        if ((key_val_match == TRUE) && ((this_key_map->key_event.type & key_type) != 0))
        {
            *gui_event = this_key_map->event;
            if (*gui_event == MSG_NULL)
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }

    //快捷键映射
    if (use_shortcut == TRUE)
    {
        this_key_map = key_shortcut_list;
        for (;; this_key_map++)
        {
            /* 达到用户最后一个快捷键, 或者达到最大快捷键个数, 退出*/
            if (0 == this_key_map->key_event.val)
            {
                break;
            }

            key_val_match = FALSE;
            if (this_key_map->key_event.val == KEY_NUMBER)
            {
                if ((input_msg->data.kmsg.val >= KEY_NUM0) && (input_msg->data.kmsg.val <= KEY_NUM9))
                {
                    key_val_match = TRUE;
                }
            }
            else
            {
                if (this_key_map->key_event.val == input_msg->data.kmsg.val)
                {
                    key_val_match = TRUE;
                }
            }

            /* 找到快捷键, 发送快捷键对应消息 */
            if ((key_val_match == TRUE) && ((this_key_map->key_event.type & key_type) != 0))
            {
                *gui_event = this_key_map->event;
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*! \endcond */
