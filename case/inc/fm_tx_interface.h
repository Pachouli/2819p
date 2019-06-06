/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM发射驱动相关宏定义，枚举类型，结构体定义等，为FM驱动和收音机应用共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _FM_TX_INTERFACE_H
#define _FM_TX_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"

/* FM 驱动命令枚举类型*/
typedef enum
{
    FM_TX_MODULE_INSTALL = 0,
    FM_TX_MODULE_UNINSTALL,
    /*FM模组初始化*/
    FM_TX_INIT,
    /*设置频点开始播放*/
    FM_TX_SETFREQ,
} fm_tx_cmd_e;

/* FM 驱动向上层提供的统一入口定义*/
typedef void *(*fm_tx_op_func)(void *, void *, void *);

/* 驱动内部实现的具体接口函数声明*/
typedef struct
{
    fm_tx_op_func FmTxModuleInstall;
    fm_tx_op_func FmTxModuleUninstall;
    fm_tx_op_func TxInit;
    fm_tx_op_func TxSetFreq;
} fm_tx_driver_operations;

#define FM_TX_ENTRY_ADDR(x)        (FM_TX_ENTRY_VA_START + (4 * x))

#define FM_TX_MODULE_INSTALL_ENTRY    (*((uint32*)(FM_TX_ENTRY_ADDR(FM_TX_MODULE_INSTALL))))
#define FM_TX_MODULE_UNINSTALL_ENTRY  (*((uint32*)(FM_TX_ENTRY_ADDR(FM_TX_MODULE_UNINSTALL))))
#define FM_TX_INIT_ENTRY           (*((uint32*)(FM_TX_ENTRY_ADDR(FM_TX_INIT))))
#define FM_TX_SETFREQ_ENTRY        (*((uint32*)(FM_TX_ENTRY_ADDR(FM_TX_SETFREQ))))

#if 0
extern int32 fm_tx_drv_init(void);
#endif
#define fm_tx_module_install()     ((int(*)(void))FM_TX_MODULE_INSTALL_ENTRY)()

#if 0
extern int32 fm_tx_drv_exit(void);
#endif
#define fm_tx_module_uninstall()   ((int(*)(void))FM_TX_MODULE_UNINSTALL_ENTRY)()

#if 0
extern void sFM_TX_Init(uint16 frq);
#endif
#define fm_tx_open(a)              ((void(*)(uint16))FM_TX_INIT_ENTRY)((a))

#if 0
extern void sFM_TX_SetFreq(uint16 frq);
#endif
#define fm_tx_set_freq(a)          ((void(*)(uint16))FM_TX_SETFREQ_ENTRY)((a))


#endif

