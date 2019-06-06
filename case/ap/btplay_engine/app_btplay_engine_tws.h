/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�����Ӧ��ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _BTPLAY_ENGINE_TWS_H
#define _BTPLAY_ENGINE_TWS_H

#include  "common_engine_app_def.h"
#include  "bluetooth_common.h"
#include  "btplay_common.h"
#include  "mmm_bp.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

/* TWS ENGINE SYNC CMD */
#define TWS_SYNC_S2M_EG_WAIT_CLEAR_REQ   0x01 //�����������������ֳ����Ա������Կ�ʼ����
#define TWS_SYNC_S2M_EG_CLEAR_CFM        0x02 //�յ������������ֳ��Ļ�Ӧ�󣬽���ȷ��

#define TWS_SYNC_M2S_EG_CLEAR_RSP        0x81 //�յ�����������ֳ��������������ֳ��󣬽��л�Ӧ

#define TWS_CLEAR_OVER_TIME  1000 //��������1S��ʱ����Ϊͨ��ʧ��

typedef enum
{
    M_CLEAR_NULL     = 0, //��������ʱ����unfilter������TWS���ţ���������ڴ�״̬������������롰�յ����󡱣��˳��м��������Ϊfilter
    M_CLEAR_RECV_REQ = 1, //�յ����󣺻�Ӧ��������֮����롰�ѻ�Ӧ��������filter
    M_CLEAR_SEND_RSP = 2, //�ѻ�Ӧ���ȴ�����ȷ�ϡ���������֮����롰���յ�ȷ�ϡ�������filter
    M_CLEAR_RECV_CFM = 3, //���յ�ȷ�ϣ�unfilter���Զ�������������
} master_clear_state_e;

typedef enum
{
    S_CLEAR_NULL     = 0, //��������ʱ����unfilter������TWS���š����м���˳�ʱ��Ҫ���������̣��ȷ����������󣬽��롰�����󡱣�����Ϊfilter
    S_CLEAR_SEND_REQ = 1, //�����󣺵ȴ������Ӧ��֮����롰���յ���Ӧ��������filter
    S_CLEAR_RECV_RSP = 2, //���յ���Ӧ����������unfilter���������Ѿ�ȷ�ϣ����롰�ѷ���ȷ�ϡ�
    S_CLEAR_SEND_CFM = 3, //�ѷ���ȷ�ϣ�����unfilter���Զ�������������
} slave_clear_state_e;

#define REPORT_EXCPT_TIME         (10*1000) //����1���ӵ�һ�α�����Ϊ�쳣�����ʱ��
#define REPORT_EXCPT_ERROR_COUNT  (10)      //����2��������γ�������Ϊ�쳣
#define CLEAR_ERROR_TIME          (30*1000) //����һ��ʱ����������������

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

