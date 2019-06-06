/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TWS 公共功能接口，包括读写命令。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

buffer_rw_t *g_p_tws_ui_m2s_pipe;
buffer_rw_t *g_p_tws_ui_s2m_pipe;

//向buffer写入数据
bool write_tws_cmd_to_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *w_buf, uint16 w_len)
{
    bool ret = buffer_rw_write(p_buf_rw, w_buf, w_len);

    if (ret == FALSE)
    {
        libc_printf_warning("w tws cmd full!\n");
    }

    return ret;
}

//从buffer读出数据
bool read_tws_cmd_from_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *r_buf, uint16 r_len)
{
    bool ret = buffer_rw_read(p_buf_rw, r_buf, r_len);

    if (ret == FALSE)
    {
        //libc_printf("r tws cmd empty!\n");
    }

    return ret;
}

//清空队列
bool flush_tws_cmd_buffer_rw(buffer_rw_t *p_buf_rw)
{
    return buffer_rw_flush(p_buf_rw);
}

//获取对箱角色
dev_role_e get_tws_role_inner(void)
{
    return g_app_info_state.tws_role;
}

//修改对箱角色
void change_tws_role_inner(dev_role_e tws_role)
{
    g_app_info_state.tws_role = tws_role;
}

//获取对箱位置
dev_spk_pos_e get_tws_spk_pos_inner(void)
{
    return g_app_info_state.tws_spk;
}

//修改对箱角色
void change_tws_spk_pos_inner(dev_spk_pos_e tws_spk)
{
    g_app_info_state.tws_spk = tws_spk;
    g_com_env_var.spk_pos = tws_spk;
}

#endif
