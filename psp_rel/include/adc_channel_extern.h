/*! \cond MMM_MP_API*/
/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao    2010-08-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     adc_channel_extern.h
 * \brief    定义LINEIN、RADIO、USB Speaker播放中间提供的接口及参数
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __ADC_CHANNEL_EXTERN_H__
#define __ADC_CHANNEL_EXTERN_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      加载系统头文件
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>

#endif

/*! \cond MMM_MP_API */

//PCM - AUX/FM 等需要ADC的应用解码所需空间
#define PCM_ADC_BUF_BLOCK   (512) //刚好单声道128个样本点，刚好够数字音效处理一帧
//NOTE：必须是POST_PROCESS_FRAME_SAMPLE的2倍，否则会出错！！
#define PCM_ADC_BUF_LEN     (PCM_ADC_BUF_BLOCK * 2)

typedef struct
{
    bool  need_update : 1;
    bool  need_resample : 1;
    bool  sample_rate_ok : 1;
    uint8 sample_rate_khz; //重采样目的采样率
    uint8 resample_src_khz; //重采样原始采样率
} get_sample_rate_ret_t;

/*!
 * \brief
 *      ADC通道提供给应用层的命令字
 */
typedef enum
{
    /*! 打开ADC通道 */
    ADC_CHANNEL_OPEN = 0,
    /*! 关闭ADC通道 */
    ADC_CHANNEL_CLOSE,

    /*! 设置ADC通道参数，包括AIN通道、采样率等 */
    ADC_CHANNEL_SETTING,

    /*! 启动ADC通道 */
    ADC_CHANNEL_START,
    /*! 停止ADC通道 */
    ADC_CHANNEL_STOP,

    /*! 获取ADC通道数据 */
    ADC_CHANNEL_GET_PIPE,
    ADC_CHANNEL_GET_MIC_PIPE,
    ADC_CHANNEL_GET_RESAMPLE_LCH_PIPE,
    ADC_CHANNEL_GET_RESAMPLE_RCH_PIPE,

    /*! 读取ADC能量 */
    ADC_CHANNEL_GET_ENERGY,

    /*! 获取采样率，单位Hz */
    ADC_CHANNEL_GET_SAMPLERATE,

    /*! 设置缓冲区 */
    ADC_CHANNEL_SET_BUFFER,
    /*! 设置重采样缓冲区 */
    ADC_CHANNEL_SET_RESAMPLE_BUFFER,
    /*! 开始重采样缓冲数据，收到该命令之前是过滤数据的 */
    ADC_CHANNEL_RESAMPLE_START_BUF_DATA,
} adc_channel_cmd_e;

/*!
 *  \brief
 *      ADC通道状态
 */
typedef enum
{
    ADC_CHANNEL_STA_CLOSE = 0,
    ADC_CHANNEL_STA_OPEN,
    ADC_CHANNEL_STA_START,
    ADC_CHANNEL_STA_STOP,
    ADC_CHANNEL_STA_ERROR,
} adc_channel_status_e;

/*!
 *  \brief
 *     音乐播放实时信息
 */
typedef struct
{
    /*! 当前能量值-最大值*/
    short adc_cur_energy;
    /*! 当前能量值-平均值*/
    short adc_cur_energy_rms;
    /*! ADC低能量持续时间，单位ms */
    uint32 adc_lowpw_time;
    /*! 当前能量值-最大值*/
    short mic_cur_energy;
    /*! 当前能量值-平均值*/
    short mic_cur_energy_rms;
} adc_energy_info_t;

/* 数据输入接口类型 */
#define ADC_SOURCE_AUX     0
#define ADC_SOURCE_MIC     1
#define ADC_SOURCE_I2SRX   2
#define ADC_SOURCE_SPDIFRX 3
#define ADC_SOURCE_HDMIARC 4 //只在PCM Player中间件有用，转换为SPDIFRX传递给ADC Channel模块
#define ADC_SOURCE_MIC_AUX 5

/* 用户类型 */
#define ADC_USER_LINEIN    0
#define ADC_USER_BTPLAY    1
#define ADC_USER_HDMI_ARC  2 //HDMI ARC延时需要达到100ms级别

/*!
 *  \brief
 *     音乐播放的设置信息
 */
typedef struct
{
    /*! 数据输入接口类型 */
    uint8 data_input_if;
    /*! 默认采样率，单位：KHz，默认值44 */
    uint8 default_samplerate;

    /*! AIN输入模式，0表示AUXL0和AUXR0立体声输入，1表示AUXL1和AUXR1立体声输入，2表示AUXL0和AUXL1混合输入，
        3表示AUXR0和AUXR1混合输入，4表示AUXL0和AUXR0混合输入，5表示IC外混合后AUXL0输入，
        6表示IC外混合后AUXR0输入，7表示IC外混合后AUXL1输入，8表示IC外混合后AUXR1输入 */
    uint8 ain_input_mode;
    /*! AIN OP Gain */
    uint8 ain_op_gain;
    /*! AIN ADC Gain */
    uint8 ain_adc_gain;
    /*! 用户类型，退化为仅在PCM Player中间件使用 */
    uint8 user_type;

    /*! 如果是MIC输入，0表示MIC0，1表示MIC1，2表示MIC0+MIC1 */
    uint8 micin_input_mode;
    /*! MIC IN OP Gain */
    uint8 micin_op_gain;
    /*! MIC IN ADC Gain */
    uint8 micin_adc_gain;

    uint8 i2srx_work_mode : 1; //角色，0-主角色，1-从角色
    uint8 i2srx_slave_mclk_use_inner : 1; //I2SRX作为Slave使用时，支持采样率在96KHz及以下，可以不用接MCLK，节省一个PIN
    uint8 i2srx_width : 2; //有效位宽，0-16bit，1-20bit，2-24bit
    uint8 i2srx_data_mode : 2; //数据传输模式，0-i2s模式，1-左对齐，2-右对齐

    /*! LINEIN低能量检测阈值（最大值），持续一段时间可以认为AUX播放源停止，可以省电关机等；0表示不使用这个条件 */
    short adc_lowpw_th_max;
    /*! LINEIN低能量检测阈值（有效值），持续一段时间可以认为AUX播放源停止，可以省电关机等；0表示不使用这个条件 */
    short adc_lowpw_th_rms;
} adc_chanel_setting_t;

typedef struct
{
    /*! adc channel 所有缓冲区均由中间件指定 */
    uint8 *adc_chan_buffer_1k;
    uint8 *adc_chan_cache;
    uint8 *mic_chan_cache;
    uint8 *mic_aux_temp_buffer_256;
    uint16 adc_chan_cache_len;
    uint16 mic_chan_cache_len;
    uint16 pcm_adc_block_size; //DMA Buffer一半，半满半空长度，最多512字节=1k/2
} adc_channel_buffer_setting_t;

//软件重采样要求数据反馈给adc_channel，这样数据流模型不变
typedef bool (*adc_chan_feedback_data_cbk)(short *buf, uint16 length);
typedef struct
{
    uint8 *resample_left_ch_buff;
    uint8 *resample_right_ch_buff;
    uint8 *resample_split_buff;
    uint16 resample_left_ch_len;
    uint16 resample_right_ch_len;
    adc_chan_feedback_data_cbk *ret_feedback_data_cbk;
} adc_channel_resample_buffer_setting_t;

#define ADC_CHANNEL_CMD_ENTRY       (*((uint32*)(AUDIO_CHANNEL_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define adc_channel_cmd(a,b,c)      ((int32(*)(void *handel, adc_channel_cmd_e cmd, uint32 args))ADC_CHANNEL_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
