/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     storage_device.h
 * \brief    ?ééè±??y?ˉ?ó?ú
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef _CARD_DEVICE_H
#define _CARD_DEVICE_H

#ifndef _ASSEMBLER_

#include <storage_device.h>
#include <typeext.h>

#define     SLOW_DRIVER   0
#define     FAST_DRIVER   1
//card 驱动能力管理
#define     SDCLKCTL_LOW_FREQUENCE              (0x01 << CMU_SDCLK_SDCLKSEL1)        // NOT divide 128
#define     SDCLKCTL_DIV_1                      (0x00 << CMU_SDCLK_SDCLKDIV_SHIFT)
#define     SDCLKCTL_DIV_2                      (0x01 << CMU_SDCLK_SDCLKDIV_SHIFT)
#define     SDCLKCTL_DIV_3                      (0x02 << CMU_SDCLK_SDCLKDIV_SHIFT)
#define     SDCLKCTL_DIV_4                      (0x03 << CMU_SDCLK_SDCLKDIV_SHIFT)
#define     SDCLKCTL_SEL_PLL                    (0x01 << CMU_SDCLK_SDCLKSEL0)        // 0: HOSC; 1: MCUPLL

#define     SDCLKCTL_LOW                        (SDCLKCTL_DIV_3 | SDCLKCTL_LOW_FREQUENCE)                            // (HOSC / 128) / 2
#define     SDCLKCTL_HOSC_DIV2                  (SDCLKCTL_DIV_2)  // (HOSC / 1) / 2
#define     SDCLKCTL_HOSC_DIV1                  (SDCLKCTL_DIV_1)  // (HOSC / 1) / 1


typedef struct
{
    /*mucpll clk div*/  
    uint8 mcupll_max_freq : 7;
    /*use mcupll or not*/
    uint8 mcupll_use : 1; 

    /*card clk driver level*/
    uint8 clk_drv_level:4;
    /*card cmd driver level*/
    uint8 cmd_drv_level:4;
    /*card data driver level*/		
    uint8 data_drv_level : 4;
    /*driver mode:0-slow drv;1-fast drv*/
    uint8 drv_mode: 1;
    /*reserved 3bit*/
    uint8 reserved: 3;

    uint8 card_type;

    /*
    * 卡的属性，应用需要在装载卡驱动之前设置这个变量;如果不设置则会使用前一次设置的值。
    bit0: 0--1_line, 1--4_line;
    bit1(sdvccout power): 0--direct, 1--GPIO;
    bit2(DAT3 pull high): 0--hardware pull, 1--GPIO pull;
    bit3(write_fast):0--not continuous, 1--continuous write;
    */
    uint8 card_para;

    /*card init clk 0:12M 1:24M*/
    uint32 sd_clk_init;
    /*card read/write clk 0:12M 1:24M 2:MCUPLL*/
    uint32 sd_clk_rw;
}card_pm_cfg_t;




#endif

#endif

