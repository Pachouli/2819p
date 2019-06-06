/*******************************************************************************
 *                              US281A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang       2017-08-30
 *******************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_le_profile.h"
#include "bt_engine_le_adv.h"
#include "bt_engine_le_uuid.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

#if (BT_ENGINE_BLE_TEST_ENABLE == 1)

static void le_computing_passthrough_speed(le_speed_test_st *speed, uint16 send_len);

#if (BLE_TEST_BASE_ENABLE == 1)
static void letest_base_init(void);
void letest_base16_autonotify_periodical_deal(uint32 set_time);
#endif

le_test_env_st le_test_env;

static const uint8 send_char_array[10] =
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

void le_start_test(void)
{
    BLE_DBG_PRINTF("le test start\n");

    sys_adjust_clk(FREQ_222M, SET_LOCK_CLK);

#if (BLE_TEST_PASSTHP_ENABLE == 1)
    if (g_ble_cfg.le_passthp_sp == 1)
    {
        passthp_test_start();
    }
#endif
}

void le_stop_test(void)
{
    BLE_DBG_PRINTF("le test stop\n");

#if (BLE_TEST_PASSTHP_ENABLE == 1)
    if (g_ble_cfg.le_passthp_sp == 1)
    {
        passthp_test_end();
    }
#endif
}


void le_computing_passthrough_speed(le_speed_test_st *speed, uint16 send_len)
{
    uint32 cur_time = sys_get_ab_timer();
    uint32 time_delta;

    if (speed == NULL)
    {
        return;
    }

    speed->send_times++;

    if (speed->last_time == 0)
    {
        speed->last_time = cur_time;
    }

    speed->send_count++;
    //BLE_DBG_PRINTF("test send count %d\n",speed->send_count);

    //BLE_PRINTF("last_time %d,send times %d\n",speed->last_time,speed->send_times);
    if (cur_time - speed->last_time > 60000)
    {
        int speed_value;
        time_delta = cur_time - speed->last_time;
        speed_value = ((speed->send_times) * send_len) / 60;
        //BLE_PRINTF("send %d byte/s",speed_value);
        speed->last_time = cur_time;
        speed->send_times = 0;
        if (speed->cbk != NULL)
        {
            speed->cbk(speed_value);
        }
    }
    else
    {
        if (cur_time < speed->last_time)
        {
            speed->last_time = cur_time;
        }
    }
}

void le_test_loop(void)
{
#if (BLE_TEST_PASSTHP_ENABLE == 1)
    le_passthp_autonotify_periodical_deal(8);
#endif
#if (LETEST_BASE_AUTONITIFY_ENABLE == 1)
    letest_base16_autonotify_periodical_deal(100);
#endif
}

#if (BLE_TEST_BASE_ENABLE == 1)

#define LETEST_CHAR_VALUE_INIT_VALUE  "0011223344"

static void letest_base_callback16(void *context, UINT8 ev, void *arg);

#define LETEST_BASE_16BITS_SERVICE_UUID 0x8800
#define LETEST_BASE_16BITS_WRITE_CHAR_UUID 0x8900
#define LETEST_BASE_16BITS_WRITE_NORSP_CHAR_UUID 0x8901
#define LETEST_BASE_16BITS_SIGNWRITE_UUID 0x8902
#define LETEST_BASE_16BITS_READ_UUID 0x8903
#define LETEST_BASE_16BITS_NOTIFY_UUID 0x8904
#define LETEST_BASE_16BITS_INDICATE_UUID 0x8905


#define LETEST_BASE_128BITS_SERVICE_UUID_STR "CB4E8800-0000-1111-2222-333344445555"
#define LETEST_BASE_128BITS_WRITE_CHAR_STR "CB4E8900-0000-1111-2222-333344445555"
#define LETEST_BASE_128BITS_WRITE_NORSP_CHAR_STR "CB4E8901-0000-1111-2222-333344445555"
#define LETEST_BASE_128BITS_SIGNWRITE_STR "CB4E8902-0000-1111-2222-333344445555"
#define LETEST_BASE_128BITS_READ_STR "CB4E8903-0000-1111-2222-333344445555"
#define LETEST_BASE_128BITS_NOTIFY_STR "CB4E8904-0000-1111-2222-333344445555"
#define LETEST_BASE_128BITS_INDICATE_STR "CB4E8905-0000-1111-2222-333344445555"

static struct le_gatt_attr letest_base_attrs16[] =
{
    //16bits service
    BTSRV_LE_GATT_PRIMARY_SERVICE(LETEST_BASE_16BITS_SERVICE_UUID, UUID_TYPE_16,
            letest_base_callback16),
    //16bits write char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_16BITS_WRITE_CHAR_UUID, UUID_TYPE_16,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_WRITE|ATT_READ)),
    //16bits write with no response char
    // BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_16BITS_WRITE_NORSP_CHAR_UUID, UUID_TYPE_16,
    //         LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_WRITE_NORSP|ATT_READ)),
    //128bits write with no response char
    BTSRV_LE_GATT_CHARACTERISTIC((uint32)LETEST_BASE_128BITS_WRITE_NORSP_CHAR_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_WRITE_NORSP|ATT_READ)),
    //16bits signed write char
    // BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_16BITS_SIGNWRITE_UUID, UUID_TYPE_16,
    //         LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_SIGNWRITE|ATT_READ)),
    //16bits read char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_16BITS_READ_UUID, UUID_TYPE_16,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, ATT_READ),
    //16bits notify char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_16BITS_NOTIFY_UUID, UUID_TYPE_16,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_NOTIFY|ATT_READ)),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(
            GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED),
    //16bits indicate char
    // BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_16BITS_INDICATE_UUID, UUID_TYPE_16,
    //         LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_INDICATE|ATT_READ)),
    //128bits indicate char
    BTSRV_LE_GATT_CHARACTERISTIC((uint32)LETEST_BASE_128BITS_INDICATE_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_INDICATE|ATT_READ)),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(
            GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED),
    BTSRV_LE_GATT_ATTRS_END(),
};

#if (LETEST_128BITS_UUID_SERVICE_ENABLE == 1)

static void letest_base_callback128(void *context, UINT8 ev, void *arg);

static struct le_gatt_attr letest_base_attrs128[] =
{
    //128bits service
    BTSRV_LE_GATT_PRIMARY_SERVICE(LETEST_BASE_128BITS_SERVICE_UUID_STR, UUID_TYPE_128,
            letest_base_callback128),
    //128bits write char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_128BITS_WRITE_CHAR_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_WRITE|ATT_READ)),
    //128bits write with no response char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_128BITS_WRITE_NORSP_CHAR_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_WRITE_NORSP|ATT_READ)),
    //128bits signed write char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_128BITS_SIGNWRITE_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_SIGNWRITE|ATT_READ)),
    //128bits read char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_128BITS_READ_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, ATT_READ),
    //128bits notify char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_128BITS_NOTIFY_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_NOTIFY|ATT_READ)),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(
            GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED),
    //128bits indicate char
    BTSRV_LE_GATT_CHARACTERISTIC(LETEST_BASE_128BITS_INDICATE_STR, UUID_TYPE_128,
            LETEST_CHAR_VALUE_INIT_VALUE,sizeof(LETEST_CHAR_VALUE_INIT_VALUE)+1, (ATT_INDICATE|ATT_READ)),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(
            GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED),

    BTSRV_LE_GATT_ATTRS_END(),
};
#endif

void rx_speed_wr_cbk(int value)
{
    BLE_PRINTF("char wr rx speed is %dbytes/s\n", value);
}

void rx_speed_wrnr_cbk(int value)
{
    BLE_PRINTF("char wrnr rx speed is %dbytes/s\n", value);
}

void tx_speed_ntf_cbk(int value)
{
    BLE_PRINTF("char ntf rx speed is %dbytes/s\n", value);
}

void tx_speed_idc_cbk(int value)
{
    BLE_PRINTF("char idc rx speed is %dbytes/s\n", value);
}

int16 letest_base_get_service_tree(void)
{
    uint16 service_tree_bits_map = 0;
    letest_base_st *p_letest_base = &le_test_env.letest_base16;

    if (p_letest_base->service == NULL)
    {
        BLE_DBG_PRINTF("letest_base service is null\n");
        return service_tree_bits_map;
    }

    service_tree_bits_map |= LETEST_BASE_SERVICE;
    p_letest_base->w_char = le_find_char_in_service(p_letest_base->service,LETEST_BASE_16BITS_WRITE_CHAR_UUID,UUID_TYPE_16);
    if (p_letest_base->w_char != NULL)
    {
        service_tree_bits_map |= LETEST_BASE_WRITE_CHAR;
    }
    p_letest_base->wnr_char = le_find_char_in_service(p_letest_base->service,(uint32)LETEST_BASE_128BITS_WRITE_NORSP_CHAR_STR,UUID_TYPE_128);
    if (p_letest_base->wnr_char != NULL)
    {
        service_tree_bits_map |= LETEST_BASE_WRITE_NORESP_CHAR;
    }
    p_letest_base->nty_char = le_find_char_in_service(p_letest_base->service,LETEST_BASE_16BITS_NOTIFY_UUID,UUID_TYPE_16);
    if (p_letest_base->nty_char != NULL)
    {
        service_tree_bits_map |= LETEST_BASE_NOTIFY_CHAR;
    }
    p_letest_base->nty_char_ccc = le_find_CCC_in_char(p_letest_base->nty_char);
    if (p_letest_base->nty_char_ccc != NULL)
    {
        service_tree_bits_map |= LETEST_BASE_NOTIFY_CHAR_CCC;
    }
    p_letest_base->idc_char = le_find_char_in_service(p_letest_base->service,(uint32)LETEST_BASE_128BITS_INDICATE_STR,UUID_TYPE_128);
    if (p_letest_base->idc_char != NULL)
    {
        service_tree_bits_map |= LETEST_BASE_INDICATE_CHAR;
    }
    p_letest_base->idc_char_ccc = le_find_CCC_in_char(p_letest_base->idc_char);
    if (p_letest_base->idc_char_ccc != NULL)
    {
        service_tree_bits_map |= LETEST_BASE_INDICATE_CHAR_CCC;
    }

    return service_tree_bits_map;
}

void letest_base_auto_notify(void)
{
    bool ret;
    static uint8 idx = 0;
    rmt_ctrl_pkg_t send_data;

    //BLE_DBG_PRINTF("letest_passthp_auto_notify\n");
    if ((g_bteg_le_env.rmt_client.serv_conn & BLE_CONNECTED) != BLE_CONNECTED)
    {
        return;
    }

    send_data.rcp_len = get_cur_rmt_client_mtu();
    send_data.rcp_data = NEW(send_data.rcp_len);

    //BLE_DBG_PRINTF("send data malloc 0x%x\n",send_data.rcp_data);
    if (send_data.rcp_data == NULL)
    {
        BLE_PRINTF("send_data.rcp_data malloc fail\n");
        return;
    }
    libc_memset(send_data.rcp_data, send_char_array[idx], send_data.rcp_len);

    if ((le_test_env.letest_base16.nty_char != NULL) && (le_test_env.letest_base16.notify_enable == GATT_NOTIFY_ENABLE))
    {
        ret = APP_GATT_Server_NotifyIndicate(le_test_env.letest_base16.nty_char->value_hdl, send_data.rcp_data, send_data.rcp_len);
        if (ret == TRUE)
        {
            //add for compute passthrough speed
            le_computing_passthrough_speed(&le_test_env.letest_base16.tx_speed_nty, send_data.rcp_len);
        }
    }

    if ((le_test_env.letest_base16.idc_char != NULL) && (le_test_env.letest_base16.indicate_enable == GATT_NOTIFY_ENABLE))
    {
        ret = APP_GATT_Server_NotifyIndicate(le_test_env.letest_base16.idc_char->value_hdl, send_data.rcp_data, send_data.rcp_len);
        if (ret == TRUE)
        {
            //add for compute passthrough speed
            le_computing_passthrough_speed(&le_test_env.letest_base16.tx_speed_idc, send_data.rcp_len);
        }
    }

    idx++;
    if (idx >= 10)
    {
        idx = 0;
    }

    //BLE_DBG_PRINTF("send data free 0x%x\n",send_data.rcp_data);
    FREE(send_data.rcp_data);
    send_data.rcp_data = NULL;
}

void letest_base16_autonotify_periodical_deal(uint32 set_time)
{
    static uint32 last_time = 0;
    uint32 cur_time = sys_get_ab_timer();
    if (cur_time - last_time > set_time)
    {
        letest_base_auto_notify();
        last_time = cur_time;
    }
    else
    {
        if (last_time > cur_time)
        {
            last_time = cur_time;
        }
    }
}

void letest_base_init(void)
{
    letest_base_st *p_letest_base16 = &le_test_env.letest_base16;

    p_letest_base16->indicate_enable = GATT_NOTIFY_DISABLE;
    p_letest_base16->notify_enable = GATT_NOTIFY_DISABLE;
    p_letest_base16->service = NULL;

    p_letest_base16->rx_speed_wr.last_time = 0;
    p_letest_base16->rx_speed_wr.send_count = 0;
    p_letest_base16->rx_speed_wr.send_times = 0;
    p_letest_base16->rx_speed_wr.cbk = rx_speed_wr_cbk;

    p_letest_base16->rx_speed_wrnr.last_time = 0;
    p_letest_base16->rx_speed_wrnr.send_count = 0;
    p_letest_base16->rx_speed_wrnr.send_times = 0;
    p_letest_base16->rx_speed_wrnr.cbk = rx_speed_wrnr_cbk;

    p_letest_base16->tx_speed_nty.last_time = 0;
    p_letest_base16->tx_speed_nty.send_count = 0;
    p_letest_base16->tx_speed_nty.send_times = 0;
    p_letest_base16->tx_speed_nty.cbk = tx_speed_ntf_cbk;

    p_letest_base16->tx_speed_idc.last_time = 0;
    p_letest_base16->tx_speed_idc.send_count = 0;
    p_letest_base16->tx_speed_idc.send_times = 0;
    p_letest_base16->tx_speed_idc.cbk = tx_speed_idc_cbk;
}


void letest_base_test_start(void)
{
    APP_LE_REGISTER_SERVICE(letest_base_attrs16);
#if (LETEST_128BITS_UUID_SERVICE_ENABLE == 1)
    APP_LE_REGISTER_SERVICE(letest_base_attrs128);
#endif
}

static void print_letest_base_rcv_data(uint8 *data, uint16 data_len)
{
    libc_printhex(data, (data_len > 32) ? 32 : data_len, 0);
}

bool letest_base16_pend_ind_ev_deal(struct ATT_PendIndStru *in)
{
    bool ret = FALSE;

    letest_base_st *p_letest_base16 = &le_test_env.letest_base16;
    struct GATT_ServiceStru *cur_service = p_letest_base16->service;

    if (cur_service == NULL)
    {
        BLE_PRINTF("letest base16 service is null\n");
        return ret;
    }

    struct ATT_HandleValueStru *it;

    BLE_DBG_PRINTF("PEND_IND 0x%x", in->op)
    it = &in->it;

    //write
    if ((in->op == WRITE_REQ) || (in->op == WRITE_CMD) /* ||(in->op == PREPARE_WRITE_REQ)*/)
    {
        //BLE_DBG_PRINTF("it_hdl %d,idc char %p,idc hdl %d\n",in->it.hdl,p_letest_base16->nty_char,p_letest_base16->nty_char_ccc->hdl);
        if (in->it.hdl == p_letest_base16->nty_char_ccc->hdl)
        {
            if ((it->val.value[0] & (uint8) GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED)
                    == GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED)
            {
                BLE_DBG_PRINTF("nty enable\n");
                p_letest_base16->notify_enable = GATT_NOTIFY_ENABLE;
            }
            else
            {
                BLE_DBG_PRINTF("nty disable\n");
                p_letest_base16->notify_enable = GATT_NOTIFY_DISABLE;
            }
        }
        else if (in->it.hdl == p_letest_base16->idc_char_ccc->hdl)
        {
            BLE_DBG_PRINTF("idc hdl %d\n",p_letest_base16->idc_char_ccc->hdl);
            if ((it->val.value[0] & (uint8) GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED)
                == GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED)
            {
                BLE_DBG_PRINTF("idc enable\n");
                p_letest_base16->indicate_enable = GATT_NOTIFY_ENABLE;
            }
            else
            {
                BLE_DBG_PRINTF("idc disable\n");
                p_letest_base16->indicate_enable = GATT_NOTIFY_DISABLE;
            }
        }
        else if (in->it.hdl == p_letest_base16->w_char->value_hdl)
        {
            BLE_PRINTF("letest base16 write char rcv %d,data:", in->it.val.len);
            print_letest_base_rcv_data(in->it.val.value, in->it.val.len);
            le_computing_passthrough_speed(&p_letest_base16->rx_speed_wr,in->it.val.len);
        }
        else if (in->it.hdl == p_letest_base16->wnr_char->value_hdl)
        {
            //BLE_PRINTF("letest base16 writenoresp char rcv %d,data:", in->it.val.len);
            //print_letest_base_rcv_data(in->it.val.value, in->it.val.len);
            le_computing_passthrough_speed(&p_letest_base16->rx_speed_wrnr,in->it.val.len);
        }
    }

    ret = TRUE;
    return ret;
}

