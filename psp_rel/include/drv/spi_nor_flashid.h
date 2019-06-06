/******************************************************************************
 *                               US212A
 *                            Module: SPI_NOR DRIVER
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2012-12-6 11:19:48          1.0              build this file
 ******************************************************************************/
/*!
 * \file     spi_nor_flashid.h
 * \brief
 * \author   wuyufan
 * \version  1.0
 * \date     2012-12-6
 *******************************************************************************/
#ifndef _SPI_NOR_FLASHID_H
#define _SPI_NOR_FLASHID_H

#include "typeext.h"


#define SUPPORT_SHOW_NORFLASH_NAME


//command code definitions

// Read array command code.
#define SPI_NOR_READ_FAST               (0x0B)

// Read array (low frequency) command code.
#define SPI_NOR_READ_SLOW               (0x03)

// Block erase command code (4K block).
#define SPI_NOR_BLOCK_ERASE_4K          (0x20)

// Block erase command code (32K block).
#define SPI_NOR_BLOCK_ERASE_32K         (0x52)

// Block erase command code (64K block).
#define SPI_NOR_BLOCK_ERASE_64K         (0xD8)

// Chip erase command code 1.
#define SPI_NOR_CHIP_ERASE_1            (0x60)

// Chip erase command code 2.
#define SPI_NOR_CHIP_ERASE_2            (0xC7)

// Byte/page program command code.
#define SPI_NOR_PAGE_PROGRAM            (0x02)

// Sequential program mode command code 1.
#define SPI_NOR_AAI_PROGRAM             (0xAD)

// Sequential program mode command code 2.
#define SPI_NOR_SEQUENTIAL_PROGRAM_2    (0xAF)

// Read status register command code.
#define SPI_NOR_READ_STATUS             (0x05)

#define SPI_NOR_READ_STATUS_HIGHT       (0x35)

// Write status register command code.
#define SPI_NOR_WRITE_STATUS            (0x01)

#define SPI_NOR_WRITE_STATUS_2          (0x31)

// Write enable command code.
#define SPI_NOR_WRITE_ENABLE            (0x06)

// Write disable command code.
#define SPI_NOR_WRITE_DISABLE           (0x04)

// Read ID
#define SPI_NOR_READ_ID                 (0x90)

// Read JEDEC ID
#define SPI_NOR_READ_JEDEC_ID           (0x9F)

// Protect sector command code.
#define SPI_NOR_PROTECT_SECTOR          (0x36)

// Unprotect sector command code.
#define SPI_NOR_UNPROTECT_SECTOR        (0x39)

// Read sector protection registers command code.
#define SPI_NOR_READ_SECTOR_PROT        (0x3C)

// Deep power-down command code.
#define SPI_NOR_DEEP_PDOWN              (0xB9)

// Resume from deep power-down command code.
#define SPI_NOR_RES_DEEP_PDOWN          (0xAB)




/*
 * 某些flash具有多个擦除区域，每个区域大小不同，定义最大允许的擦除区域个数
 */
#define MAX_ERASEREGIONS                (5)

#define ERASE_BLOCK_4K                  (4*1024 /512)
#define ERASE_BLOCK_8K                  (8*1024 /512)
#define ERASE_BLOCK_16K                 (16*1024/512)
#define ERASE_BLOCK_32K                 (32*1024/512)
#define ERASE_BLOCK_64K                 (64*1024/512)
#define ERASE_BLOCK_256K                (256*1024/512)


/*
 * 不同厂家的block size配置不尽相同
 0: support only all 64KB block; 1: support only all 32KB block; 2: support only all 4KB block;
 3: support all 64KB and 32KB block; 4: support all 64KB and 4KB block;
 5: support 3 kind of uniform block: all 64KB, all 32KB, all 4KB;
 */
#define ALL_BLOCK_64KB                  (0)
#define ALL_BLOCK_32KB                  (1)
#define ALL_BLOCK_4KB                   (2)
#define ALL_BLOCK_64KB_32KB             (3)
#define ALL_BLOCK_64KB_4KB              (4)
#define ALL_BLOCK_64KB_32KB_4KB         (5)
#define ALL_BLOCK_32KB_4KB              (6)

