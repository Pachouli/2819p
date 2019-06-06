/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������BLE֧��ͷ�ļ���
 * ���ߣ�huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_DEVICE_CONTROL_H__
#define __BT_ENGINE_DEVICE_CONTROL_H__

#include "bluetooth_engine.h"

#define ROLE_SWITCH_RETRY_ENABLE  1

#define ROLE_SWITCH_RETRY_INVL 100
#define ROLE_SWITCH_RETRY_TIMES 3

#if (ROLE_SWITCH_RETRY_ENABLE == 1)
typedef struct
{
    uint8 cur_role :1;
    uint8 expect_role :1;
    uint8 need_switch :1;
    uint8 switch_complete_cfm :1;
    uint8 reserve :4;
    uint8 try_count :4;
    uint8 try_times :4;
    uint16 invl_time;
    uint32 last_set_time;
    bt_dev_t* dev;
} role_switch_retry_st;

extern role_switch_retry_st role_witch_retry[MAX_DEVICE_LINK];

/*
 �ô��ظ����͵ķ�ʽ���õ�ǰrole switch�Ľ�ɫ����(master/slave)
 ����˵����
 bt_dev_t *dev �����豸
 uint8 role Ԥ���趨��ɫ
 uint8 retry_invl_ms ������һ���趨��ʱ������ms��������ֵ100ms
 uint8 retry_times ���Դ���,����ֵ3��
 */
extern void bt_set_cur_role_with_retry(bt_dev_t *dev, uint8 role, uint16 retry_invl_ms, uint8 retry_times);
extern void bt_role_switch_retry_deal(void);
extern void bt_role_change_ind(uint8 status, struct HCI_Switch_RoleStru *role_change);
/**
 *  ��ȡԶ���豸rssi 
 * �������  aclhdl   ����ȡrssiֵ��Զ���豸
 * rssiֵ��ͨ��hci event��ȡ��
 */
extern void bt_get_rmt_rssi(uint16 aclhdl);
#endif

#endif //endof #ifndef __BT_ENGINE_DEVICE_CONTROL_H__