static void letest_base_callback16(void *context, UINT8 ev, void *arg)
{
    switch (ev)
    {
        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
            letest_base16_pend_ind_ev_deal(arg);
            GATT_Server_PendIND_Response(arg);
            break;

        //GATT注册服务完成事件(返回HDL)
        case GATT_EV_HDL_TREE: /* Local is server */
            {
                //注册本地的service时会返回，AP记录下来
                BLE_DBG_PRINTF("letest base 0 HDL TREE\n");
                struct GATT_ServiceStru *service = arg;
                letest_base_init();
                le_test_env.letest_base16.service = arg;
                BLE_DBG_PRINTF("letest base service 0 hdl : %d~%d\n",service->hdl,service->endhdl);
                BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg);
                le_test_env.letest_base16.svctree_bmap = letest_base_get_service_tree();
                BLE_DBG_PRINTF("letest_base16 svctree bmap 0x%x\n",le_test_env.letest_base16.svctree_bmap);
                if (le_test_env.letest_base16.svctree_bmap == 0)
                {
                    BLE_DBG_PRINTF("letest_base_get_service_tree err\n");
                    while(1)
                    {
                        ;
                    }
                }
            }
            break;

        default:
            APP_GATT_Callback(context, ev, arg);
            break;
    }
}

#if (LETEST_128BITS_UUID_SERVICE_ENABLE == 1)

