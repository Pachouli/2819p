/********************************************************************************
*                              USDK281A
*                            Module: Boot
*                 Copyright(c) 2003-2016 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*      zhouxl     2015-9-10 15:00     1.0             build this file
********************************************************************************/
/*!
 * \file     boot_config_id.h
 * \brief    引导代码接口声明
 * \author   zhouxl
 * \version 1.0
 * \date  2015/9/10
*******************************************************************************/

#ifndef __BOOT_CONFIG_ID_H__
#define __BOOT_CONFIG_ID_H__

#include "types.h"

#ifndef _ASSEMBLER_

//以下ID定义值和顺序必须与 case 目录下 cfg_boot.txt 中一致
typedef enum
{
    SETTING_SYS_DEBUG_MODE_ENABLE = 0,
    SETTING_SYS_RESET_AFTER_PRODUCT,
    SETTING_SYS_EMI_REDUCTION_ENABLE,
    SETTING_SYS_ADCKEY_ADFU_ENABLE,
    SETTING_SYS_ADCKEY_RES_ENABLE,
    //启动阶段LRADC检测到为0时的功能0:不做任何特殊操作，1:进入ADFU 2：进入卡量产
    SETTING_SYS_ADCKEY_BOOT_FUNC,
    SETTING_SYS_TIMER1_PERIOD_LEVEL,
    SETTING_SYS_SPI_NOR_CLK_CONFIG,
    SETTING_SYS_SPI_NOR_ID_CONFIG,
    SETTING_SYS_CAPACITOR_SELECT,
    SETTING_SYS_VCC_VOLTAGE_SELECT,
    SETTING_SYS_LIBC_MALLOC_ADDR,
    SETTING_SYS_LIBC_MALLOC_SIZE,
}boot_config_index_e;
#endif

#endif  /*#ifndef __BOOT_CONFIG_ID_H__*/


