
#ifndef __REG_DEFINITION_H___
#define __REG_DEFINITION_H___

//--------------DMAController-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     DMAController_BASE                                                0xC00C0000
#define     DMAPRIORITY                                                       (DMAController_BASE+0x00000000)
#define     DMAIP                                                             (DMAController_BASE+0x00000004)
#define     DMAIE                                                             (DMAController_BASE+0x00000008)
#define     DMA0CTL                                                           (DMAController_BASE+0x00000010)
#define     DMA0SADDR                                                         (DMAController_BASE+0x00000014)
#define     DMA0DADDR                                                         (DMAController_BASE+0x00000018)
#define     DMA0FRAMELEN                                                      (DMAController_BASE+0x0000001c)
#define     DMA1CTL                                                           (DMAController_BASE+0x00000020)
#define     DMA1SADDR                                                         (DMAController_BASE+0x00000024)
#define     DMA1DADDR                                                         (DMAController_BASE+0x00000028)
#define     DMA1FRAMELEN                                                      (DMAController_BASE+0x0000002c)
#define     DMA2CTL                                                           (DMAController_BASE+0x00000030)
#define     DMA2SADDR                                                         (DMAController_BASE+0x00000034)
#define     DMA2DADDR                                                         (DMAController_BASE+0x00000038)
#define     DMA2FRAMELEN                                                      (DMAController_BASE+0x0000003c)
#define     DMA3CTL                                                           (DMAController_BASE+0x00000040)
#define     DMA3SADDR                                                         (DMAController_BASE+0x00000044)
#define     DMA3DADDR                                                         (DMAController_BASE+0x00000048)
#define     DMA3FRAMELEN                                                      (DMAController_BASE+0x0000004c)
#define     DMA4CTL                                                           (DMAController_BASE+0x00000050)
#define     DMA4SADDR                                                         (DMAController_BASE+0x00000054)
#define     DMA4DADDR                                                         (DMAController_BASE+0x00000058)
#define     DMA4FRAMELEN                                                      (DMAController_BASE+0x0000005c)
#define     DMA0RemainFrameLen                                                (DMAController_BASE+0x00000100)
#define     DMA1RemainFrameLen                                                (DMAController_BASE+0x00000104)
#define     DMA2RemainFrameLen                                                (DMAController_BASE+0x00000108)
#define     DMA3RemainFrameLen                                                (DMAController_BASE+0x0000010c)
#define     DMA4RemainFrameLen                                                (DMAController_BASE+0x00000110)

//--------------Bits Location------------------------------------------//
#define     DMAPRIORITY_PRIORITYTAB_e                                         2
#define     DMAPRIORITY_PRIORITYTAB_SHIFT                                     0
#define     DMAPRIORITY_PRIORITYTAB_MASK                                      (0x7<<0)

#define     DMAIP_DMA4HFIP                                                    12
#define     DMAIP_DMA3HFIP                                                    11
#define     DMAIP_DMA2HFIP                                                    10
#define     DMAIP_DMA1HFIP                                                    9
#define     DMAIP_DMA0HFIP                                                    8
#define     DMAIP_DMA4TCIP                                                    4
#define     DMAIP_DMA3TCIP                                                    3
#define     DMAIP_DMA2TCIP                                                    2
#define     DMAIP_DMA1TCIP                                                    1
#define     DMAIP_DMA0TCIP                                                    0

#define     DMAIE_DMA4HFIE                                                    12
#define     DMAIE_DMA3HFIE                                                    11
#define     DMAIE_DMA2HFIE                                                    10
#define     DMAIE_DMA1HFIE                                                    9
#define     DMAIE_DMA0HFIE                                                    8
#define     DMAIE_DMA4TCIE                                                    4
#define     DMAIE_DMA3TCIE                                                    3
#define     DMAIE_DMA2TCIE                                                    2
#define     DMAIE_DMA1TCIE                                                    1
#define     DMAIE_DMA0TCIE                                                    0

#define     DMA0CTL_DATAWIDTH                                                 12
#define     DMA0CTL_DSTTYPE_e                                                 11
#define     DMA0CTL_DSTTYPE_SHIFT                                             8
#define     DMA0CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA0CTL_SRCTYPE_e                                                 7
#define     DMA0CTL_SRCTYPE_SHIFT                                             4
#define     DMA0CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA0CTL_RELOAD                                                    1
#define     DMA0CTL_DMA0START                                                 0

#define     DMA0SADDR_DMA0SADDR_e                                             18
#define     DMA0SADDR_DMA0SADDR_SHIFT                                         0
#define     DMA0SADDR_DMA0SADDR_MASK                                          (0x7FFFF<<0)

#define     DMA0DADDR_DMA0DADDR_e                                             18
#define     DMA0DADDR_DMA0DADDR_SHIFT                                         0
#define     DMA0DADDR_DMA0DADDR_MASK                                          (0x7FFFF<<0)

#define     DMA0FRAMELEN_DMA0FRAMELEN_e                                       17
#define     DMA0FRAMELEN_DMA0FRAMELEN_SHIFT                                   0
#define     DMA0FRAMELEN_DMA0FRAMELEN_MASK                                    (0x3FFFF<<0)

#define     DMA1CTL_DATAWIDTH                                                 12
#define     DMA1CTL_DSTTYPE_e                                                 11
#define     DMA1CTL_DSTTYPE_SHIFT                                             8
#define     DMA1CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA1CTL_SRCTYPE_e                                                 7
#define     DMA1CTL_SRCTYPE_SHIFT                                             4
#define     DMA1CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA1CTL_RELOAD                                                    1
#define     DMA1CTL_DMA1START                                                 0

#define     DMA1SADDR_DMA1SADDR_e                                             18
#define     DMA1SADDR_DMA1SADDR_SHIFT                                         0
#define     DMA1SADDR_DMA1SADDR_MASK                                          (0x7FFFF<<0)

#define     DMA1DADDR_DMA1DADDR_e                                             18
#define     DMA1DADDR_DMA1DADDR_SHIFT                                         0
#define     DMA1DADDR_DMA1DADDR_MASK                                          (0x7FFFF<<0)

#define     DMA1FRAMELEN_DMA1FRAMELEN_e                                       17
#define     DMA1FRAMELEN_DMA1FRAMELEN_SHIFT                                   0
#define     DMA1FRAMELEN_DMA1FRAMELEN_MASK                                    (0x3FFFF<<0)

#define     DMA2CTL_DATAWIDTH                                                 12
#define     DMA2CTL_DSTTYPE_e                                                 11
#define     DMA2CTL_DSTTYPE_SHIFT                                             8
#define     DMA2CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA2CTL_SRCTYPE_e                                                 7
#define     DMA2CTL_SRCTYPE_SHIFT                                             4
#define     DMA2CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA2CTL_RELOAD                                                    1
#define     DMA2CTL_DMA2START                                                 0

#define     DMA2SADDR_DMA2SADDR_e                                             18
#define     DMA2SADDR_DMA2SADDR_SHIFT                                         0
#define     DMA2SADDR_DMA2SADDR_MASK                                          (0x7FFFF<<0)

#define     DMA2DADDR_DMA2DADDR_e                                             18
#define     DMA2DADDR_DMA2DADDR_SHIFT                                         0
#define     DMA2DADDR_DMA2DADDR_MASK                                          (0x7FFFF<<0)

#define     DMA2FRAMELEN_DMA2FRAMELEN_e                                       17
#define     DMA2FRAMELEN_DMA2FRAMELEN_SHIFT                                   0
#define     DMA2FRAMELEN_DMA2FRAMELEN_MASK                                    (0x3FFFF<<0)

#define     DMA3CTL_DATAWIDTH                                                 12
#define     DMA3CTL_DSTTYPE_e                                                 11
#define     DMA3CTL_DSTTYPE_SHIFT                                             8
#define     DMA3CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA3CTL_SRCTYPE_e                                                 7
#define     DMA3CTL_SRCTYPE_SHIFT                                             4
#define     DMA3CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA3CTL_RELOAD                                                    1
#define     DMA3CTL_DMA3START                                                 0

#define     DMA3SADDR_DMA3SADDR_e                                             18
#define     DMA3SADDR_DMA3SADDR_SHIFT                                         0
#define     DMA3SADDR_DMA3SADDR_MASK                                          (0x7FFFF<<0)

#define     DMA3DADDR_DMA3DADDR_e                                             18
#define     DMA3DADDR_DMA3DADDR_SHIFT                                         0
#define     DMA3DADDR_DMA3DADDR_MASK                                          (0x7FFFF<<0)

#define     DMA3FRAMELEN_DMA3FRAMELEN_e                                       17
#define     DMA3FRAMELEN_DMA3FRAMELEN_SHIFT                                   0
#define     DMA3FRAMELEN_DMA3FRAMELEN_MASK                                    (0x3FFFF<<0)

#define     DMA4CTL_DATAWIDTH                                                 12
#define     DMA4CTL_DSTTYPE_e                                                 11
#define     DMA4CTL_DSTTYPE_SHIFT                                             8
#define     DMA4CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA4CTL_SRCTYPE_e                                                 7
#define     DMA4CTL_SRCTYPE_SHIFT                                             4
#define     DMA4CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA4CTL_RELOAD                                                    1
#define     DMA4CTL_DMA4START                                                 0

#define     DMA4SADDR_DMA4SADDR_e                                             18
#define     DMA4SADDR_DMA4SADDR_SHIFT                                         0
#define     DMA4SADDR_DMA4SADDR_MASK                                          (0x7FFFF<<0)

#define     DMA4DADDR_DMA4DADDR_e                                             18
#define     DMA4DADDR_DMA4DADDR_SHIFT                                         0
#define     DMA4DADDR_DMA4DADDR_MASK                                          (0x7FFFF<<0)

#define     DMA4FRAMELEN_DMA4FRAMELEN_e                                       17
#define     DMA4FRAMELEN_DMA4FRAMELEN_SHIFT                                   0
#define     DMA4FRAMELEN_DMA4FRAMELEN_MASK                                    (0x3FFFF<<0)

#define     DMA0RemainFrameLen_DMA0RemainFrameLen_e                           17
#define     DMA0RemainFrameLen_DMA0RemainFrameLen_SHIFT                       0
#define     DMA0RemainFrameLen_DMA0RemainFrameLen_MASK                        (0x3FFFF<<0)

#define     DMA1RemainFrameLen_DMA1RemainFrameLen_e                           17
#define     DMA1RemainFrameLen_DMA1RemainFrameLen_SHIFT                       0
#define     DMA1RemainFrameLen_DMA1RemainFrameLen_MASK                        (0x3FFFF<<0)

#define     DMA2RemainFrameLen_DMA2RemainFrameLen_e                           17
#define     DMA2RemainFrameLen_DMA2RemainFrameLen_SHIFT                       0
#define     DMA2RemainFrameLen_DMA2RemainFrameLen_MASK                        (0x3FFFF<<0)

#define     DMA3RemainFrameLen_DMA3RemainFrameLen_e                           17
#define     DMA3RemainFrameLen_DMA3RemainFrameLen_SHIFT                       0
#define     DMA3RemainFrameLen_DMA3RemainFrameLen_MASK                        (0x3FFFF<<0)

#define     DMA4RemainFrameLen_DMA4RemainFrameLen_e                           17
#define     DMA4RemainFrameLen_DMA4RemainFrameLen_SHIFT                       0
#define     DMA4RemainFrameLen_DMA4RemainFrameLen_MASK                        (0x3FFFF<<0)

//--------------PMU-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     PMU_BASE                                                          0xc0020000
#define     VOUT_CTL                                                          (PMU_BASE+0x00)
#define     PMUADC_CTL                                                        (PMU_BASE+0x04)
#define     BATADC_DATA                                                       (PMU_BASE+0x08)
#define     AVCCADC_DATA                                                      (PMU_BASE+0x0C)
#define     LRADC1_DATA                                                       (PMU_BASE+0x10)
#define     LRADC2_DATA                                                       (PMU_BASE+0x14)
#define     LRADC3_DATA                                                       (PMU_BASE+0x18)
#define     LRADC4_DATA                                                       (PMU_BASE+0x1C)
#define     LRADC5_DATA                                                       (PMU_BASE+0x20)
#define     LRADC6_DATA                                                       (PMU_BASE+0x24)
#define     LRADC7_DATA                                                       (PMU_BASE+0x28)
#define     LRADC8_DATA                                                       (PMU_BASE+0x2C)
#define     LRADC9_DATA                                                       (PMU_BASE+0x30)
#define     LRADC10_DATA                                                      (PMU_BASE+0x34)
#define     BDG_CTL                                                           (PMU_BASE+0x38)
#define     LRADC_RES_SET                                                     (PMU_BASE+0x3c)
#define     SYSTEM_SET                                                        (PMU_BASE+0x40)
#define     POWER_CTL                                                         (PMU_BASE+0x44)
#define     WKEN_CTL                                                          (PMU_BASE+0x4c)
#define     WAKE_PD                                                           (PMU_BASE+0x50)
#define     ONOFF_KEY                                                         (PMU_BASE+0x54)
#define     BIAS_CTL                                                          (PMU_BASE+0x58)
#define     SPD_CTL                                                           (PMU_BASE+0X64)

//--------------Bits Location------------------------------------------//
#define     VOUT_CTL_SEG_RES_SEL                                              20
#define     VOUT_CTL_SEG_DISP_VCC_EN                                          19
#define     VOUT_CTL_SEG_LED_EN                                               18
#define     VOUT_CTL_AVDD_PD                                                  17
#define     VOUT_CTL_AVDD_VOL_e                                               16
#define     VOUT_CTL_AVDD_VOL_SHIFT                                           15
#define     VOUT_CTL_AVDD_VOL_MASK                                            (0x3<<15)
#define     VOUT_CTL_AVCC_BIASEN                                              14
#define     VOUT_CTL_AVCC_DROP_e                                              13
#define     VOUT_CTL_AVCC_DROP_SHIFT                                          12
#define     VOUT_CTL_AVCC_DROP_MASK                                           (0x3<<12)
#define     VOUT_CTL_VDDCLESS_PD                                              11
#define     VOUT_CTL_VCCOC_EN                                                 10
#define     VOUT_CTL_VCCOC_SET                                                9
#define     VOUT_CTL_VCC_SET_e                                                6
#define     VOUT_CTL_VCC_SET_SHIFT                                            4
#define     VOUT_CTL_VCC_SET_MASK                                             (0x7<<4)
#define     VOUT_CTL_VDD_SET_e                                                3
#define     VOUT_CTL_VDD_SET_SHIFT                                            0
#define     VOUT_CTL_VDD_SET_MASK                                             (0xF<<0)

#define     PMUADC_CTL_I_COMP_SET                                             14
#define     PMUADC_CTL_TEST_SARAD                                             13
#define     PMUADC_CTL_COMP_TRIM                                              12
#define     PMUADC_CTL_LRADC10_EN                                             11
#define     PMUADC_CTL_LRADC9_EN                                              10
#define     PMUADC_CTL_LRADC8_EN                                              9
#define     PMUADC_CTL_LRADC7_EN                                              8
#define     PMUADC_CTL_LRADC6_EN                                              7
#define     PMUADC_CTL_LRADC5_EN                                              6
#define     PMUADC_CTL_LRADC4_EN                                              5
#define     PMUADC_CTL_LRADC3_EN                                              4
#define     PMUADC_CTL_LRADC2_EN                                              3
#define     PMUADC_CTL_LRADC1_EN                                              2
#define     PMUADC_CTL_AVCCADC_EN                                             1
#define     PMUADC_CTL_BATADC_EN                                              0

#define     BATADC_DATA_BATADC_e                                              9
#define     BATADC_DATA_BATADC_SHIFT                                          0
#define     BATADC_DATA_BATADC_MASK                                           (0x3FF<<0)

#define     AVCCADC_DATA_AVCCADC_e                                            9
#define     AVCCADC_DATA_AVCCADC_SHIFT                                        0
#define     AVCCADC_DATA_AVCCADC_MASK                                         (0x3FF<<0)

#define     LRADC1_DATA_LRADC1_e                                              9
#define     LRADC1_DATA_LRADC1_SHIFT                                          0
#define     LRADC1_DATA_LRADC1_MASK                                           (0x3FF<<0)

#define     LRADC2_DATA_LRADC2_e                                              9
#define     LRADC2_DATA_LRADC2_SHIFT                                          0
#define     LRADC2_DATA_LRADC2_MASK                                           (0x3FF<<0)

#define     LRADC3_DATA_LRADC3_e                                              9
#define     LRADC3_DATA_LRADC3_SHIFT                                          0
#define     LRADC3_DATA_LRADC3_MASK                                           (0x3FF<<0)

#define     LRADC4_DATA_LRADC4_e                                              9
#define     LRADC4_DATA_LRADC4_SHIFT                                          0
#define     LRADC4_DATA_LRADC4_MASK                                           (0x3FF<<0)

#define     LRADC5_DATA_LRADC5_e                                              9
#define     LRADC5_DATA_LRADC5_SHIFT                                          0
#define     LRADC5_DATA_LRADC5_MASK                                           (0x3FF<<0)

#define     LRADC6_DATA_LRADC6_e                                              9
#define     LRADC6_DATA_LRADC6_SHIFT                                          0
#define     LRADC6_DATA_LRADC6_MASK                                           (0x3FF<<0)

#define     LRADC7_DATA_LRADC7_e                                              9
#define     LRADC7_DATA_LRADC7_SHIFT                                          0
#define     LRADC7_DATA_LRADC7_MASK                                           (0x3FF<<0)

#define     LRADC8_DATA_LRADC8_e                                              9
#define     LRADC8_DATA_LRADC8_SHIFT                                          0
#define     LRADC8_DATA_LRADC8_MASK                                           (0x3FF<<0)

#define     LRADC9_DATA_LRADC9_e                                              9
#define     LRADC9_DATA_LRADC9_SHIFT                                          0
#define     LRADC9_DATA_LRADC9_MASK                                           (0x3FF<<0)

#define     LRADC10_DATA_LRADC10_e                                            9
#define     LRADC10_DATA_LRADC10_SHIFT                                        0
#define     LRADC10_DATA_LRADC10_MASK                                         (0x3FF<<0)

#define     BDG_CTL_BDG_EN                                                    8
#define     BDG_CTL_BDG_FILTER                                                6
#define     BDG_CTL_BDG_PDR                                                   5
#define     BDG_CTL_BDG_VOL_e                                                 4
#define     BDG_CTL_BDG_VOL_SHIFT                                             0
#define     BDG_CTL_BDG_VOL_MASK                                              (0x1F<<0)

#define     LRADC_RES_SET_LRADC1_PD                                           6
#define     LRADC_RES_SET_R_SEL                                               5
#define     LRADC_RES_SET_RVALUE_SET_e                                        4
#define     LRADC_RES_SET_RVALUE_SET_SHIFT                                    0
#define     LRADC_RES_SET_RVALUE_SET_MASK                                     (0x1F<<0)

#define     SYSTEM_SET_VCCVREF_SEL                                            8
#define     SYSTEM_SET_LB_EN                                                  7
#define     SYSTEM_SET_LB_VOL_e                                               6
#define     SYSTEM_SET_LB_VOL_SHIFT                                           5
#define     SYSTEM_SET_LB_VOL_MASK                                            (0x3<<5)
#define     SYSTEM_SET_OC_EN                                                  4
#define     SYSTEM_SET_LVPRO_EN                                               3

#define     POWER_CTL_EN_S1                                                   0

#define     WKEN_CTL_WIO1_WKDIR_e                                             19
#define     WKEN_CTL_WIO1_WKDIR_SHIFT                                         18
#define     WKEN_CTL_WIO1_WKDIR_MASK                                          (0x3<<18)
#define     WKEN_CTL_WIO0_WKDIR_e                                             17
#define     WKEN_CTL_WIO0_WKDIR_SHIFT                                         16
#define     WKEN_CTL_WIO0_WKDIR_MASK                                          (0x3<<16)
#define     WKEN_CTL_WIO1_WKEN                                                7
#define     WKEN_CTL_WIO0_WKEN                                                6
#define     WKEN_CTL_ALARM_WKEN                                               5
#define     WKEN_CTL_BAT_WKEN                                                 4
#define     WKEN_CTL_UVLO_WKEN                                                3
#define     WKEN_CTL_RESET_WKEN                                               2
#define     WKEN_CTL_SHORT_WKEN                                               1
#define     WKEN_CTL_LONG_WKEN                                                0

#define     WAKE_PD_RESET_PD                                                  20
#define     WAKE_PD_POWEROK_PD                                                19
#define     WAKE_PD_LB_PD                                                     18
#define     WAKE_PD_OC_PD                                                     17
#define     WAKE_PD_LVPRO_PD                                                  16
#define     WAKE_PD_WIO1_PD                                                   7
#define     WAKE_PD_WIO0_PD                                                   6
#define     WAKE_PD_ALARM_PD                                                  5
#define     WAKE_PD_BATIN_PD                                                  4
#define     WAKE_PD_UVLO_PD                                                   3
#define     WAKE_PD_PRESET_PD                                                 2
#define     WAKE_PD_ONOFF_PD                                                  1
#define     WAKE_PD_LONG_ONOFF                                                0

#define     ONOFF_KEY_UVLO                                                    16
#define     ONOFF_KEY_KEY_SPEED                                               15
#define     ONOFF_KEY_RESTART_SET                                             10
#define     ONOFF_KEY_ONOFF_PRESS_TIME_e                                      9
#define     ONOFF_KEY_ONOFF_PRESS_TIME_SHIFT                                  7
#define     ONOFF_KEY_ONOFF_PRESS_TIME_MASK                                   (0x7<<7)
#define     ONOFF_KEY_ONOFF_RST_EN                                            6
#define     ONOFF_KEY_ONOFF_RST_T_SEL_e                                       5
#define     ONOFF_KEY_ONOFF_RST_T_SEL_SHIFT                                   4
#define     ONOFF_KEY_ONOFF_RST_T_SEL_MASK                                    (0x3<<4)
#define     ONOFF_KEY_ONOFF_PRESS_1                                           1
#define     ONOFF_KEY_ONOFF_PRESS_0                                           0

#define     BIAS_CTL_SIM_A_EN                                                 2
#define     BIAS_CTL_PMU_BIAS_e                                               1
#define     BIAS_CTL_PMU_BIAS_SHIFT                                           0
#define     BIAS_CTL_PMU_BIAS_MASK                                            (0x3<<0)

#define     SPD_CTL_SVCC_SET_e                                                7
#define     SPD_CTL_SVCC_SET_SHIFT                                            5
#define     SPD_CTL_SVCC_SET_MASK                                             (0x7<<5)
#define     SPD_CTL_SVCCPD                                                    4
#define     SPD_CTL_AVCCPD                                                    3
#define     SPD_CTL_VDDPD                                                     1
#define     SPD_CTL_VCCPD                                                     0

//--------------RMU_digital-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     RMU_digital_BASE                                                  0xc0000000
#define     MRCR                                                              (RMU_digital_BASE+0x00000000)

//--------------Bits Location------------------------------------------//
#define     MRCR_IRC_RESET                                                    24
#define     MRCR_UART1_RESET                                                  23
#define     MRCR_SEG_LCD_LED_RESET                                            21
#define     MRCR_LCD_RESET                                                    20
#define     MRCR_VAD_RESET                                                    19
#define     MRCR_IIC1_RESET                                                   18
#define     MRCR_SPI1_RESET                                                   17
#define     MRCR_SDIO_RESET                                                   16
#define     MRCR_MODEM_DBG_IQ_RST                                             15
#define     MRCR_SPI_CACHE_RESET                                              14
#define     MRCR_FMCLK_RESET                                                  13
#define     MRCR_AUDIO_RESET                                                  12
#define     MRCR_SDC_RESET                                                    11
#define     MRCR_USB_RESET2                                                   10
#define     MRCR_USB_RESET1                                                   9
#define     MRCR_PWM_RESET                                                    8
#define     MRCR_IIC0_RESET                                                   7
#define     MRCR_SPI0_RESET                                                   6
#define     MRCR_BIST_RESET                                                   5
#define     MRCR_UART0_RESET                                                  4
#define     MRCR_BT_RF_RESET                                                  3
#define     MRCR_BT_MODEM_RESET                                               2
#define     MRCR_BT_BB_RESET                                                  1
#define     MRCR_DMA_RESET                                                    0

