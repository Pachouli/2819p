/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen   2014-7-28 10:52     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine_a2dp_dec.h
 * \brief    cailizhen
 * \version  1.0
 * \date     2014-7-28
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_A2DP_DEC_H
#define _BLUETOOTH_ENGINE_A2DP_DEC_H

#include "bluetooth_engine_common.h"

#define AVDTP_AAC_HEADER_LEN  16
#define AVDTP_SBC_HEADER_LEN  13

//SBC数据缓存buffer
extern uint8 SBC_DIRECT_INPUT_BUFFER[A2DP_SBC_DIRECT_INPUT_BUFFER_LEN];
//AAC数据缓存buffer
#define AAC_DIRECT_INPUT_BUFFER         SBC_DIRECT_INPUT_BUFFER

extern uint8 SBC_INPUT_BUFFER_ADDRESS[A2DP_SBC_INPUT_BUFFER_LEN];/*474ms @ max 345kbps*/
#define SBC_MAX_BITPOOL_HIGH_QUALITY    (0x35)
#define SBC_MAX_BITPOOL_MIDDLE_QUALITY  (0x23)
extern uint8 LINEIN_INPUT_BUFFER_ADDRESS[LINEIN_SBC_INPUT_BUFFER_LEN];

#define AAC_INPUT_BUF                   SBC_INPUT_BUFFER_ADDRESS
#define AAC_INPUT_BUF_LENGTH            (0x2000)

#define TIMER_IGNORE_SBC_AFTER_PAUSE    (3000) //sbc解码暂停后3秒钟不处理sbc data ind

extern uint16 pause_cnt;
extern uint16 start_cnt;

extern uint8 * get_sbc_frame_data(void *data_addr);
extern uint8 * get_aac_frame_data(void *data_addr);

#endif
