/********************************************************************************
 *                              USDK 1100
 *                            Module: PM
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     pm.h
 * \brief    功耗管理宏定义和函数接口声明
 * \author   wuyueqian
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __PM_H__
#define __PM_H__

#define SUPPORT_MAX_FRE 5

typedef struct
{
    uint8 freq;
    uint8 cycle;
    uint8 divisor;
} spi_clk_param_t;

#define K_VCC_2_7               0
#define K_VCC_2_8               1
#define K_VCC_2_9               2
#define K_VCC_3_0               3
#define K_VCC_3_1               4
#define K_VCC_3_2               5
#define K_VCC_3_3               6
#define K_VCC_3_4               7

#define K_VDD_0_8               0       // 0.80V
#define K_VDD_0_85              1       // 0.85V
#define K_VDD_0_9               2       // 0.9V
#define K_VDD_0_95              3       // 0.95V
#define K_VDD_1_0               4       // 1.0V
#define K_VDD_1_05              5       // 1.05V
#define K_VDD_1_1               6       // 1.1V
#define K_VDD_1_15              7       // 1.15V
#define K_VDD_1_2               8       // 1.2V
#define K_VDD_1_25              9       // 1.25V
#define K_VDD_1_3               10      // 1.3V
#define K_VDD_1_35              11      // 1.35V
#define K_VDD_1_4               12      // 1.4V
#define K_VDD_1_45              13      // 1.45V
#define K_VDD_1_5               15      // 1.5V

typedef enum
{
    FREQ_0M = 0,
    FREQ_4M = 1,
    FREQ_8M = 2,
    FREQ_14M = 3,
    FREQ_26M = 4,
    FREQ_30M = 5,
    FREQ_36M = 6,
    FREQ_42M = 7,
    FREQ_48M = 8,
    FREQ_54M = 9,
    FREQ_60M = 10,
    FREQ_66M = 11,
    FREQ_72M = 12,
    FREQ_78M = 13,
    FREQ_84M = 14,
    FREQ_90M = 15,
    FREQ_96M = 16,
    FREQ_102M = 17,
    FREQ_108M = 18,
    FREQ_114M = 19,
    FREQ_120M = 20,
    FREQ_126M = 21,
    FREQ_132M = 22,
    FREQ_138M = 23,
    FREQ_144M = 24,
    FREQ_150M = 25,
    FREQ_156M = 26,
    FREQ_162M = 27,
    FREQ_168M = 28,
    FREQ_174M = 29,
    FREQ_180M = 30,
    FREQ_186M = 31,
    FREQ_192M = 32,
    FREQ_198M = 33,
    FREQ_204M = 34,
    FREQ_210M = 35,
    FREQ_216M = 36,
    FREQ_222M = 37,
} freq_level_e;

#define SET_SYS_CLK        0       //for ui
#define SET_DECODE_CLK     1       //for decoder & other alsp lib
#define SET_BT_CLK         2       //for bt controller & bt host & bt engine
#define SET_BTDRV_CLK      3       //for bt controller temp adjust
#define SET_LOCK_CLK       4       //for lock clk, freq==0 for unlock

#endif /*__PM_H__*/



