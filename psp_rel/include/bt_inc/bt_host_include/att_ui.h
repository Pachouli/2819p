/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2013 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
Module Name:
    att_ui.h
Abstract:
    This module defines GENERIC ATTRIBUTE PROFILE(GATT) interface functions 
    and macros.
---------------------------------------------------------------------------*/

#ifndef G_ATT_UI_H
#define G_ATT_UI_H

#include "att_uuid.h"
#include "hci_ui.h"
/*---------------------------------------------------------------------------*/
/*                          GATT Object Structure                            */
/*---------------------------------------------------------------------------*/
struct ATT_UUIDStru {
    UINT8 u[16];/* Bluetooth Base UUID + 16bits UUID, or 128bit UUID */
};

struct ATT_ErrorRspStru {/* cfm param in ATTS_Error_Cfm */
    UINT16 hdl;
    UINT8 reqcode;
    UINT8 errcode;
};

struct ATT_ValueStru {/* len, variable length value */
    UINT16 len;
    UINT8 value[1];
};

struct ATT_ModifyValueStru {/* Handle, Value */
    UINT16 hdl;
    struct ATT_ValueStru val;
};

struct ATT_HandleValueStru {/* Handle, Offset, Value */
    UINT16 hdl;
    UINT16 offset;
    struct ATT_ValueStru val;
};

struct ATT_HandleValueCbkStru {/* Remote Value got, read or Indication/Notification */
    struct HCI_AddressStru addr;
    struct ATT_HandleValueStru data;
};

struct ATT_HandlePermissionStru {/* arg in GATTC_ServerModify_Permission */
    UINT16 hdl;/* Attribute Item handle */
    UINT16 permission;/* mask,etc: ATT_READ or GATT_PROPERTIES_NEW_ALERT_ALERT_NOTIFICATION_SERVICE */
};

struct GATT_Hdl_UIStru {
    UINT16 hdl;
};

struct GATT_Server_NotifyIndicateStru {/* addr(RFU), hdl, value */
    UINT16 hdl;
    struct ATT_ValueStru val;
};

enum {/* mask in struct ATT_PendIndStru */
    ATT_PENDMASK_START = 0x01,
    ATT_PENDMASK_FINAL = 0x02,
    ATT_PENDMASK_ONE = (ATT_PENDMASK_START | ATT_PENDMASK_FINAL),
};

struct ATT_PendIndStru {/* Pendded received IND */
    UINT8 index;
    UINT8 op; /* etc, ATT_OP_READ_BLOB_REQ */
    UINT8 mask;/* etc, ATT_PENDMASK_FINAL */
    UINT8 errcode;/* etc: ATT_ERR_OK, for upper set error code */
    struct ATT_UUIDStru uuid;
    struct HCI_AddressStru addr;/* Remote, for TL */
    struct ATT_HandleValueStru it;/* Variable length tail */
};

struct ATT_Handle_Value_NotifyConfirmStru {/* arg of GATT_EV_HDLVALUE_NOTIFY_CONFIRM */
    struct HCI_AddressStru addr;
};

struct GATT_ServiceStru {
    UINT16 hdl;
    UINT16 endhdl;
    struct ATT_UUIDStru uuid;
    struct ListStru include;/* struct GATT_IncludeStru */
    struct ListStru character;/* struct GATT_CharacteristicStru */
    void *cbk;/* (*(GATTFunc *)(cbk))(context, ev, arg); */
};

struct GATT_IncludeStru {
    UINT16 hdl;/* attribute handle */
    UINT16 include_hdl;/* Included service attribute handle */
    UINT16 endhdl;/* End Group Handle */
    struct ATT_UUIDStru uuid;/* Service UUID, Use to save unsolved include service object */
};

struct GATT_CharacteristicStru {
    UINT16 hdl;
    UINT16 value_hdl;
    struct ATT_ValueStru *val;/* Need to be new/free */
    struct ListStru descriptor;/* struct GATT_DescriptorStru */
    struct ATT_UUIDStru uuid;
    UINT16 properties;/* etc: ATT_READ, + ATT_AUTHENTICATION_NEED(Only In Register Input) */
};

