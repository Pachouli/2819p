/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_passthrough_profile.h"
#include "bt_engine_le_uuid.h"
#include "bt_engine_le_test.h"
#include "bt_engine_passthrough.h"
#include "./../bt_engine_ble/bt_engine_le_server.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

static void get_param_uuid(uint32 *out_uuid, uint8 *out_uuid_type, struct uuid_string_st *in_uuid_str);
static bool passthp_serv_reg_tree(uint32 hdl, struct GATT_ServiceStru *service);
static void passthp_gatt_event_cbk(void *context, UINT8 ev, void *arg);
static void app_callback(uint32 hdl, uint8 ev, void *arg);
static bool passthp_pend_ind_ev_deal(uint32 hdl, struct ATT_PendIndStru *in);
static void passthp_gatt_event_cbk0(void *context, UINT8 ev, void *arg);
static void passthp_gatt_event_cbk1(void *context, UINT8 ev, void *arg);
static void gatt_event_cbk(uint32 hdl, void *context, UINT8 ev, void *arg);

passth_profile_st *passthp[MAX_PROFILE_INSTANCE];

static struct le_gatt_attr passthp_attrs[] =
{
BTSRV_LE_GATT_PRIMARY_SERVICE(0, UUID_TYPE_128, passthp_gatt_event_cbk0),
BTSRV_LE_GATT_CHARACTERISTIC(0, UUID_TYPE_128, NULL, DEFAULT_ATT_MTU - 3,
        (ATT_READ | ATT_NOTIFY | ATT_EXTENDED_PROPERTIES)),
BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED),
BTSRV_LE_GATT_CHARACTERISTIC(0, UUID_TYPE_128, NULL, DEFAULT_ATT_MTU - 3,
        (ATT_READ | ATT_ALLWRITE)),
BTSRV_LE_GATT_ATTRS_END(), };

const uint32 passthp_gatt_ev_cbk_list[MAX_PROFILE_INSTANCE] =
{ (uint32) passthp_gatt_event_cbk0, (uint32) passthp_gatt_event_cbk1, };

/**
 * 获取passthp空句柄
 * 返回值:
 *      0~MAX_PROFILE_INSTANCE：找到的空句柄
 *      0xff：没有找到空句柄
 */
static uint8 get_empty_passthp_hdl(void)
{
    uint8 i;

    for (i = 0; i < MAX_PROFILE_INSTANCE; i++)
    {
        if (passthp[i] == NULL)
        {
            break;
        }
    }
    if (i >= MAX_PROFILE_INSTANCE)
    {
        return EMPTY_HDL;
    }

    return IDXTOHDL(i);
}

static passth_profile_st * get_passthp_by_hdl(uint8 hdl)
{
    return passthp[HDLTOIDX(hdl)];
}

int32 passthrough_profile_register(ble_passthp_create_param_st *param)
{
    int32 hdl = 0;
    struct le_gatt_attr *p_attr;
    passth_profile_st *p_passthp;

    hdl = get_empty_passthp_hdl();
    if (hdl == EMPTY_HDL)
    {
        BLE_PRINTF_ERROR("passthp reach max instance\n");
        return hdl;
    }

    if ((param->cbk == NULL) || (param->shm_id == 0))
    {
        BLE_PRINTF_ERROR("param err\n");
        return ERR_HDL;
    }

    p_passthp = NEW(sizeof(passth_profile_st));
    if (p_passthp == NULL)
    {
        BLE_PRINTF_ERROR("passthp malloc failed\n");
        return ERR_HDL;
    }

    BLE_DBG_PRINTF("hdl %d idx %d\n", hdl, HDLTOIDX(hdl));
    passthp[HDLTOIDX(hdl)] = p_passthp;
    libc_memset(p_passthp, 0x0, sizeof(passth_profile_st));

    p_passthp->attr = NEW(sizeof(passthp_attrs));
    if (p_passthp->attr == NULL)
    {
        BLE_PRINTF_ERROR("passthp attr malloc failed\n");
        return ERR_HDL;
    }

    BLE_DBG_PRINTF("sizeof passthp_attrs is %d\n", sizeof(passthp_attrs));
    libc_memcpy(p_passthp->attr, passthp_attrs, sizeof(passthp_attrs));
    p_attr = p_passthp->attr;

    if (param->reg_uuid != NULL)
    {
        get_param_uuid(&p_attr[0].uuid, &p_attr[0].uuid_type, &param->reg_uuid->service_uuid_str);
        get_param_uuid(&p_attr[1].uuid, &p_attr[1].uuid_type, &param->reg_uuid->setchar_uuid_str);
        get_param_uuid(&p_attr[3].uuid, &p_attr[3].uuid_type, &param->reg_uuid->getchar_uuid_str);
    }
    else
    {
        //若param为空，则从配置项中获取uuid信息
        bt_manager_ble_cfg_t *p_ble_cfg = get_ble_cfg_info();

        p_attr[0].uuid = get_16bit_uuid_by_string(p_ble_cfg->act_serv_uuid);
        p_attr[0].uuid_type = UUID_TYPE_16;
        p_attr[1].uuid = get_16bit_uuid_by_string(p_ble_cfg->act_set_uuid);
        p_attr[1].uuid_type = UUID_TYPE_16;
        p_attr[3].uuid = get_16bit_uuid_by_string(p_ble_cfg->act_get_uuid);
        p_attr[3].uuid_type = UUID_TYPE_16;
    }

    p_attr[0].cbk = passthp_gatt_ev_cbk_list[HDLTOIDX(hdl)];

    p_passthp->hdl = hdl;
    p_passthp->cbk = param->cbk;
    p_passthp->notify_enable = PASSTHP_RMT_RX_DISABLE;
    p_passthp->rcv_buffer = (buffer_rw_t *) sys_shm_mount(param->shm_id);

    APP_LE_REGISTER_SERVICE(p_attr);

    return hdl;
}

