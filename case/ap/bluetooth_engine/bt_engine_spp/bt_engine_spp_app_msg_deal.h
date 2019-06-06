/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/
#ifndef __BT_ENGINE_SPP_APP_MSG_DEAL_H__
#define __BT_ENGINE_SPP_APP_MSG_DEAL_H__

#include "bluetooth_common.h"

#define BT_ENGINE_SPP_APP_MSG_DEAL 1


typedef struct
{
    //for app
    uint32 hdl;
    spp_event_cbk cbk;
    buffer_rw_t *rcv_buffer;
    receive_rcp_info_t rcv_info;
} spp_passthrough_st;

extern spp_passthrough_st spp_pass;

/**
 *  ע��͸��Э��
 * ���������
 * ble_passthp_create_param_st param
 * service_uuid_str ��Ҫע��ķ���uuid����uuidΪ16λuuidʱ��uuid = 0x1234,uuid_type UUID_TYPE_16),��uuidΪ128λʱ��uuid = "00003838-0000-1000-8000-00805F9B34FB",uuid_type UUID_TYPE_128����
 * getchar_uuid_str ��Ҫע�����ڽ����ֻ�app�˷������ݵ����ԡ�
 * setchar_uuid_str ��Ҫע���������ֻ�app�˷������ݵ����ԡ�
 * shm_id ���ڻ����ֻ�app�����������ݵĹ����ڴ�
 * 
 * ����ֵ��profile�������
 */
extern int32 spp_register(spp_reg_param_st *param);

/**
 *  ��ע��͸��Э��
 */
extern bool spp_unregister(int32 hdl);

/**
 *  ���ֻ�app�˷�������
 * ���������hdl ע��spp��ʱ�򷵻صĲ������
 * wdata ��Ҫ���͵������ڴ��ַ
 * len ��Ҫ���͵����ݳ��ȣ�bytes��
 * �����TRUE ���ͳɹ�  FALSE ����ʧ��
 */
extern bool spp_send(uint32 hdl, uint8 *wdata, uint16 len);

/**
 *  ���ֻ�app�˷�������
 * ���������hdl ע��spp��ʱ�򷵻صĲ������
 * rdata ��Ҫ���͵������ڴ��ַ
 * len ��Ҫ���͵����ݳ��ȣ�bytes��
 * �����TRUE ���ͳɹ�  FALSE ����ʧ��
 */
extern bool spp_read(uint32 hdl, uint8 *rdata, uint16 len);

/**
 *  Profile��Ϣ��ѯ
 *  �������:
 *          p_hdl ��Ҫ���ٵ�profile hdl
 *          info_type ��Ҫ��ѯ����Ϣ����
 *  ����ֵ:
 *          TRUE���ɹ���  FALSE:ʧ��
 */
extern bool spp_get_info(uint32 p_hdl,uint8 info_type,void *param);

/**
 *  spp���յ�Զ�˷������ݴ�����
 */
extern bool spp_rcv_rmt_data(uint8 *data, uint16 data_len);

#endif //end of #ifndef __BT_ENGINE_SPP_APP_MSG_DEAL_H__
