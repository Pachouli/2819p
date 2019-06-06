/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开机应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _AP_CONFIG_H
#define _AP_CONFIG_H

#include "common_front_app_def.h"
#include "config_common.h"
#include "bluetooth_common.h"

//#define CONFIG_TEST_EN

//定频测试开关
//#define FCC_TEST_MODE

/* 关机环境变量 */
typedef struct
{
    uint16 magic; //VM 变量魔数
    uint8 ap_id; //关机时的前台应用
} config_var_t;

extern config_var_t g_config_var;
extern sys_poweron_info_t g_config_poweron_info;

extern app_result_e config_poweron_option(void);
extern void config_poweron_dispatch(app_result_e result);

extern void config_power_off(void);

extern void *pa_thread_open(void *param);

extern app_result_e config_upgrade_entry(uint8 disk_type);
extern app_result_e config_autotest_entry(uint8 disk_type);

#define CHARGE_MODE_ON  0 //开机进入充电空闲状态
#define CHARGE_MODE_OFF 1 //关机进入充电空闲状态
extern app_result_e config_charge_loop(uint8 mode);

#endif

