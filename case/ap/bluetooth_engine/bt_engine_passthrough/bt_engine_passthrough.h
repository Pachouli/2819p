/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������BLE SPP ֧��ͷ�ļ���
 * ���ߣ�huanganbang
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
    uint8 enable; //ȷ��spp������͸��
    uint32 max_mtu; //���֧��MTU
    passth_send_func write; //д�ص�����������͸��ģ��ͨ��spp�������ݵ�Զ���豸
    uint32 t_loss; //�����������ܹ����������ݳ���
    uint32 c_loss; //һ�ζ������������������ݳ��ȣ����һֱû���յ�����������ۼӣ�����յ����ݺ�������

    //app register
    buffer_rw_t *receive_data_buf; //��Ҫapp�趨bug���������յ�app�趨������Ϣ���ٷ���ռ�
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
