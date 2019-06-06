/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�����Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_btplay_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

uint32 tws_play_first_error_timer;
uint32 tws_play_last_error_timer;
uint8 tws_play_error_count;
int8 phone_rssi_value;
int8 tws_rssi_value;

bool g_filter_by_tws_clear_flag;
uint8 g_filter_by_tws_forbidden_nest; //������TWS����Ƕ�ײ��

dev_role_e last_tws_role = NORMAL_DEV;

uint32 g_tws_start_clear_timer;
uint8  g_tws_clear_send_rpt;
bool   g_tws_s_clear_send_req_first;
master_clear_state_e g_tws_m_clear_sta;
slave_clear_state_e  g_tws_s_clear_sta;

//���±����Ǹ���ʹ�õ�
bool g_tws_master_already_clear_flag_s = FALSE; //�����Ƿ��Ѿ������ֳ����յ�����������ֳ���Ӧ����
bool g_tws_slave_already_clear_flag_m = FALSE; //�����Ƿ��Ѿ������ֳ����յ�����������ֳ����󼴿�
uint8 g_tws_slave_clear_rsp_count; //���ͻ�Ӧ�ļ������������20�λ�û���յ������ȷ�ϣ�����Ϊ�ǳ�ʱ��ǿ����Ϊ�Ѿ������ֳ�OK��

buffer_rw_t *p_tws_engine_m2s_pipe;
buffer_rw_t *p_tws_engine_s2m_pipe;

buffer_rw_t *p_tws_mmm_m2s_pipe;
buffer_rw_t *p_tws_mmm_s2m_pipe;

bool   g_tws_unlink_delay_unfilter_flag; //�նϿ���ӣ��Ե�200ms�ٽ�����ˣ��������ǰ̨����ʾTTS���Ե�TTS����������ŽӴ�����
uint32 g_tws_unlink_delay_unfilter_time;

bool g_tws_keep_filter_flag; //�����Ҫ�˳�����������������ֳ���ʼ�ձ��ֹ������ݣ����˳�Ӧ��ʱʹ��

bool flush_tws_cmd_buffer_rw(buffer_rw_t *p_buf_rw)
{
    uint32 temp_flag = sys_local_intc_save(0x01);

    p_buf_rw->length = 0;
    p_buf_rw->read_ptr = 0;
    p_buf_rw->write_ptr = 0;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//�ս���btplay engine���Լ��Ͽ����ʱ��Ҫ���btplay engine��btplay mmm�Ķ���
int btplay_eg_tws_flush_pipe(void)
{
    p_tws_engine_m2s_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_EG_M2S);
    if (p_tws_engine_m2s_pipe == NULL)
    {
        libc_printf_error("SHARE_MEM_ID_TWS_EG_M2S mount fail!\n");
        while (1);
    }
    flush_tws_cmd_buffer_rw(p_tws_engine_m2s_pipe);

    p_tws_engine_s2m_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_EG_S2M);
    if (p_tws_engine_s2m_pipe == NULL)
    {
        libc_printf_error("SHARE_MEM_ID_TWS_EG_S2M mount fail!\n");
        while (1);
    }
    flush_tws_cmd_buffer_rw(p_tws_engine_s2m_pipe);

    p_tws_mmm_m2s_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_MMM_M2S);
    if (p_tws_mmm_m2s_pipe == NULL)
    {
        libc_printf_error("SHARE_MEM_ID_TWS_MMM_M2S mount fail!\n");
        while (1);
    }
    flush_tws_cmd_buffer_rw(p_tws_mmm_m2s_pipe);

    p_tws_mmm_s2m_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_MMM_S2M);
    if (p_tws_mmm_s2m_pipe == NULL)
    {
        libc_printf_error("SHARE_MEM_ID_TWS_MMM_S2M mount fail!\n");
        while (1);
    }
    flush_tws_cmd_buffer_rw(p_tws_mmm_s2m_pipe);

    return 0;
}

int btplay_eg_tws_play_setting(void)
{
    music_bt_tws_setting_t bt_tws_sett;
    bt_clock_cbk_t temp_bt_clk_cbk;
    int32 ret;

    bt_tws_sett.dev_role = get_tws_role();
    bt_tws_sett.dev_spk = get_tws_spk_pos();

    com_btengine_tws_get_bt_cbk(&temp_bt_clk_cbk);

    bt_tws_sett.tws_get_bt_clk = temp_bt_clk_cbk.tws_get_bt_clk;
    bt_tws_sett.tws_set_bt_clk = temp_bt_clk_cbk.tws_set_bt_clk;
    bt_tws_sett.tws_reg_bt_clk_sync_cbk = temp_bt_clk_cbk.tws_reg_bt_clk_sync_cbk;
    bt_tws_sett.tws_has_avdtp_pkg_lost = temp_bt_clk_cbk.tws_has_avdtp_pkg_lost;

    ret = mmm_bp_cmd(bp_handle, MMM_BP_SET_TWS_MODE, (unsigned int) &bt_tws_sett);
    if (ret < 0)
    {
        libc_printf_warning("MMM_BP_SET_TWS_MODE fail!\n");
    }

    return ret;
}

#endif
