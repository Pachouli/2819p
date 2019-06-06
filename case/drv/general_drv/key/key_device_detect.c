/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动设备热拔插检测实现，包括AUX、卡、USB数据线、USB充电适配线。
 * 作者：cailizhen
 ********************************************************************************/

#include "key_inner.h"

#define DEV_DET_OUT    0 //设备检查拔出
#define DEV_DET_MAYBE  1 //设备检测可能插入
#define DEV_DET_IN     2 //设备检查插入
#define DEV_DET_UNDEF  3 //设备无法确定插入或拔出

static uint8 usb_detect_mode = USB_DET_DISABLE;

//返回值：TRUE 表示设备插着，FALSE 表示设备没插着
typedef uint8 (*check_device_state)(void);

static bool check_device_in(check_device_state cbk, uint8 debounce_time, uint8 debounce_in_time);
static bool check_device_out(check_device_state cbk, uint8 debounce_time);

static uint8 check_aux_state(void);
static uint8 check_usbdev_state(void);
static uint8 check_usbhost_state(void);
static uint8 check_adaptor_state(void);
static uint8 check_card_state(void);

uint8 check_aux_state(void)
{
#if (SUPPORT_LINEIN_DETECT == DETECT_BY_IO)
    uint8 gpio_level;

    ioctl_pad(LINEIN_DETECT_PIN, LINEIN_DETECT_FUNC, &gpio_level);

    return (gpio_level == LINEIN_DETECT_LEVEL);
#elif (SUPPORT_LINEIN_DETECT == DETECT_BY_ADCKEY)
    uint16 adcdat = hal_pmu_read_lradc(adckey_idx);

    if (g_keymap_para.vcc_voltage_select == 0)
    {
        if (adcdat < LINEIN_ADCKEY_DET_ADC_VAL_MIN)
        {
            return DEV_DET_UNDEF;
        }
        else if (adcdat <= LINEIN_ADCKEY_DET_ADC_VAL_MAX)
        {
            return DEV_DET_MAYBE;
        }
        else
        {
            return DEV_DET_OUT;
        }
    }
    else
    {
        if (adcdat < LINEIN_ADCKEY_DET_ADC_VAL_AVCC_3_1_V_MIN)
        {
            return DEV_DET_UNDEF;
        }
        else if (adcdat <= LINEIN_ADCKEY_DET_ADC_VAL_AVCC_3_1_V_MAX)
        {
            return DEV_DET_MAYBE;
        }
        else
        {
            return DEV_DET_OUT;
        }
    }

#else
    return DEV_DET_OUT;
#endif
}

void key_inner_usb_detect_switch(uint8 det_mode)
{
    if (det_mode != usb_detect_mode)
    {
        usb_detect_mode = det_mode;
        hal_usb_dev_uhost_detect_switch(det_mode);
    }
}

uint8 check_usbdev_state(void)
{
    return (0 == hal_usb_detect());
}

uint8 check_usbhost_state(void)
{
    int32 ret;

    ret = hal_uhost_detect();
    if (ret == -1)
    {
        return DEV_DET_OUT;
    }
    else if (ret == 0)
    {
        return DEV_DET_MAYBE;
    }
    else
    {
        return DEV_DET_IN;
    }
}
uint8 check_adaptor_state(void)
{
#if (SUPPORT_DC5V_DETECT == DETECT_BY_IO)
    uint8 plug_in = 0;

    ioctl_pad(DC5V_DETECT_PIN, DC5V_DETECT_FUNC, &plug_in);

    return (plug_in == DC5V_DETECT_LEVEL);
#else
    return DEV_DET_OUT;
#endif
}

uint8 check_card_state(void)
{
#if (SUPPORT_CARD_DETECT == DETECT_BY_CMD)
    int32 ret = hal_card_detect();
    if (ret == -1)
    {
        return DEV_DET_OUT;
    }
    else if (ret == 0)
    {
        return DEV_DET_MAYBE;
    }
    else
    {
        return DEV_DET_IN;
    }
#else
    return DEV_DET_OUT;
#endif
}

bool key_inner_aux_hot_detect(bool chk_in)
{
    uint16 msg = 0;
    bool in_flag = FALSE;

    if (chk_in == TRUE)
    {
        if (check_device_in(check_aux_state, 10, 10) == TRUE)
        {
            msg = MSG_SYS_LINEIN_IN;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
            in_flag = TRUE;
        }
    }
    else
    {
        if (check_device_out(check_aux_state, 3) == TRUE)
        {
            msg = MSG_SYS_LINEIN_OUT;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
        }
        else
        {
            in_flag = TRUE;
        }
    }

    return in_flag;
}

