/*******************************************************************************
 *                              5003
 *                            Module: musicdec
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-18 15:00     1.0             build this file
*******************************************************************************/
/*!
 * \file     audio_decoder_lib_dev.h
 * \brief    ��Ƶ���������Ҫ��ͷ�ļ�
 * \author   kkli
 * \version 1.0
 * \date  2008/09/18
*******************************************************************************/
#ifndef __AUDIO_PROST_PROCESS_H__
#define __AUDIO_PROST_PROCESS_H__

#include "actal_libc_dev.h"
#include "./common/music_info.h"
#include "./common/storageio.h"
#include "./common/audiout_pcm.h"

#define EQ_NUM_BANDS 14

/*!
 * \brief
 *      ��������ص�����
 */
typedef enum
{
    /*! һ��δ֪���� */
    PP_RET_UNEXPECTED = -3,
    /*! �ڴ�ռ䲻�� */
    PP_RET_OUTOFMEMORY,
    /*! ��ʽ��֧�֣����ܼ������� */
    PP_RET_UNSUPPORTED,
    /*! ���� */
    PP_RET_OK,
    /*! �������ݲ��� */
    PP_RET_DATAUNDERFLOW,
} audiopp_ret_t;

/*!
 * \brief
 *      ����������������
 */
typedef enum
{
    /*�򿪣�������������淵�ؾ����ָ�����*/
    PP_CMD_OPEN = 0,
    /*�ر�*/
    PP_CMD_CLOSE,
    /*��ȡ״̬*/
    PP_CMD_GET_STATUS,
    /*���������pcm����ṹ��Ϣ�����ͣ�audiout_pcm_t *aout;*/
    PP_CMD_FRAME_PROCESS,
    /*���������DAE���п������*/
    PP_CMD_SET_DAE_PARA,
    /* �ز�������*/
    PP_CMD_RESAMPLE_PROCESS,
    /* ��ȡ���������RMS���� */
    PP_CMD_GET_ENERGY,
    /*��ȡƵ������*/
    PP_CMD_GET_FREQ_POINT_ENERGY,
} audiopp_ex_ops_cmd_t;

typedef enum
{
    /* fade in processing*/
    PP_FADE_IN_PROCESSING = 0,
    /* fade in standby(not working or processed)*/
    PP_FADE_IN_STANDBY,
    /* fade out processing*/
    PP_FADE_OUT_PROCESSING,
    /* fade out standby(not working or processed)*/
    PP_FADE_OUT_STANDBY,

    /* mic channal fade in processing*/
    PP_MIC_FADE_IN_PROCESSING,
    /* mic channal fade in standby(not working or processed)*/
    PP_MIC_FADE_IN_STANDBY,
    /* mic channal fade out processing*/
    PP_MIC_FADE_OUT_PROCESSING,
    /* mic channal fade out standby(not working or processed)*/
    PP_MIC_FADE_OUT_STANDBY,
} audiopp_status_e;

typedef struct
{
    short fade_in_status;                 //����״̬
    short fade_out_status;                //����״̬
    short mic_fade_in_status;             //MICͨ������״̬
    short mic_fade_out_status;            //MICͨ������״̬
} audiopp_status_t;

typedef struct
{
    short energy;                           //����ͨ����ǰ֡ƽ������
    short energy_max;                       //����ͨ����ǰ֡�������
    short mic_energy;                       //micͨ����ǰ֡ƽ������
    short mic_energy_max;                   //micͨ����ǰ֡ƽ������
} audiopp_energy_t;

struct eq_band_type
{
    /*����Ƶ�㣨���߸�ͨ�˲����Ľ�ֹƵ�ʣ�����λHz*/
    short cutoff;
    /*Qֵ����λ0.1���Ƽ���Χ��3��30*/
    short q;
    /*���棬��λ0.1db���Ƽ���Χ��-240��+120*/
    short gain;
    /*�˲������ͣ�0����Ч��1��peak��2��high pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf������ֵΪreserve*/
    short type;
};

//˫ͨ��֡�����������˵����
//����ͨ��ͨ�� audiout_pcm_t.pcm[0] ���룬�����˫��������Ҫ���������Ҵ�ţ���֧��16bitλ��
//MICͨ��ͨ��  audiout_pcm_t.pcm[1] ���룬�����˫��������Ҫ���������Ҵ�ţ���֧��16bitλ��

typedef struct
{
    int sample_rate;
    int pp_buf_addr[2];     //֡����buf��������������32bitλ�����ȷֱ��ǲ���������4����������
    short pp_enable;        //musicͨ��֡�����ܿ���
    short channels;
    short block_size;
    short bypass;
    short pre_soft_volume;
    short soft_volume;
    short fade_in_time_ms;
    short fade_out_time_ms;
    short fade_flag;        //1��ʾ���룬2��ʾ����
    short output_channel_config;//1:������������ 2: ���˶���Ӧ��(L=R=ԭʼ��(L/2 + R/2)), 0 ��������
                                //3:ֻ�����������4��ֻ���������
    short bit_width;
    short reserve_0;
}music_common_para_t;

typedef struct
{
    int sample_rate;
    int pp_buf_addr[2];     //֡����buf��������������32bitλ�����ȷֱ��ǲ���������4����������
    short pp_enable;        //micͨ��֡�����ܿ���
    short channels;
    short bypass;
    short pre_soft_volume;
    short soft_volume;
    short fade_in_time_ms;
    short fade_out_time_ms;
    short fade_flag;        //1��ʾ���룬2��ʾ����
    short bit_width;
    short output_channel_config;//1:������������ 2: ���˶���Ӧ��(L=R=ԭʼ��(L/2 + R/2)), 0 ��������
                                //3:ֻ�����������4��ֻ���������
}mic_common_para_t;

typedef struct
{
    short peq_change_flag;
    short music_peq_enable;
    short mic_peq_enable;
    short mic_peq_band_num; //micͨ��PEQ������������������棻����ǰ����������ͨ��
    short music_eq_id[EQ_NUM_BANDS];//0��ʾ�ö�EQ��ʹ��(�ڲ����ǻ�ͨ��gain�ж�)��1��ʾǰ��EQ��2��ʾpostEQ
    struct eq_band_type band_settings[EQ_NUM_BANDS];
}peq_para_t;

typedef struct
{
    short enable;
    short Limiter_threshold;
    short Limiter_attack_time;
    short Limiter_release_time;
}limiter_para_t;

typedef struct
{
    short echo_enable;
    short echo_delay;
    short echo_decay;
    short echo_lowpass_fc1;
    short echo_lowpass_fc2;
    short reserve_0; //for word aligned
}echo_para_t;

typedef struct
{
    short reverb_enable;
    short mRoomSize;                //�����С(%):0~100                 Ĭ�ϣ�75
    short mPreDelay;                //Ԥ�ӳ�(ms):0~100ms                Ĭ�ϣ�10
    short mReverberance;            //�����(%):0~100                   Ĭ�ϣ�50
    short mHfDamping;               //��Ƶ˥��(%):0~100                 Ĭ�ϣ�50
    short mToneLow;                 //����Ƶ�ʣ�32~523                  Ĭ�ϣ�32
    short mToneHigh;                //����Ƶ�ʣ�523~8372Hz              Ĭ�ϣ�8372
    short mWetGain;                 //ʪ��(������)��������ڣ�-60~10dB��Ĭ��-1dB
    short mDryGain;                 //����(ֱ����)��������ڣ�-60~10dB��Ĭ��-1dB
    short mStereoWidth;             //�������˲��������壬ֻ֧�ֵ���������0����
    short mWetOnly;                 //1��ʾֻ��ʪ����Ĭ��Ϊ0
    short reserve_0;
}reverb_para_t;

//acoutic feedback cancellation(������������Х������)
typedef struct
{
    /**********************************/
    //��Ƶ+����Ӧ�ݲ���ȥХ��
    short freq_shift_enable;
    short nhs_enable;
    short fft_th1;                          //Х�м������1��Ĭ��16dB������1dB������Ϊ14dB
    short fft_th2;                          //Х�м������2��Ĭ��33dB������1dB������Ϊ30dB
    short attack_time;                      //DRC����ʱ�䣬��Χ����ͬlimiter����������Ϊ0.1ms����Ϊ10
    short release_time;                     //DRC�ͷ�ʱ�䣬��Χ����ͬlimiter����������Ϊ500ms
    short DRC_dB;                               //DRCѹ���ķ��ȣ���λ1dB����Ϊ36dB
    short reserve_0;
}afc_para_t;

//Ƶ����ʾ�㷨
typedef struct
{
    short freq_display_enable;
    short freq_point_num;
    short freq_point[10];
} freq_display_para_t;

typedef struct
{
    short pitch_shift_enable;
    short pitch_level;              //-12~12,0��ʾ�����������ʾ����������ʾ����
}pitch_shift_para_t;

