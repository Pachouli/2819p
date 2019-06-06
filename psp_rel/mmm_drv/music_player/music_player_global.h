/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز����м��  ͷ�ļ�
 ********************************************************************************/

#include "audio_decoder_lib_dev.h"
#include "audio_post_process.h"
#include "mmm_mp.h"


//#define MUSIC_MMM_DEBUG_INFO //debug�źſ��أ��򿪻��ӡ�϶�ϸ����Ϣ

#define KTD_INPUT_BUFFER_LEN   (0x100)
extern uint8 KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];

#define DAE_128_ALIGN_BUF_LEN  (0x200)
extern uint8 DAE_128_ALIGN_BUF_ADDR[DAE_128_ALIGN_BUF_LEN];

#define FADE_IN_TIME_DEFAULT   (100) //Ĭ��100ms����ʱ��
#define FADE_OUT_TIME_DEFAULT  (100) //Ĭ��100ms����ʱ��

#define FORMAT_CHECK_TEMP_BUF_LEN   (0x800)
extern uint8 FORMAT_CHECK_TEMP_BUF_ADDR[FORMAT_CHECK_TEMP_BUF_LEN];

#if (SUPPORT_OK_MIC == 1)
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE/2)
#else
#define MY_POST_PROCESS_FRAME_SAMPLE  (POST_PROCESS_FRAME_SAMPLE)
#endif

extern uint8 decode_output_cache[0x2000];
extern uint8 OK_MIC_TEMP_BUFF[0x100];
extern uint8 OK_MIC_DECODE_BUFF[0x200];

//OK MIC
#if (ENABLE_OK_MIC_FAST == 1)
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2 + 44) //��ʱӦ�þ����̣�����Ϊ1.4ms + 0.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#else
#define OK_MIC_PCM_BUF_HE_THRESHOLD (MY_POST_PROCESS_FRAME_SAMPLE*2 + 44*3) //��ʱӦ�þ����̣�����Ϊ1.4ms + 1.5ms
#define OK_MIC_RAW_BUF_THRESHOLD    (MY_POST_PROCESS_FRAME_SAMPLE*2)
#endif

//tts����Ƭ����Ϣ
typedef struct
{
    uint32 offset; //����Ƭ����ʼ��ַ
    uint32 length; //����Ƭ�����ݳ���
} tts_section_t;

//tts����Ƭ����Ϣ
typedef struct
{
    uint32 offset;              //����Ƭ����ʼ��ַ
    uint32 length    : 20;      //����Ƭ�����ݳ��ȣ�ÿ��Ƭ���1MB�������ЩƬ�γ���1MB����ô���޷�֧��v2
    uint32 tail_zero : 12;      //����Ƭ��ĩβ���кܶ�0���ݵ�����֡���ܳ���
} tts_section_v2_t;

typedef struct
{
    uint16 *p_tts_section_ids;
    uint8 tts_version; //v1=1,v2=2 ѹ��Ƭ��ĩβ0����
    uint8 tts_section_cnt;
    uint8 tts_header_len; //�ļ�ͷ���ȣ�0��16
    uint8 tts_mp3_head_read_bytes;
    uint32 tts_cur_length; //��ǰС�ڳ���
    uint32 tts_cur_zero_offset; //v2��Ч����ǰƬ��ѹ��֡ƫ�Ƶ�ַ
    uint32 tts_cur_offset; //��ǰС��ƫ�Ƶ�ַ
    uint32 tts_all_length; //����TTS�ܳ���
    uint32 tts_all_offset; //����TTSƫ�Ƶ�ַ
    uint8 tts_mp3_head[16]; //4�ֽ�ͷ+12�ֽ�ȫ0
} tts_info_t;

typedef struct
{
    bool music_file_has_endtag;        //�ļ�id3v1 ��־
    bool read_first_frame_data;         //��ȡ��һ֡���ݱ�־
    bool is_loop_play;                 // �����޷첥��
    bool filter_mute_frame;            //����֡��Ƶ
    bool filter_head_mute_frame_flag;  //�ӵ�һ֡��ʼ����
    int32 musicfile_actual_len;        //�ļ�ʵ�ʳ���
    int32 first_offset;                //��һ֡���ݵ�ƫ�Ƶ�ַ
} music_loopplay_info_t;

