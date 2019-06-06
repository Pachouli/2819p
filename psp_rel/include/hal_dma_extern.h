#ifndef __HAL_DMA_EXTERN_H__
#define __HAL_DMA_EXTERN_H__

#include <attr.h>
#include <typeext.h>

//public defines

typedef enum
{
    DMA_SRC_MEM = 0,
    DMA_SRC_BB_RAM = 1,
    //2 reserve
    DMA_SRC_UART0_RX = 3,
    DMA_SRC_MODEM_TX = 4, //Modem TX IQ Data dma to MEM, for Modem TX debug
    DMA_SRC_ADC = 5,
    DMA_SRC_SDMMC = 6,
    DMA_SRC_SD1 = 7,
    DMA_SRC_UART1_RX = 8,
    DMA_SRC_SPI1_RX = 9,
    DMA_SRC_I2S_RX = 10,
    DMA_SRC_SPDIF_RX = 11,
    //12 reserve
    DMA_SRC_SPI0_RX = 13,
} dma_src_type_e;

typedef enum
{
    DMA_DST_MEM = 0,
    DMA_DST_BB_RAM = 1,
    DMA_DST_MODEM_TX = 2, //MEM IQ Data dma to Modem TX, then bypass to RF DAC, for RF TX debug
    DMA_DST_UART0_TX = 3,
    //4 reserve
    DMA_DST_DAC_I2STX = 5,
    DMA_DST_SDMMC = 6,
    DMA_DST_SD1 = 7,
    DMA_DST_UART1_TX = 8,
    DMA_DST_SPI1_TX = 9,
    //10 reserve
    //11 reserve
    DMA_DST_LCD = 12,
    DMA_DST_SPI0_TX = 13,
} dma_dst_type_e;

typedef enum
{
    DMA_CHAN_DMA0 = 0,
    DMA_CHAN_DMA1 = 1,
    DMA_CHAN_DMA2 = 2,
    DMA_CHAN_DMA3 = 3,
    DMA_CHAN_DMA4 = 4,
} dma_chan_e;

#define FRAME_BIT_AUDIO_16BIT 0
#define FRAME_BIT_AUDIO_24BIT 1
#define FRAME_BIT_SPI1_8BIT   0
#define FRAME_BIT_SPI1_32BIT  1

#define DMA_IP_HF (1 << 0)
#define DMA_IP_TC (1 << 1)
typedef void (*dma_irq_cbk)(uint8 pending);

typedef struct
{
    uint8 * src_mem_addr;
    uint8 * dst_mem_addr;
    uint32 frame_len;
    dma_irq_cbk irq_cbk;
    dma_src_type_e src_type;
    dma_dst_type_e dst_type;
    bool hf_ie : 1;
    bool tc_ie : 1;
    bool reload : 1;
    bool start : 1;
    uint8 frame_bit : 1; //仅对AUDIO和SPI1有效
} dma_chan_setting_t;

#define WAIT_DMA_TC_ALWAYS 0 //必须等到TC才返回0
#define WAIT_DMA_TC_TRY    1 //马上返回，TC返回0，否则返回-1

#endif
