/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "audio_decoder_lib_dev.h"
#include "audio_encoder_lib_dev.h"
#include "audio_post_process.h"
#include "mmm_bp.h"
#include "mmm_mr.h"
#include "music_bt_player_tws.h"

#ifndef __MUSIC_BT_PLAYER_GLOBAL_H__
#define __MUSIC_BT_PLAYER_GLOBAL_H__

//#define BT_MMM_DEBUG_INFO //debug信号开关，打开会打印较多细节信息

#define SBC_SYNCWORD                    (0x9C)
#define SBC_MAX_BITPOOL                 (0x35)
#define SBC_MAX_FRAME_LEN               (119)

#define PCM_INPUT_BUF_BLOCK_COUNT    (2)
#define PCM_INPUT_BUF_LEN   (PCM_ADC_BUF_BLOCK * PCM_INPUT_BUF_BLOCK_COUNT)
extern uint8 PCM_INPUT_BUF_ADDR[PCM_INPUT_BUF_LEN];

#define AUX_TWS_PCM_INPUT_BUF_BLOCK_COUNT    (8)
#define AUX_TWS_PCM_INPUT_BUF_LEN   (PCM_ADC_BUF_BLOCK * AUX_TWS_PCM_INPUT_BUF_BLOCK_COUNT)

//录音1.5KB缓冲区划分
#define ENCODE_SOURCE_BUF_LEN           (0x300)
extern uint8 ENCODE_SOURCE_BUF[ENCODE_SOURCE_BUF_LEN];
#define ENCODE_IN_BUF_LEN               (0x100)
extern uint8 ENCODE_IN_BUF[ENCODE_IN_BUF_LEN];
#define ENCODE_OUT_BUF_LEN              (0x200)
extern uint8 ENCODE_OUT_BUF[ENCODE_OUT_BUF_LEN];

#define SBC_DEC_TEMP_BUFFER_LEN         (0x200)
extern uint8 SBC_DEC_TEMP_BUFFER_ADDR[SBC_DEC_TEMP_BUFFER_LEN];

#define SBC_DEC_TEMP_INPUT_LEN          (0x100)
#define SBC_DEC_OUTPUT_BLOCK_SIZE       (0x200)
extern uint8 SBC_DEC_OUTPUT_BUFFER_ADDR[SBC_DEC_OUTPUT_BLOCK_SIZE];

#define SBC_ENC_OUTPUT_BLOCK_SIZE       (0x80)
extern uint8 SBC_ENC_OUTPUT_BUFFER_ADDR[SBC_ENC_OUTPUT_BLOCK_SIZE];

#define FADE_IN_TIME_DEFAULT        (40) //默认40ms淡入时间
#define FADE_OUT_TIME_DEFAULT       (40) //默认40ms淡出时间

#define MY_ADJUST_SAMPLE_HIGH_COUNT_SLOW     (60)  //大于60%采样偏多，就认为CLOCK偏慢
#define MY_ADJUST_SAMPLE_HIGH_COUNT_FAST     (40)  //小于40%采样偏多，就认为CLOCK偏快
#define MY_ADJUST_SAMPLE_TIMER               (1000)//1S钟周期调整可能调整1次CLOCK

#define MY_BTPLAY_DELAY_TIME_MAX    (160) //缓冲或延时超过160ms开始播放
#define MY_FADE_OUT_THRESHOLD       (12)  //当缓冲数据少于12ms时淡出
#ifdef BT_MMM_DEBUG_INFO
#define MY_FRAME_SPACE_THRESHOLD    (60)  //帧间隔监控阈值
#else
#define MY_FRAME_SPACE_THRESHOLD    (100) //帧间隔监控阈值
#endif
#define MY_START_PLAY_TIME          (46)  //必须大于46ms才能开始解码播放

#define PCM_BUF_HE_THRESHOLD        (0x400) //10.6ms@48k sr

#if (SUPPORT_OK_MIC == 1)
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE/2) //块处理大小减少一半，有助于缩短延时
#define MY_PCM_DEC_BUF_BLOCK          (SBC_DEC_OUTPUT_BLOCK_SIZE/2) //块处理大小减少一半，有助于缩短延时
#else
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE)
#define MY_PCM_DEC_BUF_BLOCK          (SBC_DEC_OUTPUT_BLOCK_SIZE)
#endif

