/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ��ݼ����弰������Ϣӳ��ӿ�ʵ�֣�����ӳ����Ӧ���Զ��尴��ӳ���Ϊ���ȣ�
 *       Ӧ��Ҳ����ָ����ӳ��ϵͳ��ݼ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! \cond COMMON_API */

/*!
 * \brief
 *  key_shortcut_list����ݼ�ӳ���
 */
const key_map_t key_shortcut_list[] =
{
    /*! ���� PLAY �����ػ� */
    { { KEY_PLAY, 0, KEY_TYPE_LONG }, EVENT_ENTER_SHUTOFF_PLAY },
    /*! ���� PLAY ����2���ӹػ� */
    //{ { KEY_PLAY, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ENTER_SHUTOFF_PLAY_SEC },
    /*! �̰� POWER �����ػ� */
    { { KEY_POWER, 0, KEY_TYPE_SHORT_UP }, EVENT_ENTER_SHUTOFF },

    /*! ģʽ�л��¼� */
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_MODE_SWITCH },

    /*! ���� MODE 2���ӽ���RTCʱ����ʾ */
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_ENTER_RTC_TIME },

    /*! ���� Connect 2�����л����ӣ��Ͽ����ӻ�������һ���豸 */
    { { KEY_CONNECT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_SWITCH_CONNECT },

    /*! �̰�/��ס VADD ����ת��Ϊ EVENT_VOLUME_ADD �¼� */
    { { KEY_VADD, 0, KEY_TYPE_DOWN }, EVENT_VOLUME_ADD },
    { { KEY_VADD, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_ADD_HOLD },
    /*! �̰�/��ס VSUB ����ת��Ϊ EVENT_VOLUME_SUB �¼� */
    { { KEY_VSUB, 0, KEY_TYPE_DOWN }, EVENT_VOLUME_SUB },
    { { KEY_VSUB, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_SUB_HOLD },
    /*! ��ס NEXT_VOLADD ����ת��Ϊ EVENT_VOLUME_ADD_NEXT �¼� */
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_ADD_NEXT },
    /*! ��ס PREV_VOLSUB ����ת��Ϊ EVENT_VOLUME_SUB_PREV �¼� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VOLUME_SUB_PREV },

    /*! �̰� Mute ����ת��Ϊ EVENT_SWITCH_MUTE �¼� */
    { { KEY_MUTE, 0, KEY_TYPE_SHORT_UP }, EVENT_SWITCH_MUTE },

    /*! �̰� EQ ����ת��Ϊ EVENT_EQ_NEXT */
    /*{ { KEY_EQ, 0, KEY_TYPE_SHORT_UP }, EVENT_EQ_NEXT },*/

    /* ¼����ݼ�����ǰ�棬��ʾ¼������SIRI */
#if (SUPPORT_RECORD_FUNC == 1)
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RECORD_PLAY_PAUSE },
#endif

#if (SUPPORT_BT_SIRI == 1)
    { { KEY_F1, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_SIRI_START },
#endif

    /*! �̰�REPLAY���л���¼���ط�ģʽ */
    { { KEY_REPLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_ENTER_REPLAY },

    /*! ��Ͳ���ȿ��� */
    { { KEY_DUCKING_SWITCH, 0, KEY_TYPE_SHORT_UP }, EVENT_DUCKING_SWITCH },
    /*! ԭ���������� */
    { { KEY_REMOVAL_SWITCH, 0, KEY_TYPE_SHORT_UP }, EVENT_REMOVAL_SWITCH },
    /*! ¼������ */
    { { KEY_RECORD_SWITCH, 0, KEY_TYPE_SHORT_UP }, EVENT_RECORD_SWITCH },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 *    ������Ϣӳ�䣬�� gui ��Ϣӳ��Ϊ�¼�����֧�ֲ�ͬ���������Ӧͳһҵ����
 * \param[in]    input_msg Ҫ����ӳ��� gui ��Ϣ
 * \param[in]    key_map_list ����ӳ��İ���ӳ���
 * \param[out]   gui_event �洢ӳ����¼�
 * \return       bool
 * \retval           TRUE ӳ��õ���Ч gui �¼���
 * \retval           FALSE û��ӳ��õ���Ч gui �¼���������Ϣ�����˵��ˡ�
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1��gui ��Ϣ���¼���ѭ��
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
 gui ��Ϣ���¼���������ϣ����������Դ���Ӧ��˽����Ϣ��
 }
 * \endcode
 * \note
 * \li  ����ӳ��ӿ��У���������ӳ������ӳ�䣬һ���ǽӿ���Ĭ�ϵĿ�ݼ�ӳ���
 *      һ�����û��Զ���İ���ӳ����������ȼ��Ƚϸߡ�
 * \li  ����û�������Ӧĳ����ݼ����������ĳ����ݼ��������ã���ô�������Զ���
 *      �İ���ӳ�������Ӹð�����ӳ���
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
        /* �ﵽ�û����һ��ӳ�䣬��������ӳ�� */
        if (KEY_NULL == this_key_map->key_event.val)
        {
            break;
        }
        if (KEY_NULL_NO_SHORTCUT == this_key_map->key_event.val)
        {
            //��ʹ�ÿ�ݼ�ӳ�������Ƕ���ӳ�����
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

        /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
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

    //��ݼ�ӳ��
    if (use_shortcut == TRUE)
    {
        this_key_map = key_shortcut_list;
        for (;; this_key_map++)
        {
            /* �ﵽ�û����һ����ݼ�, ���ߴﵽ����ݼ�����, �˳�*/
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

            /* �ҵ���ݼ�, ���Ϳ�ݼ���Ӧ��Ϣ */
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