bool passthrough_profile_unregister(uint32 p_hdl)
{
    struct le_gatt_attr *p_attr;
    passth_profile_st *p_passthp;

    p_passthp = get_passthp_by_hdl(p_hdl);

    if (p_passthp != NULL)
    {
        if (p_passthp->serv != NULL)
        {
            GATT_Server_UnRegTree(p_passthp->serv->hdl);
            GATT_Free_Service(p_passthp->serv);
        }

        p_attr = p_passthp->attr;
        if ((p_attr[0].uuid_type == UUID_TYPE_128) && (p_attr[0].uuid != (uint32) NULL))
        {
            FREE(p_attr[0].uuid);
            p_attr[0].uuid = 0;
        }
        if ((p_attr[1].uuid_type == UUID_TYPE_128) && (p_attr[1].uuid != (uint32) NULL))
        {
            FREE(p_attr[1].uuid);
            p_attr[1].uuid = 0;
        }
        if ((p_attr[3].uuid_type == UUID_TYPE_128) && (p_attr[3].uuid != (uint32) NULL))
        {
            FREE(p_attr[3].uuid);
            p_attr[3].uuid = 0;
        }

        FREE(p_passthp->attr);
        p_passthp->attr = NULL;

        FREE(p_passthp);
        passthp[HDLTOIDX(p_hdl)] = NULL;
    }

    return TRUE;
}

void passthp_gatt_event_cbk0(void *context, UINT8 ev, void *arg)
{
    gatt_event_cbk(IDXTOHDL(0), context, ev, arg);
}

void passthp_gatt_event_cbk1(void *context, UINT8 ev, void *arg)
{
    gatt_event_cbk(IDXTOHDL(1), context, ev, arg);
}

void gatt_event_cbk(uint32 hdl, void *context, UINT8 ev, void *arg)
{
    switch (ev)
    {
        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
        passthp_pend_ind_ev_deal(hdl, (struct ATT_PendIndStru *) arg);
        break;

        //GATT注册服务完成事件(返回HDL)
        case GATT_EV_HDL_TREE: /* Local is server */
        //注册本地的service时会返回，AP记录下来
        if (passthp_serv_reg_tree(hdl, arg) == FALSE)
        {
            passthrough_profile_unregister(hdl);
        }
        le_update_rmt_client_conn_par(FAST_CONNECTION_INTERVAL_MIN, FAST_CONNECTION_INTERVAL_MAX,
        FAST_CONNECTION_SLAVE_LATENCY, FAST_CONNECTION_TIMEOUT_MULTIPLIER);
        break;

        default:
        APP_GATT_Callback(context, ev, arg);
        break;
    }
}

bool passthp_send(uint32 hdl, uint8 *wdata, uint16 len)
{
    if (hdl == 0)
    {
        return FALSE;
    }

    if ((wdata == NULL) || (len == 0))
    {
        return FALSE;
    }

    passth_profile_st *p_passthp = get_passthp_by_hdl(hdl);
    if ((p_passthp == NULL) || (p_passthp->set_char == NULL) || (p_passthp->set_char->value_hdl == 0))
    {
        BLE_PRINTF_ERROR("set char not exits\n");
        return FALSE;
    }

    return APP_GATT_Server_NotifyIndicate(p_passthp->set_char->value_hdl,wdata,len);
}

