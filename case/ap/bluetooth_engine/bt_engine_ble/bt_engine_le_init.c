/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_smp.h"
#include "bt_engine_le_test.h"
#include "config_id.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

bt_engine_le_env_st g_bteg_le_env;
bt_manager_ble_cfg_t g_ble_cfg;

bt_manager_ble_cfg_t *get_ble_cfg_info(void)
{
    int32 temp_flag = sys_local_irq_save();
    if (g_ble_cfg.get_ble_cfg_info_complete == 1)
    {
        sys_local_irq_restore(temp_flag);
        return &g_ble_cfg;
    }

    libc_memset(&g_ble_cfg, 0x00, sizeof(bt_manager_ble_cfg_t));
    g_ble_cfg.act_serv_uuid = NEW(UUID_STR_LEN);
    if (g_ble_cfg.act_serv_uuid == NULL)
    {
        BLE_DBG_PRINTF("act server uuid malloc fail\n");
        while (1)
            ;
    }
    com_get_config_struct(BTMANAGER_ACTION_PROFILE_SRV_UUID, g_ble_cfg.act_serv_uuid, UUID_STR_LEN);

    g_ble_cfg.act_get_uuid = NEW(UUID_STR_LEN);
    if (g_ble_cfg.act_get_uuid == NULL)
    {
        BLE_DBG_PRINTF("act server uuid malloc fail\n");
        while (1)
            ;
    }
    com_get_config_struct(BTMANAGER_ACTION_PROFILE_GET_CHAR_UUID, g_ble_cfg.act_get_uuid, UUID_STR_LEN);

    g_ble_cfg.act_set_uuid = NEW(UUID_STR_LEN);
    if (g_ble_cfg.act_set_uuid == NULL)
    {
        BLE_DBG_PRINTF("act server uuid malloc fail\n");
        while (1)
            ;
    }
    com_get_config_struct(BTMANAGER_ACTION_PROFILE_SET_CHAR_UUID, g_ble_cfg.act_set_uuid, UUID_STR_LEN);

    g_ble_cfg.ble_name = NEW(BT_MAC_NAME_LEN_MAX+1);
    if (g_ble_cfg.ble_name == NULL)
    {
        BLE_DBG_PRINTF("ble name malloc fail\n");
        while (1)
            ;
    }
    com_get_config_struct(BTSTACK_BLE_LOCAL_NAME, g_ble_cfg.ble_name, BT_MAC_NAME_LEN_MAX);
    g_ble_cfg.ble_name[BT_MAC_NAME_LEN_MAX] = 0;
    BLE_PRINTF("ble name:%s\n", g_ble_cfg.ble_name);

    //ble地址基于经典蓝牙地址，模拟随机地址
    libc_memcpy(g_ble_cfg.local_addr.bytes, g_btengine_var.device_addr.bytes, BD_ADDR_LEN);

    //模拟随机地址
    g_ble_cfg.local_addr.bytes[BLE_ADDR_CHANGE_BYTE] = (uint8) (~g_ble_cfg.local_addr.bytes[BLE_ADDR_CHANGE_BYTE]);
    g_ble_cfg.local_addr.bytes[BLE_ADDR_CHANGE_BYTE] |= BLE_ADDR_CHANGE_BIT;

    g_ble_cfg.le_manufacture_id = (uint16) com_get_config_default(BTSTACK_BLE_MANUFACTURE_ID);
    g_ble_cfg.le_ancs_sp = 0;
    g_ble_cfg.le_tip_sp = 0;
    g_ble_cfg.le_passthp_sp = com_get_config_default(BTMANAGER_BLE_PASSTH_PROFILE_SUPPORT);
    g_ble_cfg.get_ble_cfg_info_complete = 1;
    sys_local_irq_restore(temp_flag);

    return &g_ble_cfg;
}

