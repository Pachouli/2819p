/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bluetooth_engine.h"
#include "bt_engine_passthrough.h"
#include "bt_engine_spp_app_msg_deal.h"

#if (BT_ENGINE_PASSTHROUTH_SUPPORT == 1)

const int32 shm_id_list[] =
{ SHARE_MEM_ID_BLESPP_INPIPE1, SHARE_MEM_ID_BLESPP_INPIPE2 };
bt_engine_passthrough_env_st g_bteg_passth_env;

bool receive_buffer_init(rcp_server_e server_type)
{
    g_bteg_passth_env.server[server_type].receive_data_buf = (buffer_rw_t *) sys_shm_mount(shm_id_list[server_type]);
    if (g_bteg_passth_env.server[server_type].receive_data_buf == NULL)
    {
        BTEG_DBG_PRINTF("app receive buffer%d not ready\n", shm_id_list[server_type]);
        return FALSE;
    }
    return TRUE;
}

void bt_engine_passthrough_init(void)
{
    BTEG_DBG_PRINTF("bt_engine_passthrough_init\n");
    g_bteg_passth_env.cur_server = RCP_WITH_SPP;
    libc_memset(g_bteg_passth_env.server, 0x00, sizeof(passthrough_server_st) * RCP_SERVER_MAX);
    receive_buffer_init(g_bteg_passth_env.cur_server);
}

void bt_engine_passthrough_deinit(void)
{
    BTEG_DBG_PRINTF("bt_engine_passthrough_deinit\n");
    g_bteg_passth_env.cur_server = RCP_WITH_SPP;
    libc_memset(g_bteg_passth_env.server, 0x00, sizeof(passthrough_server_st) * RCP_SERVER_MAX);
}

void passth_register_server(rcp_server_e server_type, uint16 max_mtu, passth_send_func write)
{
    passthrough_server_st *server = &g_bteg_passth_env.server[server_type];

    server->enable = PASSTHROUGH_SERVER_ENABLE;
    server->max_mtu = max_mtu;
    server->write = write;
    receive_buffer_init(server_type);
}

void passth_unregister_server(rcp_server_e server_type)
{
    passthrough_server_st *server = &g_bteg_passth_env.server[server_type];

    server->enable = PASSTHROUGH_SERVER_DISABLE;
}

passthrough_server_st *passth_get_cur_server(void)
{
    return &g_bteg_passth_env.server[g_bteg_passth_env.cur_server];
}

bool passth_send_data(passth_pkg_t *p_rcp)
{
    passthrough_server_st *server = passth_get_cur_server();

    uint32 cur_size, limit_size;
    //获取当前堆空间剩余大小，当容量太小时停止发送，返回失败给上层应用
    cur_size = get_btmem_free_size();
    if (cur_size > BTMEM_RESERVE_SIZE)
    {
        limit_size = cur_size - BTMEM_RESERVE_SIZE;
    }
    else
    {
        limit_size = 0;
    }

    if (p_rcp->rcp_len > limit_size)
    {
        BTEG_DBG_PRINTF("passthrough too fast\n");
        return FALSE;
    }

    if (server->write == NULL)
    {
        BTEG_DBG_PRINTF("server %p write func not reg\n", server);
        return FALSE;
    }

    return server->write(p_rcp->rcp_data, p_rcp->rcp_len);
}

