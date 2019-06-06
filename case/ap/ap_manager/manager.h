/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用管理器头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _MANAGER_H
#define _MANAGER_H

#include  "applib.h"
#include  "common_func.h"
#include  "config_common.h"

#define APP_NAME_MAX   12 //8+.+ap+'\0'

extern void manager_msg_callback(private_msg_t *pri_msg);
extern bool manager_get_app_name(char *namebuf, uint8 ap_id);
extern app_result_e manager_message_loop(void);

#endif

