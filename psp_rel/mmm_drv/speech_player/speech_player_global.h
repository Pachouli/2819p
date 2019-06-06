/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "hfp_speech_lib_dev.h"
#include "hfp_plc_lib_dev.h"
#include "audio_decoder_lib_dev.h"
#include "audio_sp_pre_lib_dev.h"
#include "audio_sp_post_lib_dev.h"
#include "audio_post_process.h"
#include "mmm_sp.h"
#include "bt_interface.h"

#define SP_POST_BLOCK_LENGTH   30
#define SP_POST_BUFFER_LEN     (SP_POST_BLOCK_LENGTH*2)
extern uint8 SP_POST_BUFFER_ADDR[SP_POST_BUFFER_LEN];
#define SP_POST_IN_BUF_LEN     (SP_POST_BLOCK_LENGTH*2)
#define SP_POST_OUT_BUF_LEN    (SP_POST_BLOCK_LENGTH)

#define SP_RRE_DEAL_BLOCK_LEN  (60)
#define SP_PRE_BUFFER_LEN      (SP_RRE_DEAL_BLOCK_LEN)
extern uint8 SP_PRE_BUFFER_ADDR[SP_PRE_BUFFER_LEN];

#define PLC_DEAL_BLOCK_LEN     (120)

//#define SPEECH_MMM_DEBUG_INFO

#define SPEECH_PLAY_THRES_MS            (32)    //时长为32ms

#define PRE_PROCESS_BUF_LEN             (0xa00) //经过预处理后的数据缓冲区，单声道8KHz采样率，时长为176ms
extern uint8 PRE_PROCESS_BUF_ADDR[PRE_PROCESS_BUF_LEN];
#define PRE_PROCESS_BUF_HIGH_THRES_DEFAULT      (60) //缓冲60ms才开始扔有效数据，防止断音
#define PRE_PROCESS_BUF_MEAN_THRES_DEFAULT      (48) //缓冲区维持48ms数据
#define READY_MAX_TIMER                 (500) //在READY状态持续时间超过500ms则认为SCO断开，直接开始播放

#define MIC_ADC_BUF_LEN                 (0x100) //时长8ms，一半是4ms; ATS2819以L/R/L/R输出，R声道丢弃
extern uint8 MIC_ADC_BUF_ADDR[MIC_ADC_BUF_LEN];

#define MIC_INPUT_BUF_LEN               (0x380) //时长为56ms  该长度必须是 0x80 的整倍数
extern uint8 MIC_INPUT_BUF_ADDR[MIC_INPUT_BUF_LEN];
#define MIC_INPUT_BUF_LEN_THRES         (0x280) //超过40ms缓冲数据就认为是阻塞了，不做AEC/POST AGC处理以加快消耗缓冲区

#define PEQ_TEMP_BUFFER_LEN    (0x100)
extern uint8 PEQ_TEMP_BUFFER_ADDR[PEQ_TEMP_BUFFER_LEN];

#define PRE_AGC_DEAL_BUF_LEN            (0x100)
extern uint8 PRE_AGC_DEAL_BUF_ADDR[PRE_AGC_DEAL_BUF_LEN];

#define SPEECH_TEMP_BUF_LEN             (0x200)
extern uint8 SPEECH_TEMP_BUF_ADDR[SPEECH_TEMP_BUF_LEN];

#define KTD_INPUT_BUFFER_LEN            (0x100)
extern uint8 KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

//录音1.5KB缓冲区划分
#define ENCODE_SOURCE_BUF_LEN           (0x300)
extern uint8 ENCODE_SOURCE_BUF[ENCODE_SOURCE_BUF_LEN];
#define ENCODE_IN_BUF_LEN               (0x100)
extern uint8 ENCODE_IN_BUF[ENCODE_IN_BUF_LEN];
#define ENCODE_OUT_BUF_LEN              (0x200)
extern uint8 ENCODE_OUT_BUF[ENCODE_OUT_BUF_LEN];

extern uint8 OK_MIC_TEMP_BUF[0x100];

