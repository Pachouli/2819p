/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_le_uuid.h"
#include "bt_engine_le_profile.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

/**
 * 获取当前远端设备MTU
 * 输入参数：无
 * 返回值:MTU
 * */
uint16 get_cur_rmt_client_mtu(void)
{
    return g_bteg_le_env.rmt_client.mtu;
}

void APP_LE_Server_start(UINT8 *arg)
{
    APP_GATTS_Reg_GAP_GATT((bt_manager_ble_cfg_t *) arg);
#if (BT_SUPPORT_BLE_CLIENT == 1)
    le_client_start();
#endif

#if (BT_SUPPORT_BLE_BAS == 1)
    APP_GATT_BAS_Start();
#endif

#if (BT_SUPPORT_BLE_CTS == 1)
    APP_GATT_CTS_Start();
#endif

#if (BT_SUPPORT_BLE_HRS == 1)
    APP_GATT_HRS_Start(GATT_BLSC_WRIST);
#endif

#if (BT_SUPPORT_BLE_HOG == 1)
    APP_GATT_Hog_Start();
#endif

#if (BLE_TEST_BASE_ENABLE == 1)
    letest_base_test_start();
#endif
}

void APP_LE_Server_stop(UINT8 *arg)
{
#if (BT_SUPPORT_BLE_CLIENT == 1)
    le_client_stop();
#endif

#if (BT_SUPPORT_BLE_BAS == 1)
    APP_GATT_BAS_Stop();
#endif

#if (BT_SUPPORT_BLE_CTS == 1)
    APP_GATT_CTS_Stop();
#endif

#if (BT_SUPPORT_BLE_HRS == 1)
    APP_GATT_HRS_Stop();
#endif

#if (BT_SUPPORT_BLE_HOG == 1)
    APP_GATT_Hog_Stop();
#endif
}

void app_gatt_modify_device_name(uint8 *device_name)
{
    //设置设备名，否则默认为LE Samole Device
    struct ATT_ModifyValueStru *ModifyValue;
    ModifyValue = NEW(sizeof(struct ATT_ModifyValueStru) + libc_strlen(device_name));
    ModifyValue->hdl = 0x03; //GAP Server 为一个server，占用1~5句柄，1.Server 2.char for device name 3.device name char value 4 char for appearence 5 .appearence char value .其中句柄3的值为设备名
    ModifyValue->val.len = (UINT16) libc_strlen(device_name);
    libc_memcpy(ModifyValue->val.value, device_name, ModifyValue->val.len);
    GATT_Server_ModifyValue(ModifyValue);
}

/**
 * 更新le连接参数
 * 输入参数：uint16 min 最小连接间隔参数
 *          uint16 max 最大连接间隔参数
 *          uint16 latency 连接潜伏数
 *          uint16 timeout 连接超时参数
 * 返回值:无
 * */
void APP_GATTS_Reg_GAP_GATT(bt_manager_ble_cfg_t *reg_args)
{
    //LE GAP server reg etc. set device name/appearence/service change
    LE_GAP_ServiceReg(APP_GATT_Callback);

    //modify ble device name
    bt_manager_ble_cfg_t *p_ble_cfg = get_ble_cfg_info();
    app_gatt_modify_device_name(p_ble_cfg->ble_name);
}

/**
 * le注册服务
 * 输入参数：struct le_gatt_attr * le_services 创建service传入参数
 * 返回值:无
 * */
void APP_LE_REGISTER_SERVICE(struct le_gatt_attr * le_services)
{
    struct GATT_ServiceStru *service = NULL;
    struct GATT_CharacteristicStru *character = NULL;
    struct ATT_UUIDStru uuid;
    const struct le_gatt_attr * item;

    for (item = le_services;; item++)
    {
        if (item->attr_uuid == 0)
        {
            if (service != NULL)
                GATT_Server_RegTree(service);

            break;
        }
        else if (item->attr_uuid == 0x2800)
        {
            if (service != NULL)
                GATT_Server_RegTree(service);

            /* 新建主服务 */
            service = BTSRC_List_NodeNew(sizeof(struct GATT_ServiceStru));

            if (item->cbk)
            {
                service->cbk = item->cbk;
            }
            else
            {
                service->cbk = APP_GATT_Event_Default_Deal;
            }

            if (item->uuid_type == UUID_TYPE_16)
            {
                ATT_UUID_SET_U2(&service->uuid, item->uuid);
            }
            else if (item->uuid_type == UUID_TYPE_128)
            {
                ATT_UUID_SetU16(&service->uuid, (unsigned char * )item->uuid);
            }
            else
            {
                BLE_PRINTF_ERROR("NOT SUPPORT UUID TYPE\n");
                while (1)
                    ;
            }

            BLE_PRINTF("new PRIMARY_SERVICE:\n");
        }
        else if (item->attr_uuid == 0x2803)
        {
            if (service != NULL)
            {
                void * temp_data = NULL;
                void * init_data;

                if (item->init_data == NULL)
                {
                    temp_data = NEW(item->data_len);
                    libc_memset(temp_data, 0, item->data_len);
                    init_data = temp_data;
                }
                else
                {
                    init_data = (void *) item->init_data;
                }

                if (item->uuid_type == UUID_TYPE_16)
                {
                    ATT_UUID_SET_U2(&uuid, item->uuid);
                }
                else if (item->uuid_type == UUID_TYPE_128)
                {
                    ATT_UUID_SetU16(&uuid, (unsigned char * )item->uuid);
                }
                else
                {
                    BLE_PRINTF_ERROR("NOT SUPPORT UUID TYPE\n");
                    while (1)
                        ;
                }

                BLE_PRINTF("new Characteristic:\n");
                character = GATT_Add_Characteristic(service, init_data, item->data_len, &uuid, item->data_prop);
                if (item->init_data == NULL)
                {
                    FREE(temp_data);
                }
            }
        }
        else if (item->attr_uuid == 0x2902)
        {
            if (character != NULL)
            {
                BLE_PRINTF("new Descriptor_CCC\n");
                GATT_Add_Descriptor_CCC(character, (unsigned int )item->init_data, item->data_prop);
            }
        }
        else if (item->attr_uuid == 0x2908)
        {
            if (character != NULL)
            {
                const struct le_gatt_DRR * le_gatt_drr = item->init_data;

                BLE_PRINTF("new Descriptor_ReportRef\n");
                GATT_Add_Descriptor_ReportReference(character, le_gatt_drr->id, le_gatt_drr->type, item->data_prop);
            }
        }
    }
}

/**
 * 发送notify
 * 输入参数：UINT16 hdl 需要notify的char value hdl
 *          UINT8 *val notify数据
 *          UINT8 len notify数据长度
 * 返回值:无
 * */
bool APP_GATT_Server_NotifyIndicate(UINT16 hdl,UINT8 *val, UINT8 len)
{
    struct GATT_Server_NotifyIndicateStru *mod;

    //获取controller发送能力(默认值为108)，防止Notify数据由于来不及发送出去而耗尽内存
    if (HCI_LE_Capacity_Get() < LE_TX_CAPACITY_RESERVE)
    {
        //BLE_PRINTF_WARNNING("contrl not enough Capacity send data\n");
        return FALSE;
    }
    mod = NEW(sizeof(struct GATT_Server_NotifyIndicateStru) + len);

    libc_memcpy(&mod->val.value, val, len);
    mod->hdl = hdl;
    mod->val.len = len;
    GATT_Server_NotifyIndicate(mod);
    return TRUE;
}

#endif
