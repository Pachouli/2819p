/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙管理器相关的宏定义，枚举类型，结构体定义，外部接口声明等；蓝牙管理器
 *       充当 BT ENGINE 与 CASE 的中间人角色。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __BLUETOOTH_COMMON_H__
#define __BLUETOOTH_COMMON_H__

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "bluetooth_passthrouth.h"

//蓝牙自动化测试
//#define bt_auto_test
#define bt_support_ble

#define SUPPORT_ONLY_BT_TEST  0    //0:关闭测试模式，正常使用  1:只支持BT和单元测试的特殊测试模式，会关闭许多如解码之类的部分

#define SUPPORT_BT_VISIBLE_CONFIG   0//支持配置蓝牙可见性方案

#define PIPE_OP_FILTER        0x01 //设置过滤标志
#define PIPE_OP_UNFILTER      0x02 //清除过滤标志
#define PIPE_OP_FLUSH         0x80 //清除缓冲区

/*
 SUPPORT_TWS_CONNECT_WHHEN_PHONE_CONNECTED
 1表示手机连接时，该音箱可以发起组队
 0表示手机连接时，该音箱无法发起组队
 */
#define SUPPORT_TWS_CONNECT_WHEN_PHONE_CONNECTED   0

typedef struct
{
    uint8 phone_status;
    uint8 * phonebuf;
} pb_data_t;

//#define SUPPORT_READ_PHONE_TIME //是否支持读取手机时间并设置日历和时间
enum
{
    REQ_DATA_INVALID = 0, RESP_DATA_VALID, RESP_CMD_NOT_SUPPORT, RESP_PARSE_ERR, BT_NO_READY,
};

typedef struct
{
    time_t time;        //time of hfp get form phone
    date_t date;
    uint8 replay; //reply for hfp read phone clock cmd status value: 0->data_invalid,1->data_avlid,2->cmd not support,3->bt_not_ready
    uint8 status;       //1:send cmd and wait response   0:get cmd response or init
} bt_read_phone_time_t;

typedef enum
{
    HFP_3WAY_RELEASE_HELD = 0, //release the held call
    HFP_3WAY_RELEASE_ACTIVE = 1, //release the current call
    HFP_3WAY_SWAP_CALL = 2, //held/active call swap
    HFP_3WAY_TRANSFORM_HELD = 3,
//transform held to active
} hfp_3way_handle_e;

//AVRCP 最大音量
#define AVRCP_VOLUME_MAX    0x7f
//HFP 最大音量
#define HFP_VOLUME_MAX      15
#define BT_MAC_NAME_LEN_MAX    30 /*9*3 + 2*/

#define MAX_BLE_NAME_LEN               30
#define UUID_STR_LEN                   36
#define MAX_USER_ADV_LEN               32

#define BQB_TEST_ENABLE         0x01
#define BQB_DELAY_REPORT_ENABLE        0x02
#define BQB_SPP_CREATE_CONNECT        0x04

#define MGR_MAX_REMOTE_NAME_LEN         16
#define BD_ADDR_LEN                     6
#define PIN_CODE_LEN                    6

#define BT_REMOTE_NAME_MAX_LEN           30
#define UTF8_HEAD_LEN                    3
#define APP_GET_REMOTE_NAME_MAX_LEN      (BT_REMOTE_NAME_MAX_LEN+UTF8_HEAD_LEN)

typedef enum
{
    ROLE_MASTER, ROLE_SLAVER, ROLE_NULL
} bt_role_e;

typedef enum
{
    PB_FIND_NAME, PB_NAME_VALID, PB_NUM_VALID
} pb_result_t;

