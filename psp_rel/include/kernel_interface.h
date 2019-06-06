/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     kernel_interface.h
 * \brief    内核接口列表
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __KERNEL_DEFINE_H__
#define __KERNEL_DEFINE_H__

#include "attr.h"
#include "typeext.h"
#include "hal_dma_extern.h"
#include "hal_timer_extern.h"

#define UART_PRINT_MODE_DISABLE  0
#define UART_PRINT_MODE_AHB      1
#define UART_PRINT_MODE_DMA      2

typedef struct
{
    uint32 var[8];
} print_info_mem_t;

typedef struct
{
    uint8 print_mode;
    uint8 uart_idx;
    uint8 dma_chan;
    uint16 cache_len;
    uint8 *cache_buf;
} uart_print_info_t;

#ifndef _ASSEMBLER_
typedef enum
{
    LOCAL_IRQ_SAVE = 1,
    LOCAL_IRQ_RESTORE,

    MEMCPY, /*LIBC*/
    MEMSET, /*LIBC*/
    MEMCMP, /*LIBC*/

    MDELAY,
    UDELAY,

    PRINT_BY_CPU_UART_INIT, /*LIBC*/
    PRINTF, /*LIBC*/

} kernel_cmd_e_rom_0; /*需要提供给启动阶段的模块调用，此时系统尚未装载*/


typedef enum
{
    GET_AB_TIMER = 0,
    GET_AB_TIMER_US,

    OS_TIME_DLY,

    LOCAL_INTC_SAVE,
    LOCAL_INTC_RESTORE,

    OS_SCHED_LOCK,
    OS_SCHED_UNLOCK,

} kernel_cmd_e_rom_1; /*要求在系统装载之后才能调用*/


typedef enum
{
    ADJUST_CLK = 0,
    MDELAY_FIX = 1,
    SYS_RAM_RESERVE_API1 = 2,
    SYS_RAM_RESERVE_API2 = 3,
} kernel_cmd_e_ram_0;  /*要求在系统装载之后才能调用*/


typedef enum
{
    EXECE_AP = 0,
    LOAD_DATA = 1,
    FREE_AP = 2,
    FREE_CODEC = 3,

    RESERVE_API_1 = 4,
    DRV_INSTALL = 5,
    DRV_UNINSTALL = 6,
    GET_DRV_INSTALL_INFO = 7,
    RESERVE_API_2 = 8,

    SD_FOPEN = 9,
    SD_FCLOSE = 10,
    SD_FSEEK = 11,
    SD_FTELL = 12,
    SD_FREAD = 13,

    RES_FOPEN = 14,
    RES_FCLOSE = 15,
    RES_FSEEK = 16,
    RES_FTELL = 17,
    RES_FREAD = 18,

    VM_READ = 19,
    VM_WRITE = 20,

    REQUEST_IRQ = 21,
    FREE_IRQ = 22,

    KER_MALLOC = 23,
    KER_FREE = 24,

    MQ_SEND = 25, /*NOTE!! HY HDMI.a will call this API, FIX to 25*/
    MQ_RECEIVE = 26,
    MQ_FLUSH = 27,

    GET_FW_INFO = 28,

    SET_IRQ_TIMER1 = 29,
    DEL_IRQ_TIMER1 = 30,

    MOUNT_FS = 31,
    UNMOUNT_FS = 32,
    GET_FAT_TYPE_AFTER_MOUNT = 33,

    OS_TASK_RESUME = 34,
    OS_TASK_SUSPEND = 35,

    OS_TIME_DLY_RESUME = 36,

    RANDOM = 37,

    REQUEST_CLKADJUST = 38,
    FREE_CLKADJUST = 39,
    RAM_SELECT = 40,

    GET_SYSTEM_INFO = 41,
    SET_SYSTEM_INFO = 42,

    SHM_CTRL_CREAT = 43,
    SHM_CTRL_DESTROY = 44,
    SHM_CTRL_MOUNT = 45,

    DRV_LOAD_TEXT1 = 46,

    //dma
    RESET_DMA_CHAN = 47,
    SET_DMA_CHAN = 48,
    START_DMA_CHAN = 49,
    WAIT_DMA_CHAN_TC = 50,

    GET_SYS_HAL_CBK = 51,
    PERFORMANCE_MONITORING_RESTART = 52,
    PERFORMANCE_MONITORING_STOP = 53,
    PERFORMANCE_MONITORING_REPORT = 54,

    GET_POWERON_INFO = 55,

    MMU_ACCESS_ERROR_CHECK = 56,

    //spdif rx
    ENABLE_SPDIFRX = 57,
    DISABLE_SPDIFRX = 58,
    GET_SPDIFRX_RATE = 59,

    CMU_SSC_ENABLE = 60,

    FORMAT_DISK = 61,

    RESERVE_API_3 = 62,
    RESERVE_API_X = 71,
} kernel_cmd_e_1;

