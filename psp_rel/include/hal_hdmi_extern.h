
//_manfan 2018.05.18
#ifndef __hal_hdmi_extern_h__
#define __hal_hdmi_extern_h__
#include "hal_pad_extern.h"
#include "cfg_customers.h"

#define SUPPORT_HDMI_MODULE  0 //是否支持HDMI模块

typedef enum 
{
	SMBUS_0,
	SMBUS_1,
	SMBUS_2,
	SMBUS_3,
	SMBUS_4,
	SMBUS_MAX
}SMBUS_ID;

typedef enum 
{
	SMBUS_DividedBy128,
	SMBUS_DividedBy256,
	SMBUS_DividedBy512,
	SMBUS_DividedBy1024
}SMBUS_SPEED;

typedef enum 
{
	SMBUS_Normal,
	SMBUS_SkipStop,
	SMBUS_SkipStart,
	SMBUS_DataOnly
}SMBUS_MODE;

typedef enum 
{
	SMBUS_STATUS_Success,
	SMBUS_STATUS_Pending,
	SMBUS_STATUS_NoAct,
	SMBUS_STATUS_TimeOut,
	SMBUS_STATUS_ArbitrationLoss,
	SMBUS_STATUS_SDA_Low = SMBUS_STATUS_ArbitrationLoss,
}SMBUS_STATUS;

typedef struct
{
    ic_pad_e scl_pad;
    pad_function_e scl_func;
    pad_function_e scl_in_func;
    pad_phy_para_t scl_phy;

    ic_pad_e sda_pad;
    pad_function_e sda_func;
    pad_function_e sda_in_func;
    pad_phy_para_t sda_phy;
} hdmi_hw_info_t;

#endif

