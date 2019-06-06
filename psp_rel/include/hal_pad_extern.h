#ifndef __HAL_PAD_EXTERN_H__
#define __HAL_PAD_EXTERN_H__

#include <attr.h>
#include <typeext.h>

typedef enum
{
    MPAD_EJTAG4, //EJTAG4 port
    MPAD_BTDEBUG, //BT DEBUG
    PAD_GPIO0,
    PAD_GPIO1,
    PAD_GPIO2,
    PAD_GPIO3,
    PAD_GPIO4,
    PAD_GPIO5,
    PAD_GPIO6,
    PAD_GPIO7,
    PAD_GPIO8,
    PAD_GPIO9,
    PAD_GPIO10,
    PAD_GPIO11,
    PAD_GPIO12,
    PAD_GPIO13,
    PAD_GPIO14,
    PAD_GPIO15,
    PAD_GPIO16,
    PAD_GPIO17,
    PAD_GPIO18,
    PAD_GPIO19,
    PAD_GPIO20,
    PAD_GPIO21,
    PAD_GPIO22,
    PAD_GPIO23,
    PAD_GPIO24,
    PAD_GPIO25,
    PAD_GPIO26,
    PAD_GPIO27,
    PAD_GPIO28,
    PAD_GPIO29,
    PAD_GPIO30,
    PAD_GPIO31,
    PAD_GPIO32,
    PAD_GPIO33,
    PAD_GPIO34,
    PAD_GPIO35,
    PAD_GPIO36,
    PAD_GPIO37,
    PAD_VRO,   //GPIO38
    PAD_VRO_S, //GPIO39
    PAD_AUX0L, //GPIO40
    PAD_AUX0R, //GPIO41
    PAD_AOUTL, //GPIO42
    PAD_AOUTR, //GPIO43
    PAD_AUX1L, //GPIO44
    PAD_AUX1R, //GPIO45
    PAD_MIC0,  //GPIO46
    PAD_MIC1,  //GPIO47
    PAD_GPIO48,
    PAD_GPIO49,
    PAD_GPIO50,
    PAD_GPIO51,
    PAD_WIO0,  //SIO0
    PAD_WIO1,  //SIO1
    PAD_ID_MAX,
} ic_pad_e;

#define PAD_GPIO_ID_MAX (PAD_GPIO51+1)

typedef enum
{
    PAD_FUNC_DEFAULT = 0,
    PAD_FUNC_ANOLOG = 1,
    PAD_FUNC_EJTAG = 2,
    PAD_FUNC_DEBUG = 3,
    PAD_FUNC_GPIOOUT = 4,
    PAD_FUNC_GPIOIN = 5,
    PAD_FUNC_SIOOUT = 6,
    PAD_FUNC_SIOIN = 7,
    PAD_FUNC_MFP = 8,
} pad_function_e;

typedef enum
{
    PAD_OP_SET,
    PAD_OP_IOCTL,
} pad_op_e;

typedef struct
{
    bool pad_pu_en; //0表示不上拉，1表示100K/50K上拉，2表示10K强上拉
    bool pad_pd_en;
    bool smit_en;
    uint8 pad_driver;
} pad_phy_para_t;

typedef enum
{
    PWM_LOW_ACTIVE = 0,
    PWM_HIGH_ACTIVE = 1,
} pwm_polarity_e;

typedef enum
{
    PWM_MODE_NORMAL = 0,
    PWM_MODE_BREATH = 1,
} pwm_mode_e;

typedef enum
{
    PWM_PERIOD_200MS = 0,
    PWM_PERIOD_400MS = 1,
    PWM_PERIOD_1000MS = 2,
    PWM_PERIOD_2000MS = 3,
} pwm_period_e;

typedef struct
{
    pwm_mode_e mode;
    uint8 breath_div; //active only breath mode
    uint8 Q_value; //0 ~ 3
    uint8 H_time;
    uint8 L_time;
    pwm_polarity_e polarity; //active only normal mode
    uint8 duty; //active only normal mode
    pwm_period_e period;
} pwm_para_t;

typedef enum {
    pwm_duty_grade_20000 = 20000,
    pwm_duty_grade_10000 = 10000,
    pwm_duty_grade_4000 = 4000,
    pwm_duty_grade_2000 = 2000,
    pwm_duty_grade_1000 = 1000,
    pwm_duty_grade_400 = 400,
    pwm_duty_grade_200 = 200,
    pwm_duty_grade_125 = 125,
    pwm_duty_grade_100 = 100,
    pwm_duty_grade_80  = 80,
    pwm_duty_grade_50  = 50,
    pwm_duty_grade_40  = 40,
    pwm_duty_grade_25  = 25,
    pwm_duty_grade_16  = 16,
    pwm_duty_grade_10  = 10,
} sim_pwm_duty_grade_e;

typedef struct {
    ic_pad_e IC_pad;
    uint16 raise_time;   // 以250MS为单位设置
    uint16 decline_time; // 以250MS为单位设置
    uint16 H_time;      // 以100us为单位设置
    uint16 L_time;      // 以100us为单位设置
    /*
        0表示初始状态为高占空比，即 下降->低->上升->高为一个周期
        1表示初始状态为低占空比，即 上升->高->下降->低为一个周期
        设立两种呼吸模式，目的是为了满足两个led灯交替呼吸的需求。
    */
    uint8 breath_mode;
    int16 breath_count;  // -1表示一直呼吸
} sim_pwm_bre_info_t;

