#ifndef __HAL_SPI1_H__
#define __HAL_SPI1_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"


#define CONFIG_HOSC_CLK_MHZ         (26)

#define SPI_DELAY_LOOPS             (5)

/* spinor controller */
#define SSPI_CTL                    (0x00)
#define SSPI_STATUS                 (0x04)
#define SSPI_TXDAT                  (0x08)
#define SSPI_RXDAT                  (0x0c)
#define SSPI_BC                     (0x10)

#define SSPI_CTL_CLK_SEL_MASK       (1 << 31)
#define SSPI_CTL_CLK_SEL_CPU        (0 << 31)
#define SSPI_CTL_CLK_SEL_DMA        (1 << 31)

#define SSPI_CTL_WIDTH_SEL_MASK     (1 << 30)
#define SSPI_CTL_WIDTH_8_BITS       (0 << 30)
#define SSPI_CTL_WIDTH_32_BITS      (1 << 30)

#define SSPI_CTL_MODE_MASK          (3<< 28)
#define SSPI_CTL_MODE_MODE0         (0 << 28)
#define SSPI_CTL_MODE_MODE1         (1 << 28)
#define SSPI_CTL_MODE_MODE2         (2 << 28)
#define SSPI_CTL_MODE_MODE3         (3 << 28)


#define SSPI_CTL_DELAYCHAIN_MASK    (0xf << 16)
#define SSPI_CTL_DELAYCHAIN_SHIFT   (16)

#define SSPI_CTL_TX_DRQ_EN          (1 << 11)
#define SSPI_CTL_RX_DRQ_EN          (1 << 10)
#define SSPI_CTL_TX_FIFO_EN         (1 << 5)
#define SSPI_CTL_RX_FIFO_EN         (1 << 4)
#define SSPI_CTL_SS                 (1 << 3)
#define SSPI_CTL_WR_MODE_MASK       (0x3 << 0)
#define SSPI_CTL_WR_MODE_DISABLE    (0x0 << 0)
#define SSPI_CTL_WR_MODE_READ       (0x1 << 0)
#define SSPI_CTL_WR_MODE_WRITE      (0x2 << 0)
#define SSPI_CTL_WR_MODE_READ_WRITE (0x3 << 0)

#define SSPI_STATUS_BUSY            (1 << 0)
#define SSPI_STATUS_TX_FULL         (1 << 5)
#define SSPI_STATUS_TX_EMPTY        (1 << 4)
#define SSPI_STATUS_RX_FULL         (1 << 7)
#define SSPI_STATUS_RX_EMPTY        (1 << 6)


/* dma base */
#define DMA0_BASE                   (DMA0CTL)    
#define DMA1_BASE                   (DMA0_BASE + 0x10)
#define DMA2_BASE                   (DMA1_BASE + 0x10)
#define DMA3_BASE                   (DMA2_BASE + 0x10)
#define DMA4_BASE                   (DMA3_BASE + 0x10)

/* dma controller */
#define DMA_CTL                     (0x00)
#define DMA_SADDR                   (0x04)
#define DMA_DADDR                   (0x08)
#define DMA_LEN                     (0x0c)

#define DMA_START_EN                (1 << 0)

struct spi_info
{
    unsigned int base;
    unsigned char bus_width;
    unsigned char delay_chain;
    unsigned int  dma_base;
    void (*set_cs)(struct spi_info *si, int value);
};

extern int hal_spi1_init(struct spi_info *si, unsigned int clk_khz);
extern void hal_spi1_read_data(struct spi_info *si, unsigned char *buf, int len, int mode);
extern void hal_spi1_write_data(struct spi_info *si, const unsigned char *buf, int len, int mode);
extern void hal_spi1_set_cs(struct spi_info *si, int value);
extern bool hal_spi1_trans_voer(struct spi_info *si, int dir);

#endif
