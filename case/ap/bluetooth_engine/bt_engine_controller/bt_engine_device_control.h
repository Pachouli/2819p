/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
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
 用带重复发送的方式设置当前role switch的角色问题(master/slave)
 参数说明：
 bt_dev_t *dev 操作设备
 uint8 role 预期设定角色
 uint8 retry_invl_ms 尝试下一次设定的时间间隔（ms），建议值100ms
 uint8 retry_times 尝试次数,建议值3次
 */
extern void bt_set_cur_role_with_retry(bt_dev_t *dev, uint8 role, uint16 retry_invl_ms, uint8 retry_times);
extern void bt_role_switch_retry_deal(void);
extern void bt_role_change_ind(uint8 status, struct HCI_Switch_RoleStru *role_change);
/**
 *  获取远端设备rssi 
 * 输入参数  aclhdl   待获取rssi值的远端设备
 * rssi值将通过hci event获取到
 */
extern void bt_get_rmt_rssi(uint16 aclhdl);
#endif

#endif //endof #ifndef __BT_ENGINE_DEVICE_CONTROL_H__
