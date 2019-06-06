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

#define ENABLE_SBC_DROP_LOG   //ʹ��SBC�����������������¼

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
    void *stream_handle; //a2dp�����
    uint16 stream_cid;   //a2dp ��CID
    uint8 cp_active;     //���ݱ���ģʽ
    uint8 is_source;     //���ڽ綨a2dp�Ľ�ɫ1-src;0-snk
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
    uint8 a2dp_sub_status;      //a2dp��״̬:���ӣ����ţ���ͣ��
    uint8 hfp_sub_status;       //HFP��״̬:���ӣ����������磬�γ���
    uint8 service_connect;      //profile(hfp,a2dp,avrcp,hid��)���������״��
    uint8 need_auto_connect;    //�Ƿ���Ҫ�Զ�����
    int8 rssi;
    uint8 sniff_flag;           //�Ƿ����sniff��־
    uint8 phone_status;         //�绰����Ϣ״̬
    uint8 phone_number[MAX_PHONENUM];   //�绰���뻺��buf
    A2dpSourceStru a2dp_source; //a2dp ����Ϣ�ṹ
    AvrcpSourceStru avrcp_source;
    HANDLE g_hf_handle;         //hfp �������Ӿ��
    uint16 acl_handle;          //ACL��·���
    uint16 sco_handle_setup;    //SCO��·���
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
 *  bt_dev_info_t �ṩ���ϲ��Զ�������豸��Ϣ
 */
typedef struct
{
    /*! ����������豸��ַ */
    t_bdaddr bd_addr;
    /*! ������������� */
    uint8 name[MGR_MAX_REMOTE_NAME_LEN];
} bt_dev_info_t;

//Զ�������豸 TWS ���Ͷ���
typedef enum
{
    DEV_TYPE_NONE, NO_TWS_DEV, TWS_DEV,
} dev_type_e;

typedef struct
{
    bt_dev_info_t dev_info;
    uint8 linkKey[LINKKEY_SIZE];
    uint8 linkKeyType; /* MGR_COMBINATION | MGR_LOCALUNITKEY | MGR_REMOTEUNITKEY */
    uint8 mru_num;    //�豸ʹ��ʱ���ۼ�����ÿ���������豸������������豸��mru��1��mru��С��ʾ���ʹ�ã�mru����ʾ�ʱ��δʹ�ù�
    uint8 profile; //b0:hfp available b1:a2dp available b2:avrcp available b3:hid available b6:profile_valid b7:link_valid
    //��Ҫ��¼��ǰλ�õ�������Ч�ԣ���byte�����λ��ʾ��Ч��
    bt_dev_status_t *p_dev_status;
    //��Ҫ���������Ƕ����豸, �� dev_type_e ����
    uint8 dev_type :4;
    //��ǰ�����豸����Դ��������
    uint8 is_source :4;    //0-����(sink)1-��Դ(source)
    uint8 data_validity;    //0-�����Ѿ�ʧЧ 1-������Ч
} bt_dev_t;

typedef struct
{
    uint8 mru_num;    //�豸ʹ��ʱ���ۼ�����ÿ���������豸������������豸��mru��1��mru��С��ʾ���ʹ�ã�mru����ʾ�ʱ��δʹ�ù�
    uint8 profile;
    uint8 data_validity;    //0-�����Ѿ�ʧЧ 1-������Ч
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