//--------------ChipVersion-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     ChipVersion_BASE                                                  0xC0000004
#define     ChipVersion                                                       (ChipVersion_BASE+0x0000)

//--------------Bits Location------------------------------------------//
#define     ChipVersion_ChipVersion_e                                         3
#define     ChipVersion_ChipVersion_SHIFT                                     0
#define     ChipVersion_ChipVersion_MASK                                      (0xF<<0)

//--------------CMU_Control_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     CMU_Control_Register_BASE                                         0xC0001000
#define     CMU_SYSCLK                                                        (CMU_Control_Register_BASE+0x0000)
#define     CMU_DEVCLKEN                                                      (CMU_Control_Register_BASE+0x0004)
#define     CMU_ADDACLK                                                       (CMU_Control_Register_BASE+0x0008)
#define     CMU_I2SCLK                                                        (CMU_Control_Register_BASE+0x0010)
#define     CMU_UARTCLK                                                       (CMU_Control_Register_BASE+0x0014)
#define     CMU_SD0CLK                                                        (CMU_Control_Register_BASE+0x0018)
#define     CMU_SD1CLK                                                        (CMU_Control_Register_BASE+0x001C)
#define     CMU_LCDCLK                                                        (CMU_Control_Register_BASE+0x0020)
#define     CMU_SEGLCDCLK                                                     (CMU_Control_Register_BASE+0x0024)
#define     CMU_SPICLK                                                        (CMU_Control_Register_BASE+0x0028)
#define     CMU_FMCLK                                                         (CMU_Control_Register_BASE+0x002C)
#define     CMU_VADCLK                                                        (CMU_Control_Register_BASE+0x0030)
#define     CMU_LRADCCLK                                                      (CMU_Control_Register_BASE+0x0034)
#define     CMU_PWM0CLK                                                       (CMU_Control_Register_BASE+0x0038)
#define     CMU_PWM1CLK                                                       (CMU_Control_Register_BASE+0x003C)
#define     CMU_PWM2CLK                                                       (CMU_Control_Register_BASE+0x0040)
#define     CMU_PWM3CLK                                                       (CMU_Control_Register_BASE+0x0044)
#define     CMU_PWM4CLK                                                       (CMU_Control_Register_BASE+0x0048)
#define     CMU_PWM5CLK                                                       (CMU_Control_Register_BASE+0x004C)
#define     CMU_PWM6CLK                                                       (CMU_Control_Register_BASE+0x0050)
#define     CMU_PWM7CLK                                                       (CMU_Control_Register_BASE+0x0054)
#define     CMU_PWM8CLK                                                       (CMU_Control_Register_BASE+0x0058)
#define     CMU_BTCLK                                                         (CMU_Control_Register_BASE+0x005C)
#define     CMU_HCL_3K2CLK                                                    (CMU_Control_Register_BASE+0x0060)
#define     CMU_MEMCLKEN                                                      (CMU_Control_Register_BASE+0x0064)
#define     CMU_MEMCLKSEL                                                     (CMU_Control_Register_BASE+0x0068)
#define     CMU_DIGITALDEBUG                                                  (CMU_Control_Register_BASE+0x006C)

//--------------Bits Location------------------------------------------//
#define     CMU_SYSCLK_AHBCLKDIV                                              8
#define     CMU_SYSCLK_CPUCLKDIV_e                                            5
#define     CMU_SYSCLK_CPUCLKDIV_SHIFT                                        4
#define     CMU_SYSCLK_CPUCLKDIV_MASK                                         (0x3<<4)
#define     CMU_SYSCLK_CORE_CLKSEL_e                                          1
#define     CMU_SYSCLK_CORE_CLKSEL_SHIFT                                      0
#define     CMU_SYSCLK_CORE_CLKSEL_MASK                                       (0x3<<0)

#define     CMU_DEVCLKEN_BT_RFCLK_EN                                          30
#define     CMU_DEVCLKEN_BT_MODEMCLK_EN                                       29
#define     CMU_DEVCLKEN_BT_BBCLK_EN                                          28
#define     CMU_DEVCLKEN_BT_BB3P2K_EN                                         27
#define     CMU_DEVCLKEN_SPICACHECLKEN                                        26
#define     CMU_DEVCLKEN_PWMCLKEN                                             25
#define     CMU_DEVCLKEN_LRADCCLKEN                                           24
#define     CMU_DEVCLKEN_VADCLKEN                                             23
#define     CMU_DEVCLKEN_FMCLKEN                                              22
#define     CMU_DEVCLKEN_IRCLKEN                                              21
#define     CMU_DEVCLKEN_USBCLKEN                                             20
#define     CMU_DEVCLKEN_TIMER3CLKEN                                          19
#define     CMU_DEVCLKEN_TIMER2CLKEN                                          18
#define     CMU_DEVCLKEN_TIMER1CLKEN                                          17
#define     CMU_DEVCLKEN_TIMER0CLKEN                                          16
#define     CMU_DEVCLKEN_SPI1CLKEN                                            15
#define     CMU_DEVCLKEN_SPI0CLKEN                                            14
#define     CMU_DEVCLKEN_I2C1CLKEN                                            13
#define     CMU_DEVCLKEN_I2C0CLKEN                                            12
#define     CMU_DEVCLKEN_SEGLCDCLKEN                                          11
#define     CMU_DEVCLKEN_LCDCLKEN                                             10
#define     CMU_DEVCLKEN_SD1CLKEN                                             9
#define     CMU_DEVCLKEN_SD0CLKEN                                             8
#define     CMU_DEVCLKEN_UART1CLKEN                                           7
#define     CMU_DEVCLKEN_UART0CLKEN                                           6
#define     CMU_DEVCLKEN_I2SRXCLKEN                                           5
#define     CMU_DEVCLKEN_I2STXCLKEN                                           4
#define     CMU_DEVCLKEN_DACCLKEN                                             2
#define     CMU_DEVCLKEN_ADCCLKEN                                             1
#define     CMU_DEVCLKEN_DMACLKEN                                             0

#define     CMU_ADDACLK_ADCCLKPREDIV                                          7
#define     CMU_ADDACLK_ADCCLKDIV_e                                           6
#define     CMU_ADDACLK_ADCCLKDIV_SHIFT                                       4
#define     CMU_ADDACLK_ADCCLKDIV_MASK                                        (0x7<<4)
#define     CMU_ADDACLK_DACCLKPREDIV                                          3
#define     CMU_ADDACLK_DACCLKDIV_e                                           2
#define     CMU_ADDACLK_DACCLKDIV_SHIFT                                       0
#define     CMU_ADDACLK_DACCLKDIV_MASK                                        (0x7<<0)

#define     CMU_I2SCLK_I2SRXCLKSEL_e                                          9
#define     CMU_I2SCLK_I2SRXCLKSEL_SHIFT                                      8
#define     CMU_I2SCLK_I2SRXCLKSEL_MASK                                       (0x3<<8)
#define     CMU_I2SCLK_I2STXCLKDIV                                            6
#define     CMU_I2SCLK_I2STXCLKSEL_e                                          5
#define     CMU_I2SCLK_I2STXCLKSEL_SHIFT                                      4
#define     CMU_I2SCLK_I2STXCLKSEL_MASK                                       (0x3<<4)
#define     CMU_I2SCLK_I2SCLKDIV_e                                            2
#define     CMU_I2SCLK_I2SCLKDIV_SHIFT                                        0
#define     CMU_I2SCLK_I2SCLKDIV_MASK                                         (0x7<<0)

#define     CMU_UARTCLK_SEL1                                                  4
#define     CMU_UARTCLK_SEL0                                                  0

#define     CMU_SD0CLK_SD0CLKSEL1                                             8
#define     CMU_SD0CLK_SD0CLKSEL0                                             4
#define     CMU_SD0CLK_SD0CLKDIV_e                                            2
#define     CMU_SD0CLK_SD0CLKDIV_SHIFT                                        0
#define     CMU_SD0CLK_SD0CLKDIV_MASK                                         (0x7<<0)

#define     CMU_SD1CLK_SD1CLKSEL1                                             8
#define     CMU_SD1CLK_SD1CLKSEL0                                             4
#define     CMU_SD1CLK_SD1CLKDIV_e                                            2
#define     CMU_SD1CLK_SD1CLKDIV_SHIFT                                        0
#define     CMU_SD1CLK_SD1CLKDIV_MASK                                         (0x7<<0)

#define     CMU_LCDCLK_LCDCLKSEL                                              4
#define     CMU_LCDCLK_LCDCLKDIV_e                                            2
#define     CMU_LCDCLK_LCDCLKDIV_SHIFT                                        0
#define     CMU_LCDCLK_LCDCLKDIV_MASK                                         (0x7<<0)

#define     CMU_SEGLCDCLK_CLKSEL                                              4
#define     CMU_SEGLCDCLK_CLKDIV1                                             3
#define     CMU_SEGLCDCLK_CLKDIV0_e                                           2
#define     CMU_SEGLCDCLK_CLKDIV0_SHIFT                                       0
#define     CMU_SEGLCDCLK_CLKDIV0_MASK                                        (0x7<<0)

#define     CMU_SPICLK_SEL1_e                                                 15
#define     CMU_SPICLK_SEL1_SHIFT                                             14
#define     CMU_SPICLK_SEL1_MASK                                              (0x3<<14)
#define     CMU_SPICLK_DIV1_e                                                 12
#define     CMU_SPICLK_DIV1_SHIFT                                             8
#define     CMU_SPICLK_DIV1_MASK                                              (0x1F<<8)
#define     CMU_SPICLK_SEL0_e                                                 7
#define     CMU_SPICLK_SEL0_SHIFT                                             6
#define     CMU_SPICLK_SEL0_MASK                                              (0x3<<6)
#define     CMU_SPICLK_DIV0_e                                                 4
#define     CMU_SPICLK_DIV0_SHIFT                                             0
#define     CMU_SPICLK_DIV0_MASK                                              (0x1F<<0)

#define     CMU_FMCLK_FMCLKSEL_e                                              1
#define     CMU_FMCLK_FMCLKSEL_SHIFT                                          0
#define     CMU_FMCLK_FMCLKSEL_MASK                                           (0x3<<0)

#define     CMU_VADCLK_CLKDIV_e                                               1
#define     CMU_VADCLK_CLKDIV_SHIFT                                           0
#define     CMU_VADCLK_CLKDIV_MASK                                            (0x3<<0)

#define     CMU_LRADCCLK_CLKDIV_e                                             1
#define     CMU_LRADCCLK_CLKDIV_SHIFT                                         0
#define     CMU_LRADCCLK_CLKDIV_MASK                                          (0x3<<0)

#define     CMU_PWM0CLK_CLKSEL                                                12
#define     CMU_PWM0CLK_CLKDIV_e                                              8
#define     CMU_PWM0CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM0CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM1CLK_CLKSEL                                                12
#define     CMU_PWM1CLK_CLKDIV_e                                              8
#define     CMU_PWM1CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM1CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM2CLK_CLKSEL                                                12
#define     CMU_PWM2CLK_CLKDIV_e                                              8
#define     CMU_PWM2CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM2CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM3CLK_CLKSEL                                                12
#define     CMU_PWM3CLK_CLKDIV_e                                              8
#define     CMU_PWM3CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM3CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM4CLK_CLKSEL                                                12
#define     CMU_PWM4CLK_CLKDIV_e                                              8
#define     CMU_PWM4CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM4CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM5CLK_CLKSEL                                                12
#define     CMU_PWM5CLK_CLKDIV_e                                              8
#define     CMU_PWM5CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM5CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM6CLK_CLKSEL                                                12
#define     CMU_PWM6CLK_CLKDIV_e                                              8
#define     CMU_PWM6CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM6CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM7CLK_CLKSEL                                                12
#define     CMU_PWM7CLK_CLKDIV_e                                              8
#define     CMU_PWM7CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM7CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_PWM8CLK_CLKSEL                                                12
#define     CMU_PWM8CLK_CLKDIV_e                                              8
#define     CMU_PWM8CLK_CLKDIV_SHIFT                                          0
#define     CMU_PWM8CLK_CLKDIV_MASK                                           (0x1FF<<0)

#define     CMU_BTCLK_BT_ADDA_CLK_D_e                                         9
#define     CMU_BTCLK_BT_ADDA_CLK_D_SHIFT                                     4
#define     CMU_BTCLK_BT_ADDA_CLK_D_MASK                                      (0x3F<<4)
#define     CMU_BTCLK_BT_ADDA_CLK_INVERT                                      3
#define     CMU_BTCLK_BT_BB3P2K_SEL                                           2
#define     CMU_BTCLK_BT_MODEM_RXEN_SEL                                       1
#define     CMU_BTCLK_BT_MODEMCLK_SEL                                         0

#define     CMU_HCL_3K2CLK_HCL_INTERVAL_e                                     31
#define     CMU_HCL_3K2CLK_HCL_INTERVAL_SHIFT                                 16
#define     CMU_HCL_3K2CLK_HCL_INTERVAL_MASK                                  (0xFFFF<<16)
#define     CMU_HCL_3K2CLK_HCL_BUSY                                           9
#define     CMU_HCL_3K2CLK_HCL_SOFT_START                                     8
#define     CMU_HCL_3K2CLK_HOSC_OSC_TIME_e                                    7
#define     CMU_HCL_3K2CLK_HOSC_OSC_TIME_SHIFT                                6
#define     CMU_HCL_3K2CLK_HOSC_OSC_TIME_MASK                                 (0x3<<6)
#define     CMU_HCL_3K2CLK_CAL_DELAY_TIME_e                                   5
#define     CMU_HCL_3K2CLK_CAL_DELAY_TIME_SHIFT                               4
#define     CMU_HCL_3K2CLK_CAL_DELAY_TIME_MASK                                (0x3<<4)
#define     CMU_HCL_3K2CLK_HCL_PRECISION_e                                    3
#define     CMU_HCL_3K2CLK_HCL_PRECISION_SHIFT                                2
#define     CMU_HCL_3K2CLK_HCL_PRECISION_MASK                                 (0x3<<2)
#define     CMU_HCL_3K2CLK_HCL_HCM_EN                                         1
#define     CMU_HCL_3K2CLK_HCL_EN                                             0

#define     CMU_MEMCLKEN_SPICACHERAMCLKEN                                     11
#define     CMU_MEMCLKEN_SD1BUFCLKEN                                          10
#define     CMU_MEMCLKEN_SD0BUFCLKEN                                          9
#define     CMU_MEMCLKEN_URAMCLKEN                                            8
#define     CMU_MEMCLKEN_PCMRAMCLKEN                                          7
#define     CMU_MEMCLKEN_RAM4CLKEN                                            6
#define     CMU_MEMCLKEN_RAM3CLKEN                                            5
#define     CMU_MEMCLKEN_RAM2CLKEN                                            4
#define     CMU_MEMCLKEN_RAM1CLKEN                                            3
#define     CMU_MEMCLKEN_RAM0CLKEN                                            2
#define     CMU_MEMCLKEN_ROM1CLKEN                                            1
#define     CMU_MEMCLKEN_ROM0CLKEN                                            0

#define     CMU_MEMCLKSEL_SD1BUFCLKSEL                                        6
#define     CMU_MEMCLKSEL_SD0BUFCLKSEL                                        5
#define     CMU_MEMCLKSEL_URAMCLKSEL                                          4
#define     CMU_MEMCLKSEL_PCMRAMCLKSEL                                        3
#define     CMU_MEMCLKSEL_RAM4CLKSEL_e                                        2
#define     CMU_MEMCLKSEL_RAM4CLKSEL_SHIFT                                    1
#define     CMU_MEMCLKSEL_RAM4CLKSEL_MASK                                     (0x3<<1)
#define     CMU_MEMCLKSEL_RAM3CLKSEL                                          0

#define     CMU_DIGITALDEBUG_DBGEN                                            31
#define     CMU_DIGITALDEBUG_COUNT_e                                          30
#define     CMU_DIGITALDEBUG_COUNT_SHIFT                                      7
#define     CMU_DIGITALDEBUG_COUNT_MASK                                       (0xFFFFFF<<7)
#define     CMU_DIGITALDEBUG_DBG_COUNT_EN                                     6
#define     CMU_DIGITALDEBUG_SEL_e                                            5
#define     CMU_DIGITALDEBUG_SEL_SHIFT                                        0
#define     CMU_DIGITALDEBUG_SEL_MASK                                         (0x3F<<0)

//--------------CMU_Analog_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     CMU_Analog_Register_BASE                                          0xC0000100
#define     HOSC_CTL                                                          (CMU_Analog_Register_BASE+0x00)
#define     SPLL_CTL                                                          (CMU_Analog_Register_BASE+0x08)
#define     CORE_PLL_CTL                                                      (CMU_Analog_Register_BASE+0x0C)
#define     AUDIO_PLL_CTL                                                     (CMU_Analog_Register_BASE+0x10)
#define     CORE_PLL_DEBUG                                                    (CMU_Analog_Register_BASE+0x14)
#define     AUDIO_PLL_DEBUG                                                   (CMU_Analog_Register_BASE+0x18)
#define     SPLL_DEBUG                                                        (CMU_Analog_Register_BASE+0x1c)
#define     CMU_DEBUG_CTL                                                     (CMU_Analog_Register_BASE+0x20)

//--------------Bits Location------------------------------------------//
#define     HOSC_CTL_coarse_cap_xi_e                                          18
#define     HOSC_CTL_coarse_cap_xi_SHIFT                                      17
#define     HOSC_CTL_coarse_cap_xi_MASK                                       (0x3<<17)
#define     HOSC_CTL_fine_cap_xi_e                                            16
#define     HOSC_CTL_fine_cap_xi_SHIFT                                        12
#define     HOSC_CTL_fine_cap_xi_MASK                                         (0x1F<<12)
#define     HOSC_CTL_coarse_cap_xo_e                                          10
#define     HOSC_CTL_coarse_cap_xo_SHIFT                                      9
#define     HOSC_CTL_coarse_cap_xo_MASK                                       (0x3<<9)
#define     HOSC_CTL_fine_cap_xo_e                                            8
#define     HOSC_CTL_fine_cap_xo_SHIFT                                        4
#define     HOSC_CTL_fine_cap_xo_MASK                                         (0x1F<<4)
#define     HOSC_CTL_xo_ib_sel_e                                              3
#define     HOSC_CTL_xo_ib_sel_SHIFT                                          1
#define     HOSC_CTL_xo_ib_sel_MASK                                           (0x7<<1)
#define     HOSC_CTL_HOSC_EN                                                  0

#define     SPLL_CTL_VDD_HOSC_SEL                                             6
#define     SPLL_CTL_CK_24M_EN                                                5
#define     SPLL_CTL_CK_48M_EN                                                4
#define     SPLL_CTL_CK_52M_EN                                                3
#define     SPLL_CTL_SPLL_BAND_SEL                                            2
#define     SPLL_CTL_SPLL_PMD                                                 1
#define     SPLL_CTL_SPLL_EN                                                  0

#define     CORE_PLL_CTL_CORE_PLL_PMD                                         10
#define     CORE_PLL_CTL_CORE_PLL_SCLK_SEL_e                                  9
#define     CORE_PLL_CTL_CORE_PLL_SCLK_SEL_SHIFT                              8
#define     CORE_PLL_CTL_CORE_PLL_SCLK_SEL_MASK                               (0x3<<8)
#define     CORE_PLL_CTL_CORE_PLL_EN                                          7
#define     CORE_PLL_CTL_SCORE_e                                              6
#define     CORE_PLL_CTL_SCORE_SHIFT                                          0
#define     CORE_PLL_CTL_SCORE_MASK                                           (0x7F<<0)

#define     AUDIO_PLL_CTL_AUDPLL_OUTPUT_DIV                                   8
#define     AUDIO_PLL_CTL_AUDPLL_MODE_SEL                                     7
#define     AUDIO_PLL_CTL_AUDPLL_PMD                                          6
#define     AUDIO_PLL_CTL_AUDPLL_SCLK_SEL                                     5
#define     AUDIO_PLL_CTL_AUDPLLEN                                            4
#define     AUDIO_PLL_CTL_APS_e                                               3
#define     AUDIO_PLL_CTL_APS_SHIFT                                           0
#define     AUDIO_PLL_CTL_APS_MASK                                            (0xF<<0)

#define     CORE_PLL_DEBUG_CORE_PLL_PU_FORCE                                  31
#define     CORE_PLL_DEBUG_CORE_PLL_FORCE_VC_e                                30
#define     CORE_PLL_DEBUG_CORE_PLL_FORCE_VC_SHIFT                            29
#define     CORE_PLL_DEBUG_CORE_PLL_FORCE_VC_MASK                             (0x3<<29)
#define     CORE_PLL_DEBUG_IB_CORE_PLL_e                                      28
#define     CORE_PLL_DEBUG_IB_CORE_PLL_SHIFT                                  27
#define     CORE_PLL_DEBUG_IB_CORE_PLL_MASK                                   (0x3<<27)
#define     CORE_PLL_DEBUG_CORE_PLL_CS_e                                      26
#define     CORE_PLL_DEBUG_CORE_PLL_CS_SHIFT                                  25
#define     CORE_PLL_DEBUG_CORE_PLL_CS_MASK                                   (0x3<<25)
#define     CORE_PLL_DEBUG_CORE_PLL_RS_e                                      24
#define     CORE_PLL_DEBUG_CORE_PLL_RS_SHIFT                                  23
#define     CORE_PLL_DEBUG_CORE_PLL_RS_MASK                                   (0x3<<23)
#define     CORE_PLL_DEBUG_CORE_PLL_ICPP_e                                    22
#define     CORE_PLL_DEBUG_CORE_PLL_ICPP_SHIFT                                21
#define     CORE_PLL_DEBUG_CORE_PLL_ICPP_MASK                                 (0x3<<21)
#define     CORE_PLL_DEBUG_CORE_PLL_FBAND_e                                   20
#define     CORE_PLL_DEBUG_CORE_PLL_FBAND_SHIFT                               19
#define     CORE_PLL_DEBUG_CORE_PLL_FBAND_MASK                                (0x3<<19)
#define     CORE_PLL_DEBUG_TESTSEL_e                                          18
#define     CORE_PLL_DEBUG_TESTSEL_SHIFT                                      17
#define     CORE_PLL_DEBUG_TESTSEL_MASK                                       (0x3<<17)
#define     CORE_PLL_DEBUG_TEST_EN                                            16
#define     CORE_PLL_DEBUG_CORE_PLL_BYPASS                                    15
#define     CORE_PLL_DEBUG_SSC_EN                                             14
#define     CORE_PLL_DEBUG_SSC_DIV                                            13
#define     CORE_PLL_DEBUG_SDM_ORDER                                          12
#define     CORE_PLL_DEBUG_HNUM_e                                             11
#define     CORE_PLL_DEBUG_HNUM_SHIFT                                         9
#define     CORE_PLL_DEBUG_HNUM_MASK                                          (0x7<<9)
#define     CORE_PLL_DEBUG_PSTEP_e                                            8
#define     CORE_PLL_DEBUG_PSTEP_SHIFT                                        6
#define     CORE_PLL_DEBUG_PSTEP_MASK                                         (0x7<<6)
#define     CORE_PLL_DEBUG_FSTEP_e                                            5
#define     CORE_PLL_DEBUG_FSTEP_SHIFT                                        0
#define     CORE_PLL_DEBUG_FSTEP_MASK                                         (0x3F<<0)

