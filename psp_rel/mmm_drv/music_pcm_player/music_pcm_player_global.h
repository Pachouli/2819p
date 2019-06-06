/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������AUX/FM/USB�����м��ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "audio_decoder_lib_dev.h"
#include "audio_post_process.h"
#include "mmm_pp.h"

//#define PCM_MMM_DEBUG_INFO //debug�źſ��أ��򿪻��ӡ�϶�ϸ����Ϣ

#define PCM_DEC_BUF_BLOCK   (PCM_ADC_BUF_BLOCK) //NOTE��������POST_PROCESS_FRAME_SAMPLE��4��������������
extern uint8 PCM_DEC_BUF_ADDR[PCM_DEC_BUF_BLOCK];

#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE/2) //�鴦���С����һ�룬������������ʱ
#define MY_PCM_DEC_BUF_BLOCK          (PCM_DEC_BUF_BLOCK/2) //�鴦���С����һ�룬������������ʱ

#if (SUPPORT_OK_MIC == 1)
extern uint8 ok_mic_temp_buffer[0x100];
extern uint8 ok_mic_aux_temp_buffer[0x100];
#endif

#define MY_ADJUST_SAMPLE_COUNT      (100) //300ms����һ��ͳ��
#define MY_ADJUST_SAMPLE_HIGH_COUNT (50)  //100�β�������50������ƫ�࣬����ΪCLOCKƫ��
#define MY_ADJUST_STEP_COUNT        (10)  //ƫ���С�������3���ӵ���ƫ��ϴ󼶱�
#define MY_ADJUST_LENGTH_THRES      (PCM_ADC_BUF_BLOCK * 4)   //USB�������CLOCK��BLOCK��ֵ����ԼΪ12ms
#define MY_FADE_OUT_LENGTH_THRES    (PCM_DEC_BUF_BLOCK * 2)   //��������������6msʱ����

#define PCM_DEC_BUF_START_THRES     (PCM_ADC_BUF_BLOCK * 3) //��ʼ����ʱ����������Ϊ3��BLOCK

#define FADE_IN_TIME_DEFAULT        (100) //Ĭ��100ms����ʱ��
#define FADE_OUT_TIME_DEFAULT       (100) //Ĭ��100ms����ʱ��

#define PCM_BUF_HE_THRESHOLD        (0x240) //6ms@48k sr

//OK MIC
#if (ENABLE_OK_MIC_FAST == 1)
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2 + 44) //��ʱӦ�þ����̣�����Ϊ1.4ms + 0.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#else
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2 + 44*3) //��ʱӦ�þ����̣�����Ϊ1.4ms + 1.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#endif

#define KTD_INPUT_BUFFER_LEN   (0x80)
extern uint8 KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

//�ز��������������
extern short RESAMPLE_INPUT_BUFFER[128];
extern short RESAMPLE_OUTPUT_BUFFER[2][64];

//�������������ز����������Ļ���
extern short RESAMPLE_TEMP_BUFF[2][835];
extern int RESAMPLE_TABLE_BUFF[312];

//�����������뻺����
extern uint8 RESAMPLE_LEFT_CHAN_BUFF[0x800];
extern uint8 RESAMPLE_RIGHT_CHAN_BUFF[0x800];
extern uint8 RESAMPLE_LR_SPLIT_BUFF[0x100];

//¼��ģ�����軺����
extern uint8 encode_aout_buffer[ENCODE_OUT_BUFFER_SIZE];
extern uint8 RECORD_FILE_ENCODE_IN_BUF_ADDR[ENCODE_IN_FRAME_SIZE*2];

#define RECORD_ENCODE_OUTPUT_CACHE_LEN   (0x2000)
extern uint8 RECORD_FILE_ENCODE_OUT_BUF_ADDR[RECORD_ENCODE_OUTPUT_CACHE_LEN];

#define RECORD_FILE_TEMP_BUF_LEN   (0x800)
extern uint8 RECORD_FILE_TEMP_BUF_ADDR[RECORD_FILE_TEMP_BUF_LEN];


/*!
 *  \brief
 *     ���ֲ����м������Ϣ
 */
