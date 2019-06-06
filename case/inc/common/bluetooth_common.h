/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������������صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȣ�����������
 *       �䵱 BT ENGINE �� CASE ���м��˽�ɫ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BLUETOOTH_COMMON_H__
#define __BLUETOOTH_COMMON_H__

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "bluetooth_passthrouth.h"

//�����Զ�������
//#define bt_auto_test
#define bt_support_ble

#define SUPPORT_ONLY_BT_TEST  0    //0:�رղ���ģʽ������ʹ��  1:ֻ֧��BT�͵�Ԫ���Ե��������ģʽ����ر���������֮��Ĳ���

#define SUPPORT_BT_VISIBLE_CONFIG   0//֧�����������ɼ��Է���

#define PIPE_OP_FILTER        0x01 //���ù��˱�־
#define PIPE_OP_UNFILTER      0x02 //������˱�־
#define PIPE_OP_FLUSH         0x80 //���������

/*
 SUPPORT_TWS_CONNECT_WHHEN_PHONE_CONNECTED
 1��ʾ�ֻ�����ʱ����������Է������
 0��ʾ�ֻ�����ʱ���������޷��������
 */
#define SUPPORT_TWS_CONNECT_WHEN_PHONE_CONNECTED   0

typedef struct
{
    uint8 phone_status;
    uint8 * phonebuf;
} pb_data_t;

//#define SUPPORT_READ_PHONE_TIME //�Ƿ�֧�ֶ�ȡ�ֻ�ʱ�䲢����������ʱ��
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

//AVRCP �������
#define AVRCP_VOLUME_MAX    0x7f
//HFP �������
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
    STATUS_WAIT_PAIRED, //�ȴ�����״̬��������ʼ���󣬻������Ӻ��ֶϿ�������Ϊ��״̬
    STATUS_LINKED, //����״̬��ֻҪ����hfp����a2dp��һ�ֶ�����Ϊ��״̬
    STATUS_A2DP_PLAY, //���ֲ���״̬
    STATUS_A2DP_PAUSE, //������ͣ״̬
    STATUS_CALL_INCOMING, //����״̬
    STATUS_CALL_OUTGOING, //�����绰״̬
    STATUS_CALL_HFP, //ͨ��״̬�������������ϳ���
    STATUS_CALL_PHONE, //ͨ��״̬���������ֻ��ϳ���
    STATUS_SIRI, //SIRI���������ֻ�����Ӧ��
    STATUS_NONE, //��ʼ��״̬
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
    PAIRED_SP_SSP_LINKKEY,      //֧��SSP��������LinkKey
    PAIRED_SP_SSP_NO_LINKKEY,   //֧��SSP����û��LinkKey
    PAIRED_NOT_SP_SSP,          //��֧��SSP
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
    LOCAL_DEVICE, //�����豸��ָ�����
    REMOTE_DEVICE,
//Զ���豸��ָ�ֻ������������豸
} device_type;

