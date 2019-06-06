/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASET音效调试模块内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __COMMON_ASET_INNER_H
#define __COMMON_ASET_INNER_H

#include "common_ui_inner.h"
#include "common_aset.h"
#include "common_aset_command.h"

typedef enum
{
    /*ASET工具与PC无连接*/
    ASET_TOOLS_DISCONNECT = 0,
    /*ASET工具正常工作模式*/
    ASET_TOOLS_WORK,
    /*ASET工具未工作，但PC与小机已正常连接*/
    ASET_TOOLS_NOT_WORK
} aset_tools_run_state_e;

//记录ASET工具的运行状态
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
    uint8 *cmd_buffer; //malloc动态申请
    int16 main_gain_value;
    bool main_switch_flag;
} aset_var_t;

extern aset_var_t g_aset_var;


/*模块内部数据结构和函数*/

#define EQMAXPOINTCOUNT      14            //每个命令最多容纳14个PEQ点
#define EQMAXPOINTCOUNT_ALL  EQ_NUM_BANDS  //pc上最多支持的点数

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

//**********小机端的数据结构********

typedef struct
{
    int16 freq; //EQ频点的频率
    int16 gain; //EQ频点的增益值
    int32 Qval; //EQ频点的Q值----转int
    int16 type; //EQ频点的类型
    int8 enable; //EQ频点是否使能，0表示禁止，1表示Speaker PEQ，2表示Post PEQ
    int8 reserved[1]; //保留
} aset_peq_point_t;

typedef struct
{
    int16 main_gain_value; //主体增益
    int16 equivalent_gain_value; //等效增益
    int8 main_gain_enable; //主体增益使能
    int8 peq_enable; //EQ模块是否使能
    int8 nPointTotal; //点数
    int8 MaxPointTotal; //最大点数 8bytes
    aset_peq_point_t peq_array[EQMAXPOINTCOUNT]; //点列表 12*14
} aset_peq_data_t;

typedef struct
{
    int8 nBlockPointTotal; //段点数，固定为14个
    int8 nMaxPointTotal; //小机上报的最多点数
    int8 szRev[2];
    aset_peq_point_t peq_array[EQMAXPOINTCOUNT]; //点列表 12*14
} aset_peq_data_2_3_t;

typedef struct
{
    int16 threhold; //阈值，-60:0.1:0dB，初始化建议-1dB
    int16 attack_time; //启动时间，.05:0.05:10ms，传值扩大倍，即范围:5:1000，初始化为0.1ms
    int16 release_time; //释放时间，:1:1000ms，初始化为300ms
    int8 limiter_enable; //模块使能
    int8 reserved[1]; //保留
} aset_limiter_data_t;

typedef struct
{
    drc_band_t drc_bands[2]; //两段drc
    short input_gain;        //灵敏度
    short output_gain;       //功率调节
    short crossover;         //低频高频分频点
    char  szRev[25];         //保留
    bool  bDrcEnable;        //DRC2.0模块是否使能
} DRC_all_data_t;

typedef struct
{
    int8 aux_mode;    //0为非aux mode，1为aux mode
    int8 reserved[7]; //保留
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
    int8 peq_point_num; //支持的PEQ点数
    int8 download_data_over; //是否已读完数据
    int8 aux_mode;           //1为aux，为非aux
    int8 max_volume;         //小机上报最大音量级别，如果上报0则认为是default的31级音量
    int8 reserved[28]; //保留字节
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

