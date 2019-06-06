/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM���������ⲿ�ӿ��б�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "fm_tx_interface.h"
#include "fm_tx_drv.h"

/*FM ��������ӿں���*/
const fm_tx_driver_operations fm_tx_drv_op =
{
#if (SUPPORT_FM_TX == 1)
    (fm_tx_op_func) fm_tx_drv_init, //KEY�����е�FMģ��װ��
    (fm_tx_op_func) fm_tx_drv_exit, //KEY�����е�FMģ��ж��
    (fm_tx_op_func) sFM_TX_Init,    //��ʼ��
    (fm_tx_op_func) sFM_TX_SetFreq, //����Ƶ��
#else
    0,0,0,0
#endif
};

