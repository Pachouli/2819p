#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define  SPI1_NOR_TEST_EN    (0)

int spimem_transfer(struct spi_info *si, unsigned char cmd, unsigned int addr, int addr_len, void *buf, int length,
        unsigned char dummy_len, unsigned int flag);
int spimem_write_cmd_addr(struct spi_info *si, unsigned char cmd, unsigned int addr, int addr_len);
int spimem_write_cmd(struct spi_info *si, unsigned char cmd);
void spimem_read_chipid(struct spi_info *si, void *chipid, int len);
unsigned char spimem_read_status(struct spi_info *si, unsigned char cmd);
void spimem_set_write_protect(struct spi_info *si, int protect);
void spimem_read_page(struct spi_info *si, unsigned int addr, int addr_len, void *buf, int len);

static const struct spidevice_info sys_spi1_nor =
{
    .spi =
    {
        .base = SPI1_BASE,
        .bus_width = 1,
        .delay_chain = 8,
    },
    .chipid = 0,
    .clk_khz = 5000,
};

static void spi_delay(void)
{
    volatile int i = SPI_DELAY_LOOPS;

    while (i--)
    {
        ;
    }
}

static unsigned int spinor_read_status(struct spidevice_info *sni, unsigned char cmd)
{
    if (!sni)
    {
        sni = (struct spidevice_info *) &sys_spi1_nor;
    }

    return spimem_read_status(&sni->spi, cmd);
}

static int spinor_wait_ready(struct spidevice_info *sni)
{
    unsigned char status;

    while (1)
    {
        status = spinor_read_status(sni, SPINOR_CMD_READ_STATUS);
        if (!(status & 0x1))
        {
            break;
        }
    }

    return 0;
}

static void spinor_write_data(struct spidevice_info *sni, unsigned char cmd, unsigned int addr, int addr_len,
        const unsigned char *buf, int len)
{
    struct spi_info *si = &sni->spi;

    spimem_set_write_protect(si, 0);
    spimem_transfer(si, cmd, addr, addr_len, (unsigned char *) buf, len, 0, SPIMEM_TFLAG_WRITE_DATA);
    spinor_wait_ready(sni);

}

static void spinor_write_status(struct spidevice_info *sni, unsigned char cmd, unsigned char *status, int len)
{
    if (!sni)
    {
        sni = (struct spidevice_info *) &sys_spi1_nor;
    }

    spinor_write_data(sni, cmd, 0, 0, status, len);
}

static int spinor_erase_internal(struct spidevice_info *sni, unsigned char cmd, unsigned int addr)
{
    spinor_write_data(sni, cmd, addr, 3, 0, 0);
    return 0;
}

static int spinor_write_internal(struct spidevice_info *sni, unsigned int addr, const unsigned char *buf, int len)
{
    spinor_write_data(sni, SPINOR_CMD_WRITE_PAGE, addr, 3, buf, len);
    return 0;
}

static int spinor_read_internal(struct spidevice_info *sni, unsigned int addr, unsigned char *buf, int len)
{
    spimem_read_page(&sni->spi, addr, 3, buf, len);
    return 0;
}

static void spinor_reset_status(struct spidevice_info *sni)
{
    unsigned char status;

    status = spinor_read_status(sni, SPINOR_CMD_READ_STATUS);
    if (0 != (status & 0x3c))
    {
        status = 0;
        spinor_write_status(sni, SPINOR_CMD_WRITE_STATUS, &status, 1);
    }
}

static void sys_memcpy_swap(void *dst, const void *src, int length)
{
    src += length - 1;

    for (; length > 0; length--)
    {
        *((unsigned char *) dst++) = *((unsigned char *) src--);
    }
}

static void _spimem_read_data(struct spi_info *si, void *buf, int len)
{
    hal_spi1_read_data(si, (unsigned char *) buf, len,1);
}

static void _spimem_write_data(struct spi_info *si, const void *buf, int len)
{
    hal_spi1_write_data(si, (const unsigned char *) buf, len,1);
}

static void _spimem_write_byte(struct spi_info *si, unsigned char byte)
{
    _spimem_write_data(si, &byte, 1);
}

void spimem_set_cs(struct spi_info *si, int value)
{
    hal_spi1_set_cs(si,value);
}

