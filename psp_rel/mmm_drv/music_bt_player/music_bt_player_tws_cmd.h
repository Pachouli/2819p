/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "mmm_bp.h"

#ifndef __MUSIC_BT_PLAYER_TWS_CMD_H__
#define __MUSIC_BT_PLAYER_TWS_CMD_H__

#define TWS_SYNC_S2M_PLAY_INFO_EV       0x01
typedef struct
{
    bt_clock_t con_clock; //当前Controller时间戳
    uint32 dac_samples; //当前DAC出去的采样点数
} tws_play_info_t;

#define TWS_SYNC_S2M_IP_FRAME_INFO_EV   0x02
typedef struct
{
    uint16 ip_buf_rtime : 10; //input buffer 剩余时间
    uint16 ip_frame_lost : 1; //丢包标志，副箱检测到丢包后，先反馈给主箱，由主箱来主导错误处理流程，1S钟后如果主箱还没有处理，就主动报错退出
    uint16 ip_frame_id;  //input buffer 帧序号 有效值0,300,600,,,2700，-1表示从input_buffer头开始计算ip_frame_len字节，用于刚开始未播放时同步用
    uint16 ip_frame_len; //字节长度
    uint16 ip_frame_cks; //input buffer 帧校验和
} tws_ip_frame_info_t;



#define TWS_SYNC_M2S_START_PLAY_CMD     0x81
typedef struct
{
    bt_clock_t should_start_con_clock; //期望开始DAC播放的绝对时间
} tws_m2s_start_play_para_t;

#define TWS_SYNC_M2S_SET_AUDIO_APS_CMD  0x82
typedef struct
{
    uint32 bt_clock;     //如果是副箱调快调慢，则可以为0；如果是主箱与副箱同步调节，则需要在当前时间延时120ms后同步调节
    uint16 clock_cnt;    //如果 restore_flag 为TRUE，该值表示较主箱调快或调慢的时间段
    uint8  aps;          //设置副箱AUDIO APS
    uint8  restore_flag; //用来区分是仅副箱调快调慢，还是主副箱同步调节
} tws_m2s_set_audio_aps_para_t;

#endif//__MUSIC_BT_PLAYER_TWS_CMD_H__