static void letest_base_callback128(void *context, UINT8 ev, void *arg)
{
    switch (ev)
    {
        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
            GATT_Server_PendIND_Response(arg);
            break;

        //GATT注册服务完成事件(返回HDL)
        case GATT_EV_HDL_TREE: /* Local is server */
            {
                //注册本地的service时会返回，AP记录下来
                BLE_DBG_PRINTF("letest base 0 HDL TREE\n");
                struct GATT_ServiceStru *service = arg;
                BLE_DBG_PRINTF("letest base service 0 hdl : %d~%d\n",service->hdl,service->endhdl);
                BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg);
            }
            break;

        default:
            APP_GATT_Callback(context, ev, arg);
            break;
    }
}

#endif
#endif

#if (BLE_TEST_PASSTHP_ENABLE == 1)
//demo of 128bit uuid
const uint8 TEST_PASSP_SERVICE_UUID_128BITS[UUID_STR_LEN] = "12340000-2345-3456-4567-56789abcdeff"; //service uuid
const uint8 TEST_PASSP_SETCHAR_UUID_128BITS[UUID_STR_LEN] = "12340001-2345-3456-4567-56789abcdeff"; //service tx uuid
const uint8 TEST_PASSP_GETCHAR_UUID_128BITS[UUID_STR_LEN] = "12340002-2345-3456-4567-56789abcdeff"; //service rx uuid

