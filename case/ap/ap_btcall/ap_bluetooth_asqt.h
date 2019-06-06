/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASQT通信协议相关协议定义。
 * 作者：cailizhen
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
    sNotReady = 0, //还没准备好或者用户停止后
    sReady, //填写参数和选择了文件
    sRunning, //调试正常运行中
    sUserStop, //用户停止状态
    sUserUpdate, //用户更新了参数
    sUserStart, //用户正在测试状态
} PC_curStatus_e;

#define SR_8KHZ  0
#define SR_16KHZ 1

typedef struct
{
    uint16 eq_point_nums;    //asqt支持的最大PEQ点数
    uint16 eq_speaker_nums;  //asqt PEQ Speaker PEQ点数，剩余部分属于Mic PEQ点数
    uint32 eq_version;       //PEQ版本号，0表示不支持分喇叭视图和MIC试图，1表示支持分喇叭视图和MIC试图
    uint8  fw_version[8];    //固件版本号 “281B100，结束符0”
    uint8  sample_rate;      //采样率 0表示8KHz  1表示16KHz
    uint8  reserve[15];      //reserve bytes
} asqt_config_t;

typedef struct
{
    uint32 peq_changed;
    uint32 first_block_nums;   //最多14个点
    uint32 second_block_nums;  //如果为0，那么read_eq2_para(13h)命令就不用发了
} asqt_peq_status_t;

typedef struct
{
    int16 freq;    //PEQ点的滤波器中心频率或截至频率，范围20 ~ 20000，单位和步进1Hz
    int16 gain;    //PEQ点的滤波器增益值，取值范围 -120(-12db) ~ 120(12db)，单位和步进0.1db
    int16 Qval;    //PEQ点的滤波器Q值，取值范围 3(0.3) ~ 300(30.0)，单位和步进0.1
    int16 type;    //PEQ点的滤波器类型，0表示无，1表示peak，2表示high pass，3表示low pass，
                   //4表示low shelf，5表示high shelf
    int16 enable;  //PEQ点是否使能，0表示禁止，1表示Speaker PEQ，2表示Post PEQ
    int16 reserved[1];  //保留字节
} asqt_peq_point_t;

typedef struct
{
    int16 main_precut_value;         //主体预衰减
    int16 equivalent_precut_value;   //等效预衰减，只在开启MDRC后有效
    int16 main_precut_enable;        //主体预衰减使能
    int16 peq_enable;                //PEQ模块是否使能
    int16 peq_block_point;           //1st Block PEQ点数，固定为14
    int16 peq_total_point;           //PEQ总点数，最多不能超出40个点
    asqt_peq_point_t peq_array[14]; //PEQ点列表
} asqt_peq_data_t;

typedef struct
{
    int16 peq_block_point;   //2nd Block/3rd Block PEQ点数，固定为14个
    int16 peq_total_point;   //PEQ总点数，最多不能超出40个点
    int16 reserve[6];        //保留字节
    asqt_peq_point_t peq_array[14];  //PEQ点列表
} asqt_peq_data_2_3_t;


//读状态命令合并
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

