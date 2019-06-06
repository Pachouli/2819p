/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:29:29     1.0             build this file
 *******************************************************************************/
/*
 * \file     bt_src_interface.h
 * \brief    xiaomaky
 * \version 1.0
 * \date  2013/04/11
 *******************************************************************************/
#ifndef _BT_ENGINE_INTERFACE_H_
#define _BT_ENGINE_INTERFACE_H_

#include <typeext.h>
#include "btinit.h"
#include "datatype.h"
#include "global.h"
#include "hci_stru.h"

#include "a2dp_ui.h"
#include "hci_ui.h"
#include "btlist.h"
#include "avrcp_ui.h"
#include "avctp_ui.h"
#include "hfp_ui.h"
#include "sdap_uimacro.h"

#include "gap_ui.h"
#include "spp_app_ui.h"
#include "smp_ui.h"

// //add for ble
//#include "att_handler.h"
#include "att_ui.h"
#include "att_uuid.h"
#include "attr.h"
#include "gatt_com.h"
#include "hci_le.h"
#include "l2cap_ui.h"

#include "hid_ui.h"
#include "buff.h"

#include "bt_exchange.h"

#include "controller_test.h"

typedef enum
{
    BT_HW_NO_ERR = 0, BT_HW_UNKNOWN_ERR = 1,
} bt_hw_error_no_e;

typedef struct
{
    uint16 read_ptr;
    uint16 write_ptr;
    uint16 readable_len;
} struct_buffer_rw;

typedef struct
{
    uint32 log_mask;
    uint32 gpio_debug_mask;
    uint8 test_mode;
    uint8 rf_txpower_adjust :3;
    uint8 reserve[2];
} con_init_cfg_t;                       //host����������

typedef struct
{
    bt_hci_data_info_struct *p_hci_data_list;   //��¼hci������Ϣ�б�������acl/sco/event��
    struct_hci_list_rw hci_acl_data_list_rw;    //��¼acl����ϸ��Ϣ
    struct_hci_list_rw hci_event_data_list_rw;  //��¼event����ϸ��Ϣ
    struct_hci_list_rw hci_sco_data_list_rw;    //��¼sco����ϸ��Ϣ
    struct_buffer_rw hci_sco_data_buffer_rw;    //sco�Ļ���ռ����buff
    uint8 *p_hci_sco_data_buffer;               //ָ��Sco�Ļ���ռ�
    uint8 hci_sco_packet_size;                  //��¼sco������ռ��С
    uint8 need_RxReadToHCI;                     //��ȡhci�����־
    uint8 low_power_mode;                       //�͹���ģʽ���������˵����bt_con��//�͹���ģʽ��־0-�ǵ͹���;1-����͹���ģʽ��
} bt_hci_variable_struct;

typedef struct
{
    uint8 sp_hfp :1;
    uint8 con_clbrpara_enable :1;
    uint8 reserve :6;
} bt_init_para_t;

typedef struct
{
    //uint8 head
    uint8 sp_hfp :1;
    uint8 sp_cont_link_info :1;                       //�ֻ���һ�η�������ʱ�����������ʾ��ѡ�0��ʾĬ����Ϊ��1��ʾ������
    uint8 con_clbrpara_enable :1;
    uint8 support_cardplay_app :1; //�Ƿ�֧�ֲ忨����Ӧ��
    uint8 con_print_enable :2; //�Ƿ�ʹ��bt controller ��ӡ��Ϣ
    uint8 uart_print_enable :2; //�Ƿ�ʹ��Uart ��ӡ
    //uint8 tail

    //uint8 head
    uint8 con_test_mode :2;    //0-disable test mode 1-DUT_TEST 2-LE_TEST 3-DUT_TEST&LE_TEST  
    uint8 rf_txpower_adjust :3; //TX����
    unsigned char sco_rx_data_transfer_mode :1; // 1��ʾtx͸����0��ʾtx����
    unsigned char sco_tx_data_transfer_mode :1; // 1��ʾtx͸����0��ʾtx����
    uint8 reserve_bit :1;
    //uint8 tail

    uint8 bqb_test_flag;
    uint8 bqb_uart_idx; //BLE BQB UART�ڣ�0��ʾUart0��1��ʾUart1
    uint8 support_tws;
    uint32 head;
    uint32 total_size;
    uint8 bt_page_scan_act_windows_size;   //bt inquiry scan windows ��С��Ӱ�쵽���������ٶ�
} bt_cfg_info_t;

typedef struct
{
    uint8 cpt_flag;         //Ƶƫ������ɱ�־
    uint32 adc_data_addr;   //���adc���ݵĵ�ַ
    uint32 adc_data_len;    //adc���ݳ���
} test_result_t;

