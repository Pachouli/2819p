/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TWS �������ܽӿڣ�������д���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

buffer_rw_t *g_p_tws_ui_m2s_pipe;
buffer_rw_t *g_p_tws_ui_s2m_pipe;

//��bufferд������
bool write_tws_cmd_to_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *w_buf, uint16 w_len)
{
    bool ret = buffer_rw_write(p_buf_rw, w_buf, w_len);

    if (ret == FALSE)
    {
        libc_printf_warning("w tws cmd full!\n");
    }

    return ret;
}

//��buffer��������
bool read_tws_cmd_from_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *r_buf, uint16 r_len)
{
    bool ret = buffer_rw_read(p_buf_rw, r_buf, r_len);

    if (ret == FALSE)
    {
        //libc_printf("r tws cmd empty!\n");
    }

    return ret;
}

//��ն���
bool flush_tws_cmd_buffer_rw(buffer_rw_t *p_buf_rw)
{
    return buffer_rw_flush(p_buf_rw);
}

//��ȡ�����ɫ
dev_role_e get_tws_role_inner(void)
{
    return g_app_info_state.tws_role;
}

//�޸Ķ����ɫ
void change_tws_role_inner(dev_role_e tws_role)
{
    g_app_info_state.tws_role = tws_role;
}

//��ȡ����λ��
dev_spk_pos_e get_tws_spk_pos_inner(void)
{
    return g_app_info_state.tws_spk;
}

//�޸Ķ����ɫ
void change_tws_spk_pos_inner(dev_spk_pos_e tws_spk)
{
    g_app_info_state.tws_spk = tws_spk;
    g_com_env_var.spk_pos = tws_spk;
}

#endif
