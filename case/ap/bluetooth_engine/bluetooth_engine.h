/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:29:29     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine.h
 * \brief    xiaomaky
 * \version 1.0
 * \date  2013/04/11
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_H
#define _BLUETOOTH_ENGINE_H

#include "bluetooth_engine_common.h"
#include "bluetooth_engine_hfp_dec.h"
#include "bluetooth_engine_a2dp_dec.h"
#include "bluetooth_engine_unit_test.h"
#include "bt_engine_print.h"
#include "bt_engine_tws.h"
#include "bt_engine_freq_offset_config.h"
#include "bt_engine_dbg.h"
#include "bt_engine_remote_dev_info.h"
#include "bt_engine_device_control.h"
#include "bt_engine_spp_app_msg_deal.h"

#include "bt_interface.h"
#include "hid_def.h"
#include "spp_app_ui.h"

#define BT_LINK_WORK_FREQ  FREQ_108M

/*support ble function*/
//#define BT_SUPPORT_BLE

/*support spp function*/
//#define BT_SUPPORT_SPP

/*support get BR/EDR rssi info*/
#define BT_BREDR_GET_RSSI

//是否支持获取远端设备名称
#define BT_SP_RMT_NAME

//bt engine 打印的宏定义
#define BT_DEBUG_PRINT

#define ZERO_ADDRESS    BT_CON_ROM_ZERO_ADDR

#define CURRENT_DEVICE              (btengine_global_variable.g_cur_rmt_dev)
#define CURRENT_DEVICE_P_STATUS     (btengine_global_variable.g_cur_rmt_dev->p_dev_status)

#define CURRENT_DEVICE_STATUS   \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->status) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_A2DP_SUB_STATUS   \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->a2dp_sub_status) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_HFP_SUB_STATUS    \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->hfp_sub_status) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_SERVICE_CONNECT   \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->service_connect) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_NEED_AUTO_CONNECT   \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->need_auto_connect) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_SNIFF_FLAG        \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->sniff_flag) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_PHONE_STATUS      \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->phone_status) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_PHONE_NUMBER      \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->phone_number) : (*((uint8*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_ACL      \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->acl_handle) : (*((uint16*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_STREAM   \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->a2dp_source.stream_cid) : (*((uint16*)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_HFHDL    \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle) : (*((HANDLE)ZERO_ADDRESS)) )
#define CURRENT_DEVICE_SCOHDL  \
    ( ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)) ? \
    (btengine_global_variable.g_cur_rmt_dev->p_dev_status->sco_handle_setup) : (*((uint16*)ZERO_ADDRESS)) )

#define SUPPORT_SCO_ERR_REPORT  1
#define SUPPORT_SCO_UNIT_TEST                   0     //支持SCO单元测试(蓝牙通话)
#define SUPPORT_SCO_UNIT_TEST_SEND_SINE_WAVE    0     //支持在蓝牙通话中以正弦波代替mic声音
#define SUPPORT_A2DP_UNIT_TEST                  0     //支持A2DP单元测试(蓝牙播歌)

#define ENABLE_BATTERY_REPORT //使能向iphone手机发送电池电量

#define ENABLE_HFP_VOLUME_SYNC  //HFP音量同步，暂时屏蔽

/* the content protection IDs assigned by BT SIG */
#define BTA_AV_CP_SCMS_T_ID     0x0002
#define BTA_AV_CP_DTCP_ID       0x0001

#define BTA_AV_CP_LOSC                  2
#define BTA_AV_CP_INFO_LEN              3

#define FORCE_QUIT_APP_FLAG 0x80

//下面几个回连计时都是以500ms为单位
#define A2DP_CONNECT_TIME_AFTER_HFP_CONNECT         16    /* wait remote initiate a2dp-connection. */
#define HFP_CONNECT_TIME_AFTER_A2DP_CONNECT         30
#define CHECK_HID_CONNECT_TIME                      16
#define CHECK_AVRCP_CONNECT_TIME                    8

#define HFP_DISCONNECT_TIME_AFTER_A2DP_DISCONNECT   0     /* wait a2dp-disconnection.   */
#define QUIT_APP_TIME_AFTER_HFP_DISCONNECT          200   /* wait a2dp-disconnection.   */

