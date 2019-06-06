/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
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
 *  注册透传协议
 * 输入参数：
 * ble_passthp_create_param_st param
 *      struct passthp_reg_uuid_info_st* reg_uuid 服务UUID信息
 *              service_uuid_str 需要注册的服务uuid。当uuid为16位uuid时（uuid = 0x1234,uuid_type UUID_TYPE_16),当uuid为128位时（uuid = "                                                00003838-0000-1000-8000-00805F9B34FB",uuid_type UUID_TYPE_128）。
 *              getchar_uuid_str 需要注册用于接收手机app端发送数据的特性。
 *              setchar_uuid_str 需要注册用于向手机app端发送数据的特性。
 *     passthp_event_cbk cbk 用于发起profile中事件通知的回调函数
 *      shm_id 用于缓存手机app发送来的数据的共享内存
 * 
 * 返回值：TRUE 注册成功
 *        FALSE 注册失败
 */
extern int32 passthrough_profile_register(ble_passthp_create_param_st *param);

/**
 *  反注册透传协议
 *  输入参数:
 *          p_hdl 需要销毁的profile hdl
 *  返回值:
 *          TRUE：成功。  FALSE:失败
 */
extern bool passthrough_profile_unregister(uint32 p_hdl);

/**
 *  向手机app端发送数据
 * 输入参数：hdl 注册passthrough服务的时候返回的操作句柄
 * wdata 需要发送的数据内存地址
 * len 需要发送的数据长度（bytes）
 * 输出：TRUE 发送成功  FALSE 发送失败
 */
extern bool passthp_send(uint32 hdl, uint8 *wdata, uint16 len);

/**
 *  向手机app端发送数据
 * 输入参数：hdl 注册passthrough服务的时候返回的操作句柄
 * rdata 需要发送的数据内存地址
 * len 需要发送的数据长度（bytes）
 * 输出：TRUE 发送成功  FALSE 发送失败
 */
extern bool passthp_read(uint32 hdl, uint8 *rdata, uint16 len);

/**
 *  Profile信息查询
 *  输入参数:
 *          p_hdl 需要销毁的profile hdl
 *          info_type 需要查询的信息类型
 *  返回值:
 *          TRUE：成功。  FALSE:失败
 */
extern bool passthp_get_info(uint32 p_hdl,uint8 info_type,void *param);

#endif //end of #ifndef __BT_ENGINE_USER_PROFILE_H__
