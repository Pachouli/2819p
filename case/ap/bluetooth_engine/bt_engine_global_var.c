/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_global_var.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include  "bluetooth_engine.h"
//#include  "bt_src.h"

uint8 __section__(".bss.a2dp_input_frame_buf") SBC_DIRECT_INPUT_BUFFER[A2DP_SBC_DIRECT_INPUT_BUFFER_LEN];
uint8 __section__(".bss.a2dp_input_cache") SBC_INPUT_BUFFER_ADDRESS[A2DP_SBC_INPUT_BUFFER_LEN];

uint8 __section__(".bss.linein_input_cache") LINEIN_INPUT_BUFFER_ADDRESS[LINEIN_SBC_INPUT_BUFFER_LEN];

uint8 __section__(".bss.hfp_sco_err_report_buf") SCO_ERR_REPORT_BUF_ADDRESS[HFP_SCO_ERR_REPORT_BUF_LEN];
uint8 __section__(".bss.hfp_sco_output_cache") SCO_OUTPUT_CACHE_ADDRESS[HFP_SCO_OUTPUT_CACHE_LEN];
uint8 __section__(".bss.hfp_sco_receive_cache") SCO_RECEIVE_CACHE_ADDRESS[HFP_SCO_RECEIVE_CACHE_LEN];

uint8 __section__(".bss.bt_heap") BT_ENGINE_HEAP_ADDRESS[BT_ENGINE_HEAP_SIZE];

btengine_var_t g_btengine_var;
bt_devs_cfg_t devs_cfg;
vram_devs_cfg_t vram_devs_cfg;

buffer_rw_t g_a2dp_input_buf;

buffer_rw_t g_sco_input_buf;
sco_err_report_info_t g_sco_err_report_info;
buffer_rw_t g_sco_output_buf;

btengine_global_variable_struct btengine_global_variable;
btengine_config_struct g_btengine_cfg_data;
btengine_sco_deal_struct btengine_sco_deal;

btengine_info_t *g_p_info;

btengine_avrcp_data_struct *g_p_avrcp;

btengine_sco_deal_struct *g_p_sco;

uint8 current_sniff_count;

uint8 g_forbid_startup_connect; //禁止模组初始化后的回连功能
uint16 pause_cnt;
uint16 start_cnt;
bt_manager_cfg_info_t bt_manager_cfg_info;
bt_stack_cfg_info_t bt_stack_cfg_info;
bt_common_cfg_t btengine_param;
bt_auto_connect_cfg_t bt_auto_connect_cfg;
uint8 not_reconnect_a2dp_flag;
uint8 g_status_bak; //引擎状态变量备份
uint8 time_out_count; //测试模式下，连接超时计时

int8 half_second_timer_id = -1;
uint8 bt_linking_flag; //正在连接过程标志
uint32 profile_disconnect_count; //服务断开记数
test_result_t test_result;
regiter_func_t regiter_func;

uint8 ignore_sbc_after_pause;
uint32 ignore_sbc_after_pause_timer;

bool wait_bt_adjust_clk_down_flag;
uint32 bt_adjust_clk_timer;

uint8 g_last_phone_sco_created_flag;
bool g_last_phone_sniff_flag;
bool g_last_bt_linking_flag;
uint8 g_last_tws_pair_status;
bool g_last_tws_sniff_flag;
bool sco_connect_request_flag;
uint32 sco_connect_request_time;
uint8 current_visual_mode;