#define APPLE_A2DP_PAUSE_TIME (1300)  //苹果设备暂停消息延时上报时间，12S左右
#define MAX_SNIFF_COUNT 3 //发送sniff 消息的最大次数
#define MAX_ALL_PROFILE_DIS_COUNT 46 //蓝牙所有服务断开计时(0.5秒为单位)
#define START_UP_CONNECT_TIME  5 //以秒为单位
#define HCIT_COMMAND  1
#define HCIT_ACLDATA 2
#define HCIT_SCODATA 3
#define HCIT_EVENT 4

#define A2DP_SC_PREFER_CHANNELMODE_NUMBER 0x4

#define COM_AUTOCONNECT_TICK_INTEVAL  2  //一个单位0.5秒

//1:以scan速度优先的fast_scan模式  0:以功耗优先的fast_scan模式
#define FAST_SCAN_MODE  1

typedef uint32 (*func_def0)(void);
typedef void (*func_def1)(uint32 a);
typedef void (*func_def2)(void *src, void *dst, uint32 len);

#define NEW(a) mem_malloc((int)(a))
#define FREE(a) mem_free((void*)(a))
#define memcmp        libc_memcmp
#define strlen        libc_strlen
#define strncmp       libc_strncmp
#define strncpy       libc_strncpy

/* A2DP routines */
enum A2DP_MENU_ID
{
    A2DPMENU_MAIN, A2DPMENU_SRC, A2DPMENU_SNK
};

enum
{
    /* mask in struct A2DPAPP_SEPStru */
    APP_A2DP_SEPMASK_ISSRCSVC = 0x10, APP_A2DP_SEPMASK_ISSNKSVC = 0x20
};

enum STATUS_UPDATA_REASON_ENUM
{
    CAUSED_BY_HFP_CONNECTED = 0x0, CAUSED_BY_HFP_DISCONNECTED,
};

enum
{
    ONOFF_ON = 0x0, ONOFF_OFF = 0x1,
};

//bt page scan窗口大小，值越大，连接速度越快，功耗也越高
typedef enum
{
    BT_PAGE_SCAN_ACT_WIN_12,     //0x12*0.625ms   11.25ms 最小窗口，连接最慢 （建议取值12/48/120/480/1000）
    BT_PAGE_SCAN_ACT_WIN_100,    //0x100
    BT_PAGE_SCAN_ACT_WIN_200,    //0x200
    BT_PAGE_SCAN_ACT_WIN_400,    //0x400
    BT_PAGE_SCAN_ACT_WIN_800,    //0x800          1.28s
} bt_page_scan_act_window_size_e;

enum
{
    DEFAULT_SCAN_PARAM0,          //core spec default scan param 默认参数
    FAST_SCAN_PARAM1,             //fast scan reference param1 较慢的fast scan
    FAST_SCAN_PARAM2,             //fast scan reference param2 较快的fast scan
    NORMAL_SCAN_PARAM_LINEIN_TWS, //line in tws normal scan param 保证line in tws 不断音的参数
    NORMAL_SCAN_PARAM_BTPLAY_TWS, //bt_play tws normal scan param 保证btplay tws命令交互正常的参数
    SCAN_PARAM_LIST_MAX,          //scan param list max
};

typedef struct
{
    uint16 inq_scan_win;    //inquiry scan window value
    uint16 inq_scan_itv;    //inquiry scan interval value
    uint16 page_scan_win;   //page scan window value
    uint16 page_scan_itv;   //page scan interval value
} fast_scan_param_list_st;

typedef struct
{
    //目前最多是13个event，因此用16bit的mask，
    //bit 1~ bit 13分别对应AVRCP_EVENT_PLAYBACK_STATUS_CHANGED到AVRCP_EVENT_VOLUME_CHANGED
    uint16 EventMask_CT;
    uint16 EventMask_TG; // 本地作为TG被注册的event
    uint8 avrcp_play_status;
    uint8 remote_absoluteVolume; //AVRCP_ABSOLUTE_VOLUME_MIN~AVRCP_ABSOLUTE_VOLUME_MAX

} btengine_avrcp_data_struct;

typedef struct
{
    uint16 halfsec_time_count;  //半秒定时间计数器
    uint16 reconn_interval;     //回连间隔时间
    uint16 reconn_times_count;  //回连次数
} btengine_autoconnect_count_t;

