/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM�����ⲿ�ӿ��б�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "ext_rtc_inner.h"

/*RTC ��������ӿں���*/
const ext_rtc_driver_operations ext_rtc_drv_op =
{
#if (SUPPORT_EXT_RTC == 1)
    (ext_rtc_op_func) rtcdrv_init, //KEY�����е�RTCģ��װ��
    (ext_rtc_op_func) rtcdrv_exit, //KEY�����е�RTCģ��ж��
    (ext_rtc_op_func) sRTC_Init,  //��ʼ��

    (ext_rtc_op_func) sRTC_set_Time, //����ʱ��
    (ext_rtc_op_func) sRTC_get_Time, //��ȡʱ��
    (ext_rtc_op_func) sRTC_set_Date, //��������
    (ext_rtc_op_func) sRTC_get_Date,      //��ȡ����
    (ext_rtc_op_func) sRTC_set_AlarmTime,  //��������
    (ext_rtc_op_func) sRTC_get_AlarmTime,  //��������
    (ext_rtc_op_func) sRTC_enable_Alarm,    //ʹ������
    (ext_rtc_op_func) sRTC_disable_Alarm,    //�ر�����
    (ext_rtc_op_func) sRTC_get_status,    //��ȡʱ��/����״̬
#else
    0,
#endif
};

