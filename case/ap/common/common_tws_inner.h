/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：用户交互模块内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_TWS_INNER_H_
#define _COMMON_TWS_INNER_H_

#include "common_ui.h"
#include "applib_inner.h"
#include "common_sys_inner.h"
#include "common_bt_inner.h"
#include "common_aset_inner.h"
#include "common_var.h"
#include "common_func_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

extern dev_role_e get_tws_role_inner(void);
extern void change_tws_role_inner(dev_role_e tws_role);

extern bool com_tws_sync_cmd_loop_deal_m(void);
extern bool com_tws_slave_send_event_loop_deal(void);
extern bool com_tws_sync_cmd_loop_deal_s(void);

extern buffer_rw_t *g_p_tws_ui_m2s_pipe;
extern buffer_rw_t *g_p_tws_ui_s2m_pipe;

//TWS 从箱上传给主箱的用户交互信息
typedef struct
{
    uint8  bat_val_update : 1;
    uint8  has_key : 1;
    uint8  btplay_update : 2; //0表示无须更新，1表示等待发送，2表示等待回复
    uint8  is_btplay : 1; //当前是否在BTPLAY模式
    uint8  bat_val; //当前从箱电池电量
    uint32 key_data; //当前从箱获取到的逻辑按键
    uint32 btplay_timer; //副箱上报是否在BTPLAY状态的定时器
} tws_slave_ui_info_t;

extern tws_slave_ui_info_t g_tws_slave_ui_info;

extern dev_role_e get_tws_role_inner(void);
extern void change_tws_role_inner(dev_role_e tws_role);
extern dev_spk_pos_e get_tws_spk_pos_inner(void);
extern void change_tws_spk_pos_inner(dev_spk_pos_e tws_spk);
extern bool write_tws_cmd_to_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *w_buf, uint16 w_len);
extern bool read_tws_cmd_from_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *r_buf, uint16 r_len);
extern bool flush_tws_cmd_buffer_rw(buffer_rw_t *p_buf_rw);

extern bool com_tws_pair_init(dev_role_e dev_role);
extern bool com_tws_pair_exit(dev_role_e last_dev_role);
extern bool com_tws_mode_init(dev_role_e dev_role);
extern bool com_tws_mode_exit(dev_role_e last_dev_role);

extern bool com_tws_s2m_update_bat(uint8 slave_bat_val);
extern bool com_tws_s2m_update_key(uint32 key_data);
extern bool com_tws_s2m_tts_play_req_inner(uint8 tts_id);
extern bool com_tws_s2m_update_btplay_inner(bool enter);
extern bool com_tws_slave_update_btplay_inner(bool is_btplay);
extern bool com_tws_slave_send_event_loop_deal(void);
extern bool com_tws_sync_cmd_event_deal_m(tws_sync_cmd_t *p_tws_cmd);
extern bool com_tws_sync_cmd_loop_deal_m(void);

extern bool com_tws_s2m_cmd_sync_reply(tws_sync_reply_e ack, void *para, uint8 para_len);

extern bool com_tws_m2s_set_vol_val_cmd(uint8 vol);
extern bool com_tws_m2s_set_vol_limit_cmd(int16 vol_limit);
extern bool com_tws_m2s_switch_pos_cmd_inner(dev_spk_pos_e pos);
extern bool com_tws_m2s_switch_tws_mode_cmd_inner(tws_mode_e tws_mode);
extern bool com_tws_m2s_ack_update_btplay_inner(void);
#define TWS_POWEROFF_SYS  0 //系统软关机
#define TWS_POWEROFF_KEY  1 //按键软关机
#define TWS_POWEROFF_S2   2 //进S2
extern bool com_tws_m2s_poweroff_cmd(uint8 poweroff_mode);
extern bool com_tws_m2s_switch_next_app_cmd(void);
extern bool com_tws_m2s_tts_play_req(uint8 tts_id);
extern bool com_tws_m2s_tts_status_req(tws_slave_tts_state_e *tts_sta);
extern bool com_tws_sync_cmd_loop_deal_s(void);

extern void com_tws_tts_play_slave(uint8 tts_id, uint16 mode);

extern void com_tts_state_play_tws_slave_inner(uint8 tts_id, uint16 mode);

#endif

#endif