typedef struct
{
    uint16 enable_a2dp_flag :1; //是否支持a2dp
    //HFP_PB_ALL,HFP_PB_DISABLE,HFP_PB_ONLY_NUM
    uint16 hfp_pb_mode :2; //读取配置，使能电话本功能0:禁止，1:完整版本 2:只有来电号码播报:
    uint8 max_link_num;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    uint32 s_src_svc_hdl;
    struct A2DP_SEIDStru *s_local_src_seid;
#endif
    uint32 s_snk_svc_hdl;

    struct A2DP_SEIDStru *s_local_snk_seid[MAX_SEID];

    HANDLE g_hf_server_hdl;
    HANDLE g_hs_server_hdl;

    os_event_t *actaec_mutex;

    UINT8 g_hf_spk_vol;
    UINT8 g_hf_mic_vol;
    UINT8 g_hf_bvra_enable;

    struct HFP_CLCCInfoStru hfp_clcc_info;
    struct HFP_CINDInfoStru hfp_cind_info;

} btengine_config_struct;

#define MAX_BT_IDLE_TIME     (16) //蓝牙空闲最大时间8S (0.5秒为单位)
#define MAX_BT_LOWPW_WINDOW  (8)  //蓝牙低功耗观察窗口4S (0.5秒为单位)
#define MAX_BT_SBC_DATA_TRANS     (2) //蓝牙空闲最大时间1S (0.5秒为单位)

typedef enum
{
    BT_GERNE_SCAN_MODE, BT_FAST_SCAN_MODE,
} bt_scan_mode;

typedef enum
{
    FORMAT_ENCODE_TRANSMISSION = 0,     //SCO数据发送/接收时先编码后传输
    TRANSPARENT_TRANSMISSION,           //SCO数据发送/接收时直接透传
} sco_data_transfer_mode_e;

enum
{
    DATA_UNFILTER, DATA_FILTER,
};

enum
{
    INACTIVE, ACTIVATE,
};

enum
{
    SCO_CREATE_READY = 0, SCO_CREATING, SCO_CONNECTED, SCO_DISCONNECT,
};

enum
{
    VMODE_DEFAULT_VISIBLE, VMODE_ONLY_FIRST_VISIBLE, VMODE_DEFAULT_INVISIBLE,
};

typedef void (*pfunc_printf)(const uint8 *fmt, ...);

typedef struct _bt_printf_st
{
    pfunc_printf log;
    pfunc_printf log_dbg;
} bt_printf_st;

typedef struct
{
    btmempool_cfg_t btmempool_cfg;
    bt_page_scan_act_window_size_e bt_page_scan_actwin; //bt inquiry scan windows 大小，影响到蓝牙连接速度
} btengine_bt_stack_init_param_cfg;

/*异步处理清除配对设备信息，为了妥协更快完成蓝牙断开连接在发送断开连接命令后即认为已经断开连接，而清除信息需要
 完全断开服务连接后才能清除，否则获取不到设备信息会出错。*/
typedef struct
{
    uint8 mode; //clear_pl_mode
    uint8 need_clear_paired_list_async;
    uint16 clear_paired_list_timeout;
} btengine_clear_paired_list_async_deal_st;

typedef struct
{
    bt_dev_t *dev;
    uint8 full_name[BT_REMOTE_NAME_MAX_LEN];
} bt_remote_dev_name_st;

