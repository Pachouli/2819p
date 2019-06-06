/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ���м���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
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

#define SPEECH_PLAY_THRES_MS            (32)    //ʱ��Ϊ32ms

#define PRE_PROCESS_BUF_LEN             (0xa00) //����Ԥ���������ݻ�������������8KHz�����ʣ�ʱ��Ϊ176ms
extern uint8 PRE_PROCESS_BUF_ADDR[PRE_PROCESS_BUF_LEN];
#define PRE_PROCESS_BUF_HIGH_THRES_DEFAULT      (60) //����60ms�ſ�ʼ����Ч���ݣ���ֹ����
#define PRE_PROCESS_BUF_MEAN_THRES_DEFAULT      (48) //������ά��48ms����
#define READY_MAX_TIMER                 (500) //��READY״̬����ʱ�䳬��500ms����ΪSCO�Ͽ���ֱ�ӿ�ʼ����

#define MIC_ADC_BUF_LEN                 (0x100) //ʱ��8ms��һ����4ms; ATS2819��L/R/L/R�����R��������
extern uint8 MIC_ADC_BUF_ADDR[MIC_ADC_BUF_LEN];

#define MIC_INPUT_BUF_LEN               (0x380) //ʱ��Ϊ56ms  �ó��ȱ����� 0x80 ��������
extern uint8 MIC_INPUT_BUF_ADDR[MIC_INPUT_BUF_LEN];
#define MIC_INPUT_BUF_LEN_THRES         (0x280) //����40ms�������ݾ���Ϊ�������ˣ�����AEC/POST AGC�����Լӿ����Ļ�����

#define PEQ_TEMP_BUFFER_LEN    (0x100)
extern uint8 PEQ_TEMP_BUFFER_ADDR[PEQ_TEMP_BUFFER_LEN];

#define PRE_AGC_DEAL_BUF_LEN            (0x100)
extern uint8 PRE_AGC_DEAL_BUF_ADDR[PRE_AGC_DEAL_BUF_LEN];

#define SPEECH_TEMP_BUF_LEN             (0x200)
extern uint8 SPEECH_TEMP_BUF_ADDR[SPEECH_TEMP_BUF_LEN];

#define KTD_INPUT_BUFFER_LEN            (0x100)
extern uint8 KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

//¼��1.5KB����������
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
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2) //��ʱӦ�þ����̣�����Ϊ2ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#else
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2 + 8*2) //��ʱӦ�þ����̣�����Ϊ2ms + 1ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#endif

//PLC��غ궨��
#define SCO_PKG_STATE_CORRECTLY_RECEIVED  0
#define SCO_PKG_STATE_POSSIBLE_INVALID    1
#define SCO_PKG_STATE_NO_DATA_RECEIVED    2
#define SCO_PKG_STATE_PARTIALLY_LOST      3

#define MY_ADJUST_SAMPLE_COUNT      (60)  //1S����һ��ͳ�ƣ�����60�β�������ͳ��
#define MY_ADJUST_SAMPLE_HIGH_COUNT (30)  //60�β�������30������ƫ�࣬����ΪCLOCKƫ��
#define MY_ADJUST_STEP_COUNT        (15)  //ƫ���С�������30���ӵ���ƫ��ϴ󼶱�

#define FADE_IN_TIME_DEFAULT        (100) //Ĭ��100ms����ʱ��
#define FILTER_START_SOUND_COUNT    (2)   //���˿�ͷ���ɸ�Block��ÿ��Block 16ms

/*!
 *  \brief
 *     ���ֲ����м������Ϣ
 */
