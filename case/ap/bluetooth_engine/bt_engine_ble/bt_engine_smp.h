/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_SMP_H__
#define __BT_ENGINE_SMP_H__

extern void APP_SMP_Callback(unsigned char event, void *arg);
void app_smp_reglocalcap(void *sm_cbk);

#endif //end of #ifndef __BT_ENGINE_SMP_H__
