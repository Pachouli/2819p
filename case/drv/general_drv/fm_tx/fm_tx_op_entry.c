/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM发射驱动外部接口列表。
 * 作者：cailizhen
 ********************************************************************************/

#include "fm_tx_interface.h"
#include "fm_tx_drv.h"

/*FM 驱动对外接口函数*/
const fm_tx_driver_operations fm_tx_drv_op =
{
#if (SUPPORT_FM_TX == 1)
    (fm_tx_op_func) fm_tx_drv_init, //KEY驱动中的FM模块装载
    (fm_tx_op_func) fm_tx_drv_exit, //KEY驱动中的FM模块卸载
    (fm_tx_op_func) sFM_TX_Init,    //初始化
    (fm_tx_op_func) sFM_TX_SetFreq, //设置频率
#else
    0,0,0,0
#endif
};