enum
{
    STATUS_WAIT_PAIRED, //等待连接状态，蓝牙初始化后，或者连接后又断开，会置为此状态
    STATUS_LINKED, //连接状态，只要连上hfp或者a2dp任一种都会置为此状态
    STATUS_A2DP_PLAY, //音乐播放状态
    STATUS_A2DP_PAUSE, //音乐暂停状态
    STATUS_CALL_INCOMING, //来电状态
    STATUS_CALL_OUTGOING, //拨出电话状态
    STATUS_CALL_HFP, //通话状态，语音从音箱上出来
    STATUS_CALL_PHONE, //通话状态，语音从手机上出来
    STATUS_SIRI, //SIRI等语音助手或语音应用
    STATUS_NONE, //初始化状态
    STATUS_RECONNECTING,
    STATUS_LINKING,
    STATUS_PAIRED_IDLE,
    STATUS_PAIRED_BUSY,
    STATUS_ERROR,
};

enum
{
    DISABLE_FAST_SCAN, ENABLE_FAST_SCAN,
};

enum
{
    PAIRED_SP_SSP_LINKKEY,      //支持SSP并且已有LinkKey
    PAIRED_SP_SSP_NO_LINKKEY,   //支持SSP并且没有LinkKey
    PAIRED_NOT_SP_SSP,          //不支持SSP
};

typedef enum
{
    BT_ERR_NONE, BT_ERR_HW, BT_ERR_PAGE_TIMEOUT
} bt_err_e;
typedef enum
{
    THREE_WAY_HOLD_NOT_EXIST, THREE_WAY_NEW_CALL_IN, THREE_WAY_HOLD_EXIST,
} bt_3way_status;
typedef enum
{
    LOCAL_DEVICE, //本地设备，指音箱端
    REMOTE_DEVICE,
//远端设备，指手机或其他连接设备
} device_type;

typedef struct
{   //8 bit;
    uint16 sp_a2dp_cp :2; //是否支持A2DP内容保护，0-不支持，1-支持DTCP，2-支持SCMS-T
    uint16 sp_avdtp_aac :1; //是否支持AAC格式蓝牙推歌，0表示不支持，1表示支持
    uint16 sp_avrcp :1; //是否支持AVRCP 遥控服务，0表示不支持，1表示支持
    uint16 sp_avrcp_vol_syn :1; //是否支持AVRCP音量同步服务，0表示不支持，1表示支持
    uint16 sp_hfp :1; //是否支持HFP服务，0表示不支持，1表示支持
    uint16 sp_hfp_vol_syn :1; //是否支持HFP音量同步服务，0表示不支持，1表示支持
    uint16 sp_hfp_bat_report :1; //是否支持HFP电量上报服务，0表示不支持，1表示支持
    //8 bit
    uint16 sp_hid :1; //是否支持HID服务，0表示不支持，1表示支持
    uint16 sp_ble :1; //是否支持ble,0表示不支持，1表示支持
    uint16 sp_auto_connect :1; //是否支持自动回连功能:0-不支持,1-支持
    uint16 sp_hfp_3way_call :1; //是否支持三方通话，0表示不支持，1表示支持
    uint16 sp_spp :1; //是否支持spp服务，0表示不支持，1表示支持
    uint16 reserved_bit1 :3; //保留位

    uint8 max_pair_list_num :4; //最多保存几台设备连接信息
    uint8 sp_tws_link :1; //是否支持TWS对箱连接，0表示不支持，1表示支持
    uint8 sp_device_num :3; //支持同时连接几台设备

    uint8 tws_master_role_depending_on_key :1; //按键是否改变原来的主从关系，如果为0则保持原来的主从关系，如果为1则哪个按键哪个为主
    uint8 visual_cfg_mode :2; //0表示可见可连接，1表示配对后不可见可连接，2表示任何时候都不可见不可连接
    uint8 asqt_test_mode :1; //1表示ASQT测试模式，蓝牙堆空间调小让给ASQT DUMP BUFFER
    uint8 reserve2 :4;
} bt_manager_cfg_info_t; //蓝牙管理器配置信息

typedef struct
{
    uint8 *ble_name;
    uint8 *act_serv_uuid;
    uint8 *act_set_uuid;
    uint8 *act_get_uuid;
    uint8 le_ancs_sp :1; //support ancs profile
    uint8 le_tip_sp :1; //support tip profile
    uint8 le_passthp_sp :1; //support actions passthrou profile
    uint8 le_profile_sp_reserve :5;
    uint16 le_manufacture_id; //id for manufacture
    uint8 get_ble_cfg_info_complete;
    t_bdaddr local_addr;
} bt_manager_ble_cfg_t;