/*
+------------------------+
|  频率     |  dutyGrade |
+------------------------+
|  f = 0.5   |  dg = 20000|
|  f = 1    |  dg = 10000|
|  f = 2.5   |  dg = 4000 |
|  f = 5    |  dg = 2000 |
|  f = 10    |  dg = 1000 |
|  f = 25   |  dg = 400  |
|  f = 50   |  dg = 200  |
|  f = 80   |  dg = 125  |
|  f = 100  |  dg = 100  |
|  f = 125  |  dg = 80   |
|  f = 200  |  dg = 50   |
|  f = 250  |  dg = 40   |
|  f = 400  |  dg = 25   |
|  f = 500  |  dg = 20   |
|  f = 625  |  dg = 16   |
|  f = 1000 |  dg = 10   |
+------------------------+
*/
typedef struct {
    ic_pad_e IC_pad;
    sim_pwm_duty_grade_e duty_grade;
    uint16 duty;
} sim_pwm_nor_info_t;

typedef union {
    sim_pwm_nor_info_t pwm_nor_info;
    sim_pwm_bre_info_t pwm_bre_info;
} sim_pwm_info_sub_t;

typedef struct {
    sim_pwm_info_sub_t pwm_info_sub;
    pwm_mode_e work_mode;
} sim_pwm_info_t;

typedef struct {
    bool used_flag;
    int8 sim_pwm_id;
} sim_pwm_used_flag_t;


//sirq
typedef void (*interrupt_handle_cb)(uint8 triggle_type);

typedef struct
{
    interrupt_handle_cb handle;
    uint8 triggle;
    uint8 default_edge;
    uint8 realtime_mode; //实时模式，0表示对实时性要求不高,一般是可以满足要求的，比如SIRQ检测旋转按钮；
                         //          1表示实时性要求极高，会把中断响应提升为最高，会对蓝牙中断造成影响，有潜在的风险
} sirq_set_para_t;

typedef enum
{
    SIRQ_TRIGGLE_RISINGEDGE,  //0 上升沿触发
    SIRQ_TRIGGLE_FALLINGEDGE, //1 下降沿触发
    SIRQ_TRIGGLE_ALLEDGE,     //2 上升沿或下降沿触发
    SIRQ_TRIGGLE_HIGH_LEVEL,  //3 高电平触发
    SIRQ_TRIGGLE_LOW_LEVEL,   //4 低电平触发
    SIRQ_TRIGGLE_MAX,
} sirq_triggle_e;

#define GPIO8_ADCKEY_PU_DISABLE  0 //ADC KEY 上拉电阻禁用
#define GPIO8_ADCKEY_PU_ENABLE   1 //ADC KEY 上拉电阻启用


//UART CLK SEL
#define UART_CLK_AHB             0
#define UART_CLK_DMA             1

//FM CLK 内部时钟源
#define FMCLKSEL_32K             0
#define FMCLKSEL_HOSC26M         1
#define FMCLKSEL_CORED10         2
#define FMCLKSEL_CK24M           3

//GPIO0 MFP
#define GPIO0_MFP_SD0_CMD        (PAD_FUNC_MFP + 0)
#define GPIO0_MFP_GPIO0          (PAD_FUNC_MFP + 1)
#define GPIO0_MFP_UART0_CTS      (PAD_FUNC_MFP + 2)
#define GPIO0_MFP_FMCLKOUT       (PAD_FUNC_MFP + 3)
#define GPIO0_MFP_SPI1_SS        (PAD_FUNC_MFP + 4)
#define GPIO0_MFP_PWM0           (PAD_FUNC_MFP + 5)
#define GPIO0_MFP_I2STX_BCLK     (PAD_FUNC_MFP + 6)
#define GPIO0_MFP_I2SRX_BCLK     (PAD_FUNC_MFP + 7)

//GPIO1 MFP
#define GPIO1_MFP_SD0_CLK        (PAD_FUNC_MFP + 0)
#define GPIO1_MFP_GPIO1          (PAD_FUNC_MFP + 1)
#define GPIO1_MFP_UART0_RTS      (PAD_FUNC_MFP + 2)
#define GPIO1_MFP_UART0_RX       (PAD_FUNC_MFP + 3)
#define GPIO1_MFP_SPI1_CLK       (PAD_FUNC_MFP + 4)
#define GPIO1_MFP_PWM1           (PAD_FUNC_MFP + 5)
#define GPIO1_MFP_I2STX_LRCLK    (PAD_FUNC_MFP + 6)
#define GPIO1_MFP_I2SRX_LRCLK    (PAD_FUNC_MFP + 7)

//GPIO2 MFP
#define GPIO2_MFP_SD0_DAT        (PAD_FUNC_MFP + 0)
#define GPIO2_MFP_GPIO2          (PAD_FUNC_MFP + 1)
#define GPIO2_MFP_UART0_TX       (PAD_FUNC_MFP + 2)
#define GPIO2_MFP_I2C0_SDA       (PAD_FUNC_MFP + 3)
#define GPIO2_MFP_SPI1_MOSI      (PAD_FUNC_MFP + 4)
#define GPIO2_MFP_IR_RX          (PAD_FUNC_MFP + 5)
#define GPIO2_MFP_I2STX_MCLK     (PAD_FUNC_MFP + 6)
#define GPIO2_MFP_I2SRX_MCLK     (PAD_FUNC_MFP + 7)

