/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/

#ifndef _BT_ENGINE_LE_H_
#define _BT_ENGINE_LE_H_

#include "bluetooth_engine.h"
#include "bt_engine_le_profile.h"
#include "bt_engine_le_ancs.h"
#include "bt_engine_le_server.h"
#include "bt_engine_le_uuid.h"
#include "bt_engine_le_test.h"

//add for ble
//#include "att_handler.h"
#include "att_ui.h"
#include "att_uuid.h"
#include "attr.h"
#include "gatt_com.h"
#include "hci_le.h"
#include "l2cap_ui.h"

#ifdef BT_SUPPORT_BLE
#define BT_SUPPORT_BLE_HOG  0
#define BT_SUPPORT_BLE_BAS  0
#define BT_SUPPORT_BLE_CTS  0
#define BT_SUPPORT_BLE_HRS  0
#define BT_SUPPORT_BLE_CLIENT 0
#else
//BT_SUPPORT_BLE没打开，强制不支持各个BLE Profile
#define BT_SUPPORT_BLE_HOG  0
#define BT_SUPPORT_BLE_BAS  0
#define BT_SUPPORT_BLE_CTS  0
#define BT_SUPPORT_BLE_HRS  0
#define BT_SUPPORT_BLE_CLIENT 0
#endif

#if (BT_SUPPORT_BLE_CLIENT == 1)
#define BT_SUPPORT_BLE_TIP_CLINET 1
#else
#define BT_SUPPORT_BLE_TIP_CLINET 0
#endif

#if (BT_SUPPORT_BLE_TIP_CLINET == 1)
#include "bt_engine_le_tip_client.h"
#endif
#if (BT_SUPPORT_BLE_HOG == 1)
#include "bt_engine_le_hog.h"
#endif
#if (BT_SUPPORT_BLE_BAS == 1)
#include "bt_engine_le_bas.h"
#endif
#if (BT_SUPPORT_BLE_CTS == 1)
#include "bt_engine_le_cts.h"
#endif
#if (BT_SUPPORT_BLE_HRS == 1)
#include "bt_engine_le_hrs.h"
#endif

//uint16 change to little endian string
#define ENCODE2BYTE_LITTLE(des_ptr, src_data)             \
    *(UINT8 *)((des_ptr) + 1) = (UINT8)((src_data) >> 8); \
    *(UINT8 *)(des_ptr) = (UINT8)(src_data);

#define MIN_MTU_LENGTH(a, b) (a > b ? b : a)

//device name when scan
#define BLE_LOCAL_NAME "LINK_BLE_US281B_HAB"

#define MAX_SERVER_NUM 2
#define MAX_CLIENT_NUM 1

//for client
#define MAX_SERVICE_CNT 3

//max att acl connection
#define RMT_DEV_NUM 2

#define BLE_ADDR_CHANGE_BYTE (5)
#define BLE_ADDR_CHANGE_BIT ((1 << 6) | (1 << 7))

#define GATTC_SUPPORT 0
#define GATT_PROFILE_SUPPORT_MAX    10

#define APP_LE_EXCHANGE_MTU_SECURITY_MODE           LE_GAP_SECURITY_MODE1_LEVEL2
//for discover service/include/characteristic, exchange MTU
#define APP_LE_DISCOVER_SECURITY_MODE               LE_GAP_SECURITY_MODE1_LEVEL1
//LE_GAP_SECURITY_MODE1_LEVEL2//for read_value and write_value
#define APP_LE_READWRITE_SECURITY_MODE              LE_GAP_SECURITY_MODE1_LEVEL1

enum
{
    APP_PEERMASK_NAME = 0x01, APP_PEERMASK_DISPLAY = 0x02, APP_PEERMASK_LINK = 0x04, /* Connected or not */
    APP_PEERMASK_DISCOVRING = 0x10, //added by lin
    APP_PEERMASK_DISCOVRED = 0x20,
    APP_PEERMASK_PAIRED = 0x80,
};