#define     AUDIO_PLL_DEBUG_FCODE_EXT_EN                                      24
#define     AUDIO_PLL_DEBUG_REG_COREPLL_ICPI_e                                23
#define     AUDIO_PLL_DEBUG_REG_COREPLL_ICPI_SHIFT                            22
#define     AUDIO_PLL_DEBUG_REG_COREPLL_ICPI_MASK                             (0x3<<22)
#define     AUDIO_PLL_DEBUG_REG_AUDPLL_ICPI_e                                 21
#define     AUDIO_PLL_DEBUG_REG_AUDPLL_ICPI_SHIFT                             20
#define     AUDIO_PLL_DEBUG_REG_AUDPLL_ICPI_MASK                              (0x3<<20)
#define     AUDIO_PLL_DEBUG_REG_SPLL_ICPI_e                                   19
#define     AUDIO_PLL_DEBUG_REG_SPLL_ICPI_SHIFT                               18
#define     AUDIO_PLL_DEBUG_REG_SPLL_ICPI_MASK                                (0x3<<18)
#define     AUDIO_PLL_DEBUG_AUDPLL_BYPASS                                     16
#define     AUDIO_PLL_DEBUG_AUDPLL_PU_FORCE                                   15
#define     AUDIO_PLL_DEBUG_AUDPLL_FORCE_VC_e                                 14
#define     AUDIO_PLL_DEBUG_AUDPLL_FORCE_VC_SHIFT                             13
#define     AUDIO_PLL_DEBUG_AUDPLL_FORCE_VC_MASK                              (0x3<<13)
#define     AUDIO_PLL_DEBUG_IB_AUDPLL_e                                       12
#define     AUDIO_PLL_DEBUG_IB_AUDPLL_SHIFT                                   11
#define     AUDIO_PLL_DEBUG_IB_AUDPLL_MASK                                    (0x3<<11)
#define     AUDIO_PLL_DEBUG_AUDPLL_CS_e                                       10
#define     AUDIO_PLL_DEBUG_AUDPLL_CS_SHIFT                                   9
#define     AUDIO_PLL_DEBUG_AUDPLL_CS_MASK                                    (0x3<<9)
#define     AUDIO_PLL_DEBUG_AUDPLL_RS_e                                       8
#define     AUDIO_PLL_DEBUG_AUDPLL_RS_SHIFT                                   7
#define     AUDIO_PLL_DEBUG_AUDPLL_RS_MASK                                    (0x3<<7)
#define     AUDIO_PLL_DEBUG_AUDPLL_ICPP_e                                     6
#define     AUDIO_PLL_DEBUG_AUDPLL_ICPP_SHIFT                                 5
#define     AUDIO_PLL_DEBUG_AUDPLL_ICPP_MASK                                  (0x3<<5)
#define     AUDIO_PLL_DEBUG_CORE_PLL_R3_e                                     4
#define     AUDIO_PLL_DEBUG_CORE_PLL_R3_SHIFT                                 3
#define     AUDIO_PLL_DEBUG_CORE_PLL_R3_MASK                                  (0x3<<3)
#define     AUDIO_PLL_DEBUG_TESTSEL_e                                         2
#define     AUDIO_PLL_DEBUG_TESTSEL_SHIFT                                     1
#define     AUDIO_PLL_DEBUG_TESTSEL_MASK                                      (0x3<<1)
#define     AUDIO_PLL_DEBUG_TEST_EN                                           0

#define     SPLL_DEBUG_SPLL_LPF_SEL                                           17
#define     SPLL_DEBUG_SPLL_BYPASS                                            16
#define     SPLL_DEBUG_SPLL_PU_FORCE                                          15
#define     SPLL_DEBUG_SPLL_FORCE_VC_e                                        14
#define     SPLL_DEBUG_SPLL_FORCE_VC_SHIFT                                    13
#define     SPLL_DEBUG_SPLL_FORCE_VC_MASK                                     (0x3<<13)
#define     SPLL_DEBUG_IB_SPLL_e                                              12
#define     SPLL_DEBUG_IB_SPLL_SHIFT                                          11
#define     SPLL_DEBUG_IB_SPLL_MASK                                           (0x3<<11)
#define     SPLL_DEBUG_SPLL_CS_e                                              10
#define     SPLL_DEBUG_SPLL_CS_SHIFT                                          9
#define     SPLL_DEBUG_SPLL_CS_MASK                                           (0x3<<9)
#define     SPLL_DEBUG_SPLL_RS_e                                              8
#define     SPLL_DEBUG_SPLL_RS_SHIFT                                          7
#define     SPLL_DEBUG_SPLL_RS_MASK                                           (0x3<<7)
#define     SPLL_DEBUG_SPLL_ICPP_e                                            6
#define     SPLL_DEBUG_SPLL_ICPP_SHIFT                                        5
#define     SPLL_DEBUG_SPLL_ICPP_MASK                                         (0x3<<5)
#define     SPLL_DEBUG_SPLL_R3_e                                              4
#define     SPLL_DEBUG_SPLL_R3_SHIFT                                          3
#define     SPLL_DEBUG_SPLL_R3_MASK                                           (0x3<<3)
#define     SPLL_DEBUG_TESTSEL_e                                              2
#define     SPLL_DEBUG_TESTSEL_SHIFT                                          1
#define     SPLL_DEBUG_TESTSEL_MASK                                           (0x3<<1)
#define     SPLL_DEBUG_TEST_EN                                                0

#define     CMU_DEBUG_CTL_CMU_TST_IE0_OE1                                     14
#define     CMU_DEBUG_CTL_COREPLL_VB_AMSEL                                    13
#define     CMU_DEBUG_CTL_DBG_CORE_PLL_POSTDIV_e                              12
#define     CMU_DEBUG_CTL_DBG_CORE_PLL_POSTDIV_SHIFT                          11
#define     CMU_DEBUG_CTL_DBG_CORE_PLL_POSTDIV_MASK                           (0x3<<11)
#define     CMU_DEBUG_CTL_DBG_AUDPLL_POSTDIV_e                                10
#define     CMU_DEBUG_CTL_DBG_AUDPLL_POSTDIV_SHIFT                            9
#define     CMU_DEBUG_CTL_DBG_AUDPLL_POSTDIV_MASK                             (0x3<<9)
#define     CMU_DEBUG_CTL_CMU_TST_A0D1                                        8
#define     CMU_DEBUG_CTL_CMU_TST_SEL_e                                       7
#define     CMU_DEBUG_CTL_CMU_TST_SEL_SHIFT                                   5
#define     CMU_DEBUG_CTL_CMU_TST_SEL_MASK                                    (0x7<<5)
#define     CMU_DEBUG_CTL_CORE_PLL_ICP_SEL                                    4
#define     CMU_DEBUG_CTL_CMU_DEBUG_SEL_e                                     3
#define     CMU_DEBUG_CTL_CMU_DEBUG_SEL_SHIFT                                 0
#define     CMU_DEBUG_CTL_CMU_DEBUG_SEL_MASK                                  (0xF<<0)

//--------------RTC_P1-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     RTC_P1_BASE                                                       0xC0120000
#define     RTC_CTL                                                           (RTC_P1_BASE+0x00)
#define     RTC_REGUPDATA                                                     (RTC_P1_BASE+0x04)
#define     RTC_DHMSALM                                                       (RTC_P1_BASE+0x08)
#define     RTC_DHMS                                                          (RTC_P1_BASE+0x0C)
#define     RTC_YMD                                                           (RTC_P1_BASE+0x10)
#define     RTC_ACCESS                                                        (RTC_P1_BASE+0x14)
#define     WD_CTL                                                            (RTC_P1_BASE+0x1c)
#define     HCL_4Hz                                                           (RTC_P1_BASE+0x20)
#define     HCL_CNT                                                           (RTC_P1_BASE+0x24)
#define     RTC_BAK0                                                          (RTC_P1_BASE+0x30)
#define     RTC_BAK1                                                          (RTC_P1_BASE+0x34)
#define     RTC_BAK2                                                          (RTC_P1_BASE+0x38)
#define     RTC_BAK3                                                          (RTC_P1_BASE+0x3c)
#define     RTC_BAK4                                                          (RTC_P1_BASE+0x40)
#define     RTC_BAK5                                                          (RTC_P1_BASE+0x44)
#define     RTC_BAK6                                                          (RTC_P1_BASE+0x48)
#define     RTC_BAK7                                                          (RTC_P1_BASE+0x4c)

//--------------Bits Location------------------------------------------//
#define     RTC_CTL_TEST_EN                                                   16
#define     RTC_CTL_LEAP                                                      7
#define     RTC_CTL_CAL_EN                                                    4
#define     RTC_CTL_HCL_SELECT                                                2
#define     RTC_CTL_ALIE                                                      1
#define     RTC_CTL_ALIP                                                      0

#define     RTC_REGUPDATA_UPDATA_e                                            15
#define     RTC_REGUPDATA_UPDATA_SHIFT                                        0
#define     RTC_REGUPDATA_UPDATA_MASK                                         (0xFFFF<<0)

#define     RTC_DHMSALM_HOUEAL_e                                              20
#define     RTC_DHMSALM_HOUEAL_SHIFT                                          16
#define     RTC_DHMSALM_HOUEAL_MASK                                           (0x1F<<16)
#define     RTC_DHMSALM_MINAL_e                                               13
#define     RTC_DHMSALM_MINAL_SHIFT                                           8
#define     RTC_DHMSALM_MINAL_MASK                                            (0x3F<<8)
#define     RTC_DHMSALM_SECAL_e                                               5
#define     RTC_DHMSALM_SECAL_SHIFT                                           0
#define     RTC_DHMSALM_SECAL_MASK                                            (0x3F<<0)

#define     RTC_DHMS_HOUR_e                                                   20
#define     RTC_DHMS_HOUR_SHIFT                                               16
#define     RTC_DHMS_HOUR_MASK                                                (0x1F<<16)
#define     RTC_DHMS_MIN_e                                                    13
#define     RTC_DHMS_MIN_SHIFT                                                8
#define     RTC_DHMS_MIN_MASK                                                 (0x3F<<8)
#define     RTC_DHMS_SEC_e                                                    5
#define     RTC_DHMS_SEC_SHIFT                                                0
#define     RTC_DHMS_SEC_MASK                                                 (0x3F<<0)

#define     RTC_YMD_YEAR_e                                                    22
#define     RTC_YMD_YEAR_SHIFT                                                16
#define     RTC_YMD_YEAR_MASK                                                 (0x7F<<16)
#define     RTC_YMD_MON_e                                                     11
#define     RTC_YMD_MON_SHIFT                                                 8
#define     RTC_YMD_MON_MASK                                                  (0xF<<8)
#define     RTC_YMD_DATE_e                                                    4
#define     RTC_YMD_DATE_SHIFT                                                0
#define     RTC_YMD_DATE_MASK                                                 (0x1F<<0)

#define     RTC_ACCESS_ACCESS_e                                               7
#define     RTC_ACCESS_ACCESS_SHIFT                                           0
#define     RTC_ACCESS_ACCESS_MASK                                            (0xFF<<0)

#define     WD_CTL_IRQP                                                       6
#define     WD_CTL_SIGS                                                       5
#define     WD_CTL_WDEN                                                       4
#define     WD_CTL_CLKSEL_e                                                   3
#define     WD_CTL_CLKSEL_SHIFT                                               1
#define     WD_CTL_CLKSEL_MASK                                                (0x7<<1)
#define     WD_CTL_CLR                                                        0

#define     HCL_4Hz_TIMEOUT_EN                                                15
#define     HCL_4Hz_TIMEOUT_STA                                               14
#define     HCL_4Hz_HCL_TIMEOUT_SEL_e                                         13
#define     HCL_4Hz_HCL_TIMEOUT_SEL_SHIFT                                     12
#define     HCL_4Hz_HCL_TIMEOUT_SEL_MASK                                      (0x3<<12)
#define     HCL_4Hz_HCL_INTERVAL_e                                            10
#define     HCL_4Hz_HCL_INTERVAL_SHIFT                                        8
#define     HCL_4Hz_HCL_INTERVAL_MASK                                         (0x7<<8)
#define     HCL_4Hz_HOSC_OSC_TIME_e                                           7
#define     HCL_4Hz_HOSC_OSC_TIME_SHIFT                                       6
#define     HCL_4Hz_HOSC_OSC_TIME_MASK                                        (0x3<<6)
#define     HCL_4Hz_CAL_DELAY_TIME_e                                          5
#define     HCL_4Hz_CAL_DELAY_TIME_SHIFT                                      4
#define     HCL_4Hz_CAL_DELAY_TIME_MASK                                       (0x3<<4)
#define     HCL_4Hz_HCL_PRECISION_e                                           3
#define     HCL_4Hz_HCL_PRECISION_SHIFT                                       2
#define     HCL_4Hz_HCL_PRECISION_MASK                                        (0x3<<2)
#define     HCL_4Hz_HOSC_DIV_EN                                               1
#define     HCL_4Hz_HOSC_CAL_EN                                               0

#define     HCL_CNT_HCL_CNT_VALID                                             16
#define     HCL_CNT_HCL_CNT_e                                                 15
#define     HCL_CNT_HCL_CNT_SHIFT                                             0
#define     HCL_CNT_HCL_CNT_MASK                                              (0xFFFF<<0)









//--------------RTC_P2-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     RTC_P2_BASE                                                       0xC0120100
#define     T0_CTL                                                            (RTC_P2_BASE+0x00)
#define     T0_VAL                                                            (RTC_P2_BASE+0x04)
#define     T0_CNT                                                            (RTC_P2_BASE+0x08)
#define     T1_CTL                                                            (RTC_P2_BASE+0x40)
#define     T1_VAL                                                            (RTC_P2_BASE+0x44)
#define     T1_CNT                                                            (RTC_P2_BASE+0x48)
#define     T2_CTL                                                            (RTC_P2_BASE+0x80)
#define     T2_VAL                                                            (RTC_P2_BASE+0x84)
#define     T2_CNT                                                            (RTC_P2_BASE+0x88)
#define     T3_CTL                                                            (RTC_P2_BASE+0xC0)
#define     T3_VAL                                                            (RTC_P2_BASE+0xC4)
#define     T3_CNT                                                            (RTC_P2_BASE+0xC8)

//--------------Bits Location------------------------------------------//
#define     T0_CTL_En                                                         5
#define     T0_CTL_RELO                                                       2
#define     T0_CTL_ZIEN                                                       1
#define     T0_CTL_ZIPD                                                       0

#define     T0_VAL_VAL_e                                                      31
#define     T0_VAL_VAL_SHIFT                                                  0
#define     T0_VAL_VAL_MASK                                                   (0xFFFFFFFF<<0)

#define     T0_CNT_CNT_e                                                      31
#define     T0_CNT_CNT_SHIFT                                                  0
#define     T0_CNT_CNT_MASK                                                   (0xFFFFFFFF<<0)

#define     T1_CTL_En                                                         5
#define     T1_CTL_RELO                                                       2
#define     T1_CTL_ZIEN                                                       1
#define     T1_CTL_ZIPD                                                       0

#define     T1_VAL_VAL_e                                                      31
#define     T1_VAL_VAL_SHIFT                                                  0
#define     T1_VAL_VAL_MASK                                                   (0xFFFFFFFF<<0)

#define     T1_CNT_CNT_e                                                      31
#define     T1_CNT_CNT_SHIFT                                                  0
#define     T1_CNT_CNT_MASK                                                   (0xFFFFFFFF<<0)

#define     T2_CTL_DIR                                                        11
#define     T2_CTL_MODE_SEL                                                   10
#define     T2_CTL_CLK_SEL                                                    9
#define     T2_CTL_CAPTURE_IP                                                 8
#define     T2_CTL_CAPTURE_SE_e                                               7
#define     T2_CTL_CAPTURE_SE_SHIFT                                           6
#define     T2_CTL_CAPTURE_SE_MASK                                            (0x3<<6)
#define     T2_CTL_En                                                         5
#define     T2_CTL_RELO                                                       2
#define     T2_CTL_ZIEN                                                       1
#define     T2_CTL_ZIPD                                                       0

#define     T2_VAL_VAL_e                                                      31
#define     T2_VAL_VAL_SHIFT                                                  0
#define     T2_VAL_VAL_MASK                                                   (0xFFFFFFFF<<0)

#define     T2_CNT_CNT_e                                                      31
#define     T2_CNT_CNT_SHIFT                                                  0
#define     T2_CNT_CNT_MASK                                                   (0xFFFFFFFF<<0)

#define     T3_CTL_DIR                                                        11
#define     T3_CTL_MODE_SEL                                                   10
#define     T3_CTL_CLK_SEL                                                    9
#define     T3_CTL_CAPTURE_IP                                                 8
#define     T3_CTL_CAPTURE_SE_e                                               7
#define     T3_CTL_CAPTURE_SE_SHIFT                                           6
#define     T3_CTL_CAPTURE_SE_MASK                                            (0x3<<6)
#define     T3_CTL_En                                                         5
#define     T3_CTL_RELO                                                       2
#define     T3_CTL_ZIEN                                                       1
#define     T3_CTL_ZIPD                                                       0

#define     T3_VAL_VAL_e                                                      31
#define     T3_VAL_VAL_SHIFT                                                  0
#define     T3_VAL_VAL_MASK                                                   (0xFFFFFFFF<<0)

#define     T3_CNT_CNT_e                                                      31
#define     T3_CNT_CNT_SHIFT                                                  0
#define     T3_CNT_CNT_MASK                                                   (0xFFFFFFFF<<0)

//--------------InterruptController-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     InterruptController_BASE                                          0xC00B0000
#define     INTC_PD                                                           (InterruptController_BASE+0x00000000)
#define     INTC_MSK                                                          (InterruptController_BASE+0x00000004)
#define     INTC_CFG0                                                         (InterruptController_BASE+0x00000008)
#define     INTC_CFG1                                                         (InterruptController_BASE+0x0000000c)
#define     INTC_CFG2                                                         (InterruptController_BASE+0x00000010)

//--------------Bits Location------------------------------------------//
#define     INTC_PD_MPU_IP                                                    30
#define     INTC_PD_TIMER3_IP                                                 28
#define     INTC_PD_TIMER2_IP                                                 27
#define     INTC_PD_SDIO_IP                                                   26
#define     INTC_PD_SPI1_IP                                                   25
#define     INTC_PD_IRC_IP                                                    24
#define     INTC_PD_DMA4_IP                                                   22
#define     INTC_PD_DMA3_IP                                                   21
#define     INTC_PD_DMA2_IP                                                   20
#define     INTC_PD_DMA1_IP                                                   19
#define     INTC_PD_DMA0_IP                                                   18
#define     INTC_PD_SD_IP                                                     17
#define     INTC_PD_UART1_IP                                                  16
#define     INTC_PD_ADC_IP                                                    15
#define     INTC_PD_DAC_IIS_TX_IP                                             14
#define     INTC_PD_VAD_IP                                                    13
#define     INTC_PD_IIC1_IP                                                   12
#define     INTC_PD_IIC0_IP                                                   11
#define     INTC_PD_USB_IP                                                    10
#define     INTC_PD_SPI0_IP                                                   9
#define     INTC_PD_I2S_IP                                                    8
#define     INTC_PD_GPIO_IP                                                   7
#define     INTC_PD_UART0_IP                                                  6
#define     INTC_PD_RTC_IP                                                    5
#define     INTC_PD_TIMER0_IP                                                 4
#define     INTC_PD_TIMER1_IP                                                 3
#define     INTC_PD_WD_IP                                                     2
#define     INTC_PD_PMU_IP                                                    1
#define     INTC_PD_BT_BASEBAND_IP                                            0

#define     INTC_MSK_MPU_IM                                                   30
#define     INTC_MSK_TIMER3_IM                                                28
#define     INTC_MSK_TIMER2_IM                                                27
#define     INTC_MSK_SDIO_IM                                                  26
#define     INTC_MSK_SPI1_IM                                                  25
#define     INTC_MSK_IRC_IM                                                   24
#define     INTC_MSK_DMA4_IM                                                  22
#define     INTC_MSK_DMA3_IM                                                  21
#define     INTC_MSK_DMA2_IM                                                  20
#define     INTC_MSK_DMA1_IM                                                  19
#define     INTC_MSK_DMA0_IM                                                  18
#define     INTC_MSK_SD_IM                                                    17
#define     INTC_MSK_UART1_IM                                                 16
#define     INTC_MSK_ADC_IM                                                   15
#define     INTC_MSK_DAC_IIS_TX_IM                                            14
#define     INTC_MSK_VAD_IM                                                   13
#define     INTC_MSK_IIC1_IM                                                  12
#define     INTC_MSK_IIC0_IM                                                  11
#define     INTC_MSK_USB_IM                                                   10
#define     INTC_MSK_SPI0_IM                                                  9
#define     INTC_MSK_I2S_IM                                                   8
#define     INTC_MSK_GPIO_IM                                                  7
#define     INTC_MSK_UART0_IM                                                 6
#define     INTC_MSK_RTC_IM                                                   5
#define     INTC_MSK_TIMER0_IM                                                4
#define     INTC_MSK_TIMER1_IM                                                3
#define     INTC_MSK_WD_IM                                                    2
#define     INTC_MSK_PMU_IM                                                   1
#define     INTC_MSK_BT_BASEBAND_IM                                           0

#define     INTC_CFG0_MPU_CFG0                                                30
#define     INTC_CFG0_TIMER3_CFG0                                             28
#define     INTC_CFG0_TIMER2_CFG0                                             27
#define     INTC_CFG0_SDIO_CFG0                                               26
#define     INTC_CFG0_SPI1_CFG0                                               25
#define     INTC_CFG0_IRC_CFG0                                                24
#define     INTC_CFG0_DMA4_CFG0                                               22
#define     INTC_CFG0_DMA3_CFG0                                               21
#define     INTC_CFG0_DMA2_CFG0                                               20
#define     INTC_CFG0_DMA1_CFG0                                               19
#define     INTC_CFG0_DMA0_CFG0                                               18
#define     INTC_CFG0_SD_CFG0                                                 17
#define     INTC_CFG0_UART1_CFG0                                              16
#define     INTC_CFG0_ADC_CFG0                                                15
#define     INTC_CFG0_DAC_IIS_TX_CFG0                                         14
#define     INTC_CFG0_VAD_CFG0                                                13
#define     INTC_CFG0_IIC1_CFG0                                               12
#define     INTC_CFG0_IIC0_CFG0                                               11
#define     INTC_CFG0_USB_CFG0                                                10
#define     INTC_CFG0_SPI0_CFG0                                               9
#define     INTC_CFG0_I2S_CFG0                                                8
#define     INTC_CFG0_GPIO_CFG0                                               7
#define     INTC_CFG0_UART0_CFG0                                              6
#define     INTC_CFG0_RTC_CFG0                                                5
#define     INTC_CFG0_TIMER0_CFG0                                             4
#define     INTC_CFG0_TIMER1_CFG0                                             3
#define     INTC_CFG0_WD_CFG0                                                 2
#define     INTC_CFG0_PMU_CFG0                                                1
#define     INTC_CFG0_BT_BASEBAND_CFG0                                        0

#define     INTC_CFG1_MPU_CFG1                                                30
#define     INTC_CFG1_TIMER3_CFG1                                             28
#define     INTC_CFG1_TIMER2_CFG1                                             27
#define     INTC_CFG1_SDIO_CFG1                                               26
#define     INTC_CFG1_SPI1_CFG1                                               25
#define     INTC_CFG1_IRC_CFG1                                                24
#define     INTC_CFG1_DMA4_CFG1                                               22
#define     INTC_CFG1_DMA3_CFG1                                               21
#define     INTC_CFG1_DMA2_CFG1                                               20
#define     INTC_CFG1_DMA1_CFG1                                               19
#define     INTC_CFG1_DMA0_CFG1                                               18
#define     INTC_CFG1_SD_CFG1                                                 17
#define     INTC_CFG1_UART1_CFG1                                              16
#define     INTC_CFG1_ADC_CFG1                                                15
#define     INTC_CFG1_DAC_IIS_TX_CFG1                                         14
#define     INTC_CFG1_VAD_CFG1                                                13
#define     INTC_CFG1_IIC1_CFG1                                               12
#define     INTC_CFG1_IIC0_CFG1                                               11
#define     INTC_CFG1_USB_CFG1                                                10
#define     INTC_CFG1_SPI0_CFG1                                               9
#define     INTC_CFG1_I2S_CFG1                                                8
#define     INTC_CFG1_GPIO_CFG1                                               7
#define     INTC_CFG1_UART0_CFG1                                              6
#define     INTC_CFG1_RTC_CFG1                                                5
#define     INTC_CFG1_TIMER0_CFG1                                             4
#define     INTC_CFG1_TIMER1_CFG1                                             3
#define     INTC_CFG1_WD_CFG1                                                 2
#define     INTC_CFG1_PMU_CFG1                                                1
#define     INTC_CFG1_BT_BASEBAND_CFG1                                        0

