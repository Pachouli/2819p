/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASET音效调试模块，与PC ASET工具通信。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_aset_inner.h"
#include "common_func_inner.h"

#if (SUPPORT_ASET_TEST == 1)

/*!
 *  \brief
 *  g_aset_var ASET音效调试相关全局变量
 */
aset_var_t g_aset_var;

bool aset_test_global_init(void)
{
    dae_config_t *p_dae_cfg = g_p_com_dae_cfg;

    com_update_dae_config(p_dae_cfg);

    libc_memset(&g_aset_var, 0x00, sizeof(aset_var_t));

    g_aset_var.run_state.err_cnt = 0;
    g_aset_var.run_state.run_state = ASET_TOOLS_NOT_WORK;

    g_aset_var.cmd_buffer = sys_malloc(256);
    if (g_aset_var.cmd_buffer == NULL)
    {
        libc_printf("cmd_buffer\n");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    g_aset_var.main_gain_value = 0;
    g_aset_var.main_switch_flag = 1;

    //ASET DAE CFG INIT
    g_aset_var.p_dae_cfg = p_dae_cfg;
    g_aset_var.p_dae_cfg->bypass = 0;
    g_aset_var.p_dae_cfg->precut = g_aset_var.main_gain_value;

    stub_ioctrl_set(SET_TIMEOUT, 500, 0); //aset工具在调音效时，在某些时间会概率性超时断开连接，所以加大点超时时间

    if (g_aset_var.run_state.run_state == ASET_TOOLS_NOT_WORK)
    {
        aset_write_case_info();
        g_aset_var.run_state.run_state = ASET_TOOLS_WORK;
    }

    return TRUE;
}

void aset_test_global_exit(void)
{
    com_update_dae_config(g_aset_var.p_dae_cfg);
    sys_free(&(g_aset_var.cmd_buffer));
    g_aset_var.cmd_buffer = NULL;
}

void aset_loop_deal(void)
{
    int ret_val;
    aset_status_t aset_status;
    aset_status_t *p_aset_status = (aset_status_t *)(g_aset_var.cmd_buffer + 8);

    ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_STATUS, g_aset_var.cmd_buffer, sizeof(aset_status));

    if (ret_val == 0)
    {
        libc_memcpy(&aset_status, p_aset_status, sizeof(aset_status));

        if (g_aset_var.run_state.run_state == ASET_TOOLS_WORK)
        {
            aset_cmd_deal(&aset_status);
            g_aset_var.run_state.err_cnt = 0;
        }
        else if(g_aset_var.run_state.run_state == ASET_TOOLS_NOT_WORK)
        {
            aset_write_case_info();
            g_aset_var.run_state.run_state = ASET_TOOLS_WORK;
        }
        else
        {
            ; //nothing for QAC
        }
    }
    else
    {
        if (g_aset_var.run_state.err_cnt > 10)
        {
            if (g_aset_var.run_state.run_state == ASET_TOOLS_WORK)
            {
                g_aset_var.run_state.run_state = ASET_TOOLS_NOT_WORK;
            }
        }
        else
        {
            g_aset_var.run_state.err_cnt++;
        }
    }
}

void aset_test_loop_deal_inner(void)
{
    uint32 tmp_ab_timer = sys_get_ab_timer();

    //200ms执行一次任务
    if ((tmp_ab_timer - g_aset_var.last_deal_timer) >= 200)
    {
        g_aset_var.last_deal_timer = tmp_ab_timer;

        aset_loop_deal();
    }
}

#endif
