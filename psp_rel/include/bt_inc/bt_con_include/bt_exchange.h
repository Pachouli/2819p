/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2015/1/16 13:42:15     1.0             build this file
 *******************************************************************************/
/*
 * \file     bt_exchange.h
 * \brief    xiaomaky
 * \version 1.0
 * \date  2015/01/16
 *******************************************************************************/
#ifndef _BT_EXCHANGE_H
#define _BT_EXCHANGE_H

#include <typeext.h>

#define BUFFSTRU_DATA_OFFSET        0x08


#define HCI_PACKET_TYPE_COMMAND     0x01
#define HCI_PACKET_TYPE_ACL         0x02
#define HCI_PACKET_TYPE_SCO         0x03
#define HCI_PACKET_TYPE_EVENT       0x04

#define HCI_ACL_HEADER_LEN          0x04  //去掉0x02，只需要acl handle|pb_flag|bc_flag(2B) / data_len(2B)
#define HCI_SCO_HEADER_LEN          0x04  //x(1B) / sco_handle|packet_status(2B) /data_len(1B)本来只要3字节，为了后面payload是4字节对齐，所以前面预留4字节

#define HCI_ACL_DATA_LIST_START     0x00
#define MAX_HCI_ACL_DATA_LIST       0x40
#define HCI_EVENT_DATA_LIST_START   MAX_HCI_ACL_DATA_LIST
#define MAX_HCI_EVENT_DATA_LIST     0x40
#define HCI_SCO_DATA_LIST_START     (MAX_HCI_ACL_DATA_LIST + MAX_HCI_EVENT_DATA_LIST)
#define MAX_HCI_SCO_DATA_LIST       0x30
#define MAX_HCI_DATA_LIST           (MAX_HCI_ACL_DATA_LIST + MAX_HCI_EVENT_DATA_LIST + MAX_HCI_SCO_DATA_LIST)

#define HCI_ACL_DATA_MAX            0x1B58    // 7k,  875*8
#define HCI_SCO_DATA_MAX            (MAX_HCI_SCO_DATA_LIST * 0x40)    // 4k,  48*64
#define HCI_EVENT_DATA_MAX          (MAX_HCI_EVENT_DATA_LIST * 0x40)    // 4k,  64*64

#define HCI_DATA_UNUSE              0x00
#define HCI_DATA_NOT_READY          0x01
#define HCI_DATA_READY              0x02

typedef struct
{
    uint8 pkt_data_flag;    //pkt status:HCI_DATA_UNUSE/HCI_DATA_NOT_READY/HCI_DATA_READY
    uint8 pkt_type;         
    uint16 pkt_len;
    uint8 *pkt_buf;
} bt_hci_data_info_struct;

//hci_data_list是一串数据信息列表
typedef struct
{
    uint16 request_ptr; //list中下一条空记录的偏移地址（request）
    uint16 parse_ptr;   //list中下一条需解析记录的偏移地址（read）
    uint16 ready_ptr;   //list中准备发送记录的偏移地址（send）
    uint16 ready_len;   //list中准备发送记录的数据长度
    uint16 used_len;    //list中已申请记录大小（每次发送算一条记录，申请时加一，处理完成后减一）
    uint16 total_len;   //list中已发送记录数据长度（累计每一条记录发送的数据长度，发送时累加当前数据长度，处理完成后释放当前数据长度）
    uint16 start_ptr;   //list中起始位置     acl:HCI_ACL_DATA_LIST_START  event:HCI_EVENT_DATA_LIST_START   sco:HCI_SCO_DATA_LIST_START
    uint16 max_ptr;     //list中大命令数     acl:MAX_HCI_ACL_DATA_LIST    event:MAX_HCI_EVENT_DATA_LIST     sco:MAX_HCI_SCO_DATA_LIST
    uint16 max_len;     //list中最大数据长度 acl:HCI_ACL_DATA_MAX         event:HCI_EVENT_DATA_MAX          sco:HCI_SCO_DATA_MAX
} struct_hci_list_rw;


#endif/* _BT_EXCHANGE_H */

