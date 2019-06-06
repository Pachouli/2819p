/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2013 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
Module Name:
    hci_le.h
Abstract:
    This module defines LE GAP structure, macro & functions.
---------------------------------------------------------------------------*/

#ifndef G_HCI_LE_H
#define G_HCI_LE_H

#include "global.h"

/*---------------------------------------------------------------------------*/
/*                              LE GAP GATT Service                          */
/*---------------------------------------------------------------------------*/

#define LE_DEVICENAME   "LE Sample Device"

/*-----------------------------------------------------------------------------
Description:
    
-----------------------------------------------------------------------------*/
enum {/* mode in SMP_SetSecurityMode */
    /* A connection operating in LE  security mode 1 level 2 shall also 
    satisfy the security requirements for LE security mode 1 level 1.
    A connection operating in LE  security mode 1 level 3 shall also 
    satisfy the security requirements for LE security mode 1 level 2 or 
    LE security mode 1 level 1.
    A connection operating in LE security mode 1 level 3 shall also 
    satisfy the security requirements fo r LE security mode 2. */
    
    LE_GAP_SECURITY_MODE1_LEVEL1,/* No security */
    LE_GAP_SECURITY_MODE1_LEVEL2,/* Unauthenticated pairing and encryption */
    LE_GAP_SECURITY_MODE1_LEVEL3,/* Authenticated Pairing and encryption */
    LE_GAP_SECURITY_MODE2_LEVEL1,/* Unauthenticated pairing and data signing */
    LE_GAP_SECURITY_MODE2_LEVEL2/* Authenticated pairing and data signing */
};
/* Table 12.7:  Format of the preferred connection parameters structured data */
struct LE_GAP_Peripheral_ConnectionParStru {
    /* connInterval min = Conn_Interval_Min * 1.25 ms range: 0x0006 to 0x0C80, 
    Value of 0xFFFF indicates no specific minimum. */
    UINT8 min_connection_interval[2];
    /* connInterval max = Conn_Interval_Max * 1.25 ms range: 0x0006 to 0x0C80, 
    Shall be equal to or greater than the Conn_Interval_Min, Value of 0xFFFF indicates no specific maximum. */
    UINT8 max_connection_interval[2];
    UINT8 slave_latency[2];/* range: 0x0000 to 0x03E8 */
    /* Defines the connection supervisor timeout multiplier as a multiple of 10ms. 
    Range: 0xFFFF indicates no specific value requested. Range: 0x000A to 0x0C80, 
    Time = N * 10 msec, Time Range: 100 msec to 32 seconds */
    UINT8 connection_supervision_timeout_multiplier[2];
};

/*---------------------------------------------------------------------------*/
/*                          LE GAP HCI Parts                                 */
/*---------------------------------------------------------------------------*/
enum {/* LE GAP Role */
    LE_CENTRAL = 0,
    LE_PERIPHERIAL = 1
};

enum {/* Privacy flag in GATT_UUID_PERIPHERAL_PRIVACY_FLAG */
    LE_PRIVACY_DISABLE = 0,
    LE_PRIVACY_ENABLE = 1
};

enum {/* Random Address Type */
    LE_ADDRTYPE_PUBLIC,
    LE_ADDRTYPE_STATIC,
    LE_ADDRTYPE_PRIVATE_RESOLVABLE,/* set in Peripheral, with 15min lifetime */
    LE_ADDRTYPE_PRIVATE_NON_RESOLVABLE,/* Other`s device, ignore? */
    /* Local is Central, non-resolvable, in GAP GATT characteristic, Central write to Peripheral */
    LE_ADDRTYPE_PRIVATE_RECONNECTION_CENTRAL,
    LE_ADDRTYPE_PRIVATE_RECONNECTION_PERIPHERAL,/* Local is Peripheral */

    LE_ADDRTYPE_PUBLIC_STATIC,/* Local used public or static, once selected, will not changed lifetime */
};

enum {/* Discovery mode */
    LE_DISCOVERYMODE_NONE,
    LE_DISCOVERYMODE_LIMITED,
    LE_DISCOVERYMODE_GENERAL
};

enum {/* Advertising/Scanning Speed */
    LE_FAST,
    LE_SLOW,
    LE_FAST_AUTO_SLOW
};

enum {/* Connection Mode */
    LE_CONNECTMODE_NONE,
    LE_CONNECTMODE_DIRECTED,
    LE_CONNECTMODE_UNDIRECTED
};


#endif/* G_HCI_LE_H */