typedef enum
{
    REMOTE_SERVER = 0, //remote type
    REMOTE_CLIENT,
    REMOTE_EITHER

} remote_list_e;

typedef enum
{
    EXIST_CHECK_MODE, EMPTY_CHECK_MODE,
} le_get_peernode_mode_e;

/*! service type枚举 */
typedef enum
{
    RCP_ACT_SERV_FLAG = 0, //actions service
    RCP_TIP_SERV_FLAG,     //ble tip service
    RCP_ANCS_SERVN_FLAG,   //apple ancs service notify
    RCP_ANCS_SERVD_FLAG,   //apple ancs service data source

    // 发送特性
    RCP_ANCS_CTRL_POINT_FLAG,
    RCP_CTS_SERV_FLAG,  //cts service
    RCP_NDST_SERV_FLAG, //ndst service
    RCP_BAS_SERV_FLAG,  //bas service
    RCP_LLS_SERV_FLAG,  //
    RCP_IAS_SERV_FLAG,  //
    RCP_HRS_SERV_FLAG,  //
    RCP_HTS_SERV_FLAG,  //
    RCP_FMS_SERV_FLAG,  //
    RCP_HTS_SERV1_FLAG,
    RCP_RTUS_SERV_FLAG,
    RCP_SERVICE_MAX //invalid service
} rcp_service_type_e;

typedef enum
{
    ERR_RSP = 0x01,
    EXCHANGE_MTU_REQ,
    EXCHANGE_MTU_RSP,
    FIND_INFORMATION_REQ,
    FIND_INFORMATION_RSP,
    FIND_BY_TYPE_VALUE_REQ,
    FIND_BY_TYPE_VALUE_RSP,
    READ_BY_TYPE_REQ,
    READ_BY_TYPE_RSP,
    READ_REQ, //0x0a
    READ_RSP,
    READ_BLOB_REQ,
    READ_BLOB_RSP,
    READ_MULTIPLE_REQ,
    READ_MULTIPLE_RSP,
    READ_BY_GROUP_TYPE_REQ, //0x10
    READ_BY_GROUP_TYPE_RSP,
    WRITE_REQ = 0x12,
    WRITE_RSP,
    PREPARE_WRITE_REQ = 0x16,
    PREPARE_WRITE_RSP,
    EXECUTE_WRITE_REQ,
    EXECUTE_WRITE_RSP,
    HANDLE_VALUE_NOTIFY = 0x1B,
    HANDLE_VALUE_IND = 0x1D,
    HANDLE_VALUE_CONFIRM,
    WRITE_CMD = 0x52,
    SIGNED_WRITE_CMD = 0xD2

} att_opcode_e;

enum
{ /* mask of struct APP_ATTRootStru */
    APP_ATTMASK_AUTOCACHE = 0x01, APP_ATTMASK_LOCAL = 0x02, /* Local or Remote browsing */
    APP_ATTMASK_MUTEX_ON = 0x04, /*当前是否处于SPP与LE互斥状态，主要是LE被限制*/
};

struct APP_ATTProfileHdlStru
{
    uint8 handle_used;
    struct GATT_ServiceStru *cur_service;
    struct GATT_CharacteristicStru *cur_character;
    struct GATT_DescriptorStru *cur_desc;

    /* Local Services */
    struct ListStru service_list; /* node is struct GATT_ServiceStru service; */
};

typedef struct
{
    uint16 serv_uuid;
    uint16 char_uuid;
} le_service_id_t;

//add for ble
typedef struct
{
    uint8 server_name_str[MAX_BLE_NAME_LEN];
    uint8 addr[BD_ADDR_LEN];
    uint8 adv_manufact_id[2];
    uint8 uuid[UUID_STR_LEN];
    uint8 uuid_type;
} att_advertiser_info_t;

