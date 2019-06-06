/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_tws_cmd.h"

#ifndef __MUSIC_BT_PLAYER_TWS_H__
#define __MUSIC_BT_PLAYER_TWS_H__

//#define BT_TWS_DEBUG_INFO //TWS 频繁打印的Debug信息
//#define BT_TWS_DEBUG_CMD_ID //TWS 打印发送和接收命令ID

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

#define TWS_ADJUST_AUDIO_APS_TIMER           (50)  //TWS 主箱与副箱 50ms 进行一次对齐微调
#define TWS_SLAVE_UPDATE_PLAY_INFO_TIMER     (500) //TWS 副箱500ms更新一次状态给主箱

#define TWS_SAMPLES_DIFF_MAX                 (10)  //TWS主副箱播放采样点差值最大值，超过该值就置为错误状态，退出对箱播放

#define TWS_IP_FRAME_MAX_COUNT               (300) //帧校验和检查项，每300帧检测一次

#define TWS_START_PLAY_DELAY_CLOCK           (380) //主箱与副箱约定同步开始播放延时，要求精确触发 380*312.5 = 120ms
#define TWS_ADJUST_APS_DELAY_CLOCK           (380) //主箱与副箱约定同步调节Audio APS延时，要求精确触发 380*312.5 = 120ms

#define TWS_DIFF_MAX_RECENT_COUNT            (1000)//TWS主副箱最近30~60秒采样点差值最大值

typedef struct
{
    /*! TWS角色，主箱还是副箱；单箱播放则是Normal */
    dev_role_e dev_role;
    /*! TWS箱体位置，左箱还是右箱；单箱播放则是双声道 */
    dev_spk_pos_e dev_spk;
    /*! 获取蓝牙时间戳，返回FALSE表示TWS链路已断开 */
    f_get_bt_clk tws_get_bt_clk;
    /*! 设置同步触发的蓝牙时间戳，返回FALSE表示TWS链路已断开 */
    f_set_bt_clk tws_set_bt_clk;
    /*! 设置同步触发回调函数 */
    f_register_bt_clk_sync_cbk tws_reg_bt_clk_sync_cbk;
    /*! 是否发生AVDTP数据包丢失 */
    f_has_avdtp_pkg_lost tws_has_avdtp_pkg_lost;

    /* TWS 调节 Aduio APS 相关变量*/
    uint32 adjust_audio_aps_timer; //50ms周期检测定时器
    uint32 slave_update_play_info_timer; //副箱500ms上报播放信息定时器
    uint32 send_to_dac_anolog_count; //已经经过DAC anolog播放的采样点数目
    uint32 samples_diff_too_large_timer; //在推歌结束时容易出现diff变大甚至超过阈值的现象，需要持续1S都不能恢复才认为是错误状态
    uint16 samples_diff_max_recent_count1; //记次数，每次50ms，1000次一个循环
    uint8  samples_diff_max; //主副箱之间最大采样点偏差
    uint8  samples_diff_max_recent1; //最近30~60秒钟主副箱之间最大采样点偏差 1
    uint8  samples_diff_max_recent2; //最近30~60秒钟主副箱之间最大采样点偏差 2，与recent1配合使用，间隔固定为30秒
    //主箱调节Audio APS
    bool   need_master_set_audio_aps;
    uint8 master_set_audio_aps;
    bool   slave_is_setting_audio_aps_m; //副箱应该正在调节Audio APS，主箱暂时不需要判断调节Clock
    uint32 master_set_audio_aps_bt_clock;
    uint32 slave_must_set_audio_aps_ok_bt_clock_m; //副箱应该在这个时间点调节OK

    //副箱调节Audio APS
    tws_m2s_set_audio_aps_para_t slave_set_audio_aps_para; //副箱收到主箱调节Audio APS命令
    bool  need_slave_set_audio_aps; //副箱收到主箱调节Audio APS命令，正在调节过程中
    uint8 slave_set_audio_aps_state; //0表示初始状态或完成调快调慢，1表示正在调高或调低一段时间
    uint8 slave_set_audio_aps_restore; //副箱调高或调低一段时间后恢复的Audio APS
    uint32 slave_set_audio_aps_start_bt_clk; //副箱开始调高或调低时间点，然后计时一段时间恢复Audio APS

    /* 同步命令队列 */
    buffer_rw_t *p_tws_mmm_m2s_pipe;
    buffer_rw_t *p_tws_mmm_s2m_pipe;
    tws_sync_cmd_t *p_tws_mmm_m2s_play_sync_cmd;

    /* 同步播放相关变量 */
    os_event_t *start_dec_sem;
    volatile bool tws_need_start_sync_play;
    volatile bool tws_need_set_sync_stop;
    bt_clock_t start_con_clock; //主箱真正开始扔DAC的时间点
    bt_clock_t should_start_con_clock; //主箱指定对箱开始播放时间点

    tws_play_info_t tws_slave_play_info; //master 最新收到 slave 的 play_info
    tws_ip_frame_info_t tws_slave_ip_frame_info; //master 最新收到 slave 的 ip_frame_info

    tws_play_info_t tws_play_info; //SLAVE USED TO REPORT MASTER
    tws_ip_frame_info_t tws_ip_frame_info; //SLAVE USED TO REPORT MASTER

    /* TWS 防止开头丢帧，增加帧校验和 */
    uint16 ip_frame_id_count;
    uint16 ip_frame_cks;
    uint16 ip_frame_id_cks;
    bool   need_update_ip_frame_cks;

    bool   tws_link_connect_flag; //TWS链路是否连接着，通过 tws_get_bt_clk 返回值更新

    bool   tws_slave_frame_lost_flag; //TWS副箱丢包标志，会反馈给主箱，由主箱来控制退出流程
    uint8  tws_master_lost_pkg_count; //TWS主箱接收手机数据流丢包次数，如果丢包次数过多，应该报错
    uint32 tws_start_play_timer; //TWS开始播放时间戳
    uint32 tws_slave_frame_lost_timer; //TWS副箱开始丢包时间戳，用于控制1S后副箱自己退出解码
} music_bt_player_tws_info_t;

#endif

#endif//__MUSIC_BT_PLAYER_TWS_H__