typedef enum
{
    //获取系统全局变量g_sys_global_info的指针
    GET_SYS_GLOBAL_DATA_INFO = 0,
    //获取USB stub是否注册的信息: 0:还未注册; 1:已注册,STUB相关接口可以调用了
    GET_STUB_LOGIN_STATE = 1,
} get_sys_info_cmd_e;

typedef enum
{
    //系统uart打印开关设置
    SET_SYS_UART_PRINT_SWITCH = 0,
    //stub打印开关设置
    SET_SYS_STUB_PRINT_SWITCH,
    //card是否使用MCU_PLL
    SET_CARD_MCUPLL_USE,
    //设置中断入口回调函数
    SET_INT_HOOK_HANDLER,
} set_sys_info_cmd_e;


/*
 KERNEL向上层提供多个函数指针, 以数组形式存放起来, 以下是这些函数指针地址
 */
#define SYS_ROM_ENTRY_ADDR(x)           (0xBFC04200 + (4 * x))
#define SYS_ROM_RAM_ENTRY_ADDR(x)       (0xBFC20000 + (4 * x))
#define SYS_RAM_ENTRY_ADDR(x)           (0xBFC20080 + (4 * x))
#define SYS_INTERFACE_ENTRY_ADDR(x)     (KERNEL_ENTRY_VA_START + (4 * x))

#define SYS_LOCAL_IRQ_SAVE_ENTRY        (*((uint32*)(SYS_ROM_ENTRY_ADDR(LOCAL_IRQ_SAVE))))
#define SYS_LOCAL_IRQ_RESTORE_ENTRY     (*((uint32*)(SYS_ROM_ENTRY_ADDR(LOCAL_IRQ_RESTORE))))

#define SYS_EXECE_AP_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(EXECE_AP))))
#define SYS_LOAD_DATA_ENTRY             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(LOAD_DATA))))
#define SYS_FREE_AP_ENTRY               (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(FREE_AP))))
#define SYS_FREE_CODEC_ENTRY            (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(FREE_CODEC))))

#define SYS_RESERVE_API_1_ENTRY         (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RESERVE_API_1))))
#define SYS_DRV_INSTALL_ENTRY           (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(DRV_INSTALL))))
#define SYS_DRV_LOAD_TEXT1_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(DRV_LOAD_TEXT1))))
#define SYS_DRV_UNINSTALL_ENTRY         (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(DRV_UNINSTALL))))
#define SYS_GET_DRV_INSTALL_INFO_ENTRY  (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_DRV_INSTALL_INFO))))
#define SYS_RESERVE_API_2_ENTRY         (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RESERVE_API_2))))
#define SYS_GET_SYSTEM_INFO_ENTRY       (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_SYSTEM_INFO))))
#define SYS_SET_SYSTEM_INFO_ENTRY       (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SET_SYSTEM_INFO))))

#define SYS_SD_FOPEN_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SD_FOPEN))))
#define SYS_SD_FCLOSE_ENTRY             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SD_FCLOSE))))
#define SYS_SD_FSEEK_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SD_FSEEK))))
#define SYS_SD_FTELL_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SD_FTELL))))
#define SYS_SD_FREAD_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SD_FREAD))))

