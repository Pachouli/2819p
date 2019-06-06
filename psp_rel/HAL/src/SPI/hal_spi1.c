#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define SPI_RW_TEST_EN          (0)
#define SPI_DMA_TRNAS_EN        (1)

static const struct spi_info sys_spi_info =
{
    .base = SPI1_BASE,
    .bus_width = 1,
    .delay_chain = 8,
#if (SPI_DMA_TRNAS_EN == 1)

#if (SPI1_NOR_USER == SPI1_NOR_PLAY)
    .dma_base = DMA3_BASE,
#else
    .dma_base = DMA4_BASE,
#endif

#endif
};

static void spi_delay(void)
{
    volatile int i = SPI_DELAY_LOOPS;

    while (i--)
    {
        ;
    }
}

static unsigned int spi_read(struct spi_info *si, unsigned int reg)
{
    return act_readl(si->base + reg);
}

static void spi_write(struct spi_info *si, unsigned int reg, unsigned int value)
{
    act_writel(value, si->base + reg);
}

static void spi_set_bits(struct spi_info *si, unsigned int reg, unsigned int mask, unsigned int value)
{
    spi_write(si, reg, (spi_read(si, reg) & ~mask) | value);
}

static void spi_reset(struct spi_info *si)
{
    act_writel(act_readl(MRCR) & ~(1 << MRCR_SPI1_RESET), MRCR);
    spi_delay();
    act_writel(act_readl(MRCR) | (1 << MRCR_SPI1_RESET), MRCR);
}

static void spi_set_cs(struct spi_info *si, int value)
{
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_SS, value ? SSPI_CTL_SS : 0);
}


static void spi_wait_tx_complete(struct spi_info *si)
{
    while (!(spi_read(si, SSPI_STATUS) & SSPI_STATUS_TX_EMPTY))
    {
        ;
    }
    /* wait until tx fifo is empty */
    while ((spi_read(si, SSPI_STATUS) & SSPI_STATUS_BUSY))
    {
        ;
    }
}

static void spi_set_clk(unsigned int clk_khz)
{
    int div;
    if (clk_khz < (CONFIG_HOSC_CLK_MHZ * 1000 / 30))
    {
        div = 29;
    }
    else if (clk_khz >= CONFIG_HOSC_CLK_MHZ * 1000)
    {
        div = 0;
    }
    else
    {
        div = (CONFIG_HOSC_CLK_MHZ * 1000) / clk_khz - 1;
    }

    /* SPI1 module clock select */
    act_writel(((act_readl(CMU_SPICLK) & (~0x1f00)) | (1 << 14) | (div << 8)), CMU_SPICLK);
}

/*
 dir:
 0:read data
 1:write data
 */
static void spi1_dma_trans_config(struct spi_info *si, uint8 dir, int len)
{
    if (dir == 0)
    {
        spi_write(si, SPI_BC, len);
        /* switch to dma read mode */
        spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_RX_DRQ_EN | SSPI_CTL_WR_MODE_MASK,
                SSPI_CTL_CLK_SEL_DMA | SSPI_CTL_RX_DRQ_EN | SSPI_CTL_WR_MODE_READ);
    }
    else if (dir == 1)
    {
        /* switch to dma write mode */
        spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_TX_DRQ_EN | SSPI_CTL_WR_MODE_MASK,
                SSPI_CTL_CLK_SEL_DMA | SSPI_CTL_TX_DRQ_EN | SSPI_CTL_WR_MODE_WRITE);
    }
}

/*
 dir:
 0:read
 1:write
 */

bool hal_spi1_trans_voer(struct spi_info *si, int dir)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }

    spi_wait_tx_complete(si);
    if (dir == 0)
    {
        spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_RX_DRQ_EN | SSPI_CTL_WR_MODE_MASK,
                SSPI_CTL_CLK_SEL_CPU | SSPI_CTL_WR_MODE_DISABLE);
    }
    else if (dir == 1)
    {
        spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_TX_DRQ_EN | SSPI_CTL_WR_MODE_MASK,
                SSPI_CTL_CLK_SEL_CPU | SSPI_CTL_WR_MODE_DISABLE);
    }

    return 0;
}

static void spi1_read_data_by_ahb(struct spi_info *si, unsigned char *buf, int len)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }

    spi_write(si, SSPI_BC, len);

    /* switch to read mode */
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_WR_MODE_MASK, SSPI_CTL_WR_MODE_READ);
    //spi_set_cs(si, 0);
    /* read data */
    while (len--)
    {
        while (spi_read(si, SSPI_STATUS) & SSPI_STATUS_RX_EMPTY)
        {
            ;
        }
        *buf++ = spi_read(si, SSPI_RXDAT);
    }

    /* disable read mode */
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_WR_MODE_MASK, SSPI_CTL_WR_MODE_DISABLE);
    //spi_set_cs(si, 1);
}

