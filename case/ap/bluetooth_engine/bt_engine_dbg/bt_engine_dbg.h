/*******************************************************************************
 *                              us281B
 *                            Module: Config
 *                 Copyright(c) 2018-2028 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>                  <version >      <desc>
 *       anbang     2018-5-14 15:21:29      1.0             build this file
 *******************************************************************************/
/*
 * \file     bt_engine_dbg.h
 * \brief    huanganbang
 *           add some dbg method in here
 * \version 1.0
 * \date  20180514
 *******************************************************************************/
#ifndef _BLUETOOTH_ENGINE_DBG_H
#define _BLUETOOTH_ENGINE_DBG_H

#define ENABLE_BT_ENGINE_DBG        0

typedef void (*BT_DBG_TIME_TIMEOUT_CBK_FUNC)(void);

typedef struct _local_time_st
{
    bool enable;
    uint32 cur_time;
    uint32 last_time;
    uint32 time_out;
    BT_DBG_TIME_TIMEOUT_CBK_FUNC timeout_cbk;
} local_time_st;

extern void bt_dbg_time_timeout_alarm_set(uint32 timeout, BT_DBG_TIME_TIMEOUT_CBK_FUNC cbk);
extern void bt_dbg_time_loop(void);

#endif

