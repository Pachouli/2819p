#ifndef _ACTIONSAEC_EXTERN_H_
#define _ACTIONSAEC_EXTERN_H_


#undef EVALUATION_RESULT_ONLY
#define EVALUATION_RESULT_ONLY


typedef struct
{
    int aec_enable;         //aecģ�鿪�أ�1���򿪣�0���ر�
    int tail_length;        //����β������λ ms������Ϊ16�ı���,Ŀǰ��֧�� 32 ms
    //int bulk_delay;           //bulk delay
    /*int nlp_enable; ��չ���� */
    short nlp_enable;       //�����Դ�����ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ 1
    char  nlp_echolevel;     //default = 0
    char  nlp_speed;        //default = 0
    int nlp_aggressiveness; //�����Դ���̶ȣ�����0~15,Խ���������Դ���Խ����
    int dt_threshold;       //˫���������������̶ȣ�����0~15��Խ���ʾ˫���ǽ�����������Խ�࣬���ǿ��ܻ���������Ļ��������Ҹ�����ֵ��
    int aes_enable;         //��������������ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ1
    int echo_suppress;      //����������ģ�鵥���������Ʊȣ���λdB�� 0~40
    int echo_suppress_avtive; //����������ģ��˫���������Ʊȣ���λdB�� 0~40
} aec_prms_t;


typedef struct
{
    int ans_enable;         //���뿪�أ�1���򿪣�0���ر�
    int noise_suppress;     //�������Ʊȣ���λdB�� 0~25
} ns_prms_t;

typedef struct
{
    int cng_enable;     //�����������أ�1���򿪣�0���ر�
    int noise_floor;    //��������ˮƽ����λ dB Fs, һ��ӦС��-45 dB
} cng_prms_t;

typedef struct
{
    int frame_size;   //֡��ʱ�䣬��λms��Ŀǰ��֧�� 16ms ֡��
    int sampling_rate; //�����ʣ���λHz��Ŀǰ��֧�� 8000Hz ������
    // aec prms
    aec_prms_t aec_prms;
    //ns prms
    ns_prms_t ns_prms;
    //cng prms
    cng_prms_t cng_prms;

    int analysis_mode; //����ģʽ��0��ʾģ���绰ģʽ��1��ʾ��绰ģʽ
    int half_mode; //��˫��ģʽ��0��ʾȫ˫����1��ʾ��˫��

} sv_prms_t;

#ifdef PERFORMANCE_ANALYSIS
/*�Ƿ���ģʽ*/
#define ANALYSIS_MODENONE       0
/*ֻ��Ҫ����������*/
#define ANALYSIS_MODEONLYST     1
/*û��˫�����ݵ�������˫�����ܷ���*/
#define ANALYSIS_MODEDTSIM      2
/*dump�����ݺ���˫������*/
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
