/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������KEY����Ӳ����ʱ���ӿڣ���������ɨ�衢��ص�����⡣
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "key_inner.h"

void key_inner_timer_handle(void)
{
    //����ɨ��
    key_inner_key_scan();

    //��ص�������
    key_sampling_battery_voltage();
}