#define     INTC_CFG2_MPU_CFG2                                                30
#define     INTC_CFG2_TIMER3_CFG2                                             28
#define     INTC_CFG2_TIMER2_CFG2                                             27
#define     INTC_CFG2_SDIO_CFG2                                               26
#define     INTC_CFG2_SPI1_CFG2                                               25
#define     INTC_CFG2_IRC_CFG2                                                24
#define     INTC_CFG2_DMA4_CFG2                                               22
#define     INTC_CFG2_DMA3_CFG2                                               21
#define     INTC_CFG2_DMA2_CFG2                                               20
#define     INTC_CFG2_DMA1_CFG2                                               19
#define     INTC_CFG2_DMA0_CFG2                                               18
#define     INTC_CFG2_SD_CFG2                                                 17
#define     INTC_CFG2_UART1_CFG2                                              16
#define     INTC_CFG2_ADC_CFG2                                                15
#define     INTC_CFG2_DAC_IIS_TX_CFG2                                         14
#define     INTC_CFG2_VAD_CFG2                                                13
#define     INTC_CFG2_IIC1_CFG2                                               12
#define     INTC_CFG2_IIC0_CFG2                                               11
#define     INTC_CFG2_USB_CFG2                                                10
#define     INTC_CFG2_SPI0_CFG2                                               9
#define     INTC_CFG2_I2S_CFG2                                                8
#define     INTC_CFG2_GPIO_CFG2                                               7
#define     INTC_CFG2_UART0_CFG2                                              6
#define     INTC_CFG2_RTC_CFG2                                                5
#define     INTC_CFG2_TIMER0_CFG2                                             4
#define     INTC_CFG2_TIMER1_CFG2                                             3
#define     INTC_CFG2_WD_CFG2                                                 2
#define     INTC_CFG2_PMU_CFG2                                                1
#define     INTC_CFG2_BT_BASEBAND_CFG2                                        0

//--------------I2S-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     I2S_BASE                                                          0xc0053000
#define     I2S_TXCTL                                                         (I2S_BASE+0x0000)
#define     I2S_RXCTL                                                         (I2S_BASE+0x0004)
#define     I2S_RXFIFOCTL                                                     (I2S_BASE+0x0008)
#define     I2S_RX_STA                                                        (I2S_BASE+0x000C)
#define     I2S_RXDAT                                                         (I2S_BASE+0x0010)

//--------------Bits Location------------------------------------------//
#define     I2S_TXCTL_I2S_5WIRE_MODE                                          9
#define     I2S_TXCTL_I2S_LPEN                                                8
#define     I2S_TXCTL_I2S_TXMODE                                              7
#define     I2S_TXCTL_I2S_TX_SMCLK                                            6
#define     I2S_TXCTL_I2S_TXWIDTH_e                                           5
#define     I2S_TXCTL_I2S_TXWIDTH_SHIFT                                       4
#define     I2S_TXCTL_I2S_TXWIDTH_MASK                                        (0x3<<4)
#define     I2S_TXCTL_I2S_TXBCLKSET                                           3
#define     I2S_TXCTL_I2S_TXMODELSEL_e                                        2
#define     I2S_TXCTL_I2S_TXMODELSEL_SHIFT                                    1
#define     I2S_TXCTL_I2S_TXMODELSEL_MASK                                     (0x3<<1)
#define     I2S_TXCTL_I2S_TXEN                                                0

#define     I2S_RXCTL_I2S_RXMODE                                              7
#define     I2S_RXCTL_I2S_RX_SMCLK                                            6
#define     I2S_RXCTL_I2S_RXWIDTH_e                                           5
#define     I2S_RXCTL_I2S_RXWIDTH_SHIFT                                       4
#define     I2S_RXCTL_I2S_RXWIDTH_MASK                                        (0x3<<4)
#define     I2S_RXCTL_I2S_RXBCLKSET                                           3
#define     I2S_RXCTL_I2S_RXMODELSEL_e                                        2
#define     I2S_RXCTL_I2S_RXMODELSEL_SHIFT                                    1
#define     I2S_RXCTL_I2S_RXMODELSEL_MASK                                     (0x3<<1)
#define     I2S_RXCTL_I2S_RXEN                                                0

#define     I2S_RXFIFOCTL_RXFOS_e                                             5
#define     I2S_RXFIFOCTL_RXFOS_SHIFT                                         4
#define     I2S_RXFIFOCTL_RXFOS_MASK                                          (0x3<<4)
#define     I2S_RXFIFOCTL_RXFIS                                               3
#define     I2S_RXFIFOCTL_RXFFIE                                              2
#define     I2S_RXFIFOCTL_RXFFDE                                              1
#define     I2S_RXFIFOCTL_RXFRT                                               0

#define     I2S_R0_STA_RXFEF                                                  7
#define     I2S_R0_STA_RXFIP                                                  6
#define     I2S_R0_STA_RXFL_e                                                 4
#define     I2S_R0_STA_RXFL_SHIFT                                             0
#define     I2S_R0_STA_RXFL_MASK                                              (0x1F<<0)

#define     I2S_RXDAT_I2SRFDA_e                                               31
#define     I2S_RXDAT_I2SRFDA_SHIFT                                           8
#define     I2S_RXDAT_I2SRFDA_MASK                                            (0xFFFFFF<<8)

//--------------DAC_Control_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     DAC_Control_Register_BASE                                         0xc0050000
#define     DAC_DIGCTL                                                        (DAC_Control_Register_BASE+0x00)
#define     DAC_STAT                                                          (DAC_Control_Register_BASE+0x04)
#define     DAC_DAT_FIFO                                                      (DAC_Control_Register_BASE+0x08)
#define     PCM_BUF_CTL                                                       (DAC_Control_Register_BASE+0x0C)
#define     PCM_BUF_STAT                                                      (DAC_Control_Register_BASE+0x10)
#define     PCM_BUF_THRES_HE                                                  (DAC_Control_Register_BASE+0x14)
#define     PCM_BUF_THRES_HF                                                  (DAC_Control_Register_BASE+0x18)
#define     DAC_ANACTL                                                        (DAC_Control_Register_BASE+0x1C)
#define     DAC_BIAS                                                          (DAC_Control_Register_BASE+0x20)
#define     DAC_VOL                                                           (DAC_Control_Register_BASE+0x24)
#define     KT_CTL                                                            (DAC_Control_Register_BASE+0x28)

//--------------Bits Location------------------------------------------//
#define     DAC_DIGCTL_SDMNDTH_e                                              21
#define     DAC_DIGCTL_SDMNDTH_SHIFT                                          20
#define     DAC_DIGCTL_SDMNDTH_MASK                                           (0x3<<20)
#define     DAC_DIGCTL_Audio_128fs_256fs                                      19
#define     DAC_DIGCTL_DADCS                                                  18
#define     DAC_DIGCTL_DADEN                                                  17
#define     DAC_DIGCTL_DDDEN                                                  16
#define     DAC_DIGCTL_DAFIS                                                  15
#define     DAC_DIGCTL_DAFEIE                                                 14
#define     DAC_DIGCTL_DAFEDE                                                 13
#define     DAC_DIGCTL_DAFRT                                                  12
#define     DAC_DIGCTL_DALRMIX                                                10
#define     DAC_DIGCTL_DACHNUM                                                9
#define     DAC_DIGCTL_AD2DALPEN                                              8
#define     DAC_DIGCTL_LBCHNS                                                 7
#define     DAC_DIGCTL_SDMREEN                                                6
#define     DAC_DIGCTL_INTFRS_e                                               5
#define     DAC_DIGCTL_INTFRS_SHIFT                                           4
#define     DAC_DIGCTL_INTFRS_MASK                                            (0x3<<4)
#define     DAC_DIGCTL_OSRDA_e                                                3
#define     DAC_DIGCTL_OSRDA_SHIFT                                            2
#define     DAC_DIGCTL_OSRDA_MASK                                             (0x3<<2)
#define     DAC_DIGCTL_ENDITH                                                 1
#define     DAC_DIGCTL_DDEN                                                   0

#define     DAC_STAT_DAFEIP                                                   5
#define     DAC_STAT_DAFF                                                     4
#define     DAC_STAT_DAFS_e                                                   3
#define     DAC_STAT_DAFS_SHIFT                                               0
#define     DAC_STAT_DAFS_MASK                                                (0xF<<0)

#define     DAC_DAT_FIFO_DAFDAT_e                                             31
#define     DAC_DAT_FIFO_DAFDAT_SHIFT                                         8
#define     DAC_DAT_FIFO_DAFDAT_MASK                                          (0xFFFFFF<<8)

#define     PCM_BUF_CTL_PCMBEPIE                                              7
#define     PCM_BUF_CTL_PCMBFUIE                                              6
#define     PCM_BUF_CTL_PCMBHEIE                                              5
#define     PCM_BUF_CTL_PCMBHFIE                                              4

#define     PCM_BUF_STAT_PCMBEIP                                              19
#define     PCM_BUF_STAT_PCMBFIP                                              18
#define     PCM_BUF_STAT_PCMBHEIP                                             17
#define     PCM_BUF_STAT_PCMBHFIP                                             16
#define     PCM_BUF_STAT_PCMBS_e                                              12
#define     PCM_BUF_STAT_PCMBS_SHIFT                                          0
#define     PCM_BUF_STAT_PCMBS_MASK                                           (0x1FFF<<0)

#define     PCM_BUF_THRES_HE_THRESHOLD_e                                      12
#define     PCM_BUF_THRES_HE_THRESHOLD_SHIFT                                  0
#define     PCM_BUF_THRES_HE_THRESHOLD_MASK                                   (0x1FFF<<0)

#define     PCM_BUF_THRES_HF_THRESHOLD_e                                      12
#define     PCM_BUF_THRES_HF_THRESHOLD_SHIFT                                  0
#define     PCM_BUF_THRES_HF_THRESHOLD_MASK                                   (0x1FFF<<0)

#define     DAC_ANACTL_MIC12PAR                                               31
#define     DAC_ANACTL_OVLS                                                   30
#define     DAC_ANACTL_DDOVV                                                  29
#define     DAC_ANACTL_DDATPR                                                 28
#define     DAC_ANACTL_OPVROEN                                                27
#define     DAC_ANACTL_OPVROOSIQ                                              24
#define     DAC_ANACTL_OP1R2DMTR                                              23
#define     DAC_ANACTL_OP0R2DMTR                                              22
#define     DAC_ANACTL_0P0R2PAL                                               21
#define     DAC_ANACTL_0P0L2PAR                                               20
#define     DAC_ANACTL_PAIQ_e                                                 19
#define     DAC_ANACTL_PAIQ_SHIFT                                             18
#define     DAC_ANACTL_PAIQ_MASK                                              (0x3<<18)
#define     DAC_ANACTL_P2IB                                                   17
#define     DAC_ANACTL_ATP2CER                                                16
#define     DAC_ANACTL_ATP2CEL                                                15
#define     DAC_ANACTL_PAVDC                                                  14
#define     DAC_ANACTL_ATPLP2                                                 13
#define     DAC_ANACTL_ZERODT                                                 12
#define     DAC_ANACTL_OP1L2DMTL                                              11
#define     DAC_ANACTL_OP0L2DMTL                                              10
#define     DAC_ANACTL_DPBMR                                                  9
#define     DAC_ANACTL_DPBML                                                  8
#define     DAC_ANACTL_MIC02PAL                                               7
#define     DAC_ANACTL_BIASEN                                                 6
#define     DAC_ANACTL_PAOSENR                                                5
#define     DAC_ANACTL_PAOSENL                                                4
#define     DAC_ANACTL_PAENR                                                  3
#define     DAC_ANACTL_PAENL                                                  2
#define     DAC_ANACTL_DAENR                                                  1
#define     DAC_ANACTL_DAENL                                                  0

#define     DAC_BIAS_ATPLP3D                                                  25
#define     DAC_BIAS_ATPLP3                                                   24
#define     DAC_BIAS_OPINV_e                                                  21
#define     DAC_BIAS_OPINV_SHIFT                                              19
#define     DAC_BIAS_OPINV_MASK                                               (0x7<<19)
#define     DAC_BIAS_OPDAIB_e                                                 18
#define     DAC_BIAS_OPDAIB_SHIFT                                             16
#define     DAC_BIAS_OPDAIB_MASK                                              (0x7<<16)
#define     DAC_BIAS_OPDAVB_e                                                 15
#define     DAC_BIAS_OPDAVB_SHIFT                                             14
#define     DAC_BIAS_OPDAVB_MASK                                              (0x3<<14)
#define     DAC_BIAS_OPVBIB_e                                                 13
#define     DAC_BIAS_OPVBIB_SHIFT                                             12
#define     DAC_BIAS_OPVBIB_MASK                                              (0x3<<12)
#define     DAC_BIAS_OPCM1IB_e                                                11
#define     DAC_BIAS_OPCM1IB_SHIFT                                            10
#define     DAC_BIAS_OPCM1IB_MASK                                             (0x3<<10)
#define     DAC_BIAS_ALLBIAS_e                                                7
#define     DAC_BIAS_ALLBIAS_SHIFT                                            6
#define     DAC_BIAS_ALLBIAS_MASK                                             (0x3<<6)
#define     DAC_BIAS_PAIB_e                                                   5
#define     DAC_BIAS_PAIB_SHIFT                                               4
#define     DAC_BIAS_PAIB_MASK                                                (0x3<<4)
#define     DAC_BIAS_OPVROIB_e                                                3
#define     DAC_BIAS_OPVROIB_SHIFT                                            1
#define     DAC_BIAS_OPVROIB_MASK                                             (0x7<<1)
#define     DAC_BIAS_PALRSW                                                   0

#define     DAC_VOL_VOLZCTOEN                                                 21
#define     DAC_VOL_VOLZCEN                                                   20
#define     DAC_VOL_VOL_e                                                     19
#define     DAC_VOL_VOL_SHIFT                                                 0
#define     DAC_VOL_VOL_MASK                                                  (0xFFFFF<<0)

#define     KT_CTL_SKT                                                        12
#define     KT_CTL_KTFR_e                                                     11
#define     KT_CTL_KTFR_SHIFT                                                 10
#define     KT_CTL_KTFR_MASK                                                  (0x3<<10)
#define     KT_CTL_KTCNT_e                                                    9
#define     KT_CTL_KTCNT_SHIFT                                                8
#define     KT_CTL_KTCNT_MASK                                                 (0x3<<8)
#define     KT_CTL_KTVOLH_e                                                   7
#define     KT_CTL_KTVOLH_SHIFT                                               6
#define     KT_CTL_KTVOLH_MASK                                                (0x3<<6)
#define     KT_CTL_KTVOLL_e                                                   5
#define     KT_CTL_KTVOLL_SHIFT                                               0
#define     KT_CTL_KTVOLL_MASK                                                (0x3F<<0)

//--------------ADC_Control_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     ADC_Control_Register_BASE                                         0xc0051000
#define     ADC_DIGCTL                                                        (ADC_Control_Register_BASE+0x00)
#define     ADC_FIFOCTL                                                       (ADC_Control_Register_BASE+0x04)
#define     ADC_STAT                                                          (ADC_Control_Register_BASE+0x08)
#define     ADC_DAT                                                           (ADC_Control_Register_BASE+0x0C)
#define     ADC_ANACTL0                                                       (ADC_Control_Register_BASE+0x10)
#define     ADC_ANACTL1                                                       (ADC_Control_Register_BASE+0x14)
#define     ADC_BIAS                                                          (ADC_Control_Register_BASE+0x18)

//--------------Bits Location------------------------------------------//
#define     ADC_DIGCTL_ADCGC_e                                                14
#define     ADC_DIGCTL_ADCGC_SHIFT                                            11
#define     ADC_DIGCTL_ADCGC_MASK                                             (0xF<<11)
#define     ADC_DIGCTL_DMOSRS                                                 10
#define     ADC_DIGCTL_DMLEN                                                  9
#define     ADC_DIGCTL_DMREN                                                  8
#define     ADC_DIGCTL_HPFLEN                                                 7
#define     ADC_DIGCTL_HPFREN                                                 6
#define     ADC_DIGCTL_DRFS                                                   5
#define     ADC_DIGCTL_ADDEN                                                  3
#define     ADC_DIGCTL_AADEN                                                  2
#define     ADC_DIGCTL_DMICGAIN_e                                             1
#define     ADC_DIGCTL_DMICGAIN_SHIFT                                         0
#define     ADC_DIGCTL_DMICGAIN_MASK                                          (0x3<<0)

#define     ADC_FIFOCTL_ADFOS                                                 3
#define     ADC_FIFOCTL_ADFFIE                                                2
#define     ADC_FIFOCTL_ADFFDE                                                1
#define     ADC_FIFOCTL_ADFRT                                                 0

#define     ADC_STAT_ADFEF                                                    6
#define     ADC_STAT_ADFIP                                                    5
#define     ADC_STAT_ADFS_e                                                   4
#define     ADC_STAT_ADFS_SHIFT                                               0
#define     ADC_STAT_ADFS_MASK                                                (0x1F<<0)

#define     ADC_DAT_ADDAT_e                                                   31
#define     ADC_DAT_ADDAT_SHIFT                                               16
#define     ADC_DAT_ADDAT_MASK                                                (0xFFFF<<16)

#define     ADC_ANACTL0_OP1R2LPFR                                             26
#define     ADC_ANACTL0_OP1L2LPFL                                             25
#define     ADC_ANACTL0_OP0R2LPFR                                             24
#define     ADC_ANACTL0_OP0L2LPFL                                             23
#define     ADC_ANACTL0_ADLPFREN                                              22
#define     ADC_ANACTL0_ADLPFLEN                                              21
#define     ADC_ANACTL0_ADVREN                                                20
#define     ADC_ANACTL0_AUX1RPDEN                                             19
#define     ADC_ANACTL0_AUX1LPDEN                                             18
#define     ADC_ANACTL0_AUX0RPDEN                                             17
#define     ADC_ANACTL0_AUX0LPDEN                                             16
#define     ADC_ANACTL0_OP1G_e                                                15
#define     ADC_ANACTL0_OP1G_SHIFT                                            13
#define     ADC_ANACTL0_OP1G_MASK                                             (0x7<<13)
#define     ADC_ANACTL0_OP1REN                                                12
#define     ADC_ANACTL0_OP1LEN                                                11
#define     ADC_ANACTL0_AUX1REN                                               10
#define     ADC_ANACTL0_AUX1LEN                                               9
#define     ADC_ANACTL0_OP0G_e                                                8
#define     ADC_ANACTL0_OP0G_SHIFT                                            6
#define     ADC_ANACTL0_OP0G_MASK                                             (0x7<<6)
#define     ADC_ANACTL0_OP0REN                                                5
#define     ADC_ANACTL0_OP0LEN                                                4
#define     ADC_ANACTL0_AUX0REN                                               3
#define     ADC_ANACTL0_AUX0LEN                                               2
#define     ADC_ANACTL0_ADLEN                                                 1
#define     ADC_ANACTL0_ADREN                                                 0

#define     ADC_ANACTL1_MOP12LPFR                                             18
#define     ADC_ANACTL1_MOP12LPFL                                             17
#define     ADC_ANACTL1_MOP02LPFL                                             16
#define     ADC_ANACTL1_MIC1GTEN                                              15
#define     ADC_ANACTL1_MOP1EN                                                14
#define     ADC_ANACTL1_MIC1G_e                                               13
#define     ADC_ANACTL1_MIC1G_SHIFT                                           10
#define     ADC_ANACTL1_MIC1G_MASK                                            (0xF<<10)
#define     ADC_ANACTL1_MIC1OPGBEN                                            9
#define     ADC_ANACTL1_MIC1EN                                                8
#define     ADC_ANACTL1_MIC0GTEN                                              7
#define     ADC_ANACTL1_MOP0EN                                                6
#define     ADC_ANACTL1_MIC0G_e                                               5
#define     ADC_ANACTL1_MIC0G_SHIFT                                           2
#define     ADC_ANACTL1_MIC0G_MASK                                            (0xF<<2)
#define     ADC_ANACTL1_MIC0GBEN                                              1
#define     ADC_ANACTL1_MIC0EN                                                0

#define     ADC_BIAS_LPFBC_e                                                  31
#define     ADC_BIAS_LPFBC_SHIFT                                              29
#define     ADC_BIAS_LPFBC_MASK                                               (0x7<<29)
#define     ADC_BIAS_LPFBUFBC_e                                               28
#define     ADC_BIAS_LPFBUFBC_SHIFT                                           27
#define     ADC_BIAS_LPFBUFBC_MASK                                            (0x3<<27)
#define     ADC_BIAS_ADCBIAS                                                  26
#define     ADC_BIAS_MOP0BC_e                                                 19
#define     ADC_BIAS_MOP0BC_SHIFT                                             18
#define     ADC_BIAS_MOP0BC_MASK                                              (0x3<<18)
#define     ADC_BIAS_MOP1BC_e                                                 17
#define     ADC_BIAS_MOP1BC_SHIFT                                             16
#define     ADC_BIAS_MOP1BC_MASK                                              (0x3<<16)
#define     ADC_BIAS_OP0BC_e                                                  15
#define     ADC_BIAS_OP0BC_SHIFT                                              14
#define     ADC_BIAS_OP0BC_MASK                                               (0x3<<14)
#define     ADC_BIAS_OP1BC_e                                                  13
#define     ADC_BIAS_OP1BC_SHIFT                                              12
#define     ADC_BIAS_OP1BC_MASK                                               (0x3<<12)
#define     ADC_BIAS_OPBC1                                                    7£º5
#define     ADC_BIAS_OPBC23_e                                                 4
#define     ADC_BIAS_OPBC23_SHIFT                                             3
#define     ADC_BIAS_OPBC23_MASK                                              (0x3<<3)
#define     ADC_BIAS_VRDABC_e                                                 2
#define     ADC_BIAS_VRDABC_SHIFT                                             0
#define     ADC_BIAS_VRDABC_MASK                                              (0x7<<0)

//--------------I2C0-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     I2C0_BASE                                                         0xC0130000
#define     I2C0_CTL                                                          (I2C0_BASE+0x00)
#define     I2C0_CLKDIV                                                       (I2C0_BASE+0x04)
#define     I2C0_STAT                                                         (I2C0_BASE+0x08)
#define     I2C0_ADDR                                                         (I2C0_BASE+0x0c)
#define     I2C0_TXDAT                                                        (I2C0_BASE+0x10)
#define     I2C0_RXDAT                                                        (I2C0_BASE+0x14)
#define     I2C0_CMD                                                          (I2C0_BASE+0x18)
#define     I2C0_FIFOCTL                                                      (I2C0_BASE+0x1c)
#define     I2C0_FIFOSTAT                                                     (I2C0_BASE+0x20)
#define     I2C0_DATCNT                                                       (I2C0_BASE+0x24)
#define     I2C0_RCNT                                                         (I2C0_BASE+0x28)

