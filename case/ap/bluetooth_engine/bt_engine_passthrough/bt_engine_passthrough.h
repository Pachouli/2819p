/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE SPP 支持头文件。
 * 作者：huanganbang
 ********************************************************************************/

#ifndef _BT_ENGINE_PASSTHROUGH_H_
#define _BT_ENGINE_PASSTHROUGH_H_

#include "bluetooth_common.h"

#define BT_ENGINE_PASSTHROUTH_SUPPORT   1
#define BTMEM_RESERVE_SIZE 400
#define BT_ENGINE_PASSTHROUGH_SEND_WAITTING_MAX 1024

typedef uint8 (*passth_send_func)(uint8 *data, uint16 data_len);

enum
{
    PASSTHROUGH_SERVER_DISABLE, PASSTHROUGH_SERVER_ENABLE,
};

typedef struct
{
    //server register
    uint8 enable; //确认spp可用于透传
    uint32 max_mtu; //最大支持MTU
    passth_send_func write; //写回调函数，用于透传模块通过spp发送数据到远端设备
    uint32 t_loss; //接收数据中总共丢弃的数据长度
    uint32 c_loss; //一次丢弃中连续丢弃的数据长度，如果一直没有收到数据则持续累加，如果收到数据后则清零

    //app register
    buffer_rw_t *receive_data_buf; //需要app设定bug参数，接收到app设定参数消息后再分配空间
    passthrough_receive_notify_cbk notify_cbk;
} passthrough_server_st;

typedef struct
{
    rcp_server_e cur_server;
    passthrough_server_st server[RCP_SERVER_MAX];
} bt_engine_passthrough_env_st;

extern bt_engine_passthrough_env_st g_bteg_passth_env;

extern void bt_engine_passthrough_init(void);
extern void bt_engine_passthrough_deinit(void);
extern void passth_register_server(rcp_server_e server_type, uint16 mtu, passth_send_func write);
extern void passth_unregister_server(rcp_server_e server_type);
extern app_result_e app_message_deal_passthrough(private_msg_t *pri_msg);
extern bool bt_engine_passth_receive(rcp_server_e server_type, uint8 *data, uint16 data_len);

extern bool bt_engine_passth_unregister_ble_server(int32 hdl);
extern bool bt_engine_passth_unregister_ble_write(int32 hdl, uint8 *w_data, uint16 w_len);
extern bool bt_engine_passth_unregister_ble_read(int32 hdl, uint8 *r_data, uint16 *r_len);
extern bool bt_engine_passth_updata_ble_conn_param(int32 hdl, le_conn_param_cfg_st* conn_param);

#endif
