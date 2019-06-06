/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ADCͨ��ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "adc_channel_extern.h"

#define MONO_ADC     0 //������
#define DUAL_ADC     1 //������

#define FADEIN_STEP  200 //ͨ��200�����㵭��

#define SUPPORT_MUTE_LOWPOWER       1   //������ʱmute��MIC

#define MUTE_LOWPOWER_OFF_TH_RMS    10  //MIC�ϵ���ֵ
#define MUTE_LOWPOWER_OFF_TIMES     750 //MIC�ϵ�ʱ�䣬֡����1֡1.4ms��2.8ms
#define MUTE_LOWPOWER_AT_TH_RMS     60  //����mute��ֵ
#define MUTE_LOWPOWER_AT_TIMES      1500//����muteʱ�䣬֡����1֡1.4ms��2.8ms
#define MUTE_LOWPOWER_RL_TH_RMS     120 //�ͷ�mute��ֵ
#define MUTE_LOWPOWER_RL_TIMES      8   //�ͷ�muteʱ�䣬֡����1֡1.4ms��2.8ms
#define MUTE_LOWPOWER_RL_OFF_TIMES  25  //�ͷ�offʱ�䣬֡����1֡1.4ms��2.8ms

#define LOWPOWER_STA_NORMAL   0
#define LOWPOWER_STA_LOW      1
#define LOWPOWER_STA_OFF      2

#define MUTE_FADE_IN          1
#define MUTE_FADE_OUT         2

/*!
 *  \brief
 *     ���ֲ����м������Ϣ
 */
typedef struct
{
    adc_channel_status_e ac_status;
    adc_energy_info_t    adc_energy;
    adc_chanel_setting_t adc_chan_set;
    adc_channel_buffer_setting_t adc_chan_buf_set;

    buffer_rw_t adc_chan_buf_w;
    buffer_rw_t mic_chan_buf_w;
    uint8 *mic_aux_temp_buffer_256;

    buffer_rw_t resample_left_chan_buf;
    buffer_rw_t resample_right_chan_buf;
    uint8 *resample_split_buff;

    uint16 pcm_adc_block_size; //DMA Bufferһ�룬������ճ��ȣ����512�ֽ�=1k/2

    bool is_adc_dual_flag;//�� MONO_ADC,DUAL_ADC ����
    uint8 g_aout_init_flag;

    uint8 zero_frame_count;
    uint16 fadein_point_count;

    bool maybe_spdif_title_flag : 1;
    bool need_fadein_flag : 1;
    bool sample_rate_ok_flag : 1;
    bool sample_rate_need_update : 1;
    bool resample_enable : 1;
    bool need_resample : 1;
    bool start_buffer_data_flag : 1;
    uint8 resample_src_khz;
    uint8 start_buffer_data_delay;
    uint8 sample_rate_count;
    uint8 sample_rate_khz;
    uint32 sample_rate_acc;

    uint32 adc_lowpw_frame;

#if (SUPPORT_MUTE_LOWPOWER == 1)
    uint8 lowpower_mute_flag;  //0������1��������2 MIC�ϵ�
    uint8 highpower_continues; //10ms����
    uint16 lowpower_continues; //1S����
    uint16 off_continues;      //1S����
#endif
} adc_channel_info_t;

extern adc_channel_info_t g_adc_channel_info;

extern int32 adc_channel_cmd_inner(void *handel, adc_channel_cmd_e cmd, uint32 args);
extern void * adc_channel_open(void *param);
extern int32 adc_channel_close(adc_channel_info_t *p_ac_info, void *param);
extern int32 adc_channel_adc_open(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_adc_close(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_start(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_stop(adc_channel_info_t *p_ac_info);
extern void adc_channel_calc_energy(adc_channel_info_t *p_ac_info, short *frame_buf, uint16 sample_cnt);
extern void adc_channel_calc_mic_energy(adc_channel_info_t *p_ac_info, short *frame_buf, uint16 sample_cnt);

extern bool adc_channel_feedback_data_func(short *p_data_adc_read, uint16 pcm_adc_buf_block);
extern void spdif_dvd_title_frame_deal(adc_channel_info_t *p_ac_info, short *p_data_adc_read, uint16 pcm_adc_buf_block);
extern void spdif_sample_rate_deal(adc_channel_info_t *p_ac_info);
extern void spdif_resample_predeal(adc_channel_info_t *p_ac_info, short *p_data_adc_read, uint16 pcm_adc_buf_block);

extern void pcm_adc_dma_int_cbk(uint8 pending);
extern int32 adc_channel_data_input_if_aux(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_aux_exit(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_mic(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_mic_exit(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_mic_aux(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_mic_aux_exit(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_i2srx(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_i2srx_exit(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_spdifrx(adc_channel_info_t *p_ac_info);
extern int32 adc_channel_data_input_if_spdifrx_exit(adc_channel_info_t *p_ac_info);
