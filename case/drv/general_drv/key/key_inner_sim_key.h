/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动UART模拟按键模块头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "key_interface.h"

#ifndef __KEY_INNER_SIM_KEY__
#define __KEY_INNER_SIM_KEY__

//是否支持UART模拟按键；代码打开，但是要依赖于 g_keymap_para.uart_sim_key_support 配置项也打开才会起作用
#define SUPPORT_UART_SIM_KEY 1

typedef enum
{
    SIM_KEY_NULL,
    SIM_KEY_CHECK_TYPE,
    SIM_KEY_RECE_KEY_DOWN,
    SIM_KEY_RECE_KEY_UP,
    SIM_KEY_ERROR,
} sim_key_fsm_e;

typedef struct
{
    uint8  key_id;
    uint8  key_type;
    uint16 key_time;
} sim_key_t;

typedef struct
{
    sim_key_fsm_e status;
    sim_key_t sim_key;
    bool sendkey;
} sim_key_infor_t;

extern sim_key_infor_t g_sim_key_infor;

extern uint8 key_get_uart_sim_key(void);
extern uint8 key_init_uart_sim_key(void);

#endif
