/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌引擎应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _BTPLAY_ENGINE_TWS_H
#define _BTPLAY_ENGINE_TWS_H

#include  "common_engine_app_def.h"
#include  "bluetooth_common.h"
#include  "btplay_common.h"
#include  "mmm_bp.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

/* TWS ENGINE SYNC CMD */
#define TWS_SYNC_S2M_EG_WAIT_CLEAR_REQ   0x01 //副箱请求主箱清理现场，以便对箱可以开始播放
#define TWS_SYNC_S2M_EG_CLEAR_CFM        0x02 //收到主箱已清理现场的回应后，进行确认

#define TWS_SYNC_M2S_EG_CLEAR_RSP        0x81 //收到副箱的清理现场的请求，清理完现场后，进行回应

#define TWS_CLEAR_OVER_TIME  1000 //清理流程1S超时，认为通信失败

typedef enum
{
    M_CLEAR_NULL     = 0, //已清理：此时允许unfilter，允许TWS播放，如果副箱在此状态请求清理，则进入“收到请求”，退出中间件，并置为filter
    M_CLEAR_RECV_REQ = 1, //收到请求：回应副箱请求，之后进入“已回应”，保持filter
    M_CLEAR_SEND_RSP = 2, //已回应：等待副箱确认“已清理”，之后进入“已收到确认”，保持filter
    M_CLEAR_RECV_CFM = 3, //已收到确认：unfilter，自动跳到“已清理”
} master_clear_state_e;

typedef enum
{
    S_CLEAR_NULL     = 0, //已清理：此时允许unfilter，允许TWS播放。从中间件退出时需要走清理流程，先发送清理请求，进入“已请求”，并置为filter
    S_CLEAR_SEND_REQ = 1, //已请求：等待主箱回应，之后进入“已收到回应”，保持filter
    S_CLEAR_RECV_RSP = 2, //已收到回应：副箱清理，unfilter，并发送已经确认，进入“已发送确认”
    S_CLEAR_SEND_CFM = 3, //已发送确认：保持unfilter，自动跳到“已清理”
} slave_clear_state_e;

#define REPORT_EXCPT_TIME         (10*1000) //条件1：从第一次报错到认为异常的最短时间
#define REPORT_EXCPT_ERROR_COUNT  (10)      //条件2：连续多次出错后才认为异常
#define CLEAR_ERROR_TIME          (30*1000) //持续一段时间正常后就清除错误

extern uint32 tws_play_first_error_timer;
extern uint32 tws_play_last_error_timer;
extern uint8 tws_play_error_count;
extern int8 phone_rssi_value;
extern int8 tws_rssi_value;

extern bool g_filter_by_tws_clear_flag;
extern uint8 g_filter_by_tws_forbidden_nest;
extern dev_role_e last_tws_role;

extern uint32 g_tws_start_clear_timer;
extern uint8  g_tws_clear_send_rpt;
extern bool   g_tws_s_clear_send_req_first;
extern master_clear_state_e g_tws_m_clear_sta;
extern slave_clear_state_e  g_tws_s_clear_sta;

extern bool g_tws_master_already_clear_flag_s;
extern bool g_tws_slave_already_clear_flag_m;
extern uint8 g_tws_slave_clear_rsp_count;

extern buffer_rw_t *p_tws_engine_m2s_pipe;
extern buffer_rw_t *p_tws_engine_s2m_pipe;
extern buffer_rw_t *p_tws_mmm_m2s_pipe;
extern buffer_rw_t *p_tws_mmm_s2m_pipe;

extern bool   g_tws_unlink_delay_unfilter_flag;
extern uint32 g_tws_unlink_delay_unfilter_time;

extern bool g_tws_keep_filter_flag;

extern bool flush_tws_cmd_buffer_rw(buffer_rw_t *p_buf_rw);
extern int btplay_eg_tws_flush_pipe(void);
extern int btplay_eg_tws_play_setting(void);

extern bool tws_send_sync_cmd(uint8 dev_role, uint8 cmd_id);
extern bool tws_recv_sync_cmd(uint8 dev_role, uint8 *p_cmd_id);
extern app_result_e btplay_engine_tws_pause(private_msg_t *msg_ptr);
extern app_result_e btplay_engine_tws_resume(private_msg_t *msg_ptr);
extern app_result_e btplay_engine_tws_conform_mode(private_msg_t *msg_ptr);

extern int32 btplay_engine_tws_clear_start(void);
extern bool btplay_engine_tws_role_change(void);
extern int32 btplay_engine_tws_clear_deal_m(void);
extern int32 btplay_engine_tws_clear_deal_s(void);

extern bool btplay_engine_is_distance_near(void);

#endif

#endif

