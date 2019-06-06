/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen   2014-7-28 14:48     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine_common.h
 * \brief    cailizhen
 * \version  1.0
 * \date     2014-7-28
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_COMMON_H
#define _BLUETOOTH_ENGINE_COMMON_H

#include "common_btengine_app_def.h"
#include "bluetooth_engine_hid_def.h"

enum
{
    MODULE_NULL = 0, MODULE_CONTROLLER = 0x01, MODULE_HOST = (0x01 << 1)
};

extern uint8 enter_low_power(uint8 module_type);
extern uint8 exit_low_power(uint8 module_type);

#endif