/*!
 *  \brief
 *     ���ֲ����м������Ϣ
 */
typedef struct
{
    music_setting_t music_setting; /* ����������Ϣ */
    mmm_mp_fs_para_t fs_para;
    storage_io_t storage_io; /* �����ļ����� */
    music_info_t music_info; /* ������Ϣ */
    music_loopplay_info_t music_loopplay_info; /*�޷첥����Ϣ*/

    /* ���ֲ��ŵ�ʵʱ������Ϣ */
    mmm_mp_status_t music_status; /* ��ǰ������Ϣ */
    audiout_pcm_t aout; /* ��ǰ֡pcm��Ϣ */
    audiout_pcm_t aout_pp; /* ��ǰ����֡pcm��Ϣ */
    music_decode_info_t music_decode_info;/* ��ǰ֡������Ϣ */
    int32 music_ffrev_flag; /* ��ǰ�������Ϣ��>0��ʾ�����<0��ʾ���� */
    int32 music_seek_flag; /* ��ǰ�������Ϣ */
    int32 music_fade_flag; /* ��ǰ���뵭����Ϣ��0-�޵��뵭����1-����;2-���� */
    int32 music_stop_flag; /* ��ǰֹͣ��Ϣ */
    int32 music_dec_samples; /* ��ǰ�߳̽��볤����Ϣ */
    int32 music_mute_samples; /* ��ǰ��Ҫ�������������� */

    bool is_player_loop_enable; /* ��ʶ�߳��Ƿ������ѭ��������Ĵ��� */
    bool is_ok_mic_loop_enable; /* ��ʶ�߳��Ƿ������ѭ��������Ĵ��� */
    volatile bool ok_mic_wait_decode_first; /* OK MIC �̵߳ȴ������߳̽����һ֡ */
    volatile bool decode_wait_ok_mic_post_sem;

    mp_work_mode_e work_mode;
    bool buffer_need_reset;
    uint8 ok_mic_start_flag; /*0��ʾ���ˣ�1��ʾ��ʼ���棬2��ʾ��ʼ��ȡ����*/
    bool ok_mic_mute_flag; /*0��ʾ���Mute��1��ʾMute*/

    buffer_rw_t decode_output_cache_rw; /*�����߳�д�룬OK MIC�̶߳���*/

    int32 audiodec_type; /* ��ʶ��Ƶ������Ƿ񱻼��� */
    void *audiodec_handle; /* ��Ƶ�����߳̾�� */

    void *adc_channel_handle;
    buffer_rw_t *p_mic_input_buf_r; /* OK MIC Input Buffer */

    int8 thread_terminate_flag;
    int8 ok_mic_thread_terminate_flag;
    void *output_file; /* ����ļ� */

    uint8 fade_flag; /*���뵭��ѡ�0��ʾû�е��뵭������1��ʾ���룬2��ʾ����*/
    uint8 dae_update_flag; /*������Ч�������±�־��0��ʾ����Ҫ���£�1��ʾ��Ҫ����*/
    void *audiopp_handle; /*DAE AL���*/

    os_event_t *music_dec_sem; /*��OK MICģʽ�£�������OK MIC�߳�ͬ������ʵ���ͷ�ʱ��Ƭ*/
    os_event_t *music_pcmbuf_sem; /*����PCM_BUF���ȫ���ж�������߳�ͬ������*/

    volatile bool g_mp_pcm_buf_int_en_flag; /*�Ƿ��Ѿ�����PCM_BUF���ȫ�գ�������������ź�������ʱ��Ƭ*/
    volatile uint8 g_mp_pcm_buf_pending; /*1��ʾ��գ�2��ʾȫ�գ���������3��ʾ��պ�ȫ��*/

    uint8 aout_ch_num;
    uint8 aout_sample_bits;
    uint8 aout_sample_bytes;
    uint8 aout_lch_sample_bytes;

    buffer_rw_t aout_lch_buf; /*decode aout left channel buffer*/
    buffer_rw_t aout_rch_buf; /*decode aout right channel buffer*/
    buffer_rw_t dac_fifo_buf; /*dac fifo to pcm buf*/

    tts_info_t *p_tts_info; /*TTS PLAY INFO*/
    bool tts_mode;
    bool format_check_flag;

    uint8 sample_rate_index; /*48K=0,44K=1,32K=2,24K=3,22K=4,16K=5,12K=6,11K=7,8K=8*/

    bool g_aout_init_flag; /*AUDIO inited flag*/

    uint32 dec_play_time;
    uint32 dae_play_time;
    uint32 resample_time;
    uint32 release_time;
    uint32 read_card_time;
    uint32 play_samples_add;
    bool need_print_time;
    uint16 dae_freq;
    uint16 decode_freq;

    uint16 g_half_empty_thres;
    uint16 g_half_full_thres;
    bool pcm_buf_hd_err;
    bool storage_read_err;

    uint16 last_unalign_frame_sc; //��һ����֡��128���������ĵ���

    uint32 music_lowpw_frame;

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

    audio_aps_info_t audio_aps_info;

    post_process_detail_t post_process_detail;

#if (SUPPORT_AUDIO_RESAMPLE == 1)
    /*�ز������*/
    audio_resample_info_t resample_info;
#endif
} music_player_t;