//OK MIC
#if (ENABLE_OK_MIC_FAST == 1)
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE * 2 + 44*3) //延时应该尽量短，设置为1.4ms + 1.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE * 2)
#else
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE * 2 + 44*5) //延时应该尽量短，设置为1.4ms + 2.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE * 2)
#endif

#define KTD_INPUT_BUFFER_LEN        (0x80)
extern uint8 KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

#define TAIL_AUTO_DETECT_ENABLE     1      //使能手机切歌自动检测   注：也可以由bluetooth engine告诉中间件手机切歌了，但是可能有兼容性问题
#define TAIL_SILENT_TIME            (1000) //超过1S静音即认为是到末尾了
#define TAIL_FRAME_SPACE_TIME       (140)  //超过140ms没有收到数据则认为是手机切歌且暂停发数

/*!
 *  \brief
 *     音乐播放中间件的信息
 */
typedef struct
{
    mmm_bp_status_t music_status; /* 当前播放信息 */
    music_bt_setting_t music_setting; /* 播放设置信息 */
    audiout_pcm_t bp_aout; /* 当前帧pcm信息，用于DAE */

    /*ADC CHANNEL & SBC ENCODER*/
    void *adc_channel_handle;
    void *sbc_encoder_handle;
    buffer_rw_t *p_input_buf_r; /*PCM Input Buffer Pointer for All include AUX/FM/USB SOUND*/
    buffer_rw_t *p_mic_input_buf_r; /* OK MIC Input Buffer */
    f_send_frame p_f_send_frame; /*bt engine提供的发送帧数据回调函数*/

    void *audiodec_handle; /* 音频解码线程句柄 */

    bp_work_mode_e work_mode; /* 工作模式 */

    bool is_player_loop_enable; /* 标识线程是否可以跑循环体里面的代码 */
    bool thread_terminate_flag;

    os_event_t *bt_dec_sem; /*BT解码信号量，用于PCM_BUF半空全空中断与解码线程同步控制*/
    volatile bool g_bp_pcm_buf_int_en_flag; /*是否已经启动PCM_BUF半空全空，如果是则依靠信号量分配时间片*/
    volatile uint8 g_bp_pcm_buf_pending; /*1表示半空，2表示全空（断音），3表示半空和全空*/

    uint8 fade_flag; /*淡入淡出选项，0表示没有淡入淡出请求，1表示淡入，2表示淡出*/
    uint8 dae_update_flag; /*数字音效参数更新标志，0表示不需要更新，1表示需要更新*/

    bool buffer_need_reset;
    uint8 ok_mic_start_flag; /*0表示过滤，1表示开始缓存，2表示开始读取处理*/
    bool ok_mic_mute_flag; /*0表示解除Mute，1表示Mute*/
    bool ready_because_fadeout_flag; /*因为数据不足淡出等待数据足够后再开始缓冲解码，处于ready状态，需要继续OK MIC，但不要开始解码*/

    void *rec_mod_handle; /*录音模块句柄*/
    record_module_fs_para_t record_fs;
    record_module_encode_para_t encode_para;
    record_module_status_t record_status;
    buffer_rw_t encode_input_buf_w;
    uint16 encode_frame_len;
    bool record_enable;
    mmm_record_status_e mmm_record_sta;

    void *audiopp_handle; /*DAE AL句柄*/
    buffer_rw_t *input_buf; /*BT Input Buffer，Input Buffer 由BT ENGINE写入，在BT PLAY中间件读取；通过共享内存获取到该管理结构体指针*/
    buffer_rw_t aout_buf; /*decode aout buffer*/
    buffer_rw_t dac_fifo_buf; /*dac fifo to pcm buf*/

    uint16 g_sbc_sample_rate;
    uint16 g_sbc_frame_size;
    uint8  g_sbc_channel_mode;

    uint8 g_sbc_parse_flag;
    uint8 g_aout_init_flag;
    uint8 g_dae_pm_init_flag; /*获知SBC采样率之后即可设置DAE参数和时钟频率*/
    uint8 g_start_sub_pre_flag; /*开始播放预处理标志*/

    uint8 decode_pp_frame_total; /*一帧解码对应后处理帧数*/
    uint8 decode_pp_frame_index; /*后处理帧序号*/

    uint16 write_ptr_backup; //用于了解数据流变化，判断是否收到新数据包

    uint16 buffer_time_length;
    uint16 buffer_time_min;
    uint16 frame_count;

    uint32 last_frame_timestamp;
    uint32 cur_timestamp;

    uint32 adjust_sample_timer;
    uint16 adjust_sample_counter;
    uint16 adjust_sample_high_counter;
    uint8 audio_aps;

    volatile bool is_sending_to_pcm_buf_flag;
    uint32 send_to_pcm_buf_count;

    uint32 dec_play_time;
    uint32 encoder_time;
    uint32 dae_play_time;
    uint32 release_time;
    uint32 play_samples_add;
    bool need_print_time;
    uint16 dae_freq;
    uint16 decode_freq;
    uint16 encode_freq;

    uint16 g_half_empty_thres;
    uint16 g_half_full_thres;
    bool pcm_buf_hd_err;

    bool playing_setting_ok;

    uint32 bt_zero_frame;
    uint32 bt_lowpw_frame;

#if (SUPPORT_SOFT_KEYTONE == 1)
    volatile bool kt_play_enable;
    sd_handle ktd_handle;
    uint32 ktd_length;
    uint32 ktd_remain;
    resample_handle_t rs_handle;
    uint16 rs_src_sr;
    uint16 rs_dest_sr;
    uint16 rs_remain_len;
#endif

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    music_bt_player_tws_info_t tws_info;
#endif

    audio_aps_info_t audio_aps_info;

    post_process_detail_t post_process_detail;
} music_bt_player_t;

