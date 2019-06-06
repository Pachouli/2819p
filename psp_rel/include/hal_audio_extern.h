#ifndef __HAL_AUDIO_EXTERN_H__
#define __HAL_AUDIO_EXTERN_H__

#include <attr.h>
#include <typeext.h>

typedef enum
{
    AOUT_MODE_PA_ONLY       = 0, //仅PA输出
    AOUT_MODE_I2STX_ONLY    = 1, //仅I2STX输出
    AOUT_MODE_PA_I2STX_BOTH = 2, //PA和I2STX同时输出
} audio_output_mode_e;

typedef enum
{
    PA_SWING_2P8_VPP = 0, //高摆幅950mV
    PA_SWING_1P6_VPP = 1, //低摆幅560mV
} pa_swing_mode_e;

typedef struct
{
    audio_output_mode_e aout_mode; //音频输出模式选择
    pa_swing_mode_e pa_swing; //模拟输出摆幅选择
    bool pal_output : 1; //PA 左声道 output stage 使能
    bool par_output : 1; //PA 右声道 output stage 使能
    bool antipop : 1;    //是否需要进行antipop，方案决定，如果带耳机输出则需要进行antipop
    bool direct_drive_enable : 1; //耳机直驱使能
    uint8 i2stx_work_mode : 1; //角色，0-主角色，1-从角色
    uint8 i2stx_width : 2; //有效位宽，0-16bit，1-20bit，2-24bit
    uint8 i2stx_data_mode : 2; //数据传输模式，0-i2s模式，1-左对齐，2-右对齐
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
    int8  aps_freq_values[2][APS_LEVEL_MAX]; //[0]表示44.1KHz, [1]表示48KHz
} audio_aps_info_t;

typedef struct
{
    dac_src_type_e src_type; //DAC 数据源
    dac_dst_type_e dst_type; //DAC 输出通道，可选 DAC 或 I2S TX
    uint8 sample_rate; //采样率 8代表8KHz，22代表22.05Khz，44代表44.1Khz，48代表48KHz 等等
    uint8 aps; //AUDIO APS CLOCK微调
    bool mono; //DAC mono 使能标志；FALSE表示数据是双声道，TRUE表示数据是单声道；即使数据是单声道，输出也会转为双声道
    bool dacl_mute; //DAC 左声道 Playback MUTE
    bool dacr_mute; //DAC 右声道 Playback MUTE
} dac_setting_t;

typedef struct
{
    pcmbuf_irq_cbk irq_cbk; //PCM_BUF 中断服务例程回调函数
    uint16 half_empty_thres; //PCM_BUF 半空阈值，必须是偶数
    uint16 half_full_thres; //PCM_BUF 半满阈值，必须是偶数且比半空阈值大
    bool he_ie; //PCM_BUF 半空中断使能
    bool ep_ie; //PCM_BUF 全空中断使能
    bool hf_ie; //PCM_BUF 半满中断使能
    bool fu_ie; //PCM_BUF 全满中断使能
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
    uint8 volume; //按键音方波幅度
    kt_freq_e freq; //按键音方波频率
    kt_period_e period; //按键音方波周期数
} kt_setting_t;

typedef enum
{
    AIN_SOURCE_AUX  = 0, //逻辑AUX，可以是AUX0L/AUX0R/AUX1L/AUX1R的多种组合
    AIN_SOURCE_MIC1 = 1, //逻辑MIC1，可以接在MIC0 PIN或MIC1 PIN
    AIN_SOURCE_MIC2 = 2, //逻辑MIC2，可以接在MIC0 PIN或MIC1 PIN
    AIN_SOURCE_MAX,
} ain_source_type_e;

#define AIN_INPUT_L0_R0_DUAL  0 //AUXL0 和 AUXR0 输入，立体声输入无须在IC MIX
#define AIN_INPUT_L1_R1_DUAL  1 //AUXL1 和 AUXR1 输入，立体声输入无须在IC MIX
#define AIN_INPUT_L0_L1_MIX   2 //AUXL0 和 AUXL1 输入，在IC MIX
#define AIN_INPUT_R0_R1_MIX   3 //AUXR0 和 AUXR1 输入，在IC MIX
#define AIN_INPUT_L0_R0_MIX   4 //AUXL0 和 AUXR0 输入，在IC MIX
#define AIN_INPUT_ONLY_L0     5 //IC外部混合后通过AUXL0 输入，无需在IC MIX
#define AIN_INPUT_ONLY_R0     6 //IC外部混合后通过AUXR0 输入，无需在IC MIX
#define AIN_INPUT_ONLY_L1     7 //IC外部混合后通过AUXL1 输入，无需在IC MIX
#define AIN_INPUT_ONLY_R1     8 //IC外部混合后通过AUXR1 输入，无需在IC MIX

#define AIN_INPUT_MIC0_L      0 //MIC0输入，从ADC左声道输出
#define AIN_INPUT_MIC1_R      1 //MIC1输入，从ADC右声道输出

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
    uint8 sample_rate; //ADC 采样率 8代表8KHz，22代表22.05Khz，44代表44.1Khz，48代表48KHz 等等
    uint8 gain; //ADC 增益 ATS2815 单位6dB 范围 0 ~ 2; ATS2819 单位3dB 范围 0 ~ 15
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
    uint8 i2srx_work_mode : 1; //角色，0-主角色，1-从角色
    uint8 i2srx_slave_mclk_use_inner : 1; //I2SRX作为Slave使用时，支持采样率在96KHz及以下，可以不用接MCLK，节省一个PIN
    uint8 i2srx_width : 2; //有效位宽，0-16bit，1-20bit，2-24bit
    uint8 i2srx_data_mode : 2; //数据传输模式，0-i2s模式，1-左对齐，2-右对齐
} i2srx_setting_t;

#endif