typedef struct
{
    uint8 sp_linkkey_miss_reject :1; //回连时倘若手机取消配对了，则不弹出提示框并自动停止连接选项，0表示默认行为，1表示不弹出
    uint8 sp_cont_link_info :1; //手机第一次发起连接时不弹出配对提示框选项，0表示默认行为，1表示不弹出
    uint8 sp_sniff_enable :1; //是否使能进入sniff，0表示禁止; 1表示使能
    uint8 sp_ssp_enable :1; //是否使能Secure Simple Pairing 0-表示禁止；1-表示使能
    uint8 sp_sbc_audio_quality :1; //SBC编码音频质量水平，0表示高质量，1表示中等质量
    uint8 reserve :3;
    uint16 sp_bqb_test;
} bt_stack_cfg_info_t; //蓝牙协议栈配置信息

typedef struct
{
    //uint8 head
    uint8 con_print_enable :2; //是否使用bt controller 打印信息
    uint8 uart_print_enable :2; //是否使用Uart 打印
    uint8 con_test_mode :2; //0-disable test mode 1-DUT_TEST 2-LE_TEST 3-DUT_TEST&LE_TEST
    uint8 con_clbrpara_enable :1;
    uint8 sco_postdeal_enable :1;
    //uint8 tail

    uint8 stack_print_enable; //0表示不打印，1表示打印

    //uint8 head
    uint8 rf_txpower_adjust :3; //TX功率
    uint8 ignore_uncall_sco :1; //是否过滤非通话状态下的SCO数据
    uint8 support_a2dp_flag :1; //是否支持蓝牙播歌
    uint8 support_btplay_flag :1; //是否支持蓝牙播歌
    uint8 att_test_flag :1; //是否为ATT测试模式
    uint8 reserved1 :1;
    //uint8 tail

    uint8 max_link_num; //支持同时连接几台设备
    uint32 class_device; //蓝牙设备类，高字节表示major service，中字节表示major device，低字节表示minor device
    uint8 pin_code[6];
} bt_common_cfg_t;

typedef struct
{
    uint8 connect_count_timeout :4; //超时断开自动回连次数，0表示一直回连下去，1~15表示回连有限次
    uint8 connect_count_startup :4; //开机第一次进入蓝牙回连次数
    uint8 connect_count_mode :4; //切MODE键进入蓝牙回连次数
    uint8 reserved_bit1 :4;
    uint8 connect_interval; //回连时间间隔,秒为单位
    uint8 connect_type; //回连类型 参考auto_connect_type_mode_e
    uint8 first_reconnect_after_btinit_flag :1; //初始化蓝牙后第一次自动回连的标志，1:开始回连  0:停止回连
    uint8 try_con_other_times :5; //尝试回连另外一个服务的次数限制，0表示一直连接，1~20表示有限次
    uint8 auto_connect_sequence :1; //主箱回连顺序选择，0表示先连手机，1表示先连组队
    uint8 enable_slave_connect_phone :1; //是否支持副箱回连手机，0表示不支持，1表示支持

    uint8 tws_connect_type; //回连类型 参考auto_connect_type_mode_e
    uint8 tws_connect_count_timeout :4; //tws超时断开自动回连次数，0表示一直回连下去，1~15表示回连有限次
    uint8 tws_connect_count_startup :4; //tws开机第一次进入蓝牙回连次数
    uint8 tws_connect_count_before_search :4; //tws搜索前回连已组队设备次数
    uint8 reserved_bit2 :4;

} bt_auto_connect_cfg_t; //蓝牙自动回连配置信息

typedef struct
{
    bt_common_cfg_t bt_common_cfg;
    bt_manager_cfg_info_t bt_manager_cfg_info;
    bt_stack_cfg_info_t bt_stack_cfg_info;
    bt_auto_connect_cfg_t bt_auto_connect_cfg;
} btengine_param_t;

