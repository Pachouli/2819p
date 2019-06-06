#ifndef __HAL_AUDIO_EXTERN_H__
#define __HAL_AUDIO_EXTERN_H__

#include <attr.h>
#include <typeext.h>

typedef enum
{
    AOUT_MODE_PA_ONLY       = 0, //��PA���
    AOUT_MODE_I2STX_ONLY    = 1, //��I2STX���
    AOUT_MODE_PA_I2STX_BOTH = 2, //PA��I2STXͬʱ���
} audio_output_mode_e;

typedef enum
{
    PA_SWING_2P8_VPP = 0, //�߰ڷ�950mV
    PA_SWING_1P6_VPP = 1, //�Ͱڷ�560mV
} pa_swing_mode_e;

typedef struct
{
    audio_output_mode_e aout_mode; //��Ƶ���ģʽѡ��
    pa_swing_mode_e pa_swing; //ģ������ڷ�ѡ��
    bool pal_output : 1; //PA ������ output stage ʹ��
    bool par_output : 1; //PA ������ output stage ʹ��
    bool antipop : 1;    //�Ƿ���Ҫ����antipop����������������������������Ҫ����antipop
    bool direct_drive_enable : 1; //����ֱ��ʹ��
    uint8 i2stx_work_mode : 1; //��ɫ��0-����ɫ��1-�ӽ�ɫ
    uint8 i2stx_width : 2; //��Чλ��0-16bit��1-20bit��2-24bit
    uint8 i2stx_data_mode : 2; //���ݴ���ģʽ��0-i2sģʽ��1-����룬2-�Ҷ���
} pa_setting_t;

typedef enum
{
    DAC_SRC_DMA = 0,
    DAC_SRC_CPU = 1,
} dac_src_type_e;

typedef enum
{
    DAC_DST_DAC = 0,
    DAC_DST_I2S_TX = 1,
} dac_dst_type_e;

#define PCMBUF_IP_HE (1 << 0)
#define PCMBUF_IP_EP (1 << 1)
#define PCMBUF_IP_HF (1 << 2)
#define PCMBUF_IP_FU (1 << 3)
typedef void (*pcmbuf_irq_cbk)(uint8 pending);

#define APS_LEVEL_MAX  8
typedef struct
{
    uint8 normal_aps;
    uint8 slow_aps;
    uint8 fast_aps;
    uint8 slow_tws_slower_aps;
    uint8 slow_tws_faster_aps;
    uint8 fast_tws_slower_aps;
    uint8 fast_tws_faster_aps;
    int8  aps_freq_values[2][APS_LEVEL_MAX]; //[0]��ʾ44.1KHz, [1]��ʾ48KHz
} audio_aps_info_t;

typedef struct
{
    dac_src_type_e src_type; //DAC ����Դ
    dac_dst_type_e dst_type; //DAC ���ͨ������ѡ DAC �� I2S TX
    uint8 sample_rate; //������ 8����8KHz��22����22.05Khz��44����44.1Khz��48����48KHz �ȵ�
    uint8 aps; //AUDIO APS CLOCK΢��
    bool mono; //DAC mono ʹ�ܱ�־��FALSE��ʾ������˫������TRUE��ʾ�����ǵ���������ʹ�����ǵ����������Ҳ��תΪ˫����
    bool dacl_mute; //DAC ������ Playback MUTE
    bool dacr_mute; //DAC ������ Playback MUTE
} dac_setting_t;

typedef struct
{
    pcmbuf_irq_cbk irq_cbk; //PCM_BUF �жϷ������̻ص�����
    uint16 half_empty_thres; //PCM_BUF �����ֵ��������ż��
    uint16 half_full_thres; //PCM_BUF ������ֵ��������ż���ұȰ����ֵ��
    bool he_ie; //PCM_BUF ����ж�ʹ��
    bool ep_ie; //PCM_BUF ȫ���ж�ʹ��
    bool hf_ie; //PCM_BUF �����ж�ʹ��
    bool fu_ie; //PCM_BUF ȫ���ж�ʹ��
} pcmbuf_setting_t;

typedef enum
{
    KT_FREQ_250HZ = 0,
    KT_FREQ_330HZ = 1,
    KT_FREQ_500HZ = 2,
    KT_FREQ_1000HZ = 3,
} kt_freq_e;

typedef enum
{
    KT_PERIOD_32 = 0,
    KT_PERIOD_48 = 1,
    KT_PERIOD_64 = 2,
    KT_PERIOD_80 = 3,
} kt_period_e;

typedef struct
{
    uint8 volume; //��������������
    kt_freq_e freq; //����������Ƶ��
    kt_period_e period; //����������������
} kt_setting_t;

