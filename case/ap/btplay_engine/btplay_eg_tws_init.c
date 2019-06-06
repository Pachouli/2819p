/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌引擎应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_btplay_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

uint32 tws_play_first_error_timer;
uint32 tws_play_last_error_timer;
uint8 tws_play_error_count;
int8 phone_rssi_value;
int8 tws_rssi_value;

bool g_filter_by_tws_clear_flag;
uint8 g_filter_by_tws_forbidden_nest; //不允许TWS播放嵌套层次

dev_role_e last_tws_role = NORMAL_DEV;

uint32 g_tws_start_clear_timer;
uint8  g_tws_clear_send_rpt;
bool   g_tws_s_clear_send_req_first;
master_clear_state_e g_tws_m_clear_sta;
slave_clear_state_e  g_tws_s_clear_sta;

//以下变量是副箱使用的
bool g_tws_master_already_clear_flag_s = FALSE; //主箱是否已经清理现场，收到主箱的清理现场回应即可
bool g_tws_slave_already_clear_flag_m = FALSE; //副箱是否已经清理现场，收到副箱的清理现场请求即可
uint8 g_tws_slave_clear_rsp_count; //发送回应的计数，如果发送20次还没有收到副箱的确认，就认为是超时，强制认为已经清理现场OK了

buffer_rw_t *p_tws_engine_m2s_pipe;
buffer_rw_t *p_tws_engine_s2m_pipe;

buffer_rw_t *p_tws_mmm_m2s_pipe;
buffer_rw_t *p_tws_mmm_s2m_pipe;

bool   g_tws_unlink_delay_unfilter_flag; //刚断开组队，稍等200ms再解除过滤，这样如果前台有提示TTS可以等TTS播报结束后才接触过滤
uint32 g_tws_unlink_delay_unfilter_time;

bool g_tws_keep_filter_flag; //如果是要退出蓝牙，则可以清理现场后始终保持过滤数据，在退出应用时使用

bool flush_tws_cmd_buffer_rw(buffer_rw_t *p_buf_rw)
{
    uint32 temp_flag = sys_local_intc_save(0x01);

    p_buf_rw->length = 0;
    p_buf_rw->read_ptr = 0;
    p_buf_rw->write_ptr = 0;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//刚进入btplay engine，以及断开组队时需要清除btplay engine和btplay mmm的队列
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
