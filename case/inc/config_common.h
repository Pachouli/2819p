/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������ػ�Ӧ����غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ����Ӧ�ù���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __CONFIG_COMMON_H__
#define __CONFIG_COMMON_H__

#include "psp_includes.h"

/* ����Ӧ��ģʽ */
#define CONFIG_MODE_POWER_ON     0 //��������������ԭ����ϵͳ��ȡ
#define CONFIG_MODE_POWER_OFF    1 //�����ػ���ʡ��ػ�
#define CONFIG_MODE_LOWPOWER     2 //�͵�ػ�
#define CONFIG_MODE_ALARM_OFF    3 //������Ӧ���Զ��ػ�
#define CONFIG_MODE_CHARGE_LOOP  4 //�͹���ģʽUSB���ѽ�����г��ģʽ
#define CONFIG_MODE_JUST_OFF     5 //�͹���ģʽ��������ֱ�ӹػ���������TTS

#endif //__CONFIG_COMMON_H__
