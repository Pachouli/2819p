/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������BLE֧��ͷ�ļ���
 * ���ߣ�huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_USER_PROFILE_H__
#define __BT_ENGINE_USER_PROFILE_H__

#include "bt_engine_le.h"

#define HDL_MASK     0x10
#define IDXTOHDL(x)  (x+HDL_MASK)
#define HDLTOIDX(x)  (x-HDL_MASK)

#define ERR_HDL       (-1)
#define EMPTY_HDL     (-2)

#define FAST_CONNECTION_INTERVAL_MIN (24)         //24*1.25ms = 30ms
#define FAST_CONNECTION_INTERVAL_MAX (36)         //36*1.25ms = 45ms
#define FAST_CONNECTION_SLAVE_LATENCY (0)          //
#define FAST_CONNECTION_TIMEOUT_MULTIPLIER (200) //200*10ms = 2s

typedef struct
{
    //for app
    uint32 hdl;
    passthp_event_cbk cbk;
    buffer_rw_t *rcv_buffer;
    receive_rcp_info_t rcv_info;
    struct le_gatt_attr *attr;
    uint8 notify_enable;

    //for profile
    struct GATT_ServiceStru *serv;
    struct GATT_CharacteristicStru *set_char;
    struct GATT_DescriptorStru *set_char_ccc;
    struct GATT_CharacteristicStru *get_char;

} passth_profile_st;


/**
 *  ע��͸��Э��
 * ���������
 * ble_passthp_create_param_st param
 *      struct passthp_reg_uuid_info_st* reg_uuid ����UUID��Ϣ
 *              service_uuid_str ��Ҫע��ķ���uuid����uuidΪ16λuuidʱ��uuid = 0x1234,uuid_type UUID_TYPE_16),��uuidΪ128λʱ��uuid = "                                                00003838-0000-1000-8000-00805F9B34FB",uuid_type UUID_TYPE_128����
 *              getchar_uuid_str ��Ҫע�����ڽ����ֻ�app�˷������ݵ����ԡ�
 *              setchar_uuid_str ��Ҫע���������ֻ�app�˷������ݵ����ԡ�
 *     passthp_event_cbk cbk ���ڷ���profile���¼�֪ͨ�Ļص�����
 *      shm_id ���ڻ����ֻ�app�����������ݵĹ����ڴ�
 * 
 * ����ֵ��TRUE ע��ɹ�
 *        FALSE ע��ʧ��
 */
extern int32 passthrough_profile_register(ble_passthp_create_param_st *param);

/**
 *  ��ע��͸��Э��
 *  �������:
 *          p_hdl ��Ҫ���ٵ�profile hdl
 *  ����ֵ:
 *          TRUE���ɹ���  FALSE:ʧ��
 */
extern bool passthrough_profile_unregister(uint32 p_hdl);

/**
 *  ���ֻ�app�˷�������
 * ���������hdl ע��passthrough�����ʱ�򷵻صĲ������
 * wdata ��Ҫ���͵������ڴ��ַ
 * len ��Ҫ���͵����ݳ��ȣ�bytes��
 * �����TRUE ���ͳɹ�  FALSE ����ʧ��
 */
extern bool passthp_send(uint32 hdl, uint8 *wdata, uint16 len);

/**
 *  ���ֻ�app�˷�������
 * ���������hdl ע��passthrough�����ʱ�򷵻صĲ������
 * rdata ��Ҫ���͵������ڴ��ַ
 * len ��Ҫ���͵����ݳ��ȣ�bytes��
 * �����TRUE ���ͳɹ�  FALSE ����ʧ��
 */
extern bool passthp_read(uint32 hdl, uint8 *rdata, uint16 len);

/**
 *  Profile��Ϣ��ѯ
 *  �������:
 *          p_hdl ��Ҫ���ٵ�profile hdl
 *          info_type ��Ҫ��ѯ����Ϣ����
 *  ����ֵ:
 *          TRUE���ɹ���  FALSE:ʧ��
 */
extern bool passthp_get_info(uint32 p_hdl,uint8 info_type,void *param);

#endif //end of #ifndef __BT_ENGINE_USER_PROFILE_H__