typedef struct
{   //8 bit;
    uint16 sp_a2dp_cp :2; //�Ƿ�֧��A2DP���ݱ�����0-��֧�֣�1-֧��DTCP��2-֧��SCMS-T
    uint16 sp_avdtp_aac :1; //�Ƿ�֧��AAC��ʽ�����Ƹ裬0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_avrcp :1; //�Ƿ�֧��AVRCP ң�ط���0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_avrcp_vol_syn :1; //�Ƿ�֧��AVRCP����ͬ������0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_hfp :1; //�Ƿ�֧��HFP����0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_hfp_vol_syn :1; //�Ƿ�֧��HFP����ͬ������0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_hfp_bat_report :1; //�Ƿ�֧��HFP�����ϱ�����0��ʾ��֧�֣�1��ʾ֧��
    //8 bit
    uint16 sp_hid :1; //�Ƿ�֧��HID����0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_ble :1; //�Ƿ�֧��ble,0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_auto_connect :1; //�Ƿ�֧���Զ���������:0-��֧��,1-֧��
    uint16 sp_hfp_3way_call :1; //�Ƿ�֧������ͨ����0��ʾ��֧�֣�1��ʾ֧��
    uint16 sp_spp :1; //�Ƿ�֧��spp����0��ʾ��֧�֣�1��ʾ֧��
    uint16 reserved_bit1 :3; //����λ

    uint8 max_pair_list_num :4; //��ౣ�漸̨�豸������Ϣ
    uint8 sp_tws_link :1; //�Ƿ�֧��TWS�������ӣ�0��ʾ��֧�֣�1��ʾ֧��
    uint8 sp_device_num :3; //֧��ͬʱ���Ӽ�̨�豸

    uint8 tws_master_role_depending_on_key :1; //�����Ƿ�ı�ԭ�������ӹ�ϵ�����Ϊ0�򱣳�ԭ�������ӹ�ϵ�����Ϊ1���ĸ������ĸ�Ϊ��
    uint8 visual_cfg_mode :2; //0��ʾ�ɼ������ӣ�1��ʾ��Ժ󲻿ɼ������ӣ�2��ʾ�κ�ʱ�򶼲��ɼ���������
    uint8 asqt_test_mode :1; //1��ʾASQT����ģʽ�������ѿռ��С�ø�ASQT DUMP BUFFER
    uint8 reserve2 :4;
} bt_manager_cfg_info_t; //����������������Ϣ

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
    uint8 sp_linkkey_miss_reject :1; //����ʱ�����ֻ�ȡ������ˣ��򲻵�����ʾ���Զ�ֹͣ����ѡ�0��ʾĬ����Ϊ��1��ʾ������
    uint8 sp_cont_link_info :1; //�ֻ���һ�η�������ʱ�����������ʾ��ѡ�0��ʾĬ����Ϊ��1��ʾ������
    uint8 sp_sniff_enable :1; //�Ƿ�ʹ�ܽ���sniff��0��ʾ��ֹ; 1��ʾʹ��
    uint8 sp_ssp_enable :1; //�Ƿ�ʹ��Secure Simple Pairing 0-��ʾ��ֹ��1-��ʾʹ��
    uint8 sp_sbc_audio_quality :1; //SBC������Ƶ����ˮƽ��0��ʾ��������1��ʾ�е�����
    uint8 reserve :3;
    uint16 sp_bqb_test;
} bt_stack_cfg_info_t; //����Э��ջ������Ϣ

typedef struct
{
    //uint8 head
    uint8 con_print_enable :2; //�Ƿ�ʹ��bt controller ��ӡ��Ϣ
    uint8 uart_print_enable :2; //�Ƿ�ʹ��Uart ��ӡ
    uint8 con_test_mode :2; //0-disable test mode 1-DUT_TEST 2-LE_TEST 3-DUT_TEST&LE_TEST
    uint8 con_clbrpara_enable :1;
    uint8 sco_postdeal_enable :1;
    //uint8 tail

    uint8 stack_print_enable; //0��ʾ����ӡ��1��ʾ��ӡ

    //uint8 head
    uint8 rf_txpower_adjust :3; //TX����
    uint8 ignore_uncall_sco :1; //�Ƿ���˷�ͨ��״̬�µ�SCO����
    uint8 support_a2dp_flag :1; //�Ƿ�֧����������
    uint8 support_btplay_flag :1; //�Ƿ�֧����������
    uint8 att_test_flag :1; //�Ƿ�ΪATT����ģʽ
    uint8 reserved1 :1;
    //uint8 tail

    uint8 max_link_num; //֧��ͬʱ���Ӽ�̨�豸
    uint32 class_device; //�����豸�࣬���ֽڱ�ʾmajor service�����ֽڱ�ʾmajor device�����ֽڱ�ʾminor device
    uint8 pin_code[6];
} bt_common_cfg_t;