//GPIO3 MFP
#define GPIO3_MFP_I2C0_SCL       (PAD_FUNC_MFP + 0)
#define GPIO3_MFP_GPIO3          (PAD_FUNC_MFP + 1)
#define GPIO3_MFP_UART0_CTS      (PAD_FUNC_MFP + 2)
#define GPIO3_MFP_PWM2           (PAD_FUNC_MFP + 3)
#define GPIO3_MFP_SPI1_MISO      (PAD_FUNC_MFP + 4)
#define GPIO3_MFP_SPDIF_RX       (PAD_FUNC_MFP + 5)
#define GPIO3_MFP_I2STX_DOUT     (PAD_FUNC_MFP + 6)
#define GPIO3_MFP_I2SRX_DI       (PAD_FUNC_MFP + 7)
#define GPIO3_MFP_LCD_SEG16      (PAD_FUNC_MFP + 8)
#define GPIO3_MFP_SPI0_IO2       (PAD_FUNC_MFP + 9)

//GPIO4 MFP
#define GPIO4_MFP_FMCLKOUT       (PAD_FUNC_MFP + 0)
#define GPIO4_MFP_GPIO4          (PAD_FUNC_MFP + 1)
#define GPIO4_MFP_UART0_RTS      (PAD_FUNC_MFP + 2)
#define GPIO4_MFP_PWM3           (PAD_FUNC_MFP + 3)
#define GPIO4_MFP_SPDIF_RX       (PAD_FUNC_MFP + 4)
#define GPIO4_MFP_UART0_TX       (PAD_FUNC_MFP + 5)
#define GPIO4_MFP_IR_RX          (PAD_FUNC_MFP + 6)
#define GPIO4_MFP_TIMER2_CAPIN   (PAD_FUNC_MFP + 7)
#define GPIO4_MFP_LCD_SEG17      (PAD_FUNC_MFP + 8)
#define GPIO4_MFP_CK802_TRST     (PAD_FUNC_MFP + 9)
#define GPIO4_MFP_SPI0_IO3       (PAD_FUNC_MFP + 10)

//GPIO5 MFP
#define GPIO5_MFP_UART0_RX       (PAD_FUNC_MFP + 0)
#define GPIO5_MFP_GPIO5          (PAD_FUNC_MFP + 1)
#define GPIO5_MFP_CK802_TCK      (PAD_FUNC_MFP + 2)
#define GPIO5_MFP_I2C0_SDA       (PAD_FUNC_MFP + 3)
#define GPIO5_MFP_IR_RX          (PAD_FUNC_MFP + 4)
#define GPIO5_MFP_PWM4           (PAD_FUNC_MFP + 5)
#define GPIO5_MFP_I2STX_BCLK     (PAD_FUNC_MFP + 6)
#define GPIO5_MFP_I2C1_SDA       (PAD_FUNC_MFP + 7)
#define GPIO5_MFP_LCD_SEG20      (PAD_FUNC_MFP + 8)
#define GPIO5_MFP_SPI0_SS        (PAD_FUNC_MFP + 9)

//GPIO6 MFP
#define GPIO6_MFP_UART0_TX       (PAD_FUNC_MFP + 0)
#define GPIO6_MFP_GPIO6          (PAD_FUNC_MFP + 1)
#define GPIO6_MFP_CK802_TMS      (PAD_FUNC_MFP + 2)
#define GPIO6_MFP_I2C0_SCL       (PAD_FUNC_MFP + 3)
#define GPIO6_MFP_SPDIF_RX       (PAD_FUNC_MFP + 4)
#define GPIO6_MFP_PWM5           (PAD_FUNC_MFP + 5)
#define GPIO6_MFP_I2STX_LRCLK    (PAD_FUNC_MFP + 6)
#define GPIO6_MFP_I2C1_SCL       (PAD_FUNC_MFP + 7)
#define GPIO6_MFP_LCD_SEG21      (PAD_FUNC_MFP + 8)
#define GPIO6_MFP_SPI0_CLK       (PAD_FUNC_MFP + 9)

//GPIO8 MFP
#define GPIO8_MFP_UART1_TX       (PAD_FUNC_MFP + 0)
#define GPIO8_MFP_GPIO8          (PAD_FUNC_MFP + 1)
#define GPIO8_MFP_I2STX_MCLK     (PAD_FUNC_MFP + 2)
#define GPIO8_MFP_I2C0_SDA       (PAD_FUNC_MFP + 3)
#define GPIO8_MFP_SPI1_MISO      (PAD_FUNC_MFP + 4)
#define GPIO8_MFP_PWM6           (PAD_FUNC_MFP + 5)
#define GPIO8_MFP_LCD_SEG15      (PAD_FUNC_MFP + 6)
#define GPIO8_MFP_DMIC_CK        (PAD_FUNC_MFP + 7)
#define GPIO8_MFP_LCD_SEG18      (PAD_FUNC_MFP + 8)
#define GPIO8_MFP_SPI0_MOSI      (PAD_FUNC_MFP + 9)

//GPIO9 MFP
#define GPIO9_MFP_UART1_RX       (PAD_FUNC_MFP + 0)
#define GPIO9_MFP_GPIO9          (PAD_FUNC_MFP + 1)
#define GPIO9_MFP_I2STX_DOUT     (PAD_FUNC_MFP + 2)
#define GPIO9_MFP_I2C0_SCL       (PAD_FUNC_MFP + 3)
#define GPIO9_MFP_SPI1_MOSI      (PAD_FUNC_MFP + 4)
#define GPIO9_MFP_PWM7           (PAD_FUNC_MFP + 5)
#define GPIO9_MFP_SPI0_MISO      (PAD_FUNC_MFP + 6)
#define GPIO9_MFP_DMIC_DAT       (PAD_FUNC_MFP + 7)
#define GPIO9_MFP_LCD_SEG19      (PAD_FUNC_MFP + 8)

