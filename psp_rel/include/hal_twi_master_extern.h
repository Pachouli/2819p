
#ifndef __HAL_TWI_MASTER_EXTERN_H__
#define __HAL_TWI_MASTER_EXTERN_H__

#include <attr.h>
#include <typeext.h>

#include "hal_pad_extern.h"

//public defines

#define TWI_MASTER_HW   0 //Ӳ��TWI Master�ӿ�
#define TWI_MASTER_SIM  1 //���ģ��TWI Master�ӿ�

#define TWI_READ_MODE_NO   0
#define TWI_READ_MODE_YES  1

//���ģ��ӿ�
typedef struct
{
    ic_pad_e scl_pad;
    pad_function_e scl_func;
    pad_phy_para_t scl_phy;
    ic_pad_e sda_pad;
    pad_function_e sda_func;
    pad_function_e sda_in_func;
    pad_phy_para_t sda_phy;
    uint32 bitrate_bps; //�������ݴ���Ч��
    uint16 delay_chain_us; //���ߵ�ƽ�仯����ʱ��
} twi_hw_info_t;

//Ӳ���ӿ�
typedef struct
{
    uint32 bitrate_bps; //�������ݴ���Ч�� 100/400
} twi_master_hw_info_t;

#endif