struct APP_PeerNodeStru
{
    struct HCI_AddressStru addr;
    struct ListStru service_list; /* node is struct GATT_ServiceStru service; */
    struct GATT_ServiceStru *cur_service;
    struct GATT_CharacteristicStru *cur_character;
    struct GATT_DescriptorStru *cur_desc;
    unsigned int last_time; /* Last update time */
    UINT8 counter; /* update change counter */
    UINT8 class_of_device[3]; /* [031],Mask */
    UINT8 rssi; /* [134],Val */
    UINT8 remote_name[30];
    UINT8 link_key[16];
    UINT8 key_type;
    UINT8 mask; /* APP_PEERMASK_NAME */
    UINT16 acl_hdl; /* connect handle */
};

struct APP_ATTRootStru
{
    struct ListStru remote_list; /* Local is server, Node is struct APP_PeerNodeStru */

    /* Current */
    struct GATT_TL_ConnectStru tlin; /* Remote */
    struct GATT_ServiceStru *cur_service;
    struct GATT_CharacteristicStru *cur_character;
    struct GATT_DescriptorStru *cur_desc;

    /* Local Services */
    struct ListStru service_list; /* node is struct GATT_ServiceStru service; */
    UINT32 read_count; /* Auto cache, Read Job counter */
    UINT32 adv_count;
    UINT8 mask; /* task complete checker, APP_ATTMASK_AUTOCACHE */
    UINT8 task_index; /* Last task index created */
};

typedef struct
{
    UINT8 adv_data_length; /* [309],Val */
    UINT8 adv_data[31]; /* [310],Val */
} LE_Set_Adv_DataStru_t;

typedef struct
{
    struct APP_PeerNodeStru *p_remote_node; /* Local is server, Node is struct APP_PeerNodeStru */
    uint8 serv_conn;
    uint16 mtu;                      //远端设备的MTU
} le_remote_client_st;

typedef struct
{
    int s_att_inited; //标志BLE是否已经初始化
    uint8 *server_name; //BLE本地设备名
    struct APP_ATTRootStru *app_attroot; //att 协议软件结构根节点
    le_remote_client_st rmt_client;
} bt_engine_le_env_st;

extern bt_engine_le_env_st g_bteg_le_env;
extern bt_manager_ble_cfg_t g_ble_cfg;

/************************** functions  ***************************/

/****** common  **********/
extern struct APP_PeerNodeStru *APP_PeerNode_FindCUR(void);
extern void APP_PeerNode_SelectCUR(struct APP_PeerNodeStru *node);
extern INT8 APP_PeerNode_GETID_ByADDR(struct HCI_AddressStru *addr, UINT8 mode, remote_list_e which_list);
extern UINT16 APP_ATT_GET_UUID2(UINT8 *uuid, UINT32 len);
extern uint16 app_att_get_16bit_uuid(UINT8 *uuid, uint8 len);
extern void ble_name_add_mac(void);
extern void app_gatt_debug_prin_uuid_str(struct ATT_UUIDStru *uuid);
extern void app_gatt_debug_print_att_handle_with_uuid_str(struct ATT_UUIDStru *uuid);
extern void le_update_default_connection_param(void);
extern void le_update_rmt_client_conn_par(uint16 min, uint16 max, uint16 latency, uint16 timeout);

/****** client/server  **********/
extern void APP_GATT_Callback(void *context, UINT8 ev, void *arg);
extern void APP_GATTC_DisconnectGATT();
extern void *APP_GATT_ItemGetByHDL(struct APP_PeerNodeStru *remote, UINT8 *type, UINT16 hdl);
//client
extern void SendRXPackage(void);
extern void ExitRXPackage(void);