void spimem_continuous_read_reset(struct spi_info *si)
{
    spimem_set_cs(si, 0);
    _spimem_write_byte(si, SPIMEM_CMD_CONTINUOUS_READ_RESET);
    _spimem_write_byte(si, SPIMEM_CMD_CONTINUOUS_READ_RESET);
    spimem_set_cs(si, 1);
}

unsigned int spimem_prepare_op(struct spi_info *si)
{
    unsigned int orig_spi_ctl;

#if 0
    /* backup old SPI_CTL */
    orig_spi_ctl = spi_read(si, SSPI_CTL);
#endif
    return orig_spi_ctl;
}

void spimem_complete_op(struct spi_info *si, unsigned int orig_spi_ctl)
{
    /* restore old SPI_CTL */
#if 0
    spi_write(si, SSPI_CTL, orig_spi_ctl);
#endif
    spi_delay();
}

int spimem_transfer(struct spi_info *si, unsigned char cmd, unsigned int addr, int addr_len, void *buf, int length,
        unsigned char dummy_len, unsigned int flag)
{

    unsigned int orig_spi_ctl, i, addr_be;

    /* address to big endian */
    if (addr_len > 0)
    {
        sys_memcpy_swap(&addr_be, &addr, addr_len);
    }

    orig_spi_ctl = spimem_prepare_op(si);

    spimem_set_cs(si, 0);

    /* write command */
    _spimem_write_byte(si, cmd);

    if (addr_len > 0)
    {
        _spimem_write_data(si, &addr_be, addr_len);
    }

    /* send dummy bytes */
    for (i = 0; i < dummy_len; i++)
    {
        _spimem_write_byte(si, 0);
    }

    /* send or get data */
    if (length > 0)
    {
        if (flag & SPIMEM_TFLAG_WRITE_DATA)
        {
            _spimem_write_data(si, buf, length);
        }
        else
        {
            _spimem_read_data(si, buf, length);
        }
    }

    spimem_set_cs(si, 1);

    spimem_complete_op(si, orig_spi_ctl);

    return 0;
}

int spimem_write_cmd_addr(struct spi_info *si, unsigned char cmd, unsigned int addr, int addr_len)
{
    return spimem_transfer(si, cmd, addr, addr_len, 0, 0, 0, 0);
}

int spimem_write_cmd(struct spi_info *si, unsigned char cmd)
{
    return spimem_write_cmd_addr(si, cmd, 0, 0);
}

void spimem_read_chipid(struct spi_info *si, void *chipid, int len)
{
    spimem_transfer(si, SPIMEM_CMD_READ_CHIPID, 0, 0, chipid, len, 0, 0);
}

unsigned char spimem_read_status(struct spi_info *si, unsigned char cmd)
{
    unsigned int status;

    spimem_transfer(si, cmd, 0, 0, &status, 1, 0, 0);

    return status;
}

void spimem_set_write_protect(struct spi_info *si, int protect)
{
    if (protect)
    {
        spimem_write_cmd(si, SPIMEM_CMD_DISABLE_WRITE);
    }
    else
    {
        spimem_write_cmd(si, SPIMEM_CMD_ENABLE_WRITE);
    }
}

void spimem_read_page(struct spi_info *si, unsigned int addr, int addr_len, void *buf, int len)
{
    unsigned char cmd;
    unsigned int flag;

    if (si->bus_width == 4)
    {
        cmd = SPIMEM_CMD_FAST_READ_X4;
        flag = SPIMEM_TFLAG_MIO_DATA;
    }
    else if (si->bus_width == 2)
    {
        cmd = SPIMEM_CMD_FAST_READ_X2;
        flag = SPIMEM_TFLAG_MIO_DATA;
    }
    else
    {
        cmd = SPIMEM_CMD_FAST_READ;
        flag = 0;
    }

    spimem_transfer(si, cmd, addr, addr_len, buf, len, 1, flag);
}

int spimem_erase_block(struct spi_info *si, unsigned int addr)
{
    spimem_set_write_protect(si, 0);
    spimem_write_cmd_addr(si, SPIMEM_CMD_ERASE_BLOCK, addr, 3);

    return 0;
}

