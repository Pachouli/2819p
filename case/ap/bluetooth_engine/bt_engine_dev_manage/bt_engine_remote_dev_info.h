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
#ifndef _BLUETOOTH_ENGINE_DEV_INFO_H
#define _BLUETOOTH_ENGINE_DEV_INFO_H

extern bool bt_save_remote_phone_name(bt_dev_t *dev, uint8 *full_name);
extern bool bt_get_remote_phone_name(bt_dev_t *dev, uint8 *full_name);
extern bool bt_clear_remote_phone_name(bt_dev_t *dev);
extern void bt_remote_dev_info_init(void);
extern void bt_remote_dev_info_deinit(void);

#endif