typedef struct
{
    uint8 connect_count_timeout :4; //��ʱ�Ͽ��Զ�����������0��ʾһֱ������ȥ��1~15��ʾ�������޴�
    uint8 connect_count_startup :4; //������һ�ν���������������
    uint8 connect_count_mode :4; //��MODE������������������
    uint8 reserved_bit1 :4;
    uint8 connect_interval; //����ʱ����,��Ϊ��λ
    uint8 connect_type; //�������� �ο�auto_connect_type_mode_e
    uint8 first_reconnect_after_btinit_flag :1; //��ʼ���������һ���Զ������ı�־��1:��ʼ����  0:ֹͣ����
    uint8 try_con_other_times :5; //���Ի�������һ������Ĵ������ƣ�0��ʾһֱ���ӣ�1~20��ʾ���޴�
    uint8 auto_connect_sequence :1; //�������˳��ѡ��0��ʾ�����ֻ���1��ʾ�������
    uint8 enable_slave_connect_phone :1; //�Ƿ�֧�ָ�������ֻ���0��ʾ��֧�֣�1��ʾ֧��

    uint8 tws_connect_type; //�������� �ο�auto_connect_type_mode_e
    uint8 tws_connect_count_timeout :4; //tws��ʱ�Ͽ��Զ�����������0��ʾһֱ������ȥ��1~15��ʾ�������޴�
    uint8 tws_connect_count_startup :4; //tws������һ�ν���������������
    uint8 tws_connect_count_before_search :4; //tws����ǰ����������豸����
    uint8 reserved_bit2 :4;

} bt_auto_connect_cfg_t; //�����Զ�����������Ϣ

typedef struct
{
    bt_common_cfg_t bt_common_cfg;
    bt_manager_cfg_info_t bt_manager_cfg_info;
    bt_stack_cfg_info_t bt_stack_cfg_info;
    bt_auto_connect_cfg_t bt_auto_connect_cfg;
} btengine_param_t;

typedef enum
{
    TWS_IDLE, //δ���״̬
    TWS_PAIREING, //�����
    TWS_PAIRED, //��Գɹ�
    TWS_SEARCH,
//���ʧ��
} tws_paire_status_e;

typedef enum
{
    SCO_DATA_ENCODE_FORMAT_CVSD = 0, SCO_DATA_ENCODE_FORMAT_MSBC = 1,
} sco_data_encode_type_e;

typedef struct
{
    uint8 sence :5;
    uint8 encode_type :1;            //�ο�ȡֵ��sco_data_encode_type_e                
    uint8 tx_data_transfer_mode :1;  //�ο�ȡֵ��sco_data_transfer_mode_e
    uint8 rx_data_transfer_mode :1;  //�ο�ȡֵ��sco_data_transfer_mode_e
    uint8 sco_trans_status:1;//0 �����ݴ��� 1�����ڴ���
} bt_sco_info_st;

typedef struct
{
    uint8 status;//0 ��ʾ�Ѿ���ȡʧ�ܻ���δ��ȡ��Ϊ��Ч���� 1��ʾ��ȡ�ɹ�
    // rssi�ƽ����Ϊ-56dbm<min_rssi>~-30dbm<max_rssi>,�ƽ�����ڣ�rssiֵΪ0����ʾ�ź���
    //��rssi_value>max_rssiʱ��rssi = rssi_value - max_rssi;(rssi > 0,��ʾ�źŷǳ�ǿ)    
    //��rssi_value<min_rssiʱ��rssi = rssi_value - min_rssi;(rssi < 0����ʾ�źűȽ���)    
    int8 rssi;
}
bt_classic_rssi_st;

