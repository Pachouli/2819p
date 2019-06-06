
#ifndef __HAL_LED_EXTERN_H__
#define __HAL_LED_EXTERN_H__

#include <attr.h>
#include <typeext.h>

//public defines

//#define ENABLE_GPIO_LED_DISPLAY

#define COMMON_CATHODE_MODE  0 //π≤“ı
#define COMMON_ANODE_MODE    1 //π≤—Ù

#define LIGHT_LEVEL_2MA      0
#define LIGHT_LEVEL_3MA      1
#define LIGHT_LEVEL_4MA      2
#define LIGHT_LEVEL_5MA      3
#define LIGHT_LEVEL_6MA      4
#define LIGHT_LEVEL_7MA      5
#define LIGHT_LEVEL_8MA      6
#define LIGHT_LEVEL_9MA      7

typedef struct
{
    uint8 common_mode;
    uint8 light_level;
} led_phy_para_t;

typedef struct
{
    uint8 common_mode;
    uint8 light_level;
    uint8 led_pin_num;
    uint8 led_io_pin[8];
} led_io_phy_para_t;

#endif
