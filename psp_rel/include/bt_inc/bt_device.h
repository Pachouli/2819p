/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen   2014-7-28 14:54     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine_device.h
 * \brief    cailizhen
 * \version  1.0
 * \date     2014-7-28
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_DEVICE_H
#define _BLUETOOTH_ENGINE_DEVICE_H

//#include "bluetooth_engine_common.h"

#define ENABLE_SBC_DROP_LOG   //使能SBC丢包数、错误个数记录

#define LINKKEY_SIZE 16

#define BT_BLE_NAME_LEN_MAX    30 /*9*3 + 2*/

#define PAIRED_INFO_TAG         0x1234
#define L2CAP_BUF_LEN 4

#define FIXED_PIN       "0000"
#define MAX_DEVICE_LINK          2
#define MAX_SEID                 (MAX_DEVICE_LINK * 2)   // sbc and aac

/* A device may have multiple AMPs of the same type.  */
/* Bluetooth */
#define HCI_CONTROLLER_BREDR                                0x00
/* WIFI,  0x03 - 0xEF Reserved, 0xF0 - 0xFF Vendor-Specific */
#define HCI_CONTROLLER_80211                                0x01
#define HCI_CONTROLLER_ECMA368                              0x02/* UWB */
#define HCI_CONTROLLER_FAKEPAL_UDP                      0xEF/* UDP Fake PAL UPF31 */

#define HCI_USE_SOFT_TIMER             (100)

#define MAX_PHONENUM               16

#define MGR_MAX_REMOTE_NAME_LEN         16
#define BD_ADDR_LEN                     6
#define PAIRED_LIST_MAX         8

typedef struct
{
    void *stream_handle; //a2dp流句柄
    uint16 stream_cid;   //a2dp 流CID
    uint8 cp_active;     //内容保护模式
    uint8 is_source;     //用于界定a2dp的角色1-src;0-snk
} A2dpSourceStru;

typedef struct
{
    UINT16 outgoing_browsing_mtu;
    //    UINT16 local_player_uidcounter;
    UINT16 local_element_uidcounter;
    UINT16 remote_element_uidcounter;
    UINT16 remote_folder_uidcounter;
    //    UINT16 addressed_playerid;
    //    UINT16 browsed_playerid;
    //    UINT8 listplayer_appsettingvalue_req_attr_id;
    //    UINT8 local_folder_uid[8];
    //    UINT8 local_element_uid[8];
    UINT8 remote_folder_uid[8];
    UINT8 remote_element_uid[8];
    //    UINT8 current_folder_uid[8];
    //    UINT8 current_volume;
    //    UINT8 current_playbackstatus;
    //    UINT8 current_track;
    //    UINT8 current_playbackposition;
    //    UINT8 current_batterystatus;
    //    UINT8 current_systemstatus;
    UINT8 connected_flag;
} AvrcpSourceStru;

typedef struct
{
    uint8 status;
    uint8 a2dp_sub_status;      //a2dp的状态:连接，播放，暂停等
    uint8 hfp_sub_status;       //HFP的状态:连接，接听，来电，拔出等
    uint8 service_connect;      //profile(hfp,a2dp,avrcp,hid等)服务的连接状况
    uint8 need_auto_connect;    //是否需要自动回连
    int8 rssi;
    uint8 sniff_flag;           //是否进入sniff标志
    uint8 phone_status;         //电话本信息状态
    uint8 phone_number[MAX_PHONENUM];   //电话号码缓存buf
    A2dpSourceStru a2dp_source; //a2dp 流信息结构
    AvrcpSourceStru avrcp_source;
    HANDLE g_hf_handle;         //hfp 服务连接句柄
    uint16 acl_handle;          //ACL链路句柄
    uint16 sco_handle_setup;    //SCO链路句柄
    uint16 l2cap_rest_len;
    uint8 acl_drop_next_flag;
    uint8 acl_stream_flag;
} bt_dev_status_t;
typedef struct
{
    uint8 bytes[BD_ADDR_LEN];
} t_bdaddr;

/*!
 * \brief
 *  bt_dev_info_t 提供给上层的远端蓝牙设备信息
 */
typedef struct
{
    /*! 已配对蓝牙设备地址 */
    t_bdaddr bd_addr;
    /*! 已配对蓝牙名称 */
    uint8 name[MGR_MAX_REMOTE_NAME_LEN];
} bt_dev_info_t;

//远程蓝牙设备 TWS 类型定义
typedef enum
{
    DEV_TYPE_NONE, NO_TWS_DEV, TWS_DEV,
} dev_type_e;

typedef struct
{
    bt_dev_info_t dev_info;
    uint8 linkKey[LINKKEY_SIZE];
    uint8 linkKeyType; /* MGR_COMBINATION | MGR_LOCALUNITKEY | MGR_REMOTEUNITKEY */
    uint8 mru_num;    //设备使用时长累计器，每次连接新设备，所有已配对设备的mru加1，mru最小表示最近使用，mru最大表示最长时间未使用过
    uint8 profile; //b0:hfp available b1:a2dp available b2:avrcp available b3:hid available b6:profile_valid b7:link_valid
    //需要记录当前位置的数据有效性，该byte的最高位表示有效性
    bt_dev_status_t *p_dev_status;
    //主要用于区分是对箱设备, 见 dev_type_e 定义
    uint8 dev_type :4;
    //当前对箱设备是信源还是信宿
    uint8 is_source :4;    //0-信宿(sink)1-信源(source)
    uint8 data_validity;    //0-数据已经失效 1-数据有效
} bt_dev_t;

typedef struct
{
    uint8 mru_num;    //设备使用时长累计器，每次连接新设备，所有已配对设备的mru加1，mru最小表示最近使用，mru最大表示最长时间未使用过
    uint8 profile;
    uint8 data_validity;    //0-数据已经失效 1-数据有效
} vram_devs_status_t;

typedef struct
{
    uint16 magic;
    uint16 tag;
    uint8 pl_len;
    uint8 paired_count;
    vram_devs_status_t devs_status[PAIRED_LIST_MAX];
} vram_devs_cfg_t;

typedef struct
{
    uint16 magic;
    uint8 paired_flag;
    uint8 paired_count;
    bt_dev_t tws_paired_list;
} tws_vram_info_t;

typedef struct
{
    //uint16 magic;
    uint16 tag; // 1 for used
    uint8 pl_len;
    bt_dev_t paired_list[PAIRED_LIST_MAX];
    bt_dev_t tws_paired_list;
} bt_devs_cfg_t;
//ble_attr,bitmap
#define  BLE_ATTR_BIT_SUPPORT_ENABLE    0x80  //ble's device support
//#define  BLE_ATTR_BIT_ADV_ENABLE        0x01  //ble's device visibility

enum
{
    UART_HCI_STATUS_RX_NULL, UART_HCI_STATUS_RX_TYPE, UART_HCI_STATUS_RX_HEADER, UART_HCI_STATUS_RX_DATA,
};

typedef struct
{
    bt_dev_t *dev;
    uint8 acl_pb_flag;
    uint8 sco_len;
    uint8 avdtp_header_len;
    uint8 g_pkg_status_flag;
    uint16 sco_hdl;
    uint16 acl_len;
    uint8 *avdtp_sound_buffer;
    uint16 avdtp_sound_len;
    uint16 event_len;
} T_BTDEV_HCI_CTRL;

struct tagBT_Descriptor
{
    uint32 preHciTime;
    T_BTDEV_HCI_CTRL hciCtrl;
};

typedef struct 
{
    uint8 stauts;
    uint16 acl_hdl;
    int8 rssi;
}
bt_bredr_rssi_st;

#endif