#if (SUPPORT_OK_MIC == 1)
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE/8) //2ms
#define MY_PCM_ADC_BLOCK_SIZE         (POST_PROCESS_FRAME_SAMPLE/2) //2ms
#else
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE/2) //8ms
#define MY_PCM_ADC_BLOCK_SIZE         (POST_PROCESS_FRAME_SAMPLE) //4ms
#endif
//OK MIC
#if (ENABLE_OK_MIC_FAST == 1)
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2) //延时应该尽量短，设置为2ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#else
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2 + 8*2) //延时应该尽量短，设置为2ms + 1ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#endif

//PLC相关宏定义
#define SCO_PKG_STATE_CORRECTLY_RECEIVED  0
#define SCO_PKG_STATE_POSSIBLE_INVALID    1
#define SCO_PKG_STATE_NO_DATA_RECEIVED    2
#define SCO_PKG_STATE_PARTIALLY_LOST      3

#define MY_ADJUST_SAMPLE_COUNT      (60)  //1S进行一次统计，即对60次采样进行统计
#define MY_ADJUST_SAMPLE_HIGH_COUNT (30)  //60次采样中有30次以上偏多，就认为CLOCK偏慢
#define MY_ADJUST_STEP_COUNT        (15)  //偏差较小级别持续30秒钟调到偏差较大级别

#define FADE_IN_TIME_DEFAULT        (100) //默认100ms淡入时间
#define FILTER_START_SOUND_COUNT    (2)   //过滤开头若干个Block，每个Block 16ms

/*!
 *  \brief
 *     音乐播放中间件的信息
 */
