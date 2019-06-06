/********************************************************************************
*                              USDK(1100)
*                            Module: IRQ
*                 Copyright(c) 2001-2008 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*      wuyueqian     2011-9-10 15:00     1.0             build this file
********************************************************************************/
/*!
* \file     irq.h
* \brief    中断处理相关宏定义、数据结构和函数声明
* \author   wuyueqian
* \version 1.0
* \date   2011-9-10
*******************************************************************************/
#ifndef __ASM_ATJ213X_IRQ_H__
#define __ASM_ATJ213X_IRQ_H__



#define NR_IRQS                     (23)

#ifndef _ASSEMBLER_

#ifdef PC
    #define inline
#endif


/*
static inline void act_writeb(unsigned char val, unsigned int reg)
{
    *(volatile unsigned char *)(reg) = val;
}

static inline void act_writew(unsigned short val, unsigned int reg)
{
    *(volatile unsigned short *)(reg) = val;
}
static inline void act_writel(unsigned int val, unsigned int reg)
{
    *(volatile unsigned int *)(reg) = val;
}

static inline unsigned char act_readb(unsigned long port)
{
    return (*(volatile unsigned char *)port);
}

static inline unsigned short act_readw(unsigned long port)
{
    return (*(volatile unsigned short *)port);
}

static inline unsigned int act_readl(unsigned long port)
{
    return (*(volatile unsigned int *)port);
}
*/

#define act_writeb(val, reg)        ((*((volatile unsigned char *)(reg))) = (val))

#define act_writew(val, reg)        ((*((volatile unsigned short *)(reg))) = (val))

#define act_writel(val, reg)        ((*((volatile unsigned int *)(reg))) = (val))

#define act_readb(port)             (*((volatile unsigned char *)(port)))

#define act_readw(port)             (*((volatile unsigned short *)(port)))

#define act_readl(port)             (*((volatile unsigned int *)(port)))



/*!
 *  中断源 \n
 *  \li 0~31 : 共32个中断源
 *
 */
enum
{
    IRQ_BT_BASEBAND = 0,
    IRQ_PMU,
    IRQ_WD,
    IRQ_TIMER1,
    IRQ_TIMER0,
    IRQ_RTC,
    IRQ_UART0,
    IRQ_GPIO,
    IRQ_I2S,
    IRQ_SPI0,
    IRQ_USB,
    IRQ_I2C0,
    IRQ_I2C1,
    IRQ_VAD,
    IRQ_DAC_IIS_TX,
    IRQ_ADC,
    IRQ_UART1,
    IRQ_SD,
    IRQ_DMA0,
    IRQ_DMA1,
    IRQ_DMA2,
    IRQ_DMA3,
    IRQ_DMA4,
    IRQ_RESERVE1,
    IRQ_IRC,
    IRQ_SPI1,
    IRQ_SDIO,
    IRQ_TIMER2,
    IRQ_TIMER3,
    IRQ_SPDIFRX,
    IRQ_MPU,
    IRQ_RESERVE2,
    IRQ_MAX,
};





typedef int irqreturn_t;

typedef void (*interrupt_handle_t)(void);

#endif /*_ASSEMBLER_*/
#endif /*__ASM_ATJ213X_IRQ_H__*/



