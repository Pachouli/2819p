/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "mmm_bp.h"

#ifndef __MUSIC_BT_PLAYER_TWS_CMD_H__
#define __MUSIC_BT_PLAYER_TWS_CMD_H__

#define TWS_SYNC_S2M_PLAY_INFO_EV       0x01
typedef struct
{
    bt_clock_t con_clock; //��ǰControllerʱ���
    uint32 dac_samples; //��ǰDAC��ȥ�Ĳ�������
} tws_play_info_t;

#define TWS_SYNC_S2M_IP_FRAME_INFO_EV   0x02
typedef struct
{
    uint16 ip_buf_rtime : 10; //input buffer ʣ��ʱ��
    uint16 ip_frame_lost : 1; //������־�������⵽�������ȷ��������䣬���������������������̣�1S�Ӻ�������仹û�д��������������˳�
    uint16 ip_frame_id;  //input buffer ֡��� ��Чֵ0,300,600,,,2700��-1��ʾ��input_bufferͷ��ʼ����ip_frame_len�ֽڣ����ڸտ�ʼδ����ʱͬ����
    uint16 ip_frame_len; //�ֽڳ���
    uint16 ip_frame_cks; //input buffer ֡У���
} tws_ip_frame_info_t;



#define TWS_SYNC_M2S_START_PLAY_CMD     0x81
typedef struct
{
    bt_clock_t should_start_con_clock; //������ʼDAC���ŵľ���ʱ��
} tws_m2s_start_play_para_t;

#define TWS_SYNC_M2S_SET_AUDIO_APS_CMD  0x82
typedef struct
{
    uint32 bt_clock;     //����Ǹ����������������Ϊ0������������븱��ͬ�����ڣ�����Ҫ�ڵ�ǰʱ����ʱ120ms��ͬ������
    uint16 clock_cnt;    //��� restore_flag ΪTRUE����ֵ��ʾ���������������ʱ���
    uint8  aps;          //���ø���AUDIO APS
    uint8  restore_flag; //���������ǽ�����������������������ͬ������
} tws_m2s_set_audio_aps_para_t;

#endif//__MUSIC_BT_PLAYER_TWS_CMD_H__