void ble_cfg_deinit(void)
{
    int32 temp_flag = sys_local_irq_save();
    if (g_ble_cfg.ble_name != NULL)
    {
        FREE(g_ble_cfg.act_serv_uuid);
        g_ble_cfg.act_serv_uuid = NULL;
    }

    if (g_ble_cfg.act_serv_uuid != NULL)
    {
        FREE(g_ble_cfg.act_serv_uuid);
        g_ble_cfg.act_serv_uuid = NULL;
    }

    if (g_ble_cfg.act_get_uuid != NULL)
    {
        FREE(g_ble_cfg.act_get_uuid);
        g_ble_cfg.act_get_uuid = NULL;
    }

    if (g_ble_cfg.act_set_uuid != NULL)
    {
        FREE(g_ble_cfg.act_set_uuid);
        g_ble_cfg.act_set_uuid = NULL;
    }

    g_ble_cfg.get_ble_cfg_info_complete = 0;
    sys_local_irq_restore(temp_flag);
}

void bt_engine_le_env_init(void)
{
    bt_engine_le_env_st *p_le_env;
    p_le_env = &g_bteg_le_env;

    libc_memset(p_le_env, 0x00, sizeof(bt_engine_le_env_st));

    p_le_env->s_att_inited = -1;

    p_le_env->app_attroot = NEW(sizeof(struct APP_ATTRootStru));
    libc_memset(p_le_env->app_attroot, 0, sizeof(struct APP_ATTRootStru));

    bt_manager_ble_cfg_t *p_ble_cfg;
    p_ble_cfg = get_ble_cfg_info();
}

void bt_engine_le_env_deinit(void)
{
    bt_engine_le_env_st *p_le_env;
    p_le_env = &g_bteg_le_env;

    if (p_le_env->app_attroot != NULL)
    {
        FREE(p_le_env->app_attroot);
        p_le_env->app_attroot = NULL;
    }

    p_le_env->s_att_inited = -1;

    ble_cfg_deinit();
}

struct GATT_CharacteristicStru *le_find_char_in_service(struct GATT_ServiceStru *service, UINT32 char_uuid, UUID_TYPE uuid_type)
{
    struct GATT_ServiceStru *s = service;
    struct GATT_CharacteristicStru *c;
    if(uuid_type == UUID_TYPE_128 && get_16bit_uuid_by_128bit_uuid((struct ATT_UUIDStru *)char_uuid))
    {
        uuid_type = UUID_TYPE_16;
        char_uuid = get_16bit_uuid_by_128bit_uuid((struct ATT_UUIDStru *)char_uuid);
    }

    if (service)
    {
        c = s->character.head;

        while (c)
        {
            if (uuid_type == UUID_TYPE_16)
            {
                if (get_16bit_uuid_by_16or128bit_uuid(c->uuid.u, 2) == (UINT16)char_uuid)
                {
                    return c;
                }
                
                if (get_16bit_uuid_by_16or128bit_uuid(c->uuid.u, 16) == (UINT16)char_uuid)
                {
                    return c;
                }
            }
            else if (uuid_type == UUID_TYPE_128)
            {
                if (libc_memcmp(c->uuid.u, (UINT8 *)char_uuid, 16) == 0)
                {
                    return c;
                }

                struct ATT_UUIDStru uuid_128;
                get_128bit_uuid_by_string(&uuid_128,char_uuid);
                if (libc_memcmp(c->uuid.u, uuid_128.u, 16) == 0)
                {
                    return c;
                }
            }

            c = LNEXT(c);
        }
    }
    return NULL;
}

struct GATT_ServiceStru* le_find_service_in_remote(struct APP_PeerNodeStru *remote, UINT32 srv_uuid, UUID_TYPE uuid_type)
{
    struct GATT_ServiceStru* s = remote->service_list.head;

    if (!remote)
    {
        BLE_DBG_PRINTF("remote is null\n");
        return NULL;
    }

    if(uuid_type == UUID_TYPE_128 && get_16bit_uuid_by_128bit_uuid((struct ATT_UUIDStru *)srv_uuid))
    {
        uuid_type = UUID_TYPE_16;
        srv_uuid = get_16bit_uuid_by_128bit_uuid((struct ATT_UUIDStru *)srv_uuid);
    }

    while (s)
    {
        if (uuid_type == UUID_TYPE_16 ? get_16bit_uuid_by_16or128bit_uuid(s->uuid.u, 2) == srv_uuid
                        || get_16bit_uuid_by_16or128bit_uuid(s->uuid.u, 16) == srv_uuid
                        : !libc_memcmp(s->uuid.u, (UINT8 *)srv_uuid, 16))
            return s;

        s = LNEXT(s);
    }
    return NULL;
}

