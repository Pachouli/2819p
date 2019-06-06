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

//ͨ�������л�ȡ��ǰ��ͨ��״̬�����ڽ����Щ�ֻ�(����)û���յ�call active�����ϲ�״̬���ҵ�����
#define BT_SP_GETCALLINFO
#define NOISE_CONTROL
#define ENABLE_SIRI
#define SUPPORT_3_WAY_CALL      //�Ƿ�֧��3��ͨ��

#define SCO_OUT_PAYLOAD_SIZE            120//����SCO�����ش�С
#define SCO_RECV_PAYLOAD_SIZE_MAX       120//����SCO����������С NOTE!!! �����޸�

//ͳ��sco�����ĵ�λʱ��
#define SCO_TRANS_PKG_TIME          (200)
//��������sco��ʱ������ֵ��С
#define SCO_TRANS_PKG_NUM_THRESHOLD ((100*SCO_TRANS_PKG_TIME)/1000)

extern uint8 SCO_ERR_REPORT_BUF_ADDRESS[HFP_SCO_ERR_REPORT_BUF_LEN];

//��໺��16�����ݰ����󱨸棬��Ч����С��С30�ֽ�
#define SCO_ERR_REPORT_COUNT            (HFP_SCO_ERR_REPORT_BUF_LEN/sizeof(sco_err_report_t))

extern uint8 SCO_OUTPUT_CACHE_ADDRESS[HFP_SCO_OUTPUT_CACHE_LEN]; //��󻺳�32ms@pcm/64ms@cvsd

extern uint8 SCO_RECEIVE_CACHE_ADDRESS[HFP_SCO_RECEIVE_CACHE_LEN]; //��󻺳�32ms@pcm/64ms@cvsd

typedef enum
{
    HFP_PB_DISABLE = 0, HFP_PB_ALL, HFP_PB_ONLY_NUM,
} hfp_pb_mode_e;

//ÿ����յ�sco������
typedef struct _sco_pkg_num_count_st
{
    //��ʼ�����������ʱ���
    uint32 last_time_stamp;
    //��λʱ���ڴ���sco������
    uint16 pkg_count:14;
    //����sco���Ƿ��ڴ���,0��ʾδ���䣬1��ʾ���ڴ���
    uint16 sco_data_in_last_1s:1;
}
sco_pncps_st;

typedef struct
{
    //Ӧ������Ҫ��sco��ȡ����Ϣ��������ǰscoʹ�ó�����ʹ�õı����ʽ�Լ�tx/rx�Ƿ���͸��
    bt_sco_info_st sco_info;
    //sco����״̬
    uint8 sco_conn_status :1;        //0:δ���� 1��������
    //�Ƿ����sco���ݣ�1��ʾ���ˣ�0����ʾ������
    uint8 filter_sco_input_flag :1;  //����sco���ݱ�־
    //Զ�˷�����sco���ݷ��ʹ����ͳ��
    uint8 sco_input_err_flag :1;     //��δʹ��
    uint8 reserve1 :5;
    //����sco��·�Ķ�ʱ��id
    int8 create_sco_timer_id;
    //sco ���
    uint16 handle;
    //����sco mic
    int8 data_deal_irq_timer_id;
    //���˵�SCOǰ��15������������Щ�����ǰ��ձ��������ģ��ܿ쵼��Buffer������
    uint8 filter_package_count;
    //ͳ�Ƶ�λʱ����sco���յ��İ�����
    sco_pncps_st pncps;
} sco_st;

typedef struct
{
    //hfp ����״̬��ͨ��״̬��
    //uint8 status;
    //����ͨ������״̬
    //bt_3way_status three_way_status;
    //����ʶ����״̬
    uint8 voice_recognition_status;
    //�绰����Ϣ״̬
    //uint8 phone_status;
    //���
    HANDLE handle;
    //Զ���ֻ�����
    uint8 rmt_dev_vol;
    //�Ƿ�����ͬ��������Ӧ���б�־��1��ʾ����0��ʾ������
    uint8 allow_sync_vol_to_ap_flag;
#ifdef BT_SP_GETCALLINFO
    //��ȡ��ǰͨ��״̬��ʱ��id
    int get_call_status_timer_id;
#endif
#ifdef ENABLE_SIRI
    //�鿴SIRI�Ƿ��Ѿ��������
    uint8 check_siri_answer;
#endif
    //����绰���룬����������벥��
    //uint8 phone_number[MAX_PHONENUM];
    //hfp ����Ͽ���ԭ��
    uint8 disconnect_reason;
    sco_st sco;

#ifdef CONFIG_PHONEBOOK
uint8 *g_pbook_buf;
#endif
} hfp_st;

typedef struct
{
uint8 rx_sco_pkg_size; //HCI SCO����С
uint8 g_pkg_status_flag; //SCO״̬��
uint8 g_pkg_count; //��ǰ��֡��������PLC֡��С����
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
