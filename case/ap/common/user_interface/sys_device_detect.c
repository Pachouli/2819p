/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������豸���ӿڣ�����ʱ�����ڵ��ü�⣻����AUX������USB�����ߣ�USB��������ߡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! \cond COMMON_API */

void linein_hot_detect(void)
{
    if (get_linein_state_inner() == LINEIN_STATE_LINEIN_NULL)
    {
        key_aux_hot_detect(TRUE);
    }
    else
    {
        key_aux_hot_detect(FALSE);
    }
}

//�����Ƿ�������룬������������ϵͳ���߼�����״̬���Ե�
bool usbhost_hot_detect(void)
{
    if (get_uhost_state_inner() == UHOST_NULL)
    {
        return key_usbhost_hot_detect(TRUE);
    }
    else
    {
        return key_usbhost_hot_detect(FALSE);
    }
}
//�����Ƿ�������룬������������ϵͳ���߼�����״̬���Ե�
bool usbdev_hot_detect(void)
{
    if (get_cable_state_inner() == CABLE_STATE_CABLE_NULL)
    {
        bool ret = FALSE;
        //���ĳЩU�̼��������⣬����ʹ��Udeviceģʽ����ЩU������ͨ��
        if (g_com_comval.is_udev_det_base_dc5v_det == 1)
        {
            if (get_adaptor_state_inner() == ADAPTOR_STATE_CABLE_IN)
            {
                key_usb_detect_switch(USB_DET_FOR_DEVICE);
                ret = key_usbdev_hot_detect(TRUE);
            }
        }
        else
        {
            ret = key_usbdev_hot_detect(TRUE);
        }

        return ret;
    }
    else
    {
        return key_usbdev_hot_detect(FALSE);
    }
}

bool is_dc5v_active(void)
{
#if (SUPPORT_DC5V_DETECT == DETECT_BY_IO)
    uint8 plug_in = 0;

    ioctl_pad(DC5V_DETECT_PIN, DC5V_DETECT_FUNC, &plug_in);

    return (plug_in == DC5V_DETECT_LEVEL);
#else
    return FALSE;
#endif
}

bool sys_card_detect_inner(void)
{
    return (hal_card_detect() != -1);
}

bool sys_usb_detect_inner(void)
{
    if ((g_com_comval.is_udev_det_base_dc5v_det == 1) && (FALSE == is_dc5v_active()))
    {
        return FALSE;
    }
    return (hal_usb_detect() != -1);
}

bool sys_uhost_detect_inner(void)
{
    if ((g_com_comval.is_udev_det_base_dc5v_det == 1) && (TRUE == is_dc5v_active()))
    {
        return FALSE;
    }
    return (hal_uhost_detect() != -1);
}

void detect_usb_device_host_deal(void)
{
    static uint8 s_detect_uhost = 1,usb_switch_cnt = 10;

    if (g_com_comval.support_uhostplay == 0)
    {
        if (s_detect_uhost == 1)
        {
            key_usb_detect_switch(USB_DET_FOR_DEVICE);
            s_detect_uhost = 0;
        }
    }

    if (get_cur_func_id() == APP_FUNC_PLAYUHOST)
    {
        usbhost_hot_detect();
        return;
    }

    if ((get_cur_func_id() == APP_FUNC_UDISK) || (get_cur_func_id() == APP_FUNC_USOUND) || (com_get_stub_tools_type_inner() != 0))
    {
        usbdev_hot_detect();
        return;
    }
    if (g_com_comval.support_iphonecharge == 1)
    {
        if ((s_detect_uhost == 0) || (usb_switch_cnt > 0))
        {
            key_usb_detect_switch(USB_DET_FOR_UHOST);
            s_detect_uhost = 1;
        }
        usbhost_hot_detect();
        return;
    }

    if (s_detect_uhost == 1)
    {
        if (usbhost_hot_detect() == FALSE)
        {
            if ((g_com_comval.support_usbbox == 1) || (g_com_comval.support_usbmsc == 1))
            {
                if (g_com_comval.is_udev_det_base_dc5v_det == 0)
                {
                    key_usb_detect_switch(USB_DET_FOR_DEVICE);
                }
                s_detect_uhost = 0;
            }
        }
    }
    else
    {
        if (usbdev_hot_detect() == FALSE)
        {
            if (g_com_comval.support_uhostplay == 1)
            {
                if (g_com_comval.is_udev_det_base_dc5v_det == 1)
                {
                    if (is_dc5v_active() == FALSE)
                    {
                        key_usb_detect_switch(USB_DET_FOR_UHOST);
                    }
                }
                else
                {
                    key_usb_detect_switch(USB_DET_FOR_UHOST);
                }
                s_detect_uhost = 1;
            }
        }
    }

}

void adaptor_hot_detect(void)
{
    if (get_adaptor_state_inner() == ADAPTOR_STATE_CABLE_NULL)
    {
        key_adaptor_hot_detect(TRUE);
    }
    else
    {
        key_adaptor_hot_detect(FALSE);
    }
}

void card_hot_detect(void)
{
    //U���¿������Ӧ���Լ�����U��Ӧ��һ����ÿ�������
    if (get_cur_func_id() == APP_FUNC_UDISK)
    {
        return;
    }

#if (SUPPORT_CARD_DETECT == DETECT_BY_CMD)
    //�忨����Ӧ��/��¼���ط�Ӧ���£�����������Բ�Ҫ����Ӧ���Լ������Ա��⿨����뿨���ݶ�д���������ͻ
    if ((get_cur_func_id() == APP_FUNC_PLAYCARD)
        || (get_cur_func_id() == APP_FUNC_CARD_REPLAY)
        || (get_cur_func_id() == APP_FUNC_ALARM_SDCARD))
    {
        return;
    }
#endif

    if (get_card_state_inner() == CARD_STATE_CARD_NULL)
    {
        key_card_hot_detect(TRUE);
    }
    else
    {
        key_card_hot_detect(FALSE);
    }
}

void sys_hot_detect_handle_inner(void)
{
    if ((g_com_comval.support_linein == 1) && (g_com_comval.support_aux_detect == TRUE))
    {
        linein_hot_detect();
    }

    if ((g_com_comval.support_usbbox == 1) || (g_com_comval.support_usbmsc == 1) || (g_com_comval.support_uhostplay == 1))
    {
        detect_usb_device_host_deal();
    }

    if (g_com_comval.support_dc5v_detect == 1)
    {
        adaptor_hot_detect();
    }

    if (g_com_comval.hard_support_card == 1)
    {
        card_hot_detect();
    }
}

/*! \endcond */
