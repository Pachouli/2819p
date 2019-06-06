/*******************************************************************************
 *                              us282a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       kehaitao   2014-12-15 14:15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     bt_stack_stereo_device.h
 * \brief    kehaitao
 * \version 1.0
 * \date  2015/12/15
 *******************************************************************************/
#ifndef _BT_STACK_TWS_H
#define _BT_STACK_TWS_H

#include "bluetooth_engine.h"
#include "bt_common.h"

#define TWS_PRE   "<TWS>"

#ifdef ENABLE_TRUE_WIRELESS_STEREO
#define TWS_QUIT_TIMEOUT               200
#define PAIR_PENDING_TIMEOUT	8000
#define TWS_SYNC_BASE  0xee
#define TWS_SYNC_START_COUNT  0xe1
#define TWS_SYNC_EXIT_MMM  0xe3
#define TWS_SYNC_PHONE_CONN   0xe4
//general sync cmd
//#define TWS_SYNC_VOLUME  0xe5
#define TWS_GENERAL_SYNC_CMD  0xe5

#define TWS_SYNC_TTS_START 0xe6
#define TWS_SYNC_TTS_STOP  0xe7
#define SUB_SYNC_QUIT_BL  0xe8
#define SUB_SYNC_BACK_BL  0xe9
#define TWS_SYNC_POWER_OFF 0xea
#define TWS_SYNC_SRC_VAL 0xeb
#define TWS_SYNC_SAMPLE_CNT 0xed
#define TWS_SYNC_PIPE_LENGTH 0xec
#define TWS_SYNC_PAUSE_CMD 0xa2
#define TWS_SYNC_PLAY_CMD 0xa3
#define TWS_SYNC_FORCE_ULINK_CMD 0xa4
#define TWS_SYNC_MSG 0xa5
#define TWS_SYNC_CMD 0xa6

#define MY_START_PLAY_TIME    (23)
#define TWS_PLAY_SYNC_TIME (10000)

#define SEND_LIST_MAX 32

#define TWS_LINK_LOSS_TIMEOUT   0x1900//625us为单位
#define INQUIRY_CNT                 6
#define INQUIRY_TIME_DELAY_SOURCE   4000    //ms为单位
#define INQUIRY_TIME_DELAY_SNK      8000

#define TWS_STREAM_PLAY_TIMEOUT     2000//2S内主箱没有向副箱发送数据则认为tws的stream已经停止了

enum
{
    TWS_STREAM_RESET,               //stream_sending_data_flag的初始值
    TWS_STREAM_SENDDING,            //tws音箱之间正在进行数据传输
};

enum
{
    TWS_STREAM_STATUS_PAUSE, TWS_STREAM_STATUS_PLAY,
};

enum
{
    TWS_SEARCH_NEW_DEV,            //只进行搜索组队
    TWS_PAIR_PAIRED,               //先进行一次组队回连，接下来进行搜索组队
};

//
enum
{
    TWS_DEV_RECONNECT_PHONE_OK = 1,
    TWS_DEV_RECONNECT_PHONE_QUIT_NO_SRC_BOX,
    TWS_DEV_RECONNECT_PHONE_QUIT_SINGLE_BOX,
    TWS_DEV_RECONNECT_PHONE_QUIT_RECONNECT_DEV_NULL,
};

//#define TWS_SLAVE_NO_DISC_PAIRED_BEFORE;
typedef struct
{
    buffer_rw_t avdtp_cache;
    uint16 data_len[SEND_LIST_MAX];
    uint8 count;
    uint8 send_ptr;
    uint8 recv_prt;
} struct_send_list;

typedef enum
{
    A2DP_STREAM_DONE, A2DP_STREAM_CREATED,
} A2DP_STREAM_STATU;

typedef struct
{
    A2DP_STREAM_STATU status;
    void *stream_hdl;
} A2dpStreamStatus_st;

