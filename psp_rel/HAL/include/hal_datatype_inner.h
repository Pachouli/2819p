
#ifndef __HAL_DATATYPE_INNER_H__
#define __HAL_DATATYPE_INNER_H__

#include "Design_Specification.h"

typedef volatile unsigned int *REG32;
typedef volatile unsigned short *REG16;
typedef volatile unsigned char *REG8;

#define act_writeb(val, reg)        ((*((volatile unsigned char *)(reg))) = (val))
#define act_writew(val, reg)        ((*((volatile unsigned short *)(reg))) = (val))
#define act_writel(val, reg)        ((*((volatile unsigned int *)(reg))) = (val))

#define act_readb(port)             (*((volatile unsigned char *)(port)))
#define act_readw(port)             (*((volatile unsigned short *)(port)))
#define act_readl(port)             (*((volatile unsigned int *)(port)))

#define act_setb(val, reg)          (*(volatile uint8 *)(reg)   =  (*(volatile uint8 *)(reg)) | (val))
#define act_setl(val, reg)          (*(volatile uint32 *)(reg)  =  (*(volatile uint32 *)(reg)) | (val))
#define act_clearb(val, reg)        (*(volatile uint8 *)(reg)   =  (*(volatile uint8 *)(reg)) & (~(val)))
#define act_clearl(val, reg)        (*(volatile uint32 *)(reg)  =  (*(volatile uint32 *)(reg)) & (~(val)))

#define CLEAR_WATCH_DOG             (act_writel((0x01 | act_readl(WD_CTL)), WD_CTL))

#endif
