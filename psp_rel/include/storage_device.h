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
 * \file     storage_device.h
 * \brief    块设备驱动接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef _STORAGE_DEVICE_H
#define _STORAGE_DEVICE_H

#define PLUG_OUT  -1
#define PLUG_IN 0
#define CHARGING 1

#include <ucos/init.h>
#include "api.h"

#ifndef _ASSEMBLER_

typedef enum
{
    BASE_VRAM_READ = 0,
    BASE_VRAM_WRITE = 1,
    BASE_GET_INFO = 2,
    BASE_SET_CONFIG = 3,
    BASE_VRAM_MERGE = 4,
    BASE_RES_READ = 5,
    BASE_RES_SEARCH = 6,
    BASE_SPI_CLOCK_SET = 7,
    BASE_SPI_CLOCK_PLL_SET = 8,
    BASE_RESERV_API1 = 9,
    BASE_RESERV_API2 = 10,    
    BASE_RESERV_API3 = 11,   
    BASE_RESERV_API4 = 12,
    BASE_RESERV_API5 = 13,
    BASE_RESERV_API6 = 14,
} base_stg_cmd_e;


typedef enum
{
    START_RW = 0,
    STOP_RW,
} rw_status_e;


typedef enum
{
    SPI_FREQ_1M = 0,
    SPI_FREQ_2M,
    SPI_FREQ_3M,
    SPI_FREQ_4M,
    SPI_FREQ_6M,
    SPI_FREQ_13M,
    SPI_FREQ_26M,
    SPI_FREQ_48M,
} spi_freq_level_e;


typedef enum
{
    PLL_DIV_01 = 0,
    PLL_DIV_02,
    PLL_DIV_04,
    PLL_DIV_08,
}pll_div_e;


typedef enum
{
    B_PLL_DIV_01 = 0,
    B_PLL_DIV_02,
    B_PLL_DIV_03,
    B_PLL_DIV_04,
    B_PLL_DIV_05,
    B_PLL_DIV_06,
    B_PLL_DIV_07,
    B_PLL_DIV_08,
    B_PLL_DIV_09,
    B_PLL_DIV_10,
    B_PLL_DIV_11,
    B_PLL_DIV_12,
    B_PLL_DIV_13,
    B_PLL_DIV_14,
    B_PLL_DIV_15,
    B_PLL_DIV_16,
    B_PLL_DIV_17,
    B_PLL_DIV_18,
    B_PLL_DIV_19,
    B_PLL_DIV_20,
    B_PLL_DIV_21,
    B_PLL_DIV_22,
    B_PLL_DIV_23,
    B_PLL_DIV_24,
    B_PLL_DIV_25,
    B_PLL_DIV_26,
    B_PLL_DIV_27,
    B_PLL_DIV_28,
    B_PLL_DIV_29,
    B_PLL_DIV_30,
    B_PLL_DIV_1_5,
    B_PLL_DIV_2_5,
} b_pll_div_e;


typedef struct
{
    uint8 iscorepll;
    uint8 pllvalue;
    uint8 maxspiclk;
}sysclk_param_t;

//get base storage related info
#define BASE_GET_BASE_TYPE              (0)
#define BASE_GET_CAPACITY               (1)
#define BASE_GET_BLOCK_SIZE             (2)
#define BASE_GET_FLASH_ID               (3)
#define BASE_GET_TEST_KEY               (4)
#define BASE_GET_LFI_START_ADDR         (5)
#define BASE_GET_WRITE_PROTECT          (6)

#define BASE_GET_SDK_TYPE               (8)
#define BASE_GET_CHIP_TYPE              (9)
#define BASE_GET_SUB_CHIP_TYPE          (10)
#define BASE_GET_ADFU_LAUNCHER          (11)
#define BASE_CHECK_WP_SET               (12)
#define BASE_SUPPORT_4K_ERASE           (13)


//set base storage related config
#define BASE_SET_VRAM_BUFFER            (0)
#define BASE_ENABLE_INTERRUPT_LOCK      (1)
#define BASE_DISABLE_INTERRUPT_LOCK     (2)


/*
 对上层提供的接口函数指针, 以数组形式存放起来, 以下是这些函数指针地址
*/
#define BASE_INTERFACE_ENTRY_ADDR(x)    (BASE_ENTRY_VA_START + (4 * x))

#define BASE_VRAM_READ_ENTRY            (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_VRAM_READ))))
#define BASE_VRAM_WRITE_ENTRY           (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_VRAM_WRITE))))
#define BASE_GET_INFO_ENTRY             (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_GET_INFO))))
#define BASE_SET_CONFIG_ENTRY           (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_SET_CONFIG))))
#define BASE_VRAM_MERGE_ENTRY           (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_VRAM_MERGE))))
#define BASE_RES_READ_ENTRY             (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_RES_READ))))
#define BASE_RES_SEARCH_ENTRY           (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_RES_SEARCH))))
#define BASE_SPI_CLOCK_SET_ENTRY        (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_SPI_CLOCK_SET))))
#define BASE_SPI_CLOCK_PLL_SET_PLL_ENTRY  (*((uint32*)(BASE_INTERFACE_ENTRY_ADDR(BASE_SPI_CLOCK_PLL_SET))))