//--------------Bits Location------------------------------------------//
#define     I2C0_CTL_IRQC                                                     10
#define     I2C0_CTL_IRQE                                                     6
#define     I2C0_CTL_EN                                                       5
#define     I2C0_CTL_GBCC_e                                                   3
#define     I2C0_CTL_GBCC_SHIFT                                               2
#define     I2C0_CTL_GBCC_MASK                                                (0x3<<2)
#define     I2C0_CTL_RB                                                       1
#define     I2C0_CTL_GRAS                                                     0

#define     I2C0_CLKDIV_CLKDIV_e                                              7
#define     I2C0_CLKDIV_CLKDIV_SHIFT                                          0
#define     I2C0_CLKDIV_CLKDIV_MASK                                           (0xFF<<0)

#define     I2C0_STAT_SRGC                                                    10
#define     I2C0_STAT_SAMB                                                    9
#define     I2C0_STAT_LBST                                                    8
#define     I2C0_STAT_TCB                                                     7
#define     I2C0_STAT_BBB                                                     6
#define     I2C0_STAT_STAD                                                    5
#define     I2C0_STAT_STPD                                                    4
#define     I2C0_STAT_IRQP                                                    2
#define     I2C0_STAT_BEB                                                     1
#define     I2C0_STAT_RACK                                                    0

#define     I2C0_ADDR_SDAD_e                                                  7
#define     I2C0_ADDR_SDAD_SHIFT                                              1
#define     I2C0_ADDR_SDAD_MASK                                               (0x7F<<1)

#define     I2C0_TXDAT_DA_e                                                   7
#define     I2C0_TXDAT_DA_SHIFT                                               0
#define     I2C0_TXDAT_DA_MASK                                                (0xFF<<0)

#define     I2C0_RXDAT_DA_e                                                   7
#define     I2C0_RXDAT_DA_SHIFT                                               0
#define     I2C0_RXDAT_DA_MASK                                                (0xFF<<0)

#define     I2C0_CMD_SECL                                                     15
#define     I2C0_CMD_WRS                                                      12
#define     I2C0_CMD_MSS                                                      11
#define     I2C0_CMD_SE                                                       10
#define     I2C0_CMD_NS                                                       9
#define     I2C0_CMD_DE                                                       8
#define     I2C0_CMD_SAS_e                                                    7
#define     I2C0_CMD_SAS_SHIFT                                                5
#define     I2C0_CMD_SAS_MASK                                                 (0x7<<5)
#define     I2C0_CMD_RBE                                                      4
#define     I2C0_CMD_AS_e                                                     3
#define     I2C0_CMD_AS_SHIFT                                                 1
#define     I2C0_CMD_AS_MASK                                                  (0x7<<1)
#define     I2C0_CMD_SBE                                                      0

#define     I2C0_FIFOCTL_TFR                                                  2
#define     I2C0_FIFOCTL_RFR                                                  1
#define     I2C0_FIFOCTL_NIB                                                  0

#define     I2C0_FIFOSTAT_RFD_e                                               15
#define     I2C0_FIFOSTAT_RFD_SHIFT                                           12
#define     I2C0_FIFOSTAT_RFD_MASK                                            (0xF<<12)
#define     I2C0_FIFOSTAT_TFD_e                                               11
#define     I2C0_FIFOSTAT_TFD_SHIFT                                           8
#define     I2C0_FIFOSTAT_TFD_MASK                                            (0xF<<8)
#define     I2C0_FIFOSTAT_WRS                                                 6
#define     I2C0_FIFOSTAT_TFF                                                 5
#define     I2C0_FIFOSTAT_TFE                                                 4
#define     I2C0_FIFOSTAT_RFF                                                 3
#define     I2C0_FIFOSTAT_RFE                                                 2
#define     I2C0_FIFOSTAT_RNB                                                 1
#define     I2C0_FIFOSTAT_CECB                                                0

#define     I2C0_DATCNT_TC_e                                                  7
#define     I2C0_DATCNT_TC_SHIFT                                              0
#define     I2C0_DATCNT_TC_MASK                                               (0xFF<<0)

#define     I2C0_RCNT_TC_e                                                    7
#define     I2C0_RCNT_TC_SHIFT                                                0
#define     I2C0_RCNT_TC_MASK                                                 (0xFF<<0)

//--------------I2C1-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     I2C1_BASE                                                         0xC0150000
#define     I2C1_CTL                                                          (I2C1_BASE+0x00)
#define     I2C1_CLKDIV                                                       (I2C1_BASE+0x04)
#define     I2C1_STAT                                                         (I2C1_BASE+0x08)
#define     I2C1_ADDR                                                         (I2C1_BASE+0x0c)
#define     I2C1_TXDAT                                                        (I2C1_BASE+0x10)
#define     I2C1_RXDAT                                                        (I2C1_BASE+0x14)
#define     I2C1_CMD                                                          (I2C1_BASE+0x18)
#define     I2C1_FIFOCTL                                                      (I2C1_BASE+0x1c)
#define     I2C1_FIFOSTAT                                                     (I2C1_BASE+0x20)
#define     I2C1_DATCNT                                                       (I2C1_BASE+0x24)
#define     I2C1_RCNT                                                         (I2C1_BASE+0x28)

//--------------Bits Location------------------------------------------//
#define     I2C1_CTL_IRQC                                                     10
#define     I2C1_CTL_IRQE                                                     6
#define     I2C1_CTL_EN                                                       5
#define     I2C1_CTL_GBCC_e                                                   3
#define     I2C1_CTL_GBCC_SHIFT                                               2
#define     I2C1_CTL_GBCC_MASK                                                (0x3<<2)
#define     I2C1_CTL_RB                                                       1
#define     I2C1_CTL_GRAS                                                     0

#define     I2C1_CLKDIV_CLKDIV_e                                              7
#define     I2C1_CLKDIV_CLKDIV_SHIFT                                          0
#define     I2C1_CLKDIV_CLKDIV_MASK                                           (0xFF<<0)

#define     I2C1_STAT_SRGC                                                    10
#define     I2C1_STAT_SAMB                                                    9
#define     I2C1_STAT_LBST                                                    8
#define     I2C1_STAT_TCB                                                     7
#define     I2C1_STAT_BBB                                                     6
#define     I2C1_STAT_STAD                                                    5
#define     I2C1_STAT_STPD                                                    4
#define     I2C1_STAT_IRQP                                                    2
#define     I2C1_STAT_BEB                                                     1
#define     I2C1_STAT_RACK                                                    0

#define     I2C1_ADDR_SDAD_e                                                  7
#define     I2C1_ADDR_SDAD_SHIFT                                              1
#define     I2C1_ADDR_SDAD_MASK                                               (0x7F<<1)

#define     I2C1_TXDAT_DA_e                                                   7
#define     I2C1_TXDAT_DA_SHIFT                                               0
#define     I2C1_TXDAT_DA_MASK                                                (0xFF<<0)

#define     I2C1_RXDAT_DA_e                                                   7
#define     I2C1_RXDAT_DA_SHIFT                                               0
#define     I2C1_RXDAT_DA_MASK                                                (0xFF<<0)

#define     I2C1_CMD_SECL                                                     15
#define     I2C1_CMD_WRS                                                      12
#define     I2C1_CMD_MSS                                                      11
#define     I2C1_CMD_SE                                                       10
#define     I2C1_CMD_NS                                                       9
#define     I2C1_CMD_DE                                                       8
#define     I2C1_CMD_SAS_e                                                    7
#define     I2C1_CMD_SAS_SHIFT                                                5
#define     I2C1_CMD_SAS_MASK                                                 (0x7<<5)
#define     I2C1_CMD_RBE                                                      4
#define     I2C1_CMD_AS_e                                                     3
#define     I2C1_CMD_AS_SHIFT                                                 1
#define     I2C1_CMD_AS_MASK                                                  (0x7<<1)
#define     I2C1_CMD_SBE                                                      0

#define     I2C1_FIFOCTL_TFR                                                  2
#define     I2C1_FIFOCTL_RFR                                                  1
#define     I2C1_FIFOCTL_NIB                                                  0

#define     I2C1_FIFOSTAT_RFD_e                                               15
#define     I2C1_FIFOSTAT_RFD_SHIFT                                           12
#define     I2C1_FIFOSTAT_RFD_MASK                                            (0xF<<12)
#define     I2C1_FIFOSTAT_TFD_e                                               11
#define     I2C1_FIFOSTAT_TFD_SHIFT                                           8
#define     I2C1_FIFOSTAT_TFD_MASK                                            (0xF<<8)
#define     I2C1_FIFOSTAT_WRS                                                 6
#define     I2C1_FIFOSTAT_TFF                                                 5
#define     I2C1_FIFOSTAT_TFE                                                 4
#define     I2C1_FIFOSTAT_RFF                                                 3
#define     I2C1_FIFOSTAT_RFE                                                 2
#define     I2C1_FIFOSTAT_RNB                                                 1
#define     I2C1_FIFOSTAT_CECB                                                0

#define     I2C1_DATCNT_TC_e                                                  7
#define     I2C1_DATCNT_TC_SHIFT                                              0
#define     I2C1_DATCNT_TC_MASK                                               (0xFF<<0)

#define     I2C1_RCNT_TC_e                                                    7
#define     I2C1_RCNT_TC_SHIFT                                                0
#define     I2C1_RCNT_TC_MASK                                                 (0xFF<<0)

//--------------UART0-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     UART0_BASE                                                        0xc01a0000
#define     UART0_CTL                                                         (UART0_BASE+0x0000)
#define     UART0_RXDAT                                                       (UART0_BASE+0x0004)
#define     UART0_TXDAT                                                       (UART0_BASE+0x0008)
#define     UART0_STA                                                         (UART0_BASE+0x000c)
#define     UART0_BR                                                          (UART0_BASE+0x0010)

//--------------Bits Location------------------------------------------//
#define     UART0_CTL_RXENABLE                                                31
#define     UART0_CTL_TXENABLE                                                30
#define     UART0_CTL_DBGSEL_e                                                25
#define     UART0_CTL_DBGSEL_SHIFT                                            24
#define     UART0_CTL_DBGSEL_MASK                                             (0x3<<24)
#define     UART0_CTL_TX_FIFO_EN                                              23
#define     UART0_CTL_RX_FIFO_EN                                              22
#define     UART0_CTL_TXAHB_DMA_SEL                                           21
#define     UART0_CTL_LBEN                                                    20
#define     UART0_CTL_TXIE                                                    19
#define     UART0_CTL_RXIE                                                    18
#define     UART0_CTL_TXDE                                                    17
#define     UART0_CTL_RXDE                                                    16
#define     UART0_CTL_EN                                                      15
#define     UART0_CTL_RXAHB_DMA_SEL                                           14
#define     UART0_CTL_RTSE                                                    13
#define     UART0_CTL_AFE                                                     12
#define     UART0_CTL_RDIC_e                                                  11
#define     UART0_CTL_RDIC_SHIFT                                              10
#define     UART0_CTL_RDIC_MASK                                               (0x3<<10)
#define     UART0_CTL_TDIC_e                                                  9
#define     UART0_CTL_TDIC_SHIFT                                              8
#define     UART0_CTL_TDIC_MASK                                               (0x3<<8)
#define     UART0_CTL_CTSE                                                    7
#define     UART0_CTL_PRS_e                                                   6
#define     UART0_CTL_PRS_SHIFT                                               4
#define     UART0_CTL_PRS_MASK                                                (0x7<<4)
#define     UART0_CTL_STPS                                                    2
#define     UART0_CTL_DWLS_e                                                  1
#define     UART0_CTL_DWLS_SHIFT                                              0
#define     UART0_CTL_DWLS_MASK                                               (0x3<<0)

#define     UART0_RXDAT_RXDAT_e                                               7
#define     UART0_RXDAT_RXDAT_SHIFT                                           0
#define     UART0_RXDAT_RXDAT_MASK                                            (0xFF<<0)

#define     UART0_TXDAT_TXDAT_e                                               7
#define     UART0_TXDAT_TXDAT_SHIFT                                           0
#define     UART0_TXDAT_TXDAT_MASK                                            (0xFF<<0)

#define     UART0_STA_PAER                                                    23
#define     UART0_STA_STER                                                    22
#define     UART0_STA_UTBB                                                    21
#define     UART0_STA_TXFL_e                                                  20
#define     UART0_STA_TXFL_SHIFT                                              16
#define     UART0_STA_TXFL_MASK                                               (0x1F<<16)
#define     UART0_STA_RXFL_e                                                  15
#define     UART0_STA_RXFL_SHIFT                                              11
#define     UART0_STA_RXFL_MASK                                               (0x1F<<11)
#define     UART0_STA_TFES                                                    10
#define     UART0_STA_RFFS                                                    9
#define     UART0_STA_RTSS                                                    8
#define     UART0_STA_CTSS                                                    7
#define     UART0_STA_TFFU                                                    6
#define     UART0_STA_RFEM                                                    5
#define     UART0_STA_RXST                                                    4
#define     UART0_STA_TFER                                                    3
#define     UART0_STA_RXER                                                    2
#define     UART0_STA_TIP                                                     1
#define     UART0_STA_RIP                                                     0

#define     UART0_BR_TXBRDIV_e                                                27
#define     UART0_BR_TXBRDIV_SHIFT                                            16
#define     UART0_BR_TXBRDIV_MASK                                             (0xFFF<<16)
#define     UART0_BR_RXBRDIV_e                                                11
#define     UART0_BR_RXBRDIV_SHIFT                                            0
#define     UART0_BR_RXBRDIV_MASK                                             (0xFFF<<0)

//--------------UART1-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     UART1_BASE                                                        0xc01b0000
#define     UART1_CTL                                                         (UART1_BASE+0x0000)
#define     UART1_RXDAT                                                       (UART1_BASE+0x0004)
#define     UART1_TXDAT                                                       (UART1_BASE+0x0008)
#define     UART1_STA                                                         (UART1_BASE+0x000c)
#define     UART1_BR                                                          (UART1_BASE+0x0010)

//--------------Bits Location------------------------------------------//
#define     UART1_CTL_RXENABLE                                                31
#define     UART1_CTL_TXENABLE                                                30
#define     UART1_CTL_DBGSEL_e                                                25
#define     UART1_CTL_DBGSEL_SHIFT                                            24
#define     UART1_CTL_DBGSEL_MASK                                             (0x3<<24)
#define     UART1_CTL_TX_FIFO_EN                                              23
#define     UART1_CTL_RX_FIFO_EN                                              22
#define     UART1_CTL_TXAHB_DMA_SEL                                           21
#define     UART1_CTL_LBEN                                                    20
#define     UART1_CTL_TXIE                                                    19
#define     UART1_CTL_RXIE                                                    18
#define     UART1_CTL_TXDE                                                    17
#define     UART1_CTL_RXDE                                                    16
#define     UART1_CTL_EN                                                      15
#define     UART1_CTL_RXAHB_DMA_SEL                                           14
#define     UART1_CTL_RTSE                                                    13
#define     UART1_CTL_AFE                                                     12
#define     UART1_CTL_RDIC_e                                                  11
#define     UART1_CTL_RDIC_SHIFT                                              10
#define     UART1_CTL_RDIC_MASK                                               (0x3<<10)
#define     UART1_CTL_TDIC_e                                                  9
#define     UART1_CTL_TDIC_SHIFT                                              8
#define     UART1_CTL_TDIC_MASK                                               (0x3<<8)
#define     UART1_CTL_CTSE                                                    7
#define     UART1_CTL_PRS_e                                                   6
#define     UART1_CTL_PRS_SHIFT                                               4
#define     UART1_CTL_PRS_MASK                                                (0x7<<4)
#define     UART1_CTL_STPS                                                    2
#define     UART1_CTL_DWLS_e                                                  1
#define     UART1_CTL_DWLS_SHIFT                                              0
#define     UART1_CTL_DWLS_MASK                                               (0x3<<0)

#define     UART1_RXDAT_RXDAT_e                                               7
#define     UART1_RXDAT_RXDAT_SHIFT                                           0
#define     UART1_RXDAT_RXDAT_MASK                                            (0xFF<<0)

#define     UART1_TXDAT_TXDAT_e                                               7
#define     UART1_TXDAT_TXDAT_SHIFT                                           0
#define     UART1_TXDAT_TXDAT_MASK                                            (0xFF<<0)

#define     UART1_STA_PAER                                                    23
#define     UART1_STA_STER                                                    22
#define     UART1_STA_UTBB                                                    21
#define     UART1_STA_TXFL_e                                                  20
#define     UART1_STA_TXFL_SHIFT                                              16
#define     UART1_STA_TXFL_MASK                                               (0x1F<<16)
#define     UART1_STA_RXFL_e                                                  15
#define     UART1_STA_RXFL_SHIFT                                              11
#define     UART1_STA_RXFL_MASK                                               (0x1F<<11)
#define     UART1_STA_TFES                                                    10
#define     UART1_STA_RFFS                                                    9
#define     UART1_STA_RTSS                                                    8
#define     UART1_STA_CTSS                                                    7
#define     UART1_STA_TFFU                                                    6
#define     UART1_STA_RFEM                                                    5
#define     UART1_STA_RXST                                                    4
#define     UART1_STA_TFER                                                    3
#define     UART1_STA_RXER                                                    2
#define     UART1_STA_TIP                                                     1
#define     UART1_STA_RIP                                                     0

#define     UART1_BR_TXBRDIV_e                                                27
#define     UART1_BR_TXBRDIV_SHIFT                                            16
#define     UART1_BR_TXBRDIV_MASK                                             (0xFFF<<16)
#define     UART1_BR_RXBRDIV_e                                                11
#define     UART1_BR_RXBRDIV_SHIFT                                            0
#define     UART1_BR_RXBRDIV_MASK                                             (0xFFF<<0)

//--------------SPI-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SPI_BASE                                                          0xc0110000
#define     SPI_CTL                                                           (SPI_BASE+0x0000)
#define     SPI_STA                                                           (SPI_BASE+0x0004)
#define     SPI_TXDAT                                                         (SPI_BASE+0x0008)
#define     SPI_RXDAT                                                         (SPI_BASE+0x000C)
#define     SPI_BC                                                            (SPI_BASE+0x0010)

//--------------Bits Location------------------------------------------//
#define     SPI_CTL_CLKSEL                                                    31
#define     SPI_CTL_NO_CRC_RAND_RXEN                                          29
#define     SPI_CTL_SPI_MODE_SELECT                                           28
#define     SPI_CTL_DUAL_QUAD_SELECT                                          27
#define     SPI_CTL_RX_WRITE_SEL                                              26
#define     SPI_CTL_TIME_OUT_CTRL_e                                           25
#define     SPI_CTL_TIME_OUT_CTRL_SHIFT                                       24
#define     SPI_CTL_TIME_OUT_CTRL_MASK                                        (0x3<<24)
#define     SPI_CTL_CRC_ERROR_TIME_e                                          23
#define     SPI_CTL_CRC_ERROR_TIME_SHIFT                                      22
#define     SPI_CTL_CRC_ERROR_TIME_MASK                                       (0x3<<22)
#define     SPI_CTL_CRC_IRQ_EN                                                21
#define     SPI_CTL_CRC_EN                                                    20
#define     SPI_CTL_SPI_DELAY_e                                               19
#define     SPI_CTL_SPI_DELAY_SHIFT                                           16
#define     SPI_CTL_SPI_DELAY_MASK                                            (0xF<<16)
#define     SPI_CTL_RAND_PAUSE                                                15
#define     SPI_CTL_RAND_SEL                                                  14
#define     SPI_CTL_RAND_TXEN                                                 13
#define     SPI_CTL_RAND_RXEN                                                 12
#define     SPI_CTL_SPI_IO_MODE_e                                             11
#define     SPI_CTL_SPI_IO_MODE_SHIFT                                         10
#define     SPI_CTL_SPI_IO_MODE_MASK                                          (0x3<<10)
#define     SPI_CTL_SPI_3WIRE                                                 9
#define     SPI_CTL_SPI_REQ                                                   8
#define     SPI_CTL_SPI_TDRQ_EN                                               7
#define     SPI_CTL_SPI_RDRQ_EN                                               6
#define     SPI_CTL_SPI_TX_FIFO_EN                                            5
#define     SPI_CTL_SPI_RX_FIFO_EN                                            4
#define     SPI_CTL_SPI_SS                                                    3
#define     SPI_CTL_SPI_LOOP                                                  2
#define     SPI_CTL_SPI_WR_e                                                  1
#define     SPI_CTL_SPI_WR_SHIFT                                              0
#define     SPI_CTL_SPI_WR_MASK                                               (0x3<<0)

#define     SPI_STA_SPI_READY                                                 8
#define     SPI_STA_CRC_PD                                                    7
#define     SPI_STA_SPI_BUSY                                                  6
#define     SPI_STA_SPI_TXFU                                                  5
#define     SPI_STA_SPI_TXEM                                                  4
#define     SPI_STA_SPI_RXFU                                                  3
#define     SPI_STA_SPI_RXEM                                                  2
#define     SPI_STA_CRC_ERROR                                                 1

#define     SPI_TXDAT_SPI_TXDAT_e                                             7
#define     SPI_TXDAT_SPI_TXDAT_SHIFT                                         0
#define     SPI_TXDAT_SPI_TXDAT_MASK                                          (0xFF<<0)

#define     SPI_RXDAT_SPI_RXDAT_e                                             7
#define     SPI_RXDAT_SPI_RXDAT_SHIFT                                         0
#define     SPI_RXDAT_SPI_RXDAT_MASK                                          (0xFF<<0)

#define     SPI_BC_REMAIN_CNT_e                                               31
#define     SPI_BC_REMAIN_CNT_SHIFT                                           16
#define     SPI_BC_REMAIN_CNT_MASK                                            (0xFFFF<<16)
#define     SPI_BC_SPI_BC_e                                                   15
#define     SPI_BC_SPI_BC_SHIFT                                               0
#define     SPI_BC_SPI_BC_MASK                                                (0xFFFF<<0)

//--------------SPI1-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SPI1_BASE                                                         0xC00F0000
#define     SPI1_CTL                                                          (SPI1_BASE+0x0000)
#define     SPI1_STA                                                          (SPI1_BASE+0x0004)
#define     SPI1_TXDAT                                                        (SPI1_BASE+0x0008)
#define     SPI1_RXDAT                                                        (SPI1_BASE+0x000C)
#define     SPI1_BC                                                           (SPI1_BASE+0x0010)

//--------------Bits Location------------------------------------------//
#define     SPI1_CTL_CLKSEL                                                   31
#define     SPI1_CTL_FWS                                                      30
#define     SPI1_CTL_SPI_MODE_SELECT_e                                        29
#define     SPI1_CTL_SPI_MODE_SELECT_SHIFT                                    28
#define     SPI1_CTL_SPI_MODE_SELECT_MASK                                     (0x3<<28)
#define     SPI1_CTL_MSS                                                      22
#define     SPI1_CTL_MSB                                                      21
#define     SPI1_CTL_RX_WRITE_SEL                                             20
#define     SPI1_CTL_SPI_DELAY_e                                              19
#define     SPI1_CTL_SPI_DELAY_SHIFT                                          16
#define     SPI1_CTL_SPI_DELAY_MASK                                           (0xF<<16)
#define     SPI1_CTL_SPI_TDRQ_EN                                              11
#define     SPI1_CTL_SPI_RDRQ_EN                                              10
#define     SPI1_CTL_SPI_TIRQ_EN                                              9
#define     SPI1_CTL_SPI_RIRQ_EN                                              8
#define     SPI1_CTL_SPI_TX_FIFO_EN                                           5
#define     SPI1_CTL_SPI_RX_FIFO_EN                                           4
#define     SPI1_CTL_SPI_SS                                                   3
#define     SPI1_CTL_SPI_LOOP                                                 2
#define     SPI1_CTL_SPI_WR_e                                                 1
#define     SPI1_CTL_SPI_WR_SHIFT                                             0
#define     SPI1_CTL_SPI_WR_MASK                                              (0x3<<0)

