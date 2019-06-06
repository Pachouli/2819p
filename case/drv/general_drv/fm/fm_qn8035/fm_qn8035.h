/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM QN8035 模组驱动头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _FM_QN8035_H
#define _FM_QN8035_H

#include "psp_includes.h"
#include "fm_interface.h"

////register information


/**********************************QN8035's clock source selection**************
 1.setting QN8035's clock source and clock type,recommendation use 32768Hz clock as
 chip's clock.
 2.this group value is 32768HZ clock as QN8035's clock source.
 3.user need to modify according to actual hardware platform,notice clock unit is Hz.
 4.clock formula:
 XTAL_DIV = Round(Clock/32768);
 PLL_DLT = Round((28500000*512*XTAL_DIV)/Clock)-442368
 *******************************************************************************/
#define QND_INPUT_CLOCK                 0x00    //using external clock or oscillator as qn8035's clock 
#define QND_CRYSTAL_CLOCK               0x80    //using crystal as qn8035's clock
#define QND_SINE_WAVE_CLOCK             0x00    //inject sine-wave clock
#define QND_DIGITAL_CLOCK               0x80    //inject digital clock,default is inject digital clock
//32768HZ
#define QND_XTAL_DIV0_32K                   0x01
#define QND_XTAL_DIV1_32K                   0x08
#define QND_XTAL_DIV2_32K                   0x5C

//13MHZ
#define QND_XTAL_DIV0_13M                   0x8D
#define QND_XTAL_DIV1_13M                   0x89
#define QND_XTAL_DIV2_13M                   0x65

//24MHZ
#define QND_XTAL_DIV0_24M                   0xDC
#define QND_XTAL_DIV1_24M                   0x02
#define QND_XTAL_DIV2_24M                   0x54

//26MHZ
#define QND_XTAL_DIV0_26M                   0x19
#define QND_XTAL_DIV1_26M                   0x03
#define QND_XTAL_DIV2_26M                   0x54

////fm module I2C address
#define I2C_READ_ADDR                   (0x21)
#define I2C_WRITE_ADDR                  (0x20)

#define QND_MODE_SLEEP                  0
#define QND_MODE_WAKEUP                 1
// RX / TX value is using upper 8 bit

#define QND_MODE_RX                     0x8000
#define QND_MODE_TX                     0x4000
// AM / FM value is using lower 8 bit
// need to check datasheet to get right bit
#define QND_MODE_FM                     0x0000
#define BAND_FM                         0

// tune
#define QND_FSTEP_50KHZ                 0
#define QND_FSTEP_100KHZ                1
#define QND_FSTEP_200KHZ                2
//control audio
#define QND_CONFIG_MONO                 0x01
#define QND_CONFIG_MUTE                 0x02
#define QND_CONFIG_VOLUME               0x03
#define QND_RX_AUDIO_MONO               0x20
#define QND_RX_AUDIO_STEREO             0x00

// RDS, TMC
#define QND_EUROPE_FLEXIBILITY_DISABLE  0
#define QND_EUROPE_FLEXIBILITY_ENABLE   1
#define QND_RDS_OFF                     0
#define QND_RDS_ON                      1
#define QND_RDS_BUFFER_NOT_READY        0
#define QND_RDS_BUFFER_READY            1

/****************************Chips ID definition*******************************/
/****************************Chips ID definition*******************************/
#define CHIPID_QN8000                   0x00
#define CHIPID_QN8005                   0x20
#define CHIPID_QN8005B1                 0x21
#define CHIPID_QN8006                   0x30
#define CHIPID_QN8006LB                 0x71
#define CHIPID_QN8007B1                 0x11
#define CHIPID_QN8007                   0x10
#define CHIPID_QN8006A1                 0x30
#define CHIPID_QN8006B1                 0x31
#define CHIPID_QN8016                   0xe0
#define CHIPID_QN8016_1                 0xb0
#define CHIPID_QN8015                   0xa0
#define CHIPID_QN8065                   0xa0
#define CHIPID_QN8067                   0xd0
#define CHIPID_QN8065N                  0xa0
#define CHIPID_QN8027                   0x40
#define CHIPID_QN8025                   0x80
#define CHIPID_QN8035                   0x84
#define CHIPSUBID_QN8035A0              0x01
#define CHIPSUBID_QN8035A1              0x02
#define CHIPID_QN8026                   0x3C
#define CHIPID_QN8036                   0x34
#define CHIPID_QN8301                   0x44

/***************************************End************************************/

/******************************country selection*******************************/
#define COUNTRY_CHINA                   0
#define COUNTRY_USA                     1
#define COUNTRY_JAPAN                   2
/***************************************End************************************/

/*******************************************************************************
 Performance configuration
 *******************************************************************************/
