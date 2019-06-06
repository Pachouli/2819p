/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_le_adv.h"
#include "bt_engine_le_uuid.h"
#include "bt_engine_le_profile.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

void ATT_Set_Advertising_Parameters(void);
void ATT_set_advertising_data(att_advertiser_info_t *att_advertiser_info);
void ATT_Set_Scan_Response_Data(att_advertiser_info_t *att_advertiser_info);
void ATTS_Set_Advertiser(att_advertiser_info_t *att_advertiser_info);

void APP_LE_Set_Advertise_Enable(UINT8 *arg)
{
    BLE_PRINTF("ADV ON");
    LE_Set_Advertise_Enable((UINT8)HCI_ADVERTISING_ENABLE_ENABLED);
}

void APP_LE_Set_Advertise_Disable(UINT8 *arg)
{
    BLE_PRINTF("ADV OFF");
    LE_Set_Advertise_Enable((UINT8)HCI_ADVERTISING_ENABLE_DISABLED_DEFAULT);
}

void ATT_Set_Advertising_Parameters(void)
{
    /*LE_FAST_AUTO_SLOW*/
    LE_Set_Advertising_Parameters((UINT8)5, (UINT8)LE_DISCOVERYMODE_GENERAL, (UINT8)LE_SLOW,
            (UINT8)HCI_ADVERTISING_FILTER_POLICY_SCAN_ANY_CONNECT_ANY);

    /*LE_FAST_AUTO_SLOW*/
    /*LE_Set_Advertising_Parameters((UINT8)HCI_ADVERTISING_TYPE_CONNECTABLE_UNDIRECTED,
     (UINT8) LE_DISCOVERYMODE_GENERAL, (UINT8) LE_FAST,
     (UINT8) HCI_ADVERTISING_FILTER_POLICY_SCAN_ANY_CONNECT_ANY);*/
}

void ATT_set_advertising_data(att_advertiser_info_t *att_advertiser_info)
{
    NEWSTRU(struct HCI_LE_Set_Advertising_DataStru, in);
    struct ListStru adlist;
    struct HCI_EIR_DataNodeStru *node;
    UINT16 service_uuid16;

    bt_manager_ble_cfg_t *p_ble_cfg;
    p_ble_cfg = get_ble_cfg_info();

    //set uuid
    service_uuid16 = get_16bit_uuid_by_string(att_advertiser_info->uuid);
    if (service_uuid16 == 0)
    {
        BLE_PRINTF("server uuid in not 16bits uuid\n");
    }
    else
    {
        BLE_DBG_PRINTF("adv server uuid 0x%x\n", service_uuid16);
    }

    //clr advertising data struct
    libc_memset(in, 0, sizeof(struct HCI_LE_Set_Advertising_DataStru));
    libc_memset(&adlist, 0, sizeof(struct ListStru));

    //discover mode set
    node = BTSRC_List_NodeNew(sizeof(struct HCI_EIR_DataNodeStru));
    node->len = 1 + 1;
    node->eir_data_type = (UINT8) HCI_EIR_DATATYPE_FLAGS;
    node->u.flags = (UINT8) HCI_EIR_FLAGS_LE_GENERAL_DISCOVERABLE_MODE
            | (UINT8) HCI_EIR_FLAGS_LE_AND_BREDR_TO_SAME_DEVICE_CAPABLE_CONTROLLER; /* Dual mode */
    BLE_PRINTF("ad add discover mode\n");
    BTSRC_List_AddTail(&adlist, node);

    //server uuid set
    if (service_uuid16 != 0)
    {
        node = BTSRC_List_NodeNew(sizeof(struct HCI_EIR_DataNodeStru) + (1 /* Number of UUIDs */- 1) * 2);
        node->len = 1 + 1 * 2;
        node->eir_data_type = (UINT8) HCI_EIR_DATATYPE_COMPLETE_LIST_OF_16BIT_SERVICE_CLASS_UUIDS;
        ENCODE2BYTE_LITTLE(&node->u.uuid16[0], service_uuid16)
        BLE_PRINTF("ad add uuid16\n");
        BTSRC_List_AddTail(&adlist, node);
    }

    //appearence set
    node = BTSRC_List_NodeNew(sizeof(struct HCI_EIR_DataNodeStru));
    node->len = 1 + 2;
    node->eir_data_type = (UINT8) HCI_EIR_DATATYPE_APPEARANCE;
    ENCODE2BYTE_LITTLE(node->u.appearence, (UINT16)GATT_APPEARANCE_CATEGORY_ACT_RCP_ISO_APK)
    BLE_PRINTF("ad add Appearance\n");
    BTSRC_List_AddTail(&adlist, node);

    //Manufacturer specific data set
    node = BTSRC_List_NodeNew(sizeof(struct HCI_EIR_DataNodeStru));
    node->len = 1 + 2 + 6;
    node->eir_data_type = (uint8) HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA;
    //add Manufact id
    libc_memcpy(&node->u.manufacturer_data.company_identifier_code, att_advertiser_info->adv_manufact_id, 2);
    //add local ble addr
    libc_memcpy(node->u.manufacturer_data.data, p_ble_cfg->local_addr.bytes, BD_ADDR_LEN);
    BLE_PRINTF("ad add manufacturer data\n");
    BTSRC_List_AddTail(&adlist, node);

    in->advertising_data_length = BTSRC_HCI_Extended_Inquiry_Response_Encode(&adlist, in->advertising_data,
            in->advertising_data + 31);

    HCI_Command_SendInter(in, (UINT32)HCI_OPS_LE_SET_ADVERTISING_DATA);
}

