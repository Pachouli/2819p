/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：回转型编码器驱动实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "key_inner.h"

#if (SUPPORT_ROTARY_ENCODER == 1)

#define ROTARY_ENCODER_OVERTIME  2000 //单位1us

rotary_encoder_info_t g_rotary_encode_info;

void terminalA_SIRQ_handle(uint8 triggle_type)
{
    sirq_set_para_t sirq_set;
    sirq_set.handle = terminalA_SIRQ_handle;
    if (g_rotary_encode_info.A_sirq_triggle == SIRQ_TRIGGLE_LOW_LEVEL)
    {
        libc_printf("AL\n");
        if ((g_rotary_encode_info.start_check_flag == TRUE)
                && (sys_get_ab_timer_us() - g_rotary_encode_info.A_jitter_timer > ROTARY_ENCODER_OVERTIME))
        {
            g_rotary_encode_info.A_low_flag = 1;
        }
        g_rotary_encode_info.A_high_flag = FALSE;
        sirq_set.triggle = SIRQ_TRIGGLE_HIGH_LEVEL;
        g_rotary_encode_info.A_sirq_triggle = SIRQ_TRIGGLE_HIGH_LEVEL;
    }
    else
    {
        libc_printf("AH\n");
        g_rotary_encode_info.A_high_flag = TRUE;
        if (g_rotary_encode_info.B_high_flag == TRUE)
        {
            g_rotary_encode_info.start_check_flag = TRUE;
        }
        sirq_set.triggle = SIRQ_TRIGGLE_LOW_LEVEL;
        g_rotary_encode_info.A_sirq_triggle = SIRQ_TRIGGLE_LOW_LEVEL;
    }
    sirq_set.default_edge = 0;
    sirq_set.realtime_mode = 0;
    g_rotary_encode_info.A_jitter_timer = sys_get_ab_timer_us();

    hal_set_sirq((RC_TERMINAL_A_PIN-PAD_GPIO0),&sirq_set);

    if ((g_rotary_encode_info.B_low_flag != 0) && (g_rotary_encode_info.A_low_flag != 0))
    {
        g_rotary_encode_info.A_low_flag = 0;
        g_rotary_encode_info.B_low_flag = 0;
        g_rotary_encode_info.start_check_flag = FALSE;
        g_rotary_encode_info.EnStatus = EnKEY_BACKWARD;
        libc_printf("RSUB\n");
    }
}

void terminalB_SIRQ_handle(uint8 triggle_type)
{
    sirq_set_para_t sirq_set;
    sirq_set.handle = terminalB_SIRQ_handle;
    if (g_rotary_encode_info.B_sirq_triggle == SIRQ_TRIGGLE_LOW_LEVEL)
    {
        libc_printf("BL\n");
        if ((g_rotary_encode_info.start_check_flag == TRUE)
                && (sys_get_ab_timer_us() - g_rotary_encode_info.B_jitter_timer > ROTARY_ENCODER_OVERTIME))
        {
            g_rotary_encode_info.B_low_flag = 1;
        }
        g_rotary_encode_info.B_high_flag = FALSE;
        sirq_set.triggle = SIRQ_TRIGGLE_HIGH_LEVEL;
        g_rotary_encode_info.B_sirq_triggle = SIRQ_TRIGGLE_HIGH_LEVEL;
    }
    else
    {
        libc_printf("BH\n");
        g_rotary_encode_info.B_high_flag = TRUE;
        if (g_rotary_encode_info.A_high_flag == TRUE)
        {
            g_rotary_encode_info.start_check_flag = TRUE;
        }
        sirq_set.triggle = SIRQ_TRIGGLE_LOW_LEVEL;
        g_rotary_encode_info.B_sirq_triggle = SIRQ_TRIGGLE_LOW_LEVEL;
    }
    sirq_set.default_edge = 0;
    sirq_set.realtime_mode = 0;
    g_rotary_encode_info.B_jitter_timer = sys_get_ab_timer_us();

    hal_set_sirq((RC_TERMINAL_B_PIN-PAD_GPIO0),&sirq_set);

    if ((g_rotary_encode_info.A_low_flag != 0) && (g_rotary_encode_info.B_low_flag != 0))
    {
        g_rotary_encode_info.A_low_flag = 0;
        g_rotary_encode_info.B_low_flag = 0;
        g_rotary_encode_info.start_check_flag = FALSE;
        g_rotary_encode_info.EnStatus = EnKEY_FORWARD;
        libc_printf("RADD\n");
    }
}

void key_inner_rotary_encoder_init(void)
{
    sirq_set_para_t sirq_set;
    pad_phy_para_t phy_para;

    //初始化全局变量
    g_rotary_encode_info.A_high_flag = TRUE;
    g_rotary_encode_info.B_high_flag = TRUE;
    g_rotary_encode_info.start_check_flag = TRUE;
    g_rotary_encode_info.EnStatus = EnKEY_STOP;

    phy_para.pad_pu_en = 1;
    phy_para.pad_pd_en = 0;
    phy_para.pad_driver = 1;

    set_pad_function(RC_TERMINAL_A_PIN, PAD_FUNC_GPIOIN, &phy_para, 0);
    set_pad_function(RC_TERMINAL_B_PIN, PAD_FUNC_GPIOIN, &phy_para, 0);

    sirq_set.handle = terminalA_SIRQ_handle;
    sirq_set.triggle = SIRQ_TRIGGLE_LOW_LEVEL;
    sirq_set.default_edge = 0;
    sirq_set.realtime_mode = 0;
    g_rotary_encode_info.A_sirq_triggle = SIRQ_TRIGGLE_LOW_LEVEL;
    hal_set_sirq((RC_TERMINAL_A_PIN-PAD_GPIO0), &sirq_set);

    sirq_set.handle = terminalB_SIRQ_handle;
    sirq_set.triggle = SIRQ_TRIGGLE_LOW_LEVEL;
    sirq_set.default_edge = 0;
    sirq_set.realtime_mode = 0;
    g_rotary_encode_info.B_sirq_triggle = SIRQ_TRIGGLE_LOW_LEVEL;
    hal_set_sirq((RC_TERMINAL_B_PIN-PAD_GPIO0), &sirq_set);
}

#endif