struct GATT_DescriptorStru {
    UINT16 hdl;
    UINT16 properties;/* etc: ATT_READ, only used in local server, not for attribute cache since it is hidden */
    struct ATT_UUIDStru uuid;
    struct ATT_ValueStru *val;/* Need to be new/free */
};

typedef void (GATTFunc) (void *context, UINT8 ev, void *arg);/* GATT Callback function */

enum {/* require in GATT_TL_Connect */
    GATT_TLREQUIRE_LE =         0x01,
    GATT_TLREQUIRE_BREDR =      0x02,
    GATT_TLREQUIRE_ANY =        0x03/* But Perfer LE */
};

struct GATT_TL_ConnectStru {
    struct HCI_AddressStru addr;
    void *context;
    void *cbk;
    UINT32 timeout;/* IDLE timeout when task complete */
    UINT8 require;/* etc: GATT_TLREQUIRE_ANY */
    UINT8 sec_mode;/* etc: LE_GAP_SECURITY_MODE1_LEVEL1 */
};

struct GATT_TL_DisconnectStru {
    struct HCI_AddressStru addr;
    void *context;
    void *cbk;
};

struct GATT_TaskCancelStru {/* Arg of GATT_Task_Cancel */
    struct HCI_AddressStru addr;
    UINT8 index;/* index of the task */
};

struct GATT_TaskCompleteStru {/* Arg of GATT_EV_TASK_COMPLETE/GATT_EV_TASK_CREATE */
    struct HCI_AddressStru addr;
    UINT8 index;/* index of the task */
    UINT8 task;/* etc, GATT_TASK_EXCHANGE_MTU */
    UINT8 errcode;/* etc, ATT_ERR_OK */
};

enum 
{
    /* GATT PROCEDURE, Task Index */
    /* Feature mask */
    GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END =            0x80,
    GATT_TASK_MASK_WRITE_CT =                           0x40,
    GATT_TASK_MASK_LIST_CT =                            0x20,
    
    /* Server Configuration */
    GATT_TASK_EXCHANGE_MTU =                            (0x01),/* struct GATT_MTU_CTStru */

    /* Primary Service Discovery */
    /* struct GATT_2HDL_UUID_CTStru */
    GATT_TASK_DISCOVER_ALLSERVICES =                    (0x02 | GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END),
    /* struct GATT_2HDL_UUID_CTStru */
    GATT_TASK_DISCOVER_SERVICES_BYUUID =                (0x03 | GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END),

    /* Relationship Discovery */
    /* struct GATT_2HDL_UUID_CTStru */
    GATT_TASK_FIND_INCLUDED_SERVICES =                  (0x04 | GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END),

    /* Characteristic Discovery */
    /* struct GATT_2HDL_UUID_CTStru */
    GATT_TASK_DISCOVER_ALLCHARACTERISTIC_OFASERVICE =   (0x05 | GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END),
    /* struct GATT_2HDL_UUID_CTStru */
    GATT_TASK_DISCOVER_CHARACTERISTIC_BYUUID =          (0x06 | GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END),

    /* Characteristic Descriptor Discovery */
    /* struct GATT_2HDL_CTStru */
    GATT_TASK_DISCOVER_ALLCHARACTERISTIC_DESCRIPTORS =  (0x07 | GATT_TASK_MASK_ATTRIBUTE_NOT_FOUND_END),

    /* Characteristic Value Read */
    /* Characteristic Descriptor Value Read */
    /* struct GATT_Read_CTStru */
    GATT_TASK_READ_VALUE =                              0x08,
    /* struct GATT_2HDL_UUID_CTStru */
    GATT_TASK_READ_USING_CHARACTERISTIC_UUID =          0x09,
    /* struct GATT_List_CTStru, node is UINT16 node */
    GATT_TASK_READ_MULTIPLE_CHARACTERISTIC_VALUES =     (0x0a | GATT_TASK_MASK_LIST_CT),

    /* Characteristic Value Write */
    /* Characteristic Descriptor Value Write */
    /* struct GATT_List_CTStru, node is struct ATT_HandleValueStru *node */
    GATT_TASK_RELIABLE_WRITES =                         (0x0b | GATT_TASK_MASK_LIST_CT),
    /* struct GATT_Write_CTStru, Signed or not Signed */
    GATT_TASK_WRITE_WITHOUT_RESPONSE =                  (0x0c | GATT_TASK_MASK_WRITE_CT),
    /* struct GATT_Write_CTStru, Write_Request or Convert to Reliable_Write inside */
    GATT_TASK_WRITE_VALUE =                             (0x0d | GATT_TASK_MASK_WRITE_CT),