typedef struct tAudioParam
{
    /*  Audio properties    */
    unsigned int nVersion;
    unsigned int nSamplesPerSec;
    unsigned int nAvgBytesPerSec;
    unsigned int nBlockAlign;
    unsigned int nChannels;
    unsigned int nSamplesPerBlock;
    unsigned int nEncodeOpt;

    /*  ASF Header  */
    unsigned int dwChannelMask;
    unsigned int cbHeader;
    unsigned int cbPacketSize;
    unsigned int cPackets;
    unsigned int nBitsPerSample;
    unsigned int wAudioStreamId;
    unsigned int TotalTime;
    unsigned int cbPreroll;
} AUDIOPARAM;

extern AUDIOPARAM *piAudioparam_global;

#define MAX_OBJ_HDR_LEN             (36)//4//����Ϊ4�ı���
#define FORMAT_CHECK_BUF_LEN        (512 + MAX_OBJ_HDR_LEN)

extern PP_para_info_t g_PP_para_info;
extern void update_PP_para_info(music_player_t *music_player);
extern void set_music_decoder_clk(music_player_t *music_player);

extern music_player_t* music_player_global;

extern freq_point_config_t g_freq_point_cfg;

extern bool is_music_file_has_idv3v1(music_player_t *music_player, void *buf);
extern int32 loopplay_find_musicfile_first_frame_count(music_player_t *music_player, int32 count);
extern int32 loopplay_find_musicfile_last_frame_data(music_player_t *music_player, int32 count);
extern int32 loopplay_is_the_last_full_play_end(music_player_t *music_player, int32 count);
extern bool filter_mute_frame(music_player_t *music_player);

//�ļ���ʽ���ͼ��ʱ����ʱbuf;
extern uint8* format_check_temp_buf;

extern int32 mmm_mp_read(void *buf, int32 size, int32 count, storage_io_t *io);
extern int32 mmm_mp_seek(storage_io_t *io, int32 offset, int32 whence);
extern int32 mmm_mp_tell(storage_io_t *io);
extern int32 mmm_mp_read_sd(void *buf, int32 size, int32 count, storage_io_t *io);
extern int32 mmm_mp_seek_sd(storage_io_t *io, int32 offset, int32 whence);
extern int32 mmm_mp_tell_sd(storage_io_t *io);

extern int32 mmm_mp_read_res(void *buf, int32 size, int32 count, storage_io_t *io);
extern int32 mmm_mp_seek_res(storage_io_t *io, int32 offset, int32 whence);
extern int32 mmm_mp_tell_res(storage_io_t *io);
extern int32 mmm_mp_read_extlib(void *buf, int32 size, int32 count, storage_io_t *io);
extern int32 mmm_mp_seek_extlib(storage_io_t *io, int32 offset, int32 whence);
extern int32 mmm_mp_tell_extlib(storage_io_t *io);

extern void *player_loop(music_player_t *music_player);
extern void *ok_mic_loop(music_player_t *music_player);