//demo of 16bit uuid
#define TEST_PASSP_SERVICE_UUID_16BITS 0x5550 //service uuid
#define TEST_PASSP_SETCHAR_UUID_16BITS 0x5500 //service tx uuid
#define TEST_PASSP_GETCHAR_UUID_16BITS 0x5501 //service rx uuid

void letest_passthp_auto_notify(void)
{
    bool ret;
    static uint8 idx = 0;
    rmt_ctrl_pkg_t send_data;

    if (le_test_env.passthp.op_hdl == 0)
    {
        return;
    }

    //BLE_DBG_PRINTF("letest_passthp_auto_notify\n");

    send_data.rcp_len = get_cur_rmt_client_mtu();
    send_data.rcp_data = NEW(send_data.rcp_len);

    //BLE_DBG_PRINTF("send data malloc 0x%x\n",send_data.rcp_data);
    if (send_data.rcp_data == NULL)
    {
        BLE_PRINTF("send_data.rcp_data malloc fail\n");
        return;
    }
    libc_memset(send_data.rcp_data, send_char_array[idx], send_data.rcp_len);

    ret = passthp_send(le_test_env.passthp.op_hdl, send_data.rcp_data, send_data.rcp_len);
    if (ret == TRUE)
    {
        //add for compute passthrough speed
        le_computing_passthrough_speed(&le_test_env.passthp.rx_speed, send_data.rcp_len);
        idx++;
        if (idx >= 10)
        {
            idx = 0;
        }
    }

    //BLE_DBG_PRINTF("send data free 0x%x\n",send_data.rcp_data);
    FREE(send_data.rcp_data);
    send_data.rcp_data = NULL;
}