#define SYS_RES_FOPEN_ENTRY             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RES_FOPEN))))
#define SYS_RES_FCLOSE_ENTRY            (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RES_FCLOSE))))
#define SYS_RES_FSEEK_ENTRY             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RES_FSEEK))))
#define SYS_RES_FTELL_ENTRY             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RES_FTELL))))
#define SYS_RES_FREAD_ENTRY             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RES_FREAD))))

#define SYS_VM_READ_ENTRY               (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(VM_READ))))
#define SYS_VM_WRITE_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(VM_WRITE))))

#define SYS_REQUEST_IRQ_ENTRY           (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(REQUEST_IRQ))))
#define SYS_FREE_IRQ_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(FREE_IRQ))))

#define SYS_MALLOC_ENTRY                (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(KER_MALLOC))))
#define SYS_FREE_ENTRY                  (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(KER_FREE))))

#define SYS_MQ_SEND_ENTRY               (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(MQ_SEND))))
#define SYS_MQ_RECEIVE_ENTRY            (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(MQ_RECEIVE))))
#define SYS_MQ_FLUSH_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(MQ_FLUSH))))

#define SYS_GET_FW_INFO_ENTRY           (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_FW_INFO))))

#define SYS_GET_AB_TIMER_ENTRY          (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(GET_AB_TIMER))))
#define SYS_GET_AB_TIMER_US_ENTRY       (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(GET_AB_TIMER_US))))

#define SYS_MDELAY_ENTRY                (*((uint32*)(SYS_ROM_ENTRY_ADDR(MDELAY))))
#define SYS_UDELAY_ENTRY                (*((uint32*)(SYS_ROM_ENTRY_ADDR(UDELAY))))

#define SYS_SET_IRQ_TIMER1_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SET_IRQ_TIMER1))))
#define SYS_DEL_IRQ_TIMER1_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(DEL_IRQ_TIMER1))))

#define SYS_MOUNT_FS_ENTRY              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(MOUNT_FS))))
#define SYS_UNMOUNT_FS_ENTRY            (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(UNMOUNT_FS))))
#define SYS_GET_FAT_TYPE_AFTER_MOUNT_ENTRY  \
                                        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_FAT_TYPE_AFTER_MOUNT))))

#define SYS_OS_TASK_RESUME_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(OS_TASK_RESUME))))
#define SYS_OS_TASK_SUSPEND_ENTRY       (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(OS_TASK_SUSPEND))))

#define SYS_OS_TIME_DLY_ENTRY           (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(OS_TIME_DLY))))
#define SYS_OS_TIME_DLY_RESUME_ENTRY    (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(OS_TIME_DLY_RESUME))))

#define SYS_LOCAL_INTC_SAVE_ENTRY       (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(LOCAL_INTC_SAVE))))
#define SYS_LOCAL_INTC_RESTORE_ENTRY    (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(LOCAL_INTC_RESTORE))))

#define SYS_OS_SCHED_LOCK_ENTRY         (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(OS_SCHED_LOCK))))
#define SYS_OS_SCHED_UNLOCK_ENTRY       (*((uint32*)(SYS_ROM_RAM_ENTRY_ADDR(OS_SCHED_UNLOCK))))

#define SYS_RANDOM_ENTRY                (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RANDOM))))

#define SYS_REQUEST_CLKADJUST_ENTRY     (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(REQUEST_CLKADJUST))))
#define SYS_FREE_CLKADJUST_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(FREE_CLKADJUST))))
#define SYS_ADJUST_CLK_ENTRY            (*((uint32*)(SYS_RAM_ENTRY_ADDR(ADJUST_CLK))))
#define SYS_MDELAY_FIX_ENTRY            (*((uint32*)(SYS_RAM_ENTRY_ADDR(MDELAY_FIX))))

#define SYS_RAM_SELECT_ENTRY            (*((uint32*)(SYS_RAM_ENTRY_ADDR(RAM_SELECT))))

#define SYS_SHM_CTRL_CREAT_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SHM_CTRL_CREAT))))
#define SYS_SHM_CTRL_DESTROY_ENTRY      (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SHM_CTRL_DESTROY))))
#define SYS_SHM_CTRL_MOUNT_ENTRY        (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SHM_CTRL_MOUNT))))

