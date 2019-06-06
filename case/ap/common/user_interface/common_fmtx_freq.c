/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：实现FM发射频点调节功能
 * 作者：wangli
 ********************************************************************************/

#include "common_ui_inner.h"

#if (SUPPORT_FM_TX == 1)

void com_fm_tx_freq_sub_inner(void);
void com_fm_tx_freq_add_inner(void);

void com_fm_tx_freq_sub_inner(void)
{
    if (g_com_env_var.fm_tx_freq > 875)
    {
        g_com_env_var.fm_tx_freq--;
    }
    else
    {
        g_com_env_var.fm_tx_freq = 1080;
    }

    libc_printf("fm tx freq:%d\n", g_com_env_var.fm_tx_freq);
    fm_tx_set_freq(g_com_env_var.fm_tx_freq);
    com_save_sys_comval_inner(); //实时保存FM发射频点值
}

void com_fm_tx_freq_add_inner(void)
{
    if (g_com_env_var.fm_tx_freq < 1080)
    {
        g_com_env_var.fm_tx_freq++;
    }
    else
    {
        g_com_env_var.fm_tx_freq = 875;
    }
    libc_printf("fm tx freq:%d\n", g_com_env_var.fm_tx_freq);
    fm_tx_set_freq(g_com_env_var.fm_tx_freq);
    com_save_sys_comval_inner(); //实时保存FM发射频点值
}

#endif
