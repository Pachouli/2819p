/*******************************************************************************/
/*!
 * \file     audio_resample.h
 * \brief    音频重采样
 * \author   zhanguoliang
 * \version  1.0
 * \date     2018-9-23
 *******************************************************************************/

#ifndef __AUDIO_RESMPLE_H__
#define __AUDIO_RESMPLE_H__

#define SUPPORT_AUDIO_RESAMPLE   0 //是否支持重采样功能，可以固定转换为44.1KHz/48KHz输出

#define RESAMPLE_TEMP_SAMPLES    835 //重采样临时缓冲采样点数（最大，某些采样率转换会少一些）
#define RESAMPLE_TABLE_ELEMENTS  312 //重采样系数表元素数目  （最大，某些采样率转换会少一些）

//重采样信息
typedef struct
{
    void *resample_handle_left;     //左声道重采样句柄
    void *resample_handle_right;    //右声道重采样句柄
    uint32 convert_input_rate_hz;   //重采样输入采样率
    uint32 convert_output_rate_hz;  //重采样输出采样率，根据该值设置dac_fifo的频率
    uint16 expected_input_sample_cnt;  //重采样转换函数的所要求的输入缓冲的采样点对的数量，该值根据不同的采样率之间的转换而不同，具体要查看重采样转换表
    uint16 expected_output_sample_cnt; //重采样转换的输出采样点大小
    uint16 cur_resample_input_len;     //当前重采样缓冲区中的输入采样点对的数量
    bool enable_resample;              //根据输入文件采样率和重采样映射表确定是否使能重采样转换，FALSE表示不使能，TRUE表示使能
} audio_resample_info_t;

//重采样操作映射表，用于定义重采样操作的输入采样率和转换要求的映射关系
typedef struct
{
    uint32 input_rate_hz;       //重采样输入采样率hz
    uint32 output_rate_hz;      //重采样输出采样率hz
    uint16 input_samples;       //重采样转换操作要求的输入采样点数量
    uint16 output_samples;      //重采样转换后的输出采样点数量
} resample_op_map_t;

#endif