typedef struct
{
    short voice_removal_enable;     //������������
    short resever_0;
}voice_removal_para_t;


typedef struct
{
    void *src_addr;
    void *dst_addr;
    short src_len;
    short dst_len;
    short channel;
    short reserve_0;
}resample_inout_para_t;

typedef struct
{
    int resample_enable;
    int src_sample_rate;
    int dst_sample_rate;
}resample_para_t;

typedef struct
{
    short noise_gate_enable;        /* ʹ�� */
    short noise_gate_threshold;     /* ���ޣ����ڸ�����˥����default:-750 */
    short noise_gate_attack_time;   /* ��ʼ˥��������ʱ�䣬default:1000 */
    short noise_gate_release_time;  /* ����˥�����ͷ�ʱ�䣬default:10 */
    short noise_gate_ratio;         /* ˥��������default:5 */
    short set0_threshold;           /* ��˥����Ĳ���ֵǿ����0�����ޣ�default:0 */
}noise_gate_para_t;

typedef struct
{
    short threshold;
    unsigned short detect_attact_time;
    unsigned short detect_release_time;
    unsigned short attact_time;
    unsigned short release_time;

    unsigned short cutoff_freq;
    unsigned short q;
    unsigned short type;
} DRC_band_para_t;

typedef struct
{
    int DRC_change_flag;
    short DRC_enable;
    short DRC_crossover; //100~400Hz,step:5Hz
    DRC_band_para_t DRC_bands[2];
} DRC_para_t;

typedef struct
{
    short Ducking_enable;
    short side_chain_threshold;
    unsigned short rms_detection_time;
    unsigned short attack_time;
    unsigned short release_time;
    short main_chain_target_gain;
} ducking_effect_para_out_t;

/*for DAE AL*/
typedef struct
{
    music_common_para_t music_common_para;
    mic_common_para_t mic_common_para;
    peq_para_t peq_para;
    limiter_para_t music_limiter_para;
    limiter_para_t mic_limiter_para;
    echo_para_t echo_para;
    reverb_para_t reverb_para;
    afc_para_t afc_para;
    freq_display_para_t freq_display_para;
    pitch_shift_para_t pitch_shift_para;
    voice_removal_para_t voice_removal_para;
    resample_para_t resample_para;
    noise_gate_para_t noise_gate_para;
    DRC_para_t DRC_para;
    ducking_effect_para_out_t ducking_effect_para;
    /****************** OK MIC BUFFER *****************/
    unsigned int echo_buf[6]; //ÿ��Buffer 4K�����24KB
    unsigned int freq_shift_buf; //0x590
    /***********************************/
    short mic_mix_config_l; //������ͨ���������2��channels����������������mix�������������ķ�ʽ��0��ʾ��mix��1��ʾmix��2��ʾ�滻
    short mic_mix_config_r; //������ͨ���������2��channels����������������mix�������������ķ�ʽ��0��ʾ��mix��1��ʾmix��2��ʾ�滻
} PP_para_info_t;

#define AL_POST_PROCESS_OPS_ENTRY   (*((uint32*)(AL_PEQ_ENTRY_VA_START)))
//int post_process_ops(void *handel, audiopp_ex_ops_cmd_t cmd, unsigned int args);
#define post_process_ops(handel,cmd,args)   ((int(*)(void *, audiopp_ex_ops_cmd_t , uint32 ))AL_POST_PROCESS_OPS_ENTRY)((handel),(cmd),(args))

#define AL_RESAMPLE_OPEN_ENTRY      (*((uint32*)(AL_PEQ_ENTRY_VA_START + 4)))
#define AL_RESAMPLE_CLOSE_ENTRY     (*((uint32*)(AL_PEQ_ENTRY_VA_START + 8)))
#define AL_RESAMPLE_PROCCESS_ENTRY  (*((uint32*)(AL_PEQ_ENTRY_VA_START + 12)))

#define pp_resample_open(rin,rout,ch,temp_buf,table_buf) \
        ((void*(*)(int, int, int, short*, int*))AL_RESAMPLE_OPEN_ENTRY)((rin),(rout),(ch),(temp_buf),(table_buf))

#define pp_resample_close(hd)   ((void(*)(void*))AL_RESAMPLE_CLOSE_ENTRY)((hd))

#define pp_resample_proccess(hd,input,input_len,output) \
        ((int(*)(void *, short *, int, short *))AL_RESAMPLE_PROCCESS_ENTRY)((hd),(input),(input_len),(output))

#endif  // __PROST_PROCESS_H__
