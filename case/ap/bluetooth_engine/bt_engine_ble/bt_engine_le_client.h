/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE BAS支持头文件。
 * 作者：dengcong
 ********************************************************************************/
#ifndef __BT_ENGINE_LE_CLIENT_PROFILE_H__
#define __BT_ENGINE_LE_CLIENT_PROFILE_H__

#include "bt_engine_le.h"

#if (BT_SUPPORT_BLE_CLIENT == 1)

struct write_ccc_stru
{
    UINT32 srv_uuid;
    UINT32 c_uuid;
    UUID_TYPE uuid_type;
    bool enable;
    void *cbk;
};

typedef void(*cbk_ptr)(void * arg);

struct gatt_notify_cbkstru
{
    struct HCI_AddressStru addr;
    UINT16 hdl;
    cbk_ptr cbk;
};

struct cbk_stru
{
    cbk_ptr cbk;
};

void APP_GATT_Discover(struct APP_PeerNodeStru *remote);
void APP_GATT_Client_Remove_services(struct APP_PeerNodeStru* remote);
void le_client_discover_cbk_reg(cbk_ptr arg);
void le_client_disconnect_cbk_reg(cbk_ptr arg);

void APP_GATT_Character_ReadValue(void *cbk, struct GATT_CharacteristicStru *read_c);
void APP_GATT_Desc_WriteCCC_Enable(struct APP_PeerNodeStru *remote, struct write_ccc_stru *ccc_arg);
void le_remote_service_notify_reg(void *arg);
void le_remote_service_notify_unreg(void *arg);
void le_notify_ev_hdlvalue(struct ATT_HandleValueCbkStru *arg);

void le_client_connect(struct APP_PeerNodeStru * remote);
void le_client_disconnect(struct APP_PeerNodeStru * remote);
void le_client_start(void);
void le_client_stop(void);
//void le_client_test_loop(void);

#endif //end of #if (BT_SUPPORT_BLE_CLIENT == 1)

#endif //end of #ifndef __BT_ENGINE_LE_CLIENT_PROFILE_H__