//GPIO10 MFP
#define GPIO10_MFP_DM            (PAD_FUNC_MFP + 0)
#define GPIO10_MFP_GPIO10        (PAD_FUNC_MFP + 1)
#define GPIO10_MFP_UART1_CTS     (PAD_FUNC_MFP + 2)
#define GPIO10_MFP_UART0_CTS     (PAD_FUNC_MFP + 3)
#define GPIO10_MFP_I2SRX_DI      (PAD_FUNC_MFP + 4)
#define GPIO10_MFP_PWM8          (PAD_FUNC_MFP + 5)

//GPIO11 MFP
#define GPIO11_MFP_DP            (PAD_FUNC_MFP + 0)
#define GPIO11_MFP_GPIO11        (PAD_FUNC_MFP + 1)
#define GPIO11_MFP_UART1_RTS     (PAD_FUNC_MFP + 2)
#define GPIO11_MFP_UART0_RTS     (PAD_FUNC_MFP + 3)
#define GPIO11_MFP_I2SRX_MCLK    (PAD_FUNC_MFP + 4)
#define GPIO11_MFP_PWM0          (PAD_FUNC_MFP + 5)

//GPIO12 MFP
#define GPIO12_MFP_SPI0_IO2      (PAD_FUNC_MFP + 0)
#define GPIO12_MFP_GPIO12        (PAD_FUNC_MFP + 1)
#define GPIO12_MFP_UART0_TX      (PAD_FUNC_MFP + 2)
#define GPIO12_MFP_I2C1_SDA      (PAD_FUNC_MFP + 3)
#define GPIO12_MFP_SPI1_SS       (PAD_FUNC_MFP + 4)
#define GPIO12_MFP_PWM8          (PAD_FUNC_MFP + 5)
#define GPIO12_MFP_I2SRX_BCLK    (PAD_FUNC_MFP + 6)
#define GPIO12_MFP_UART1_CTS     (PAD_FUNC_MFP + 7)
#define GPIO12_MFP_TIMER3_CAPIN  (PAD_FUNC_MFP + 8)

//GPIO13 MFP
#define GPIO13_MFP_SPI0_IO3      (PAD_FUNC_MFP + 0)
#define GPIO13_MFP_GPIO13        (PAD_FUNC_MFP + 1)
#define GPIO13_MFP_UART0_RX      (PAD_FUNC_MFP + 2)
#define GPIO13_MFP_I2C1_SCL      (PAD_FUNC_MFP + 3)
#define GPIO13_MFP_SPI1_CLK      (PAD_FUNC_MFP + 4)
#define GPIO13_MFP_IR_RX         (PAD_FUNC_MFP + 5)
#define GPIO13_MFP_I2SRX_LRCLK   (PAD_FUNC_MFP + 6)
#define GPIO13_MFP_UART1_RTS     (PAD_FUNC_MFP + 7)
#define GPIO13_MFP_PWM0          (PAD_FUNC_MFP + 8)
#define GPIO13_MFP_LED_SEG7      (PAD_FUNC_MFP + 9)

//GPIO14 MFP
#define GPIO14_MFP_LCD_COM0      (PAD_FUNC_MFP + 0)
#define GPIO14_MFP_GPIO14        (PAD_FUNC_MFP + 1)
#define GPIO14_MFP_LED_COM0      (PAD_FUNC_MFP + 2)
#define GPIO14_MFP_LCD_WRB       (PAD_FUNC_MFP + 3)
#define GPIO14_MFP_EM_WRB        (PAD_FUNC_MFP + 4)
#define GPIO14_MFP_I2STX_BCLK    (PAD_FUNC_MFP + 5)
#define GPIO14_MFP_UART1_TX      (PAD_FUNC_MFP + 6)
#define GPIO14_MFP_SPI1_SS       (PAD_FUNC_MFP + 7)
#define GPIO14_MFP_I2SRX_BCLK    (PAD_FUNC_MFP + 8)

//GPIO15 MFP
#define GPIO15_MFP_LCD_COM1      (PAD_FUNC_MFP + 0)
#define GPIO15_MFP_GPIO15        (PAD_FUNC_MFP + 1)
#define GPIO15_MFP_LED_COM1      (PAD_FUNC_MFP + 2)
#define GPIO15_MFP_LCD_RS        (PAD_FUNC_MFP + 3)
#define GPIO15_MFP_EM_RS         (PAD_FUNC_MFP + 4)
#define GPIO15_MFP_I2STX_LRCLK   (PAD_FUNC_MFP + 5)
#define GPIO15_MFP_UART1_RX      (PAD_FUNC_MFP + 6)
#define GPIO15_MFP_SPI1_CLK      (PAD_FUNC_MFP + 7)
#define GPIO15_MFP_I2SRX_LRCLK   (PAD_FUNC_MFP + 8)

