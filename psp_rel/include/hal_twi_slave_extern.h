
#ifndef __HAL_TWI_SLAVE_EXTERN_H__
#define __HAL_TWI_SLAVE_EXTERN_H__

#include <attr.h>
#include <typeext.h>

//public defines

typedef bool (*TWI_slave_read_handle)(uint8);
typedef uint8 (*TWI_slave_write_handle)(void);
typedef void (*TWI_slave_stop_handle)(void);
typedef void (*TWI_slave_buserr_handle)(void);

typedef struct
{
    uint8 addr; //slave device address, ·ÅÔÚbit1 ~ bit7, bit0 fix 0
    TWI_slave_read_handle r_handle;
    TWI_slave_write_handle w_handle;
    TWI_slave_stop_handle stop_handle;
    TWI_slave_buserr_handle err_handle;
} twi_slave_hw_info_t;

#endif
