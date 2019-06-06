/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "audio_decoder_lib_dev.h"
#include "audio_encoder_lib_dev.h"
#include "audio_post_process.h"
#include "mmm_bp.h"
#include "mmm_mr.h"
#include "music_bt_player_tws.h"

#ifndef __MUSIC_BT_PLAYER_GLOBAL_H__
#define __MUSIC_BT_PLAYER_GLOBAL_H__

//#define BT_MMM_DEBUG_INFO //debug�źſ��أ��򿪻��ӡ�϶�ϸ����Ϣ

#define SBC_SYNCWORD                    (0x9C)
#define SBC_MAX_BITPOOL                 (0x35)
#define SBC_MAX_FRAME_LEN               (119)

#define PCM_INPUT_BUF_BLOCK_COUNT    (2)
#define PCM_INPUT_BUF_LEN   (PCM_ADC_BUF_BLOCK * PCM_INPUT_BUF_BLOCK_COUNT)
extern uint8 PCM_INPUT_BUF_ADDR[PCM_INPUT_BUF_LEN];

#define AUX_TWS_PCM_INPUT_BUF_BLOCK_COUNT    (8)
#define AUX_TWS_PCM_INPUT_BUF_LEN   (PCM_ADC_BUF_BLOCK * AUX_TWS_PCM_INPUT_BUF_BLOCK_COUNT)

//¼��1.5KB����������
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

#define FADE_IN_TIME_DEFAULT        (40) //Ĭ��40ms����ʱ��
#define FADE_OUT_TIME_DEFAULT       (40) //Ĭ��40ms����ʱ��

#define MY_ADJUST_SAMPLE_HIGH_COUNT_SLOW     (60)  //����60%����ƫ�࣬����ΪCLOCKƫ��
#define MY_ADJUST_SAMPLE_HIGH_COUNT_FAST     (40)  //С��40%����ƫ�࣬����ΪCLOCKƫ��
#define MY_ADJUST_SAMPLE_TIMER               (1000)//1S�����ڵ������ܵ���1��CLOCK

#define MY_BTPLAY_DELAY_TIME_MAX    (160) //�������ʱ����160ms��ʼ����
#define MY_FADE_OUT_THRESHOLD       (12)  //��������������12msʱ����
#ifdef BT_MMM_DEBUG_INFO
#define MY_FRAME_SPACE_THRESHOLD    (60)  //֡��������ֵ
#else
#define MY_FRAME_SPACE_THRESHOLD    (100) //֡��������ֵ
#endif
#define MY_START_PLAY_TIME          (46)  //�������46ms���ܿ�ʼ���벥��

#define PCM_BUF_HE_THRESHOLD        (0x400) //10.6ms@48k sr

#if (SUPPORT_OK_MIC == 1)
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE/2) //�鴦���С����һ�룬������������ʱ
#define MY_PCM_DEC_BUF_BLOCK          (SBC_DEC_OUTPUT_BLOCK_SIZE/2) //�鴦���С����һ�룬������������ʱ
#else
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE)
#define MY_PCM_DEC_BUF_BLOCK          (SBC_DEC_OUTPUT_BLOCK_SIZE)
#endif

//OK MIC
#if (ENABLE_OK_MIC_FAST == 1)
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE * 2 + 44*3) //��ʱӦ�þ����̣�����Ϊ1.4ms + 1.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE * 2)
#else
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE * 2 + 44*5) //��ʱӦ�þ����̣�����Ϊ1.4ms + 2.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE * 2)
#endif

#define KTD_INPUT_BUFFER_LEN        (0x80)
extern uint8 KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

#define TAIL_AUTO_DETECT_ENABLE     1      //ʹ���ֻ��и��Զ����   ע��Ҳ������bluetooth engine�����м���ֻ��и��ˣ����ǿ����м���������
#define TAIL_SILENT_TIME            (1000) //����1S��������Ϊ�ǵ�ĩβ��
#define TAIL_FRAME_SPACE_TIME       (140)  //����140msû���յ���������Ϊ���ֻ��и�����ͣ����

/*!
 *  \brief
 *     ���ֲ����м������Ϣ
 */
typedef struct
{
    mmm_bp_status_t music_status; /* ��ǰ������Ϣ */
    music_bt_setting_t music_setting; /* ����������Ϣ */
    audiout_pcm_t bp_aout; /* ��ǰ֡pcm��Ϣ������DAE */

    /*ADC CHANNEL & SBC ENCODER*/
    void *adc_channel_handle;
    void *sbc_encoder_handle;
    buffer_rw_t *p_input_buf_r; /*PCM Input Buffer Pointer for All include AUX/FM/USB SOUND*/
    buffer_rw_t *p_mic_input_buf_r; /* OK MIC Input Buffer */
    f_send_frame p_f_send_frame; /*bt engine�ṩ�ķ���֡���ݻص�����*/

    void *audiodec_handle; /* ��Ƶ�����߳̾�� */

    bp_work_mode_e work_mode; /* ����ģʽ */

    bool is_player_loop_enable; /* ��ʶ�߳��Ƿ������ѭ��������Ĵ��� */
    bool thread_terminate_flag;

    os_event_t *bt_dec_sem; /*BT�����ź���������PCM_BUF���ȫ���ж�������߳�ͬ������*/
    volatile bool g_bp_pcm_buf_int_en_flag; /*�Ƿ��Ѿ�����PCM_BUF���ȫ�գ�������������ź�������ʱ��Ƭ*/
    volatile uint8 g_bp_pcm_buf_pending; /*1��ʾ��գ�2��ʾȫ�գ���������3��ʾ��պ�ȫ��*/

    uint8 fade_flag; /*���뵭��ѡ�0��ʾû�е��뵭������1��ʾ���룬2��ʾ����*/
    uint8 dae_update_flag; /*������Ч�������±�־��0��ʾ����Ҫ���£�1��ʾ��Ҫ����*/

    bool buffer_need_reset;
    uint8 ok_mic_start_flag; /*0��ʾ���ˣ�1��ʾ��ʼ���棬2��ʾ��ʼ��ȡ����*/
    bool ok_mic_mute_flag; /*0��ʾ���Mute��1��ʾMute*/
    bool ready_because_fadeout_flag; /*��Ϊ���ݲ��㵭���ȴ������㹻���ٿ�ʼ������룬����ready״̬����Ҫ����OK MIC������Ҫ��ʼ����*/

    void *rec_mod_handle; /*¼��ģ����*/
    record_module_fs_para_t record_fs;
    record_module_encode_para_t encode_para;
    record_module_status_t record_status;
    buffer_rw_t encode_input_buf_w;
    uint16 encode_frame_len;
    bool record_enable;
    mmm_record_status_e mmm_record_sta;

    void *audiopp_handle; /*DAE AL���*/
    buffer_rw_t *input_buf; /*BT Input Buffer��Input Buffer ��BT ENGINEд�룬��BT PLAY�м����ȡ��ͨ�������ڴ��ȡ���ù���ṹ��ָ��*/
    buffer_rw_t aout_buf; /*decode aout buffer*/
    buffer_rw_t dac_fifo_buf; /*dac fifo to pcm buf*/

    uint16 g_sbc_sample_rate;
    uint16 g_sbc_frame_size;
    uint8  g_sbc_channel_mode;

    uint8 g_sbc_parse_flag;
    uint8 g_aout_init_flag;
    uint8 g_dae_pm_init_flag; /*��֪SBC������֮�󼴿�����DAE������ʱ��Ƶ��*/
    uint8 g_start_sub_pre_flag; /*��ʼ����Ԥ�����־*/

    uint8 decode_pp_frame_total; /*һ֡�����Ӧ����֡��*/
    uint8 decode_pp_frame_index; /*����֡���*/

    uint16 write_ptr_backup; //�����˽��������仯���ж��Ƿ��յ������ݰ�

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