typedef enum
{
    AIN_SOURCE_AUX  = 0, //�߼�AUX��������AUX0L/AUX0R/AUX1L/AUX1R�Ķ������
    AIN_SOURCE_MIC1 = 1, //�߼�MIC1�����Խ���MIC0 PIN��MIC1 PIN
    AIN_SOURCE_MIC2 = 2, //�߼�MIC2�����Խ���MIC0 PIN��MIC1 PIN
    AIN_SOURCE_MAX,
} ain_source_type_e;

#define AIN_INPUT_L0_R0_DUAL  0 //AUXL0 �� AUXR0 ���룬����������������IC MIX
#define AIN_INPUT_L1_R1_DUAL  1 //AUXL1 �� AUXR1 ���룬����������������IC MIX
#define AIN_INPUT_L0_L1_MIX   2 //AUXL0 �� AUXL1 ���룬��IC MIX
#define AIN_INPUT_R0_R1_MIX   3 //AUXR0 �� AUXR1 ���룬��IC MIX
#define AIN_INPUT_L0_R0_MIX   4 //AUXL0 �� AUXR0 ���룬��IC MIX
#define AIN_INPUT_ONLY_L0     5 //IC�ⲿ��Ϻ�ͨ��AUXL0 ���룬������IC MIX
#define AIN_INPUT_ONLY_R0     6 //IC�ⲿ��Ϻ�ͨ��AUXR0 ���룬������IC MIX
#define AIN_INPUT_ONLY_L1     7 //IC�ⲿ��Ϻ�ͨ��AUXL1 ���룬������IC MIX
#define AIN_INPUT_ONLY_R1     8 //IC�ⲿ��Ϻ�ͨ��AUXR1 ���룬������IC MIX

#define AIN_INPUT_MIC0_L      0 //MIC0���룬��ADC���������
#define AIN_INPUT_MIC1_R      1 //MIC1���룬��ADC���������

typedef enum
{
    MIC_OP_26DB = 0,
    MIC_OP_30DB = 1,
    MIC_OP_31DB5 = 2,
    MIC_OP_33DB = 3,
    MIC_OP_34DB5 = 4,
    MIC_OP_36DB = 5,
    MIC_OP_37DB5 = 6,
    MIC_OP_39DB = 7,
} mic_op_gain_e;

typedef enum
{
    AIN_OP_M12DB = 0,
    AIN_OP_M3DB = 1,
    AIN_OP_0DB = 2,
    AIN_OP_1DB5 = 3,
    AIN_OP_3DB = 4,
    AIN_OP_4DB5 = 5,
    AIN_OP_6DB = 6,
    AIN_OP_7DB5 = 7,
} ain_op_gain_e;

typedef union
{
    mic_op_gain_e mic_op_gain;
    ain_op_gain_e ain_op_gain;
} op_gain_u;

typedef struct
{
    uint8     ain_input_mode;
    op_gain_u op_gain;
    bool      ain_pa_mode;
} ain_setting_t;

typedef struct
{
    uint8 sample_rate; //ADC ������ 8����8KHz��22����22.05Khz��44����44.1Khz��48����48KHz �ȵ�
    uint8 gain; //ADC ���� ATS2815 ��λ6dB ��Χ 0 ~ 2; ATS2819 ��λ3dB ��Χ 0 ~ 15
} adc_setting_t;

#define I2STX_WORK_MASTER  0
#define I2STX_WORK_SLAVE   1

#define I2STX_WIDTH_16BIT  0
#define I2STX_WIDTH_20BIT  1
#define I2STX_WIDTH_24BIT  2

#define I2STX_DATA_I2S_MODEL     0
#define I2STX_DATA_LEFT_JUSTED   1
#define I2STX_DATA_RIGHT_JUSTED  2

#define I2SRX_WORK_MASTER  0
#define I2SRX_WORK_SLAVE   1

#define I2SRX_WIDTH_16BIT  0
#define I2SRX_WIDTH_20BIT  1
#define I2SRX_WIDTH_24BIT  2

#define I2SRX_DATA_I2S_MODEL     0
#define I2SRX_DATA_LEFT_JUSTED   1
#define I2SRX_DATA_RIGHT_JUSTED  2

typedef struct
{
    uint8 sample_rate;
    uint8 i2srx_work_mode : 1; //��ɫ��0-����ɫ��1-�ӽ�ɫ
    uint8 i2srx_slave_mclk_use_inner : 1; //I2SRX��ΪSlaveʹ��ʱ��֧�ֲ�������96KHz�����£����Բ��ý�MCLK����ʡһ��PIN
    uint8 i2srx_width : 2; //��Чλ��0-16bit��1-20bit��2-24bit
    uint8 i2srx_data_mode : 2; //���ݴ���ģʽ��0-i2sģʽ��1-����룬2-�Ҷ���
} i2srx_setting_t;

#endif