extern music_bt_player_t* music_player_global;

extern PP_para_info_t g_PP_para_info;
extern freq_point_config_t g_freq_point_cfg;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
extern music_bt_player_tws_info_t *p_tws_info;
#endif

extern void update_PP_para_info(music_bt_player_t *music_player);
extern void set_music_decoder_clk(music_bt_player_t *music_player);

extern void *player_loop(music_bt_player_t *music_player);

extern void *mmm_bp_open(void *param);
extern int32 mmm_bp_close(music_bt_player_t *music_player, void *param);

extern int32 mmm_bp_aout_open(music_bt_player_t *music_player);
extern int32 mmm_bp_aout_close(music_bt_player_t *music_player);
extern void send_pcm_to_pcm_buf_by_dac_fifo(short *pcm_buffer, uint32 sample_cnt, bool start);
extern void send_pcm_to_pcm_buf_by_dac_fifo_start(void) __FAR__;
extern void init_pcm_buf(uint32 half_empty_thres, uint32 half_full_thres);

extern void bt_pcm_buf_int_cbk(uint8 pending);

extern int32 start_music_decoder(music_bt_player_t *music_player);
extern int32 start_music_decoder_sub(music_bt_player_t *music_player);
extern int32 start_music_decoder_sub_pre(music_bt_player_t *music_player);
extern int32 stop_music_decoder(music_bt_player_t *music_player);
extern int32 stop_music_decoder_sub(music_bt_player_t *music_player, bool stop);
extern int32 stop_music_decoder_fadeout_start(music_bt_player_t *music_player, bool pause);
extern int32 stop_music_decoder_fadeout_wait_cp(music_bt_player_t *music_player);

extern int32 audio_mmm_bp_cmd(void *handel, mmm_bp_cmd_t cmd, uint32 args);

extern uint16 read_bt_input_buf(buffer_rw_t *input_buf, uint8 *read_buf, uint16 data_len);
extern void update_bt_input_buf(buffer_rw_t *input_buf, uint16 read_len);
extern int sbc_check_sample_rate(music_bt_player_t *music_player);
extern int sbc_calc_frame_count(music_bt_player_t *music_player);

extern int32 sbc_encoder_frame_deal(music_bt_player_t *music_player);

extern uint16 calc_buffer_time_length(music_bt_player_t *music_player);
extern void monitor_aps_adjust(music_bt_player_t *music_player);
extern void monitor_fadeout_check(music_bt_player_t *music_player);

extern int32 keytone_start(music_bt_player_t *music_player, void *param);
extern int32 keytone_stop(music_bt_player_t *music_player);
extern int32 keytone_frame_deal(music_bt_player_t *music_player, short *pcm_out, uint16 samples_count);
extern int32 keytone_playend_deal(music_bt_player_t *music_player);

extern int32 start_music_record(music_bt_player_t *music_player);
extern int32 stop_music_record(music_bt_player_t *music_player);
extern int32 player_encode_frame_deal(music_bt_player_t *music_player);
extern int32 player_encode_source_deal(music_bt_player_t *music_player, short *src, int32 samples_count);

#endif//__MUSIC_BT_PLAYER_GLOBAL_H__
