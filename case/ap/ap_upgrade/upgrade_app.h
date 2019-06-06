/*!
 * \file      upgrade_app.h
 * \brief     升级应用头文件
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#ifndef _UPGRADE_APP_H
#define _UPGRADE_APP_H


#ifndef PRINT_LEVEL
#define PRINT_LEVEL  PRINT_LEVEL_DEBUG
#endif

#include "common_front_app_def.h"


#define _OK    TRUE
#define _FAIL  FALSE

typedef uint8   u8_t;
typedef uint32  u32_t;
typedef uint32  uint_t;

#define __reg32(_reg)  (*(volatile u32_t*)(_reg))


#define SUPPORT_HW_KEYTONE  1
#undef  keytone_play


typedef struct
{
    u8_t   ctrl_no;
    u8_t   tx_gpio_no;
    u8_t   tx_mfp_sel;
    u32_t  baudrate;
    
} uart_ctrl_configs_t;


enum UPGRADE_MODE
{
    UPGRADE_MODE_FACTORY  = 1,
    UPGRADE_MODE_END_USER = 2,
};


typedef struct
{
    u32_t  checksum;
    u8_t   format[4];
    u32_t  load_size;
    u32_t  load_addr;
    u32_t  main_func;
    
} upgrade_header_t;


typedef struct
{
    int (*printf)(const char* format, ...);
    
} upgrade_api_t;


typedef struct
{
    uart_ctrl_configs_t  cfg_uart;
    
    upgrade_api_t  api;
    
    u8_t*  upg_name;
    u8_t   upg_mode;
    u8_t   upg_disk;
    
    void  (*show_progress)(int percent);
    
} upgrade_params_t;


typedef struct
{
    u32_t  vfs_mount;
    u8_t*  upg_name;
    u32_t  upg_file;
    u8_t   upg_mode;

    upgrade_params_t  upg_params;
    
} upgrade_app_context_t;


static inline upgrade_app_context_t* upgrade_app_get_context(void)
{
    extern upgrade_app_context_t  upgrade_app_context;

    return &upgrade_app_context;
}


/* 打开升级文件
 */
bool upgrade_file_open(u8_t disk);


/* 读取升级文件
 */
bool upgrade_file_read(void* buf, u32_t offs, int len);


/* 是否重启
 */
void sys_enable_reboot(bool enable);


/* LED 灯控制
 */
void led_light_ctrl(bool on_off);


/* 按键音播放
 */
void keytone_play(uint_t freq, uint_t period);


#endif  // _UPGRADE_APP_H