    /* Characteristic Value Notification */
    GATT_TASK_NOTIFICATIONS =                           (0x0e | GATT_TASK_MASK_WRITE_CT),

    /* Characteristic Value Indication */
    GATT_TASK_INDICATIONS =                             (0x0f | GATT_TASK_MASK_WRITE_CT)    
};

enum 
{
    /* GATT Callback events */
    /* (Server), ListNode struct GATT_ServiceStru, Callback this service with handles filled */
    GATT_EV_HDL_TREE =                  1,
    /* (Client), ListNode struct GATT_ServiceStru */
    GATT_EV_CREATE_SERVICE =            2,
    /* (Client), ListNode struct GATT_IncludeStru */
    GATT_EV_CREATE_INCLUDE =            3,
    /* (Client), ListNode struct GATT_CharacteristicStru */
    GATT_EV_CREATE_CHARACTER =          4,
    /* (Client), ListNode struct GATT_DescriptorStru */
    GATT_EV_CREATE_DESCRIPTOR =         5,
    /* (Client/Server), struct ATT_HandleValueStru, include indication or notification, remote database value */
    GATT_EV_HDLVALUE =                  6,
    /* (Client/Server), struct GATT_TaskCompleteStru */
    GATT_EV_TASK_CREATE =               7,
    /* (Client/Server), struct GATT_TaskCompleteStru */
    GATT_EV_TASK_COMPLETE =             8,
    /* (Server), struct ATT_PendIndStru */
    GATT_EV_PEND_IND =                  9,
    /* (Server), struct ATT_Handle_Value_NotifyConfirmStru, Handle_Value_Notification/Confirmation */
    GATT_EV_HDLVALUE_NOTIFY_CONFIRM =   10
};

enum 
{
    /* value type, how to visit u in struct ATT_ItemStru */
    ATT_VALUE_INTERNAL =                        0,/* u->data, internal characteristic value */
    ATT_VALUE_TL_RELATED =                      1,/* u->list, etc: CCC... */
};

enum {/* Error Code in Error Response */

    /* Internal */
    ATT_ERR_OK =                                0x00,

    /* The attribute handle given was not valid on this server. */
    ATT_ERR_INVALID_HANDLE =                    0x01,

    /* The attribute cannot be read. */
    ATT_ERR_READ_NOT_PERMITTED =                0x02,

    /* The attribute cannot be written. */
    ATT_ERR_WRITE_NOT_PERMITTED =               0x03,

    /* The attribute PDU was invalid. */
    ATT_ERR_INVALID_PDU =                       0x04,

    /* The attribute requires authentication before it can be read or written. */
    ATT_ERR_INSUFFICIENT_AUTHENTICATION =       0x05,

    /* Attribute server does not support the request received from the client. */
    ATT_ERR_REQUEST_NOT_SUPPORTED =             0x06,

    /* Offset specified was past the end of the attribute */
    ATT_ERR_INVALID_OFFSET =                    0x07,

    /* The attribute requires authorization before it can be read or written. */
    ATT_ERR_INSUFFICIENT_AUTHORIZATION =        0x08,

    /* Too many prepare writes have been queued, PrepareWrite */
    ATT_ERR_PREPARE_QUEUE_FULL =                0x09,

    /* No attribute found within the given attribute handle range. */
    ATT_ERR_ATTRIBUTE_NOT_FOUND =               0x0A,

    /* The attribute cannot be read or written using the Read Blob Request, errara 4887 I suggest remove it */
    ATT_ERR_ATTRIBUTE_NOT_LONG =                0x0B,

    /* The Encryption Key Size used for encrypting this link is insufficient. */
    ATT_ERR_INSUFFICIENT_ENCRYPTION_KEYSIZE =   0x0C,

    /* The attribute value length is invalid for the operation, WriteReq? */
    ATT_ERR_INVALID_ATTRIBUTE_VALUE_LENGTH =    0x0D,

    /* The attribute request that was requested has encountered 
       an error that was unlikely, and therefore could not be completed as requested.
     */
    ATT_ERR_UNLIKELY_ERROR =                    0x0E,

