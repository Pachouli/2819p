/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM QN8027 发射模组驱动头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _FM_QN8027_H
#define _FM_QN8027_H

#include "psp_includes.h"
#include "fm_tx_interface.h"

#define  ID_QN8007		0xf0
#define  QN8007_ADDR    0x56

#define  ID_QN8027		0x40
#define  QN8027_ADDR    0x58

//fm tx module I2C address
#define I2C_READ_ADDR           (QN8027_ADDR|0x01)
#define I2C_WRITE_ADDR          (QN8027_ADDR)

//register defines
#define REG_SYSTEM     0x00
#define REG_CH1        0x01
#define REG_GPLT       0x02
#define REG_XTL        0x03
#define REG_VGA        0x04
#define REG_CID1       0x05
#define REG_CID2       0x06

#endif /*_FM_QN8027_H */