typedef struct
{
    btengine_info_t g_btengine_info;
    uint16 connect_count;
    uint16 disconnect_count;
    uint16 quit_count;
    uint16 read_rf_tmpr_count;
    uint8 recv_quit_flag;
    uint8 local_device_support_profile; //本地设备支持的协议
    bt_dev_t *g_cur_rmt_dev; //当前远端设备信息
    bt_dev_t *linking_dev; //当前远端设备信息
    bt_dev_t *linked_dev_list[MAX_DEVICE_LINK]; //已连接的设备信息
    bt_dev_t *autoconnect_dev_list[MAX_DEVICE_LINK]; //自动回接列表的设备信息
    uint8 g_sbc_thread_exist :1;
    uint8 a2dp_linking_flag :1; //a2dp服务正在连接状态
    uint8 connect_lock_flag; //当前建立的acl 链路数
    uint8 reconnect_another_cnt; //在连接成功后，后台尝试回连另外一个服务的计数，
    //连上每个服务都清0,后面尝试回连的时候就+1，次数到达limit了，就不继续尝试
    uint16 onlyone_disconnect_cnt;
    uint16 avrcp_disconnect_cnt;
    uint8 media_codec_type;
    uint8 visual_mode; //当前可见性，可连接性状况
#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
    uint8 global_visible; //1表示默认可见，0表示强制不可见。
#endif
    uint8 g_bLinkedOld;

    btengine_avrcp_data_struct btengine_avrcp_data;
    bool filter_avdtp_sound_flag;

    uint8 btdbg_sel_flag; //0:don't support bt debug 1：support bt debug & con don't set debug sel 2:surpport & con set debug sel
    uint32 btdbg_sel_value;

    uint16 bt_idle_cnt;
    uint16 bt_highpw_cnt;
    uint32 last_avdtp_data_timer;
    uint16 last_avdtp_seq;
    uint16 bqb_test;  //bit0:BQB_TEST_ENABLE 1:BQB_DELAY_REPORT_ENABLE
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    uint8 dev_role; //用于界定local dev 为TWS_SLAVE/TWS_MASTER
#endif
    uint8 sbc_max_bitpool; //0x35高质量，0x23中等质量
    hfp_st hfp;
    bt_printf_st printf;
    bt_hw_error_no_e hw_error_no;

    btengine_autoconnect_count_t reconnect_status;
    bool disable_bt_sniff_by_app;  //某些app进入时禁止进入sniff，可以置位此标志位防止bt进入sniff
    btengine_bt_stack_init_param_cfg bt_stack_init_param_cfg;
    avdtp_data_source_e avdtp_data_source;
    bool need_connect_a2dp_right_now;
    uint8 g_cur_rmt_dev_paried_index;   //当前设备在配对列表中的序号
    uint8 enable_calib_bt_freq_offset;  //允许进入频偏校准模式
    uint16 sbc_data_stop_trans_cnt;
    btengine_clear_paired_list_async_deal_st clpl_async_info;
    uint8 fast_scan_status;
} btengine_global_variable_struct;

#define CLEAR_PLIST_ASYNC_DEAL_TIMEOUT 1000

typedef struct
{
    UINT8 sf4_chn4;
    UINT8 bl4_sb2_allo2;
    UINT8 min_bitpool8;
    UINT8 max_bitpool8;
} A2dpSbcInfoStru;

extern void _app_init(void);
extern void _app_exit(void);
//bt_engine_message_handle.c
extern bool app_msg_deal_hfp_connect(bt_dev_t *dev);

extern void auto_connect_deal(void);
extern void bt_auto_reconnect(void);
extern void reconnect_by_addr(uint8 *bd_addr);
extern void reconnect_last_device(bt_dev_t *dev);
extern void app_reconnect_last_device(void);
extern void app_msg_deal_force_waitpaired(uint8 force_type);
extern app_result_e app_msg_deal_app_quit(void);
extern void app_msg_deal_transmit_dtmf(uint8 dtmf_code);
extern void app_msg_deal_dial_phone_number(uint8 *phone_num);
extern void app_hfp_volume_update(uint8 g_volume);
extern void app_avrcp_volume_update(bt_dev_t *dev, uint8 g_volume);
extern void app_msg_deal_volume_sync(uint8 g_volume);
extern uint8 app_message_deal_a2dp(private_msg_t *pri_msg);
extern uint8 app_message_deal_avrcp(private_msg_t *pri_msg);
extern void APP_Create_SCO_Timer_Proc(void);
extern uint8 app_message_deal_hfp(private_msg_t *pri_msg);
extern uint8 app_message_deal_hid(private_msg_t *pri_msg);
extern uint8 app_message_deal_btcfg(private_msg_t *pri_msg);
extern uint8 app_message_deal_bqb(private_msg_t *pri_msg);
extern void bt_get_paired_list_info(private_msg_t *pri_msg);
extern void bt_clr_paired_list_info(void);
extern void add_dev_to_paired_list(private_msg_t *pri_msg);
extern uint8 app_message_deal_info(private_msg_t *pri_msg);
extern uint8 app_message_deal_misc(private_msg_t *pri_msg);
extern app_result_e app_message_deal_enter_s2(private_msg_t *pri_msg);
extern app_result_e app_message_deal(private_msg_t *pri_msg);

extern void check_device_disconnect(void);

extern void print_link_key(unsigned char *buf, int32 len);
extern void bt_set_address(void);