struct GATT_DescriptorStru * le_find_CCC_in_char(struct GATT_CharacteristicStru* c)
{
    struct GATT_DescriptorStru *d;

    if (c)
    {
        d = c->descriptor.head;
        while (d)
        {
            if (get_16bit_uuid_by_16or128bit_uuid(d->uuid.u, 2) == GATT_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION
                    || get_16bit_uuid_by_16or128bit_uuid(d->uuid.u, 16)
                            == GATT_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION)
            {
                return d;
            }
            d = LNEXT(d);
        }
    }
    return NULL;
}

struct GATT_CharacteristicStru* le_find_char_in_remote(struct APP_PeerNodeStru *remote, UINT32 srv_uuid, UINT32 c_uuid, UUID_TYPE uuid_type)
{
    struct GATT_ServiceStru* s = NULL;

    if ((s = le_find_service_in_remote(remote, srv_uuid, uuid_type)))
        return le_find_char_in_service(s, c_uuid, uuid_type);

    return NULL;
}

void *APP_GATT_ItemGetByHDL(struct APP_PeerNodeStru *remote, UINT8 *type, UINT16 hdl)
{
    struct GATT_ServiceStru *s = remote->service_list.head;
    struct GATT_CharacteristicStru *c;
    struct GATT_DescriptorStru *d;
    struct GATT_IncludeStru *inc;

    while (s != NULL)
    {
        if (s->hdl == hdl)
        {
            *type = ATT_TYPE_SERVICE;
            return s;
        }

        inc = s->include.head;
        while (inc != NULL)
        {
            if (inc->hdl == hdl)
            {
                *type = ATT_TYPE_INCLUDE;
                return inc;
            }
            inc = LNEXT(inc);
        }

        c = s->character.head;
        while (c != NULL)
        {
            if (c->hdl == hdl)
            { //Declearation handle...
                *type = ATT_TYPE_CHARACTER;
                return c;
            }
            if (c->value_hdl == hdl)
            { //Value handle...
                *type = ATT_TYPE_CHARACTER_VALUE;
                return c;
            }
            d = c->descriptor.head;
            while (d != NULL)
            {
                if (d->hdl == hdl)
                {
                    *type = ATT_TYPE_DESCRIPTOR;
                    return d;
                }
                d = LNEXT(d);
            }
            c = LNEXT(c);
        }
        s = LNEXT(s);
    }

    return NULL;
}

void App_att_start(void)
{
    if (bt_manager_cfg_info.sp_ble == 0)
    {
        BLE_PRINTF("not suport LE\n");
        g_bteg_le_env.s_att_inited = -1;
        return;
    }
    else
    {
        BLE_PRINTF("App_att_start\n"); //for debug
        bt_engine_le_env_init();
    }

    //reg smp encry key size to set local capacity
    app_smp_reglocalcap(APP_SMP_Callback);

    //reg Callback to exchange MTU size
    GATT_AttChannelCbk_Reg((void * )APP_GATT_Channel_Callback);

    //reg Callback to deal GATT event
    GATT_NotificationCbk_Reg(APP_GATT_Callback); /* Notification callback */

    APP_LE_Server_start(NULL);
    APP_LE_Set_Advertiser(NULL);

    g_bteg_le_env.s_att_inited = 1; //只是保证app这边已经ready
}

void App_att_stop(void)
{
    BLE_PRINTF("App_att_stop\n");

    if (g_bteg_le_env.s_att_inited < 0)
    {
        return;
    }

    APP_PeerNode_RemoveDevices(); /* Remove peer nodes */

    { /* Remove services */
        struct GATT_ServiceStru *service, *tmp;

        service = g_bteg_le_env.app_attroot->service_list.head;
        while (service != NULL)
        {
            tmp = service;
            service = LNEXT(service);
            GATT_Free_Service(tmp);
        }
    }

    APP_LE_Server_stop(NULL);

    //release request memory space
    bt_engine_le_env_deinit();
}

#endif
