/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙BLE或SPP数传接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_bt_inner.h"

uint8 __section__(".bss_blespp_recv_cache1") blespp_recv_cache1[BLESPP_RECV_CACHE1_LEN];
uint8 __section__(".bss_blespp_recv_cache2") blespp_recv_cache2[BLESPP_RECV_CACHE2_LEN];

static buffer_rw_t s_blespp_recv_buffer1;
static buffer_rw_t s_blespp_recv_buffer2;

typedef struct
{
    bt_passth_link_type ltype;
    buffer_rw_t * buf;
} passth_recv_t;

static const passth_recv_t all_passth_recv[2] =
{
    {BT_PASSTH_BLE, &s_blespp_recv_buffer2},
    {BT_PASSTH_SPP, &s_blespp_recv_buffer1},
};

uint32 ble_passth_handle;
bool ble_passth_available;
bool ble_passth_notify_enable;

uint32 spp_passth_handle;

static uint8 s_blespp_select; //0表示BLE，1表示SPP

static data_trans_dispatch_func s_data_trans_dispatch;

bool bt_manager_blespp_data_trans_init(void)
{
    libc_memset(&s_blespp_recv_buffer1, 0x00, sizeof(buffer_rw_t));
    s_blespp_recv_buffer1.raw_data = blespp_recv_cache1;
    s_blespp_recv_buffer1.capacity = sizeof(blespp_recv_cache1);

    if (sys_shm_creat(SHARE_MEM_ID_BLESPP_INPIPE1, &s_blespp_recv_buffer1, sizeof(buffer_rw_t)) == -1)
    {
        while (1);
    }

    libc_memset(&s_blespp_recv_buffer2, 0x00, sizeof(buffer_rw_t));
    s_blespp_recv_buffer2.raw_data = blespp_recv_cache2;
    s_blespp_recv_buffer2.capacity = sizeof(blespp_recv_cache2);

    if (sys_shm_creat(SHARE_MEM_ID_BLESPP_INPIPE2, &s_blespp_recv_buffer2, sizeof(buffer_rw_t)) == -1)
    {
        while (1);
    }

    return TRUE;
}

bool bt_manager_blespp_data_trans_exit(void)
{
    sys_shm_destroy (SHARE_MEM_ID_BLESPP_INPIPE1);
    sys_shm_destroy (SHARE_MEM_ID_BLESPP_INPIPE2);

    return TRUE;
}

static void ble_passthp_event_cbk(uint32 hdl, uint8 ev, void *arg)
{
    switch (ev)
    {
        case PASSTHP_REGISTER_COMPLETE:
        {
            if (arg == NULL)
            {
                break;
            }

            libc_printf("create ble passthp ok\n");
            ble_passth_available = TRUE;
        }
        break;
        case PASSTHP_RMT_RXSTA_CHANGE:
        {
            if ((uint32) arg == PASSTHP_RMT_RX_ENABLE)
            {
                libc_printf("rmt enable rx\n");
                ble_passth_notify_enable = TRUE;
            }
            else
            {
                libc_printf("rmt disable rx\n");
                ble_passth_notify_enable = FALSE;
            }
        }
        break;
        case PASSTHP_RMT_TXDATA_IND:
        {

        }
        break;
        default:
        break;
    }
}

bool bt_manager_blespp_data_trans_create_ble_profile(void)
{
    msg_apps_t msg;
    bt_managet_create_passth_param_st passth_param;
    ble_passthp_create_param_st ble_passthp_param;

    //创建数据透传profile
    ble_passthp_param.reg_uuid = NULL;
    ble_passthp_param.cbk = ble_passthp_event_cbk;
    ble_passthp_param.shm_id = SHARE_MEM_ID_BLESPP_INPIPE2;

    msg.type = MSG_BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC;
    passth_param.hdl = &ble_passth_handle;
    passth_param.arg = &ble_passthp_param;
    msg.content.addr = &passth_param;

    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    libc_printf("create ble passthp, handle:0x%x\n", ble_passth_handle);

    return TRUE;
}

bool bt_manager_blespp_data_trans_destroy_ble_profile(void)
{
    msg_apps_t msg;

    //注销数据透传profile
    msg.type = MSG_BTENGINE_DELETE_BLE_PASSTHROUGH_SYNC;
    msg.content.addr = ble_passth_handle;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    ble_passth_available = FALSE;
    ble_passth_notify_enable = FALSE;

    return TRUE;
}

static void spp_passthp_event_cbk(uint32 hdl, uint8 ev, void *arg)
{

}

bool bt_manager_blespp_data_trans_create_spp_profile(void)
{
    msg_apps_t msg;
    bt_managet_create_passth_param_st passth_param;
    spp_reg_param_st spp_reg_param;

    //创建数据透传profile
    spp_reg_param.cbk = spp_passthp_event_cbk;
    spp_reg_param.shm_id = SHARE_MEM_ID_BLESPP_INPIPE1;

    msg.type = MSG_BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC;
    passth_param.hdl = &spp_passth_handle;
    passth_param.arg = &spp_reg_param;
    msg.content.addr = &passth_param;

    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    libc_printf("create spp passthp, handle:0x%x\n", spp_passth_handle);

    return TRUE;
}

bool bt_manager_blespp_data_trans_destroy_spp_profile(void)
{
    msg_apps_t msg;

    //注销数据透传profile
    msg.type = MSG_BTENGINE_DELETE_SPP_PASSTHROUGH_SYNC;
    msg.content.addr = spp_passth_handle;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return TRUE;
}

bool bt_manager_blespp_data_trans_register_dispatch_callback_inner(data_trans_dispatch_func f_data_trans_dispatch)
{
    s_data_trans_dispatch = f_data_trans_dispatch;
    return TRUE;
}

bool bt_manager_blespp_data_trans_deal_loop(void)
{
    if (s_data_trans_dispatch != NULL)
    {
        s_data_trans_dispatch(all_passth_recv[0].ltype, all_passth_recv[0].buf);
        s_data_trans_dispatch(all_passth_recv[1].ltype, all_passth_recv[1].buf);

        return TRUE;
    }

    return FALSE;
}

bool bt_manager_blespp_data_trans_send_inner(bt_passth_link_type ltype, uint8* p_data, uint16 data_len)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;
    bt_managet_data_wr_st data_wr;

    if (ltype == BT_PASSTH_BLE)
    {
        if ((ble_passth_available == FALSE) || (ble_passth_notify_enable == FALSE))
        {
            return FALSE;
        }

        data_wr.hdl = ble_passth_handle;
        data_wr.data = p_data;
        data_wr.len = data_len;

        msg.type = MSG_BTENGINE_BLE_PASSTHROUGH_DATA_SEND_SYNC;
        msg.content.addr = &data_wr;
        send_sync_msg(APP_ID_BTENGINE, &msg, &temp_reply, 0);
    }
    else
    {
        data_wr.hdl = spp_passth_handle;
        data_wr.data = p_data;
        data_wr.len = data_len;

        msg.type = MSG_BTENGINE_SPP_PASSTHROUGH_DATA_SEND_SYNC;
        msg.content.addr = &data_wr;
        send_sync_msg(APP_ID_BTENGINE, &msg, &temp_reply, 0);
    }

    return TRUE;
}