extern void half_second_handle(void);
extern app_result_e btengine_deal_app_msg(void);
extern app_result_e get_message_loop(void);
extern void btengine_reply_msg(void *msg_ptr, bool ret_vals);
extern app_result_e app_message_deal(private_msg_t *pri_msg);
extern uint32 gen_random_lap(void);
extern void check_sniff_status(void);

extern void bt_disconnect_acl(bt_dev_t *dev);
extern void bt_reset_controller(void);
extern void bt_set_link_policy(bt_dev_t *dev, uint8 link_policy_setting);
extern void bt_set_default_link_policy(uint8 link_policy_setting);
extern void bt_set_default_errordata_report(void);
extern void bt_get_cur_role(bt_dev_t *dev);
extern void bt_set_cur_role(bt_dev_t *dev, uint8 role);
extern void cancel_create_connection(bt_dev_t *dev);
//bt_engine_device_control.c
extern app_result_e BtDev_Open(void);
extern void BtDev_Close(void);
extern void cancel_create_connection(bt_dev_t *dev);

extern void btengine_get_btinfo(private_msg_t *pri_msg);
extern void btengine_set_btinfo(private_msg_t *pri_msg);
extern void btengine_set_connect_info(private_msg_t *pri_msg);
extern void bt_dut_test(void);
extern void bt_set_visual_mode(int visual_mode);
extern void bt_write_link_supervision_timeout(uint16 aclhdl, uint16 timeout); //625us为单位
extern void bt_set_DiscoverableMode(uint16 inquiry_scan_interval, uint16 inquiry_scan_window, uint8 default_scan_mode,
        uint8 isLimitedMode);

//bt_engine_lib_init.c
extern void A2dpRegSep(void);
extern void A2dpRegSnkService(void);
extern void APP_A2DP_Start(void);
extern void APP_A2DP_Stop(void);
extern void APP_A2DP_Stop(void);
extern void APP_AVRCP_Stop(void);
extern void APP_HFP_Start(void);
extern void APP_HFP_Stop(void);
extern void APP_HID_REG(void);
extern void AppStart(void);
extern void AppStop(void);
extern void FreeTransportLayer(void);

extern int32 BtDev_LibInit(void);
extern void btengine_auto_connect(void);
extern void BT_SetReconnectFlag(uint8 dev_type, uint8 flag);

extern void bt_enter_sniff(bt_dev_t *dev);
extern void bt_exit_sniff(bt_dev_t *dev);
extern void BtDev_PowerDown(void);

extern void A2dpIndCbk(UINT8 event, void *param);
extern int AppWaitForInput(char *out_str, int str_size);

extern void APP_AVRCP_RegisterNotification_Req(UINT8 *bd, UINT8 event_id);
extern void APP_AVRCP_Cbk(UINT8 *bd, UINT32 event, UINT8 *param, UINT16 len);
extern int8 APP_AVRCP_PassThrough_Req(bt_dev_t *dev, UINT8 op_id, UINT8 vendor_id);
extern int8 APP_AVRCP_PassThrough_Press(bt_dev_t *dev, UINT8 op_id, UINT8 vendor_id);
extern int8 APP_AVRCP_PassThrough_Release(bt_dev_t *dev, UINT8 op_id, UINT8 vendor_id);
extern void APP_AVRCP_VolumeChangedNTF(UINT8 *bd, UINT8 rsp_code, UINT8 cur_volume);
extern void app_avrcp_volume_update(bt_dev_t *dev, uint8 g_volume);
extern void Avrcp_Connect_Req(bt_dev_t *dev);
extern UINT8 APP_HFEventCbk(void *hdl, UINT8 event, UINT8 *param, UINT32 len);
extern void HFCmdInRing(bt_dev_t *dev, int32 choice);
extern bool A2dpConnect(uint8 type, uint8* bt_addr);
extern void A2dpDisconnect(bt_dev_t *dev);
extern bool HFConnectAG(bt_dev_t *dev);
extern void HFDisconnectAG(bt_dev_t *dev);
extern void gap_reg_sco_cbk(void);
extern void gap_unreg_sco_cbk(void);
extern bool app_msg_deal_a2dp_connect(bt_dev_t *dev);

extern bool HID_connect(bt_dev_t *dev);
extern void HidAppCbk(UINT32 inst_handle, tHid_Event event, void *param);
#ifdef ENABLE_HFP_VOLUME_SYNC
extern void app_hfp_volume_update(uint8 g_volume);
#endif

extern void AppStop(void);

