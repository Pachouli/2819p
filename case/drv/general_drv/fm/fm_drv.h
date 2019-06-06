/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _FM_DRV_H
#define _FM_DRV_H

#include "psp_includes.h"
#include "fm_interface.h"


extern uint8 twi_master_idx;
extern uint8 fm_clk_source_sel;
extern uint32 fm_clk_hz;

/*FM 驱动对外接口函数声明*/
extern int sFM_Init(FM_Drv_Init_Para_t *p_init_para);

extern int sFM_Standby(uint8 mode);

extern int sFM_SetFreq(uint32 freq);

extern int sFM_GetStatus(FM_Drv_Status_t *p_Drv_Status);

extern int sFM_Mute(FM_MUTE_e mode);

extern int sFM_SetThrod(uint32 level);

extern int sFM_Search(uint32 freq);

extern int32 fmdrv_init(uint32 para);
extern int32 fmdrv_exit(void);

#endif /*_FM_DRV_H */

