/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_lib_init.c
 * \brief    global variableÄ£¿é
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/

#include "bluetooth_engine.h"

#if (ENABLE_BT_ENGINE_DBG == 1)

local_time_st gst_bt_dbg_time;

void bt_dbg_time_timeout_alarm_set(uint32 timeout,BT_DBG_TIME_TIMEOUT_CBK_FUNC cbk)
{
    gst_bt_dbg_time.enable = TRUE;
    gst_bt_dbg_time.last_time = gst_bt_dbg_time.cur_time = sys_get_ab_timer();
    gst_bt_dbg_time.time_out = timeout;
    gst_bt_dbg_time.timeout_cbk = cbk;
}

void bt_dbg_time_loop(void)
{
    if (gst_bt_dbg_time.enable == TRUE)
    {
        gst_bt_dbg_time.cur_time = sys_get_ab_timer();
        if (gst_bt_dbg_time.cur_time - gst_bt_dbg_time.last_time >= gst_bt_dbg_time.time_out)
        {
            if (gst_bt_dbg_time.timeout_cbk != NULL)
            {
                gst_bt_dbg_time.timeout_cbk();
            }
            gst_bt_dbg_time.last_time = gst_bt_dbg_time.cur_time;
            gst_bt_dbg_time.enable = FALSE;
        }
    }
}

#endif