#define SMSTART_VAL                     0x13
#define HCCSTART_VAL                    0x21
#define SNCSTART_VAL                    0x37
//the smaller is this value,the more is number of Automatic scan channels ,fake channels is also increased .
enum
{
    // Bit[15-8] of the word: RSSI Threshold
    // Bit[7-0] of the word: SNR Threshold
    // e.g. 0x1E06 => RSSI_TH = 0x1E, SNR_TH = 0x06
    // notice: the rang of RSSI is 0x0A ~ 0x3F
    // notice: the rang of SNR is 0x00 ~ 0x3F

    CCA_SENSITIVITY_LEVEL_0 = 0x1406,
    CCA_SENSITIVITY_LEVEL_1 = 0x1407, //if using the pilot as CCA,reference this item.
    CCA_SENSITIVITY_LEVEL_2 = 0x1408,
    CCA_SENSITIVITY_LEVEL_3 = 0x1409,
    CCA_SENSITIVITY_LEVEL_4 = 0x140a, //if not using the pilot as CCA,reference this item.
    CCA_SENSITIVITY_LEVEL_5 = 0x140b,
    CCA_SENSITIVITY_LEVEL_6 = 0x140c,
    CCA_SENSITIVITY_LEVEL_7 = 0x140d,
    CCA_SENSITIVITY_LEVEL_8 = 0x140e,
    CCA_SENSITIVITY_LEVEL_9 = 0x140f
};

/***************************************End************************************/

/*******************************************************************************
 definition register
 *******************************************************************************/
#define SYSTEM1                         0x00
#define CCA                             0x01
#define SNR                             0x02
#define RSSISIG                         0x03
#define STATUS1                         0x04
#define CID1                            0x05
#define CID2                            0x06
#define CH                              0x07
#define CH_START                        0x08
#define CH_STOP                         0x09
#define CH_STEP                         0x0a
#define RDSD0                           0x0b
#define RDSD1                           0x0c
#define RDSD2                           0x0d
#define RDSD3                           0x0e
#define RDSD4                           0x0f
#define RDSD5                           0x10
#define RDSD6                           0x11
#define RDSD7                           0x12
#define STATUS2                         0x13
#define VOL_CTL                         0x14
#define XTAL_DIV0                       0x15
#define XTAL_DIV1                       0x16
#define XTAL_DIV2                       0x17
#define INT_CTRL                        0x18
#define SMP_HLD_THRD                    0x19
#define RXAGC_GAIN                      0x1a
#define GAIN_SEL                        0x1b
#define SYSTEM_CTL1                     0x1c
#define SYSTEM_CTL2                     0x1d
#define RDSCOSTAS                       0x1e
#define REG_TEST                        0x1f
#define STATUS4                         0x20
#define RDSAGC2                         0x21
#define CCA1                            0x27
#define CCA2                            0x28
#define CCA3                            0x29
#define CCA4                            0x2a
#define CCA5                            0x2b
#define PLT1                            0x2f
#define PLT2                            0x30
#define SMSTART                         0x34
#define SNCSTART                        0x35
#define HCCSTART                        0x36
#define CCA_CNT1                        0x37
#define CCA_CNT2                        0x38
#define CCA_SNR_TH_1                    0x39
#define CCA_SNR_TH_2                    0x3a
#define NCCFIR3                         0x40
#define NCO_COMP_VAL                    0x46
#define REG_REF                         0x49
#define REG_DAC                         0x4c


/***************************************End************************************/

/*******************************************************************************
 definition operation bit of register
 *******************************************************************************/
#define CCA_CH_DIS                      0x01
#define CHSC                            0x02
#define RDSEN                           0x08
#define CH_CH                           0x03
#define CH_CH_START                     0x0c
#define CH_CH_STOP                      0x30
#define STNBY_MODE                      0x20
#define RX_MODE                         0x10
#define IDLE_MODE                       0x00
#define IMR                             0x40
#define RDS_RXUPD                       0x80
#define ST_MO_RX                        0x01
#define STNBY_RX_MASK                   0x30
#define RXCCA_MASK                      0x03
#define RX_CCA                          0x02
#define RXCCA_FAIL                      0x08
#define RX_MONO                         0x04
#define ICPREF                          0x0f

/***************************************End************************************/

//value to be set
typedef struct
{
    uint32 freq_low; //frequency lowest Hz
    uint32 freq_high; //frequency highest Hz
    uint8 span; //channel span: 50KHZ, 100KHZ, 200KHZ
    uint8 threshold; //seek threshold
    uint8 de_emphasis_tc; //去加重时间常数 50us, 75us
} set_value_t;

extern void QND_RXSetTH(uint8 db);

extern int QND_WriteReg(uint8 RegAddr, uint8 Data);

extern uint8 QND_ReadReg(uint8 RegAddr);

extern int QNF_SetRegBit(uint8 RegAddr, uint8 bitMask, uint8 data_val);

#endif /*_FM_QN8035_H */