typedef struct
{
    mmm_sp_status_t speech_status; /* 当前播放信息 */
    speech_setting_t speech_setting; /* 播放设置信息 */

    int8 is_player_loop_enable; /* 标识线程是否可以跑循环体里面的代码 */

    int8 thread_terminate_flag;

    bool buffer_need_reset;
    uint8 ok_mic_start_flag; /*0表示过滤，1表示开始缓存，2表示开始读取处理*/
    bool ok_mic_mute_flag; /*0表示解除Mute，1表示Mute*/
    sp_work_mode_e work_mode;

    os_event_t *speech_dec_sem; /*BT解码信号量，用于PCM_BUF半空全空中断与解码线程同步控制*/
    volatile bool g_speech_pcm_buf_int_en_flag; /*是否已经启动PCM_BUF半空全空，如果是则依靠信号量分配时间片*/
    volatile uint8 g_sp_pcm_buf_pending; /*1表示半空，2表示全空（断音），3表示半空和全空*/

    buffer_rw_t *sco_input_buf_asqt;
    uint8 *soc_input_buf_asqt_tempbuf;
    buffer_rw_t *sco_input_buf; /*SCO Input Buffer，Input Buffer 由BT ENGINE写入，在SPEECH中间件读取；通过共享内存获取到该管理结构体指针*/
    buffer_rw_t *sco_output_buf; /*SCO Output Buffer，Output Buffer 在SPEECH中间件写入，由BT ENGINE读取；通过共享内存获取到该管理结构体指针*/
    buffer_rw_t pre_process_buf; /*PRE PROCESS Buffer*/
    buffer_rw_t post_process_buf; /*POST PROCESS Buffer, 只是用raw_data、length和read_ptr，与pre_process_buf共用一块Buffer，
     所以要注意read_ptr不要被pre_process_buf->write_ptr指针赶上导致被覆盖了*/
    buffer_rw_t mic_input_buf; /*MIC Input Buffer*/

    volatile bool start_play_flag; /*speech开始之后，ADC中断触发等到SCO Input Buffer已经缓冲了才开始扔有效数据播放*/
    volatile bool g_speech_need_start;
    volatile bool g_speech_pre_enough_flag;
    volatile bool g_speech_reset_buffer_flag;

    AGCHandle *agc_handle_pre; /*预处理AGC句柄*/
    AGCHandle *agc_handle_post; /*后处理AGC句柄*/
    void *aec_handle; /*AEC句柄*/
    void *plc_handle; /*PLC句柄*/

    int32 weak_result; /*声道衰减32768的分数*/

    uint32 pre_deal_time;
    uint32 post_deal_time;
    uint32 plc_deal_time;
    uint32 pre_agc_deal_time;
    uint32 post_agc_deal_time;
    uint32 aec_deal_time;
    uint32 playback_time;
    uint32 release_time;
    uint32 play_samples_add;
    uint32 pre_peq_time;
    uint32 post_peq_time;
    uint32 resample_time;
    uint32 enc_play_time;
    uint8 need_print_time;

    uint8 audio_aps;
    uint16 adjust_step_cnt;
    uint16 adjust_sample_counter;
    uint16 adjust_sample_high_counter;

    uint16 sco_pkg_err_count;
    uint16 sco_pkg_err2_count; //只有连续2个包有1个错误，就统计2个包
    uint16 sco_pkg_all_count;
    uint16 sco_pkg_err_continuous;
    uint16 sco_pkg_err_continuous_max;

    uint16 agc_temp_buf_len; /*AGC临时缓冲区中的数据长度*/

    uint16 g_half_empty_thres;
    uint16 g_half_full_thres;
    bool pcm_buf_hd_err;

    bool print_sco_out_full_flag;

    bool error_need_reset_flag;

    bool playing_setting_speech_ok : 1;
    bool playing_setting_plc_ok : 1;
    bool playing_setting_aec_ok : 1;
    bool playing_setting_preagc_ok : 1;
    bool playing_setting_postagc_ok : 1;

    uint32 start_play_delay_timer;

    asqt_dump_buf_t *p_asqt_dump_buf;

    short * sco_data_predeal_buf;
    uint8 * agc_predeal_buf;

    audiout_pcm_t sp_aout; /* 当前帧pcm信息 */
    void *audiopp_handle; /*DAE AL句柄*/
    uint8 fade_flag; /*淡入淡出选项，0表示没有淡入淡出请求，1表示淡入，2表示淡出*/
    uint8 dae_update_flag; /*数字音效参数更新标志，0表示不需要更新，1表示需要更新*/
    uint16 dae_freq;
    uint16 decode_freq;
    uint16 encode_freq;

#if (SUPPORT_SOFT_KEYTONE == 1)
    volatile bool kt_play_enable;
    sd_handle ktd_handle;
    uint32 ktd_length;
    uint32 ktd_remain;
#endif

    bool set_hfp_connected_flag;
    volatile bool g_speech_fadein_flag;
    uint8 g_filter_start_sound_count;
    uint32 ready_start_timer;
    uint16 pre_less_count;

    /*! PRE HIGH THRES*/
    uint16 pre_high_thres;
    /*! PRE MEAN THRES */
    uint16 pre_mean_thres;

    plc_para_t *g_p_speech_plc_para;
    agc_para_t *g_p_speech_pre_agc_para;
    agc_para_t *g_p_speech_post_agc_para;
    actionsaec_prms_t *g_p_speech_aec_prms;

    void *sp_pre_handle;
    void *sp_post_handle;

    uint8  *sp_post_buffer;
    uint16 sp_post_buffer_remain;
    uint8  *sp_pre_buffer;
    uint16 sp_pre_buffer_remain;
    bool sco_pkg_flag;

    uint8  sample_rate;
    uint8  pcmbuf_fill_block; //2ms
    uint16 aec_deal_block; //时长为16ms, post agc deal block, post peq deal block * N
    uint16 codec_deal_block; //pre process block @ sco pre deal, 1/2@cvsd or 1/4@msbc of plc_deal_block
    uint16 plc_deal_block; //pre process block
    uint16 pre_agc_deal_block; //pre peq deal block

    audio_aps_info_t audio_aps_info;
    post_process_detail_t post_process_detail;

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    /*重采样相关*/
    audio_resample_info_t resample_info;
#endif

    void *rec_mod_handle; /*录音模块句柄*/
    record_module_fs_para_t record_fs;
    record_module_encode_para_t encode_para;
    record_module_status_t record_status;
    buffer_rw_t encode_input_buf_w;
    uint16 encode_frame_len;
    bool record_enable;
    mmm_record_status_e mmm_record_sta;
} speech_player_t;