extern void *mmm_mp_open(void *param);
extern int32 mmm_mp_aout_open(music_player_t *music_player, void *param);
extern int32 mmm_mp_close(music_player_t *music_player, void *param);
extern int32 mmm_mp_aout_close(music_player_t *music_player, void *param);
extern bool get_sample_rate_index(music_player_t *music_player);
extern void music_pcm_buf_int_cbk(uint8 pending);
extern void init_pcm_buf(uint32 half_empty_thres, uint32 half_full_thres);
extern void send_pcm_to_pcm_buf_by_dac_fifo(short *pcm_buffer, uint32 sample_cnt);

extern int32 mmm_mp_init_file(music_player_t *music_player, uint32 args);
extern int32 mmm_mp_clear_file(music_player_t *music_player);

extern int32 get_media_info(music_player_t *music_player, uint32 args);
extern int32 start_music_decoder(music_player_t *music_player);
extern int32 stop_music_decoder(music_player_t *music_player);
extern int32 stop_music_decoder_wait_pcmbuf_empty(music_player_t *music_player);
extern int32 update_ffrev_setting(music_player_t *music_player, uint32 args);
extern int32 seek_process(music_player_t *music_player, int32 *delay_time);
extern int32 audio_mmm_mp_cmd(void *handel, mmm_mp_cmd_t cmd, uint32 args);

extern int32 read_n_bytes(music_info_t *music_info, int32 count, uint8 *header);
extern int32 skip_n_bytes(music_info_t *music_info, int32 count);
extern int32 set_n_bytes(music_info_t *music_info, int32 count);

extern int32 tts_read_sd(sd_handle tts_rc_handle, void *buf, int32 count);
extern bool tts_play_init(music_player_t *music_player, uint16 *section_ids, uint8 section_cnt);
extern bool tts_play_exit(music_player_t *music_player);

extern int32 tts_read_ext(lib_file_t * tts_rc_handle, void *buf, int32 count);

extern int32 mp3check(music_info_t *music_info, bool only_mono);
extern int32 wavcheck(music_info_t *music_info);
extern int32 wmacheck(music_info_t *music_info);
extern int32 flaccheck(music_info_t *music_info);
extern int32 apecheck(music_info_t *music_info);
extern int32 format_check(music_info_t *music_info);

extern int32 keytone_start(music_player_t *music_player, void *param);
extern int32 keytone_stop(music_player_t *music_player);
extern int32 keytone_frame_deal(music_player_t *music_player, short *pcm_out, uint16 samples_count);
extern int32 keytone_playend_deal(music_player_t *music_player);

extern int32 loop_send_pcm_to_pcm_buf(music_player_t *music_player, short *send_data, uint16 samples_cnt, \
    bool *pcm_buf_full_flag, bool ignore_dec_too_late);

#if (SUPPORT_AUDIO_RESAMPLE == 1)

//�ز���ת�����뻺�����Ĵ�С��ȡת���� input samples �����ֵ
#define RESAMPLE_INPUT_BUFF_SIZE 128
extern short RESAMPLE_INPUT_BUFFER[2][RESAMPLE_INPUT_BUFF_SIZE];

//�ز���ת������������Ĵ�С��ȡת���� output samples �����ֵ
#define RESAMPLE_OUTPUT_BUFF_SIZE 256
extern short RESAMPLE_OUTPUT_BUFFER[3][RESAMPLE_OUTPUT_BUFF_SIZE];

//�������������ز����������Ļ���
extern short RESAMPLE_TEMP_BUFF[2][RESAMPLE_TEMP_SAMPLES];
extern int RESAMPLE_TABLE_BUFF[RESAMPLE_TABLE_ELEMENTS];

extern void set_resample_info(audio_resample_info_t *resample_info, uint32 input_rate_khz);
extern void aout_turn_mix_to_seperated_16bit_format(short * left_output_buff, short * right_output_buff, short * mixed_input_buff, int32 samples);
extern void resample_turn_seperated_to_mix_16bit_format(int32 samples);
extern bool resample_and_send_pcm_to_pcm_buf(music_player_t *music_player, short *in_data, int32 samples_cnt, \
		bool *pcm_buf_full_flag, bool ignore_dec_too_late);

#endif