typedef enum
{
    TWS_IDLE, //未组对状态
    TWS_PAIREING, //组对中
    TWS_PAIRED, //组对成功
    TWS_SEARCH,
//组对失败
} tws_paire_status_e;

typedef enum
{
    SCO_DATA_ENCODE_FORMAT_CVSD = 0, SCO_DATA_ENCODE_FORMAT_MSBC = 1,
} sco_data_encode_type_e;

typedef struct
{
    uint8 sence :5;
    uint8 encode_type :1;            //参考取值：sco_data_encode_type_e                
    uint8 tx_data_transfer_mode :1;  //参考取值：sco_data_transfer_mode_e
    uint8 rx_data_transfer_mode :1;  //参考取值：sco_data_transfer_mode_e
    uint8 sco_trans_status:1;//0 无数据传输 1：正在传输
} bt_sco_info_st;

typedef struct
{
    uint8 status;//0 表示已经获取失败或者未获取，为无效数据 1表示获取成功
    // rssi黄金距离为-56dbm<min_rssi>~-30dbm<max_rssi>,黄金距离内，rssi值为0，表示信号中
    //当rssi_value>max_rssi时，rssi = rssi_value - max_rssi;(rssi > 0,表示信号非常强)    
    //当rssi_value<min_rssi时，rssi = rssi_value - min_rssi;(rssi < 0，表示信号比较弱)    
    int8 rssi;
}
bt_classic_rssi_st;

typedef struct
{
    uint8 status; //引擎状态
    uint8 error_no; //错误类型
    uint8 service_connect; //服务的连接状况;bit[0-3];bit0-HFP连接;bit1-A2DP连接;bit2-AVRCP连接;bit3-HID连接
    uint8 need_auto_connect; //是否需要自动回连的profile,某个bit为0表示该服务已经连接，为1表示没有连接上。bit0-HFP;bit1-A2DP;bit2-AVRCP;bit3-HID
    uint8 isneed_reconnect_flag; //是否需要自动回连标志
    uint8 test_mode :4; //0-正常蓝牙应用模式;1-蓝牙测试模式，包括回连等。2-只测试修改蓝牙地址和名称
    uint8 cur_role :4; //当前小机充电的角色；0-master;1-slaver;2-未连接状态
    bt_err_e err_status; //出错信息
    uint8 phone_status; //来电电话本状态;0-只有来电信息;1-电话本名字有效;2-只有电话号码有效
    uint8 *phone_buf; //存放电话本信息的临时buf
    uint8 ble_connect_flag; //0表示ble未连接，1表示ble已连接
    uint8 avdtp_data_pending; //1表示有接收到AVDTP数据包，不管是否过滤都会置位
    uint8 avdtp_data_flag :1; //1表示最近5S收到AVDTP音频数据包
    uint8 sniff_flag :1; //sniff 状态;
    uint8 low_power_mode :1; //低功耗模式标志0-非低功耗;1-进入低功耗模式
    uint8 bt_in_idle :1; //蓝牙是否进入空闲状态。0：非空闲状态   1：空闲状态
    uint8 vol_syn_flag :2; //手机->音箱音量是否需要同步标志;0-不需要；1-直接同步远端的音量;2-当前音量加；3-当前音量减
    uint8 enter_hfp_flag :1; //是否进入免提状态标志
    uint8 sco_created_flag :1; //sco链路建立标志(只要是拔出电话或通电话时才有sco链路)
    uint8 remote_volume; //需要同步更新的手机音量
    bt_3way_status three_way_status; //3方通话状态 0：没有3方通话，1：存在3方通话
    uint8 dev_type; //主要用于区分是单箱还是对箱设备
    uint8 dev_role; //用于区分主箱,从箱
    uint8 tws_pair_status; //对箱当前状态
    bt_sco_info_st sco_info;//记录sco的包传输状态
    bt_classic_rssi_st phone_rssi;//记录手机的rssi强度
    bt_classic_rssi_st tws_rssi;//记录tws设备的rssi强度
} btengine_info_t;