extern void format_long_name(const uint8 *src_name, uint8 *dest_name);

extern void read_avdtp_sound_hook(void);
extern void *bt_con_high_handle(void *param);
extern void *bt_con_low_handle(void *param);
extern void deal_micout_sco_data(void);
extern void report_battery_to_rmtdevice(uint8 mode, uint8 bat_val);
extern void bt_adjust_clk(uint8 freq_level);

extern void save_rmtdev_info(void);
extern void save_plist_to_vram(void);
extern void init_plist_info(void);
extern void clear_plist_info(uint8 clear_mode);
extern void clear_plist_info_async_start(uint8 clear_mode);
extern void clear_plist_info_async_deal(btengine_clear_paired_list_async_deal_st* p_clpl_aync_info);
extern bool read_plist_from_vram(bt_dev_t *bt_dev, uint8 list_num);

extern int RmtFindPairedDevByBD(uint8 *bdAddress, bt_dev_t *p_dev);
extern bt_dev_t *RmtFindDevByBD(UINT8 *bd);
extern bt_dev_t *BTFindDevByAclHdl(uint16 acl_hdl);

extern void init_callback_func(void);

extern void load_micout_timer(void);
extern void unload_micout_timer(void);
//bt_engine_gap_link.c
extern bt_dev_t *RmtFindDevInAutoConnectList(void);
extern void RmtRemoveDevice(bt_dev_t *p_dev);
extern void RmtRemoveDevFromAutoConnectList(bt_dev_t *p_dev);
extern void RmtRemoveDevFromLinkedList(bt_dev_t *p_dev);
extern bt_dev_t *BT_RmtFindDevByBD(uint8 *bdAddress);

extern void change_cur_device_status(uint8 cur_status);
extern uint8 get_cur_device_status(bt_dev_t *dev);
extern void set_cur_device_status(bt_dev_t* dev, uint8 bt_status);

extern void set_auto_connect_flag(uint8 flag);
extern void deal_device_phone_status(void);
extern bt_dev_t *RmtAddDevice(uint8 *bdAddress);
extern void RmtAddSearchedDev(struct GAP_RemoteDeviceInfoStru *in);
extern void ReadRemoteNameCfmCbk(void *context, UINT16 result, struct GAP_ExecuteCommandCfmStru *cfm_par);
extern void RmtUpdateName(uint8 *bd);
extern void RmtDevConnected(struct GAP_ConnectionEventStru *in);
extern void RmtLinkKeyReply(struct HCI_Security_Link_Key_Ask4Stru *in);
extern void RmtLinkKeyCreated(struct HCI_Security_Link_Key_SaveStru *in);
extern tGAP_IndCbk_Result RmtConnectionReply(struct HCI_Connection_RequestEvStru *in);
extern void RmtInquiryComplete(void);
extern void RmtAuthenticationFail(void);
extern void RmtRemoteDeviceNameUpdate(void);
extern void RmtUserConfirmationRequest(void);
extern void RmtUserPasskeyRequest(void);
extern void RmtSimplePairingComplete(struct HCI_Simple_Pairing_CompleteEvStru *in);
extern void RmtUserPasskeyNotification(void);
extern void RmtAuthenticationRequest(void);
extern void RmtAuthenticationAbort(void);
extern void RmtModeChange(struct HCI_Mode_ChangeEvStru *in);
extern void RmtRemovePairedDev(bt_dev_t *p_dev);
extern bt_dev_t *RmtFindDevInLinkedList(void);

extern tGAP_IndCbk_Result AppGapCbk(HANDLE tl_handle, void *context, tGAP_Ind_Type ind_type, void *parameter,
UINT32 size);

//bt_engine_message_loop.c
extern void check_is_needed_reconnect(void);

//bt_engine_autotest_deal.c
extern void check_connect_timeout(void);
extern uint8 test_add_to_connectList(bt_dev_t *dev);
extern void test_disconnect_acl(bt_dev_t *dev);
extern void boot_reconnect_deal(void);

extern void bt_SetFasterScanMode(void);
extern void bt_RestoreNormalScanMode(void);

#ifdef BT_SP_GETCALLINFO
extern void load_call_timer(void);
extern void unload_call_timer(void);
#endif

extern btengine_var_t g_btengine_var;
extern bt_devs_cfg_t devs_cfg;
extern vram_devs_cfg_t vram_devs_cfg;

