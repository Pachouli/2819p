/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙管理器内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __COMMON_BT_INNER_H__
#define __COMMON_BT_INNER_H__

#include "applib_inner.h"
#include "bluetooth_common.h"
#include "vm_def.h"
#include "config_id.h"
#include "common_tws_inner.h"

/*0x00:每次开机上电校准，RF校准参数未保存在VRAM中; 0x01:量产后第一次开机校准并保存到VRAM，后续开机使用配置参数*/
#define BT_CON_CLBRPARA_WRITE 0x01

//HFP AT命令电池电量上报状态机
typedef enum
{
    AT_BAT_NULL,
    AT_BAT_ENABLE,
    AT_BAT_UPDATE,
} at_bat_status_e;

//蓝牙管理器环境变量
typedef struct
{
    /*! HFP AT命令电池电量上报状态机 */
    at_bat_status_e at_bat_status;
    /*! HFP AT命令电池电量上报电量，0xff表示尚未上报 */
    uint8 at_bat_value;
    /*! HFP 服务是否连接 */
    uint8 hfp_linked;
    /*! 蓝牙物理连接 */
    uint8 service_connect_status;
    /*! BLE是否已连接 */
    uint8 ble_connect_flag;
    /*! 最近BT ENGINE状态 */
    uint8 last_status;
    /*! 是否正在获取手机时间 */
    uint8 read_phone_clock_flag;
    uint8 read_phone_clock_count;
    uint8 read_phone_clock_ot;
    /*! HFP AT命令相关定时变量*/
    uint32 at_timer;
    /*! 最近组队状态 */
    tws_paire_status_e last_tws_pair_status;
    /*! 最近设备TWS角色 */
    dev_role_e last_dev_role;
} com_bt_env_t;

typedef struct
{
    bool hfp_volume_sync_enable;
    uint8 hfp_start_flag;/*0表示尚未创建同步音量定时器，1表示已创建同步音量定时器，2表示已同步音量*/
    int hfp_timer_id;
} bt_hfp_info_t;

typedef struct
{
    bool avrcp_volume_sync_enable;
    uint8 a2dp_start_flag;/*0表示尚未创建同步音量定时器，1表示已创建同步音量定时器，2表示已同步音量*/
    int a2dp_timer_id;
} bt_a2dp_info_t;

//蓝牙状态信息
extern btengine_info_t bt_info;
extern com_bt_env_t g_com_bt_env;
extern bt_hfp_info_t bt_hfp_info;
extern bt_a2dp_info_t bt_a2dp_info;

extern bool power_on_flag;


extern void com_syn_vol_to_phone(uint8 volume);
extern void send_vol_to_phone(uint8 mode, uint8 g_volume);


//inner function
extern bool com_creat_bt_engine_inner(uint32 bt_att_mode);
extern bool com_close_bt_engine_inner(void);
extern bool com_init_btengine_info_inner(btengine_var_t *p_btengine_var);
extern void com_bt_event_loop_inner(void);
extern void com_bt_check_tws_pair_exit(void);
extern void com_get_btengine_status_inner(btengine_info_t *p_bt_info);

extern bool com_btengine_set_visual_mode_inner(uint8 visual_mode);
extern bool com_btengine_connect_deal_inner(uint8 mode, void *para);
extern bool com_btengine_set_sbc_play_inner(bool enter);
extern bool com_btengine_set_play_pipe_filter_inner(uint8 filter_opt);
extern bool com_btengine_set_hfp_call_inner(bool enter);
extern bool com_btengine_set_sco_pipe_filter_inner(uint8 filter_opt);
extern bool com_btengine_hid_take_photo_inner(void);
extern bool com_btengine_force_disable_sniff_inner(bool force_disable);

extern bool com_enter_btdbg_flag_inner(uint32 *btdbg_sel);
extern void com_enter_duttest_check_inner(void);

extern bool com_btengine_switch_avdtp_source(avdtp_data_source_e source);

extern bool com_btengine_hfp_siri_start(void);

/*!
 * \brief
 *  test_state_t 测试状态结构体
 */
typedef struct
{
    /*! 是否进入测试模式 */
    uint8 test_mode;
} test_state_t;

/*! 测试模式 */
extern test_state_t test_state;

#ifdef ENABLE_TRUE_WIRELESS_STEREO

extern bool com_btengine_tws_get_bt_cbk_inner(bt_clock_cbk_t *p_bt_clock_cbk);
extern bool com_btengine_tws_get_send_frame_cbk_inner(f_send_frame *p_f_send_frame);

#endif


//BLE/SPP 数传接口
extern bool bt_manager_blespp_data_trans_init(void);
extern bool bt_manager_blespp_data_trans_exit(void);

extern bool bt_manager_blespp_data_trans_create_ble_profile(void);
extern bool bt_manager_blespp_data_trans_destroy_ble_profile(void);

extern bool bt_manager_blespp_data_trans_create_spp_profile(void);
extern bool bt_manager_blespp_data_trans_destroy_spp_profile(void);

extern bool bt_manager_blespp_data_trans_register_dispatch_callback_inner(data_trans_dispatch_func f_data_trans_dispatch);
extern bool bt_manager_blespp_data_trans_deal_loop(void);
extern bool bt_manager_blespp_data_trans_send_inner(bt_passth_link_type ltype, uint8* p_data, uint16 data_len);


#endif //__COMMON_BT_H__