bool passthp_read(uint32 hdl, uint8 *rdata, uint16 len)
{
    if (hdl == 0)
    {
        return FALSE;
    }

    if ((rdata == NULL) || (len == 0))
    {
        BLE_PRINTF_ERROR("passthp read param err\n");
        return FALSE;
    }

    passth_profile_st *p_passthp = get_passthp_by_hdl(hdl);
    if ((p_passthp == NULL) || (p_passthp->rcv_buffer == NULL))
    {
        BLE_PRINTF_ERROR("passthp or rcv_buffer is null\n");
        return FALSE;
    }

    if (buffer_rw_read(p_passthp->rcv_buffer,rdata,len) == FALSE)
    {
        rdata = 0;
        return FALSE;
    }
    return TRUE;

}

void app_callback(uint32 hdl, uint8 ev, void *arg)
{
    passth_profile_st *p_passthp = get_passthp_by_hdl(hdl);

    if (p_passthp->cbk)
    {
        p_passthp->cbk(hdl, ev, arg);
    }
}

static void print_receive(uint8 *data, uint16 data_len)
{
    libc_printf("passthp recv: %d\n", data_len);
    libc_printhex(data, (data_len > 32) ? 32 : data_len, 0);
}

bool passthp_pend_ind_ev_deal(uint32 hdl, struct ATT_PendIndStru *in)
{
    bool ret = FALSE;
    passth_profile_st *p_passthp = get_passthp_by_hdl(hdl);

    struct GATT_CharacteristicStru *set_char = p_passthp->set_char;
    struct GATT_DescriptorStru *set_char_ccc = p_passthp->set_char_ccc;
    struct GATT_CharacteristicStru *get_char = p_passthp->get_char;
    struct ATT_HandleValueStru *it;

    //BLE_DBG_PRINTF("PEND_IND 0x%x\n", in->op)
    it = &in->it;

    //write
    if ((in->op == WRITE_REQ) || (in->op == WRITE_CMD) /* ||(in->op == PREPARE_WRITE_REQ)*/)
    {
        if (in->it.hdl == set_char_ccc->hdl)
        {
            if (((it->val.value[0] & (uint8) GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED)
                    == GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED)
                    || ((it->val.value[0] & (uint8) GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED)
                            == GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED))
            {
                p_passthp->notify_enable = PASSTHP_RMT_RX_ENABLE;
            }
            else
            {
                p_passthp->notify_enable = PASSTHP_RMT_RX_DISABLE;
            }
            app_callback(hdl, PASSTHP_RMT_RXSTA_CHANGE, (void *) p_passthp->notify_enable);
        }
        else if (in->it.hdl == get_char->value_hdl)
        {
            if (p_passthp->rcv_buffer == NULL)
            {
                goto DEAL_ERR;
            }
            //print_receive(in->it.val.value, in->it.val.len);
            p_passthp->rcv_info.rcv_len = in->it.val.len;

            if (buffer_rw_write(p_passthp->rcv_buffer, in->it.val.value, in->it.val.len) == FALSE)
            {
                p_passthp->rcv_info.rcv_len = 0;
                p_passthp->rcv_info.t_loss += in->it.val.len;
                p_passthp->rcv_info.c_loss += in->it.val.len;
                BTEG_PRINTF("passth buffer full,closs %d,tloss\n", p_passthp->rcv_info.t_loss);
                goto DEAL_ERR;
            }
            else
            {
                if (p_passthp->rcv_info.c_loss != 0)
                {
                    p_passthp->rcv_info.c_loss = 0;
                }
            }

            app_callback(hdl, PASSTHP_RMT_TXDATA_IND, &p_passthp->rcv_info);
            //bt_engine_passth_receive(RCP_WITH_BLE, it->val.value, it->val.len);
        }
    }

    ret = TRUE;
    DEAL_ERR:
    GATT_Server_PendIND_Response(in);
    return ret;
}