typedef struct
{
    uint8 start_inquire :1; //开始全网扫描组对标志
    uint8 search_device_flag :1; //组对设备搜索中标志
    uint8 force_ulink_flag :1; //强制断开连接标志
    uint8 is_src_flag :1; //是否为信源(source);0-不是信源；1-是信源
    uint8 need_repaired :1; //是否需要重新组对标志
    uint8 have_paired_dev :1; //是否已经存在组对信息标志
    uint8 cur_reconnect_dev :2; //当前正在回连的设备是什么;0-无;1-非对箱设备2-对箱设备
    uint8 classOfDevice[3]; //组对设备类信息
    uint16 acl_handle; //组对设备的acl链路句柄
    uint16 src_stream_cid; //组对设备信源流ID
    uint16 sequence_number; //数据流序号
    uint8 start_inquire_cnt;
    int8 inquire_timer_id;
    int16 tws_acl_link_time;
    uint16 tws_acl_link_time_count;
    uint32 tws_acl_link_time_timer;
    uint8 paired_count;
    uint8 paried_mode; //pair_paireddev_first_flag;
    uint8 tws_paired_src_reconn_phone_enable; //TWS组队后主箱是否连接手机（line in tws场景下主箱回连手机会卡音）
    uint8 delay_reconn; //1：有需要设备需要延时回连 0：没有设备需要延时回连
    A2dpStreamStatus_st a2dp_stream_status;
} tws_status_info_t;

bt_dev_t *TWS_FindByStreamHdl(void *stream_hdl);
bt_dev_t *TWS_FindByAclHdl(uint16 acl_hdl);
bt_dev_t *TWS_FindPairedDevByAddr(uint8 *bdAddress);
void TWS_MatchAvailableDevice(struct HCI_EIR_DataNodeStru *node, struct GAP_RemoteDeviceInfoStru *in);
void TWS_RestartInquire(void);
void TWS_DevInquireComplete(void);
void TWS_CancelDeviceDiscovery(void);
void TWS_A2dpConnectComplete(struct A2DP_OpenedStru *in, bt_dev_t *dev);
void TWS_PerfectPairedInfo(uint8 *bd_addr, bt_dev_t *p_de);
bt_dev_t *TWS_ConfigPairedInfo(uint8 *bdAddress);
uint8 TWS_GetDevType(bt_dev_t *dev);
void TWS_SearchMatchingDev(void);
void TWS_CheckPairedState(void);
bool TWS_CheckDev(bt_dev_t *dev);
uint8 TWS_AutoPaired(void);
void TWS_CheckPairedStatus(void);
void TWS_GetClassOfDevice(void);
void TWS_SaveDevInfo(bt_dev_t *dev);
uint8 TWS_ManuPaired(void);
void TWS_CheckForceUlink(void);
void TWS_RepairedTimer(void);
void TWS_DisconnectAclLink(void);
void TWS_SetPairedStatus(uint8 cur_status);
uint8 TWS_GetPairedStatus(void);

bt_dev_t* tws_find_notws_dev(void);
void bt_Qos_setup(uint32 token_rate, uint32 latency);
bt_dev_t *TWS_FindA2dpSrcDev(void);

void TWS_A2dpStreamStart(uint8 dev_type);
void TWS_A2dpStreamSuspend(uint8 dev_type);
bt_dev_t* tws_find_dev_by_role(uint8 dev_role);

void TWS_A2dpRegSrcService(void);
uint8 bt_random(void);
void TWS_RmtSearchDev(void);

void TWS_MsgListInit(void);
uint8 TWS_DisconnectDeal(bt_dev_t *dev, struct GAP_ConnectionEventStru *in);

bool TWS_SourceSendData(uint8* send_data, uint32 in_len);
void TWS_Get_Cmd_Data_ToSend(uint8 packetnum);
bool TWS_GetCmdToSend(void);
void TWS_Send_10ms_Handle(void);
uint8 TWS_RxCmdDeal(uint8* pHciInfo, uint8 length);
void bt_read_inquiry_page_scan_activity(void);
void bt_write_inquiry_scan_activity(uint16 window, uint16 interval);
void bt_write_page_scan_activity(uint16 window, uint16 interval);
void bt_Write_Inquiry_Scan_Type(uint8 mode);
void bt_Write_Page_Scan_Type(uint8 mode);
extern void tws_buffer_manager_init(void);
extern void tws_buffer_manager_destroy(void);
extern uint8 tws_deal_tws_quit(void);