/*
 函数调用接口,通过函数指针方式调用
*/
#define base_vram_read(a, b, c)         ((int32(*)(uint8* r_buf, uint32 index, uint32 bytes))BASE_VRAM_READ_ENTRY)((a), (b), (c))
#define base_vram_write(a, b, c)        ((int32(*)(uint8* w_buf, uint32 index, uint32 bytes))BASE_VRAM_WRITE_ENTRY)((a), (b), (c))
#define base_get_info(a)                ((uint32(*)(uint32 info_type))BASE_GET_INFO_ENTRY)(a)
#define base_set_config(a, b)           ((int32(*)(uint32 info_type, void* value))BASE_SET_CONFIG_ENTRY)((a), (b))
#define base_res_read(a, b, c)          ((int32(*)(uint32 byte_addr, uint8 *buf, uint32 byte_num))BASE_RES_READ_ENTRY)((a), (b), (c))
#define base_res_search(a)              ((int32(*)(uint8 *name))BASE_RES_SEARCH_ENTRY)((a))
/*
int32 base_vram_merge(void)
功能描述：vram数据整理
参数说明：
无
返回值：0-可用空间还很大,无需整理; 大于0-数据整理完成
*/
#define base_vram_merge()               ((int32(*)(void))BASE_VRAM_MERGE_ENTRY)()
#define adjust_spi_clock(a)             ((uint32(*)(uint32 nor_clock))BASE_SPI_CLOCK_SET_ENTRY)(a)
#define adjust_spi_clock_pll(a)         ((uint32(*)(sysclk_param_t *))BASE_SPI_CLOCK_PLL_SET_PLL_ENTRY)(a)



typedef enum
{
    DEVICE_READ = 0,
    DEVICE_WRITE,
    DEVICE_UPDATE,
    DEVICE_GET_INFO,
    DEVICE_SET_RW_STATUS,
} device_cmd_e;

//获取存储设备的命令信息
#define DEV_GET_SECTOR_SIZE             (0)
#define DEV_GET_STG_TYPE                (1)
#define DEV_GET_CAPACITY                (2)
#define DEV_GET_WRITE_PROTECT           (3)

/*
 对上层提供的接口函数指针, 以数组形式存放起来, 以下是这些函数指针地址
*/
#define DEVICE_INTERFACE_ENTRY_ADDR(x)  (STG_DRV_ENTRY_VA_START + (4 * x))

#define DEVICE_READ_ENTRY               (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_READ))))
#define DEVICE_WRITE_ENTRY              (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_WRITE))))
#define DEVICE_UPDATE_ENTRY             (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_UPDATE))))
#define DEVICE_SET_RW_STATUS_ENTRY      (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_SET_RW_STATUS))))
#define DEVICE_GET_INFO_ENTRY           (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_GET_INFO))))

/*
 函数调用接口,通过函数指针方式调用
*/

/*
uint32 device_read(uint32 lba, uint8 *buf, uint32 sec_num)
功能描述：从存储设备中读取一定大小的数据
参数说明：
lba：存储设备逻辑块地址;
buf: 存储数据的缓存空间
sec_num：读的扇区数
返回值：0-读取成功；非0-读取失败
*/
#define device_read(a, b, c)            ((int32(*)(UINT32 addr, UINT8* r_buf, UINT32 sectors))DEVICE_READ_ENTRY)((a), (b), (c))

/*
uint32 device_write(uint32 lba, uint8 *buf, uint32 sec_num)
功能描述：往存储设备中写入一定大小的数据
参数说明：
lba：存储设备逻辑块地址;
buf: 存放需要写入存储设备的数据缓存空间
sec_num：写的扇区数
返回值：0-写入成功；非0-写入失败
*/
#define device_write(a, b, c)           ((int32(*)(UINT32 addr, UINT8* w_buf, UINT32 sectors))DEVICE_WRITE_ENTRY)((a), (b), (c))
/*
uint32 device_update(void)
功能描述：更新存储设备中的缓存数据到实际物理地址中
参数说明：无
返回值：0-更新成功；非0-更新失败
*/
#define device_update()                 ((int32(*)(void))DEVICE_UPDATE_ENTRY)()

/*
uint32 device_set_rw_status(uint8 status)
功能描述：开始或结束数据设备的设定状态
参数说明：
status:   0-开始进行数据读写；1-结束数据读写
返回值：0-成功；非0-失败
说明:该函数只用于device下读写状态的设置
*/
#define device_set_rw_status(a)                 ((int32(*)(uint8))DEVICE_SET_RW_STATUS_ENTRY)(a)

/*
uint32 device_get_info(uint32 info_type)
功能描述：获取存储设备信息
参数说明：
info_type:需要获取的信息类别，具体参照storage_device.h中的//获取存储设备的命令信息
返回值：实际想获取的信息值
*/
#define device_get_info(a)              ((uint32(*)(uint32 info_type))DEVICE_GET_INFO_ENTRY)(a)


#endif

#endif

