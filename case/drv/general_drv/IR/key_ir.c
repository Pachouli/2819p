/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动按键扫描接口，按键消息发送接口；支持LRADC KEY、ONOFF KEY、UART SIM KEY。
 * 作者：cailizhen
 ********************************************************************************/

#include "ir_inner.h"

#if (SUPPORT_IR_KEY == 1)

irmap_para_t g_irmap_para;

typedef enum
{
    IR_STATUS_NO_KEY = 0x0,
    IR_STATUS_PRESS,
} ir_key_status_e;

ir_key_status_e g_ir_key_sta;
uint8 g_ir_old_key;
uint8 g_ir_idle_count;

void __section__(".rcode") key_ir_detect_key_cbk(uint8 key_data)
{
    uint8 temp_key_ir_value = KEY_NULL, i;

    for (i = 0; i < g_irmap_para.ir_key_num; i++)
    {
        if (key_data == g_irmap_para.ir_key_code[i])
        {
            temp_key_ir_value = g_irmap_para.ir_key_val[i];
            break;
        }
    }

    if (temp_key_ir_value == KEY_NULL)
    {
        libc_printf_warning("ir key code error:%d\n", key_data);
        g_ir_key_sta = IR_STATUS_NO_KEY;
        return;
    }

    libc_printf("ir key:%d -> %d\n", key_data, temp_key_ir_value);

    g_ir_idle_count = 0;
    if (g_ir_key_sta == IR_STATUS_NO_KEY)
    {
        PutSysMsg(FALSE, temp_key_ir_value, KEY_PHY_TYPE_IR);
        g_ir_old_key = temp_key_ir_value;
        g_ir_key_sta = IR_STATUS_PRESS;
    }
    else
    {
        if (temp_key_ir_value == g_ir_old_key)
        {//RC5/RC6 Repeat Key
            PutSysMsg(FALSE, g_ir_old_key, KEY_PHY_TYPE_IR);
        }
        else
        {
            //OLD IR KEY ShortUp
            PutSysMsg(TRUE, 0, KEY_PHY_TYPE_IR);

            //SWITCH TO NEW IR KEY
            PutSysMsg(FALSE, temp_key_ir_value, KEY_PHY_TYPE_IR);
            g_ir_old_key = temp_key_ir_value;
        }
    }
}

void __section__(".rcode") key_ir_repeat_key_cbk(void)
{
    libc_printf("ir repeat\n");

    g_ir_idle_count = 0;
    if (g_ir_key_sta == IR_STATUS_NO_KEY)
    {
        libc_printf_warning("ir repeat key error at IR_STATUS_NO_KEY\n");
        return;
    }
    else
    {
        PutSysMsg(FALSE, g_ir_old_key, KEY_PHY_TYPE_IR);
    }
}

void key_ir_init(irmap_para_t *p_irmap_para)
{
    pad_phy_para_t phy_para;

    libc_printf_info("IR install!\n");

    libc_memcpy(&g_irmap_para, p_irmap_para, sizeof(irmap_para_t));

    phy_para.pad_pu_en = 2;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
    phy_para.pad_driver = 1;
    set_pad_function(IR_RX_PIN, IR_RX_FUNC, &phy_para, NULL);

    irc_hardware_init(g_irmap_para.ir_protocol, g_irmap_para.ir_usercode, key_ir_detect_key_cbk, key_ir_repeat_key_cbk);
}

void key_ir_deinit(void)
{
    libc_printf_info("IR uninstall!\n");

    irc_hardware_exit();
}

void __section__(".rcode") key_ir_check_status(void)
{
    uint32 usercode;

    if (irc_correct_usercode(&usercode) < 0)
    {
        if (usercode != g_irmap_para.ir_usercode)
        {
            libc_printf_warning("ir usercode 0x%x is wrong, the right usercode is : 0x%x\n", g_irmap_para.ir_usercode, usercode);
            g_irmap_para.ir_usercode = usercode;
        }
    }

    if (g_ir_key_sta == IR_STATUS_PRESS)
    {
        g_ir_idle_count++;
        if (g_ir_idle_count >= 8)
        {
            //OLD IR KEY ShortUp
            PutSysMsg(TRUE, 0, KEY_PHY_TYPE_IR);
            g_ir_key_sta = IR_STATUS_NO_KEY;
            g_ir_idle_count = 0;
        }
    }

    return;
}

#endif
