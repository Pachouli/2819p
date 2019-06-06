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
 * \file     spi_nor_id_tbl.c
 * \brief
 * \author   wuyufan
 * \version  1.0
 * \date     2012-12-6
 *******************************************************************************/
#include "spi_nor_flashid.h"

#pragma __PRQA_IGNORE_START__
const nor_chipinfo_t nor_chip_tbl[] =
{
    /*
     * .chipid      = chip id
     * .capacity    = nor capacity
     * .block_erase = erase region
     * .name        = flash name
     */

    /* ------------------------------ AMIC ------------------------------ */
    //A25L40PU
    {
        .chipid = {AMIC_ID, 0x20, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 7},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L40PU",
#endif
    },

    //A25L80P
    {
        .chipid = {AMIC_ID, 0x20, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 15},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L80P",
#endif
    },

    //A25L16PT
    {
        .chipid = {AMIC_ID, 0x20, 0x25, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 31},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_4K, 2},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 2,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L16PT",
#endif
    },

    //A25L16PU
    {
        .chipid = {AMIC_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 31},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L16PU",
#endif
    },

    //A25L040(NO datasheet)
    {
        .chipid = {AMIC_ID, 0x30, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L040",
#endif
    },

    //A25L080(NO datasheet)
    {
        .chipid = {AMIC_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L080",
#endif
    },

    //A25L016
    {
        .chipid = {AMIC_ID, 0x30, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L016",
#endif
    },

    //A25L032(NO datasheet)
    {
        .chipid = {AMIC_ID, 0x30, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25L032",
#endif
    },

    //A25LQ16
    {
        .chipid = {AMIC_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_2M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_2M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_2M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25LQ16",
#endif
    },

    //A25LQ32A
    {
        .chipid = {AMIC_ID, 0x40, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_4M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_4M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_4M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25LQ32A",
#endif
    },

    //A25LQ064
    {
        .chipid = {AMIC_ID, 0x40, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK, NOR_WP_8M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 84,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "A25LQ64",
#endif
    },


    /* ------------------------------ ATMEL ------------------------------ */
    //AT25DF041A
    {
        .chipid = {ATMEL_ID, 0x44, 0x01, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_2 | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 70,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF041A",
#endif
    },

    //AT25DF081
    {
        .chipid = {ATMEL_ID, 0x45, 0x02, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF081",
#endif
    },

    //AT25DF081A
    {
        .chipid = {ATMEL_ID, 0x45, 0x01, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF081A",
#endif
    },

    //AT25DF161
    {
        .chipid = {ATMEL_ID, 0x46, 0x02, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF161",
#endif
    },

    //AT25DF321
    {
        .chipid = {ATMEL_ID, 0x47, 0x00, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 70,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF321",
#endif
    },

    //AT25DF321A
    {
        .chipid = {ATMEL_ID, 0x47, 0x01, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF321A",
#endif
    },

    //AT25DF641(A)
    {
        .chipid = {ATMEL_ID, 0x48, 0x00, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_8M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DF641(A)",
#endif
    },

    //AT25FS040
    {
        .chipid = {ATMEL_ID, 0x66, 0x04, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK, NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25FS040",
#endif
    },

    //AT26F004
    {
        .chipid = {ATMEL_ID, 0x04, 0x00, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_2 | REG_ATTR_VOLATILE),
        .wp_clr_mask = 0,
        .block_protect =
        {
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 33,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT26F004",
#endif
    },


    //AT25DQ161
    {
        .chipid = {ATMEL_ID, 0x86, 0x00, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT25DQ161",
#endif
    },

    //AT26DF081A
    {
        .chipid = {ATMEL_ID, 0x45, 0x01, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_2 | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 70,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT26DF081A",
#endif
    },

    //AT26DF161
    {
        .chipid = {ATMEL_ID, 0x46, 0x00, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_1 | REG_ATTR_VOLATILE),
        .wp_clr_mask = (0),
        .block_protect =
        {
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT26DF161",
#endif
    },

    //AT26DF161A
    {
        .chipid = {ATMEL_ID, 0x46, 0x01, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 70,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "AT26DF161A",
#endif
    },

    /* ------------------------------ ESMT ------------------------------ */
    //F25L08PA
    {
        .chipid = {ESMT_ID, 0x20, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "F25L08PA",
#endif
    },

    //F25L16PA
    {
        .chipid = {ESMT_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "F25L16PA",
#endif
    },

    //F25L32PA
    {
        .chipid = {ESMT_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "F25L32PA",
#endif
    },

    /* ------------------------------ EON ------------------------------ */
    //EN25B40
    {
        .chipid = {EON_ID, 0x20, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 7},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_256K},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25B40",
#endif
    },

    //EN25B80
    {
        .chipid = {EON_ID, 0x20, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 15},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_512K},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_64K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25B80",
#endif
    },

    //EN25B16
    {
        .chipid = {EON_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 31},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_64K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25B16",
#endif
    },

    //EN25B32
    {
        .chipid = {EON_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 63},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_64K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25B32",
#endif
    },

    //EN25B64
    {
        .chipid = {EON_ID, 0x20, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_4K, 2},
            {ERASE_BLOCK_8K, 1},
            {ERASE_BLOCK_16K, 1},
            {ERASE_BLOCK_32K, 1},
            {ERASE_BLOCK_64K, 127},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_64K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25B64",
#endif
    },

    //EN25F40
    {
        .chipid = {EON_ID, 0x31, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25F40",
#endif
    },

    //EN25F80
    {
        .chipid = {EON_ID, 0x31, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25F80",
#endif
    },

    //EN25F16
    {
        .chipid = {EON_ID, 0x31, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25F16",
#endif
    },

    //EN25F32(实测,实测�?EN25P32 一�?
    {
        .chipid = {EON_ID, 0x31, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25F32",
#endif
    },

    //EN25F32(datasheet中写保护与实测不一�?
    {
        .chipid = {EON_ID, 0x31, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
                NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP0_MASK), NOR_WP_4M_64},                  //实测为只保护最�?个block
            {(NOR_SR_BP1_MASK), NOR_WP_4M_128},                 //实测为只保护最�?个block
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},   //实测为只保护后一�
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25F32",
#endif
    },

    //EN25F64(NO datasheet)
    {
        .chipid = {EON_ID, 0x31, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25F64",
#endif
    },

    //EN25Q40A(EN25Q40没有32KB block, block protect也不相同)
    {
        .chipid = {EON_ID, 0x30, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 1,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
                NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_512K_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_512K_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q40(A)",
#endif
    },

    //EN25Q80B
    {
        .chipid = {EON_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_32},
            {(NOR_SR_BP2_MASK), NOR_WP_1M_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_128},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q80B",
#endif
    },

    //EN25D80
    {
        .chipid = {EON_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25D80",
#endif
    },

    //EN25Q80A
    {
        .chipid = {EON_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP2_MASK), NOR_WP_1M_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_128},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q80A",
#endif
    },

    //EN25D16
    {
        .chipid = {EON_ID, 0x30, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25D16",
#endif
    },

    //EN25Q16(A)
    {
        .chipid = {EON_ID, 0x30, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP0_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q16(A)",
#endif
    },

    //EN25Q32A(B)
    {
        .chipid = {EON_ID, 0x30, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP0_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q32A(B)",
#endif
    },

    //EN25Q64
    {
        .chipid = {EON_ID, 0x30, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_BP0_MASK), NOR_WP_8M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_8M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_6M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q64",
#endif
    },

    //EN25Q128
    {
        .chipid = {EON_ID, 0x30, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_16M},
            {(NOR_SR_BP0_MASK), NOR_WP_16M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_16M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_14M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25Q128",
#endif
    },

    //EN25QH16(EN25QH16A�?2KB block)
    {
        .chipid = {EON_ID, 0x70, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25QH16(A)",
#endif
    },

    //EN25QH32
    {
        .chipid = {EON_ID, 0x70, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25QH32",
#endif
    },

    //EN25QH64
    {
        .chipid = {EON_ID, 0x70, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25QH64",
#endif
    },

    //EN25QH128A
    {
        .chipid = {EON_ID, 0x70, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_32K, 512},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_16M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25QH128",
#endif
    },

    //EN25T80
    {
        .chipid = {EON_ID, 0x51, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25T80",
#endif
    },

    //EN25T16A
    {
        .chipid = {EON_ID, 0x51, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP0_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25T16A",
#endif
    },

    //EN25S40
    {
        .chipid = {EON_ID, 0x38, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {(NOR_SR_BP0_MASK), NOR_WP_512K_64},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25S40",
#endif
    },

    //EN25S80
    {
        .chipid = {EON_ID, 0x38, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25S80",
#endif
    },

    //EN25S16
    {
        .chipid = {EON_ID, 0x38, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP0_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25S16",
#endif
    },

    //EN25S32
    {
        .chipid = {EON_ID, 0x38, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP0_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP1_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25S32",
#endif
    },

    //EN25S64
    {
        .chipid = {EON_ID, 0x38, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_BP0_MASK), NOR_WP_8M_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "EN25S64",
#endif
    },

    /* ------------------------------ FIDELIX ------------------------- */
    //FM25Q16
    {
        .chipid = {FIDELIX_ID, 0x32, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_TB_MASK | NOR_SR_SEC_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_2M},
            {NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "FM25Q16",
#endif
    },

    /* ------------------------------ GigaDevice ------------------------------ */
    //GD25F40
    {
        .chipid = {GIGADEVICE_ID, 0x20, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 150,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25F40",
#endif
    },

    //GD25F80
    {
        .chipid = {GIGADEVICE_ID, 0x20, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 150,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25F80",
#endif
    },

    //GD25D40
    {
        .chipid = {GIGADEVICE_ID, 0x30, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 150,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25D40",
#endif
    },

    //GD25D80
    {
        .chipid = {GIGADEVICE_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 150,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25D80",
#endif
    },

    //GD25D16 (NO datasheet)
    {
        .chipid = {GIGADEVICE_ID, 0x30, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25D16",
#endif
    },

    //GD25D32 (NO datasheet)
    {
        .chipid = {GIGADEVICE_ID, 0x30, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25D32",
#endif
    },

    //GD25D64 (NO datasheet)
    {
        .chipid = {GIGADEVICE_ID, 0x30, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_8M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25D64",
#endif
    },

    //GD25D128 (NO datasheet)
    {
        .chipid = {GIGADEVICE_ID, 0x30, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_16M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25D128",
#endif
    },

    //GD25Q40B/41B
    {
        .chipid = {GIGADEVICE_ID, 0x40, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_BP3_MASK
            | NOR_SR_BP4_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_512K},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_512K_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_512K_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_256K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25Q40/41B",
#endif
    },

    //GD25Q80(B/C)
    {
        .chipid = {GIGADEVICE_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_BP3_MASK
            | NOR_SR_BP4_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_1M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_1M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_1M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK, NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25Q80(B/C)",
#endif
    },

    //GD25Q16(B/C)
    {
        .chipid = {GIGADEVICE_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_BP3_MASK
            | NOR_SR_BP4_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_2M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_2M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_2M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25Q16(B/C)",
#endif
    },

    //GD25Q32(B/C)(GD25Q32C write status 需要两个命令配合完�?
    {
        .chipid = {GIGADEVICE_ID, 0x40, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_BP3_MASK
            | NOR_SR_BP4_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_4M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_4M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_4M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25Q32(B/C)",
#endif
    },

    //GD25Q64(B/C)(GD25Q64C write status 需要两个命令配合完�?
    {
        .chipid = {GIGADEVICE_ID, 0x40, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_BP3_MASK
            | NOR_SR_BP4_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_8M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_8M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK, NOR_WP_6M},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_4M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25Q64(B/C)",
#endif
    },

    //GD25Q128C
    {
        .chipid = {GIGADEVICE_ID, 0x40, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_32K, 512},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit | REG_ATTR_WSR_TWICE),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_BP3_MASK
            | NOR_SR_BP4_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_16M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_16M_256},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_8M},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25Q128C",
#endif
    },

   //GD25T80
    {
        .chipid = {GIGADEVICE_ID, 0x31, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK),
        .block_protect =
        {
            {NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP1_MASK, NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "GD25T80",
#endif
    },

    /* ------------------------------ Macronix ------------------------------ */
    //MX25L4005A
    {
        .chipid = {MACRONIX_ID, 0x20, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L4005A",
#endif
    },

    //MX25L8005
    {
        .chipid = {MACRONIX_ID, 0x20, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L8005",
#endif
    },

    //MX25L1605/MX25L1608D/MX25L1606
    {
        .chipid = {MACRONIX_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L1605/08/06",
#endif
    },

    //MX25L1605/MX25L1608D
    {
        .chipid = {MACRONIX_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L1605/1608D",
#endif
    },

    //MX25L1606
    {
        .chipid = {MACRONIX_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L1606",
#endif
    },

    //MX25L3205/MX25L3208D/MX25L3206E/MX25L3275E/MX25L3235E
    {
        .chipid = {MACRONIX_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect =
        {
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L320x/75/35",
#endif
    },

    //MX25L3205/MX25L3208D
    {
        .chipid = {MACRONIX_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L3205/3208D",
#endif
    },

    //MX25L3206E
    {
        .chipid = {MACRONIX_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L3206E",
#endif
    },

    //MX25L3275E/MX25L3235E
    {
        .chipid = {MACRONIX_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit | REG_ATTR_SECTOR_PROTECT_2),
        .wp_clr_mask = (MX_CR_TB_MASK | NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(MX_CR_TB_MASK | NOR_SR_BP3_MASK), NOR_WP_4M},
            {(MX_CR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(MX_CR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 86,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L3275E/3235",
#endif
    },

    //MX25L6445E
    {
        .chipid = {MACRONIX_ID, 0x20, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L6445E",
#endif
    },

    //MX25L6405/MX25L6408D
    {
        .chipid = {MACRONIX_ID, 0x20, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_8M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L6405/6408D",
#endif
    },

    //MX25L12805
    {
        .chipid = {MACRONIX_ID, 0x20, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_16M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L12805",
#endif
    },

    //MX25L12835E/MX25L12845E
    {
        .chipid = {MACRONIX_ID, 0x20, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_32K, 512},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_16M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 70,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L12835E/45E",
#endif
    },

    //MX25L1635D
    {
        .chipid = {MACRONIX_ID, 0x24, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L1635D",
#endif
    },

    //MX25L3235D_0.06
    {
        .chipid = {MACRONIX_ID, 0x24, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L3235D_0.06",
#endif
    },

    //MX25L3235D_1.1
    {
        .chipid = {MACRONIX_ID, 0x5E, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK), NOR_WP_4M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L3235D_1.1",
#endif
    },

    //MX25L1635E
    {
        .chipid = {MACRONIX_ID, 0x25, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25L1635E",
#endif
    },

    //MX25R1635F
    {
        .chipid = {MACRONIX_ID, 0x28, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (MX_CR_TB_MASK | NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(MX_CR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(MX_CR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 60,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25R1635F",
#endif
    },

    //MX25U1635E
    {
        .chipid = {MACRONIX_ID, 0x25, 0x35, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_2),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 84,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25U1635E",
#endif
    },

    //MX25U3235E
    {
        .chipid = {MACRONIX_ID, 0x25, 0x36, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_2),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_4M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 84,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25U3235E",
#endif
    },

    //MX25U6435E
    {
        .chipid = {MACRONIX_ID, 0x25, 0x37, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_SECTOR_PROTECT_2),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_8M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_8M_128},
            {(NOR_SR_BP3_MASK), NOR_WP_4M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_READ,
        .fast_read_clk = 84,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MX25U6435E",
#endif
    },

    /* ------------------------------ Numonyx ------------------------------ */
    //M25PE40(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x80, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PE40",
#endif
    },

    //M25PE80(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x80, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PE80",
#endif
    },

    //M25PE16(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x80, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PE16",
#endif
    },

    //M45PE40(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x40, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M45PE40",
#endif
    },

    //M45PE80(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M45PE80",
#endif
    },

    //M45PE16(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M45PE16",
#endif
    },

    /* ------------------------------ Numonyx, Micron, ST ------------------------------ */
    //N25Q016 (NO datasheet)(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0xBA, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25Q016",
#endif
    },

    //N25Q032A
    {
        .chipid = {NUMONYX_ID, 0xBA, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25Q032A",
#endif
    },

    //N25Q064A
    {
        .chipid = {NUMONYX_ID, 0xBA, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK | NOR_SR_BP3_MASK),
        .block_protect =
        {
            {(MR_SR_BP3_MASK | NOR_SR_TB_MASK), NOR_WP_8M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25Q064A",
#endif
    },

    //N25Q0128 (NO datasheet)(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0xBA, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25Q0128",
#endif
    },

    /* ------------------------------ Nantronics ------------------------------ */
    //N25S40 (NO datasheet)(to_be_update)
    {
        .chipid = {NANTRONIC_ID, 0x30, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25S40",
#endif
    },

    //N25S80
    {
        .chipid = {NANTRONIC_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25S80",
#endif
    },

    //N25S16 (NO datasheet)(to_be_update)
    {
        .chipid = {NANTRONIC_ID, 0x30, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25S16",
#endif
    },

    //N25S32
    {
        .chipid = {NANTRONIC_ID, 0x30, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "N25S32",
#endif
    },

    /* ------------------------------ PMC ------------------------------ */
    //PM25LV080B
    {
        .chipid = {PMC_ID, 0x7F, 0x13, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "PM25LV080B",
#endif
    },

    //PM25LV016B
    {
        .chipid = {PMC_ID, 0x7F, 0x14, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "PM25LV016B",
#endif
    },

    /* ------------------------------ ISSI ------------------------------ */
    //IS25LQ080
    {
        .chipid = {PMC_ID, 0x13, 0x44, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_1M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "IS25LQ080",
#endif
    },

    //IS25LQ016
    {
        .chipid = {PMC_ID, 0x14, 0x45, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {(NOR_SR_BP3_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "IS25LQ016",
#endif
    },

    /* ------------------------------ Sanyo ------------------------------ */
    //LF25FW203A (NO datasheet)(to_be_update)
    {
        .chipid = {SANYO_ID, 0x16, 0x00, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "LF25FW203A",
#endif
    },

    /* ------------------------------ Spansion ------------------------------ */
    //S25FL004A(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x02, 0x12, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL004A",
#endif
    },

    //S25FL008A(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x02, 0x13, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL008A",
#endif
    },

    //S25FL016A(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x02, 0x14, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL016A",
#endif
    },

    //S25FL032A(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x02, 0x15, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL032A",
#endif
    },

    //S25FL064A(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x02, 0x16, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL064A",
#endif
    },

    //S25FL204(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x40, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 85,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL204",
#endif
    },

    //S25FL208(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK|NOR_SR_BP1_MASK|NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_1M_64},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK|NOR_SR_BP0_MASK), NOR_WP_1M_128},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 76,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL208",
#endif
    },

    //S25FL216(to_be_update)
    {
        .chipid = {SPANSION_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 65,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "S25FL216",
#endif
    },

    /* ------------------------------ SST ------------------------------ */
    //SST25LF040A
    {
        .chipid = {SST_ID, 0x44, 0xff, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = FALSE,
        .write_cmd = SPI_NOR_SEQUENTIAL_PROGRAM_2,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_32K,
        .erase_mode = 3,
        .uniform_blk_info = ALL_BLOCK_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 33,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25LF040A",
#endif
    },

    //SST25LF080A(to_be_update)
    {
        .chipid = {SST_ID, 0x80, 0xff, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_32K, 32},
        },
        .dma_write = FALSE,
        .write_cmd = SPI_NOR_SEQUENTIAL_PROGRAM_2,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_32K,
        .erase_mode = 3,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 33,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25LF080A",
#endif
    },

    //SST25VF040
    {
        .chipid = {SST_ID, 0x25, 0x8D, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = FALSE,
        .write_cmd = SPI_NOR_AAI_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25VF040",
#endif
    },

    //SST25VF080B
    {
        .chipid = {SST_ID, 0x25, 0x8E, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = FALSE,
        .write_cmd = SPI_NOR_AAI_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25VF080B",
#endif
    },

    //SST25VF016
    {
        .chipid = {SST_ID, 0x25, 0x41, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = FALSE,
        .write_cmd = SPI_NOR_AAI_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25VF016",
#endif
    },

    //SST25VF032B
    {
        .chipid = {SST_ID, 0x25, 0x4A, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = FALSE,
        .write_cmd = SPI_NOR_AAI_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 66,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25VF032B",
#endif
    },

    //SST25VF064C
    {
        .chipid = {SST_ID, 0x25, 0x4B, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit | REG_ATTR_VOLATILE),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP3_MASK), NOR_WP_8M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "SST25VF064C",
#endif
    },

    /* ------------------------------ ST ------------------------------ */
    //M25P40(to_be_update)
    {
        .chipid = {NUMONYX_ID, 0x20, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25P40",
#endif
    },

    //M25P80(to_be_update)
    {
        .chipid = {ST_ID, 0x20, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25P80",
#endif
    },

    //M25P16(to_be_update)
    {
        .chipid = {ST_ID, 0x20, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25P16",
#endif
    },

    //M25P32(to_be_update)
    {
        .chipid = {ST_ID, 0x20, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25P32",
#endif
    },

    //M25P64(to_be_update)
    {
        .chipid = {ST_ID, 0x20, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25P64",
#endif
    },

    //M25P128(to_be_update)
    {
        .chipid = {ST_ID, 0x20, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_256K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25P128",
#endif
    },

    //M25PX80(to_be_update)
    {
        .chipid = {ST_ID, 0x71, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PX80",
#endif
    },

    //M25PX16(to_be_update)
    {
        .chipid = {ST_ID, 0x71, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PX16",
#endif
    },

    //M25PX32(to_be_update)
    {
        .chipid = {ST_ID, 0x71, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PX32",
#endif
    },

    //M25PX64(to_be_update)
    {
        .chipid = {ST_ID, 0x71, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "M25PX64",
#endif
    },

    /* ------------------------------ Winbond ------------------------------ */
    //W25Q40BL/W25Q40CL(W25Q40CL support clock up to 104M)
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_512K},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_512K_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_512K_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q40BL/Q40CL",
#endif
    },

    //W25Q80/W25Q80BV/W25Q80DV
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect =
        {
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q80(BV)(DV)",
#endif
    },

    //W25Q80
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q80",
#endif
    },

    //W25Q80BV/W25Q80DV
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_1M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_1M_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q80BV/Q80DV",
#endif
    },

    //W25Q16
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q16",
#endif
    },

    //W25Q32
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q32",
#endif
    },

    //W25Q64BV
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_32K, 256},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_SEC_MASK | NOR_SR_TB_MASK
            | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q64BV",
#endif
    },

    //W25Q128BV
    {
        .chipid = {WINBOND_NEX_ID, 0x40, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
            {ERASE_BLOCK_32K, 512},
            {ERASE_BLOCK_4K, 4096},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_16M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_16M_256},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_8M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_2M},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q128BV",
#endif
    },

    //W25Q40.W(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x50, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q40.W",
#endif
    },

    //W25Q80.W(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x50, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q80.W",
#endif
    },

    //W25Q16.W(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x50, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q16.W",
#endif
    },

    //W25Q32.W(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x50, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q32.W",
#endif
    },

    //W25Q64.W(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x50, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q64.W",
#endif
    },

    //W25Q128.W(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x50, 0x18, 0xff},
        .capacity = NOR_CAP_16M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 256},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25Q128.W",
#endif
    },

    //W25X40/A/L/AL(W25X40L最�?0M, W25X40AL最�?0M, W25X40最�?5M, W25X40A最�?00M)
    {
        .chipid = {WINBOND_NEX_ID, 0x30, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK), NOR_WP_512K},
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25X40/A/L/AL",
#endif
    },

    //W25X80/A/L/AL(W25X80L最�?0M, W25X80AL最�?0M, W25840最�?5M, W25X80A最�?00M)
    {
        .chipid = {WINBOND_NEX_ID, 0x30, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25X80/A/L/AL",
#endif
    },

    //W25X16/A
    {
        .chipid = {WINBOND_NEX_ID, 0x30, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25X16/A",
#endif
    },

    //W25X32
    {
        .chipid = {WINBOND_NEX_ID, 0x30, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_256K},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25X32",
#endif
    },

    //W25X64
    {
        .chipid = {WINBOND_NEX_ID, 0x30, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
            {ERASE_BLOCK_4K, 2048},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_8M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_4M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 75,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "W25X64",
#endif
    },

    //MK25Q32(NO datasheet)(to_be_update)
    {
        .chipid = {WINBOND_NEX_ID, 0x70, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MK25Q32",
#endif
    },


    /* -------------------------- shagnhai fudan micro --------------------------- */
    //FM25F04A
    {
        .chipid = {FD_MICRO_ID, 0x31, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {(NOR_SR_BP2_MASK), NOR_WP_512K_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_512K_128},
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_256K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 100,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "FM25F04A",
#endif
    },

    //FM25Q08
    {
        .chipid = {FD_MICRO_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_1M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_1M_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "FM25Q08",
#endif
    },

    //FM25Q16
    {
        .chipid = {FD_MICRO_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_2M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "FM25Q16",
#endif
    },

    //FM25Q32
    {
        .chipid = {FD_MICRO_ID, 0x40, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK
            | NOR_SR_TB_MASK | NOR_SR_SEC_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_4M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_4M_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_4M_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK), NOR_WP_2M},
            {(NOR_SR_TB_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 104,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "FM25Q32",
#endif
    },

    /* -------------------------- zhuhai Berg micro(Bo Guan) --------------------------- */
    //BG25Q40A
    {
        .chipid = {ZH_BERG_ID, 0x40, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_512K},
            {NOR_SR_CMP_MASK | NOR_SR_SEC_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_512K_32},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_512K_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK, NOR_WP_256K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "BG25Q40A",
#endif
    },

    //BQ25Q80A
    {
        .chipid = {ZH_BERG_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_1M},
            {NOR_SR_CMP_MASK | NOR_SR_SEC_MASK | NOR_SR_BP2_MASK, NOR_WP_1M_32},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_1M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_1M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK, NOR_WP_512K},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "BQ25Q80A",
#endif
    },

    //BQ25Q16A
    {
        .chipid = {ZH_BERG_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_TB_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_2M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_2M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_2M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK, NOR_WP_1M},
            {NOR_SR_TB_MASK | NOR_SR_BP2_MASK, NOR_WP_512K},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "BQ25Q16A",
#endif
    },

    //BG25Q32A
    {
        .chipid = {ZH_BERG_ID, 0x40, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
            {ERASE_BLOCK_32K, 128},
            {ERASE_BLOCK_4K, 1024},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {NOR_SR_CMP_MASK, NOR_WP_4M},
            {NOR_SR_CMP_MASK | NOR_SR_BP0_MASK, NOR_WP_4M_64},
            {NOR_SR_CMP_MASK | NOR_SR_BP1_MASK, NOR_WP_4M_128},
            {NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK, NOR_WP_2M},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "BG25Q32A",
#endif
    },

    /* -------------------------- zhuhai Boya Microelectronics --------------------------- */
    //BY25D80
    {
        .chipid = {ZH_BOYA_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_SRP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK, NOR_WP_1M},
            {NOR_SR_BP0_MASK | NOR_SR_BP1_MASK, NOR_WP_1M_32},
            {NOR_SR_BP2_MASK, NOR_WP_1M_64},
            {NOR_SR_BP0_MASK | NOR_SR_BP2_MASK, NOR_WP_1M_128},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "BY25D80",
#endif
    },

	//BY25D16
    {
        .chipid = {ZH_BOYA_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK | NOR_SR_SRP0_MASK),
        .block_protect =
        {
            {NOR_SR_BP0_MASK | NOR_SR_BP1_MASK | NOR_SR_BP2_MASK, NOR_WP_2M},
            {NOR_SR_BP2_MASK, NOR_WP_2M_64},
            {NOR_SR_BP0_MASK | NOR_SR_BP2_MASK, NOR_WP_2M_128},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 108,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "BY25D16",
#endif
    },

    /* -------------------------- Fremont Micro Device(FMD) --------------------------- */
    //FT25H16
    {
        .chipid = {FMD_ID, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP4_MASK | NOR_SR_BP3_MASK |
            NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK | NOR_SR_CMP_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK), NOR_WP_2M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_2M_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP3_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "FT25H16",
#endif
    },

    /* -------------------------- GD costdown norflash --------------------------- */
    //MD25D40
    {
        .chipid = {GIGADEVICE_ID2, 0x40, 0x13, 0xff},
        .capacity = NOR_CAP_512K,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 8},
            {ERASE_BLOCK_32K, 16},
            {ERASE_BLOCK_4K, 128},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_512K},
            {(NOR_SR_BP2_MASK), NOR_WP_512K_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_512K_128},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MD25D40",
#endif
    },

    //MD25D80
    {
        .chipid = {GIGADEVICE_ID2, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP2_MASK), NOR_WP_1M_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_128},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MD25D80",
#endif
    },

    //MD25D16(NO datasheet)(to_be_update)
    {
        .chipid = {GIGADEVICE_ID2, 0x40, 0x15, 0xff},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
            {ERASE_BLOCK_32K, 64},
            {ERASE_BLOCK_4K, 512},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK
            | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_2M},
            {(NOR_SR_BP2_MASK), NOR_WP_2M_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_2M_128},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 120,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MD25D16",
#endif
    },

    //MD25D32(NO datasheet)(to_be_update)
    {
        .chipid = {GIGADEVICE_ID2, 0x40, 0x16, 0xff},
        .capacity = NOR_CAP_4M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 64},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 60,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MD25D32",
#endif
    },

    //MD25D64(NO datasheet)(to_be_update)
    {
        .chipid = {GIGADEVICE_ID2, 0x40, 0x17, 0xff},
        .capacity = NOR_CAP_8M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 128},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 60,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "MD25D64",
#endif
    },

    //XD25D80
    {
        .chipid = {GIGADEVICE_ID1, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {(NOR_SR_BP2_MASK), NOR_WP_1M_64},
            {(NOR_SR_BP2_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_128},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "XD25D80",
#endif
    },

    /* ------------------------------ 芯天�?------------------------------ */
    //PN25F08B
    {
        .chipid = {XINTIANXIA_ID, 0x40, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_8bit),
        .wp_clr_mask = (NOR_SR_SRP0_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK), NOR_WP_1M},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
        },
        .dual_read_mode = DUAL_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "PN25F08B",
#endif
    },

    /* ------------------------------ PUYA ------------------------------ */
    //PY25Q80H
    {
        .chipid = {0x85, 0x60, 0x14, 0xff},
        .capacity = NOR_CAP_1M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 16},
            {ERASE_BLOCK_32K, 32},
            {ERASE_BLOCK_4K, 256},
            {0, 0},
            {0, 0},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB_32KB_4KB,
        .feature_mask = (REG_ATTR_WP | REG_ATTR_SR_16bit),
        .wp_clr_mask = (NOR_SR_CMP_MASK | NOR_SR_BP3_MASK | NOR_SR_BP2_MASK | NOR_SR_BP1_MASK | NOR_SR_BP0_MASK),
        .block_protect =
        {
            {(NOR_SR_CMP_MASK),                   NOR_WP_1M},
            {(NOR_SR_CMP_MASK | NOR_SR_BP0_MASK), NOR_WP_1M_64},
            {(NOR_SR_CMP_MASK | NOR_SR_BP1_MASK), NOR_WP_1M_128},
            {(NOR_SR_CMP_MASK | NOR_SR_BP2_MASK), NOR_WP_512K},
            {0, 0},
        },
        .dual_read_mode = DUAL_IO_OUTPUT_READ,
        .fast_read_clk = 80,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "PY25Q80H",
#endif
    },

    /* ------------------------------ default ------------------------------ */
    //Unknown chip
    {
        .chipid = {0x11, 0x11, 0x11, 0x11},
        .capacity = NOR_CAP_2M,
        .block_erase =
        {
            {ERASE_BLOCK_64K, 32},
        },
        .dma_write = TRUE,
        .write_cmd = SPI_NOR_PAGE_PROGRAM,
        .erase_cmd = SPI_NOR_BLOCK_ERASE_64K,
        .erase_mode = 0,
        .uniform_blk_info = ALL_BLOCK_64KB,
        .feature_mask = 0,
        .wp_clr_mask = 0,
        .block_protect = {0},
        .dual_read_mode = NO_DUAL_READ,
        .fast_read_clk = 50,
        .reserved_1 = {0},
#ifdef  SUPPORT_SHOW_NORFLASH_NAME
        .name = "Unknown chip",
#endif
    }
};
#pragma __PRQA_IGNORE_END__
