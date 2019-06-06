#ifndef AGC_INTERFACE_H
#define AGC_INTERFACE_H

typedef struct
{

    /*目标幅度：4000-24000，推荐值8000*/
    int agc_level;
    /*采样率：目前仅支持8000或16000*/
    int sample_rate;
    /*通道数：目前仅支持1*/
    int channels;

    /*扩展参数，暂未实现*/
    int agc_mode;
    /*放大是最大的增益，零：disable，非零：增益上限值*/
    int minimum_gain;
    /*缩小时的最小增益，零：disable，非零：增益下限值*/
    int maximum_gain;

    int zero_crose;

    /*平稳时间的，单位ms*/
    int hold_time;
    /*衰减时间，单位ms*/
    int decay_time;
    /*提升时间，单位ms*/
    int attack_time;

    /*是否启动噪声门槛，1：启动，0：关闭*/
    int noise_gate_enable;
    /*噪声门槛值，范围200-1000，推荐值400*/
    int noise_gate_threshold;
    /*噪声门槛类别，0：固定，1：自适应*/
    int noise_gate_type;
} AGCSetting;

typedef struct
{
    /*每次处理的样本点数，假定样本点为short类型*/
    int process_block_size;

    int agc_level;
    int sample_rate;
    int channels;

    void *st;
} AGCHandle;

#endif