typedef struct st_t_dataBuf
{
    uint8 *buf;
    uint8 transport;
    uint16 dataLen;
    uint16 bufLen;
} t_DataBuf;

typedef struct
{
    uint8 con_print_enable;
} bt_drv_cfg_st;

//����ģʽnormal
typedef enum
{
    BT_NORMAL_MODE = 0,   //0-��������Ӧ��ģʽ;
    BT_TEST_ALL = 1,      //1-��������ģʽ�����������ȡ�
    BT_TEST_BTADDR = 2,   //2-ֻ�����޸�������ַ������
} test_mode_e;

//controller ����ģʽ
typedef enum
{
    DISABLE_TEST = 0,   //0-��������Ӧ��ģʽ;
    CON_DUT_TEST = 1,   //1-dut ����ģʽ,������������ģʽ
    CON_LE_TEST = 2,   //2-le ����,��Ҫ���ó�hci����Ȩ����������BQBģʽҲ������ģʽ
    CON_DUT_LE_TEST = 3, //3-Ĭ�Ͻ���DUT����ģʽ��CBT�ɷ���reset�����л���LE����ģʽ
} con_test_mode_e;

typedef enum
{
    SCO_EVENT = 0, SBC_EVENT = 1, OTHER_EVENT = 2, MAX_EVENT = 3,
} hci_event_e;

typedef enum
{
    CONTRL_DEFAULT_PRINT, CONTRL_BTPALY_PLAY_STATUS_IGNOR_HCI_EVENT_PRINT,
} hci_print_control_e;

typedef bt_dev_t* (*func_find_dev)(UINT16 acl_hdl);
typedef uint8 (*func_get_ignore_flag)(UINT8 mode, UINT32 para);
typedef void (*func_deal_acl_status)(void);
typedef UINT16 (*func_get_frame_data)(void *data_addr, uint16 data_len);
typedef void (*func_print_data)(uint8 *buf, int32 len, uint32 mode, uint8 print_control);
typedef void (*func_save_sco_data)(uint8 *buf, int32 len, uint8 pkg_status_flag);
typedef uint8 (*func_deal_linkkey_miss)(uint8 *buf);
typedef uint8 (*func_tws_rx_cmd_deal)(uint8 *buf, uint8 len);
typedef void (*func_tws_GetCmdData_Send)(uint8);
typedef void (*func_tws_deal_acl_handle)(uint16, uint8);
typedef uint32 (*func_tws_get_aclhandle_sendcount)(void);
typedef void (*func_hci_data_transmission_hook)(void);
typedef void (*func_hci_lowpower_switch_hook)(uint8);
//func_hci_role_change_ind �� func_hci_role_discover_complete �ȼۣ�ֻ��Ϊ�������ݾɰ汾
typedef void (*func_hci_role_change_ind)(uint16 connect_hdl,uint8 role);
typedef void (*func_hci_role_discover_complete)(uint16 connect_hdl, uint8 role);
typedef void (*func_printf)(const uint8 *fmt, ...);
typedef void (*func_hci_role_already_change_ind)(uint8 status, struct HCI_Switch_RoleStru *role_change);
typedef void (*func_hci_get_rssi_complete)(bt_bredr_rssi_st *rssi);

typedef struct
{
    func_find_dev FindDevByAclHdl;
    func_get_ignore_flag get_ignore_flag;
    func_deal_acl_status deal_acl_status;
    func_get_frame_data get_frame_data;
    func_save_sco_data save_sco_data;
    func_deal_linkkey_miss deal_linkkey_miss;
    func_tws_rx_cmd_deal tws_rx_cmd_deal;
    func_tws_GetCmdData_Send tws_GetCmdData_Send;
    func_tws_deal_acl_handle tws_deal_acl_handle;
    func_tws_get_aclhandle_sendcount tws_get_aclhandle_sendcount;
    func_hci_data_transmission_hook hci_data_transmission_hook;
    func_hci_lowpower_switch_hook hci_lowpower_switch_hook;
    func_hci_role_discover_complete hci_role_change_ind;
    func_printf stack_printf_log;       //log��ӡ������host��ӡ���ò�Ϊ0ʱ���Դ�ӡ
    func_printf stack_printf_debug;     //debug log��ӡ������host��ӡ���ô���2����ʱ�ſ��Դ�ӡ
    func_hci_role_already_change_ind hci_role_already_change_ind;
    func_hci_get_rssi_complete hci_get_rssi_complete;
} regiter_func_t;
#endif/* _BT_SRC_INTERFACE_H_ */