/*
 * 2x I/O读模式,不同厂家支持的2x I/O在发送读命令和地址阶段会有差别
 0: NOT support dual output or dual I/O mode
 1: Dual output fast read(CMD:0x3B, 地址以1线模式发出,数据以2线模式读回);
 2: Dual Input Output fast read(CMD:0xBB, 地址和数据均以2线模式操作);
 3: 两种模式均支持
 */
#define NO_DUAL_READ                    (0)
#define DUAL_OUTPUT_READ                (1)
#define DUAL_IO_READ                    (2)
#define DUAL_IO_OUTPUT_READ             (3)



/*Capcity */
#define NOR_CAP_512K                    (1*512*1024/512)
#define NOR_CAP_1M                      (1*1024*1024/512)
#define NOR_CAP_2M                      (2*1024*1024/512)
#define NOR_CAP_4M                      (4*1024*1024/512)
#define NOR_CAP_8M                      (8*1024*1024/512)
#define NOR_CAP_16M                     (16*1024*1024/512)

#define REG_WP_BIT                      (0)
#define REG_SR_BIT                      (1)
#define REG_WSR_TWICE_BIT               (2)
#define REG_SECTOR_PROTECT_BIT          (3)
#define REG_VOLATILE_BIT                (5)

//NOR写保护支持
#define REG_ATTR_WP                     (1 << REG_WP_BIT)

//写保护状态寄存器size: 0:8bit; 1:16bit
#define REG_ATTR_SR_MASK                (0x1 << REG_SR_BIT)
#define REG_ATTR_SR_8bit                (0 << REG_SR_BIT)
#define REG_ATTR_SR_16bit               (1 << REG_SR_BIT)

#define REG_ATTR_WSR_MASK               (1 << REG_WSR_TWICE_BIT)
#define REG_ATTR_WSR_TWICE              (1 << REG_WSR_TWICE_BIT)

//support sector protect; Upon device power-up or after a device reset,
//each Sector Protection Register will default to the logical “1” state indicating that all
//sectors are protected and cannot be programmed or erased.
#define REG_ATTR_SECTOR_PROTECT_MASK    (0x03 << REG_SECTOR_PROTECT_BIT)
//uniforn protect sector of 64KB each
#define REG_ATTR_SECTOR_PROTECT         (1 << REG_SECTOR_PROTECT_BIT)
//uniforn protect sector of 128KB each
#define REG_ATTR_SECTOR_PROTECT_1       (2 << REG_SECTOR_PROTECT_BIT)
//uniforn protect sector of 64KB each except top block
#define REG_ATTR_SECTOR_PROTECT_2       (3 << REG_SECTOR_PROTECT_BIT)

//status register non-volatile property: 0:non-volatile; 1:volatile
#define REG_ATTR_VOLATILE_MASK          (0x1 << REG_VOLATILE_BIT)
#define REG_ATTR_NON_VOLATILE           (0 << REG_VOLATILE_BIT)
#define REG_ATTR_VOLATILE               (1 << REG_VOLATILE_BIT)

//write protect
#define NOR_WP_16M                      (NOR_CAP_16M)
#define NOR_WP_14M                      (14*1024*1024/512)
#define NOR_WP_8M                       (NOR_CAP_8M)
#define NOR_WP_7M                       (NOR_WP_14M / 2)
#define NOR_WP_6M                       (6*1024*1024/512)
#define NOR_WP_5M                       (5*1024*1024/512)
#define NOR_WP_4M                       (NOR_CAP_4M)
#define NOR_WP_3M                       (3*1024*1024/512)
#define NOR_WP_2M                       (NOR_CAP_2M)
#define NOR_WP_1M                       (NOR_CAP_1M)
#define NOR_WP_512K                     (NOR_CAP_512K)
#define NOR_WP_64K                      (64*1024/512)
#define NOR_WP_32K                      (32*1024/512)
#define NOR_WP_127K                     (127*1024/512)
#define NOR_WP_256K                     (256*1024/512)

//保护除最后两个block的区域
#define NOR_WP_16M_256                  (NOR_WP_16M - 512)
#define NOR_WP_16M_128                  (NOR_WP_16M - 256)
#define NOR_WP_8M_128                   (NOR_WP_8M - 256)
#define NOR_WP_4M_128                   (NOR_WP_4M - 256)
#define NOR_WP_2M_128                   (NOR_WP_2M - 256)
#define NOR_WP_1M_128                   (NOR_WP_1M - 256)