#define SYS_RESET_DMA_CHAN              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(RESET_DMA_CHAN))))
#define SYS_SET_DMA_CHAN                (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(SET_DMA_CHAN))))
#define SYS_START_DMA_CHAN              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(START_DMA_CHAN))))
#define SYS_WAIT_DMA_CHAN_TC            (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(WAIT_DMA_CHAN_TC))))

#define SYS_GET_SYS_HAL_CBK             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_SYS_HAL_CBK))))

#define SYS_PERFORMANCE_MONITORING_RESTART (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(PERFORMANCE_MONITORING_RESTART))))
#define SYS_PERFORMANCE_MONITORING_STOP    (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(PERFORMANCE_MONITORING_STOP))))
#define SYS_PERFORMANCE_MONITORING_REPORT  (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(PERFORMANCE_MONITORING_REPORT))))

#define SYS_GET_POWERON_INFO_ENTRY      (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_POWERON_INFO))))

#define SYS_MMU_ACCESS_ERROR_CHECK      (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(MMU_ACCESS_ERROR_CHECK))))

#define AUD_ENABLE_SPDIFRX              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(ENABLE_SPDIFRX))))
#define AUD_DISABLE_SPDIFRX             (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(DISABLE_SPDIFRX))))
#define AUD_GET_SPDIFRX_RATE            (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(GET_SPDIFRX_RATE))))

#define SYS_CMU_SSC_ENABLE              (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(CMU_SSC_ENABLE))))

#define SYS_FORMAT_DISK_ENTRY           (*((uint32*)(SYS_INTERFACE_ENTRY_ADDR(FORMAT_DISK))))

/*
 kernel函数调用接口,通过函数指针方式调用
 */

/* ROM开头全大写API接口，是无需系统加载即可调用的，可供启动阶段调用 */

#define ROM_SYS_LOCAL_IRQ_SAVE()            ((uint32(*)(void))SYS_LOCAL_IRQ_SAVE_ENTRY)()
#define ROM_SYS_LOCAL_IRQ_RESTORE(a)        ((void(*)(uint32 ))SYS_LOCAL_IRQ_RESTORE_ENTRY)(a)

#define ROM_SYS_MDELAY(a)                   ((void(*)(uint32 num))SYS_MDELAY_ENTRY)(a)
#define ROM_SYS_UDELAY(a)                   ((void(*)(uint32 num))SYS_UDELAY_ENTRY)(a)


#define sys_exece_ap(a,b,c)             ((int(*)(char* , bool , void* ))SYS_EXECE_AP_ENTRY)((a), (b), (c))
#define sys_load_data(a,b)              ((int(*)(char* , bool ))SYS_LOAD_DATA_ENTRY)((a), (b))
#define sys_local_intc_save(a)          ((uint32(*)(uint32 ))SYS_LOCAL_INTC_SAVE_ENTRY)(a)
#define sys_free_ap(a)                  ((void(*)(bool ))SYS_FREE_AP_ENTRY)(a)
#define sys_free_codec(a)               ((void(*)(bool ))SYS_FREE_CODEC_ENTRY)(a)
#define sys_local_intc_restore(a)       ((void(*)(uint32 ))SYS_LOCAL_INTC_RESTORE_ENTRY)(a)

#define sys_reserve_api_1()             ((int(*)(void))SYS_RESERVE_API_1_ENTRY)()
#define sys_drv_install(a,b,c)          ((void*(*)(uint32 , uint32, void*))SYS_DRV_INSTALL_ENTRY)((a), (b), (c))
#define sys_drv_uninstall(a)            ((int(*)(uint32 ))SYS_DRV_UNINSTALL_ENTRY)(a)
#define sys_drv_load_text1(a)           ((int(*)(const char *drv_name))SYS_DRV_LOAD_TEXT1_ENTRY)(a)
#define sys_get_drv_install_info(a)     ((int(*)(uint32 ))SYS_GET_DRV_INSTALL_INFO_ENTRY)(a)
#define sys_reserve_api_2()             ((int(*)(void))SYS_RESERVE_API_2_ENTRY)()
#define sys_get_system_info(a)          ((void* (*)(uint32 module_id))SYS_GET_SYSTEM_INFO_ENTRY)(a)
#define sys_set_system_info(a, b)       ((int (*)(uint32 module_id, void* info))SYS_SET_SYSTEM_INFO_ENTRY)((a), (b))

