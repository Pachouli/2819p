/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ʵ��FM����Ƶ����ڹ���
 * ���ߣ�wangli
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
    com_save_sys_comval_inner(); //ʵʱ����FM����Ƶ��ֵ
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
    com_save_sys_comval_inner(); //ʵʱ����FM����Ƶ��ֵ
}

#endif
