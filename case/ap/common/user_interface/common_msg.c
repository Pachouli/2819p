/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ����Ϣ���Ӻ���������ϵͳ����״̬�����ʡ��ػ���ϵͳ��ʱ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ǰ̨Ӧ��˽����Ϣ��������
 * \param[in]    private_msg ��ȡ����˽����Ϣָ��
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 *******************************************************************************/
void com_app_msg_hook(private_msg_t *private_msg)
{
    msg_apps_type_e msg_type = private_msg->msg.type;
    bool vip_event_flag = FALSE;
    switch (msg_type)
    {
        case MSG_SD_IN:
        change_card_state_inner (CARD_STATE_CARD_IN);
        vip_event_flag = TRUE;
        break;
        case MSG_SD_OUT:
        change_card_state_inner (CARD_STATE_CARD_NULL);
        vip_event_flag = TRUE;
        break;

        case MSG_UH_IN:
#if (SUPPORT_PHONE_CHARGING == 1)
        if (g_com_comval.support_iphonecharge == 1)
        {
            change_uhost_type_inner(hal_get_uhost_type());
        }
#endif
        change_uhost_state(UHOST_IN);

        vip_event_flag = TRUE;
        break;
        case MSG_UH_OUT:
        change_uhost_state(UHOST_NULL);
#if (SUPPORT_PHONE_CHARGING == 1)
        if (g_com_comval.support_iphonecharge == 1)
        {
            change_uhost_type_inner(DEFAULT_TYPE);
        }
#endif
        vip_event_flag = TRUE;
        break;

        case MSG_LINEIN_IN:
        change_linein_state(LINEIN_STATE_LINEIN_IN);
        vip_event_flag = TRUE;
        break;
        case MSG_LINEIN_OUT:
        change_linein_state(LINEIN_STATE_LINEIN_NULL);
        vip_event_flag = TRUE;
        break;

        case MSG_USB_STICK:
        change_cable_state (CABLE_STATE_CABLE_IN);
        vip_event_flag = TRUE;
        break;

        case MSG_USB_UNSTICK:
        change_cable_state (CABLE_STATE_CABLE_NULL);
        vip_event_flag = TRUE;
        break;

        case MSG_ADAPTOR_IN:
        change_adaptor_state (ADAPTOR_STATE_CABLE_IN);
        vip_event_flag = TRUE;
        break;

        case MSG_ADAPTOR_OUT:
        change_adaptor_state (ADAPTOR_STATE_CABLE_NULL);
        vip_event_flag = TRUE;
        break;

        case MSG_BT_CON_CHANGED:
        vip_event_flag = TRUE;
        break;

        case MSG_KEEP_LED_DISPLAY:
        vip_event_flag = TRUE;
        break;

        default:
        break;
    }

    if (vip_event_flag == TRUE)
    {
        //ʡ��ػ���ʱ ��0
        sys_counter_reset();
    }
}

/*! \endcond */