//GPIO16 MFP
#define GPIO16_MFP_LCD_COM2      (PAD_FUNC_MFP + 0)
#define GPIO16_MFP_GPIO16        (PAD_FUNC_MFP + 1)
#define GPIO16_MFP_LED_COM2      (PAD_FUNC_MFP + 2)
#define GPIO16_MFP_LCD_RDB       (PAD_FUNC_MFP + 3)
#define GPIO16_MFP_EM_RDB        (PAD_FUNC_MFP + 4)
#define GPIO16_MFP_I2STX_MCLK    (PAD_FUNC_MFP + 5)
#define GPIO16_MFP_UART1_CTS     (PAD_FUNC_MFP + 6)
#define GPIO16_MFP_SPI1_MOSI     (PAD_FUNC_MFP + 7)
#define GPIO16_MFP_I2SRX_DI      (PAD_FUNC_MFP + 8)

//GPIO17 MFP
#define GPIO17_MFP_LCD_COM3      (PAD_FUNC_MFP + 0)
#define GPIO17_MFP_GPIO17        (PAD_FUNC_MFP + 1)
#define GPIO17_MFP_LED_COM3      (PAD_FUNC_MFP + 2)
#define GPIO17_MFP_LCD_CEB       (PAD_FUNC_MFP + 3)
#define GPIO17_MFP_EM_CEB0       (PAD_FUNC_MFP + 4)
#define GPIO17_MFP_I2STX_DOUT    (PAD_FUNC_MFP + 5)
#define GPIO17_MFP_UART1_RTS     (PAD_FUNC_MFP + 6)
#define GPIO17_MFP_SPI1_MISO     (PAD_FUNC_MFP + 7)
#define GPIO17_MFP_I2SRX_MCLK    (PAD_FUNC_MFP + 8)
#define GPIO17_MFP_CK802_TRST    (PAD_FUNC_MFP + 9)

//GPIO18 MFP
#define GPIO18_MFP_LCD_COM4      (PAD_FUNC_MFP + 0)
#define GPIO18_MFP_GPIO18        (PAD_FUNC_MFP + 1)
#define GPIO18_MFP_LCD_SEG22     (PAD_FUNC_MFP + 2)
#define GPIO18_MFP_SPDIF_RX      (PAD_FUNC_MFP + 3)
#define GPIO18_MFP_EM_CEB1       (PAD_FUNC_MFP + 4)
#define GPIO18_MFP_FMCLKOUT      (PAD_FUNC_MFP + 5)
#define GPIO18_MFP_PWM0          (PAD_FUNC_MFP + 6)
#define GPIO18_MFP_SD1_CMD       (PAD_FUNC_MFP + 7)
#define GPIO18_MFP_CK802_TCK     (PAD_FUNC_MFP + 8)

//GPIO19 MFP
#define GPIO19_MFP_LCD_COM5      (PAD_FUNC_MFP + 0)
#define GPIO19_MFP_GPIO19        (PAD_FUNC_MFP + 1)
#define GPIO19_MFP_LCD_SEG23     (PAD_FUNC_MFP + 2)
#define GPIO19_MFP_IR_RX         (PAD_FUNC_MFP + 3)
#define GPIO19_MFP_EM_CEB2       (PAD_FUNC_MFP + 4)
#define GPIO19_MFP_PWM8          (PAD_FUNC_MFP + 5)
#define GPIO19_MFP_TIMER3_CAPIN  (PAD_FUNC_MFP + 6)
#define GPIO19_MFP_SD1_CLK       (PAD_FUNC_MFP + 7)
#define GPIO19_MFP_CK802_TMS     (PAD_FUNC_MFP + 8)

//GPIO20 MFP
#define GPIO20_MFP_LCD_SEG0      (PAD_FUNC_MFP + 0)
#define GPIO20_MFP_GPIO20        (PAD_FUNC_MFP + 1)
#define GPIO20_MFP_LED_SEG0      (PAD_FUNC_MFP + 2)
#define GPIO20_MFP_LCD_D0        (PAD_FUNC_MFP + 3)
#define GPIO20_MFP_EM_D0         (PAD_FUNC_MFP + 4)
#define GPIO20_MFP_PWM1          (PAD_FUNC_MFP + 5)
#define GPIO20_MFP_I2STX_BCLK    (PAD_FUNC_MFP + 6)
#define GPIO20_MFP_SD1_CMD       (PAD_FUNC_MFP + 7)
#define GPIO20_MFP_SPI1_SS       (PAD_FUNC_MFP + 8)

//GPIO21 MFP
#define GPIO21_MFP_LCD_SEG1      (PAD_FUNC_MFP + 0)
#define GPIO21_MFP_GPIO21        (PAD_FUNC_MFP + 1)
#define GPIO21_MFP_LED_SEG1      (PAD_FUNC_MFP + 2)
#define GPIO21_MFP_LCD_D1        (PAD_FUNC_MFP + 3)
#define GPIO21_MFP_EM_D1         (PAD_FUNC_MFP + 4)
#define GPIO21_MFP_PWM2          (PAD_FUNC_MFP + 5)
#define GPIO21_MFP_I2STX_LRCLK   (PAD_FUNC_MFP + 6)
#define GPIO21_MFP_SD1_CLK       (PAD_FUNC_MFP + 7)
#define GPIO21_MFP_SPI1_CLK      (PAD_FUNC_MFP + 8)

