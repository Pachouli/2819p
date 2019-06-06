/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������������صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȣ�����������
 *       �䵱 BT ENGINE �� CASE ���м��˽�ɫ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BLUETOOTH_PASSTH_COMMON_H__
#define __BLUETOOTH_PASSTH_COMMON_H__

#define MAX_PROFILE_INSTANCE 2

enum
{
    PASSTHP_REGISTER_COMPLETE, //profileע�����
    PASSTHP_RMT_RXSTA_CHANGE,  //client����rx char notify ʹ��״̬
    PASSTHP_RMT_TXDATA_IND,    //client�������ݵ���
};

enum
{
    PASSTHP_RMT_RX_ENABLE,  //����rx char���Է���notify
    PASSTHP_RMT_RX_DISABLE, //����rx char�����Է���notify
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

//����ע��passthrough profileʱuuid����
struct uuid_string_st
{
    unsigned int *uuid;    //16λuuid��0x1234������128λuuid��"00003838-0000-1000-8000-00805F9B34FB"��
    unsigned char uuid_type; //UUID_TYPE_16 or UUID_TYPE_128
};

struct passthp_reg_uuid_info_st
{
    struct uuid_string_st service_uuid_str; //��Ҫ����
    struct uuid_string_st getchar_uuid_str; //����
    struct uuid_string_st setchar_uuid_str; //����
};

/**
 *  Profile�¼�֪ͨ�ص�����
 *  �������:
 *          hdl Profile Hdl
 *          ev Profile֪ͨ�¼�
 *          arg �¼�����
 *  ����ֵ:
 *          ��
 */
typedef void (*passthp_event_cbk)(unsigned int hdl, unsigned char ev, void *arg);

//����ע��passthrough profile
typedef struct ble_passth_profile_create_param_st
{
    struct passthp_reg_uuid_info_st* reg_uuid;
    passthp_event_cbk cbk;
    unsigned int shm_id;                    //�����ֻ�app�������ݵĹ����ڴ�ID
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
    SPP_RCV_RMT_DATA_IND, //spp�������ݵ���
};


/**
 *  Spp�¼�֪ͨ�ص�����
 *  �������:
 *          hdl spp Hdl
 *          ev spp֪ͨ�¼�������
 *          arg �¼�����
 *  ����ֵ:
 *          ��
 */
typedef void (*spp_event_cbk)(uint8 ev, void *arg);

//����ע��spp
typedef struct
{
    spp_event_cbk cbk;
    uint32 shm_id; //�����ֻ�app�������ݵĹ����ڴ�ID
} spp_reg_param_st;




#endif //end of #ifndef __BLUETOOTH_COMMON_H__

