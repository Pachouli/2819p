/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙管理器相关的宏定义，枚举类型，结构体定义，外部接口声明等；蓝牙管理器
 *       充当 BT ENGINE 与 CASE 的中间人角色。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __BLUETOOTH_PASSTH_COMMON_H__
#define __BLUETOOTH_PASSTH_COMMON_H__

#define MAX_PROFILE_INSTANCE 2

enum
{
    PASSTHP_REGISTER_COMPLETE, //profile注册结束
    PASSTHP_RMT_RXSTA_CHANGE,  //client配置rx char notify 使能状态
    PASSTHP_RMT_TXDATA_IND,    //client发送数据到来
};

enum
{
    PASSTHP_RMT_RX_ENABLE,  //配置rx char可以发送notify
    PASSTHP_RMT_RX_DISABLE, //配置rx char不可以发送notify
};

typedef enum
{
    PASSTHP_GET_MTU_SIZE,
    PASSTHP_GET_NOTIFY_STATUS,
}BLE_PASSTHP_GET_INFO_TYPE;

typedef enum
{
    BT_PASSTH_BLE,
    BT_PASSTH_SPP,
} bt_passth_link_type;

//用于注册passthrough profile时uuid传入
struct uuid_string_st
{
    unsigned int *uuid;    //16位uuid（0x1234）或者128位uuid（"00003838-0000-1000-8000-00805F9B34FB"）
    unsigned char uuid_type; //UUID_TYPE_16 or UUID_TYPE_128
};

struct passthp_reg_uuid_info_st
{
    struct uuid_string_st service_uuid_str; //首要服务
    struct uuid_string_st getchar_uuid_str; //接收
    struct uuid_string_st setchar_uuid_str; //发送
};

/**
 *  Profile事件通知回调函数
 *  输入参数:
 *          hdl Profile Hdl
 *          ev Profile通知事件
 *          arg 事件参数
 *  返回值:
 *          无
 */
typedef void (*passthp_event_cbk)(unsigned int hdl, unsigned char ev, void *arg);

//用于注册passthrough profile
typedef struct ble_passth_profile_create_param_st
{
    struct passthp_reg_uuid_info_st* reg_uuid;
    passthp_event_cbk cbk;
    unsigned int shm_id;                    //接收手机app发送数据的共享内存ID
} ble_passthp_create_param_st;

typedef struct
{
    uint32 *hdl;
    void *arg;
} bt_managet_create_passth_param_st;

typedef struct
{
    uint32 hdl;
    void *data;
    uint16 len;
} bt_managet_data_wr_st;

typedef struct
{
    uint32 *hdl;
    BLE_PASSTHP_GET_INFO_TYPE type ;
    void *rdata;
} bt_managet_ble_passth_get_info_st;


enum
{
    SPP_RCV_RMT_DATA_IND, //spp发送数据到来
};


/**
 *  Spp事件通知回调函数
 *  输入参数:
 *          hdl spp Hdl
 *          ev spp通知事件；类型
 *          arg 事件参数
 *  返回值:
 *          无
 */
typedef void (*spp_event_cbk)(uint8 ev, void *arg);

//用于注册spp
typedef struct
{
    spp_event_cbk cbk;
    uint32 shm_id; //接收手机app发送数据的共享内存ID
} spp_reg_param_st;




#endif //end of #ifndef __BLUETOOTH_COMMON_H__