//GPIO22 MFP
#define GPIO22_MFP_LCD_SEG2      (PAD_FUNC_MFP + 0)
#define GPIO22_MFP_GPIO22        (PAD_FUNC_MFP + 1)
#define GPIO22_MFP_LED_SEG2      (PAD_FUNC_MFP + 2)
#define GPIO22_MFP_LCD_D2        (PAD_FUNC_MFP + 3)
#define GPIO22_MFP_EM_D2         (PAD_FUNC_MFP + 4)
#define GPIO22_MFP_PWM3          (PAD_FUNC_MFP + 5)
#define GPIO22_MFP_I2STX_MCLK    (PAD_FUNC_MFP + 6)
#define GPIO22_MFP_SD1_DAT0      (PAD_FUNC_MFP + 7)
#define GPIO22_MFP_SPI1_MOSI     (PAD_FUNC_MFP + 8)

//GPIO23 MFP
#define GPIO23_MFP_LCD_SEG3      (PAD_FUNC_MFP + 0)
#define GPIO23_MFP_GPIO23        (PAD_FUNC_MFP + 1)
#define GPIO23_MFP_LED_SEG3      (PAD_FUNC_MFP + 2)
#define GPIO23_MFP_LCD_D3        (PAD_FUNC_MFP + 3)
#define GPIO23_MFP_EM_D3         (PAD_FUNC_MFP + 4)
#define GPIO23_MFP_PWM4          (PAD_FUNC_MFP + 5)
#define GPIO23_MFP_I2STX_DOUT    (PAD_FUNC_MFP + 6)
#define GPIO23_MFP_SD1_DAT1      (PAD_FUNC_MFP + 7)
#define GPIO23_MFP_SPI1_MISO     (PAD_FUNC_MFP + 8)

//GPIO24 MFP
#define GPIO24_MFP_LCD_SEG4      (PAD_FUNC_MFP + 0)
#define GPIO24_MFP_GPIO24        (PAD_FUNC_MFP + 1)
#define GPIO24_MFP_LED_SEG4      (PAD_FUNC_MFP + 2)
#define GPIO24_MFP_LCD_D4        (PAD_FUNC_MFP + 3)
#define GPIO24_MFP_EM_D4         (PAD_FUNC_MFP + 4)
#define GPIO24_MFP_PWM5          (PAD_FUNC_MFP + 5)
#define GPIO24_MFP_I2SRX_DI      (PAD_FUNC_MFP + 6)
#define GPIO24_MFP_SD1_DAT2      (PAD_FUNC_MFP + 7)
#define GPIO24_MFP_UART0_TX      (PAD_FUNC_MFP + 8)

//GPIO25 MFP
#define GPIO25_MFP_LCD_SEG5      (PAD_FUNC_MFP + 0)
#define GPIO25_MFP_GPIO25        (PAD_FUNC_MFP + 1)
#define GPIO25_MFP_LED_SEG5      (PAD_FUNC_MFP + 2)
#define GPIO25_MFP_LCD_D5        (PAD_FUNC_MFP + 3)
#define GPIO25_MFP_EM_D5         (PAD_FUNC_MFP + 4)
#define GPIO25_MFP_PWM6          (PAD_FUNC_MFP + 5)
#define GPIO25_MFP_I2SRX_MCLK    (PAD_FUNC_MFP + 6)
#define GPIO25_MFP_SD1_DAT3      (PAD_FUNC_MFP + 7)
#define GPIO25_MFP_UART0_RX      (PAD_FUNC_MFP + 8)

//GPIO26 MFP
#define GPIO26_MFP_LCD_SEG6      (PAD_FUNC_MFP + 0)
#define GPIO26_MFP_GPIO26        (PAD_FUNC_MFP + 1)
#define GPIO26_MFP_LED_SEG6      (PAD_FUNC_MFP + 2)
#define GPIO26_MFP_LCD_D6        (PAD_FUNC_MFP + 3)
#define GPIO26_MFP_EM_D6         (PAD_FUNC_MFP + 4)
#define GPIO26_MFP_PWM7          (PAD_FUNC_MFP + 5)
#define GPIO26_MFP_SPDIF_RX      (PAD_FUNC_MFP + 6)
#define GPIO26_MFP_IR_RX         (PAD_FUNC_MFP + 7)
#define GPIO26_MFP_UART1_TX      (PAD_FUNC_MFP + 8)

//GPIO27 MFP
#define GPIO27_MFP_LCD_SEG7      (PAD_FUNC_MFP + 0)
#define GPIO27_MFP_GPIO27        (PAD_FUNC_MFP + 1)
#define GPIO27_MFP_LED_SEG7      (PAD_FUNC_MFP + 2)
#define GPIO27_MFP_LCD_D7        (PAD_FUNC_MFP + 3)
#define GPIO27_MFP_EM_D7         (PAD_FUNC_MFP + 4)
#define GPIO27_MFP_IR_RX         (PAD_FUNC_MFP + 5)
#define GPIO27_MFP_SPDIF_RX      (PAD_FUNC_MFP + 6)
#define GPIO27_MFP_FMCLKOUT      (PAD_FUNC_MFP + 7)
#define GPIO27_MFP_UART1_RX      (PAD_FUNC_MFP + 8)