    /* The attribute requires encryption before it can be read or written. */
    ATT_ERR_INSUFFICIENT_ENCRYPTION =           0x0F,

    /* ReadByGroupType, The attribute type is not a supported grouping attribute 
       as defined by a higher layer specification. 
     */
    ATT_ERR_UNSUPPORTED_GROUPTYPE =             0x10,

    /* Insufficient Resources to complete the request */
    ATT_ERR_INSUFFICIENT_RESOURCES =            0x11,/* For Write, too long */
    /* 0x12-0x7F are reserved */
    
    /* 0x80-0xFF are application error */
    /* CSS v2 Common Profile and service error code descriptions */
    ATT_ERR_OUT_OF_RANGE =                      0xFF,
    ATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS =     0xFE,
    ATT_ERR_CCC_IMPROPERLY_CONFIGURED =         0xFD,

    /* Unknown is just considered to be a unknown reason */

    /* Internal */
    ATT_ERR_SMPFAILED =                         0xE9,
    ATT_ERR_TLIDLETIMEOUT =                     0xEA,
    ATT_ERR_CANCEL =                            0xEB,
    ATT_ERR_DONE =                              0xEC,
    ATT_ERR_TL_CONNECT_FAILED =                 0xED,
    ATT_ERR_RTXTIMEOUT =                        0xEE,
    ATT_ERR_IGNORE =                            0xEF,
};

enum {/* For Application */
    ATT_TYPE_SERVICE =                          0,
    ATT_TYPE_INCLUDE =                          1,
    ATT_TYPE_CHARACTER =                        2,/* Characteristic Declaration */
    ATT_TYPE_CHARACTER_VALUE =                  3,/* Characteristic Value */
    ATT_TYPE_DESCRIPTOR =                       4
};

/*---------------------------------------------------------------------------*/
/*                                  GATT Macros                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Characteristic Extended Properties:
    Handle(2), Type(2,Characteristic Extended Properties),  
    Extended Properties Bit Field(2), Permission(Read Only, No Auth).
---------------------------------------------------------------------------*/
enum {/* Characteristic Extended Properties Bit Field */
    /* Reliable_Write/Execute_Write */
    ATT_EXTP_RELIABLE_WRITE =       GATT_CHARACTERISTIC_EXTENDED_PROPERTIES_RELIABLE_WRITE_ENABLED,

    /* Characteristic User Description writable or not */
    ATT_EXTP_WRITABLE_AUXILIARIES = GATT_CHARACTERISTIC_EXTENDED_PROPERTIES_WRITABLE_AUXILIARIES_ENABLED
    /* Reserved for Future Use */
};

/*---------------------------------------------------------------------------
Client Characteristic Configuration:
    Handle(2), Type(2,Client Characteristic Configuration),  
    Configuration Bits, Permission(Higher Layer).
---------------------------------------------------------------------------*/
enum {/* Client Characteristic Configuration Bit Field */
    ATT_CCCMASK_NOTIFICATION =      GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED, 
    ATT_CCCMASK_INDICATION =        GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_INDICATIONS_ENABLED
    /* Reserved for Future Use 0xFFF4 */
};

/*---------------------------------------------------------------------------
Server Characteristic Configuration:
    Handle(2), Type(2,Server Characteristic Configuration),  
    Configuration Bits, Permission(Higher Layer).
---------------------------------------------------------------------------*/
enum {/* Server Characteristic Configuration Bit Field */
    ATT_SCCMASK_BROADCAST =         GATT_SERVER_CHARACTERISTIC_CONFIGURATION_BROADCASTS_ENABLED
    /* Reserced for Future Use 0xFFF2 */
};

/*---------------------------------------------------------------------------
Characteristic Presentation Format:
    Handle(2), Type(2,Characteristic Presentation Format),  
    Format(1), Exponent(1), Unit(2), Name Space(1), Description(2), Permission(Higher Layer).
etc, GATT_CHARACTERISTIC_PRESENTATION_FORMAT_FORMAT_BOOLEAN
---------------------------------------------------------------------------*/


//att_channel_cbk 'S EVENT
enum {
    ATT_CHANNEL_MTU_REQ =  			0,/* MTU REQ*/
};



#endif/* G_ATT_UI_H */