//#define CREATE_PASSTH_SERVICE_SET_UUID_TEST

#ifdef CREATE_PASSTH_SERVICE_SET_UUID_TEST
static struct passthp_reg_uuid_info_st * get_passp_test_create_service_param(void)
{
    struct passthp_reg_uuid_info_st *param;

    param = NEW(sizeof(ble_passthp_create_param_st));
    if (param == NULL)
    {
        return NULL;
    }

    //demo of 16bit uuid
    param->service_uuid_str.uuid = (uint32 *)TEST_PASSP_SERVICE_UUID_16BITS;
    param->service_uuid_str.uuid_type = UUID_TYPE_16;

    param->setchar_uuid_str.uuid = (uint32 *)TEST_PASSP_SETCHAR_UUID_16BITS;
    param->setchar_uuid_str.uuid_type = UUID_TYPE_16;

    param->getchar_uuid_str.uuid = (uint32 *)TEST_PASSP_GETCHAR_UUID_16BITS;
    param->getchar_uuid_str.uuid_type = UUID_TYPE_16;

//    //demo of 128bit uuid
//    param->service_uuid_str.uuid = NEW(UUID_STR_LEN+1);
//    if (param->service_uuid_str.uuid == NULL)
//    {
//        FREE(param);
//        return NULL;
//    }
//    libc_memcpy(param->service_uuid_str.uuid,TEST_PASSP_SERVICE_UUID_128BITS,UUID_STR_LEN);
//    param->service_uuid_str.uuid[UUID_STR_LEN] = 0;
//    param->service_uuid_str.uuid_type = UUID_TYPE_128;
//
//    param->setchar_uuid_str.uuid = NEW(UUID_STR_LEN+1);
//    if (param->setchar_uuid_str.uuid == NULL)
//    {
//        FREE(param->service_uuid_str.uuid);
//        FREE(param);
//        return NULL;
//    }
//    libc_memcpy(param->setchar_uuid_str.uuid,TEST_PASSP_SETCHAR_UUID_128BITS,UUID_STR_LEN);
//    param->setchar_uuid_str.uuid[UUID_STR_LEN] = 0;
//    param->setchar_uuid_str.uuid_type = UUID_TYPE_128;
//
//    param->getchar_uuid_str.uuid = NEW(UUID_STR_LEN+1);
//    if (param->getchar_uuid_str.uuid == NULL)
//    {
//        FREE(param->service_uuid_str.uuid);
//        FREE(param->setchar_uuid_str.uuid);
//        FREE(param);
//        return NULL;
//    }
//    libc_memcpy(param->getchar_uuid_str.uuid,TEST_PASSP_GETCHAR_UUID_128BITS,UUID_STR_LEN);
//    param->getchar_uuid_str.uuid[UUID_STR_LEN] = 0;
//    param->getchar_uuid_str.uuid_type = UUID_TYPE_128;