static void spi1_read_data_by_dma(struct spi_info *si, const unsigned char *buf, int len)
{
    spi_write(si, SPI_BC, len);

    /* switch to dma read mode */
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_RX_DRQ_EN | SSPI_CTL_WR_MODE_MASK, SSPI_CTL_CLK_SEL_DMA
            | SSPI_CTL_RX_DRQ_EN | SSPI_CTL_WR_MODE_READ);

    /* move spi1 rx fifo to memory */
    act_writel(0x90, si->dma_base + DMA_CTL);
    act_writel(si->base + SSPI_RXDAT, si->dma_base + DMA_SADDR);
    act_writel((unsigned int) buf, si->dma_base + DMA_DADDR);
    act_writel(len, si->dma_base + DMA_LEN);

    /* start dma */
    act_writel(act_readl(si->dma_base + DMA_CTL) | DMA_START_EN, si->dma_base + DMA_CTL);

    while (act_readl(si->dma_base + DMA_CTL) & DMA_START_EN)
    {
        /* wait */
    }

    spi_delay();
    spi_wait_tx_complete(si);

    spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_RX_DRQ_EN | SSPI_CTL_WR_MODE_MASK, SSPI_CTL_CLK_SEL_CPU | SSPI_CTL_WR_MODE_DISABLE);
}

/*
 mode:
 0: AHB transfer
 1: sync DMA transfer
 2: async DMA transfer
*/
void hal_spi1_read_data(struct spi_info *si, unsigned char *buf, int len, int mode)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }
    if(mode != 2)
    {
        if((len > 16) && (si->dma_base))
        {
            spi1_read_data_by_dma(si, (unsigned char *) buf, len);
        }
        else
        {
            spi1_read_data_by_ahb(si, (unsigned char *) buf, len);
        }
    }
    else
    {
        spi1_dma_trans_config(si,0, len);
    }
}


static void spi1_write_data_by_ahb(struct spi_info *si, const unsigned char *buf, int len)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }
    /* switch to write mode */
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_WR_MODE_MASK, SSPI_CTL_WR_MODE_WRITE);
    /* write data */
    while (len--)
    {
        while (spi_read(si, SSPI_STATUS) & SSPI_STATUS_TX_FULL)
        {
            ;
        }
        spi_write(si, SSPI_TXDAT, *buf++);
    }

    spi_delay();
    spi_wait_tx_complete(si);

    /* disable write mode */
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_WR_MODE_MASK, SSPI_CTL_WR_MODE_DISABLE);
}

static void spi1_write_data_by_dma(struct spi_info *si, const unsigned char *buf, int len)
{
    /* switch to dma write mode */
    spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_TX_DRQ_EN | SSPI_CTL_WR_MODE_MASK, SSPI_CTL_CLK_SEL_DMA
            | SSPI_CTL_TX_DRQ_EN | SSPI_CTL_WR_MODE_WRITE);

    /* move memory data to spi1 tx fifo */
    act_writel(0x900, si->dma_base + DMA_CTL);
    act_writel((unsigned int) buf, si->dma_base + DMA_SADDR);
    act_writel(si->base + SSPI_TXDAT, si->dma_base + DMA_DADDR);
    act_writel(len, si->dma_base + DMA_LEN);

    /* start dma */
    act_writel(act_readl(si->dma_base + DMA_CTL) | DMA_START_EN, si->dma_base + DMA_CTL);

    while (act_readl(si->dma_base + DMA_CTL) & DMA_START_EN)
    {
        /* wait */
    }

    spi_delay();
    spi_wait_tx_complete(si);

    spi_set_bits(si, SSPI_CTL, SSPI_CTL_CLK_SEL_MASK | SSPI_CTL_TX_DRQ_EN | SSPI_CTL_WR_MODE_MASK, SSPI_CTL_CLK_SEL_CPU | SSPI_CTL_WR_MODE_DISABLE);
}

/*
 mode:
 0: AHB transfer
 1: sync DMA transfer
 2: async DMA transfer
 */
void hal_spi1_write_data(struct spi_info *si, const unsigned char *buf, int len, int mode)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }
    if(mode != 2)
    {
        if((len > 16) && (si->dma_base))
        {
            spi1_write_data_by_dma(si, (unsigned char *)buf, len);
        }
        else
        {
            spi1_write_data_by_ahb(si, (unsigned char *)buf, len);
        }
    }
    else
    {
        spi1_dma_trans_config(si,1, len);
    }

}

void hal_spi1_set_cs(struct spi_info *si, int value)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }

    spi_set_cs(si, value);
}

const uint8 mybuf[] =
{
#if (SPI_RW_TEST_EN == 1)
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x18, 0x49, 0x4a, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
#endif
};

void spi_test(struct spi_info *si)
{
#if (SPI_RW_TEST_EN == 1)

    uint8 times = 10;
    while (times--)
    {
        hal_spi1_write_data(si, mybuf, 50);
    }
#endif
}

int hal_spi1_init(struct spi_info *si, unsigned int clk_khz)
{
    if (!si)
    {
        si = (struct spi_info *) &sys_spi_info;
    }

    /* SPI module clock select */
    spi_set_clk(clk_khz);

    /* enable SPI0 module clock */
    act_writel(act_readl(CMU_DEVCLKEN) | (1 << CMU_DEVCLKEN_SPI1CLKEN), CMU_DEVCLKEN);

    /* reset SPI module */
    spi_reset(si);
    /* select SPI mode 3, SPI_SS output high¡ê?crc enable */
    act_writel(0x3008003a, SPI1_CTL);

    /*spi test entry*/
    //spi_test(si);
    return 0;
}