extern speech_player_t* speech_player_global;

extern PP_para_info_t g_PP_para_info;

extern void *player_loop(speech_player_t *speech_player);

extern void *mmm_sp_open(void *param);
extern int32 mmm_sp_close(speech_player_t *speech_player, void *param);

extern int32 mmm_sp_aout_open(speech_player_t *speech_player);
extern int32 mmm_sp_aout_close(speech_player_t *speech_player);
extern void send_pcm_to_pcm_buf_by_dac_fifo(short *pcm_buffer, uint32 sample_cnt);
extern void init_pcm_buf(uint32 half_empty_thres, uint32 half_full_thres);
extern void speech_adc_dma_int_cbk(uint8 pending);

extern void speech_pcm_buf_int_cbk(uint8 pending);

extern int32 start_speech_decoder(speech_player_t *speech_player);
extern int32 stop_speech_decoder(speech_player_t *speech_player);
extern int32 start_speech_decoder_agc_init(speech_player_t *speech_player, agc_para_t *p_agc_para);
extern int32 start_speech_decoder_aec_init(speech_player_t *speech_player, actionsaec_prms_t *p_aec_prms);

extern int32 audio_mmm_sp_cmd(void *handel, mmm_sp_cmd_t cmd, uint32 args);

extern void speech_post_process_frame(speech_player_t *speech_player);
extern void speech_pre_process_frame(speech_player_t *speech_player, bool fill_empty_block);
extern bool dump_data_to_asqt_dump_buffer(asqt_dump_buf_t *p_asqt_dump_buf, uint8 bitmap, uint8 *dump_buf,
        uint16 dump_length);
extern bool write_raw_data_to_sco_in(buffer_rw_t *p_buf_rw, uint8 *w_buf, uint16 w_len);

extern void update_PP_para_info(speech_player_t *speech_player);
extern void update_PP_enable(speech_player_t *speech_player, bool music_pp_enable, bool mic_pp_enable);
extern void set_speech_decoder_clk(speech_player_t *speech_player);

extern int32 keytone_start(speech_player_t *speech_player, void *param);
extern int32 keytone_stop(speech_player_t *speech_player);
extern int32 keytone_frame_deal(speech_player_t *speech_player, short *pcm_out, uint16 samples_count);
extern int32 keytone_playend_deal(speech_player_t *speech_player);

extern void speech_playback(speech_player_t *speech_player);

#if (SUPPORT_AUDIO_RESAMPLE == 1)

//重采样转换输入缓冲区的大小，取转换表 input samples 的最大值，但是最小是64，作为single to dual的buffer
#define RESAMPLE_SINGLE_TO_DUAL_SAMPLES 32
#define RESAMPLE_INPUT_BUFF_SIZE 64
extern short RESAMPLE_INPUT_BUFFER[1][RESAMPLE_INPUT_BUFF_SIZE];

//重采样转换输出缓冲区的大小，取转换表 output samples 的最大值
#define RESAMPLE_OUTPUT_BUFF_SIZE 192
extern short RESAMPLE_OUTPUT_BUFFER[1][RESAMPLE_OUTPUT_BUFF_SIZE];

//创建左右声道重采样句柄所需的缓冲
extern short RESAMPLE_TEMP_BUFF[1][RESAMPLE_TEMP_SAMPLES];
extern int RESAMPLE_TABLE_BUFF[RESAMPLE_TABLE_ELEMENTS];

extern void set_resample_info(audio_resample_info_t *resample_info, uint32 input_rate_khz);
extern void resample_and_send_pcm_to_pcm_buf(speech_player_t *speech_player, short * data, int32 sample_cnt);

//录音功能接口
extern int32 start_speech_record(speech_player_t *speech_player);
extern int32 stop_speech_record(speech_player_t *speech_player);
extern int32 player_encode_frame_deal(speech_player_t *speech_player);
extern int32 player_encode_source_deal(speech_player_t *speech_player, short *spk_src, short *mic_src, int32 samples_count);

#endif