typedef struct
{
    mmm_sp_status_t speech_status; /* ��ǰ������Ϣ */
    speech_setting_t speech_setting; /* ����������Ϣ */

    int8 is_player_loop_enable; /* ��ʶ�߳��Ƿ������ѭ��������Ĵ��� */

    int8 thread_terminate_flag;

    bool buffer_need_reset;
    uint8 ok_mic_start_flag; /*0��ʾ���ˣ�1��ʾ��ʼ���棬2��ʾ��ʼ��ȡ����*/
    bool ok_mic_mute_flag; /*0��ʾ���Mute��1��ʾMute*/
    sp_work_mode_e work_mode;

    os_event_t *speech_dec_sem; /*BT�����ź���������PCM_BUF���ȫ���ж�������߳�ͬ������*/
    volatile bool g_speech_pcm_buf_int_en_flag; /*�Ƿ��Ѿ�����PCM_BUF���ȫ�գ�������������ź�������ʱ��Ƭ*/
    volatile uint8 g_sp_pcm_buf_pending; /*1��ʾ��գ�2��ʾȫ�գ���������3��ʾ��պ�ȫ��*/

    buffer_rw_t *sco_input_buf_asqt;
    uint8 *soc_input_buf_asqt_tempbuf;
    buffer_rw_t *sco_input_buf; /*SCO Input Buffer��Input Buffer ��BT ENGINEд�룬��SPEECH�м����ȡ��ͨ�������ڴ��ȡ���ù���ṹ��ָ��*/
    buffer_rw_t *sco_output_buf; /*SCO Output Buffer��Output Buffer ��SPEECH�м��д�룬��BT ENGINE��ȡ��ͨ�������ڴ��ȡ���ù���ṹ��ָ��*/
    buffer_rw_t pre_process_buf; /*PRE PROCESS Buffer*/
    buffer_rw_t post_process_buf; /*POST PROCESS Buffer, ֻ����raw_data��length��read_ptr����pre_process_buf����һ��Buffer��
     ����Ҫע��read_ptr��Ҫ��pre_process_buf->write_ptrָ����ϵ��±�������*/
    buffer_rw_t mic_input_buf; /*MIC Input Buffer*/

    volatile bool start_play_flag; /*speech��ʼ֮��ADC�жϴ����ȵ�SCO Input Buffer�Ѿ������˲ſ�ʼ����Ч���ݲ���*/
    volatile bool g_speech_need_start;
    volatile bool g_speech_pre_enough_flag;
    volatile bool g_speech_reset_buffer_flag;

    AGCHandle *agc_handle_pre; /*Ԥ����AGC���*/
    AGCHandle *agc_handle_post; /*����AGC���*/
    void *aec_handle; /*AEC���*/
    void *plc_handle; /*PLC���*/

    int32 weak_result; /*����˥��32768�ķ���*/

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
    uint16 sco_pkg_err2_count; //ֻ������2������1�����󣬾�ͳ��2����
    uint16 sco_pkg_all_count;
    uint16 sco_pkg_err_continuous;
    uint16 sco_pkg_err_continuous_max;

    uint16 agc_temp_buf_len; /*AGC��ʱ�������е����ݳ���*/

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

    audiout_pcm_t sp_aout; /* ��ǰ֡pcm��Ϣ */
    void *audiopp_handle; /*DAE AL���*/
    uint8 fade_flag; /*���뵭��ѡ�0��ʾû�е��뵭������1��ʾ���룬2��ʾ����*/
    uint8 dae_update_flag; /*������Ч�������±�־��0��ʾ����Ҫ���£�1��ʾ��Ҫ����*/
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
    uint16 aec_deal_block; //ʱ��Ϊ16ms, post agc deal block, post peq deal block * N
    uint16 codec_deal_block; //pre process block @ sco pre deal, 1/2@cvsd or 1/4@msbc of plc_deal_block
    uint16 plc_deal_block; //pre process block
    uint16 pre_agc_deal_block; //pre peq deal block

    audio_aps_info_t audio_aps_info;
    post_process_detail_t post_process_detail;

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    /*�ز������*/
    audio_resample_info_t resample_info;
#endif

    void *rec_mod_handle; /*¼��ģ����*/
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

//�ز���ת�����뻺�����Ĵ�С��ȡת���� input samples �����ֵ��������С��64����Ϊsingle to dual��buffer
#define RESAMPLE_SINGLE_TO_DUAL_SAMPLES 32
#define RESAMPLE_INPUT_BUFF_SIZE 64
extern short RESAMPLE_INPUT_BUFFER[1][RESAMPLE_INPUT_BUFF_SIZE];

//�ز���ת������������Ĵ�С��ȡת���� output samples �����ֵ
#define RESAMPLE_OUTPUT_BUFF_SIZE 192
extern short RESAMPLE_OUTPUT_BUFFER[1][RESAMPLE_OUTPUT_BUFF_SIZE];

//�������������ز����������Ļ���
extern short RESAMPLE_TEMP_BUFF[1][RESAMPLE_TEMP_SAMPLES];
extern int RESAMPLE_TABLE_BUFF[RESAMPLE_TABLE_ELEMENTS];

extern void set_resample_info(audio_resample_info_t *resample_info, uint32 input_rate_khz);
extern void resample_and_send_pcm_to_pcm_buf(speech_player_t *speech_player, short * data, int32 sample_cnt);

//¼�����ܽӿ�
extern int32 start_speech_record(speech_player_t *speech_player);
extern int32 stop_speech_record(speech_player_t *speech_player);
extern int32 player_encode_frame_deal(speech_player_t *speech_player);
extern int32 player_encode_source_deal(speech_player_t *speech_player, short *spk_src, short *mic_src, int32 samples_count);

#endif
