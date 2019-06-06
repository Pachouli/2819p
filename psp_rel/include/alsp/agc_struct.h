#ifndef AGC_INTERFACE_H
#define AGC_INTERFACE_H

typedef struct
{

    /*Ŀ����ȣ�4000-24000���Ƽ�ֵ8000*/
    int agc_level;
    /*�����ʣ�Ŀǰ��֧��8000��16000*/
    int sample_rate;
    /*ͨ������Ŀǰ��֧��1*/
    int channels;

    /*��չ��������δʵ��*/
    int agc_mode;
    /*�Ŵ����������棬�㣺disable�����㣺��������ֵ*/
    int minimum_gain;
    /*��Сʱ����С���棬�㣺disable�����㣺��������ֵ*/
    int maximum_gain;

    int zero_crose;

    /*ƽ��ʱ��ģ���λms*/
    int hold_time;
    /*˥��ʱ�䣬��λms*/
    int decay_time;
    /*����ʱ�䣬��λms*/
    int attack_time;

    /*�Ƿ����������ż���1��������0���ر�*/
    int noise_gate_enable;
    /*�����ż�ֵ����Χ200-1000���Ƽ�ֵ400*/
    int noise_gate_threshold;
    /*�����ż����0���̶���1������Ӧ*/
    int noise_gate_type;
} AGCSetting;

typedef struct
{
    /*ÿ�δ���������������ٶ�������Ϊshort����*/
    int process_block_size;

    int agc_level;
    int sample_rate;
    int channels;

    void *st;
} AGCHandle;

#endif