//保护除最后一个block的区域
#define NOR_WP_16M_64                   (NOR_WP_16M - 128)
#define NOR_WP_16M_64                   (NOR_WP_16M - 128)
#define NOR_WP_8M_64                    (NOR_WP_8M - 128)
#define NOR_WP_4M_64                    (NOR_WP_4M - 128)
#define NOR_WP_2M_64                    (NOR_WP_2M - 128)
#define NOR_WP_1M_64                    (NOR_WP_1M - 128)
#define NOR_WP_512K_64                  (NOR_WP_512K - 128)
#define NOR_WP_512K_128                 (NOR_WP_512K - 256)

#define NOR_WP_2M_32                    (NOR_WP_2M - 64)
#define NOR_WP_1M_32                    (NOR_WP_1M - 64)
#define NOR_WP_512K_32                  (NOR_WP_512K - 64)


//通用flash status寄存器定义
#define NOR_SR_WIP_BIT                  (0)
#define NOR_SR_WEL_BIT                  (1)
#define NOR_SR_BP0_BIT                  (2)
#define NOR_SR_BP1_BIT                  (3)
#define NOR_SR_BP2_BIT                  (4)
#define NOR_SR_BP3_BIT                  (5)
#define NOR_SR_BP4_BIT                  (6)

#define NOR_SR_TB_BIT                   (5)
#define NOR_SR_SEC_BIT                  (6)
#define NOR_SR_SRP0_BIT                 (7)

#define NOR_SR_SRP1_BIT                 (8)
#define NOR_SR_QE_BIT                   (9)
#define NOR_SR_LB1_BIT                  (11)
#define NOR_SR_LB2_BIT                  (12)
#define NOR_SR_LB3_BIT                  (13)
#define NOR_SR_CMP_BIT                  (14)
#define NOR_SR_SUS_BIT                  (15)

#define NOR_SR_WIP_MASK                 (1 << NOR_SR_WIP_BIT)
#define NOR_SR_WEL_MASK                 (1 << NOR_SR_WEL_BIT)
#define NOR_SR_BP0_MASK                 (1 << NOR_SR_BP0_BIT)
#define NOR_SR_BP1_MASK                 (1 << NOR_SR_BP1_BIT)
#define NOR_SR_BP2_MASK                 (1 << NOR_SR_BP2_BIT)
#define NOR_SR_BP3_MASK                 (1 << NOR_SR_BP3_BIT)
#define NOR_SR_BP4_MASK                 (1 << NOR_SR_BP4_BIT)

#define NOR_SR_TB_MASK                  (1 << NOR_SR_TB_BIT)
#define NOR_SR_SEC_MASK                 (1 << NOR_SR_SEC_BIT)
#define NOR_SR_SRP0_MASK                (1 << NOR_SR_SRP0_BIT)

#define NOR_SR_SRP1_MASK                (1 << NOR_SR_SRP1_BIT)
#define NOR_SR_QE_MASK                  (1 << NOR_SR_QE_BIT)
#define NOR_SR_LB1_MASK                 (1 << NOR_SR_LB1_BIT)
#define NOR_SR_LB2_MASK                 (1 << NOR_SR_LB2_BIT)
#define NOR_SR_LB3_MASK                 (1 << NOR_SR_LB3_BIT)
#define NOR_SR_CMP_MASK                 (1 << NOR_SR_CMP_BIT)
#define NOR_SR_SUS_MASK                 (1 << NOR_SR_SUS_BIT)

//MACRONIX flash写保护(status + configure)
#define MX_CR_TB_BIT                    (11)
#define MX_CR_TB_MASK                   (1 << MX_CR_TB_BIT)

//MICRON flash写保护
#define MR_SR_BP3_BIT                   (6)
#define MR_SR_BP3_MASK                  (1 << MR_SR_BP3_BIT)





