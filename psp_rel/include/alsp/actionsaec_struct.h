#ifndef _ACTIONSAEC_EXTERN_H_
#define _ACTIONSAEC_EXTERN_H_


#undef EVALUATION_RESULT_ONLY
#define EVALUATION_RESULT_ONLY


typedef struct
{
    int aec_enable;         //aec模块开关，1：打开，0：关闭
    int tail_length;        //回声尾长，单位 ms，必须为16的倍数,目前仅支持 32 ms
    //int bulk_delay;           //bulk delay
    /*int nlp_enable; 扩展参数 */
    short nlp_enable;       //非线性处理子模块开关，1：打开，0：关闭，为 1 时必须保证 aec_enable 为 1
    char  nlp_echolevel;     //default = 0
    char  nlp_speed;        //default = 0
    int nlp_aggressiveness; //非线性处理程度，整数0~15,越大代表非线性处理越厉害
    int dt_threshold;       //双讲近端语音保留程度，整数0~15，越大表示双讲是近端语音保留越多，但是可能会带来单讲的回声，需找个折中值。
    int aes_enable;         //残留回声抑制子模块开关，1：打开，0：关闭，为 1 时必须保证 aec_enable 为1
    int echo_suppress;      //回声抑制子模块单讲回声抑制比，单位dB， 0~40
    int echo_suppress_avtive; //回声抑制子模块双讲回声抑制比，单位dB， 0~40
} aec_prms_t;


typedef struct
{
    int ans_enable;         //降噪开关，1：打开，0：关闭
    int noise_suppress;     //噪声抑制比，单位dB， 0~25
} ns_prms_t;

typedef struct
{
    int cng_enable;     //舒适噪声开关，1：打开，0：关闭
    int noise_floor;    //舒适噪声水平，单位 dB Fs, 一般应小于-45 dB
} cng_prms_t;

typedef struct
{
    int frame_size;   //帧长时间，单位ms，目前仅支持 16ms 帧长
    int sampling_rate; //采样率，单位Hz，目前仅支持 8000Hz 采样率
    // aec prms
    aec_prms_t aec_prms;
    //ns prms
    ns_prms_t ns_prms;
    //cng prms
    cng_prms_t cng_prms;

    int analysis_mode; //测试模式，0表示模拟打电话模式，1表示打电话模式
    int half_mode; //半双工模式，0表示全双工，1表示半双工

} sv_prms_t;

#ifdef PERFORMANCE_ANALYSIS
/*非分析模式*/
#define ANALYSIS_MODENONE       0
/*只需要单讲的数据*/
#define ANALYSIS_MODEONLYST     1
/*没有双讲数据但是想获得双讲性能分析*/
#define ANALYSIS_MODEDTSIM      2
/*dump的数据含有双讲数据*/
#define ANALYSIS_MODEDTINSIDE   3

typedef struct
{
    int tic;                //initial converge time
    int erl;                //echo return loss
    int erle;               //echo return loss enhancement1 (caculate in time domain, may be related to SNR)
    int best_erle;              //echo return loss enhancement2 (caculate using coherence function, not correlated with SNR)
    int snr;                //signal to noise ratio
    int snri;               //SNR improvement
    int dt_threshold;       //be related to SNR
    /* following status data should be caculated in the interface module, not inside the AEC or NS*/
    int ardt;               //receive data attenuation during double talk, only meaningful when double talk occursand with the ANALYSIS_MODEDTSIM mode)
    int asdt;               //trasmit data attenuation during double talk, only meaningful when double talk occursand with the ANALYSIS_MODEDTSIM mode)
    int fnr;                //far to near ratio, only meaningful when double talk occurs and with the ANALYSIS_MODEDTSIM mode.
} sv_status_t;

typedef struct
{
    int tic;
    int erl;
    int erle1;
    int best_erle1;
    int erle2;
    int best_erle2;
    int snr;
    int dtt;
    int fartalk;
    int neartalk;
    int nearms;
    int fnr;
    int asdt;
    int ardt;
} aec_status_t;

typedef struct
{
    int asdt;
    int snri;
} ns_status_t;
#endif

#ifdef EVALUATION_RESULT_ONLY
typedef struct
{
    int nms_in;
    int nms_mic;
    int nms_echo;
    int nms_aec;
    int nms_ns;
    int nms_out;
    int maxrefenv;
    int refisaudible;
    int leak;
    int coeff;
    int dt;
} sv_info_t;

typedef struct
{
    int nms_in;
    int nms_mic;
    int nms_echo;
    int nms_aec;
    int maxrefenv;
    int refisaudible;
    int leak;
    int coeff;
    int dt;
} aec_status_t;

typedef struct
{
    int nms_ns;
} ns_status_t;

#endif

typedef enum
{
    SV_CMD_SET_CONFIGS = 0,
    SV_CMD_GET_CONFIGS,
    SV_CMD_GET_STATUS,
} sv_cmd_t;

#endif