#define     SPI1_STA_TFWO                                                     11
#define     SPI1_STA_RFWO                                                     9
#define     SPI1_STA_TFRO                                                     8
#define     SPI1_STA_SPI_RXFU                                                 7
#define     SPI1_STA_SPI_RXEM                                                 6
#define     SPI1_STA_SPI_TXFU                                                 5
#define     SPI1_STA_SPI_TXEM                                                 4
#define     SPI1_STA_SPI_TIRQ_PD                                              3
#define     SPI1_STA_SPI_RIRQ_PD                                              2
#define     SPI1_STA_SPI_BUSY                                                 0

#define     SPI1_TXDAT_SPI_TXDAT_e                                            31
#define     SPI1_TXDAT_SPI_TXDAT_SHIFT                                        0
#define     SPI1_TXDAT_SPI_TXDAT_MASK                                         (0xFFFFFFFF<<0)

#define     SPI1_RXDAT_SPI_RXDAT_e                                            31
#define     SPI1_RXDAT_SPI_RXDAT_SHIFT                                        0
#define     SPI1_RXDAT_SPI_RXDAT_MASK                                         (0xFFFFFFFF<<0)

#define     SPI1_BC_SPI_BC_e                                                  12
#define     SPI1_BC_SPI_BC_SHIFT                                              0
#define     SPI1_BC_SPI_BC_MASK                                               (0x1FFF<<0)

//--------------IRC-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     IRC_BASE                                                          0xc01c0000
#define     IRC_CTL                                                           (IRC_BASE+0x0050)
#define     IRC_STA                                                           (IRC_BASE+0x0054)
#define     IRC_CC                                                            (IRC_BASE+0x0058)
#define     IRC_KDC                                                           (IRC_BASE+0x005C)

//--------------Bits Location------------------------------------------//
#define     IRC_CTL_DBB_EN                                                    16
#define     IRC_CTL_DBC_e                                                     15
#define     IRC_CTL_DBC_SHIFT                                                 4
#define     IRC_CTL_DBC_MASK                                                  (0xFFF<<4)
#define     IRC_CTL_IRE                                                       3
#define     IRC_CTL_IIE                                                       2
#define     IRC_CTL_ICMS_e                                                    1
#define     IRC_CTL_ICMS_SHIFT                                                0
#define     IRC_CTL_ICMS_MASK                                                 (0x3<<0)

#define     IRC_STA_UCMP                                                      6
#define     IRC_STA_KDCM                                                      5
#define     IRC_STA_RCD                                                       4
#define     IRC_STA_IIP                                                       2
#define     IRC_STA_IREP                                                      0

#define     IRC_CC_CCRCV_e                                                    31
#define     IRC_CC_CCRCV_SHIFT                                                16
#define     IRC_CC_CCRCV_MASK                                                 (0xFFFF<<16)
#define     IRC_CC_ICCC_e                                                     15
#define     IRC_CC_ICCC_SHIFT                                                 0
#define     IRC_CC_ICCC_MASK                                                  (0xFFFF<<0)

#define     IRC_KDC_IKDC_e                                                    15
#define     IRC_KDC_IKDC_SHIFT                                                0
#define     IRC_KDC_IKDC_MASK                                                 (0xFFFF<<0)

//--------------PWM-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     PWM_BASE                                                          0xC00D0000
#define     PWM0_CTL                                                          (PWM_BASE+0x00)
#define     PWM1_CTL                                                          (PWM_BASE+0x04)
#define     PWM2_CTL                                                          (PWM_BASE+0x08)
#define     PWM3_CTL                                                          (PWM_BASE+0x0C)
#define     PWM4_CTL                                                          (PWM_BASE+0x10)
#define     PWM5_CTL                                                          (PWM_BASE+0x14)
#define     PWM6_CTL                                                          (PWM_BASE+0x18)
#define     PWM7_CTL                                                          (PWM_BASE+0x1C)
#define     PWM8_CTL                                                          (PWM_BASE+0x20)

//--------------Bits Location------------------------------------------//
#define     PWM0_CTL_PWM_EN                                                   28
#define     PWM0_CTL_POL_SEL                                                  27
#define     PWM0_CTL_Mode_SEL                                                 26
#define     PWM0_CTL_Q_e                                                      25
#define     PWM0_CTL_Q_SHIFT                                                  24
#define     PWM0_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM0_CTL_H_e                                                      23
#define     PWM0_CTL_H_SHIFT                                                  16
#define     PWM0_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM0_CTL_L_e                                                      15
#define     PWM0_CTL_L_SHIFT                                                  8
#define     PWM0_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM0_CTL_DUTY_e                                                   7
#define     PWM0_CTL_DUTY_SHIFT                                               0
#define     PWM0_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM1_CTL_PWM_EN                                                   28
#define     PWM1_CTL_POL_SEL                                                  27
#define     PWM1_CTL_Mode_SEL                                                 26
#define     PWM1_CTL_Q_e                                                      25
#define     PWM1_CTL_Q_SHIFT                                                  24
#define     PWM1_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM1_CTL_H_e                                                      23
#define     PWM1_CTL_H_SHIFT                                                  16
#define     PWM1_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM1_CTL_L_e                                                      15
#define     PWM1_CTL_L_SHIFT                                                  8
#define     PWM1_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM1_CTL_DUTY_e                                                   7
#define     PWM1_CTL_DUTY_SHIFT                                               0
#define     PWM1_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM2_CTL_PWM_EN                                                   28
#define     PWM2_CTL_POL_SEL                                                  27
#define     PWM2_CTL_Mode_SEL                                                 26
#define     PWM2_CTL_Q_e                                                      25
#define     PWM2_CTL_Q_SHIFT                                                  24
#define     PWM2_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM2_CTL_H_e                                                      23
#define     PWM2_CTL_H_SHIFT                                                  16
#define     PWM2_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM2_CTL_L_e                                                      15
#define     PWM2_CTL_L_SHIFT                                                  8
#define     PWM2_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM2_CTL_DUTY_e                                                   7
#define     PWM2_CTL_DUTY_SHIFT                                               0
#define     PWM2_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM3_CTL_PWM_EN                                                   28
#define     PWM3_CTL_POL_SEL                                                  27
#define     PWM3_CTL_Mode_SEL                                                 26
#define     PWM3_CTL_Q_e                                                      25
#define     PWM3_CTL_Q_SHIFT                                                  24
#define     PWM3_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM3_CTL_H_e                                                      23
#define     PWM3_CTL_H_SHIFT                                                  16
#define     PWM3_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM3_CTL_L_e                                                      15
#define     PWM3_CTL_L_SHIFT                                                  8
#define     PWM3_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM3_CTL_DUTY_e                                                   7
#define     PWM3_CTL_DUTY_SHIFT                                               0
#define     PWM3_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM4_CTL_PWM_EN                                                   28
#define     PWM4_CTL_POL_SEL                                                  27
#define     PWM4_CTL_Mode_SEL                                                 26
#define     PWM4_CTL_Q_e                                                      25
#define     PWM4_CTL_Q_SHIFT                                                  24
#define     PWM4_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM4_CTL_H_e                                                      23
#define     PWM4_CTL_H_SHIFT                                                  16
#define     PWM4_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM4_CTL_L_e                                                      15
#define     PWM4_CTL_L_SHIFT                                                  8
#define     PWM4_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM4_CTL_DUTY_e                                                   7
#define     PWM4_CTL_DUTY_SHIFT                                               0
#define     PWM4_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM5_CTL_PWM_EN                                                   28
#define     PWM5_CTL_POL_SEL                                                  27
#define     PWM5_CTL_Mode_SEL                                                 26
#define     PWM5_CTL_Q_e                                                      25
#define     PWM5_CTL_Q_SHIFT                                                  24
#define     PWM5_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM5_CTL_H_e                                                      23
#define     PWM5_CTL_H_SHIFT                                                  16
#define     PWM5_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM5_CTL_L_e                                                      15
#define     PWM5_CTL_L_SHIFT                                                  8
#define     PWM5_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM5_CTL_DUTY_e                                                   7
#define     PWM5_CTL_DUTY_SHIFT                                               0
#define     PWM5_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM6_CTL_PWM_EN                                                   28
#define     PWM6_CTL_POL_SEL                                                  27
#define     PWM6_CTL_Mode_SEL                                                 26
#define     PWM6_CTL_Q_e                                                      25
#define     PWM6_CTL_Q_SHIFT                                                  24
#define     PWM6_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM6_CTL_H_e                                                      23
#define     PWM6_CTL_H_SHIFT                                                  16
#define     PWM6_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM6_CTL_L_e                                                      15
#define     PWM6_CTL_L_SHIFT                                                  8
#define     PWM6_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM6_CTL_DUTY_e                                                   7
#define     PWM6_CTL_DUTY_SHIFT                                               0
#define     PWM6_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM7_CTL_PWM_EN                                                   28
#define     PWM7_CTL_POL_SEL                                                  27
#define     PWM7_CTL_Mode_SEL                                                 26
#define     PWM7_CTL_Q_e                                                      25
#define     PWM7_CTL_Q_SHIFT                                                  24
#define     PWM7_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM7_CTL_H_e                                                      23
#define     PWM7_CTL_H_SHIFT                                                  16
#define     PWM7_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM7_CTL_L_e                                                      15
#define     PWM7_CTL_L_SHIFT                                                  8
#define     PWM7_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM7_CTL_DUTY_e                                                   7
#define     PWM7_CTL_DUTY_SHIFT                                               0
#define     PWM7_CTL_DUTY_MASK                                                (0xFF<<0)

#define     PWM8_CTL_PWM_EN                                                   28
#define     PWM8_CTL_POL_SEL                                                  27
#define     PWM8_CTL_Mode_SEL                                                 26
#define     PWM8_CTL_Q_e                                                      25
#define     PWM8_CTL_Q_SHIFT                                                  24
#define     PWM8_CTL_Q_MASK                                                   (0x3<<24)
#define     PWM8_CTL_H_e                                                      23
#define     PWM8_CTL_H_SHIFT                                                  16
#define     PWM8_CTL_H_MASK                                                   (0xFF<<16)
#define     PWM8_CTL_L_e                                                      15
#define     PWM8_CTL_L_SHIFT                                                  8
#define     PWM8_CTL_L_MASK                                                   (0xFF<<8)
#define     PWM8_CTL_DUTY_e                                                   7
#define     PWM8_CTL_DUTY_SHIFT                                               0
#define     PWM8_CTL_DUTY_MASK                                                (0xFF<<0)

//--------------LCDC-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     LCDC_BASE                                                         0xc0180000
#define     LCD_CTL                                                           (LCDC_BASE+0x0000)
#define     LCD_CLKCTL                                                        (LCDC_BASE+0x0004)
#define     EXTMEM_CTL                                                        (LCDC_BASE+0x0008)
#define     EXTMEM_CLKCTL                                                     (LCDC_BASE+0x000c)
#define     EXTMEM_DATA                                                       (LCDC_BASE+0x0010)
#define     LCD_IF_PCS                                                        (LCDC_BASE+0x0014)

//--------------Bits Location------------------------------------------//
#define     LCD_CTL_LCDFI                                                     31
#define     LCD_CTL_PC_EN                                                     30
#define     LCD_CTL_FOVF                                                      17
#define     LCD_CTL_FIFOET                                                    10
#define     LCD_CTL_EMDE                                                      7
#define     LCD_CTL_FORMATS                                                   4
#define     LCD_CTL_SEQ                                                       3
#define     LCD_CTL_MLS                                                       2
#define     LCD_CTL_C86                                                       1
#define     LCD_CTL_EN                                                        0

#define     LCD_CLKCTL_CLKHDU_e                                               21
#define     LCD_CLKCTL_CLKHDU_SHIFT                                           16
#define     LCD_CLKCTL_CLKHDU_MASK                                            (0x3F<<16)
#define     LCD_CLKCTL_CLKL2DU_e                                              13
#define     LCD_CLKCTL_CLKL2DU_SHIFT                                          8
#define     LCD_CLKCTL_CLKL2DU_MASK                                           (0x3F<<8)
#define     LCD_CLKCTL_CLKLDU_e                                               5
#define     LCD_CLKCTL_CLKLDU_SHIFT                                           0
#define     LCD_CLKCTL_CLKLDU_MASK                                            (0x3F<<0)

#define     EXTMEM_CTL_CESEL_e                                                31
#define     EXTMEM_CTL_CESEL_SHIFT                                            29
#define     EXTMEM_CTL_CESEL_MASK                                             (0x7<<29)
#define     EXTMEM_CTL_IFSEL                                                  8
#define     EXTMEM_CTL_RS                                                     0

#define     EXTMEM_CLKCTL_EXCLKH_e                                            21
#define     EXTMEM_CLKCTL_EXCLKH_SHIFT                                        16
#define     EXTMEM_CLKCTL_EXCLKH_MASK                                         (0x3F<<16)
#define     EXTMEM_CLKCTL_EXCL2KL_e                                           13
#define     EXTMEM_CLKCTL_EXCL2KL_SHIFT                                       8
#define     EXTMEM_CLKCTL_EXCL2KL_MASK                                        (0x3F<<8)
#define     EXTMEM_CLKCTL_EXCLKL_e                                            5
#define     EXTMEM_CLKCTL_EXCLKL_SHIFT                                        0
#define     EXTMEM_CLKCTL_EXCLKL_MASK                                         (0x3F<<0)

#define     EXTMEM_DATA_EXT_DATAH_e                                           15
#define     EXTMEM_DATA_EXT_DATAH_SHIFT                                       8
#define     EXTMEM_DATA_EXT_DATAH_MASK                                        (0xFF<<8)
#define     EXTMEM_DATA_EXT_DATAL_e                                           7
#define     EXTMEM_DATA_EXT_DATAL_SHIFT                                       0
#define     EXTMEM_DATA_EXT_DATAL_MASK                                        (0xFF<<0)

#define     LCD_IF_PCS_PCS_e                                                  7
#define     LCD_IF_PCS_PCS_SHIFT                                              0
#define     LCD_IF_PCS_PCS_MASK                                               (0xFF<<0)

//--------------SEG_SREEN-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SEG_SREEN_BASE                                                    0xc0190000
#define     SEG_DISP_CTL                                                      (SEG_SREEN_BASE+0x0000)
#define     SEG_DISP_DATA0                                                    (SEG_SREEN_BASE+0x0004)
#define     SEG_DISP_DATA1                                                    (SEG_SREEN_BASE+0x0008)
#define     SEG_DISP_DATA2                                                    (SEG_SREEN_BASE+0x000C)
#define     SEG_DISP_DATA3                                                    (SEG_SREEN_BASE+0x0010)
#define     SEG_DISP_DATA4                                                    (SEG_SREEN_BASE+0x0014)
#define     SEG_DISP_DATA5                                                    (SEG_SREEN_BASE+0x0018)
#define     SEG_RC_EN                                                         (SEG_SREEN_BASE+0x001c)
#define     SEG_BIAS_EN                                                       (SEG_SREEN_BASE+0x0020)

//--------------Bits Location------------------------------------------//
#define     SEG_DISP_CTL_LCD_POWER                                            31
#define     SEG_DISP_CTL_LED_COM_DZ_e                                         10
#define     SEG_DISP_CTL_LED_COM_DZ_SHIFT                                     8
#define     SEG_DISP_CTL_LED_COM_DZ_MASK                                      (0x7<<8)
#define     SEG_DISP_CTL_SEGOFF                                               7
#define     SEG_DISP_CTL_LCD_OUT_EN                                           5
#define     SEG_DISP_CTL_REFRSH                                               4
#define     SEG_DISP_CTL_MODE_SEL_e                                           3
#define     SEG_DISP_CTL_MODE_SEL_SHIFT                                       0
#define     SEG_DISP_CTL_MODE_SEL_MASK                                        (0xF<<0)

#define     SEG_DISP_DATA0_DISP_DATA0_BYTE3_e                                 31
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE3_SHIFT                             24
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE3_MASK                              (0xFF<<24)
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE2_e                                 23
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE2_SHIFT                             16
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE2_MASK                              (0xFF<<16)
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE1_e                                 15
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE1_SHIFT                             8
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE1_MASK                              (0xFF<<8)
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE0_e                                 7
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE0_SHIFT                             0
#define     SEG_DISP_DATA0_DISP_DATA0_BYTE0_MASK                              (0xFF<<0)

#define     SEG_DISP_DATA1_COM1_byte3_e                                       31
#define     SEG_DISP_DATA1_COM1_byte3_SHIFT                                   24
#define     SEG_DISP_DATA1_COM1_byte3_MASK                                    (0xFF<<24)
#define     SEG_DISP_DATA1_COM1_byte2_e                                       23
#define     SEG_DISP_DATA1_COM1_byte2_SHIFT                                   16
#define     SEG_DISP_DATA1_COM1_byte2_MASK                                    (0xFF<<16)
#define     SEG_DISP_DATA1_COM1_byte1_e                                       15
#define     SEG_DISP_DATA1_COM1_byte1_SHIFT                                   8
#define     SEG_DISP_DATA1_COM1_byte1_MASK                                    (0xFF<<8)
#define     SEG_DISP_DATA1_COM1_byte0_e                                       7
#define     SEG_DISP_DATA1_COM1_byte0_SHIFT                                   0
#define     SEG_DISP_DATA1_COM1_byte0_MASK                                    (0xFF<<0)

#define     SEG_DISP_DATA2_COM2_word_e                                        23
#define     SEG_DISP_DATA2_COM2_word_SHIFT                                    0
#define     SEG_DISP_DATA2_COM2_word_MASK                                     (0xFFFFFF<<0)

#define     SEG_DISP_DATA3_COM3_word_e                                        23
#define     SEG_DISP_DATA3_COM3_word_SHIFT                                    0
#define     SEG_DISP_DATA3_COM3_word_MASK                                     (0xFFFFFF<<0)

#define     SEG_DISP_DATA4_COM4_word_e                                        23
#define     SEG_DISP_DATA4_COM4_word_SHIFT                                    0
#define     SEG_DISP_DATA4_COM4_word_MASK                                     (0xFFFFFF<<0)

#define     SEG_DISP_DATA5_COM5_word_e                                        23
#define     SEG_DISP_DATA5_COM5_word_SHIFT                                    0
#define     SEG_DISP_DATA5_COM5_word_MASK                                     (0xFFFFFF<<0)

#define     SEG_RC_EN_LED_SEG7                                                7
#define     SEG_RC_EN_LED_SEG6                                                6
#define     SEG_RC_EN_LED_SEG5                                                5
#define     SEG_RC_EN_LED_SEG4                                                4
#define     SEG_RC_EN_LED_SEG3                                                3
#define     SEG_RC_EN_LED_SEG2                                                2
#define     SEG_RC_EN_LED_SEG1                                                1
#define     SEG_RC_EN_LED_SEG0                                                0

#define     SEG_BIAS_EN_LED_SEG_ALL_EN                                        4
#define     SEG_BIAS_EN_LED_Cathode_Anode_Mode                                3
#define     SEG_BIAS_EN_LED_SEG_BIAS_e                                        2
#define     SEG_BIAS_EN_LED_SEG_BIAS_SHIFT                                    0
#define     SEG_BIAS_EN_LED_SEG_BIAS_MASK                                     (0x7<<0)


//--------------GPIO_MFP-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     GPIO_MFP_BASE                                                     0xC0090000
#define     JTAG_CTL                                                          (GPIO_MFP_BASE+0x0000)
#define     GPIO0_CTL                                                         (GPIO_MFP_BASE+0x000C)
#define     GPIO1_CTL                                                         (GPIO_MFP_BASE+0x0010)
#define     GPIO2_CTL                                                         (GPIO_MFP_BASE+0x0014)
#define     GPIO3_CTL                                                         (GPIO_MFP_BASE+0x0018)
#define     GPIO4_CTL                                                         (GPIO_MFP_BASE+0x001C)
#define     GPIO5_CTL                                                         (GPIO_MFP_BASE+0x0020)
#define     GPIO6_CTL                                                         (GPIO_MFP_BASE+0x0024)
#define     GPIO7_CTL                                                         (GPIO_MFP_BASE+0x0028)
#define     GPIO8_CTL                                                         (GPIO_MFP_BASE+0x002C)
#define     GPIO9_CTL                                                         (GPIO_MFP_BASE+0x0030)
#define     GPIO10_CTL                                                        (GPIO_MFP_BASE+0x0034)
#define     GPIO11_CTL                                                        (GPIO_MFP_BASE+0x0038)
#define     GPIO12_CTL                                                        (GPIO_MFP_BASE+0x003C)
#define     GPIO13_CTL                                                        (GPIO_MFP_BASE+0x0040)
#define     GPIO14_CTL                                                        (GPIO_MFP_BASE+0x0044)
#define     GPIO15_CTL                                                        (GPIO_MFP_BASE+0x0048)
#define     GPIO16_CTL                                                        (GPIO_MFP_BASE+0x004C)
#define     GPIO17_CTL                                                        (GPIO_MFP_BASE+0x0050)
#define     GPIO18_CTL                                                        (GPIO_MFP_BASE+0x0054)
#define     GPIO19_CTL                                                        (GPIO_MFP_BASE+0x0058)
#define     GPIO20_CTL                                                        (GPIO_MFP_BASE+0x005C)
#define     GPIO21_CTL                                                        (GPIO_MFP_BASE+0x0060)
#define     GPIO22_CTL                                                        (GPIO_MFP_BASE+0x0064)
#define     GPIO23_CTL                                                        (GPIO_MFP_BASE+0x0068)
#define     GPIO24_CTL                                                        (GPIO_MFP_BASE+0x006C)
#define     GPIO25_CTL                                                        (GPIO_MFP_BASE+0x0070)
#define     GPIO26_CTL                                                        (GPIO_MFP_BASE+0x0074)
#define     GPIO27_CTL                                                        (GPIO_MFP_BASE+0x0078)
#define     GPIO28_CTL                                                        (GPIO_MFP_BASE+0x007C)
#define     GPIO29_CTL                                                        (GPIO_MFP_BASE+0x0080)
#define     GPIO30_CTL                                                        (GPIO_MFP_BASE+0x0084)
#define     GPIO31_CTL                                                        (GPIO_MFP_BASE+0x0088)
#define     GPIO32_CTL                                                        (GPIO_MFP_BASE+0x008C)
#define     GPIO33_CTL                                                        (GPIO_MFP_BASE+0x0090)
#define     GPIO34_CTL                                                        (GPIO_MFP_BASE+0x0094)
#define     GPIO35_CTL                                                        (GPIO_MFP_BASE+0x0098)
#define     GPIO36_CTL                                                        (GPIO_MFP_BASE+0x009C)
#define     GPIO37_CTL                                                        (GPIO_MFP_BASE+0x00A0)
#define     GPIO38_CTL                                                        (GPIO_MFP_BASE+0x00A4)
#define     GPIO39_CTL                                                        (GPIO_MFP_BASE+0x00A8)
#define     GPIO40_CTL                                                        (GPIO_MFP_BASE+0x00AC)
#define     GPIO41_CTL                                                        (GPIO_MFP_BASE+0x00B0)
#define     GPIO42_CTL                                                        (GPIO_MFP_BASE+0x00B4)
#define     GPIO43_CTL                                                        (GPIO_MFP_BASE+0x00B8)
#define     GPIO44_CTL                                                        (GPIO_MFP_BASE+0x00BC)
#define     GPIO45_CTL                                                        (GPIO_MFP_BASE+0x00C0)
#define     GPIO46_CTL                                                        (GPIO_MFP_BASE+0x00C4)
#define     GPIO47_CTL                                                        (GPIO_MFP_BASE+0x00C8)
#define     GPIO48_CTL                                                        (GPIO_MFP_BASE+0x00CC)
#define     GPIO49_CTL                                                        (GPIO_MFP_BASE+0x00D0)
#define     GPIO50_CTL                                                        (GPIO_MFP_BASE+0x00D4)
#define     GPIO51_CTL                                                        (GPIO_MFP_BASE+0x00D8)
#define     GPIO_ODAT0                                                        (GPIO_MFP_BASE+0x0100)
#define     GPIO_ODAT1                                                        (GPIO_MFP_BASE+0x0104)
#define     GPIO_BSR0                                                         (GPIO_MFP_BASE+0x0108)
#define     GPIO_BSR1                                                         (GPIO_MFP_BASE+0x010C)
#define     GPIO_BRR0                                                         (GPIO_MFP_BASE+0x0110)
#define     GPIO_BRR1                                                         (GPIO_MFP_BASE+0x0114)
#define     GPIO_IDAT0                                                        (GPIO_MFP_BASE+0x0118)
#define     GPIO_IDAT1                                                        (GPIO_MFP_BASE+0x011C)
#define     GPIO_PD0                                                          (GPIO_MFP_BASE+0x0120)
#define     GPIO_PD1                                                          (GPIO_MFP_BASE+0x0124)
#define     WIO0_CTL                                                          (GPIO_MFP_BASE+0x0140)
#define     WIO1_CTL                                                          (GPIO_MFP_BASE+0x0144)
#define     DEBUGSEL                                                          (GPIO_MFP_BASE+0x0180)
#define     DEBUGIE0                                                          (GPIO_MFP_BASE+0x0184)
#define     DEBUGIE1                                                          (GPIO_MFP_BASE+0x0188)
#define     DEBUGOE0                                                          (GPIO_MFP_BASE+0x018C)
#define     DEBUGOE1                                                          (GPIO_MFP_BASE+0x0190)