typedef struct
{
    uint8 bt_name[56];
    uint8 bt_addr[6];
    uint8 bt_PairedAddr[6];
    uint8 bt_addr_type;
    uint8 bt_name_len;
    uint8 force_write;
    uint8 modify_mode; //修改模式:0-修改蓝牙名称；1-修改蓝牙地址； 2-同时修改名称和地址
    uint8 test_mode; //0:半成品测试 1:模组测试
} btengine_setinfo_t;

typedef struct
{
    uint16 magic; //VM 变量魔数
    t_bdaddr device_addr; //蓝牙地址
    uint8 device_name[BT_MAC_NAME_LEN_MAX + 1]; //设备名称
    uint8 burn_lock_flag; //蓝牙地址是否烧写标志位
} btengine_var_t;
typedef enum
{
    BTINFO_MODIFY_NAME_SUCCEED, //修改蓝牙名称成功
    BTINFO_MODIFY_ADDR_SUCCEED, //修改蓝牙地址成功
    BTINFO_MODIFY_ALL_SUCCEED, //修改蓝牙名称和地址成功
    BTINFO_MODIFY_ERROR, //蓝牙地址修改错误
    BTINFO_MODIFY_SKIP
//无需修改
} bt_modify_result_e;

typedef enum
{
    BT_MODIFY_NAME, //修改蓝牙名称
    BT_MODIFY_ADDR, //修改蓝牙地址
    BT_MODIFY_ALL
//修改蓝牙名称和地址
} bt_modify_mode;

typedef enum AUTO_CONNECT_TYPE_MODE_ENUM
{
    AUTO_CONNECT_TYPE_STARTUP = 0,      //开机回连
    AUTO_CONNECT_TYPE_MODE,             //按mode键切换应用后启用回连
    AUTO_CONNECT_TYPE_TIMEOUT,          //超时回连
    AUTO_CONNECT_TYPE_SEARCH,           //先TWS组队回连，失败后再进行查找设备组队
    AUTO_CONNECT_TYPE_CLOSE,            //回连结束后需要将type置为CLOSE
} auto_connect_type_mode_e;

//clear paired list mode
enum clear_pl_mode
{
    CLEAR_ALL_PHONE_PAIRED_INFO_ONLY = 0x1,       //仅清除手机配对信息
    CLEAR_TWS_DEV_PAIRED_INFO_ONLY = 0x2,         //仅清除TWS设备配对信息
    CLEAR_ALL_DEV_PAIRED_INFO = 0x3,              //同时清除手机配对信息以及TWS设备配对信息
};

#define HFP_SUPPORTED   0x01
#define A2DP_SUPPORTED  0x02
#define AVRCP_SUPPORTED 0x04
#define HID_SUPPORTED   0x08
#define PROFILE_VALID   0x80
#define LINKKEY_VALID   0x40

#define HFP_CONNECTED           0x01
#define A2DP_CONNECTED          0x02
#define AVRCP_CONNECTED_MY      0x04//与avrcp.h中t_AVRCP_STATE重名，改为AVRCP_CONNECTED_MY
#define HID_CONNECTED           0x08
#define BLE_CONNECTED           0x10
#define SPP_CONNECTED           0x20

#define HFP_DISCONNECTED        0xFE
#define A2DP_DISCONNECTED       0xFD
#define AVRCP_DISCONNECTED      0xFB
#define HID_DISCONNECTED        0xF7
#define BLE_DISCONNECTED        0xEF
#define SPP_DISCONNECTED        0xDF

#define VOLUME_ADJUST_MAX  13//调节音量映射表，最大音量可调范围为 0db ~ -12db
#define PAIRED_LIST_MAX         8
/*!
 * \brief
 *  btengine_list_mem_t 蓝牙引擎配对列表成员数据结构
 */
typedef struct
{
    /*! 成员当前位置，只读 */
    uint8 cur_index;
    /*! 成员改变后新位置，超出列表最大位置PAIRED_LIST_MAX-1时表示删除该成员 */
    uint8 new_index;
    /*! 数据是否有效，只读 */
    uint8 valid;
} btengine_list_mem_t;

