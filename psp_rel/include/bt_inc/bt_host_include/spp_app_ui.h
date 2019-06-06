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

#ifndef _APP_SPP_UI_H
#define _APP_SPP_UI_H

#include "global.h"

typedef enum _tSpp_Event
{
    /* Server Events */
    APP_SPP_EV_SERVER_START_CFM = 1,    /* struct SppStartServerCfmStru* */

    /* Common Events */
    APP_SPP_EV_CONNECT_COMPLETE,        /* struct SppConnectionInforStru* */
    APP_SPP_EV_DATA_CFM,                /* struct SppDataCfmStru* */
    APP_SPP_EV_DATA_IND,                /* struct SppDataIndStru* */
    APP_SPP_EV_DISCONNECT_COMPLETE,     /* struct SppConnectionInforStru* */
    APP_SPP_EV_RPN_CFM,
    APP_SPP_EV_INVALID = 0xFF,
} tSpp_Event;

typedef enum _tSpp_Connection_Role
{
    APP_SPP_ROLE_CLIENT,
    APP_SPP_ROLE_SERVER
} tSpp_Connection_Role;

typedef enum _tSpp_Error_Code
{
    APP_SPP_ER_SUCCESS,
    APP_SPP_ER_RFCOMM_CONNECT_FAILED,
    APP_SPP_ER_SDAP_FAILED,
} tSpp_Error_Code;

typedef void (*tApp_Spp_Callback)(UINT32 inst_handle, tSpp_Event event, void *param);

/* Parameter for APP_SPP_EV_SERVER_START_CFM */
struct SppStartServerCfmStru
{
    UINT8  server_channel;
    UINT32 service_record_handle;
};

/* Parameter for APP_SPP_EV_CONNECT_COMPLETE */
struct SppConnectionInforStru
{
    UINT32                  connection_handle;
    UINT8                   bd[6];
    tSpp_Error_Code         result;
    tSpp_Connection_Role    role;
};

/* Parameter for APP_SPP_EV_DATA_CFM */
struct SppDataCfmStru
{
    UINT8   *data;
    UINT32  data_size;
    UINT32  sent_size;
};

/* Parameter for APP_SPP_EV_DATA_IND */
struct SppDataIndStru
{
    UINT8   *data;
    UINT32  data_size;
};
/* Parameter for configure */
struct Spp_RpnParStru
{
    UCHAR   baudrate;
    UCHAR   numbits: 2;
    UCHAR   stopbits: 1;
    UCHAR   parity: 1;
    UCHAR   ptype: 2;
    UCHAR   fctrl: 6;
    UCHAR   xon;
    UCHAR   xoff;
    UCHAR   pm1;
    UCHAR   pm2;
};

/* Parameter for configure */
struct SppRpnReqStru
{
    tApp_Spp_Callback rpn_cfm_cbk;
    UINT32 connection_handle;
    struct Spp_RpnParStru *spp_rpn;
};
/* API */
void APP_Spp_Start(void);
void App_Spp_Stop(void);
void App_Spp_StartServerReq(tApp_Spp_Callback server_cbk, UINT8 max_credits, UINT8 max_connections);
void App_Spp_StopServerReq(UINT32 server_handle);
void App_Spp_ConnectReq(tApp_Spp_Callback server_cbk, UINT8 *bd, UINT8 max_credits, UINT8 server_channel);
void App_Spp_DisconnectReq(UINT32 connection_handle);
void App_Spp_DataReq(tApp_Spp_Callback data_cfm_cbk, UINT32 connection_handle, UINT8 *data, UINT32 data_size);
void App_Spp_GetCFG(tApp_Spp_Callback rpn_cfm_cbk, UINT32 connection_handle, struct Spp_RpnParStru *Rpn);
UINT16 App_Spp_Get_Credits(UINT32 connection_handle);
#endif /* _APP_SPP_UI_H */
