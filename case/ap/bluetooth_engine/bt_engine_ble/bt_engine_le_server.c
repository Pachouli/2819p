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

#ifdef BT_SUPPORT_BLE //ATT ���ܷ���ʵ��

/**
 * ��ȡ��ǰԶ���豸MTU
 * �����������
 * ����ֵ:MTU
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
    //�����豸��������Ĭ��ΪLE Samole Device
    struct ATT_ModifyValueStru *ModifyValue;
    ModifyValue = NEW(sizeof(struct ATT_ModifyValueStru) + libc_strlen(device_name));
    ModifyValue->hdl = 0x03; //GAP Server Ϊһ��server��ռ��1~5�����1.Server 2.char for device name 3.device name char value 4 char for appearence 5 .appearence char value .���о��3��ֵΪ�豸��
    ModifyValue->val.len = (UINT16) libc_strlen(device_name);
    libc_memcpy(ModifyValue->val.value, device_name, ModifyValue->val.len);
    GATT_Server_ModifyValue(ModifyValue);
}

/**
 * ����le���Ӳ���
 * ���������uint16 min ��С���Ӽ������
 *          uint16 max ������Ӽ������
 *          uint16 latency ����Ǳ����
 *          uint16 timeout ���ӳ�ʱ����
 * ����ֵ:��
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
 * leע�����
 * ���������struct le_gatt_attr * le_services ����service�������
 * ����ֵ:��
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

            /* �½������� */
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
 * ����notify
 * ���������UINT16 hdl ��Ҫnotify��char value hdl
 *          UINT8 *val notify����
 *          UINT8 len notify���ݳ���
 * ����ֵ:��
 * */
bool APP_GATT_Server_NotifyIndicate(UINT16 hdl,UINT8 *val, UINT8 len)
{
    struct GATT_Server_NotifyIndicateStru *mod;

    //��ȡcontroller��������(Ĭ��ֵΪ108)����ֹNotify�����������������ͳ�ȥ���ľ��ڴ�
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
