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
 *  注册透传协议
 * 输入参数：
 * ble_passthp_create_param_st param
 * service_uuid_str 需要注册的服务uuid。当uuid为16位uuid时（uuid = 0x1234,uuid_type UUID_TYPE_16),当uuid为128位时（uuid = "00003838-0000-1000-8000-00805F9B34FB",uuid_type UUID_TYPE_128）。
 * getchar_uuid_str 需要注册用于接收手机app端发送数据的特性。
 * setchar_uuid_str 需要注册用于向手机app端发送数据的特性。
 * shm_id 用于缓存手机app发送来的数据的共享内存
 * 
 * 返回值：profile操作句柄
 */
extern int32 spp_register(spp_reg_param_st *param);

/**
 *  反注册透传协议
 */
extern bool spp_unregister(int32 hdl);

/**
 *  向手机app端发送数据
 * 输入参数：hdl 注册spp的时候返回的操作句柄
 * wdata 需要发送的数据内存地址
 * len 需要发送的数据长度（bytes）
 * 输出：TRUE 发送成功  FALSE 发送失败
 */
extern bool spp_send(uint32 hdl, uint8 *wdata, uint16 len);

/**
 *  向手机app端发送数据
 * 输入参数：hdl 注册spp的时候返回的操作句柄
 * rdata 需要发送的数据内存地址
 * len 需要发送的数据长度（bytes）
 * 输出：TRUE 发送成功  FALSE 发送失败
 */
extern bool spp_read(uint32 hdl, uint8 *rdata, uint16 len);

/**
 *  Profile信息查询
 *  输入参数:
 *          p_hdl 需要销毁的profile hdl
 *          info_type 需要查询的信息类型
 *  返回值:
 *          TRUE：成功。  FALSE:失败
 */
extern bool spp_get_info(uint32 p_hdl,uint8 info_type,void *param);

/**
 *  spp接收到远端发送数据处理函数
 */
extern bool spp_rcv_rmt_data(uint8 *data, uint16 data_len);

#endif //end of #ifndef __BT_ENGINE_SPP_APP_MSG_DEAL_H__