typedef struct
{
    uint8 status; //����״̬
    uint8 error_no; //��������
    uint8 service_connect; //���������״��;bit[0-3];bit0-HFP����;bit1-A2DP����;bit2-AVRCP����;bit3-HID����
    uint8 need_auto_connect; //�Ƿ���Ҫ�Զ�������profile,ĳ��bitΪ0��ʾ�÷����Ѿ����ӣ�Ϊ1��ʾû�������ϡ�bit0-HFP;bit1-A2DP;bit2-AVRCP;bit3-HID
    uint8 isneed_reconnect_flag; //�Ƿ���Ҫ�Զ�������־
    uint8 test_mode :4; //0-��������Ӧ��ģʽ;1-��������ģʽ�����������ȡ�2-ֻ�����޸�������ַ������
    uint8 cur_role :4; //��ǰС�����Ľ�ɫ��0-master;1-slaver;2-δ����״̬
    bt_err_e err_status; //������Ϣ
    uint8 phone_status; //����绰��״̬;0-ֻ��������Ϣ;1-�绰��������Ч;2-ֻ�е绰������Ч
    uint8 *phone_buf; //��ŵ绰����Ϣ����ʱbuf
    uint8 ble_connect_flag; //0��ʾbleδ���ӣ�1��ʾble������
    uint8 avdtp_data_pending; //1��ʾ�н��յ�AVDTP���ݰ��������Ƿ���˶�����λ
    uint8 avdtp_data_flag :1; //1��ʾ���5S�յ�AVDTP��Ƶ���ݰ�
    uint8 sniff_flag :1; //sniff ״̬;
    uint8 low_power_mode :1; //�͹���ģʽ��־0-�ǵ͹���;1-����͹���ģʽ
    uint8 bt_in_idle :1; //�����Ƿ�������״̬��0���ǿ���״̬   1������״̬
    uint8 vol_syn_flag :2; //�ֻ�->���������Ƿ���Ҫͬ����־;0-����Ҫ��1-ֱ��ͬ��Զ�˵�����;2-��ǰ�����ӣ�3-��ǰ������
    uint8 enter_hfp_flag :1; //�Ƿ��������״̬��־
    uint8 sco_created_flag :1; //sco��·������־(ֻҪ�ǰγ��绰��ͨ�绰ʱ����sco��·)
    uint8 remote_volume; //��Ҫͬ�����µ��ֻ�����
    bt_3way_status three_way_status; //3��ͨ��״̬ 0��û��3��ͨ����1������3��ͨ��
    uint8 dev_type; //��Ҫ���������ǵ��仹�Ƕ����豸
    uint8 dev_role; //������������,����
    uint8 tws_pair_status; //���䵱ǰ״̬
    bt_sco_info_st sco_info;//��¼sco�İ�����״̬
    bt_classic_rssi_st phone_rssi;//��¼�ֻ���rssiǿ��
    bt_classic_rssi_st tws_rssi;//��¼tws�豸��rssiǿ��
} btengine_info_t;

typedef struct
{
    uint8 bt_name[56];
    uint8 bt_addr[6];
    uint8 bt_PairedAddr[6];
    uint8 bt_addr_type;
    uint8 bt_name_len;
    uint8 force_write;
    uint8 modify_mode; //�޸�ģʽ:0-�޸��������ƣ�1-�޸�������ַ�� 2-ͬʱ�޸����ƺ͵�ַ
    uint8 test_mode; //0:���Ʒ���� 1:ģ�����
} btengine_setinfo_t;

typedef struct
{
    uint16 magic; //VM ����ħ��
    t_bdaddr device_addr; //������ַ
    uint8 device_name[BT_MAC_NAME_LEN_MAX + 1]; //�豸����
    uint8 burn_lock_flag; //������ַ�Ƿ���д��־λ
} btengine_var_t;
typedef enum
{
    BTINFO_MODIFY_NAME_SUCCEED, //�޸��������Ƴɹ�
    BTINFO_MODIFY_ADDR_SUCCEED, //�޸�������ַ�ɹ�
    BTINFO_MODIFY_ALL_SUCCEED, //�޸��������ƺ͵�ַ�ɹ�
    BTINFO_MODIFY_ERROR, //������ַ�޸Ĵ���
    BTINFO_MODIFY_SKIP
//�����޸�
} bt_modify_result_e;

typedef enum
{
    BT_MODIFY_NAME, //�޸���������
    BT_MODIFY_ADDR, //�޸�������ַ
    BT_MODIFY_ALL
//�޸��������ƺ͵�ַ
} bt_modify_mode;

