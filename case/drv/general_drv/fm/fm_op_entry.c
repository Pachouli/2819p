/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM�����ⲿ�ӿ��б�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "fm_interface.h"
#include "fm_drv.h"

/*FM ��������ӿں���*/
const fm_driver_operations fm_drv_op =
{
#if (SUPPORT_FM_FUNC == 1)
    (fm_op_func) fmdrv_init, //KEY�����е�FMģ��װ��
    (fm_op_func) fmdrv_exit, //KEY�����е�FMģ��ж��
    (fm_op_func) sFM_Init,  //��ʼ��
    (fm_op_func) sFM_Standby, //standby
    (fm_op_func) sFM_SetFreq, //����Ƶ��
    (fm_op_func) sFM_GetStatus, //��ȡ״̬��������ǰ����Ƶ�ʡ����������ź�ǿ�ȵ�
    (fm_op_func) sFM_Mute,      //���þ�����������
    (fm_op_func) sFM_SetThrod,  //����������̨�ź�ǿ������
    (fm_op_func) sFM_Search,    //������̨�������Ƿ���Ч��̨
#else
    0,
#endif
};