    return param;
}

static bool get_passp_test_free_service_param(struct passthp_reg_uuid_info_st *param)
{
    if ((param->service_uuid_str.uuid != NULL) && (param->service_uuid_str.uuid_type == UUID_TYPE_128))
    {
        FREE(param->service_uuid_str.uuid);
    }
    if ((param->setchar_uuid_str.uuid != NULL) && (param->setchar_uuid_str.uuid_type == UUID_TYPE_128))
    {
        FREE(param->setchar_uuid_str.uuid);
    }
    if ((param->getchar_uuid_str.uuid != NULL) && (param->getchar_uuid_str.uuid_type == UUID_TYPE_128))
    {
        FREE(param->getchar_uuid_str.uuid);
    }
    if (param != NULL)
    {
        FREE(param);
    }
}
#endif //end of #ifdef CREATE_PASSTH_SERVICE_SET_UUID_TEST

void rx_speed_test_cbk(int value)
{
    BLE_PRINTF("rx speed is %dbytes/s\n", value);
}

void tx_speed_test_cbk(int value)
{
    BLE_PRINTF("tx speed is %dbytes/s\n", value);
}

void le_test_passthp_event_cbk(uint32 hdl, uint8 ev, void *arg)
{
    switch (ev)
    {
        case PASSTHP_REGISTER_COMPLETE:
        {
            if (arg == NULL)
            {
                break;
            }

            le_test_env.passthp.op_hdl = (uint32) arg;
        }
        break;
        case PASSTHP_RMT_RXSTA_CHANGE:
        {
            if ((uint32) arg == PASSTHP_RMT_RX_ENABLE)
            {
                BLE_DBG_PRINTF("rmt enable rx\n");
                le_test_env.passthp.notify_enable = TRUE;
                //le_notify_reset();

            }
            else
            {
                BLE_DBG_PRINTF("rmt disable rx\n");
                le_test_env.passthp.notify_enable = FALSE;
            }

        }
        break;
        case PASSTHP_RMT_TXDATA_IND:
        {
            uint8 *rdata;
            receive_rcp_info_t *p_rcv_info = (receive_rcp_info_t *) arg;
            rdata = NEW(p_rcv_info->rcv_len);
            if (passthp_read(le_test_env.passthp.op_hdl, rdata, p_rcv_info->rcv_len) == TRUE)
            {
                le_computing_passthrough_speed(&le_test_env.passthp.rx_speed, p_rcv_info->rcv_len);
                //BLE_DBG_PRINTF("rcv data :");
                //libc_printhex(rdata,p_rcv_info->rcv_len,0);
                rdata[0] = 'r';
                if (le_test_env.passthp.notify_enable == TRUE)
                {
                    if (passthp_send(le_test_env.passthp.op_hdl, rdata, p_rcv_info->rcv_len) == TRUE)
                    {
                        le_computing_passthrough_speed(&le_test_env.passthp.tx_speed, p_rcv_info->rcv_len);
                    }
                }
            }

            if (p_rcv_info->c_loss != 0)
            {
                BLE_DBG_PRINTF("t_loss %d,c_loss %d\n", p_rcv_info->t_loss, p_rcv_info->c_loss);
            }

            FREE(rdata);
        }
        break;
        default:
        break;
    }
}

