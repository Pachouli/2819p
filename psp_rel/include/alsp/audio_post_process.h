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
 * \brief    音频解码库所需要的头文件
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
 *      定义后处理返回的类型
 */
typedef enum
{
    /*! 一般未知错误 */
    PP_RET_UNEXPECTED = -3,
    /*! 内存空间不够 */
    PP_RET_OUTOFMEMORY,
    /*! 格式不支持，不能继续解码 */
    PP_RET_UNSUPPORTED,
    /*! 正常 */
    PP_RET_OK,
    /*! 输入数据不够 */
    PP_RET_DATAUNDERFLOW,
} audiopp_ret_t;

/*!
 * \brief
 *      定义后处理命令的类型
 */
typedef enum
{
    /*打开，输入参数，保存返回句柄的指针变量*/
    PP_CMD_OPEN = 0,
    /*关闭*/
    PP_CMD_CLOSE,
    /*获取状态*/
    PP_CMD_GET_STATUS,
    /*输入参数：pcm输入结构信息，类型，audiout_pcm_t *aout;*/
    PP_CMD_FRAME_PROCESS,
    /*输入参数：DAE所有可设参数*/
    PP_CMD_SET_DAE_PARA,
    /* 重采样处理*/
    PP_CMD_RESAMPLE_PROCESS,
    /* 获取最大能量和RMS能量 */
    PP_CMD_GET_ENERGY,
    /*获取频点能量*/
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
    short fade_in_status;                 //淡入状态
    short fade_out_status;                //淡出状态
    short mic_fade_in_status;             //MIC通道淡入状态
    short mic_fade_out_status;            //MIC通道淡出状态
} audiopp_status_t;

typedef struct
{
    short energy;                           //播歌通道当前帧平均能量
    short energy_max;                       //播歌通道当前帧最大能量
    short mic_energy;                       //mic通道当前帧平均能量
    short mic_energy_max;                   //mic通道当前帧平均能量
} audiopp_energy_t;

struct eq_band_type
{
    /*中心频点（或者高通滤波器的截止频率），单位Hz*/
    short cutoff;
    /*Q值，单位0.1，推荐范围是3到30*/
    short q;
    /*增益，单位0.1db，推荐范围是-240到+120*/
    short gain;
    /*滤波器类型：0是无效，1是peak，2是high pass，3表示low pass，4表示low shelf，5表示high shelf，其它值为reserve*/
    short type;
};

//双通道帧处理输入参数说明：
//播放通道通过 audiout_pcm_t.pcm[0] 输入，如果是双声道，则要求左右左右存放；仅支持16bit位深
//MIC通道通过  audiout_pcm_t.pcm[1] 输入，如果是双声道，则要求左右左右存放；仅支持16bit位深

typedef struct
{
    int sample_rate;
    int pp_buf_addr[2];     //帧处理buf，分左右声道，32bit位宽，长度分别是采样对数的4倍，传进来
    short pp_enable;        //music通道帧处理总开关
    short channels;
    short block_size;
    short bypass;
    short pre_soft_volume;
    short soft_volume;
    short fade_in_time_ms;
    short fade_out_time_ms;
    short fade_flag;        //1表示淡入，2表示淡出
    short output_channel_config;//1:左右声道交换 2: 单端耳机应用(L=R=原始的(L/2 + R/2)), 0 不做处理
                                //3:只输出左声道，4：只输出右声道
    short bit_width;
    short reserve_0;
}music_common_para_t;

typedef struct
{
    int sample_rate;
    int pp_buf_addr[2];     //帧处理buf，分左右声道，32bit位宽，长度分别是采样对数的4倍，传进来
    short pp_enable;        //mic通道帧处理总开关
    short channels;
    short bypass;
    short pre_soft_volume;
    short soft_volume;
    short fade_in_time_ms;
    short fade_out_time_ms;
    short fade_flag;        //1表示淡入，2表示淡出
    short bit_width;
    short output_channel_config;//1:左右声道交换 2: 单端耳机应用(L=R=原始的(L/2 + R/2)), 0 不做处理
                                //3:只输出左声道，4：只输出右声道
}mic_common_para_t;

typedef struct
{
    short peq_change_flag;
    short music_peq_enable;
    short mic_peq_enable;
    short mic_peq_band_num; //mic通道PEQ点数，分配在数组后面；数组前面分配给播歌通道
    short music_eq_id[EQ_NUM_BANDS];//0表示该段EQ不使能(内部还是会通过gain判断)，1表示前置EQ，2表示postEQ
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
    short mRoomSize;                //房间大小(%):0~100                 默认：75
    short mPreDelay;                //预延迟(ms):0~100ms                默认：10
    short mReverberance;            //混响感(%):0~100                   默认：50
    short mHfDamping;               //高频衰减(%):0~100                 默认：50
    short mToneLow;                 //低切频率，32~523                  默认：32
    short mToneHigh;                //高切频率，523~8372Hz              默认：8372
    short mWetGain;                 //湿声(混响声)的增益调节，-60~10dB，默认-1dB
    short mDryGain;                 //干声(直达声)的增益调节，-60~10dB，默认-1dB
    short mStereoWidth;             //单声道此参数无意义，只支持单声道，设0即可
    short mWetOnly;                 //1表示只有湿声，默认为0
    short reserve_0;
}reverb_para_t;

//acoutic feedback cancellation(声反馈消除或啸叫抑制)
typedef struct
{
    /**********************************/
    //移频+自适应陷波器去啸叫
    short freq_shift_enable;
    short nhs_enable;
    short fft_th1;                          //啸叫检测门限1，默认16dB，精度1dB，现在为14dB
    short fft_th2;                          //啸叫检测门限2，默认33dB，精度1dB，现在为30dB
    short attack_time;                      //DRC启动时间，范围精度同limiter参数，现在为0.1ms，设为10
    short release_time;                     //DRC释放时间，范围精度同limiter参数，现在为500ms
    short DRC_dB;                               //DRC压缩的幅度，单位1dB，现为36dB
    short reserve_0;
}afc_para_t;

//频谱显示算法
typedef struct
{
    short freq_display_enable;
    short freq_point_num;
    short freq_point[10];
} freq_display_para_t;

typedef struct
{
    short pitch_shift_enable;
    short pitch_level;              //-12~12,0表示不变调，负表示降调，正表示升调
}pitch_shift_para_t;

typedef struct
{
    short voice_removal_enable;     //人声消除开关
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
    short noise_gate_enable;        /* 使能 */
    short noise_gate_threshold;     /* 门限，低于该门限衰减，default:-750 */
    short noise_gate_attack_time;   /* 开始衰减的启动时间，default:1000 */
    short noise_gate_release_time;  /* 结束衰减的释放时间，default:10 */
    short noise_gate_ratio;         /* 衰减比例，default:5 */
    short set0_threshold;           /* 对衰减后的采样值强制置0的门限，default:0 */
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
    unsigned int echo_buf[6]; //每块Buffer 4K，最多24KB
    unsigned int freq_shift_buf; //0x590
    /***********************************/
    short mic_mix_config_l; //按键音通道（如果是2个channels输入则用左声道）mix到播放左声道的方式，0表示不mix，1表示mix，2表示替换
    short mic_mix_config_r; //按键音通道（如果是2个channels输入则用右声道）mix到播放右声道的方式，0表示不mix，1表示mix，2表示替换
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