app_result_e app_message_deal_passthrough(private_msg_t *pri_msg)
{
    uint8 rst = RESULT_NULL;

    switch (pri_msg->msg.type)
    {
//        /* app发送数据到remote device */
//        case MSG_BTENGINE_PASSTHROUGH_SEND_DATA:
//        {
//            passth_pkg_t *p_rcp = (passth_pkg_t *) (pri_msg->msg.content.addr);
//
//            passth_send_data(p_rcp);
//            btengine_reply_msg(pri_msg, TRUE);
//        }
//        break;

        case MSG_BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC:
        {
#ifdef BT_SUPPORT_BLE
            bt_managet_create_passth_param_st *param = (bt_managet_create_passth_param_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC_ERR;
            }

            if ((param->hdl == NULL) || (param->arg == NULL))
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC_ERR;
            }

            *param->hdl = passthrough_profile_register(param->arg);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_DELETE_BLE_PASSTHROUGH_SYNC :
        {
#ifdef BT_SUPPORT_BLE
            uint32 ble_hdl = (uint32)(pri_msg->msg.content.addr);
            if (passthrough_profile_unregister(ble_hdl) == TRUE)
            {
                btengine_reply_msg(pri_msg, TRUE);
            }
            else
#endif
            {
                btengine_reply_msg(pri_msg, FALSE);
            }
        }
        break;

        case MSG_BTENGINE_BLE_PASSTHROUGH_DATA_SEND_SYNC:
        {
#ifdef BT_SUPPORT_BLE
            bt_managet_data_wr_st *param = (bt_managet_data_wr_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_BLE_PASSTHROUGH_DATA_SEND_SYNC_ERR;
            }

            passthp_send(param->hdl,param->data,param->len);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_BLE_PASSTHROUGH_DATA_SEND_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_BLE_PASSTHROUGH_DATA_READ_SYNC:
        {
#ifdef BT_SUPPORT_BLE
            bt_managet_data_wr_st *param = (bt_managet_data_wr_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_BLE_PASSTHROUGH_DATA_READ_SYNC_ERR;
            }

            passthp_read(param->hdl,param->data,param->len);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_BLE_PASSTHROUGH_DATA_READ_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_BLE_PASSTHROUGH_GET_INFO_SYNC:
        {
#ifdef BT_SUPPORT_BLE
            bt_managet_ble_passth_get_info_st *param = (bt_managet_ble_passth_get_info_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_BLE_PASSTHROUGH_GET_INFO_SYNC_ERR;
            }

            passthp_get_info(param->hdl,param->type,param->rdata);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_BLE_PASSTHROUGH_GET_INFO_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        /* 注册接收通知回调函数，在接收到remote device发来的数据时发生回调 */
//        case MSG_BTENGINE_REGISTER_RECEIVE_DATA_NOTIFY_CALLBACK:
//        {
//            passthrough_server_st *server = passth_get_cur_server();
//
//            server->notify_cbk = (passthrough_receive_notify_cbk) pri_msg->msg.content.addr;
//            btengine_reply_msg(pri_msg, TRUE);
//        }
//        break;

        case MSG_BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC:
        {
#ifdef BT_SUPPORT_SPP
            bt_managet_create_passth_param_st *param = (bt_managet_create_passth_param_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC_ERR;
            }

            if ((param->hdl == NULL) || (param->arg == NULL))
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC_ERR;
            }

            *param->hdl = spp_register(param->arg);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_DELETE_SPP_PASSTHROUGH_SYNC :
        {
#ifdef BT_SUPPORT_SPP
            uint32 spp_hdl = (uint32)(pri_msg->msg.content.addr);
            if (spp_unregister(spp_hdl) == TRUE)
            {
                btengine_reply_msg(pri_msg, TRUE);
            }
            else
#endif
            {
                btengine_reply_msg(pri_msg, FALSE);
            }
        }
        break;

        case MSG_BTENGINE_SPP_PASSTHROUGH_DATA_SEND_SYNC:
        {
#ifdef BT_SUPPORT_SPP
            bt_managet_data_wr_st *param = (bt_managet_data_wr_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_SPP_PASSTHROUGH_DATA_SEND_SYNC_ERR;
            }

            spp_send(param->hdl,param->data,param->len);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_SPP_PASSTHROUGH_DATA_SEND_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_SPP_PASSTHROUGH_DATA_READ_SYNC:
        {
#ifdef BT_SUPPORT_SPP
            bt_managet_data_wr_st *param = (bt_managet_data_wr_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_SPP_PASSTHROUGH_DATA_READ_SYNC_ERR;
            }

            spp_read(param->hdl,param->data,param->len);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_SPP_PASSTHROUGH_DATA_READ_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_SPP_PASSTHROUGH_GET_INFO_SYNC:
        {
#ifdef BT_SUPPORT_SPP
            bt_managet_ble_passth_get_info_st *param = (bt_managet_ble_passth_get_info_st *)(pri_msg->msg.content.addr);
            if (param == NULL)
            {
                BLE_DBG_PRINTF("param is error\n");
                goto BTENGINE_SPP_PASSTHROUGH_GET_INFO_SYNC_ERR;
            }

            spp_get_info(param->hdl,param->type,param->rdata);
            btengine_reply_msg(pri_msg, TRUE);

            BTENGINE_SPP_PASSTHROUGH_GET_INFO_SYNC_ERR:
#endif
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

//        case MSG_BTENGINE_SET_RECEIVE_BUFFER_PARAM:
//        {
//     
//            buffer_rw_t *buf = (buffer_rw_t *)pri_msg->msg.content.addr;
//            passthrough_server_st *server = passth_get_cur_server();
//
//            if (buf == NULL)
//            {
//                BTEG_PRINTF("set spp rev buf param err\n");
//                btengine_reply_msg(pri_msg, FALSE);
//            }
//
//            //buf在应用退出时释放
//            server->receive_data_buf = sys_malloc(sizeof(buffer_rw_t));
//            if (server->receive_data_buf == NULL)
//            {
//                BTEG_PRINTF("receive buf malloc err\n");
//                btengine_reply_msg(pri_msg, FALSE);
//            }
//
//            libc_memcpy(server->receive_data_buf,buf,sizeof(buffer_rw_t));
//            btengine_reply_msg(pri_msg, TRUE);
//        }
//        break;

//        case MSG_BTENGINE_GET_RCP_SERVER_SUPPORT_INFO:
//        {
//            btengine_reply_msg(pri_msg, TRUE);
//        }
//        break;

        default:
        rst = RESULT_IGNORE;
        break;
    }

    return rst;
}

static void print_receive(uint8 *data, uint16 data_len)
{
    libc_printf("pass recv: %d\n", data_len);
    //libc_printhex(data, (data_len > 32) ? 32 : data_len, 0);
}

bool bt_engine_passth_receive(rcp_server_e server_type, uint8 *data, uint16 data_len)
{
    receive_rcp_info_t receive_rcp_info;

    if (server_type != g_bteg_passth_env.cur_server)
    {
        BTEG_PRINTF("not in active passth server\n");
        return FALSE;
    }

    passthrough_server_st *server = &g_bteg_passth_env.server[server_type];

    if (server->receive_data_buf == NULL)
    {
        if (receive_buffer_init(shm_id_list[server_type]) == FALSE)
        {
            return FALSE;
        }
    }

    print_receive(data, data_len);

    if (buffer_rw_write(server->receive_data_buf,data,data_len) == FALSE)
    {
        server->t_loss += data_len;
        server->c_loss += data_len;
        BTEG_PRINTF("passth buffer full,closs %d,tloss\n", server->t_loss);
        return FALSE;
    }
    else
    {
        if (server->c_loss != 0)
        {
            server->c_loss = 0;
        }
    }

    if (server->notify_cbk != NULL)
    {
        receive_rcp_info.c_loss = server->c_loss;
        receive_rcp_info.t_loss = server->t_loss;
        server->notify_cbk(&receive_rcp_info);
    }
    return TRUE;
}

bool bt_engine_passth_updata_conn_param(uint32 handle, le_conn_param_cfg_st* conn_param)
{
    return TRUE;
}
;

#endif //end of #if (BT_ENGINE_PASSTHROUTH_SUPPORT == 1)