#ifndef _ASSEMBLER_
/*Vendor ID */
typedef enum
{
    /*! Alliance Semiconductor */
    ALLIANCE_ID = 0x52,
    /*! AMD */
    AMD_ID = 0x01,
    /*! AMIC */
    AMIC_ID = 0x37,
    /*! ATMEL */
    ATMEL_ID = 0x1f,
    /*! Catalyst */
    CATALYST_ID = 0x31,
    /*! ESMT */
    ESMT_ID  = 0x8c,
    /*! EON */
    EON_ID = 0x1c,
    /*! Fujitsu */
    FUJITSU_ID = 0x04,
    /*! GigaDevice */
    GIGADEVICE_ID = 0xc8,
    /*! Hyundai */
    HYUNDAI_ID = 0xad,
    /*! Intel */
    INTEL_ID = 0x89,
    /*! Macronix (MX) */
    MACRONIX_ID = 0xc2,
    /*! PMC */
    PMC_ID = 0x9d,
    /*! SANYO */
    SANYO_ID = 0x62,
    /*! SHARP */
    SHARP_ID = 0xb0,
    /*! Spansion */
    SPANSION_ID = 0x01,
    /*! SST */
    SST_ID = 0xbf,
    /*! Numonyx, Micron, ST */
    NUMONYX_ID = 0x20,
    /*! SyncMOS (SM) and Mosel Vitelic Corporation (MVC) */
    SYNCMOS_MVC_ID = 0x40,
    /*! Texas Instruments */
    TI_ID = 0x97,
    /*! Winbond (ex Nexcom) */
    WINBOND_NEX_ID = 0xef,
    /*! Winbond */
    WINBOND_ID = 0xda,
    /*! shanghai fudan microelectronics */
    FD_MICRO_ID = 0xA1,
    /*! ZhuHai Berg microelectronics (Bo Guan) */
    ZH_BERG_ID = 0xE0,
    /*! ZhuHai Boya Microelectronics */
    ZH_BOYA_ID = 0x68,
    /*! Fremont Micro Device(FMD) */
    FMD_ID = 0x0E,
    /*! GigaDevice2 */
    GIGADEVICE_ID1 = 0x50,
    GIGADEVICE_ID2 = 0x51,
    /*! 芯天下 */
    XINTIANXIA_ID = 0x5E,
    /*! Nantronics */
    NANTRONIC_ID = 0xd5,
    /*! FIDELIX*/
    FIDELIX_ID = 0xf8,
}nor_vendor_e;

#define ST_ID  (NUMONYX_ID) /* ST / SGS/Thomson / Numonyx (later acquired by Micron) */


typedef struct
{
    uint16 size;  /* Eraseblock size, sector uint */
    uint16 count; /* Eraseblock count */
}block_erase_t;

#define MAX_PROTECT_REGIONS  5

typedef struct
{
    uint16 wp_val;
    uint16 wp_size;
}block_protect_t;

//总共128字节,前面64byte为NOR硬件属性数据(硬件属数据长度不能超过64Byte)
typedef struct
{
    uint8 chipid[4];        //4bytes
    uint32 capacity;        //4bytes
    block_erase_t block_erase[MAX_ERASEREGIONS]; //20bytes
    uint8 dma_write;        //1byte

    uint8 write_cmd;
    uint8 erase_cmd;
    uint8 erase_mode;       //0: normal erase mode(every 64KB); 1: bottom boot; 2:top boot; 3: 32KB erase mode;
    uint8 uniform_blk_info; //support uniform block size:
                            //0: support only all 64KB block; 1: support only all 32KB block; 2: support only all 4KB block;
                            //3: support all 64KB and 32KB block; 4: support all 64KB and 4KB block;
                            //5: support 3 kind of uniform block: all 64KB, all 32KB, all 4KB;
    uint8 feature_mask;
    uint16 wp_clr_mask;
    block_protect_t block_protect[MAX_PROTECT_REGIONS]; //20bytes

    //2线读模式; 1: Dual output fast read(CMD:0x3B, 地址以1线模式发出,数据以2线模式读回);
    //2: Dual Input Output fast read(CMD:0xBB, 地址和数据均以2线模式操作); 3:两种模式均支持
    uint8 dual_read_mode;

    //支持的最大读频率,Mhz单位
    uint8 fast_read_clk;

    uint8 reserved_1[54];
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
    uint8 name[16];        //16bytes
#endif
}nor_chipinfo_t;

#endif  /*  _ASSEMBLER_ */
#endif  /*  _SPI_NOR_FLASHID_H */