typedef enum AUTO_CONNECT_TYPE_MODE_ENUM
{
    AUTO_CONNECT_TYPE_STARTUP = 0,      //��������
    AUTO_CONNECT_TYPE_MODE,             //��mode���л�Ӧ�ú����û���
    AUTO_CONNECT_TYPE_TIMEOUT,          //��ʱ����
    AUTO_CONNECT_TYPE_SEARCH,           //��TWS��ӻ�����ʧ�ܺ��ٽ��в����豸���
    AUTO_CONNECT_TYPE_CLOSE,            //������������Ҫ��type��ΪCLOSE
} auto_connect_type_mode_e;

//clear paired list mode
enum clear_pl_mode
{
    CLEAR_ALL_PHONE_PAIRED_INFO_ONLY = 0x1,       //������ֻ������Ϣ
    CLEAR_TWS_DEV_PAIRED_INFO_ONLY = 0x2,         //�����TWS�豸�����Ϣ
    CLEAR_ALL_DEV_PAIRED_INFO = 0x3,              //ͬʱ����ֻ������Ϣ�Լ�TWS�豸�����Ϣ
};

#define HFP_SUPPORTED   0x01
#define A2DP_SUPPORTED  0x02
#define AVRCP_SUPPORTED 0x04
#define HID_SUPPORTED   0x08
#define PROFILE_VALID   0x80
#define LINKKEY_VALID   0x40

#define HFP_CONNECTED           0x01
#define A2DP_CONNECTED          0x02
#define AVRCP_CONNECTED_MY      0x04//��avrcp.h��t_AVRCP_STATE��������ΪAVRCP_CONNECTED_MY
#define HID_CONNECTED           0x08
#define BLE_CONNECTED           0x10
#define SPP_CONNECTED           0x20

#define HFP_DISCONNECTED        0xFE
#define A2DP_DISCONNECTED       0xFD
#define AVRCP_DISCONNECTED      0xFB
#define HID_DISCONNECTED        0xF7
#define BLE_DISCONNECTED        0xEF
#define SPP_DISCONNECTED        0xDF

#define VOLUME_ADJUST_MAX  13//��������ӳ�����������ɵ���ΧΪ 0db ~ -12db
#define PAIRED_LIST_MAX         8
/*!
 * \brief
 *  btengine_list_mem_t ������������б��Ա���ݽṹ
 */
typedef struct
{
    /*! ��Ա��ǰλ�ã�ֻ�� */
    uint8 cur_index;
    /*! ��Ա�ı����λ�ã������б����λ��PAIRED_LIST_MAX-1ʱ��ʾɾ���ó�Ա */
    uint8 new_index;
    /*! �����Ƿ���Ч��ֻ�� */
    uint8 valid;
} btengine_list_mem_t;

/*!
 * \brief
 *  btengine_paired_list_t ����������������б����ݽṹ
 */
typedef struct
{
    /*! ����б��Ա */
    btengine_list_mem_t list_mem[PAIRED_LIST_MAX];
    /*! reason = 1ʱ��ʾ������Աλ�ã������б����λ��PAIRED_LIST_MAX-1ʱ��ʾ���������Ϣ
     *  reason = 2ʱ��ʾ��Ҫ���µľ��豸��ǰ�����б��е�λ��
     */
    uint8 index;
    /*! ����ÿ��������ʱ���������б����µ�ԭ����----1:������豸��2:���¾��豸*/
    uint8 reason;
} btengine_paired_list_t;

/*!
 * \brief
 *  bt_dev_info_t �ṩ���ϲ��������б�
 */
typedef struct
{
    /*! �����豸��Ϣ */
    bt_dev_info_t dev[PAIRED_LIST_MAX];
    /*! �����豸���� */
    uint8 dev_num;
} bt_paired_info_t;

/*!
 * \brief
 *  tws_dev_reconn_phone_policy_t �ṩ���ϲ����tws�豸ֹͣ�����ֻ�
 */