extern bool tws_get_bt_clk_inner(bt_clock_t *p_bt_clock);
extern bool tws_set_bt_clk_inner(uint32 inform_clk);
extern void tws_register_bt_clk_sync_cbk_inner(f_bt_clk_sync_cbk bt_clk_sync_cbk);
extern bool tws_has_avdtp_pkg_lost_inner(void);
extern tGAP_IndCbk_Result tws_connectionreply_deal(uint8 *bdAddress);
extern void tws_connect_deal(bt_dev_t *dev);
extern void TWS_PairedDev_Copyto_TWSDev(bt_dev_t *dev);
extern void tws_reconnect_lastphone_init(bt_dev_t *dev);
extern void tws_deal_acl_handle(uint16 aclpackets, uint8 mode);
extern uint32 tws_get_aclhandle_sendcount(void);

extern void TWS_global_var_init(void);
extern void clear_tws_info(void);
extern void TWS_GetPairedInfo_Form_Vram(void);
extern uint8 app_message_deal_tws(private_msg_t *pri_msg);
extern void TWS_SetA2DPSource(bt_dev_t *dev, uint8 mask);
extern uint8 app_set_tws_dev_reconnect_phone_policy(tws_dev_reconn_phone_policy_t *par);

extern uint16 tws_tick_count;
extern uint8 tws_paired_count;

extern struct_send_list send_list;

extern struct_buffer_rw send_buffer_rw;

extern uint8 g_tws_avdtp_cache[A2DP_M2S_AVDTP_CACHE_LEN];
extern uint8 g_tws_avdtp_packet[A2DP_M2S_AVDTP_PACKET_LEN];

#define TWS_SYNC_START_TIME  (800000)

#define TWS_SYNC_START_OFFSET_COUNT (TWS_SYNC_START_TIME/3125)

extern uint8 a2dp_snk_cnt; //记录注册snk端点个数

extern int32 check_twsinfo_timer_id;
extern uint8 send_start_cmd;
extern uint8 tws_dev_local_role;
extern uint8 tws_role_change_role;

extern buffer_rw_t g_tws_mmm_m2s_pipe;
extern buffer_rw_t g_tws_mmm_s2m_pipe;
extern tws_sync_cmd_t g_tws_mmm_m2s_buffer[6];
extern tws_sync_cmd_t g_tws_mmm_m2s_play_sync_cmd;
extern tws_sync_cmd_t g_tws_mmm_s2m_buffer[8];

extern buffer_rw_t g_tws_engine_m2s_pipe;
extern buffer_rw_t g_tws_engine_s2m_pipe;
extern tws_sync_cmd_t g_tws_engine_m2s_buffer[4];
extern tws_sync_cmd_t g_tws_engine_s2m_buffer[4];

extern buffer_rw_t g_tws_ui_m2s_pipe;
extern buffer_rw_t g_tws_ui_s2m_pipe;
extern tws_sync_cmd_t g_tws_ui_m2s_buffer[4];
extern tws_sync_cmd_t g_tws_ui_s2m_buffer[4];
extern int32 send_data_10ms_timer;
extern int32 repaired_timer;
extern tws_status_info_t tws_status_info;

extern uint8 already_start_connect_phone_flag;

extern uint8 snk_to_source_judge_flag;

extern uint8 random_value;
extern uint8 tws_quit_time;
extern uint8 tws_wait_quitflag;
extern uint8 phone_connect_flag;
extern uint8 tws_connect_flag;
extern uint8 tws_sync_group_all_count;
extern uint16 total_acl_pktnums;
extern uint16 tws_max_acl_pktnums;
extern uint8 slave_connect_phone_flag;
extern uint8 have_wait_for_manupair_flag;  //标志位目前用于hfp接收到回连请求后，会拒绝连接。并且在hfp关闭后立即发起回连
extern uint8 senddata_cnt;
extern bt_dev_t * wrong_phone_dev;

extern uint32 g_tws_src_last_cmd_send_timer;
extern bool g_tws_src_last_has_cmd_to_send;
#endif
#endif/* _BT_STACK_TWS_H */

