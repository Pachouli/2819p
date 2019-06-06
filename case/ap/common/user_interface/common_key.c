/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：按键消息钩子函数，支持按键音，支持按键过滤处理，清除省电关机等系统计时器。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! 过滤掉按键长按动作，要过滤的按键 */
static uint8 this_filter_key_value;
/*! 过滤掉按键长按后续动作，即 KEY_TYPE_HOLD & KEY_TYPE_SHORT_UP */
static uint8 filter_key;
/*! 过滤掉按键抬起动作，即 KEY_TYPE_SHORT_UP */
static uint8 filter_key_up;
/*! 当前按键类型，区分是否TWS副箱按键 */
static input_msg_type_e this_input_msg_type;
/*! 超长时间按键计时 */
static uint32 long_key_time_start;
/*! 过滤一小段时间内的任意按键: 过滤时间长度，0表示不过滤 */
static uint32 filter_time_length;
/*! 过滤一小段时间内的任意按键: 过滤起始时间点 */
static uint32 filter_time_start;

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    开始播放按键音。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/
void com_start_key_tone_inner(void)
{
    if ((uint8) com_get_config_default_inner(SETTING_SOUND_ENABLE_KEYTONE) == 1)
    {
#if (SUPPORT_SOFT_KEYTONE == 1)
        keytone_play_inner (KT_ID_KEYTONE);
#else
        keytone_play_inner (KT_MODE_KEYTONE);
#endif
    }
}