//GPIO28 MFP
#define GPIO28_MFP_LCD_SEG8      (PAD_FUNC_MFP + 0)
#define GPIO28_MFP_GPIO28        (PAD_FUNC_MFP + 1)
#define GPIO28_MFP_LED_COM4      (PAD_FUNC_MFP + 2)
#define GPIO28_MFP_LCD_D8        (PAD_FUNC_MFP + 3)
#define GPIO28_MFP_EM_D8         (PAD_FUNC_MFP + 4)
#define GPIO28_MFP_I2C1_SDA      (PAD_FUNC_MFP + 5)
#define GPIO28_MFP_I2SRX_DI      (PAD_FUNC_MFP + 6)
#define GPIO28_MFP_UART0_CTS     (PAD_FUNC_MFP + 7)
#define GPIO28_MFP_SD1_DAT0      (PAD_FUNC_MFP + 8)

//GPIO29 MFP
#define GPIO29_MFP_LCD_SEG9      (PAD_FUNC_MFP + 0)
#define GPIO29_MFP_GPIO29        (PAD_FUNC_MFP + 1)
#define GPIO29_MFP_LED_COM5      (PAD_FUNC_MFP + 2)
#define GPIO29_MFP_LCD_D9        (PAD_FUNC_MFP + 3)
#define GPIO29_MFP_EM_D9         (PAD_FUNC_MFP + 4)
#define GPIO29_MFP_I2C1_SCL      (PAD_FUNC_MFP + 5)
#define GPIO29_MFP_I2SRX_MCLK    (PAD_FUNC_MFP + 6)
#define GPIO29_MFP_UART0_RTS     (PAD_FUNC_MFP + 7)
#define GPIO29_MFP_SD1_DAT1      (PAD_FUNC_MFP + 8)

//GPIO30 MFP
#define GPIO30_MFP_LCD_SEG10     (PAD_FUNC_MFP + 0)
#define GPIO30_MFP_GPIO30        (PAD_FUNC_MFP + 1)
#define GPIO30_MFP_LED_COM6      (PAD_FUNC_MFP + 2)
#define GPIO30_MFP_LCD_D10       (PAD_FUNC_MFP + 3)
#define GPIO30_MFP_EM_D10        (PAD_FUNC_MFP + 4)
#define GPIO30_MFP_I2C0_SDA      (PAD_FUNC_MFP + 5)
#define GPIO30_MFP_I2STX_BCLK    (PAD_FUNC_MFP + 6)
#define GPIO30_MFP_UART0_RX      (PAD_FUNC_MFP + 7)
#define GPIO30_MFP_SD1_DAT2      (PAD_FUNC_MFP + 8)

//GPIO31 MFP
#define GPIO31_MFP_LCD_SEG11     (PAD_FUNC_MFP + 0)
#define GPIO31_MFP_GPIO31        (PAD_FUNC_MFP + 1)
#define GPIO31_MFP_LED_COM7      (PAD_FUNC_MFP + 2)
#define GPIO31_MFP_LCD_D11       (PAD_FUNC_MFP + 3)
#define GPIO31_MFP_EM_D11        (PAD_FUNC_MFP + 4)
#define GPIO31_MFP_I2C0_SCL      (PAD_FUNC_MFP + 5)
#define GPIO31_MFP_I2STX_LRCLK   (PAD_FUNC_MFP + 6)
#define GPIO31_MFP_UART0_TX      (PAD_FUNC_MFP + 7)
#define GPIO31_MFP_SD1_DAT3      (PAD_FUNC_MFP + 8)

//GPIO32 MFP
#define GPIO32_MFP_LCD_SEG12     (PAD_FUNC_MFP + 0)
#define GPIO32_MFP_GPIO32        (PAD_FUNC_MFP + 1)
#define GPIO32_MFP_SPI1_SS       (PAD_FUNC_MFP + 2)
#define GPIO32_MFP_LCD_D12       (PAD_FUNC_MFP + 3)
#define GPIO32_MFP_EM_D12        (PAD_FUNC_MFP + 4)
#define GPIO32_MFP_PWM0          (PAD_FUNC_MFP + 5)
#define GPIO32_MFP_I2STX_MCLK    (PAD_FUNC_MFP + 6)
#define GPIO32_MFP_FMCLKOUT      (PAD_FUNC_MFP + 7)
#define GPIO32_MFP_DMIC_CK       (PAD_FUNC_MFP + 8)

//GPIO33 MFP
#define GPIO33_MFP_LCD_SEG13     (PAD_FUNC_MFP + 0)
#define GPIO33_MFP_GPIO33        (PAD_FUNC_MFP + 1)
#define GPIO33_MFP_SPI1_CLK      (PAD_FUNC_MFP + 2)
#define GPIO33_MFP_LCD_D13       (PAD_FUNC_MFP + 3)
#define GPIO33_MFP_EM_D13        (PAD_FUNC_MFP + 4)
#define GPIO33_MFP_PWM1          (PAD_FUNC_MFP + 5)
#define GPIO33_MFP_I2STX_DOUT    (PAD_FUNC_MFP + 6)
#define GPIO33_MFP_UART1_CTS     (PAD_FUNC_MFP + 7)
#define GPIO33_MFP_DMIC_DAT      (PAD_FUNC_MFP + 8)

//GPIO34 MFP
#define GPIO34_MFP_LCD_SEG14     (PAD_FUNC_MFP + 0)
#define GPIO34_MFP_GPIO34        (PAD_FUNC_MFP + 1)
#define GPIO34_MFP_SPI1_MOSI     (PAD_FUNC_MFP + 2)
#define GPIO34_MFP_LCD_D14       (PAD_FUNC_MFP + 3)
#define GPIO34_MFP_EM_D14        (PAD_FUNC_MFP + 4)
#define GPIO34_MFP_PWM2          (PAD_FUNC_MFP + 5)
#define GPIO34_MFP_IR_RX         (PAD_FUNC_MFP + 6)
#define GPIO34_MFP_SPDIF_RX      (PAD_FUNC_MFP + 7)
#define GPIO34_MFP_FMCLKOUT      (PAD_FUNC_MFP + 8)