static void get_param_uuid(uint32 *out_uuid, uint8 *out_uuid_type, struct uuid_string_st *in_uuid_str)
{
    struct ATT_UUIDStru *uuid;
    if (in_uuid_str->uuid_type == UUID_TYPE_16)
    {
        *out_uuid = (uint16) ((uint32) in_uuid_str->uuid & 0xffff);
        *out_uuid_type = in_uuid_str->uuid_type;
    }
    else if (in_uuid_str->uuid_type == UUID_TYPE_128)
    {
        uuid = (struct ATT_UUIDStru *) NEW(sizeof(struct ATT_UUIDStru));
        get_128bit_uuid_by_string(uuid, (uint8 *) in_uuid_str->uuid);
        out_uuid = (uint32 *) uuid;
        *out_uuid_type = in_uuid_str->uuid_type;
    }

    return;
}

static bool passthp_serv_reg_tree(uint32 hdl, struct GATT_ServiceStru *service)
{
    struct GATT_CharacteristicStru *c;
    passth_profile_st *p_passthp = get_passthp_by_hdl(hdl);

    //update service info
    if (service == NULL)
    {
        return FALSE;
    }
    p_passthp->serv = service;
    BLE_DBG_PRINTF("passthp service(%p) hdl : %d~%d\n", p_passthp->serv, p_passthp->serv->hdl, p_passthp->serv->endhdl);

    struct ATT_UUIDStru set_uuid;
    if (p_passthp->attr[1].uuid_type == UUID_TYPE_16)
    {
        get_128bit_uuid_by_16bits_uuid(&set_uuid, p_passthp->attr[1].uuid);
    }
    else if (p_passthp->attr[1].uuid_type == UUID_TYPE_128)
    {
        get_128bit_uuid_by_string(&set_uuid, (uint8 *) p_passthp->attr[1].uuid);
    }

    struct ATT_UUIDStru get_uuid;
    if (p_passthp->attr[3].uuid_type == UUID_TYPE_16)
    {
        get_128bit_uuid_by_16bits_uuid(&get_uuid, p_passthp->attr[3].uuid);
    }
    else if (p_passthp->attr[3].uuid_type == UUID_TYPE_128)
    {
        get_128bit_uuid_by_string(&get_uuid, (uint8 *) p_passthp->attr[3].uuid);
    }

    //update Character info
    c = service->character.head;
    while (c != NULL)
    {
        if (libc_memcmp(c->uuid.u, set_uuid.u, 16))
        {
            p_passthp->get_char = c;
        }
        else if (libc_memcmp(c->uuid.u, get_uuid.u, 16))
        {
            p_passthp->set_char = c;
        }
        else
        {
            BLE_PRINTF("Unknown uuid :");
            libc_printhex(c->uuid.u, 16, 0);
        }
        c = LNEXT(c);
    }

    if ((p_passthp->set_char == NULL) || (p_passthp->get_char == NULL))
    {
        BLE_PRINTF_ERROR("set char or get char is null\n")
        return FALSE;
    }

    p_passthp->set_char_ccc = p_passthp->set_char->descriptor.head;
    if (p_passthp->set_char_ccc == NULL)
    {
        BLE_PRINTF_ERROR("set char ccc is null\n")
        return FALSE;
    }

    BLE_DBG_PRINTF("passthp set char hdl -> %p,val hdl -> %p\n", p_passthp->set_char->hdl,
            p_passthp->set_char->value_hdl);
    BLE_DBG_PRINTF("passthp set char ccc hdl -> %p\n", p_passthp->set_char->hdl);
    BLE_DBG_PRINTF("passthp get char hdl -> %p,val hdl -> %p\n", p_passthp->get_char->hdl,
            p_passthp->get_char->value_hdl);

    p_passthp->hdl = hdl;
    p_passthp->cbk(hdl, PASSTHP_REGISTER_COMPLETE, (void *) hdl);
    return TRUE;
}

bool passthp_get_info(uint32 p_hdl,uint8 info_type,void *param)
{
    passth_profile_st *p_passthp = get_passthp_by_hdl(p_hdl);

    if (p_passthp = NULL)
    {
        BLE_DBG_PRINTF("passthp get_info,p_passthp is null\n");
        return FALSE;
    }

    switch(info_type)
    {
        case PASSTHP_GET_MTU_SIZE :
        {
            *(uint16 *)param = get_cur_rmt_client_mtu();
        }
        break;
        case PASSTHP_GET_NOTIFY_STATUS :
        {
            *(uint16 *)param = p_passthp->notify_enable;
        }
        break;
        default:
        break;
    }
    return TRUE;
}

#endif //end of #ifdef BT_SUPPORT_BLE
