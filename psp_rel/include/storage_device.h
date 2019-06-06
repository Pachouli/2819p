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
 * \brief    ���豸�����ӿ�
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
 ���ϲ��ṩ�Ľӿں���ָ��, ��������ʽ�������, ��������Щ����ָ���ַ
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
 �������ýӿ�,ͨ������ָ�뷽ʽ����
*/
#define base_vram_read(a, b, c)         ((int32(*)(uint8* r_buf, uint32 index, uint32 bytes))BASE_VRAM_READ_ENTRY)((a), (b), (c))
#define base_vram_write(a, b, c)        ((int32(*)(uint8* w_buf, uint32 index, uint32 bytes))BASE_VRAM_WRITE_ENTRY)((a), (b), (c))
#define base_get_info(a)                ((uint32(*)(uint32 info_type))BASE_GET_INFO_ENTRY)(a)
#define base_set_config(a, b)           ((int32(*)(uint32 info_type, void* value))BASE_SET_CONFIG_ENTRY)((a), (b))
#define base_res_read(a, b, c)          ((int32(*)(uint32 byte_addr, uint8 *buf, uint32 byte_num))BASE_RES_READ_ENTRY)((a), (b), (c))
#define base_res_search(a)              ((int32(*)(uint8 *name))BASE_RES_SEARCH_ENTRY)((a))
/*
int32 base_vram_merge(void)
����������vram��������
����˵����
��
����ֵ��0-���ÿռ仹�ܴ�,��������; ����0-�����������
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

//��ȡ�洢�豸��������Ϣ
#define DEV_GET_SECTOR_SIZE             (0)
#define DEV_GET_STG_TYPE                (1)
#define DEV_GET_CAPACITY                (2)
#define DEV_GET_WRITE_PROTECT           (3)

/*
 ���ϲ��ṩ�Ľӿں���ָ��, ��������ʽ�������, ��������Щ����ָ���ַ
*/
#define DEVICE_INTERFACE_ENTRY_ADDR(x)  (STG_DRV_ENTRY_VA_START + (4 * x))

#define DEVICE_READ_ENTRY               (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_READ))))
#define DEVICE_WRITE_ENTRY              (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_WRITE))))
#define DEVICE_UPDATE_ENTRY             (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_UPDATE))))
#define DEVICE_SET_RW_STATUS_ENTRY      (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_SET_RW_STATUS))))
#define DEVICE_GET_INFO_ENTRY           (*((uint32*)(DEVICE_INTERFACE_ENTRY_ADDR(DEVICE_GET_INFO))))

/*
 �������ýӿ�,ͨ������ָ�뷽ʽ����
*/

/*
uint32 device_read(uint32 lba, uint8 *buf, uint32 sec_num)
�����������Ӵ洢�豸�ж�ȡһ����С������
����˵����
lba���洢�豸�߼����ַ;
buf: �洢���ݵĻ���ռ�
sec_num������������
����ֵ��0-��ȡ�ɹ�����0-��ȡʧ��
*/
#define device_read(a, b, c)            ((int32(*)(UINT32 addr, UINT8* r_buf, UINT32 sectors))DEVICE_READ_ENTRY)((a), (b), (c))

/*
uint32 device_write(uint32 lba, uint8 *buf, uint32 sec_num)
�������������洢�豸��д��һ����С������
����˵����
lba���洢�豸�߼����ַ;
buf: �����Ҫд��洢�豸�����ݻ���ռ�
sec_num��д��������
����ֵ��0-д��ɹ�����0-д��ʧ��
*/
#define device_write(a, b, c)           ((int32(*)(UINT32 addr, UINT8* w_buf, UINT32 sectors))DEVICE_WRITE_ENTRY)((a), (b), (c))
/*
uint32 device_update(void)
�������������´洢�豸�еĻ������ݵ�ʵ�������ַ��
����˵������
����ֵ��0-���³ɹ�����0-����ʧ��
*/
#define device_update()                 ((int32(*)(void))DEVICE_UPDATE_ENTRY)()

/*
uint32 device_set_rw_status(uint8 status)
������������ʼ����������豸���趨״̬
����˵����
status:   0-��ʼ�������ݶ�д��1-�������ݶ�д
����ֵ��0-�ɹ�����0-ʧ��
˵��:�ú���ֻ����device�¶�д״̬������
*/
#define device_set_rw_status(a)                 ((int32(*)(uint8))DEVICE_SET_RW_STATUS_ENTRY)(a)

/*
uint32 device_get_info(uint32 info_type)
������������ȡ�洢�豸��Ϣ
����˵����
info_type:��Ҫ��ȡ����Ϣ��𣬾������storage_device.h�е�//��ȡ�洢�豸��������Ϣ
����ֵ��ʵ�����ȡ����Ϣֵ
*/
#define device_get_info(a)              ((uint32(*)(uint32 info_type))DEVICE_GET_INFO_ENTRY)(a)


#endif

#endif

