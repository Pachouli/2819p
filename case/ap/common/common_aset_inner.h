/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ASET��Ч����ģ���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __COMMON_ASET_INNER_H
#define __COMMON_ASET_INNER_H

#include "common_ui_inner.h"
#include "common_aset.h"
#include "common_aset_command.h"

typedef enum
{
    /*ASET������PC������*/
    ASET_TOOLS_DISCONNECT = 0,
    /*ASET������������ģʽ*/
    ASET_TOOLS_WORK,
    /*ASET����δ��������PC��С������������*/
    ASET_TOOLS_NOT_WORK
} aset_tools_run_state_e;

//��¼ASET���ߵ�����״̬
typedef struct
{
    uint8 err_cnt :6; //7bit
    uint8 run_state :2;
} aset_tools_run_state_t;

typedef struct
{
    dae_config_t *p_dae_cfg;
    uint32 last_deal_timer;
    aset_tools_run_state_t run_state;
    uint8 *cmd_buffer; //malloc��̬����
    int16 main_gain_value;
    bool main_switch_flag;
} aset_var_t;

extern aset_var_t g_aset_var;


/*ģ���ڲ����ݽṹ�ͺ���*/

#define EQMAXPOINTCOUNT      14            //ÿ�������������14��PEQ��
#define EQMAXPOINTCOUNT_ALL  EQ_NUM_BANDS  //pc�����֧�ֵĵ���

typedef struct
{
    uint8 state; //0: not start 1:start
    uint8 upload_data; //0: not upload 1:upload
    uint8 volume_changed; //0:not changed 1:changed
    uint8 download_data; //0:not download 1:download

    uint8 upload_case_info; //0:not upload 1:upload
    uint8 main_switch_changed; //0:not changed 1:changed
    uint8 reserved[10];
    //ofs = 16
    uint8 eq_data_changed; //0:not change 1:change
    uint8 vbass_data_changed;
    uint8 te_data_changed;
    uint8 limiter_data_changed;
    // osf = 20
    uint8 reserved1[10];
    uint8 drc_data_changed;
    uint8 reserved2[1];
} aset_status_t;

//**********С���˵����ݽṹ********

typedef struct
{
    int16 freq; //EQƵ���Ƶ��
    int16 gain; //EQƵ�������ֵ
    int32 Qval; //EQƵ���Qֵ----תint
    int16 type; //EQƵ�������
    int8 enable; //EQƵ���Ƿ�ʹ�ܣ�0��ʾ��ֹ��1��ʾSpeaker PEQ��2��ʾPost PEQ
    int8 reserved[1]; //����
} aset_peq_point_t;

typedef struct
{
    int16 main_gain_value; //��������
    int16 equivalent_gain_value; //��Ч����
    int8 main_gain_enable; //��������ʹ��
    int8 peq_enable; //EQģ���Ƿ�ʹ��
    int8 nPointTotal; //����
    int8 MaxPointTotal; //������ 8bytes
    aset_peq_point_t peq_array[EQMAXPOINTCOUNT]; //���б� 12*14
} aset_peq_data_t;

typedef struct
{
    int8 nBlockPointTotal; //�ε������̶�Ϊ14��
    int8 nMaxPointTotal; //С���ϱ���������
    int8 szRev[2];
    aset_peq_point_t peq_array[EQMAXPOINTCOUNT]; //���б� 12*14
} aset_peq_data_2_3_t;

typedef struct
{
    int16 threhold; //��ֵ��-60:0.1:0dB����ʼ������-1dB
    int16 attack_time; //����ʱ�䣬.05:0.05:10ms����ֵ���󱶣�����Χ:5:1000����ʼ��Ϊ0.1ms
    int16 release_time; //�ͷ�ʱ�䣬:1:1000ms����ʼ��Ϊ300ms
    int8 limiter_enable; //ģ��ʹ��
    int8 reserved[1]; //����
} aset_limiter_data_t;

typedef struct
{
    drc_band_t drc_bands[2]; //����drc
    short input_gain;        //������
    short output_gain;       //���ʵ���
    short crossover;         //��Ƶ��Ƶ��Ƶ��
    char  szRev[25];         //����
    bool  bDrcEnable;        //DRC2.0ģ���Ƿ�ʹ��
} DRC_all_data_t;

typedef struct
{
    int8 aux_mode;    //0Ϊ��aux mode��1Ϊaux mode
    int8 reserved[7]; //����
} application_properties_t;

typedef struct
{
    int8 bEQ_v_1_0;
    int8 bVBass_v_1_0;
    int8 bTE_v_1_0;
    int8 bSurround_v_1_0;

    int8 bLimiter_v_1_0;
    int8 bMDRC_v_1_0;
    int8 bSRC_v_1_0;
    int8 bSEE_v_1_0;

    int8 bSEW_v_1_0;
    int8 bSD_v_1_0;
    int8 bEQ_v_1_1;
    int8 bMS_v_1_0;

    int8 bVBass_v_1_1;
    int8 bTE_v_1_1;
    int8 bEQ_v_1_2;
    int8 bMDRC_v_1_1;
    int8 bComPressor_v_1_0;
    int8 bMDRC_v_2_0;

    int8 szRev[118];
    int8 project_ver[8];//281B101
} aset_interface_config_t;

typedef struct
{
    int8 peq_point_num; //֧�ֵ�PEQ����
    int8 download_data_over; //�Ƿ��Ѷ�������
    int8 aux_mode;           //1Ϊaux��Ϊ��aux
    int8 max_volume;         //С���ϱ����������������ϱ�0����Ϊ��default��31������
    int8 reserved[28]; //�����ֽ�
    aset_interface_config_t stInterface;
} aset_case_info_t;

extern int32 aset_cmd_deal(aset_status_t *aset_status);
extern int32 aset_write_case_info(void);
extern int32 aset_upload_sound_param(void);

//inner function
extern bool aset_test_global_init(void);
extern void aset_test_global_exit(void);
extern void aset_test_loop_deal_inner(void);
extern int32 aset_switch_aux_mode_inner(uint8 mode);

#endif

