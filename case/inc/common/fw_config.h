/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：固件配置项相关宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _FW_CONFIG_H_
#define _FW_CONFIG_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"

//各种类型的配置项参考结构体

/*!
 * \brief
 *  config_string_t 字符串配置项数据结构
 */
typedef struct
{
    /*! 字符串内容，可变长数组，内容与txt输入一致，以'\0'结束 */
    uint8 value[1];
} config_string_t;

/*!
 * \brief
 *  config_linear_t 线性数值配置项数据结构
 */
typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 取值区间的最小值 */
    uint16 min;
    /*! 取值区间的最大值 */
    uint16 max;
    /*! 步长 */
    uint16 step;
} config_linear_t;

/*!
 * \brief
 *  config_nonlinear_t 非线性数值配置项数据结构
 */
typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 有效值个数 */
    uint16 total;
    /*! 有效值数组，可变长数组 */
    uint16 value[1];
} config_nonlinear_t;

/*!
 * \brief
 *  config_integer_t 32位整型数值参数配置项数据结构
 */
typedef struct
{
    uint32 value;
} config_integer_t;

//配置项接口声明
#if 0
extern bool com_get_config_struct_inner(uint16 config_id, uint8 *buf, uint16 buf_len);
#endif
#define com_get_config_struct(a,b,c)   ((bool(*)(uint16 , uint8 *, uint16 ))COM_COM_GET_CONFIG_STRUCT)((a),(b),(c))

#if 0
extern uint16 com_get_config_default_inner(uint16 config_id);
#endif
#define com_get_config_default(a)      ((bool(*)(uint16 ))COM_COM_GET_CONFIG_DEFAULT)((a))

#endif

