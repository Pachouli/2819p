/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_setoption.c
* By     : jc0245
* Version: 1> v1.00     first version     2018/07/04  18:12
* Description: set some global param option
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)



bool Sdrv_SetOption(uint8 type, uint8 param)
{
    if(0 == type)  // 设置默认语言
    {
        Sdrv_LanguageSet(param);  //默认语言设置
    }
    else if(1 == type)  // 滚动是否反白
    {
        gInvertScrollRegion = param;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


#endif