/****** client  **********/
//extern void APP_GATT_Character_ReadValue(UINT8 *arg);
//extern void APP_GATT_Service_FindInclude(UINT8 *arg);
//extern void APP_GATT_Service_DiscoverCharacter(UINT8 *arg);
extern void APP_GATTC_Link_Connected_Hook(struct GAP_ConnectionEventStru *in);
extern void APP_GATTC_Link_Loss_Hook(struct GAP_ConnectionEventStru *in);
extern void APP_LE_Set_Advertiser(UINT8 *arg);
extern void APP_PeerNode_RemoveDevices(void);
extern void APP_LE_Set_Advertise_Disable(UINT8 *arg);
extern void APP_GATTS_Reg_GAP_GATT(bt_manager_ble_cfg_t *reg_args);
extern void APP_GATT_AutoCache(UINT8 task, void *context);
extern void APP_GATTC_Remote_usrble_Init(struct APP_PeerNodeStru *remote);
extern void APP_GATT_Select_Service(struct GATT_ServiceStru *service);
extern void APP_GATT_Select_Character(struct GATT_CharacteristicStru *c);
extern void APP_GATT_Character_WriteValue2(UINT8 *arg, UINT8 len);
//extern void APP_GATT_Character_ReadValue(UINT8 *arg, struct GATT_CharacteristicStru *read_c);
extern void APP_GATT_sel_remote_service(UINT8 sel_serv_id);
extern UINT8 APP_GATT_get_cur_service(void);
extern UINT8 APP_GATT_get_service_uuids(UINT8 sel_serv_id, le_service_id_t *id_arry);
extern UINT8 APP_GATT_get_servid_by_chuuid(UINT16 ch_uuid);
extern void deal_gatt_ev_creat_service(struct GATT_ServiceStru *service);
extern void deal_gatt_ev_creat_include(struct GATT_IncludeStru *include);
extern void deal_gatt_ev_creat_character(struct GATT_CharacteristicStru *c);
extern void deal_gatt_ev_creat_descriptor(struct GATT_DescriptorStru *desc);

/****** server  **********/
extern void APP_LE_Get_ServerName(UINT8 type);
extern void APP_LE_Server_start(UINT8 *arg);
extern void APP_server_actions_uuid_get(void);
extern void APP_server_actions_uuid_release(void);
extern void APP_Remove_services(struct APP_PeerNodeStru *remote);
extern void APP_GATTS_write_Service_deal(UINT8 *arg, uint8 *rcp_data, rmt_ctrl_pkg_t *rcp_par);
extern void APP_GATTS_read_Service_deal(UINT8 *arg, rmt_ctrl_pkg_t *rcp_par);
extern bool APP_GATT_Server_NotifyIndicate(UINT16 hdl,UINT8 *val, UINT8 len);
extern bool APP_Tx_Send(UINT8 *arg, UINT16 len, UINT8 serv_flag, UINT8 remote_id);
extern void deal_gatt_ev_hdlvalue(struct ATT_HandleValueCbkStru *in);
extern void deal_gatt_ev_pend_ind(struct ATT_PendIndStru *in);
extern void APP_SMP_Callback(UINT8 event, void *arg);
void APP_GATT_Channel_Callback(void *context, UINT8 Event, void *arg);
void APP_GATT_Callback(void *context, UINT8 ev, void *arg);

extern bt_manager_ble_cfg_t *get_ble_cfg_info(void);
extern void bt_engine_le_env_init(void);
extern void App_att_start(void);
extern void App_att_stop(void);

extern struct APP_PeerNodeStru *APP_PeerNode_Create(struct HCI_AddressStru *addr);
extern struct APP_PeerNodeStru *APP_PeerNode_FindByADDR(struct HCI_AddressStru *addr);
extern int32 le_new_profile(void);
extern void APP_GATT_Event_Default_Deal(void *context, UINT8 ev, void *arg);
extern struct GATT_CharacteristicStru *le_find_char_in_service(struct GATT_ServiceStru* service,
UINT32 char_uuid, UUID_TYPE uuid_type);
extern struct GATT_ServiceStru* le_find_service_in_remote(struct APP_PeerNodeStru *remote,
UINT32 srv_uuid, UUID_TYPE uuid_type);
extern struct GATT_CharacteristicStru* le_find_char_in_remote(struct APP_PeerNodeStru *remote,
UINT32 srv_uuid, UINT32 c_uuid, UUID_TYPE uuid_type);
struct GATT_DescriptorStru * le_find_CCC_in_char(struct GATT_CharacteristicStru* c);

#if (BT_SUPPORT_BLE_CLIENT == 1)
#include "bt_engine_le_client.h"
#endif

#endif /*_BT_ENGINE_GATT_H*/