typedef struct
{
    uint8 cancel_cur_create_connection :1; //1���˳���ǰ�������ӵ���·��������ڣ� 0�����˳�
    uint8 tws_paired_src_reconn_phone_enable :1; //1:�ر��Զ����� 0�����ر��Զ�����
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
 *  btengine_volume_t ������������������ֵ���ݽṹ
 */
typedef struct
{
    /*! ����ֵ */
    uint8 volume;
    /*! ͬ���������Ǹ�Э��(A2DP or HFP)*/
    uint8 need_sync; //0-A2DP,1-HFP
} btengine_volume_t;

#define VISUAL_MODE_NODISC_NOCON   0 //�豸���ɼ�����������
#define VISUAL_MODE_NODISC_CON     1 //�豸���ɼ���������
#define VISUAL_MODE_DISC_NOCON     2 //�豸�ɼ�����������
#define VISUAL_MODE_DISC_CON       3 //�豸�ɼ���������
#if 0
extern bool com_btengine_set_visual_mode_inner(uint8 visual_mode);
#endif
#define com_btengine_set_visual_mode(a)       ((bool(*)(uint8))COM_COM_BTENGINE_SET_VISUAL_MODE)((a))

#define CON_DEAL_FORCE_DISCONNECT  0 //ǿ�ƶϿ�����
#define CON_DEAL_GET_PIARED_LIST   1 //��ȡ����б�
#define CON_DEAL_CLR_PAIRED_LIST   2 //�������б�
#define CON_DEAL_CONNECT_LAST      3 //�������ʹ���豸
#define CON_DEAL_CONNECT_ADDR      4 //����ָ����ַ�豸
#define CON_DEAL_DISC_CONN_SWITCH  5 //�Ͽ�������������������豸����
#define CON_DEAL_TWS_LINK          6 //TWS �������
#define TWS_LINK_SRC  0 //��ΪSRC���
#define TWS_LINK_SINK 1 //��ΪSINK���
#define CON_DEAL_TWS_UNLINK        7 //TWS �Ͽ����
#define CON_DEAL_SET_TWS_RECONN_PHONE_POLICY  8 //����TWS�豸������ӣ��Ļ������ԣ�������line in tws��������������ֻ�ռ��̫����Դ�ᵼ�¶�������ʱ�����ֹ����
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

#define TWS_POS_MODE_ALWAYS_MASTER_LEFT  0 //����ʼ�����䣬�����л�������
#define TWS_POS_MODE_NEW_MASTER_LEFT     1 //������ӻ�Ӹ����Ϊ����ʱǿ��Ϊ���䣬���л�������
#define TWS_POS_MODE_KEEP_LAST           2 //�̶�һ�������������ʼ�ձ��֣������������ϵ�仯���仯�����л�������
#define TWS_POS_MODE                     TWS_POS_MODE_NEW_MASTER_LEFT

#define SUPPORT_LEFT_RIGHT_SWITCH        0 //�Ƿ�֧�ְ����л�������

typedef struct
{
    /*! ��ȡ����ʱ��� */
    f_get_bt_clk tws_get_bt_clk;
    /*! ����ͬ������������ʱ��� */
    f_set_bt_clk tws_set_bt_clk;
    /*! ����ͬ�������ص����� */
    f_register_bt_clk_sync_cbk tws_reg_bt_clk_sync_cbk;
    /*! �Ƿ���AVDTP���ݰ���ʧ */
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

/*! Զ�̿������ݰ����޸�ʽ������д rcp */
typedef struct
{
    /*! ���ݰ����� */
    void * rcp_data;
    /*! ���ݰ����� */
    uint16 rcp_len;
    //uint8_t reserve[2];
    /*! ���ַ��� */
    uint8 rcp_flag;
    /*! ����Զ���豸�� */
    uint8 rcp_remote;
} rmt_ctrl_pkg_t;

/*! Զ�̿������ݰ����޸�ʽ������д rcp */
typedef struct
{
    /*! ���ݰ����� */
    void * rcp_data;
    /*! ���ݰ����� */
    uint16 rcp_len;
} passth_pkg_t;

typedef struct
{
    uint8 *rcp_data;
    rmt_ctrl_pkg_t *rcp_para;
} send_rcp_para_t;

typedef struct
{
    /* ���ν��յ����ݳ��� */
    uint16 rcv_len;
    /* ��������ͳ�� */
    uint32 c_loss;
    /* �ܹ�����ͳ�� */
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
