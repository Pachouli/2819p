/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM驱动外部接口列表。
 * 作者：cailizhen
 ********************************************************************************/

#include "fm_interface.h"
#include "fm_drv.h"

/*FM 驱动对外接口函数*/
const fm_driver_operations fm_drv_op =
{
#if (SUPPORT_FM_FUNC == 1)
    (fm_op_func) fmdrv_init, //KEY驱动中的FM模块装载
    (fm_op_func) fmdrv_exit, //KEY驱动中的FM模块卸载
    (fm_op_func) sFM_Init,  //初始化
    (fm_op_func) sFM_Standby, //standby
    (fm_op_func) sFM_SetFreq, //设置频率
    (fm_op_func) sFM_GetStatus, //获取状态，包括当前播放频率、立体声、信号强度等
    (fm_op_func) sFM_Mute,      //设置静音或解除静音
    (fm_op_func) sFM_SetThrod,  //设置搜索电台信号强度门限
    (fm_op_func) sFM_Search,    //搜索电台，返回是否有效电台
#else
    0,
#endif
};

