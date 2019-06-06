/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2013 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
Module Name:
    app_spp_ui.h
Abstract:
    This module defines all of the SPP user interfaces provided to upper layer.
    also defines all indication callback function provided to L2CAP.
-----------------------------------------------------------------------------*/

#ifndef _APP_HID_UI_H
#define _APP_HID_UI_H

#include "global.h"

#define HID_CONTROL_CHANNEL					0x00
#define HID_INTERRUPT_CHANNEL					0x01

typedef enum _tHid_Event
{
    /* Server Events */
    APP_HID_EV_SERVER_START_CFM = 1, /* struct HidStartServerCfmStru* */

    /* Common Events */
    APP_HID_EV_CONNECT_COMPLETE, /* struct HidConnectionInforStru* */
    APP_HID_EV_DATA_CFM, /* struct HidDataCfmStru* */
    APP_HID_EV_DATA_IND, /* struct HidDataIndStru* */
    APP_HID_EV_DISCONNECT_COMPLETE, /* struct HidConnectionInforStru* */
    APP_HID_EV_RPN_CFM, APP_HID_EV_INVALID = 0xFF,
} tHid_Event;

typedef enum _tHid_Connection_Role
{
    APP_HID_ROLE_DEVICE, APP_HID_ROLE_HOST
} tHid_Connection_Role;

typedef enum _tHid_Error_Code
{
    APP_HID_ER_SUCCESS, APP_HID_ER_FAILED,
} tHid_Error_Code;

typedef void (*tApp_Hid_Callback)(UINT32 inst_handle, tHid_Event event, void *param);

struct HID_RegInfoStru
{
    UINT16 mask; /* optional or mandatory Bool type attribute mask */
    UINT16 release_num; /* HID device release number */
    UINT16 parser_ver; /* HID parser version */
    UINT8 sub_cls; /* HID device subclass */
    UINT8 country_code; /* HID country code */
    UINT16 super_to; /* HID supervision timeout */
    UINT16 desc_list_size; /* total size of the descriptor list. It also marks the start point
     of the report list in the successive memory. */
    UINT8 *desc_ptr;
};

/* Parameter for APP_HID_EV_SERVER_START_CFM */
struct HidStartServerCfmStru
{
    UINT8 server_channel;
    UINT32 service_record_handle;
};

struct HID_ConnectReqStru
{
    /* param of connect_req and disconnect_req */
    UINT8 bd_addr[BD_ADDR_LEN];
};

/* Parameter for APP_HID_EV_CONNECT_COMPLETE */
struct HidConnectionInforStru
{
    UINT32 connection_handle;
    UINT8 bd[6];
    tHid_Error_Code result;
    tHid_Connection_Role role;
};

/* Parameter for APP_HID_EV_DATA_CFM */
struct HidDataCfmStru
{
    UINT8 *data;
    UINT32 data_size;
    UINT32 sent_size;
};

/* Parameter for APP_HID_EV_DATA_IND */
struct HidDataIndStru
{
    UINT8 *data;
    UINT32 data_size;
};

#define APP_HID_CUDATA                      ((struct HidFsmUserDataStru *)(app_hid_fsm_inst->user_data))
#define APP_HID_SERVER_LIST                 (APP_HID_CUDATA->server_list)
#define APP_HID_CONNECTION_LIST             (APP_HID_CUDATA->connection_list)
#define APP_HID_HANDLE_BASE                 (APP_HID_CUDATA->handle_base)



#endif /* _APP_HID_UI_H */