bool key_inner_usbhost_hot_detect(bool chk_in)
{
    uint16 msg = 0;
    bool in_flag = FALSE;

    if (chk_in == TRUE)
    {
        if (check_device_in(check_usbhost_state, 8, 3) == TRUE)
        {
            msg = MSG_SYS_UH_IN;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
            in_flag = TRUE;
        }
    }
    else
    {
        if (check_device_out(check_usbhost_state, 3) == TRUE)
        {
            msg = MSG_SYS_UH_OUT;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
        }
        else
        {
            in_flag = TRUE;
        }
    }

    return in_flag;
}

bool key_inner_usbdev_hot_detect(bool chk_in)
{
    uint16 msg = 0;
    bool in_flag = FALSE;

    if (chk_in == TRUE)
    {
        if (check_device_in(check_usbdev_state, 25, 20) == TRUE)
        {
            msg = MSG_SYS_USB_STICK;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
            in_flag = TRUE;
        }
    }
    else
    {
        if (check_device_out(check_usbdev_state, 3) == TRUE)
        {
            msg = MSG_SYS_USB_UNSTICK;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
        }
        else
        {
            in_flag = TRUE;
        }
    }

    return in_flag;
}

bool key_inner_adaptor_hot_detect(bool chk_in)
{
    uint16 msg = 0;
    bool in_flag = FALSE;

    if (chk_in == TRUE)
    {
        if (check_device_in(check_adaptor_state, 3, 3) == TRUE)
        {
            msg = MSG_SYS_ADAPTOR_IN;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
            in_flag = TRUE;
        }
    }
    else
    {
        if (check_device_out(check_adaptor_state, 3) == TRUE)
        {
            msg = MSG_SYS_ADAPTOR_OUT;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
        }
        else
        {
            in_flag = TRUE;
        }
    }

    return in_flag;
}

bool key_inner_card_hot_detect(bool chk_in)
{
    uint16 msg = 0;
    bool in_flag = FALSE;

    if (chk_in == TRUE)
    {
        if (check_device_in(check_card_state, 50, 3) == TRUE)
        {
            msg = MSG_SYS_SD_IN;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
            in_flag = TRUE;
        }
    }
    else
    {
        if (check_device_out(check_card_state, 3) == TRUE)
        {
            msg = MSG_SYS_SD_OUT;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
        }
        else
        {
            in_flag = TRUE;
        }
    }

    return in_flag;
}

void key_inner_device_detect_init(bool need_usb_detect)
{
    pad_phy_para_t phy_para;

    if (need_usb_detect == TRUE)
    {//由于Uhost兼容性问题，默认配置为Uhost模式
        hal_set_usb_detect(USB_DETECT_ENABLE);
        usb_detect_mode = USB_DET_FOR_UHOST;
        hal_usb_dev_uhost_detect_switch(USB_DET_FOR_UHOST);
    }

    //打开DC5V检测口
#if (SUPPORT_DC5V_DETECT == DETECT_BY_IO)
    phy_para.pad_pu_en = !(DC5V_DETECT_LEVEL);
    phy_para.pad_pd_en = DC5V_DETECT_LEVEL;
    phy_para.smit_en = 0;
    phy_para.pad_driver = 1;
    set_pad_function(DC5V_DETECT_PIN, DC5V_DETECT_FUNC, &phy_para, 0);
#endif
    //打开LINEIN检测口
#if (SUPPORT_LINEIN_DETECT == DETECT_BY_IO)
    phy_para.pad_pu_en = !(LINEIN_DETECT_LEVEL);
    phy_para.pad_pd_en = LINEIN_DETECT_LEVEL;
    phy_para.smit_en = 0;
    phy_para.pad_driver = 1;
    set_pad_function(LINEIN_DETECT_PIN, LINEIN_DETECT_FUNC, &phy_para, 0);
#endif
}

bool check_device_in(check_device_state cbk, uint8 debounce_time, uint8 debounce_in_time)
{
    bool ret_val = FALSE;
    uint8 cur_flag = 0;
    uint8 debounce = 0;
    uint8 debounce_in = 0;

    while (1)
    {
        cur_flag = cbk();

        if (cur_flag == DEV_DET_MAYBE)
        {
            debounce++;
            if (debounce < debounce_time)
            {
                sys_os_time_dly(1);
                continue;
            }
            else
            {
                ret_val = TRUE;
                break;
            }
        }
        else if (cur_flag == DEV_DET_IN)
        {
            debounce_in++;
            if (debounce_in < debounce_in_time)
            {
                sys_os_time_dly(1);
                continue;
            }
            else
            {
                ret_val = TRUE;
                break;
            }
        }
        else
        {
            break;
        }
    }

    return ret_val;
}

bool check_device_out(check_device_state cbk, uint8 debounce_time)
{
    bool ret_val = FALSE;
    uint8 cur_flag = 0;
    uint8 debounce = 0;

    while (1)
    {
        cur_flag = cbk();

        if (cur_flag == 0)
        {
            debounce++;
            if (debounce < debounce_time)
            {
                sys_os_time_dly(1);
                continue;
            }
            else
            {
                ret_val = TRUE;
                break;
            }
        }
        else
        {
            break;
        }
    }

    return ret_val;
}

