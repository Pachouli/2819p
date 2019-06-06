/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������̼���������غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _FW_CONFIG_H_
#define _FW_CONFIG_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"

//�������͵�������ο��ṹ��

/*!
 * \brief
 *  config_string_t �ַ������������ݽṹ
 */
typedef struct
{
    /*! �ַ������ݣ��ɱ䳤���飬������txt����һ�£���'\0'���� */
    uint8 value[1];
} config_string_t;

/*!
 * \brief
 *  config_linear_t ������ֵ���������ݽṹ
 */
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ȡֵ�������Сֵ */
    uint16 min;
    /*! ȡֵ��������ֵ */
    uint16 max;
    /*! ���� */
    uint16 step;
} config_linear_t;

/*!
 * \brief
 *  config_nonlinear_t ��������ֵ���������ݽṹ
 */
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��Чֵ���飬�ɱ䳤���� */
    uint16 value[1];
} config_nonlinear_t;

/*!
 * \brief
 *  config_integer_t 32λ������ֵ�������������ݽṹ
 */
typedef struct
{
    uint32 value;
} config_integer_t;

//������ӿ�����
#if 0
extern bool com_get_config_struct_inner(uint16 config_id, uint8 *buf, uint16 buf_len);
#endif
#define com_get_config_struct(a,b,c)   ((bool(*)(uint16 , uint8 *, uint16 ))COM_COM_GET_CONFIG_STRUCT)((a),(b),(c))

#if 0
extern uint16 com_get_config_default_inner(uint16 config_id);
#endif
#define com_get_config_default(a)      ((bool(*)(uint16 ))COM_COM_GET_CONFIG_DEFAULT)((a))

#endif