void ATTS_Set_Advertiser(att_advertiser_info_t *att_advertiser_info)
{
    BLE_PRINTF("LE Set Advertising Parameters\n");
    ATT_Set_Advertising_Parameters(); /* internal */
    BLE_PRINTF("LE Set Scan advertising Data\n");
    ATT_set_advertising_data(att_advertiser_info); /* was external call, now internal call */
    BLE_PRINTF("LE Set Scan Response Data\n");
    ATT_Set_Scan_Response_Data(att_advertiser_info); /* was external call, now internal call */
    BLE_PRINTF("LE Set Advertise Enable\n");
    LE_Set_Advertise_Enable((UINT8)HCI_ADVERTISING_ENABLE_ENABLED);
}

void APP_LE_Set_Advertiser(UINT8 *arg)
{
    bt_manager_ble_cfg_t *p_ble_cfg;
    p_ble_cfg = get_ble_cfg_info();

    att_advertiser_info_t *p_att_advertiser_info = NEW(sizeof(att_advertiser_info_t));
    if (p_att_advertiser_info == NULL)
    {
        BLE_PRINTF("adv info strcut malloc fail\n");
        return;
    }

    //get ble server name
    libc_memcpy(p_att_advertiser_info->server_name_str, p_ble_cfg->ble_name, libc_strlen(p_ble_cfg->ble_name));
    p_att_advertiser_info->server_name_str[libc_strlen(p_ble_cfg->ble_name)] = 0;
    ;

    //get manufacture id
    p_att_advertiser_info->adv_manufact_id[0] = (p_ble_cfg->le_manufacture_id & 0xFF00) >> 8;
    p_att_advertiser_info->adv_manufact_id[1] = (p_ble_cfg->le_manufacture_id & 0xff);

    libc_memcpy(p_att_advertiser_info->addr, (uint8 * )p_ble_cfg->local_addr.bytes, BD_ADDR_LEN);

    //get server uuid
    libc_memcpy(p_att_advertiser_info->uuid, p_ble_cfg->act_serv_uuid, UUID_STR_LEN);

    ATTS_Set_Advertiser(p_att_advertiser_info);

    FREE(p_att_advertiser_info);
    p_att_advertiser_info = 0;
}

void ATT_Set_Scan_Response_Data(att_advertiser_info_t *att_advertiser_info)
{
    NEWSTRU(struct HCI_LE_Set_Scan_Response_DataStru, in);
    NEWSTRU(struct HCI_LE_Set_Random_AddressStru, random_in);
    struct ListStru adlist;
    struct HCI_EIR_DataNodeStru *node;

    bt_manager_ble_cfg_t *p_ble_cfg;
    p_ble_cfg = get_ble_cfg_info();

    UINT8 *newname = att_advertiser_info->server_name_str;
    UINT8 len = (UINT8) libc_strlen(newname);

    libc_memset(&adlist, 0, sizeof(struct ListStru));
    libc_memset(in, 0, sizeof(struct HCI_LE_Set_Scan_Response_DataStru));

    //set device name to adv scan resp
    BLE_PRINTF("scan resp add ble name\n");
    node = BTSRC_List_NodeNew(sizeof(struct HCI_EIR_DataNodeStru) + len);
    node->len = 1 + len;
    node->eir_data_type = (UINT8) HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME;
    libc_memcpy(node->u.name.data, newname, len);
    BTSRC_List_AddTail(&adlist, node);

    in->scan_response_data_length = BTSRC_HCI_Extended_Inquiry_Response_Encode(&adlist, in->scan_response_data,
            in->scan_response_data + 31); /* LE_Set_ADNodes */

    HCI_Command_SendInter(in, (UINT32)HCI_OPS_LE_SET_SCAN_RESPONSE_DATA);

    //APP_LE_Set_Random_Addr(random_in->random_address);
    libc_memcpy(random_in->random_address, p_ble_cfg->local_addr.bytes, BD_ADDR_LEN);

    HCI_Command_SendInter(random_in, (UINT32)HCI_OPS_LE_SET_RANDOM_ADDRESS);
}

#endif