#define BLE_TEST_BUF_LEN    1024

void passthp_test_start(void)
{
    le_test_env.passthp.ble_test_buf.raw_data = NEW(BLE_TEST_BUF_LEN);
    le_test_env.passthp.ble_test_buf.capacity = BLE_TEST_BUF_LEN;
    le_test_env.passthp.ble_test_buf.length = 0;
    le_test_env.passthp.ble_test_buf.read_ptr = 0;
    le_test_env.passthp.ble_test_buf.write_ptr = 0;

    if (sys_shm_creat(SHARE_MEM_ID_BLESPP_INPIPE2, &le_test_env.passthp.ble_test_buf, sizeof(buffer_rw_t)) == -1)
    {
        BLE_PRINTF_ERROR("ble_test_buf shm create fail\n");
        while (1)
            ;
    }
#ifdef CREATE_PASSTH_SERVICE_SET_UUID_TEST
    //测试使用自定义uuid定义passth profile
    //ble_passthp_create_param_st passp_param;
    //passp_param.reg_uuid = get_passp_test_create_service_param();
    // passp_param.shm_id = 0;
    // passp_param.cbk = le_test_passthp_event_cbk;
    //passthrough_profile_register(&passp_param);
    //get_passp_test_free_service_param(passp_param.reg_uuid);
    //passp_param.reg_uuid = NULL;
#else
    le_test_env.passthp.op_hdl = 0;
    le_test_env.passthp.notify_enable = FALSE;
    ble_passthp_create_param_st passp_param;
    passp_param.shm_id = SHARE_MEM_ID_BLESPP_INPIPE2;
    passp_param.cbk = le_test_passthp_event_cbk;
    passp_param.reg_uuid = NULL;
    //初始化接收速度测试
    le_test_env.passthp.rx_speed.cbk = rx_speed_test_cbk;
    le_test_env.passthp.rx_speed.last_time = 0;
    le_test_env.passthp.rx_speed.send_count = 0;
    le_test_env.passthp.rx_speed.send_times = 0;
    //初始化发送速度测试
    le_test_env.passthp.tx_speed.last_time = 0;
    le_test_env.passthp.tx_speed.cbk = tx_speed_test_cbk;
    le_test_env.passthp.tx_speed.send_count = 0;
    le_test_env.passthp.tx_speed.send_times = 0;
    //从配置项获取信息进行默认创建
    passthrough_profile_register(&passp_param);
    //BLE_DBG_PRINTF("PASSP Service create complete\n");
#endif //end of #ifdef CREATE_PASSTH_SERVICE_SET_UUID_TEST
}