//--------------Bits Location------------------------------------------//
#define     JTAG_CTL_EJTAGEN                                                  4
#define     JTAG_CTL_EJTAGMAP_e                                               1
#define     JTAG_CTL_EJTAGMAP_SHIFT                                           0
#define     JTAG_CTL_EJTAGMAP_MASK                                            (0x3<<0)

#define     GPIO0_CTL_GPIO_INTC_MSK                                           25
#define     GPIO0_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO0_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO0_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO0_CTL_GPIO_INTCEN                                             20
#define     GPIO0_CTL_PADDRV_e                                                14
#define     GPIO0_CTL_PADDRV_SHIFT                                            12
#define     GPIO0_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO0_CTL_10KPUEN                                                 11
#define     GPIO0_CTL_GPIO100KPDEN                                            9
#define     GPIO0_CTL_GPIO50KPUEN                                             8
#define     GPIO0_CTL_GPIOINEN                                                7
#define     GPIO0_CTL_GPIOOUTEN                                               6
#define     GPIO0_CTL_SMIT                                                    5
#define     GPIO0_CTL_MFP_e                                                   3
#define     GPIO0_CTL_MFP_SHIFT                                               0
#define     GPIO0_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO1_CTL_GPIO_INTC_MSK                                           25
#define     GPIO1_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO1_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO1_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO1_CTL_GPIO_INTCEN                                             20
#define     GPIO1_CTL_PADDRV_e                                                14
#define     GPIO1_CTL_PADDRV_SHIFT                                            12
#define     GPIO1_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO1_CTL_10KPUEN                                                 11
#define     GPIO1_CTL_GPIO100KPDEN                                            9
#define     GPIO1_CTL_GPIO100KPUEN                                            8
#define     GPIO1_CTL_GPIOINEN                                                7
#define     GPIO1_CTL_GPIOOUTEN                                               6
#define     GPIO1_CTL_SMIT                                                    5
#define     GPIO1_CTL_MFP_e                                                   3
#define     GPIO1_CTL_MFP_SHIFT                                               0
#define     GPIO1_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO2_CTL_GPIO_INTC_MSK                                           25
#define     GPIO2_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO2_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO2_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO2_CTL_GPIO_INTCEN                                             20
#define     GPIO2_CTL_PADDRV_e                                                14
#define     GPIO2_CTL_PADDRV_SHIFT                                            12
#define     GPIO2_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO2_CTL_10KPUEN                                                 11
#define     GPIO2_CTL_GPIO100KPDEN                                            9
#define     GPIO2_CTL_GPIO50KPUEN                                             8
#define     GPIO2_CTL_GPIOINEN                                                7
#define     GPIO2_CTL_GPIOOUTEN                                               6
#define     GPIO2_CTL_SMIT                                                    5
#define     GPIO2_CTL_MFP_e                                                   3
#define     GPIO2_CTL_MFP_SHIFT                                               0
#define     GPIO2_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO3_CTL_GPIO_INTC_MSK                                           25
#define     GPIO3_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO3_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO3_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO3_CTL_GPIO_INTCEN                                             20
#define     GPIO3_CTL_PADDRV_e                                                14
#define     GPIO3_CTL_PADDRV_SHIFT                                            12
#define     GPIO3_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO3_CTL_10KPUEN                                                 11
#define     GPIO3_CTL_GPIO100KPDEN                                            9
#define     GPIO3_CTL_GPIO100KPUEN                                            8
#define     GPIO3_CTL_GPIOINEN                                                7
#define     GPIO3_CTL_GPIOOUTEN                                               6
#define     GPIO3_CTL_SMIT                                                    5
#define     GPIO3_CTL_AD_Select                                               4
#define     GPIO3_CTL_MFP_e                                                   3
#define     GPIO3_CTL_MFP_SHIFT                                               0
#define     GPIO3_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO4_CTL_GPIO_INTC_MSK                                           25
#define     GPIO4_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO4_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO4_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO4_CTL_GPIO_INTCEN                                             20
#define     GPIO4_CTL_PADDRV_e                                                14
#define     GPIO4_CTL_PADDRV_SHIFT                                            12
#define     GPIO4_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO4_CTL_10KPUEN                                                 11
#define     GPIO4_CTL_GPIO100KPDEN                                            9
#define     GPIO4_CTL_GPIO100KPUEN                                            8
#define     GPIO4_CTL_GPIOINEN                                                7
#define     GPIO4_CTL_GPIOOUTEN                                               6
#define     GPIO4_CTL_SMIT                                                    5
#define     GPIO4_CTL_AD_Select                                               4
#define     GPIO4_CTL_MFP_e                                                   3
#define     GPIO4_CTL_MFP_SHIFT                                               0
#define     GPIO4_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO5_CTL_GPIO_INTC_MSK                                           25
#define     GPIO5_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO5_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO5_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO5_CTL_GPIO_INTCEN                                             20
#define     GPIO5_CTL_PADDRV_e                                                14
#define     GPIO5_CTL_PADDRV_SHIFT                                            12
#define     GPIO5_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO5_CTL_10KPUEN                                                 11
#define     GPIO5_CTL_GPIO100KPDEN                                            9
#define     GPIO5_CTL_GPIO100KPUEN                                            8
#define     GPIO5_CTL_GPIOINEN                                                7
#define     GPIO5_CTL_GPIOOUTEN                                               6
#define     GPIO5_CTL_SMIT                                                    5
#define     GPIO5_CTL_AD_Select                                               4
#define     GPIO5_CTL_MFP_e                                                   3
#define     GPIO5_CTL_MFP_SHIFT                                               0
#define     GPIO5_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO6_CTL_GPIO_INTC_MSK                                           25
#define     GPIO6_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO6_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO6_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO6_CTL_GPIO_INTCEN                                             20
#define     GPIO6_CTL_PADDRV_e                                                14
#define     GPIO6_CTL_PADDRV_SHIFT                                            12
#define     GPIO6_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO6_CTL_10KPUEN                                                 11
#define     GPIO6_CTL_GPIO100KPDEN                                            9
#define     GPIO6_CTL_GPIO100KPUEN                                            8
#define     GPIO6_CTL_GPIOINEN                                                7
#define     GPIO6_CTL_GPIOOUTEN                                               6
#define     GPIO6_CTL_SMIT                                                    5
#define     GPIO6_CTL_AD_Select                                               4
#define     GPIO6_CTL_MFP_e                                                   3
#define     GPIO6_CTL_MFP_SHIFT                                               0
#define     GPIO6_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO7_CTL_GPIO_INTC_MSK                                           25
#define     GPIO7_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO7_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO7_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO7_CTL_GPIO_INTCEN                                             20
#define     GPIO7_CTL_PADDRV_e                                                14
#define     GPIO7_CTL_PADDRV_SHIFT                                            12
#define     GPIO7_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO7_CTL_10KPUEN                                                 11
#define     GPIO7_CTL_GPIO100KPDEN                                            9
#define     GPIO7_CTL_GPIO100KPUEN                                            8
#define     GPIO7_CTL_GPIOINEN                                                7
#define     GPIO7_CTL_GPIOOUTEN                                               6
#define     GPIO7_CTL_SMIT                                                    5
#define     GPIO7_CTL_AD_Select                                               4

#define     GPIO8_CTL_GPIO_INTC_MSK                                           25
#define     GPIO8_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO8_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO8_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO8_CTL_GPIO_INTCEN                                             20
#define     GPIO8_CTL_PADDRV_e                                                14
#define     GPIO8_CTL_PADDRV_SHIFT                                            12
#define     GPIO8_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO8_CTL_10KPUEN                                                 11
#define     GPIO8_CTL_GPIO100KPDEN                                            9
#define     GPIO8_CTL_GPIO100KPUEN                                            8
#define     GPIO8_CTL_GPIOINEN                                                7
#define     GPIO8_CTL_GPIOOUTEN                                               6
#define     GPIO8_CTL_SMIT                                                    5
#define     GPIO8_CTL_MFP_e                                                   3
#define     GPIO8_CTL_MFP_SHIFT                                               0
#define     GPIO8_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO9_CTL_GPIO_INTC_MSK                                           25
#define     GPIO9_CTL_GPIO_TRIG_CTL_e                                         23
#define     GPIO9_CTL_GPIO_TRIG_CTL_SHIFT                                     21
#define     GPIO9_CTL_GPIO_TRIG_CTL_MASK                                      (0x7<<21)
#define     GPIO9_CTL_GPIO_INTCEN                                             20
#define     GPIO9_CTL_PADDRV_e                                                14
#define     GPIO9_CTL_PADDRV_SHIFT                                            12
#define     GPIO9_CTL_PADDRV_MASK                                             (0x7<<12)
#define     GPIO9_CTL_10KPUEN                                                 11
#define     GPIO9_CTL_GPIO100KPDEN                                            9
#define     GPIO9_CTL_GPIO100KPUEN                                            8
#define     GPIO9_CTL_GPIOINEN                                                7
#define     GPIO9_CTL_GPIOOUTEN                                               6
#define     GPIO9_CTL_SMIT                                                    5
#define     GPIO9_CTL_MFP_e                                                   3
#define     GPIO9_CTL_MFP_SHIFT                                               0
#define     GPIO9_CTL_MFP_MASK                                                (0xF<<0)

#define     GPIO10_CTL_GPIO_INTC_MSK                                          25
#define     GPIO10_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO10_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO10_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO10_CTL_GPIO_INTCEN                                            20
#define     GPIO10_CTL_PADDRV_e                                               14
#define     GPIO10_CTL_PADDRV_SHIFT                                           12
#define     GPIO10_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO10_CTL_1P5KPUEN                                               11
#define     GPIO10_CTL_GPIO15KPDEN                                            9
#define     GPIO10_CTL_GPIO150KPUEN                                           8
#define     GPIO10_CTL_GPIOINEN                                               7
#define     GPIO10_CTL_GPIOOUTEN                                              6
#define     GPIO10_CTL_SMIT                                                   5
#define     GPIO10_CTL_MFP_e                                                  3
#define     GPIO10_CTL_MFP_SHIFT                                              0
#define     GPIO10_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO11_CTL_GPIO_INTC_MSK                                          25
#define     GPIO11_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO11_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO11_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO11_CTL_GPIO_INTCEN                                            20
#define     GPIO11_CTL_PADDRV_e                                               14
#define     GPIO11_CTL_PADDRV_SHIFT                                           12
#define     GPIO11_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO11_CTL_1P5KPUEN                                               11
#define     GPIO11_CTL_GPIO15KPDEN                                            9
#define     GPIO11_CTL_GPIO150KPUEN                                           8
#define     GPIO11_CTL_GPIOINEN                                               7
#define     GPIO11_CTL_GPIOOUTEN                                              6
#define     GPIO11_CTL_SMIT                                                   5
#define     GPIO11_CTL_MFP_e                                                  3
#define     GPIO11_CTL_MFP_SHIFT                                              0
#define     GPIO11_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO12_CTL_GPIO_INTC_MSK                                          25
#define     GPIO12_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO12_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO12_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO12_CTL_GPIO_INTCEN                                            20
#define     GPIO12_CTL_PADDRV_e                                               14
#define     GPIO12_CTL_PADDRV_SHIFT                                           12
#define     GPIO12_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO12_CTL_10KPUEN                                                11
#define     GPIO12_CTL_GPIO100KPDEN                                           9
#define     GPIO12_CTL_GPIO100KPUEN                                           8
#define     GPIO12_CTL_GPIOINEN                                               7
#define     GPIO12_CTL_GPIOOUTEN                                              6
#define     GPIO12_CTL_SMIT                                                   5
#define     GPIO12_CTL_AD_Select                                              4
#define     GPIO12_CTL_MFP_e                                                  3
#define     GPIO12_CTL_MFP_SHIFT                                              0
#define     GPIO12_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO13_CTL_GPIO_INTC_MSK                                          25
#define     GPIO13_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO13_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO13_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO13_CTL_GPIO_INTCEN                                            20
#define     GPIO13_CTL_PADDRV_e                                               14
#define     GPIO13_CTL_PADDRV_SHIFT                                           12
#define     GPIO13_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO13_CTL_10KPUEN                                                11
#define     GPIO13_CTL_GPIO100KPDEN                                           9
#define     GPIO13_CTL_GPIO100KPUEN                                           8
#define     GPIO13_CTL_GPIOINEN                                               7
#define     GPIO13_CTL_GPIOOUTEN                                              6
#define     GPIO13_CTL_SMIT                                                   5
#define     GPIO13_CTL_AD_Select                                              4
#define     GPIO13_CTL_MFP_e                                                  3
#define     GPIO13_CTL_MFP_SHIFT                                              0
#define     GPIO13_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO14_CTL_GPIO_INTC_MSK                                          25
#define     GPIO14_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO14_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO14_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO14_CTL_GPIO_INTCEN                                            20
#define     GPIO14_CTL_PADDRV_e                                               14
#define     GPIO14_CTL_PADDRV_SHIFT                                           12
#define     GPIO14_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO14_CTL_10KPUEN                                                11
#define     GPIO14_CTL_GPIO100KPDEN                                           9
#define     GPIO14_CTL_GPIO100KPUEN                                           8
#define     GPIO14_CTL_GPIOINEN                                               7
#define     GPIO14_CTL_GPIOOUTEN                                              6
#define     GPIO14_CTL_SMIT                                                   5
#define     GPIO14_CTL_MFP_e                                                  3
#define     GPIO14_CTL_MFP_SHIFT                                              0
#define     GPIO14_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO15_CTL_GPIO_INTC_MSK                                          25
#define     GPIO15_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO15_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO15_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO15_CTL_GPIO_INTCEN                                            20
#define     GPIO15_CTL_PADDRV_e                                               14
#define     GPIO15_CTL_PADDRV_SHIFT                                           12
#define     GPIO15_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO15_CTL_10KPUEN                                                11
#define     GPIO15_CTL_GPIO100KPDEN                                           9
#define     GPIO15_CTL_GPIO100KPUEN                                           8
#define     GPIO15_CTL_GPIOINEN                                               7
#define     GPIO15_CTL_GPIOOUTEN                                              6
#define     GPIO15_CTL_SMIT                                                   5
#define     GPIO15_CTL_MFP_e                                                  3
#define     GPIO15_CTL_MFP_SHIFT                                              0
#define     GPIO15_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO16_CTL_GPIO_INTC_MSK                                          25
#define     GPIO16_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO16_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO16_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO16_CTL_GPIO_INTCEN                                            20
#define     GPIO16_CTL_PADDRV_e                                               14
#define     GPIO16_CTL_PADDRV_SHIFT                                           12
#define     GPIO16_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO16_CTL_10KPUEN                                                11
#define     GPIO16_CTL_GPIO100KPDEN                                           9
#define     GPIO16_CTL_GPIO100KPUEN                                           8
#define     GPIO16_CTL_GPIOINEN                                               7
#define     GPIO16_CTL_GPIOOUTEN                                              6
#define     GPIO16_CTL_SMIT                                                   5
#define     GPIO16_CTL_MFP_e                                                  3
#define     GPIO16_CTL_MFP_SHIFT                                              0
#define     GPIO16_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO17_CTL_GPIO_INTC_MSK                                          25
#define     GPIO17_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO17_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO17_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO17_CTL_GPIO_INTCEN                                            20
#define     GPIO17_CTL_PADDRV_e                                               14
#define     GPIO17_CTL_PADDRV_SHIFT                                           12
#define     GPIO17_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO17_CTL_10KPUEN                                                11
#define     GPIO17_CTL_GPIO100KPDEN                                           9
#define     GPIO17_CTL_GPIO100KPUEN                                           8
#define     GPIO17_CTL_GPIOINEN                                               7
#define     GPIO17_CTL_GPIOOUTEN                                              6
#define     GPIO17_CTL_SMIT                                                   5
#define     GPIO17_CTL_MFP_e                                                  3
#define     GPIO17_CTL_MFP_SHIFT                                              0
#define     GPIO17_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO18_CTL_GPIO_INTC_MSK                                          25
#define     GPIO18_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO18_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO18_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO18_CTL_GPIO_INTCEN                                            20
#define     GPIO18_CTL_PADDRV_e                                               14
#define     GPIO18_CTL_PADDRV_SHIFT                                           12
#define     GPIO18_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO18_CTL_10KPUEN                                                11
#define     GPIO18_CTL_GPIO100KPDEN                                           9
#define     GPIO18_CTL_GPIO50KPUEN                                            8
#define     GPIO18_CTL_GPIOINEN                                               7
#define     GPIO18_CTL_GPIOOUTEN                                              6
#define     GPIO18_CTL_SMIT                                                   5
#define     GPIO18_CTL_AD_Select                                              4
#define     GPIO18_CTL_MFP_e                                                  3
#define     GPIO18_CTL_MFP_SHIFT                                              0
#define     GPIO18_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO19_CTL_GPIO_INTC_MSK                                          25
#define     GPIO19_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO19_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO19_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO19_CTL_GPIO_INTCEN                                            20
#define     GPIO19_CTL_PADDRV_e                                               14
#define     GPIO19_CTL_PADDRV_SHIFT                                           12
#define     GPIO19_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO19_CTL_10KPUEN                                                11
#define     GPIO19_CTL_GPIO100KPDEN                                           9
#define     GPIO19_CTL_GPIO100KPUEN                                           8
#define     GPIO19_CTL_GPIOINEN                                               7
#define     GPIO19_CTL_GPIOOUTEN                                              6
#define     GPIO19_CTL_SMIT                                                   5
#define     GPIO19_CTL_AD_Select                                              4
#define     GPIO19_CTL_MFP_e                                                  3
#define     GPIO19_CTL_MFP_SHIFT                                              0
#define     GPIO19_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO20_CTL_GPIO_INTC_MSK                                          25
#define     GPIO20_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO20_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO20_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO20_CTL_GPIO_INTCEN                                            20
#define     GPIO20_CTL_PADDRV_e                                               14
#define     GPIO20_CTL_PADDRV_SHIFT                                           12
#define     GPIO20_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO20_CTL_10KPUEN                                                11
#define     GPIO20_CTL_GPIO100KPDEN                                           9
#define     GPIO20_CTL_GPIO50KPUEN                                            8
#define     GPIO20_CTL_GPIOINEN                                               7
#define     GPIO20_CTL_GPIOOUTEN                                              6
#define     GPIO20_CTL_SMIT                                                   5
#define     GPIO20_CTL_AD_Select                                              4
#define     GPIO20_CTL_MFP_e                                                  3
#define     GPIO20_CTL_MFP_SHIFT                                              0
#define     GPIO20_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO21_CTL_GPIO_INTC_MSK                                          25
#define     GPIO21_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO21_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO21_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO21_CTL_GPIO_INTCEN                                            20
#define     GPIO21_CTL_PADDRV_e                                               14
#define     GPIO21_CTL_PADDRV_SHIFT                                           12
#define     GPIO21_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO21_CTL_10KPUEN                                                11
#define     GPIO21_CTL_GPIO100KPDEN                                           9
#define     GPIO21_CTL_GPIO100KPUEN                                           8
#define     GPIO21_CTL_GPIOINEN                                               7
#define     GPIO21_CTL_GPIOOUTEN                                              6
#define     GPIO21_CTL_SMIT                                                   5
#define     GPIO21_CTL_AD_Select                                              4
#define     GPIO21_CTL_MFP_e                                                  3
#define     GPIO21_CTL_MFP_SHIFT                                              0
#define     GPIO21_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO22_CTL_GPIO_INTC_MSK                                          25
#define     GPIO22_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO22_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO22_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO22_CTL_GPIO_INTCEN                                            20
#define     GPIO22_CTL_PADDRV_e                                               14
#define     GPIO22_CTL_PADDRV_SHIFT                                           12
#define     GPIO22_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO22_CTL_10KPUEN                                                11
#define     GPIO22_CTL_GPIO100KPDEN                                           9
#define     GPIO22_CTL_GPIO50KPUEN                                            8
#define     GPIO22_CTL_GPIOINEN                                               7
#define     GPIO22_CTL_GPIOOUTEN                                              6
#define     GPIO22_CTL_SMIT                                                   5
#define     GPIO22_CTL_AD_Select                                              4
#define     GPIO22_CTL_MFP_e                                                  3
#define     GPIO22_CTL_MFP_SHIFT                                              0
#define     GPIO22_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO23_CTL_GPIO_INTC_MSK                                          25
#define     GPIO23_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO23_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO23_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO23_CTL_GPIO_INTCEN                                            20
#define     GPIO23_CTL_PADDRV_e                                               14
#define     GPIO23_CTL_PADDRV_SHIFT                                           12
#define     GPIO23_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO23_CTL_10KPUEN                                                11
#define     GPIO23_CTL_GPIO100KPDEN                                           9
#define     GPIO23_CTL_GPIO50KPUEN                                            8
#define     GPIO23_CTL_GPIOINEN                                               7
#define     GPIO23_CTL_GPIOOUTEN                                              6
#define     GPIO23_CTL_SMIT                                                   5
#define     GPIO23_CTL_AD_Select                                              4
#define     GPIO23_CTL_MFP_e                                                  3
#define     GPIO23_CTL_MFP_SHIFT                                              0
#define     GPIO23_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO24_CTL_GPIO_INTC_MSK                                          25
#define     GPIO24_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO24_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO24_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO24_CTL_GPIO_INTCEN                                            20
#define     GPIO24_CTL_PADDRV_e                                               14
#define     GPIO24_CTL_PADDRV_SHIFT                                           12
#define     GPIO24_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO24_CTL_10KPUEN                                                11
#define     GPIO24_CTL_GPIO100KPDEN                                           9
#define     GPIO24_CTL_GPIO50KPUEN                                            8
#define     GPIO24_CTL_GPIOINEN                                               7
#define     GPIO24_CTL_GPIOOUTEN                                              6
#define     GPIO24_CTL_SMIT                                                   5
#define     GPIO24_CTL_AD_Select                                              4
#define     GPIO24_CTL_MFP_e                                                  3
#define     GPIO24_CTL_MFP_SHIFT                                              0
#define     GPIO24_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO25_CTL_GPIO_INTC_MSK                                          25
#define     GPIO25_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO25_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO25_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO25_CTL_GPIO_INTCEN                                            20
#define     GPIO25_CTL_PADDRV_e                                               14
#define     GPIO25_CTL_PADDRV_SHIFT                                           12
#define     GPIO25_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO25_CTL_10KPUEN                                                11
#define     GPIO25_CTL_GPIO100KPDEN                                           9
#define     GPIO25_CTL_GPIO50KPUEN                                            8
#define     GPIO25_CTL_GPIOINEN                                               7
#define     GPIO25_CTL_GPIOOUTEN                                              6
#define     GPIO25_CTL_SMIT                                                   5
#define     GPIO25_CTL_AD_Select                                              4
#define     GPIO25_CTL_MFP_e                                                  3
#define     GPIO25_CTL_MFP_SHIFT                                              0
#define     GPIO25_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO26_CTL_GPIO_INTC_MSK                                          25
#define     GPIO26_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO26_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO26_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO26_CTL_GPIO_INTCEN                                            20
#define     GPIO26_CTL_PADDRV_e                                               14
#define     GPIO26_CTL_PADDRV_SHIFT                                           12
#define     GPIO26_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO26_CTL_10KPUEN                                                11
#define     GPIO26_CTL_GPIO100KPDEN                                           9
#define     GPIO26_CTL_GPIO100KPUEN                                           8
#define     GPIO26_CTL_GPIOINEN                                               7
#define     GPIO26_CTL_GPIOOUTEN                                              6
#define     GPIO26_CTL_SMIT                                                   5
#define     GPIO26_CTL_AD_Select                                              4
#define     GPIO26_CTL_MFP_e                                                  3
#define     GPIO26_CTL_MFP_SHIFT                                              0
#define     GPIO26_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO27_CTL_GPIO_INTC_MSK                                          25
#define     GPIO27_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO27_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO27_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO27_CTL_GPIO_INTCEN                                            20
#define     GPIO27_CTL_PADDRV_e                                               14
#define     GPIO27_CTL_PADDRV_SHIFT                                           12
#define     GPIO27_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO27_CTL_10KPUEN                                                11
#define     GPIO27_CTL_GPIO100KPDEN                                           9
#define     GPIO27_CTL_GPIO100KPUEN                                           8
#define     GPIO27_CTL_GPIOINEN                                               7
#define     GPIO27_CTL_GPIOOUTEN                                              6
#define     GPIO27_CTL_SMIT                                                   5
#define     GPIO27_CTL_AD_Select                                              4
#define     GPIO27_CTL_MFP_e                                                  3
#define     GPIO27_CTL_MFP_SHIFT                                              0
#define     GPIO27_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO28_CTL_GPIO_INTC_MSK                                          25
#define     GPIO28_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO28_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO28_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO28_CTL_GPIO_INTCEN                                            20
#define     GPIO28_CTL_PADDRV_e                                               14
#define     GPIO28_CTL_PADDRV_SHIFT                                           12
#define     GPIO28_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO28_CTL_10KPUEN                                                11
#define     GPIO28_CTL_GPIO100KPDEN                                           9
#define     GPIO28_CTL_GPIO50KPUEN                                            8
#define     GPIO28_CTL_GPIOINEN                                               7
#define     GPIO28_CTL_GPIOOUTEN                                              6
#define     GPIO28_CTL_SMIT                                                   5
#define     GPIO28_CTL_AD_Select                                              4
#define     GPIO28_CTL_MFP_e                                                  3
#define     GPIO28_CTL_MFP_SHIFT                                              0
#define     GPIO28_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO29_CTL_GPIO_INTC_MSK                                          25
#define     GPIO29_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO29_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO29_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO29_CTL_GPIO_INTCEN                                            20
#define     GPIO29_CTL_PADDRV_e                                               14
#define     GPIO29_CTL_PADDRV_SHIFT                                           12
#define     GPIO29_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO29_CTL_10KPUEN                                                11
#define     GPIO29_CTL_GPIO100KPDEN                                           9
#define     GPIO29_CTL_GPIO50KPUEN                                            8
#define     GPIO29_CTL_GPIOINEN                                               7
#define     GPIO29_CTL_GPIOOUTEN                                              6
#define     GPIO29_CTL_SMIT                                                   5
#define     GPIO29_CTL_AD_Select                                              4
#define     GPIO29_CTL_MFP_e                                                  3
#define     GPIO29_CTL_MFP_SHIFT                                              0
#define     GPIO29_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO30_CTL_GPIO_INTC_MSK                                          25
#define     GPIO30_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO30_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO30_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO30_CTL_GPIO_INTCEN                                            20
#define     GPIO30_CTL_PADDRV_e                                               14
#define     GPIO30_CTL_PADDRV_SHIFT                                           12
#define     GPIO30_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO30_CTL_10KPUEN                                                11
#define     GPIO30_CTL_GPIO100KPDEN                                           9
#define     GPIO30_CTL_GPIO50KPUEN                                            8
#define     GPIO30_CTL_GPIOINEN                                               7
#define     GPIO30_CTL_GPIOOUTEN                                              6
#define     GPIO30_CTL_SMIT                                                   5
#define     GPIO30_CTL_AD_Select                                              4
#define     GPIO30_CTL_MFP_e                                                  3
#define     GPIO30_CTL_MFP_SHIFT                                              0
#define     GPIO30_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO31_CTL_GPIO_INTC_MSK                                          25
#define     GPIO31_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO31_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO31_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO31_CTL_GPIO_INTCEN                                            20
#define     GPIO31_CTL_PADDRV_e                                               14
#define     GPIO31_CTL_PADDRV_SHIFT                                           12
#define     GPIO31_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO31_CTL_10KPUEN                                                11
#define     GPIO31_CTL_GPIO100KPDEN                                           9
#define     GPIO31_CTL_GPIO50KPUEN                                            8
#define     GPIO31_CTL_GPIOINEN                                               7
#define     GPIO31_CTL_GPIOOUTEN                                              6
#define     GPIO31_CTL_SMIT                                                   5
#define     GPIO31_CTL_AD_Select                                              4
#define     GPIO31_CTL_MFP_e                                                  3
#define     GPIO31_CTL_MFP_SHIFT                                              0
#define     GPIO31_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO32_CTL_GPIO_INTC_MSK                                          25
#define     GPIO32_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO32_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO32_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO32_CTL_GPIO_INTCEN                                            20
#define     GPIO32_CTL_PADDRV_e                                               14
#define     GPIO32_CTL_PADDRV_SHIFT                                           12
#define     GPIO32_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO32_CTL_10KPUEN                                                11
#define     GPIO32_CTL_GPIO100KPDEN                                           9
#define     GPIO32_CTL_GPIO100KPUEN                                           8
#define     GPIO32_CTL_GPIOINEN                                               7
#define     GPIO32_CTL_GPIOOUTEN                                              6
#define     GPIO32_CTL_SMIT                                                   5
#define     GPIO32_CTL_MFP_e                                                  3
#define     GPIO32_CTL_MFP_SHIFT                                              0
#define     GPIO32_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO33_CTL_GPIO_INTC_MSK                                          25
#define     GPIO33_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO33_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO33_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO33_CTL_GPIO_INTCEN                                            20
#define     GPIO33_CTL_PADDRV_e                                               14
#define     GPIO33_CTL_PADDRV_SHIFT                                           12
#define     GPIO33_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO33_CTL_10KPUEN                                                11
#define     GPIO33_CTL_GPIO100KPDEN                                           9
#define     GPIO33_CTL_GPIO100KPUEN                                           8
#define     GPIO33_CTL_GPIOINEN                                               7
#define     GPIO33_CTL_GPIOOUTEN                                              6
#define     GPIO33_CTL_SMIT                                                   5
#define     GPIO33_CTL_MFP_e                                                  3
#define     GPIO33_CTL_MFP_SHIFT                                              0
#define     GPIO33_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO34_CTL_GPIO_INTC_MSK                                          25
#define     GPIO34_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO34_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO34_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO34_CTL_GPIO_INTCEN                                            20
#define     GPIO34_CTL_PADDRV_e                                               14
#define     GPIO34_CTL_PADDRV_SHIFT                                           12
#define     GPIO34_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO34_CTL_10KPUEN                                                11
#define     GPIO34_CTL_GPIO100KPDEN                                           9
#define     GPIO34_CTL_GPIO100KPUEN                                           8
#define     GPIO34_CTL_GPIOINEN                                               7
#define     GPIO34_CTL_GPIOOUTEN                                              6
#define     GPIO34_CTL_SMIT                                                   5
#define     GPIO34_CTL_MFP_e                                                  3
#define     GPIO34_CTL_MFP_SHIFT                                              0
#define     GPIO34_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO35_CTL_GPIO_INTC_MSK                                          25
#define     GPIO35_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO35_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO35_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO35_CTL_GPIO_INTCEN                                            20
#define     GPIO35_CTL_PADDRV_e                                               14
#define     GPIO35_CTL_PADDRV_SHIFT                                           12
#define     GPIO35_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO35_CTL_10KPUEN                                                11
#define     GPIO35_CTL_GPIO100KPDEN                                           9
#define     GPIO35_CTL_GPIO100KPUEN                                           8
#define     GPIO35_CTL_GPIOINEN                                               7
#define     GPIO35_CTL_GPIOOUTEN                                              6
#define     GPIO35_CTL_SMIT                                                   5
#define     GPIO35_CTL_AD_Select                                              4
#define     GPIO35_CTL_MFP_e                                                  3
#define     GPIO35_CTL_MFP_SHIFT                                              0
#define     GPIO35_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO36_CTL_GPIO_INTC_MSK                                          25
#define     GPIO36_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO36_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO36_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO36_CTL_GPIO_INTCEN                                            20
#define     GPIO36_CTL_PADDRV_e                                               14
#define     GPIO36_CTL_PADDRV_SHIFT                                           12
#define     GPIO36_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO36_CTL_10KPUEN                                                11
#define     GPIO36_CTL_GPIO100KPDEN                                           9
#define     GPIO36_CTL_GPIO100KPUEN                                           8
#define     GPIO36_CTL_GPIOINEN                                               7
#define     GPIO36_CTL_GPIOOUTEN                                              6
#define     GPIO36_CTL_SMIT                                                   5
#define     GPIO36_CTL_MFP_e                                                  3
#define     GPIO36_CTL_MFP_SHIFT                                              0
#define     GPIO36_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO37_CTL_GPIO_INTC_MSK                                          25
#define     GPIO37_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO37_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO37_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO37_CTL_GPIO_INTCEN                                            20
#define     GPIO37_CTL_PADDRV_e                                               14
#define     GPIO37_CTL_PADDRV_SHIFT                                           12
#define     GPIO37_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO37_CTL_10KPUEN                                                11
#define     GPIO37_CTL_GPIO100KPDEN                                           9
#define     GPIO37_CTL_GPIO100KPUEN                                           8
#define     GPIO37_CTL_GPIOINEN                                               7
#define     GPIO37_CTL_GPIOOUTEN                                              6
#define     GPIO37_CTL_SMIT                                                   5
#define     GPIO37_CTL_MFP_e                                                  3
#define     GPIO37_CTL_MFP_SHIFT                                              0
#define     GPIO37_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO38_CTL_PADDRV_e                                               14
#define     GPIO38_CTL_PADDRV_SHIFT                                           12
#define     GPIO38_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO38_CTL_GPIO100KPDEN                                           9
#define     GPIO38_CTL_GPIO100KPUEN                                           8
#define     GPIO38_CTL_GPIOINEN                                               7
#define     GPIO38_CTL_GPIOOUTEN                                              6
#define     GPIO38_CTL_AD_Select                                              4