#define sys_sd_fopen(a)                 ((sd_file_t*(*)(const char* ))SYS_SD_FOPEN_ENTRY)(a)
#define sys_sd_fclose(a)                ((int(*)(sd_file_t* ))SYS_SD_FCLOSE_ENTRY)(a)
#define sys_sd_fseek(a,b,c)             ((int(*)(sd_file_t* , uint8 , int32 ))SYS_SD_FSEEK_ENTRY)((a), (b), (c))
#define sys_sd_ftell(a)                 ((uint32(*)(sd_file_t* ))SYS_SD_FTELL_ENTRY)(a)
#define sys_sd_fread(a,b,c)             ((int32(*)(sd_file_t* , void* , uint32 ))SYS_SD_FREAD_ENTRY)((a), (b), (c))

#define sys_res_fopen(a)                ((res_file_t*(*)(const char* ))SYS_RES_FOPEN_ENTRY)(a)
#define sys_res_fclose(a)               ((int(*)(res_file_t* ))SYS_RES_FCLOSE_ENTRY)(a)
#define sys_res_fseek(a,b,c)            ((int(*)(res_file_t* , uint8 , int32 ))SYS_RES_FSEEK_ENTRY)((a), (b), (c))
#define sys_res_ftell(a)                ((uint32(*)(res_file_t* ))SYS_RES_FTELL_ENTRY)(a)
#define sys_res_fread(a,b,c)            ((int32(*)(res_file_t* , void* , uint32 ))SYS_RES_FREAD_ENTRY)((a), (b), (c))

#ifndef pc
#define sys_vm_read(a,b,c)              ((int(*)(void* , uint32 , uint32 ))SYS_VM_READ_ENTRY)((a), (b), (c))
#define sys_vm_write(a,b,c)             ((int(*)(void* , uint32 , uint32 ))SYS_VM_WRITE_ENTRY)((a), (b), (c))
#else
#define sys_vm_read(a,b,c)              sim_vm_read(a,(unsigned int)b, (unsigned int)c)
#define sys_vm_write(a,b,c)             sim_vm_write(a,(unsigned int)b)
#endif

#define sys_request_irq(a,b)            ((int(*)(uint32 , interrupt_handle_t ))SYS_REQUEST_IRQ_ENTRY)((a), (b))
#define sys_free_irq(a)                 ((void(*)(uint32 ))SYS_FREE_IRQ_ENTRY)(a)

#define sys_malloc(a)                   ((void*(*)(uint32 ))SYS_MALLOC_ENTRY)((a))
#define sys_free(a)                     ((void(*)(void* ))SYS_FREE_ENTRY)(a)

#define sys_mq_send(a,b)                ((int(*)(uint8 , void* ))SYS_MQ_SEND_ENTRY)((a), (b))
#define sys_mq_receive(a,b)             ((int(*)(uint8 , void* ))SYS_MQ_RECEIVE_ENTRY)((a), (b))
#define sys_mq_flush(a)                 ((int(*)(uint8 ))SYS_MQ_FLUSH_ENTRY)(a)

#define sys_get_fw_info(a,b,c)          ((int(*)(uint8* , uint32 , uint32 ))SYS_GET_FW_INFO_ENTRY)((a), (b), (c))

#define sys_get_ab_timer()              ((uint32(*)(void))SYS_GET_AB_TIMER_ENTRY)()
#define sys_get_ab_timer_us()           ((uint32(*)(void))SYS_GET_AB_TIMER_US_ENTRY)()
#define sys_mdelay(a)                   ((void(*)(uint32 num))SYS_MDELAY_FIX_ENTRY)(a)
#define sys_udelay(a)                   ((void(*)(uint32 num))SYS_UDELAY_ENTRY)(a)