//专给副箱设置当前按键用
bool com_gui_msg_hook_slave(input_gui_msg_t *input_msg)
{
    key_value_e key_val = input_msg->data.kmsg.val;
    key_type_e key_type = input_msg->data.kmsg.type;

    //更新当前按键值
    if ((key_type == KEY_TYPE_SHORT_UP) //如果按键已经抬起，那么设置过滤按键无效
    || (key_type == KEY_TYPE_LONG_UP))
    {
        this_filter_key_value = KEY_NULL;
    }
    else
    {
        this_filter_key_value = key_val;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    按键发生特殊处理，作为 gui 消息处理的回调函数。
 * \param[in]    input_msg 获取到的 gui消息指针
 * \param[out]   input_msg 可能会对input_msg进行修改，比如将val修改为KEY_HOLD虚拟按键
 * \return       bool
 * \retval           TRUE 获取到的按键有效，没有被过滤掉。
 * \retval           FALSE 获取到的按键无效，被过滤掉了。
 * \ingroup      common_func
 * \note
 *******************************************************************************/
bool com_gui_msg_hook(input_gui_msg_t *input_msg)
{
    key_value_e key_val = input_msg->data.kmsg.val;
    key_type_e key_type = input_msg->data.kmsg.type;

    //过滤掉一段时间内的任意按键，这样可以更好支持组合按键（组合按键按下后抬起时因为无法保证同时抬起，会有一个尾巴需要处理掉）
    if (filter_time_length != 0)
    {
        if ((sys_get_ab_timer() - filter_time_start) < filter_time_length)
        {
            return FALSE;
        }
        else
        {
            filter_time_length = 0;
        }
    }

    //1.长按按键后过滤掉后续按键（检测到按键抬起，或者其他按键按下，或该按键的新周期，就算过滤完成）
    if (key_val == filter_key)
    {
        //过滤还没完成就收到DOWN消息，表明SHORTUP消息在切换应用时被清除掉了，
        //那么DOWN消息就属于新的按键周期的消息，不应该被过滤
        if (key_type == KEY_TYPE_DOWN) //收到该按键的新周期，过滤完成
        {
            filter_key = KEY_NULL;
        }
        else
        {
            //过滤LONG和HOLD按键
            if ((key_type != KEY_TYPE_SHORT_UP)
                    && (key_type != KEY_TYPE_LONG_UP))
            {
                return FALSE;
            }
            else //过滤完成，但还是不响应SHORT_UP按键
            {
                this_filter_key_value = KEY_NULL;
                filter_key = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //收到新按键，过滤完成
    {
        filter_key = KEY_NULL;
    }

    //2.长按按键后过滤掉后续抬起按键（检测到按键抬起，或者其他按键按下，或该按键的新周期，就算过滤完成）
    if (key_val == filter_key_up)
    {
        //过滤还没完成就收到DOWN消息，表明SHORTUP消息在切换应用时被清除掉了，
        //那么DOWN消息就属于新的按键周期的消息，不应该被过滤
        if (key_type == KEY_TYPE_DOWN) //收到该按键的新周期，过滤完成
        {
            filter_key_up = KEY_NULL;
        }
        else
        {
            //仅过滤抬起按键
            if ((key_type == KEY_TYPE_SHORT_UP)
                    || (key_type == KEY_TYPE_LONG_UP))
            {
                this_filter_key_value = KEY_NULL;
                filter_key_up = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //收到新按键，过滤完成
    {
        filter_key_up = KEY_NULL;
    }

    //新按键按下重新计时
    if (key_type == KEY_TYPE_DOWN)
    {
        long_key_time_start = sys_get_ab_timer();
    }

    //更新当前按键值
    if ((key_type == KEY_TYPE_SHORT_UP) //如果按键已经抬起，那么设置过滤按键无效
    || (key_type == KEY_TYPE_LONG_UP))
    {
        this_filter_key_value = KEY_NULL;
    }
    else
    {
        this_filter_key_value = key_val;
    }

    //3.按键音处理，仅在按键按下时发出按键音
    if ((key_type == KEY_TYPE_DOWN) && (input_msg->type == INPUT_MSG_KEY))
    {
        //没音频输出，非静音状态，且按键音使能，才发出按键音
        com_start_key_tone_inner();
    }

    //4.省电关机计时 清0
    sys_counter_reset();

    this_input_msg_type = input_msg->type;

    //至此收到一个按键消息，进行双击按键处理
    if (g_key_deal_infor.double_click_enable == TRUE)
    {
        switch(g_key_deal_infor.dc_sta)
        {
        case DC_KEY_STATUS_NULL:
            if (g_key_deal_infor.dc_key_1 != KEY_NULL)
            {
                if ((key_val == g_key_deal_infor.dc_key_1) && (key_type == KEY_TYPE_DOWN))
                {
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_FIRST_DOWN;
                    g_key_deal_infor.dc_last_time = sys_get_ab_timer();
                    g_key_deal_infor.dc_key_cur = key_val;
                }
            }
            if (g_key_deal_infor.dc_key_2 != KEY_NULL)
            {
                if ((key_val == g_key_deal_infor.dc_key_2) && (key_type == KEY_TYPE_DOWN))
                {
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_FIRST_DOWN;
                    g_key_deal_infor.dc_last_time = sys_get_ab_timer();
                    g_key_deal_infor.dc_key_cur = key_val;
                }
            }
            break;

        case DC_KEY_STATUS_FIRST_DOWN:
            if ((key_val == g_key_deal_infor.dc_key_cur) && (key_type == KEY_TYPE_SHORT_UP))
            {
                g_key_deal_infor.dc_sta = DC_KEY_STATUS_FIRST_UP;
                g_key_deal_infor.dc_last_time = sys_get_ab_timer();
            }
            else
            {
                if (key_val != g_key_deal_infor.dc_key_cur)
                {
                    libc_printf_warning("dc unexpect key val!!\n");
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
                    return FALSE;
                }
                else
                {
                    g_key_deal_infor.dc_fail_reason = DC_FAIL_FIRST_LONG;
                }
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        case DC_KEY_STATUS_FIRST_UP:
            if ((key_val == g_key_deal_infor.dc_key_cur) && (key_type == KEY_TYPE_DOWN))
            {
                g_key_deal_infor.dc_sta = DC_KEY_STATUS_SECOND_DOWN;
                g_key_deal_infor.dc_last_time = sys_get_ab_timer();
            }
            else
            {
                if (key_val != g_key_deal_infor.dc_key_cur)
                {
                    g_key_deal_infor.dc_fail_reason = DC_FAIL_SECOND_OTHER_KEY;
                    g_key_deal_infor.other_key = key_val;
                }
                else
                {
                    libc_printf_warning("dc unexpect key type!!\n");
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
                    return FALSE;
                }
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        case DC_KEY_STATUS_SECOND_DOWN:
            if ((key_val == g_key_deal_infor.dc_key_cur) && (key_type == KEY_TYPE_SHORT_UP))
            {
                //双击按键成功
                input_msg->data.kmsg.type = KEY_TYPE_DOUBLE_CLICK; //更改按键类型为 KEY_TYPE_DOUBLE_CLICK
                g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
            }
            else
            {
                if (key_val != g_key_deal_infor.dc_key_cur)
                {
                    libc_printf_warning("unexpect key val!!\n");
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
                    return FALSE;
                }
                else
                {
                    g_key_deal_infor.dc_fail_reason = DC_FAIL_SECOND_LONG;
                }
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        default:
            break;
        }

        if (g_key_deal_infor.dc_sta != DC_KEY_STATUS_NULL)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    过滤掉当前按键后续动作，即相同值按键消息。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 * \li  该接口用于过滤当前按键值的后续动作，如需过滤掉任意指定按键，需直接调用
 *      change_filter_key 设置。
 * \li  在检测到当前按键抬起消息或者其他按键消息后，由 com_gui_msg_hook 自动调用解除。
 * \li  如果此时没有按键按下，该接口调用不会产生真正的按键过滤效果。
 *******************************************************************************/
void com_filter_key_hold_inner(void)
{
    filter_key = this_filter_key_value;
}

void com_filter_key_up_inner(void)
{
    filter_key_up = this_filter_key_value;
}

void com_filter_key_period_of_time_inner(uint16 time_length)
{
    filter_time_start = sys_get_ab_timer();
    filter_time_length = time_length;
}

//按键按住时间长度，从收到DOWN消息开始计算
uint32 com_get_long_key_time_inner(void)
{
    return (sys_get_ab_timer() - long_key_time_start);
}

//获取当前正在处理的按键类型
input_msg_type_e com_get_input_msg_type_inner(void)
{
    return this_input_msg_type;
}

/*! \endcond */
