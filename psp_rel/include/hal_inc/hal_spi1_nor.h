#ifndef __HAL_SPI1_NOR_H__
#define __HAL_SPI1_NOR_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"



#define	SPIMEM_CMD_READ_CHIPID			0x9f	    /* JEDEC ID */

#define	SPIMEM_CMD_FAST_READ			0x0b	    /* fast read */
#define	SPIMEM_CMD_FAST_READ_X2			0x3b	    /* data x2 */
#define	SPIMEM_CMD_FAST_READ_X4			0x6b	    /* data x4 */
#define	SPIMEM_CMD_FAST_READ_X2IO		0xbb	    /* addr & data x2 */
#define	SPIMEM_CMD_FAST_READ_X4IO		0xeb	    /* addr & data x4 */

#define	SPIMEM_CMD_ENABLE_WRITE			0x06	    /* enable write */
#define	SPIMEM_CMD_DISABLE_WRITE		0x04	    /* disable write */

#define	SPIMEM_CMD_WRITE_PAGE			0x02	    /* write one page */
#define SPIMEM_CMD_ERASE_BLOCK			0xd8	    /* 64KB erase */
#define SPIMEM_CMD_CONTINUOUS_READ_RESET	0xff	/* exit quad continuous_read mode */

#define SPIMEM_TFLAG_MIO_DATA		0x01
#define SPIMEM_TFLAG_MIO_ADDR_DATA	0x02
#define SPIMEM_TFLAG_MIO_CMD_ADDR_DATA	0x04
#define SPIMEM_TFLAG_MIO_MASK		0x07
#define SPIMEM_TFLAG_WRITE_DATA		0x08
#define SPIMEM_TFLAG_ENABLE_RANDOMIZE	0x10

/* spinor parameters */
#define SPINOR_WRITE_PAGE_SIZE_BITS	8
#define SPINOR_ERASE_SECTOR_SIZE_BITS	12
#define SPINOR_ERASE_BLOCK_SIZE_BITS	16

#define SPINOR_WRITE_PAGE_SIZE		(1 << SPINOR_WRITE_PAGE_SIZE_BITS)
#define SPINOR_ERASE_SECTOR_SIZE	(1 << SPINOR_ERASE_SECTOR_SIZE_BITS)
#define SPINOR_ERASE_BLOCK_SIZE		(1 << SPINOR_ERASE_BLOCK_SIZE_BITS)

#define SPINOR_WRITE_PAGE_MASK		(SPINOR_WRITE_PAGE_SIZE - 1)
#define SPINOR_ERASE_SECTOR_MASK	(SPINOR_ERASE_SECTOR_SIZE - 1)
#define SPINOR_ERASE_BLOCK_MASK		(SPINOR_ERASE_BLOCK_SIZE - 1)

/* spinor commands */
#define	SPINOR_CMD_WRITE_PAGE		0x02	/* write one page */
#define	SPINOR_CMD_DISABLE_WRITE	0x04	/* disable write */
#define	SPINOR_CMD_READ_STATUS		0x05	/* read status1 */
#define	SPINOR_CMD_READ_STATUS2		0x35	/* read status2 */
#define	SPINOR_CMD_READ_STATUS3		0x15	/* read status3 */
#define	SPINOR_CMD_WRITE_STATUS		0x01	/* write status1 */
#define	SPINOR_CMD_WRITE_STATUS2	0x31	/* write status2 */
#define	SPINOR_CMD_WRITE_STATUS3	0x11	/* write status3 */
#define	SPINOR_CMD_ENABLE_WRITE		0x06	/* enable write */
#define	SPINOR_CMD_FAST_READ		0x0b	/* fast read */
#define SPINOR_CMD_ERASE_SECTOR		0x20	/* 4KB erase */
#define SPINOR_CMD_ERASE_BLOCK_32K	0x52	/* 32KB erase */
#define SPINOR_CMD_ERASE_BLOCK		0xd8	/* 64KB erase */
#define	SPINOR_CMD_READ_CHIPID		0x9f	/* JEDEC ID */
#define	SPINOR_CMD_DISABLE_QSPI		0xff	/* disable QSPI */

/* NOR Flash vendors ID */
#define SPINOR_MANU_ID_ALLIANCE		0x52	/* Alliance Semiconductor */
#define SPINOR_MANU_ID_AMD		    0x01	/* AMD */
#define SPINOR_MANU_ID_AMIC		    0x37	/* AMIC */
#define SPINOR_MANU_ID_ATMEL		0x1f	/* ATMEL */
#define SPINOR_MANU_ID_CATALYST		0x31	/* Catalyst */
#define SPINOR_MANU_ID_ESMT		    0x8c	/* ESMT */
#define SPINOR_MANU_ID_EON		    0x1c	/* EON */
#define SPINOR_MANU_ID_FD_MICRO		0xa1	/* shanghai fudan microelectronics */
#define SPINOR_MANU_ID_FIDELIX		0xf8	/* FIDELIX */
#define SPINOR_MANU_ID_FMD		    0x0e	/* Fremont Micro Device(FMD) */
#define SPINOR_MANU_ID_FUJITSU		0x04	/* Fujitsu */
#define SPINOR_MANU_ID_GIGADEVICE	0xc8	/* GigaDevice */
#define SPINOR_MANU_ID_GIGADEVICE2	0x51	/* GigaDevice2 */
#define SPINOR_MANU_ID_HYUNDAI		0xad	/* Hyundai */
#define SPINOR_MANU_ID_INTEL		0x89	/* Intel */
#define SPINOR_MANU_ID_MACRONIX		0xc2	/* Macronix (MX) */
#define SPINOR_MANU_ID_NANTRONIC	0xd5	/* Nantronics */
#define SPINOR_MANU_ID_NUMONYX		0x20	/* Numonyx, Micron, ST */
#define SPINOR_MANU_ID_PMC		    0x9d	/* PMC */
#define SPINOR_MANU_ID_SANYO		0x62	/* SANYO */
#define SPINOR_MANU_ID_SHARP		0xb0	/* SHARP */
#define SPINOR_MANU_ID_SPANSION		0x01	/* SPANSION */
#define SPINOR_MANU_ID_SST		    0xbf	/* SST */
#define SPINOR_MANU_ID_SYNCMOS_MVC	0x40	/* SyncMOS (SM) and Mosel Vitelic Corporation (MVC) */
#define SPINOR_MANU_ID_TI		    0x97	/* Texas Instruments */
#define SPINOR_MANU_ID_WINBOND		0xda	/* Winbond */
#define SPINOR_MANU_ID_WINBOND_NEX	0xef	/* Winbond (ex Nexcom) */
#define SPINOR_MANU_ID_ZH_BERG		0xe0	/* ZhuHai Berg microelectronics (Bo Guan) */


struct spidevice_info
{
    struct spi_info spi;
    unsigned int chipid;
    unsigned int clk_khz;
};

extern int hal_spi1nor_init(void);
extern unsigned int hal_spi1nor_read_chipid(void);
extern int hal_spi1nor_erase(unsigned int addr, int size);
extern int hal_spi1nor_read(unsigned int addr, void *data, int len);
extern int hal_spi1nor_write(unsigned int addr, const void *data, int len);


#endif