extern buffer_rw_t g_a2dp_input_buf;
extern buffer_rw_t g_sco_input_buf;
extern buffer_rw_t g_sco_output_buf;
extern sco_err_report_info_t g_sco_err_report_info;

extern btengine_global_variable_struct btengine_global_variable;
extern btengine_sco_deal_struct btengine_sco_deal;
extern btengine_config_struct g_btengine_cfg_data;
extern btengine_info_t *g_p_info;
extern btengine_avrcp_data_struct *g_p_avrcp;
extern btengine_sco_deal_struct *g_p_sco;

extern uint8 current_sniff_count;
extern uint8 total_sniff_count;
extern uint8 g_forbid_startup_connect;

extern uint8 g_status_bak; //引擎状态备份
extern uint8 time_out_count;

extern int8 half_second_timer_id;

extern bt_manager_cfg_info_t bt_manager_cfg_info;
extern bt_stack_cfg_info_t bt_stack_cfg_info;
extern bt_common_cfg_t btengine_param;
extern bt_auto_connect_cfg_t bt_auto_connect_cfg;
extern uint8 bt_linking_flag;           //表示蓝牙连接过程，从gap_inquiry到profile连接完成
extern uint32 profile_disconnect_count; //服务断开记数
extern test_result_t test_result;
extern regiter_func_t regiter_func;
extern uint8 not_reconnect_a2dp_flag;
extern uint8 ignore_sbc_after_pause;
extern uint32 ignore_sbc_after_pause_timer;
extern bool wait_bt_adjust_clk_down_flag;
extern uint32 bt_adjust_clk_timer;

extern uint8 g_last_phone_sco_created_flag;
extern bool g_last_phone_sniff_flag;
extern bool g_last_bt_linking_flag;
extern uint8 g_last_tws_pair_status;
extern bool g_last_tws_sniff_flag;
extern bool sco_connect_request_flag;
extern uint32 sco_connect_request_time;
extern uint8 current_visual_mode;

#if (SUPPORT_SCO_UNIT_TEST == 1)
extern sco_unit_test_info_st *gpst_sco_ut_info;
#endif
#if (SUPPORT_A2DP_UNIT_TEST == 1)
extern a2dp_unit_test_info_st *gpst_a2dp_ut_info;
#endif

extern void HID_DataInd(struct HidDataIndStru * dataInd);
extern void HID_Send_Handshake_Packet(HID_HANDSHAKE_MESSAGE message);
extern void HID_Handle_Control_Message(HID_CONTROL_OP operation);

#if (SUPPORT_SCO_UNIT_TEST == 1)
extern void sco_unit_test_init(void);
extern void sco_unit_test_deinit(void);
extern uint8 sco_unit_test_get_status(void);
extern void sco_unit_test_set_status(uint8 status);
extern void sco_unit_test_receive_err_pkg_count(uint8 *buf, int32 len, uint8 pkg_status_flag);
extern void sco_unit_test_send_micout_data(void);
#endif

#if (SUPPORT_A2DP_UNIT_TEST == 1)
extern void a2dp_unit_test_init(void);
extern void a2dp_unit_test_deinit(void);
extern uint8 a2dp_unit_test_get_status(void);
extern void a2dp_unit_test_set_status(uint8 status);
extern void a2dp_unit_test_get_package(void *data_addr, uint16 data_len);
#endif

void linein_tws_buffer_init(void);
void linein_tws_send_one_frame(uint8 *frame_data, uint16 data_len);

extern int adjust_frequency_offset(uint32 ajust_flag, uint32 bt_status);

extern void hci_data_transmission_hook(void);
extern void hci_lowpower_switch_hook(uint8 low_power_mode);
extern bool bt_test_active(void);
extern bool bt_check_dev_exist(bt_dev_t* dev);
#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
void disable_visible(void);
#endif
extern void sbc_data_transmission_hook(void);

extern uint8 BT_ENGINE_HEAP_ADDRESS[BT_ENGINE_HEAP_SIZE];

extern uint8 updata_paired_list_length(bt_devs_cfg_t *p_dev_cfg);
extern void SppStart(void);
extern void SppStop(void);
extern void SppAppCreateConnection(void);
extern void SppAppDisconnect(void);
extern void SppAppCbk(UINT32 inst_handle, tSpp_Event event, void *param);

/* _BLUETOOTH_ENGINE_H */
#endif