typedef struct
{
    mmm_pp_status_t music_status; /* ��ǰ������Ϣ */
    music_pcm_setting_t music_setting; /* ����������Ϣ */
    audiout_pcm_t pp_aout; /* ��ǰ֡pcm��Ϣ������DAE */

    void *rec_mod_handle; /*¼��ģ����*/
    record_module_fs_para_t record_fs;
    record_module_encode_para_t encode_para;
    record_module_status_t record_status;
    buffer_rw_t encode_input_buf_w;
    uint16 encode_frame_len;
    bool record_enable;
    mmm_record_status_e mmm_record_sta;

    int8 is_player_loop_enable; /* ��ʶ�߳��Ƿ������ѭ��������Ĵ��� */
    int8 thread_terminate_flag;
    int8 is_monitoring_flag; /* �Ƿ����������ܣ�Ĭ�Ͽ��� */
    int8 is_idle_mode; /*�Ƿ����ģʽ��Ĭ�Ϲر�*/

    os_event_t *pcm_dec_sem; /*PCM�����ź���������PCM_BUF���ȫ���ж�������߳�ͬ������*/
    volatile bool g_pp_pcm_buf_int_en_flag; /*�Ƿ��Ѿ�����PCM_BUF���ȫ�գ�������������ź�������ʱ��Ƭ*/
    volatile uint8 g_pp_pcm_buf_pending; /*1��ʾ��գ�2��ʾȫ�գ���������3��ʾ��պ�ȫ��*/

    uint8 fade_flag; /*���뵭��ѡ�0��ʾû�е��뵭������1��ʾ���룬2��ʾ����*/
    uint8 dae_update_flag; /*������Ч�������±�־��0��ʾ����Ҫ���£�1��ʾ��Ҫ����*/
    bool buffer_need_reset;
    uint8 ok_mic_start_flag; /*0��ʾ���ˣ�1��ʾ��ʼ���棬2��ʾ��ʼ��ȡ����*/
    bool ok_mic_mute_flag; /*0��ʾ���Mute��1��ʾMute*/
    pp_work_mode_e work_mode;

    void *audiopp_handle; /*DAE AL���*/
    buffer_rw_t *p_input_buf_r; /*PCM Input Buffer Pointer for All include AUX/FM/USB SOUND*/
    buffer_rw_t *p_micin_buf_r;

    //�����˽��������仯
    uint16 adjust_sample_counter;
    uint16 adjust_sample_high_counter;
    uint16 adjust_step_cnt;
    uint8 audio_aps;
    bool need_adjust_aps;

    uint32 dec_play_time;
    uint32 dae_play_time;
    uint32 enc_play_time;
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

    bool is_adc_dual_flag;
    uint8 g_aout_init_flag;

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

    void *adc_channel_handle;

    audio_aps_info_t audio_aps_info;

    uint8 usound_max_block;
    uint8 usound_min_block;

    bool need_update_samplerate;
    uint16 adjust_block_thres;

    post_process_detail_t post_process_detail;

    adc_chan_feedback_data_cbk feedback_data_cbk;
    buffer_rw_t *p_resample_lch_r;
    buffer_rw_t *p_resample_rch_r;
    void *resample_handle_left;     //�������ز������
    void *resample_handle_right;    //�������ز������
    bool need_resample;
    uint8 resample_src_khz;
    uint8 resample_dest_khz;
} music_pcm_player_t;

extern music_pcm_player_t* music_player_global;

extern freq_point_config_t g_freq_point_cfg;

extern PP_para_info_t g_PP_para_info;
extern void update_PP_para_info(music_pcm_player_t *music_player);
extern void set_music_decoder_clk(music_pcm_player_t *music_player);

extern void *player_loop(music_pcm_player_t *music_player);

extern void *mmm_pp_open(void *param) __FAR__;
extern int32 mmm_pp_close(music_pcm_player_t *music_player, void *param) __FAR__;

extern int32 mmm_pp_aout_open(music_pcm_player_t *music_player);
extern int32 mmm_pp_aout_close(music_pcm_player_t *music_player);
extern void wait_dma_complete(void);
extern void send_pcm_to_pcm_buf_by_dac_fifo(short *pcm_buffer, uint32 sample_cnt);
extern void init_pcm_buf(uint32 half_empty_thres, uint32 half_full_thres);

extern void pcm_pcm_buf_int_cbk(uint8 pending);

extern int32 start_music_decoder(music_pcm_player_t *music_player);
extern int32 start_music_decoder_sub(music_pcm_player_t *music_player);
extern int32 stop_music_decoder(music_pcm_player_t *music_player);
extern int32 stop_music_decoder_fadeout_start(music_pcm_player_t *music_player, bool pause);
extern int32 stop_music_decoder_fadeout_wait_cp(music_pcm_player_t *music_player);
extern int32 stop_music_decoder_sub(music_pcm_player_t *music_player);

extern int32 audio_mmm_pp_cmd(void *handel, mmm_pp_cmd_t cmd, uint32 args);

extern void monitor_aps_adjust(music_pcm_player_t *music_player);
extern void monitor_fadeout_check(music_pcm_player_t *music_player);

extern int32 keytone_start(music_pcm_player_t *music_player, void *param);
extern int32 keytone_stop(music_pcm_player_t *music_player);
extern int32 keytone_frame_deal(music_pcm_player_t *music_player, short *pcm_out, uint16 samples_count);
extern int32 keytone_playend_deal(music_pcm_player_t *music_player);

extern int32 start_music_decoder_load_audiopp(music_pcm_player_t *music_player);
extern int32 start_music_decoder_load_adc_chan(music_pcm_player_t *music_player);
extern int32 start_resample_module(music_pcm_player_t *music_player);

extern int32 start_music_record(music_pcm_player_t *music_player);
extern int32 stop_music_record(music_pcm_player_t *music_player);
extern int32 player_encode_frame_deal(music_pcm_player_t *music_player);
extern int32 player_encode_source_deal(music_pcm_player_t *music_player, short *src, int32 samples_count);