#define sys_set_irq_timer1(a,b)         ((int(*)(void(*)(void ), uint16 ))SYS_SET_IRQ_TIMER1_ENTRY)((a), (b))
#define sys_del_irq_timer1(a)           ((int(*)(uint32 ))SYS_DEL_IRQ_TIMER1_ENTRY)(a)

#define sys_mount_fs(a,b,c)             ((uint32(*)(uint8 drv_type, uint8 disk, uint8 work_mode))SYS_MOUNT_FS_ENTRY)((a), (b), (c))
#define sys_unmount_fs(a)               ((int(*)(uint32 vfs_mount))SYS_UNMOUNT_FS_ENTRY)(a)
#define sys_get_fat_type_after_mount()  ((uint8(*)(void))SYS_GET_FAT_TYPE_AFTER_MOUNT_ENTRY)()

#define sys_format_disk(a,b,c)          ((int(*)(uint8 dry_type, uint8 partition_num, uint8 fat_type))SYS_FORMAT_DISK_ENTRY)((a), (b), (c))

#ifndef pc
#define sys_local_irq_save()            ((uint32(*)(void))SYS_LOCAL_IRQ_SAVE_ENTRY)()
#define sys_local_irq_restore(a)        ((void(*)(uint32 ))SYS_LOCAL_IRQ_RESTORE_ENTRY)(a)
#else
#define sys_local_irq_save() 1
#define sys_local_irq_restore(a)1
#endif

#define sys_os_task_resume(a)           ((INT8U(*)(INT8U ))SYS_OS_TASK_RESUME_ENTRY)(a)
#define sys_os_task_suspend(a)          ((INT8U(*)(INT8U ))SYS_OS_TASK_SUSPEND_ENTRY)(a)

#define sys_os_time_dly(a)              ((void(*)(INT16U ))SYS_OS_TIME_DLY_ENTRY)(a)
#define sys_os_time_dly_resume(a)       ((INT8U(*)(INT8U ))SYS_OS_TIME_DLY_RESUME_ENTRY)(a)

#define sys_os_sched_lock()             ((void(*)(void))SYS_OS_SCHED_LOCK_ENTRY)()
#define sys_os_sched_unlock()           ((void(*)(void))SYS_OS_SCHED_UNLOCK_ENTRY)()

#define sys_random()                    ((uint32(*)(void))SYS_RANDOM_ENTRY)()

#define sys_request_clkadjust(a)        ((int(*)(void(*)(const frequency_param_t* )))SYS_REQUEST_CLKADJUST_ENTRY)(a)
#define sys_free_clkadjust(a)           ((int(*)(uint32 ))SYS_FREE_CLKADJUST_ENTRY)(a)

#ifndef pc
#define sys_adjust_clk(a, b)            ((freq_level_e(*)(freq_level_e , uint8 ))SYS_ADJUST_CLK_ENTRY)((a), (b))
#else
#define sys_adjust_clk(a, b)            1
#endif

#define RAM_BLK_RAM3   0
#define RAM3_CLK_CPU   0
#define RAM3_CLK_VAD   1

#define RAM_BLK_RAM4   1
#define RAM4_CLK_CPU   0
#define RAM4_CLK_VAD   1
#define RAM4_CLK_DAC   2

#define RAM_BLK_PCMRAM 2
#define PCMRAM_CLK_CPU 0
#define PCMRAM_CLK_DAC 1

#define RAM_BLK_URAM   3
#define URAM_CLK_CPU   0
#define URAM_CLK_USBC  1

#define sys_ram_select(a, b)            ((void(*)(uint8 , uint8 ))SYS_RAM_SELECT_ENTRY)((a), (b))

/*
 int sys_shm_creat(int8 shm_id, uint8 *shm_addr, uint16 shm_size)
 该接口用于创建共享内存空间
 shm_id：共享内存标识ID
 shm_addr：内存地址
 shm_size：内存大小
 返回值int：0-成功，非0失败
 !!!NOTE:bank代码，禁止在中断调用
 */