/*!
 * \brief
 *  btengine_paired_list_t 蓝牙引擎设置配对列表数据结构
 */
typedef struct
{
    /*! 配对列表成员 */
    btengine_list_mem_t list_mem[PAIRED_LIST_MAX];
    /*! reason = 1时表示新增成员位置，超出列表最大位置PAIRED_LIST_MAX-1时表示不保存该信息
     *  reason = 2时表示需要更新的旧设备当前处于列表中的位置
     */
    uint8 index;
    /*! 仅在每次有连接时会更新配对列表，更新的原因有----1:添加新设备；2:更新旧设备*/
    uint8 reason;
} btengine_paired_list_t;

/*!
 * \brief
 *  bt_dev_info_t 提供给上层的已配对列表
 */
typedef struct
{
    /*! 蓝牙设备信息 */
    bt_dev_info_t dev[PAIRED_LIST_MAX];
    /*! 蓝牙设备个数 */
    uint8 dev_num;
} bt_paired_info_t;

/*!
 * \brief
 *  tws_dev_reconn_phone_policy_t 提供给上层控制tws设备停止回连手机
 */
typedef struct
{
    uint8 cancel_cur_create_connection :1; //1：退出当前正在连接的链路（如果存在） 0：不退出
    uint8 tws_paired_src_reconn_phone_enable :1; //1:关闭自动回连 0：不关闭自动回连
    uint8 reserve :6;
} tws_dev_reconn_phone_policy_t;

//app read btengine info status
enum read_status_e
{
    STATUS_READ_READY,
    STATUS_READ_FINISH,
    STATUS_READ_SUCCESS,
    STATUS_READ_ERROR_DEV_NOT_EXITS,
    STATUS_READ_ERROR_PARAM_ERR,
    STATUS_READ_ERROR_READ_LEN_ERR,
    STATUS_READ_ERROR_BTEG_GET_REMOTE_NAME_FAILED,
};

typedef struct
{
    uint8 phone_name[APP_GET_REMOTE_NAME_MAX_LEN];
    uint8 phone_name_max_len;
    uint8 read_status; //see read_status_e
} bt_get_phone_name_info_st;

#define BTENGINE_MSG_SIRI_START     0x01
#define BTENGINE_MSG_SIRI_END       0x00

#if 0
extern bool com_init_btengine_info_inner(btengine_var_t *p_btengine_var);
#endif
#define com_init_btengine_info(a)   ((bool(*)(btengine_var_t *))COM_COM_INIT_BTENGINE_INFO)((a))

#if 0
extern void com_bt_event_loop_inner(void);
#endif
#define com_bt_event_loop()         ((void(*)(void))COM_COM_BT_EVENT_LOOP)()

#if 0
extern void com_get_btengine_status_inner(btengine_info_t *p_bt_info);
#endif
#define com_get_btengine_status(a)  ((void(*)(btengine_info_t *))COM_COM_GET_BTENGINE_STATUS)(a)

/***************************************************************************************/

/*!
 * \brief
 *  btengine_volume_t 往蓝牙引擎设置音量值数据结构
 */
typedef struct
{
    /*! 音量值 */
    uint8 volume;
    /*! 同步音量给那个协议(A2DP or HFP)*/
    uint8 need_sync; //0-A2DP,1-HFP
} btengine_volume_t;

#define VISUAL_MODE_NODISC_NOCON   0 //设备不可见，不可连接
#define VISUAL_MODE_NODISC_CON     1 //设备不可见，可连接
#define VISUAL_MODE_DISC_NOCON     2 //设备可见，不可连接
#define VISUAL_MODE_DISC_CON       3 //设备可见，可连接
#if 0
extern bool com_btengine_set_visual_mode_inner(uint8 visual_mode);
#endif
#define com_btengine_set_visual_mode(a)       ((bool(*)(uint8))COM_COM_BTENGINE_SET_VISUAL_MODE)((a))

