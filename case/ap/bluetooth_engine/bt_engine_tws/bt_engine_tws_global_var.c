/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������btstack ȫ�ֱ������塣
 * ���ߣ�kehaitao
 ********************************************************************************/

#include  "bluetooth_engine.h"

//!!! ��Ҫע�⣬��Ŀ¼ֻ�ܴ��twsδ��ʼ�������ݣ����ܴ�ų�ʼ������

#ifdef ENABLE_TRUE_WIRELESS_STEREO

uint8 __section__(".bss.tws_avdtp_cache") g_tws_avdtp_cache[A2DP_M2S_AVDTP_CACHE_LEN];
uint8 __section__(".bss.tws_avdtp_packet") g_tws_avdtp_packet[A2DP_M2S_AVDTP_PACKET_LEN];

uint16 tws_tick_count = 0;
uint8 tws_paired_count = 0;

struct_send_list send_list;

//TWS������ͨ�Źܵ�
buffer_rw_t g_tws_mmm_m2s_pipe;
buffer_rw_t g_tws_mmm_s2m_pipe;
tws_sync_cmd_t g_tws_mmm_m2s_buffer[6];
tws_sync_cmd_t g_tws_mmm_m2s_play_sync_cmd;
tws_sync_cmd_t g_tws_mmm_s2m_buffer[8];

buffer_rw_t g_tws_engine_m2s_pipe;
buffer_rw_t g_tws_engine_s2m_pipe;
tws_sync_cmd_t g_tws_engine_m2s_buffer[4];
tws_sync_cmd_t g_tws_engine_s2m_buffer[4];

buffer_rw_t g_tws_ui_m2s_pipe;
buffer_rw_t g_tws_ui_s2m_pipe;
tws_sync_cmd_t g_tws_ui_m2s_buffer[4];
tws_sync_cmd_t g_tws_ui_s2m_buffer[4];
int32 send_data_10ms_timer = -1;
int32 repaired_timer = -1;
tws_status_info_t tws_status_info;

uint8 already_start_connect_phone_flag;

uint8 snk_to_source_judge_flag;
uint8 random_value;
uint8 tws_quit_time;
uint8 tws_wait_quitflag;
uint8 phone_connect_flag;
uint8 tws_connect_flag;
uint8 tws_sync_group_all_count;
uint16 total_acl_pktnums;
uint16 tws_max_acl_pktnums;
uint8 slave_connect_phone_flag;
uint8 have_wait_for_manupair_flag = 0;   //��־λĿǰ����hfp���յ���������󣬻�ܾ����ӡ�������hfp�رպ������������
uint8 senddata_cnt;
bt_dev_t * wrong_phone_dev = NULL;

uint32 g_tws_src_last_cmd_send_timer;
bool g_tws_src_last_has_cmd_to_send;

#endif