//GPIO35 MFP
#define GPIO35_MFP_LCD_SEG15     (PAD_FUNC_MFP + 0)
#define GPIO35_MFP_GPIO35        (PAD_FUNC_MFP + 1)
#define GPIO35_MFP_SPI1_MISO     (PAD_FUNC_MFP + 2)
#define GPIO35_MFP_LCD_D15       (PAD_FUNC_MFP + 3)
#define GPIO35_MFP_EM_D15        (PAD_FUNC_MFP + 4)
#define GPIO35_MFP_PWM3          (PAD_FUNC_MFP + 5)
#define GPIO35_MFP_UART1_RTS     (PAD_FUNC_MFP + 6)
#define GPIO35_MFP_TIMER2_CAPIN  (PAD_FUNC_MFP + 7)

//GPIO36 MFP
#define GPIO36_MFP_LCD_SEG16     (PAD_FUNC_MFP + 0)
#define GPIO36_MFP_GPIO36        (PAD_FUNC_MFP + 1)
#define GPIO36_MFP_I2STX_BCLK    (PAD_FUNC_MFP + 2)
#define GPIO36_MFP_SPI1_SS       (PAD_FUNC_MFP + 3)
#define GPIO36_MFP_EM_CEB1       (PAD_FUNC_MFP + 4)
#define GPIO36_MFP_PWM4          (PAD_FUNC_MFP + 5)
#define GPIO36_MFP_I2SRX_BCLK    (PAD_FUNC_MFP + 6)
#define GPIO36_MFP_I2C0_SCL      (PAD_FUNC_MFP + 7)
#define GPIO36_MFP_UART1_TX      (PAD_FUNC_MFP + 8)

//GPIO37 MFP
#define GPIO37_MFP_LCD_SEG17     (PAD_FUNC_MFP + 0)
#define GPIO37_MFP_GPIO37        (PAD_FUNC_MFP + 1)
#define GPIO37_MFP_I2STX_LRCLK   (PAD_FUNC_MFP + 2)
#define GPIO37_MFP_SPI1_CLK      (PAD_FUNC_MFP + 3)
#define GPIO37_MFP_EM_CEB2       (PAD_FUNC_MFP + 4)
#define GPIO37_MFP_PWM5          (PAD_FUNC_MFP + 5)
#define GPIO37_MFP_I2SRX_LRCLK   (PAD_FUNC_MFP + 6)
#define GPIO37_MFP_I2C0_SDA      (PAD_FUNC_MFP + 7)
#define GPIO37_MFP_UART1_RX      (PAD_FUNC_MFP + 8)

//GPIO48 MFP
#define GPIO48_MFP_SPI0_MISO     (PAD_FUNC_MFP + 0)
#define GPIO48_MFP_GPIO48        (PAD_FUNC_MFP + 1)
#define GPIO48_MFP_SPI0_CLK      (PAD_FUNC_MFP + 2)
#define GPIO48_MFP_PWM8          (PAD_FUNC_MFP + 3)
#define GPIO48_MFP_TIMER2_CAPIN  (PAD_FUNC_MFP + 4)
#define GPIO48_MFP_UART0_TX      (PAD_FUNC_MFP + 5)
#define GPIO48_MFP_UART1_TX      (PAD_FUNC_MFP + 6)
#define GPIO48_MFP_IR_RX         (PAD_FUNC_MFP + 7)
#define GPIO48_MFP_FMCLKOUT      (PAD_FUNC_MFP + 8)
#define GPIO48_MFP_SPDIF_RX      (PAD_FUNC_MFP + 9)

//GPIO49 MFP
#define GPIO49_MFP_SPI0_SS       (PAD_FUNC_MFP + 0)
#define GPIO49_MFP_GPIO49        (PAD_FUNC_MFP + 1)
#define GPIO49_MFP_SPI0_MOSI     (PAD_FUNC_MFP + 2)

//GPIO50 MFP
#define GPIO50_MFP_SPI0_CLK      (PAD_FUNC_MFP + 0)
#define GPIO50_MFP_GPIO50        (PAD_FUNC_MFP + 1)
#define GPIO50_MFP_SPI0_SS       (PAD_FUNC_MFP + 2)

//GPIO51 MFP
#define GPIO51_MFP_SPI0_MOSI     (PAD_FUNC_MFP + 0)
#define GPIO51_MFP_GPIO51        (PAD_FUNC_MFP + 1)
#define GPIO51_MFP_SPI0_MISO     (PAD_FUNC_MFP + 2)


extern int8 hal_set_pad_function(ic_pad_e pad_id, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para);
extern void hal_ioctl_pad(ic_pad_e pad_id, pad_function_e pad_func, void *para);

#define PAD_KEY_LRADC1       PAD_GPIO7
#define PAD_KEY_LRADCX       PAD_GPIO3
#define PAD_KEY_LRADCX_IDX   9

//only for welcome
extern int8 hal_pad_gpio3_op(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para);
extern int8 hal_pad_gpio7_op(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para);
#define hal_pad_lradc1_op    hal_pad_gpio7_op
#define hal_pad_lradcx_op    hal_pad_gpio3_op

#endif