#define CON_DEAL_FORCE_DISCONNECT  0 //强制断开连接
#define CON_DEAL_GET_PIARED_LIST   1 //获取配对列表
#define CON_DEAL_CLR_PAIRED_LIST   2 //清除配对列表
#define CON_DEAL_CONNECT_LAST      3 //连接最近使用设备
#define CON_DEAL_CONNECT_ADDR      4 //连接指定地址设备
#define CON_DEAL_DISC_CONN_SWITCH  5 //断开连接与连接最近设用设备交替
#define CON_DEAL_TWS_LINK          6 //TWS 发起组队
#define TWS_LINK_SRC  0 //作为SRC组队
#define TWS_LINK_SINK 1 //作为SINK组队
#define CON_DEAL_TWS_UNLINK        7 //TWS 断开组队
#define CON_DEAL_SET_TWS_RECONN_PHONE_POLICY  8 //配置TWS设备（已组队）的回连策略，例如在line in tws中由于主箱回连手机占用太多资源会导致断音，此时必须禁止回连
#if 0
extern bool com_btengine_connect_deal_inner(uint8 mode, void *para);
#endif
#define com_btengine_connect_deal(a,b)        ((bool(*)(uint8,void *))COM_COM_BTENGINE_CONNECT_DEAL)((a),(b))

#if 0
extern bool com_btengine_set_sbc_play_inner(bool enter);
#endif
#define com_btengine_set_sbc_play(a)          ((bool(*)(bool))COM_COM_BTENGINE_SET_SBC_PLAY)((a))

#if 0
extern bool com_btengine_set_play_pipe_filter_inner(uint8 filter_opt);
#endif
#define com_btengine_set_play_pipe_filter(a)  ((bool(*)(uint8))COM_COM_BTENGINE_SET_PLAY_PIPE_FILTER)((a))

#if 0
extern bool com_btengine_set_hfp_call_inner(bool enter);
#endif
#define com_btengine_set_hfp_call(a)          ((bool(*)(bool))COM_COM_BTENGINE_SET_HFP_CALL)((a))

#if 0
extern bool com_btengine_set_sco_pipe_filter_inner(uint8 filter_opt);
#endif
#define com_btengine_set_sco_pipe_filter(a)   ((bool(*)(uint8))COM_COM_BTENGINE_SET_SCO_PIPE_FILTER)((a))

#if 0
extern bool com_btengine_hid_take_photo_inner(void);
#endif
#define com_btengine_hid_take_photo()         ((void(*)(void))COM_COM_BTENGINE_HID_TAKE_PHOTO)()

#if 0
extern bool com_btengine_force_disable_sniff_inner(bool force_disable);
#endif
#define com_btengine_force_disable_sniff(a)   ((bool(*)(bool))COM_COM_BTENGINE_FORCE_DISABLE_SNIFF)((a))

#if 0
extern bool com_enter_btdbg_flag_inner(uint32 *btdbg_sel);
#endif
#define com_enter_btdbg_flag(a)               ((bool(*)(uint32 *))COM_COM_ENTER_BTDBG_FLAG)((a))

#if 0
extern void com_enter_duttest_check_inner(void);
#endif
#define com_enter_duttest_check()             ((void(*)(void))COM_COM_ENTER_DUTTEST_CHECK)()

typedef enum
{
    AVDTP_SOURCE_BT_A2DP = 0, AVDTP_SOURCE_LINEIN_ADC = 1,
} avdtp_data_source_e;

#ifdef ENABLE_TRUE_WIRELESS_STEREO

#define TWS_POS_MODE_ALWAYS_MASTER_LEFT  0 //主箱始终左箱，不可切换左右箱
#define TWS_POS_MODE_NEW_MASTER_LEFT     1 //初次组队或从副箱变为主箱时强制为左箱，可切换左右箱
#define TWS_POS_MODE_KEEP_LAST           2 //固定一对音箱的左右箱始终保持，不随主副箱关系变化而变化，可切换左右箱
#define TWS_POS_MODE                     TWS_POS_MODE_NEW_MASTER_LEFT

