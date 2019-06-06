/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ASQTͨ��Э�����Э�鶨�塣
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _AP_BLUETOOTH_ASQT_H
#define _AP_BLUETOOTH_ASQT_H

#include  "common_front_app_def.h"
#include  "bluetooth_common.h"
#include  "btcall_common.h"
#include  "stub_asqt_command.h"

typedef enum
{
    ASQT_STATUS_NULL,
    ASQT_STATUS_CONFIG,
    ASQT_STATUS_RUN,
    ASQT_STATUS_ERROR,
} asqt_state_e;

typedef enum
{
    sNotReady = 0, //��û׼���û����û�ֹͣ��
    sReady, //��д������ѡ�����ļ�
    sRunning, //��������������
    sUserStop, //�û�ֹͣ״̬
    sUserUpdate, //�û������˲���
    sUserStart, //�û����ڲ���״̬
} PC_curStatus_e;

#define SR_8KHZ  0
#define SR_16KHZ 1

typedef struct
{
    uint16 eq_point_nums;    //asqt֧�ֵ����PEQ����
    uint16 eq_speaker_nums;  //asqt PEQ Speaker PEQ������ʣ�ಿ������Mic PEQ����
    uint32 eq_version;       //PEQ�汾�ţ�0��ʾ��֧�ַ�������ͼ��MIC��ͼ��1��ʾ֧�ַ�������ͼ��MIC��ͼ
    uint8  fw_version[8];    //�̼��汾�� ��281B100��������0��
    uint8  sample_rate;      //������ 0��ʾ8KHz  1��ʾ16KHz
    uint8  reserve[15];      //reserve bytes
} asqt_config_t;

typedef struct
{
    uint32 peq_changed;
    uint32 first_block_nums;   //���14����
    uint32 second_block_nums;  //���Ϊ0����ôread_eq2_para(13h)����Ͳ��÷���
} asqt_peq_status_t;

typedef struct
{
    int16 freq;    //PEQ����˲�������Ƶ�ʻ����Ƶ�ʣ���Χ20 ~ 20000����λ�Ͳ���1Hz
    int16 gain;    //PEQ����˲�������ֵ��ȡֵ��Χ -120(-12db) ~ 120(12db)����λ�Ͳ���0.1db
    int16 Qval;    //PEQ����˲���Qֵ��ȡֵ��Χ 3(0.3) ~ 300(30.0)����λ�Ͳ���0.1
    int16 type;    //PEQ����˲������ͣ�0��ʾ�ޣ�1��ʾpeak��2��ʾhigh pass��3��ʾlow pass��
                   //4��ʾlow shelf��5��ʾhigh shelf
    int16 enable;  //PEQ���Ƿ�ʹ�ܣ�0��ʾ��ֹ��1��ʾSpeaker PEQ��2��ʾPost PEQ
    int16 reserved[1];  //�����ֽ�
} asqt_peq_point_t;

typedef struct
{
    int16 main_precut_value;         //����Ԥ˥��
    int16 equivalent_precut_value;   //��ЧԤ˥����ֻ�ڿ���MDRC����Ч
    int16 main_precut_enable;        //����Ԥ˥��ʹ��
    int16 peq_enable;                //PEQģ���Ƿ�ʹ��
    int16 peq_block_point;           //1st Block PEQ�������̶�Ϊ14
    int16 peq_total_point;           //PEQ�ܵ�������಻�ܳ���40����
    asqt_peq_point_t peq_array[14]; //PEQ���б�
} asqt_peq_data_t;

typedef struct
{
    int16 peq_block_point;   //2nd Block/3rd Block PEQ�������̶�Ϊ14��
    int16 peq_total_point;   //PEQ�ܵ�������಻�ܳ���40����
    int16 reserve[6];        //�����ֽ�
    asqt_peq_point_t peq_array[14];  //PEQ���б�
} asqt_peq_data_2_3_t;


//��״̬����ϲ�
typedef struct
{
    asqt_configs_t    cfg_data;
    uint32            volume_data;
    asqt_peq_status_t peq_status_data;
} asqt_configs_ext_t;


#if (SUPPORT_ASQT_TEST == 1)

extern asqt_state_e g_asqt_state;
extern PC_curStatus_e g_pc_status;
extern uint32 g_pc_state_timer;
extern uint8 g_pc_state_count;
extern uint8 g_pc_pa_volume;

extern asqt_configs_t g_asqt_configs;
extern asqt_dump_buf_t g_asqt_dump_buf;
extern asqt_dump_tag_t g_asqt_dump_tag;
extern buffer_rw_t *p_sco_input_buf;
extern uint16 g_dump_data_index;

extern buffer_rw_t g_sco_in_cache;
extern bool need_load_full;

extern asqt_config_t g_asqt_config;
extern uint8 g_asqt_config_count;

extern void asqt_loop_deal(void);

#endif

#endif