void passthp_test_end(void)
{
    passthrough_profile_unregister(le_test_env.passthp.op_hdl);
    sys_shm_destroy(SHARE_MEM_ID_BLESPP_INPIPE2);

    if (le_test_env.passthp.ble_test_buf.raw_data != NULL)
    {
        FREE(le_test_env.passthp.ble_test_buf.raw_data);
        le_test_env.passthp.ble_test_buf.raw_data = NULL;
    }
    libc_memset(&le_test_env.passthp.ble_test_buf, 0x0, sizeof(buffer_rw_t));

    le_test_env.passthp.notify_enable = FALSE;
    le_test_env.passthp.op_hdl = 0;
}


void le_passthp_autonotify_periodical_deal(uint32 set_time)
{
    static uint32 last_time = 0;
    uint32 cur_time = sys_get_ab_timer();
    if (cur_time - last_time > set_time)
    {
        letest_passthp_auto_notify();
        last_time = cur_time;
    }
    else
    {
        if (last_time > cur_time)
        {
            last_time = cur_time;
        }
    }
}
#endif //end of #if (BLE_TEST_PASSTHP_ENABLE == 1)

#ifdef NEW_ADV_INTERFACT_TEST
bool app_user_advertiser_info_set(void)
{
    bool ret = FALSE;
    void *adv_info_list;
    adv_info_list = app_new_adv_info();
    if (adv_info_list == NULL)
    {
        return ret;
    }

    //set eir data type flags
    uint8 adv_datatype_flag;
    adv_datatype_flag = HCI_EIR_FLAGS_LE_GENERAL_DISCOVERABLE_MODE | (UINT8)HCI_EIR_FLAGS_LE_AND_BREDR_TO_SAME_DEVICE_CAPABLE_CONTROLLER;
    app_set_ble_adv_data(adv_info_list, (void *)(adv_datatype_flag & 0x000000ff), 1, HCI_EIR_DATATYPE_FLAGS);

    //set user uuid
    if (g_bteg_le_env.act_profile != NULL)
    {
        uint16 uuid_16bits;
        uint8 uuid_str[2];
        uuid_16bits = get_16bit_uuid_by_string(get_act_profile_server_uuid(g_bteg_le_env.act_profile));
        ENCODE2BYTE_LITTLE(uuid_str, uuid_16bits);
        app_set_ble_adv_data(adv_info_list, uuid_str, sizeof(uint16), HCI_EIR_DATATYPE_COMPLETE_LIST_OF_16BIT_SERVICE_CLASS_UUIDS);
    }

    //set appearence
    uint8 appearence[2];
    ENCODE2BYTE_LITTLE(appearence, (UINT16)GATT_APPEARANCE_CATEGORY_ACT_RCP_ISO_APK);
    app_set_ble_adv_data(adv_info_list, appearence, 2, HCI_EIR_DATATYPE_FLAGS);

    //set manufacturerdata
    eir_manufacturer_data_st manufacturerdata;

    bt_manager_ble_cfg_t *p_ble_cfg = get_ble_cfg_info();
    manufacturerdata.company_identifier_code[0] = (p_ble_cfg->le_manufacture_id & 0xff00) >> 8;
    manufacturerdata.company_identifier_code[1] = (p_ble_cfg->le_manufacture_id & 0xff);
    libc_memcpy(manufacturerdata.ble_addr, g_btengine_var.device_addr.bytes, BD_ADDR_LEN);
    app_set_ble_adv_data(adv_info_list, (void *)&manufacturerdata, sizeof(eir_manufacturer_data_st), HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA);

    ret = app_set_ble_adv_data_complete(adv_info_list);
    if (ret == FALSE)
    {
        BLE_PRINTF("ble adv set fail\n");
    }

    FREE(adv_info_list);
    adv_info_list = NULL;

    return ret;
}
#endif //endif of #if NEW_ADV_INTERFAC＿TEST

#endif //end of #if (BT_ENGINE_BLE_TEST_ENABLE == 1)
#endif //end of #ifdef BT_SUPPORT_BLE