#define SUPPORT_LEFT_RIGHT_SWITCH        0 //是否支持按键切换左右箱

typedef struct
{
    /*! 获取蓝牙时间戳 */
    f_get_bt_clk tws_get_bt_clk;
    /*! 设置同步触发的蓝牙时间戳 */
    f_set_bt_clk tws_set_bt_clk;
    /*! 设置同步触发回调函数 */
    f_register_bt_clk_sync_cbk tws_reg_bt_clk_sync_cbk;
    /*! 是否发生AVDTP数据包丢失 */
    f_has_avdtp_pkg_lost tws_has_avdtp_pkg_lost;
} bt_clock_cbk_t;

#if 0
extern bool com_btengine_tws_get_bt_cbk_inner(bt_clock_cbk_t *p_bt_clock_cbk);
#endif
#define com_btengine_tws_get_bt_cbk(a)  ((bool(*)(bt_clock_cbk_t *))COM_COM_BTENGINE_TWS_GET_BT_CBK)((a))

#if 0
extern bool com_btengine_tws_get_send_frame_cbk_inner(f_send_frame *pp_f_send_frame);
#endif
#define com_btengine_tws_get_send_frame_cbk(a)  ((bool(*)(f_send_frame *))COM_COM_BTENGINE_TWS_GET_SEND_FRAME_CBK)((a))

#endif

//passthrouth module refer
#define PASSTH_SPP_SUPPORT_MASK 0x0001
#define PASSTH_BLE_SUPPORT_MASK 0x0100

typedef enum
{
    RCP_WITH_SPP, RCP_WITH_BLE, RCP_SERVER_MAX,
} rcp_server_e;

/*! 远程控制数据包（无格式），简写 rcp */
typedef struct
{
    /*! 数据包数据 */
    void * rcp_data;
    /*! 数据包长度 */
    uint16 rcp_len;
    //uint8_t reserve[2];
    /*! 区分服务 */
    uint8 rcp_flag;
    /*! 区分远端设备号 */
    uint8 rcp_remote;
} rmt_ctrl_pkg_t;

/*! 远程控制数据包（无格式），简写 rcp */
typedef struct
{
    /*! 数据包数据 */
    void * rcp_data;
    /*! 数据包长度 */
    uint16 rcp_len;
} passth_pkg_t;

typedef struct
{
    uint8 *rcp_data;
    rmt_ctrl_pkg_t *rcp_para;
} send_rcp_para_t;

typedef struct
{
    /* 本次接收到数据长度 */
    uint16 rcv_len;
    /* 连续丢包统计 */
    uint32 c_loss;
    /* 总共丢包统计 */
    uint32 t_loss;
} receive_rcp_info_t;

typedef struct
{
    UINT16 interval_min;
    UINT16 interval_max;
    UINT16 slave_latency;
    UINT16 timeout_multiplier;
} le_conn_param_cfg_st;

typedef void (*passthrough_receive_notify_cbk)(receive_rcp_info_t *receive_rcp_info);


typedef bool (*data_trans_dispatch_func)(bt_passth_link_type ltype, buffer_rw_t* buf);

#if 0
extern bool bt_manager_blespp_data_trans_register_dispatch_callback_inner(data_trans_dispatch_func f_data_trans_dispatch);
#endif
#define bt_manager_blespp_data_trans_register_dispatch_callback(a) \
        ((bool(*)(data_trans_dispatch_func))COM_BT_MANAGER_BLESPP_DATA_TRANS_REGISTER_DISPATCH_CALLBACK)((a))

#if 0
extern bool bt_manager_blespp_data_trans_send(uint8* p_data, uint16 data_len);
#endif
#define bt_manager_blespp_data_trans_send(a, b) \
        ((bool(*)(uint8*, uint16))COM_BT_MANAGER_BLESPP_DATA_TRANS_REGISTER_DISPATCH_CALLBACK)((a),(b))

#endif //__BLUETOOTH_COMMON_H__
