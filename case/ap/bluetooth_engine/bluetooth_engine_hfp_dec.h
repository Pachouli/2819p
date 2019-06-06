/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen   2014-7-28 10:52     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine_hfp_dec.h
 * \brief    cailizhen
 * \version  1.0
 * \date     2014-7-28
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_HFP_DEC_H
#define _BLUETOOTH_ENGINE_HFP_DEC_H

#include "bluetooth_engine_common.h"

//通话过程中获取当前的通话状态，用于解决有些手机(红米)没有收到call active导致上层状态错乱的问题
#define BT_SP_GETCALLINFO
#define NOISE_CONTROL
#define ENABLE_SIRI
#define SUPPORT_3_WAY_CALL      //是否支持3方通话

#define SCO_OUT_PAYLOAD_SIZE            120//发送SCO包负载大小
#define SCO_RECV_PAYLOAD_SIZE_MAX       120//接收SCO包负载最大大小 NOTE!!! 不能修改

//统计sco包数的单位时长
#define SCO_TRANS_PKG_TIME          (200)
//正常接收sco包时包数阈值大小
#define SCO_TRANS_PKG_NUM_THRESHOLD ((100*SCO_TRANS_PKG_TIME)/1000)

extern uint8 SCO_ERR_REPORT_BUF_ADDRESS[HFP_SCO_ERR_REPORT_BUF_LEN];

//最多缓冲16个数据包错误报告，有效包大小最小30字节
#define SCO_ERR_REPORT_COUNT            (HFP_SCO_ERR_REPORT_BUF_LEN/sizeof(sco_err_report_t))

extern uint8 SCO_OUTPUT_CACHE_ADDRESS[HFP_SCO_OUTPUT_CACHE_LEN]; //最大缓冲32ms@pcm/64ms@cvsd

extern uint8 SCO_RECEIVE_CACHE_ADDRESS[HFP_SCO_RECEIVE_CACHE_LEN]; //最大缓冲32ms@pcm/64ms@cvsd

typedef enum
{
    HFP_PB_DISABLE = 0, HFP_PB_ALL, HFP_PB_ONLY_NUM,
} hfp_pb_mode_e;

//每秒接收的sco包个数
typedef struct _sco_pkg_num_count_st
{
    //开始计算包个数的时间点
    uint32 last_time_stamp;
    //单位时间内传输sco包个数
    uint16 pkg_count:14;
    //表明sco包是否在传输,0表示未传输，1表示正在传输
    uint16 sco_data_in_last_1s:1;
}
sco_pncps_st;

typedef struct
{
    //应用中需要对sco获取的信息，包括当前sco使用场景、使用的编码格式以及tx/rx是否开启透传
    bt_sco_info_st sco_info;
    //sco连接状态
    uint8 sco_conn_status :1;        //0:未连接 1：已连接
    //是否过滤sco数据，1表示过滤，0，表示不过滤
    uint8 filter_sco_input_flag :1;  //过滤sco数据标志
    //远端发来的sco数据发送错误的统计
    uint8 sco_input_err_flag :1;     //暂未使用
    uint8 reserve1 :5;
    //创建sco链路的定时器id
    int8 create_sco_timer_id;
    //sco 句柄
    uint16 handle;
    //处理sco mic
    int8 data_deal_irq_timer_id;
    //过滤掉SCO前面15个包，可能这些包不是按照比特率来的，很快导致Buffer被填满
    uint8 filter_package_count;
    //统计单位时间内sco接收到的包个数
    sco_pncps_st pncps;
} sco_st;

typedef struct
{
    //hfp 工作状态（通话状态）
    //uint8 status;
    //三方通话工作状态
    //bt_3way_status three_way_status;
    //语音识别工作状态
    uint8 voice_recognition_status;
    //电话本信息状态
    //uint8 phone_status;
    //句柄
    HANDLE handle;
    //远端手机音量
    uint8 rmt_dev_vol;
    //是否允许同步音量到应用中标志，1表示允许，0表示不允许
    uint8 allow_sync_vol_to_ap_flag;
#ifdef BT_SP_GETCALLINFO
    //获取当前通话状态定时器id
    int get_call_status_timer_id;
#endif
#ifdef ENABLE_SIRI
    //查看SIRI是否已经处理完毕
    uint8 check_siri_answer;
#endif
    //保存电话号码，用于来电号码播报
    //uint8 phone_number[MAX_PHONENUM];
    //hfp 服务断开的原因
    uint8 disconnect_reason;
    sco_st sco;

#ifdef CONFIG_PHONEBOOK
uint8 *g_pbook_buf;
#endif
} hfp_st;

typedef struct
{
uint8 rx_sco_pkg_size; //HCI SCO包大小
uint8 g_pkg_status_flag; //SCO状态字
uint8 g_pkg_count; //当前子帧数，用于PLC帧大小调整
} btengine_sco_deal_struct;

extern void btengine_save_sco_data(uint8 *buf, int32 len, uint8 pkg_status_flag);
extern void bt_engine_hfp_sco_connect_ind_handle(void);
extern void bt_engine_hfp_sco_disconnect_ind_handle(void);
extern bool send_one_sco_package(void);
extern void hfp_thread_init(void);
extern void hfp_thread_exit(void);
extern void clear_sco_buf(void);
extern void hfp_standby_deal(void);
extern bt_read_phone_time_t *hfp_get_bt_read_phone_time(void);
extern void parse_hfp_cclk_cmd_param(uint8 *buf);
extern void sco_create_timeout_check_timer(void);
extern void sco_delete_timeout_check_timer(void);
extern int8 BT_HfpCheckSiriStatus(void);
extern void init_sco_pkg_count_insecond(void);
extern void sco_pkg_count_insecond(void);




#endif