#define sys_shm_creat(shm_id,shm_addr,shm_size)       \
((int(*)(int32 , void* , uint32 ))SYS_SHM_CTRL_CREAT_ENTRY)((shm_id),(shm_addr),(shm_size))

/*
 int sys_shm_destroy(int8 shm_id)
 该接口用于删除共享内存空间
 shm_id：共享内存标识ID
 返回值int：0-成功，非0失败
 !!!NOTE:bank代码，禁止在中断调用
 */
#define sys_shm_destroy(shm_id)         \
((int(*)(int32))SYS_SHM_CTRL_DESTROY_ENTRY)((shm_id))

/*
 uint8* sys_shm_mount(int8 shm_id)
 该接口用于请求共享内存地址
 shm_id：共享内存标识ID
 返回值uint8*：共享内存地址，NULL-为请求失败
 !!!NOTE:bank代码，禁止在中断调用
 */
#define sys_shm_mount(shm_id)           \
((uint8*(*)(int32))SYS_SHM_CTRL_MOUNT_ENTRY)((shm_id))

static inline void set_critical_reg(uint32 reg_addr, uint32 set_reg_bit)
{
    uint32 cpu_sr;
    cpu_sr = sys_local_irq_save();
    *(volatile uint32 *) (reg_addr) |= set_reg_bit;
    sys_local_irq_restore(cpu_sr);
}

static inline void clear_critical_reg(uint32 reg_addr, uint32 clr_reg_bit)
{
    uint32 cpu_sr;
    cpu_sr = sys_local_irq_save();
    *(volatile uint32 *) (reg_addr) &= clr_reg_bit;
    sys_local_irq_restore(cpu_sr);
}

//dma interface
#define sys_reset_dma_chan(a)           ((void(*)(dma_chan_e))SYS_RESET_DMA_CHAN)((a))
#define sys_set_dma_chan(a,b)           ((void(*)(dma_chan_e, dma_chan_setting_t *))SYS_SET_DMA_CHAN)((a), (b))
#define sys_start_dma_chan(a,b)         ((void(*)(dma_chan_e, uint32))SYS_START_DMA_CHAN)((a), (b))
#define sys_wait_dma_chan_tc(a,b)       ((int32(*)(dma_chan_e, uint8))SYS_WAIT_DMA_CHAN_TC)((a), (b))

#define sys_get_sys_hal_cbk(a)          ((sys_hal_callback_t *(*)(void))SYS_GET_SYS_HAL_CBK)()

#define sys_performance_monitoring_restart()   ((void(*)(void))SYS_PERFORMANCE_MONITORING_RESTART)()
#define sys_performance_monitoring_stop()      ((void(*)(void))SYS_PERFORMANCE_MONITORING_STOP)()
#define sys_performance_monitoring_report()    ((void(*)(void))SYS_PERFORMANCE_MONITORING_REPORT)()

#define sys_get_poweron_info(a)                ((void(*)(sys_poweron_info_t *))SYS_GET_POWERON_INFO_ENTRY)((a))

#define sys_mmu_access_error_check()           ((void(*)(void))SYS_MMU_ACCESS_ERROR_CHECK)()

#if 0
extern void hal_audio_enable_spdifrx(void);
#endif
#define enable_spdifrx()            ((void(*)(void))AUD_ENABLE_SPDIFRX)()

#if 0
extern void hal_audio_disable_spdifrx(void);
#endif
#define disable_spdifrx()           ((void(*)(void))AUD_DISABLE_SPDIFRX)()

#if 0
//error_check:bit0表示检测并请pending，bit1表示打印错误信息
extern uint32 hal_audio_get_spdifrx_rate(uint8 error_check, uint32 *pd);
#endif
#define get_spdifrx_rate(a,b)       ((uint32(*)(uint8, uint32 *))AUD_GET_SPDIFRX_RATE)((a),(b))

#if 0
extern void hal_cmu_ssc_enable(bool enable);
#endif
#define cmu_ssc_enable(a)           ((void(*)(bool))SYS_CMU_SSC_ENABLE)((a))

#endif //_ASSEMBLER_

#endif //__KERNEL_DEFINE_H__
