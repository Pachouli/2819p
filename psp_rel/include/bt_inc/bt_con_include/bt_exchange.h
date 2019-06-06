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

#define HCI_ACL_HEADER_LEN          0x04  //ȥ��0x02��ֻ��Ҫacl handle|pb_flag|bc_flag(2B) / data_len(2B)
#define HCI_SCO_HEADER_LEN          0x04  //x(1B) / sco_handle|packet_status(2B) /data_len(1B)����ֻҪ3�ֽڣ�Ϊ�˺���payload��4�ֽڶ��룬����ǰ��Ԥ��4�ֽ�

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

//hci_data_list��һ��������Ϣ�б�
typedef struct
{
    uint16 request_ptr; //list����һ���ռ�¼��ƫ�Ƶ�ַ��request��
    uint16 parse_ptr;   //list����һ���������¼��ƫ�Ƶ�ַ��read��
    uint16 ready_ptr;   //list��׼�����ͼ�¼��ƫ�Ƶ�ַ��send��
    uint16 ready_len;   //list��׼�����ͼ�¼�����ݳ���
    uint16 used_len;    //list���������¼��С��ÿ�η�����һ����¼������ʱ��һ��������ɺ��һ��
    uint16 total_len;   //list���ѷ��ͼ�¼���ݳ��ȣ��ۼ�ÿһ����¼���͵����ݳ��ȣ�����ʱ�ۼӵ�ǰ���ݳ��ȣ�������ɺ��ͷŵ�ǰ���ݳ��ȣ�
    uint16 start_ptr;   //list����ʼλ��     acl:HCI_ACL_DATA_LIST_START  event:HCI_EVENT_DATA_LIST_START   sco:HCI_SCO_DATA_LIST_START
    uint16 max_ptr;     //list�д�������     acl:MAX_HCI_ACL_DATA_LIST    event:MAX_HCI_EVENT_DATA_LIST     sco:MAX_HCI_SCO_DATA_LIST
    uint16 max_len;     //list��������ݳ��� acl:HCI_ACL_DATA_MAX         event:HCI_EVENT_DATA_MAX          sco:HCI_SCO_DATA_MAX
} struct_hci_list_rw;


#endif/* _BT_EXCHANGE_H */

