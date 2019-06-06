/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM���������ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _FM_TX_DRV_H
#define _FM_TX_DRV_H

#include "psp_includes.h"
#include "fm_tx_interface.h"

extern uint8 fm_tx_twi_master_idx;

/*FM ��������ӿں�������*/
extern void sFM_TX_Init(uint16 freq);

extern void sFM_TX_SetFreq(uint16 freq);

extern int32 fm_tx_drv_init(void);
extern int32 fm_tx_drv_exit(void);

#endif /*_FM_DRV_H */