int hal_spi1nor_read(unsigned int addr, void *data, int len)
{
    struct spidevice_info *sni = NULL;

    if (!len)
    {
        return 0;
    }

    if (!sni)
    {
        sni = (struct spidevice_info *) &sys_spi1_nor;
    }

    spinor_read_internal(sni, addr, data, len);

    return 0;
}

int hal_spi1nor_write(unsigned int addr, const void *data, int len)
{
    int unaligned_len, remaining, write_size;
    struct spidevice_info *sni = NULL;

    if (!len)
    {
        return 0;
    }

    if (!sni)
    {
        sni = (struct spidevice_info *) &sys_spi1_nor;
    }

    /* unaligned write? */
    if (addr & SPINOR_WRITE_PAGE_MASK)
    {
        unaligned_len = SPINOR_WRITE_PAGE_SIZE - (addr & SPINOR_WRITE_PAGE_MASK);
    }
    else
    {
        unaligned_len = 0;
    }

    remaining = len;
    while (remaining > 0)
    {
        if (unaligned_len)
        {
            /* write unaligned page data */
            if (unaligned_len > len)
            {
                write_size = len;
            }
            else
            {
                write_size = unaligned_len;
            }
            unaligned_len = 0;
        }
        else if (remaining < SPINOR_WRITE_PAGE_SIZE)
        {
            write_size = remaining;
        }
        else
        {
            write_size = SPINOR_WRITE_PAGE_SIZE;
        }

        spinor_write_internal(sni, addr, data, write_size);

        addr += write_size;
        data += write_size;
        remaining -= write_size;
    }

    return 0;
}

int hal_spi1nor_erase(unsigned int addr, int size)
{
    int remaining, erase_size;
    unsigned char cmd;
    struct spidevice_info *sni = NULL;

    if (!size)
    {
        return 0;
    }

    if (addr & SPINOR_ERASE_SECTOR_MASK || size & SPINOR_ERASE_SECTOR_MASK)
    {
        return -1;
    }

    if (!sni)
    {
        sni = (struct spidevice_info *) &sys_spi1_nor;
    }

    /* write aligned page data */
    remaining = size;
    while (remaining > 0)
    {
        if (addr & SPINOR_ERASE_BLOCK_MASK || remaining < SPINOR_ERASE_BLOCK_SIZE)
        {
            cmd = SPINOR_CMD_ERASE_SECTOR;
            erase_size = SPINOR_ERASE_SECTOR_SIZE;
        }
        else
        {
            cmd = SPINOR_CMD_ERASE_BLOCK;
            erase_size = SPINOR_ERASE_BLOCK_SIZE;
        }

        spinor_erase_internal(sni, cmd, addr);

        addr += erase_size;
        remaining -= erase_size;
    }

    return 0;
}

unsigned int hal_spi1nor_read_chipid(void)
{
    unsigned int chipid;

    struct spidevice_info *sni = NULL;

    sni = (struct spidevice_info *) &sys_spi1_nor;

    spimem_read_chipid(&sni->spi, &chipid, 4);

    return chipid;
}


void spi_nor_rw_test(void)
{
#if (SPI1_NOR_TEST_EN == 1)

    unsigned int addr = 0x0,size = 0x1000;

    g_p_sys_hal_cbks->hal_memset(0xbfc39200, 0x5a, size);

    hal_spi1nor_erase(0,size);

    hal_spi1nor_write(addr,0xbfc39200,size);

    hal_spi1nor_read(addr,0xbfc3b200,size);
#endif
}


int hal_spi1nor_init(void)
{
    unsigned char mid;
    unsigned int nor_id;
    int i;

    struct spidevice_info *sni = NULL;

    if (!sni)
    {
        sni = (struct spidevice_info *) &sys_spi1_nor;
    }

    hal_spi1_init(&sni->spi, sni->clk_khz);

    /* read nor chipid */
    for (i = 0; i < 4; i++)
    {
        nor_id = hal_spi1nor_read_chipid();
        mid = nor_id & 0xff;

        if ((mid != 0xff) && (mid != 0x00))
        {
            break;
        }
    }
    //g_p_sys_hal_cbks->hal_printf("read nor id:0x%x\n", nor_id);

    if (i == 4)
    {
        return -1;
    }

    /* spi1 nor read write test */
    spi_nor_rw_test();

    return 0;
}