#define     GPIO39_CTL_PADDRV_e                                               14
#define     GPIO39_CTL_PADDRV_SHIFT                                           12
#define     GPIO39_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO39_CTL_GPIO100KPDEN                                           9
#define     GPIO39_CTL_GPIO100KPUEN                                           8
#define     GPIO39_CTL_GPIOINEN                                               7
#define     GPIO39_CTL_GPIOOUTEN                                              6
#define     GPIO39_CTL_AD_Select                                              4

#define     GPIO40_CTL_PADDRV_e                                               14
#define     GPIO40_CTL_PADDRV_SHIFT                                           12
#define     GPIO40_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO40_CTL_GPIO100KPDEN                                           9
#define     GPIO40_CTL_GPIO100KPUEN                                           8
#define     GPIO40_CTL_GPIOINEN                                               7
#define     GPIO40_CTL_GPIOOUTEN                                              6
#define     GPIO40_CTL_AD_Select                                              4

#define     GPIO41_CTL_PADDRV_e                                               14
#define     GPIO41_CTL_PADDRV_SHIFT                                           12
#define     GPIO41_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO41_CTL_GPIO100KPDEN                                           9
#define     GPIO41_CTL_GPIO100KPUEN                                           8
#define     GPIO41_CTL_GPIOINEN                                               7
#define     GPIO41_CTL_GPIOOUTEN                                              6
#define     GPIO41_CTL_AD_Select                                              4

#define     GPIO42_CTL_PADDRV_e                                               14
#define     GPIO42_CTL_PADDRV_SHIFT                                           12
#define     GPIO42_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO42_CTL_GPIO100KPDEN                                           9
#define     GPIO42_CTL_GPIO100KPUEN                                           8
#define     GPIO42_CTL_GPIOINEN                                               7
#define     GPIO42_CTL_GPIOOUTEN                                              6
#define     GPIO42_CTL_AD_Select                                              4

#define     GPIO43_CTL_PADDRV_e                                               14
#define     GPIO43_CTL_PADDRV_SHIFT                                           12
#define     GPIO43_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO43_CTL_GPIO100KPDEN                                           9
#define     GPIO43_CTL_GPIO100KPUEN                                           8
#define     GPIO43_CTL_GPIOINEN                                               7
#define     GPIO43_CTL_GPIOOUTEN                                              6
#define     GPIO43_CTL_AD_Select                                              4

#define     GPIO44_CTL_PADDRV_e                                               14
#define     GPIO44_CTL_PADDRV_SHIFT                                           12
#define     GPIO44_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO44_CTL_GPIO100KPDEN                                           9
#define     GPIO44_CTL_GPIO100KPUEN                                           8
#define     GPIO44_CTL_GPIOINEN                                               7
#define     GPIO44_CTL_GPIOOUTEN                                              6
#define     GPIO44_CTL_AD_Select                                              4

#define     GPIO45_CTL_PADDRV_e                                               14
#define     GPIO45_CTL_PADDRV_SHIFT                                           12
#define     GPIO45_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO45_CTL_GPIO100KPDEN                                           9
#define     GPIO45_CTL_GPIO100KPUEN                                           8
#define     GPIO45_CTL_GPIOINEN                                               7
#define     GPIO45_CTL_GPIOOUTEN                                              6
#define     GPIO45_CTL_AD_Select                                              4

#define     GPIO46_CTL_PADDRV_e                                               14
#define     GPIO46_CTL_PADDRV_SHIFT                                           12
#define     GPIO46_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO46_CTL_GPIO100KPDEN                                           9
#define     GPIO46_CTL_GPIO100KPUEN                                           8
#define     GPIO46_CTL_GPIOINEN                                               7
#define     GPIO46_CTL_GPIOOUTEN                                              6
#define     GPIO46_CTL_AD_Select                                              4

#define     GPIO47_CTL_PADDRV_e                                               14
#define     GPIO47_CTL_PADDRV_SHIFT                                           12
#define     GPIO47_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO47_CTL_GPIO100KPDEN                                           9
#define     GPIO47_CTL_GPIO100KPUEN                                           8
#define     GPIO47_CTL_GPIOINEN                                               7
#define     GPIO47_CTL_GPIOOUTEN                                              6
#define     GPIO47_CTL_AD_Select                                              4

#define     GPIO48_CTL_GPIO_INTC_MSK                                          25
#define     GPIO48_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO48_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO48_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO48_CTL_GPIO_INTCEN                                            20
#define     GPIO48_CTL_PADDRV_e                                               14
#define     GPIO48_CTL_PADDRV_SHIFT                                           12
#define     GPIO48_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO48_CTL_10KPUEN                                                11
#define     GPIO48_CTL_GPIO100KPDEN                                           9
#define     GPIO48_CTL_GPIO100KPUEN                                           8
#define     GPIO48_CTL_GPIOINEN                                               7
#define     GPIO48_CTL_GPIOOUTEN                                              6
#define     GPIO48_CTL_SMIT                                                   5
#define     GPIO48_CTL_AD_Select                                              4
#define     GPIO48_CTL_MFP_e                                                  3
#define     GPIO48_CTL_MFP_SHIFT                                              0
#define     GPIO48_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO49_CTL_GPIO_INTC_MSK                                          25
#define     GPIO49_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO49_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO49_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO49_CTL_GPIO_INTCEN                                            20
#define     GPIO49_CTL_PADDRV_e                                               14
#define     GPIO49_CTL_PADDRV_SHIFT                                           12
#define     GPIO49_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO49_CTL_10KPUEN                                                11
#define     GPIO49_CTL_GPIO100KPDEN                                           9
#define     GPIO49_CTL_GPIO100KPUEN                                           8
#define     GPIO49_CTL_GPIOINEN                                               7
#define     GPIO49_CTL_GPIOOUTEN                                              6
#define     GPIO49_CTL_SMIT                                                   5
#define     GPIO49_CTL_MFP_e                                                  3
#define     GPIO49_CTL_MFP_SHIFT                                              0
#define     GPIO49_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO50_CTL_GPIO_INTC_MSK                                          25
#define     GPIO50_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO50_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO50_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO50_CTL_GPIO_INTCEN                                            20
#define     GPIO50_CTL_PADDRV_e                                               14
#define     GPIO50_CTL_PADDRV_SHIFT                                           12
#define     GPIO50_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO50_CTL_10KPUEN                                                11
#define     GPIO50_CTL_GPIO100KPDEN                                           9
#define     GPIO50_CTL_GPIO100KPUEN                                           8
#define     GPIO50_CTL_GPIOINEN                                               7
#define     GPIO50_CTL_GPIOOUTEN                                              6
#define     GPIO50_CTL_SMIT                                                   5
#define     GPIO50_CTL_MFP_e                                                  3
#define     GPIO50_CTL_MFP_SHIFT                                              0
#define     GPIO50_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO51_CTL_GPIO_INTC_MSK                                          25
#define     GPIO51_CTL_GPIO_TRIG_CTL_e                                        23
#define     GPIO51_CTL_GPIO_TRIG_CTL_SHIFT                                    21
#define     GPIO51_CTL_GPIO_TRIG_CTL_MASK                                     (0x7<<21)
#define     GPIO51_CTL_GPIO_INTCEN                                            20
#define     GPIO51_CTL_PADDRV_e                                               14
#define     GPIO51_CTL_PADDRV_SHIFT                                           12
#define     GPIO51_CTL_PADDRV_MASK                                            (0x7<<12)
#define     GPIO51_CTL_10KPUEN                                                11
#define     GPIO51_CTL_GPIO100KPDEN                                           9
#define     GPIO51_CTL_GPIO100KPUEN                                           8
#define     GPIO51_CTL_GPIOINEN                                               7
#define     GPIO51_CTL_GPIOOUTEN                                              6
#define     GPIO51_CTL_SMIT                                                   5
#define     GPIO51_CTL_MFP_e                                                  3
#define     GPIO51_CTL_MFP_SHIFT                                              0
#define     GPIO51_CTL_MFP_MASK                                               (0xF<<0)

#define     GPIO_ODAT0_GPIO_ODAT_e                                            31
#define     GPIO_ODAT0_GPIO_ODAT_SHIFT                                        0
#define     GPIO_ODAT0_GPIO_ODAT_MASK                                         (0xFFFFFFFF<<0)

#define     GPIO_ODAT1_GPIO_ODAT_e                                            19
#define     GPIO_ODAT1_GPIO_ODAT_SHIFT                                        0
#define     GPIO_ODAT1_GPIO_ODAT_MASK                                         (0xFFFFF<<0)

#define     GPIO_BSR0_GPIO_BSR_e                                              31
#define     GPIO_BSR0_GPIO_BSR_SHIFT                                          0
#define     GPIO_BSR0_GPIO_BSR_MASK                                           (0xFFFFFFFF<<0)

#define     GPIO_BSR1_GPIO_BSR_e                                              19
#define     GPIO_BSR1_GPIO_BSR_SHIFT                                          0
#define     GPIO_BSR1_GPIO_BSR_MASK                                           (0xFFFFF<<0)

#define     GPIO_BRR0_GPIO_BRR_e                                              31
#define     GPIO_BRR0_GPIO_BRR_SHIFT                                          0
#define     GPIO_BRR0_GPIO_BRR_MASK                                           (0xFFFFFFFF<<0)

#define     GPIO_BRR1_GPIO_BRR_e                                              19
#define     GPIO_BRR1_GPIO_BRR_SHIFT                                          0
#define     GPIO_BRR1_GPIO_BRR_MASK                                           (0xFFFFF<<0)

#define     GPIO_IDAT0_GPIO_IDAT_e                                            31
#define     GPIO_IDAT0_GPIO_IDAT_SHIFT                                        0
#define     GPIO_IDAT0_GPIO_IDAT_MASK                                         (0xFFFFFFFF<<0)

#define     GPIO_IDAT1_GPIO_IDAT_e                                            19
#define     GPIO_IDAT1_GPIO_IDAT_SHIFT                                        0
#define     GPIO_IDAT1_GPIO_IDAT_MASK                                         (0xFFFFF<<0)

#define     GPIO_PD0_GPIO_PD_e                                                31
#define     GPIO_PD0_GPIO_PD_SHIFT                                            0
#define     GPIO_PD0_GPIO_PD_MASK                                             (0xFFFFFFFF<<0)

#define     GPIO_PD1_GPIO_PD1_e                                               19
#define     GPIO_PD1_GPIO_PD1_SHIFT                                           16
#define     GPIO_PD1_GPIO_PD1_MASK                                            (0xF<<16)
#define     GPIO_PD1_GPIO_PD0_e                                               5
#define     GPIO_PD1_GPIO_PD0_SHIFT                                           0
#define     GPIO_PD1_GPIO_PD0_MASK                                            (0x3F<<0)

#define     WIO0_CTL_SIODAT                                                   16
#define     WIO0_CTL_PADDRV_e                                                 14
#define     WIO0_CTL_PADDRV_SHIFT                                             12
#define     WIO0_CTL_PADDRV_MASK                                              (0x7<<12)
#define     WIO0_CTL_SIO500KPDEN                                              9
#define     WIO0_CTL_SIO500KPUEN                                              8
#define     WIO0_CTL_SIOINEN                                                  7
#define     WIO0_CTL_SIOOUTEN                                                 6

#define     WIO1_CTL_SIODAT                                                   16
#define     WIO1_CTL_PADDRV_e                                                 14
#define     WIO1_CTL_PADDRV_SHIFT                                             12
#define     WIO1_CTL_PADDRV_MASK                                              (0x7<<12)
#define     WIO1_CTL_SIO500KPDEN                                              9
#define     WIO1_CTL_SIO500KPUEN                                              8
#define     WIO1_CTL_SIOINEN                                                  7
#define     WIO1_CTL_SIOOUTEN                                                 6

#define     DEBUGSEL_DBG_SEL_e                                                4
#define     DEBUGSEL_DBG_SEL_SHIFT                                            0
#define     DEBUGSEL_DBG_SEL_MASK                                             (0x1F<<0)

#define     DEBUGIE0_DEBUG_IE0_e                                              31
#define     DEBUGIE0_DEBUG_IE0_SHIFT                                          0
#define     DEBUGIE0_DEBUG_IE0_MASK                                           (0xFFFFFFFF<<0)

#define     DEBUGIE1_DEBUG_IE1_e                                              1
#define     DEBUGIE1_DEBUG_IE1_SHIFT                                          0
#define     DEBUGIE1_DEBUG_IE1_MASK                                           (0x3<<0)

#define     DEBUGOE0_DEBUG_OE0_e                                              31
#define     DEBUGOE0_DEBUG_OE0_SHIFT                                          0
#define     DEBUGOE0_DEBUG_OE0_MASK                                           (0xFFFFFFFF<<0)

#define     DEBUGOE1_DEBUG_OE1_e                                              1
#define     DEBUGOE1_DEBUG_OE1_SHIFT                                          0
#define     DEBUGOE1_DEBUG_OE1_MASK                                           (0x3<<0)

//---------------------------------------------------------------------//
#endif
