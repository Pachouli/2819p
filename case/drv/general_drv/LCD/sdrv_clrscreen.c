/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : UI_ClrScreen.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

////*******************************************************************************/
///*
//** FUNCTION:     ClearScreen
//**
//** Description:  把指定的屏幕区域清为背景色
//**
//**  input
//**        region_t *clrregion :要清屏的区域，如为 NULL 则清全屏
//**
//**  output
//**     none
//********************************************************************************/

void Sdrv_ClearScreen(region_t *clrregion)
{
    uint16 ImgDataLen;

    //全屏清除
    if (clrregion == NULL)
    {
        region_t temp_region;

        temp_region.x = 0;
        temp_region.y = 0;
        temp_region.width = Display_Length;
        temp_region.height = Display_Height;

        ImgDataLen = DisplayBufLen;
        libc_memset(temp_DisplayBuffer, 0, ImgDataLen);
        Hdrv_SetWindow(&temp_region);
        Hdrv_BuffDataTrans(temp_DisplayBuffer, ImgDataLen);

        TextColumn=0;
        TextPage=0;
    }
    else
    {
        uint8 lines;

        if ((clrregion->x >= Display_Length) || (clrregion->y >= Display_Height))
        {
            return;
        }

        if ((clrregion->height + clrregion->y) > Display_Height)
        {
            clrregion->height = Display_Height - clrregion->y;
        }
        if ((clrregion->width + clrregion->x) > Display_Length)
        {
            clrregion->width = Display_Length - clrregion->x;
        }

        if ((clrregion->width == 0) || (clrregion->height == 0))
        {
            return;
        }

        lines = clrregion->height / 8;
        if ((clrregion->height % 8) > 0)
        {
            lines++;
        }
        ImgDataLen = lines*clrregion->width;
        libc_memset(temp_DisplayBuffer, 0, ImgDataLen);

        //如果是阿拉伯和希伯莱语,会先将坐标进行映射处理,然后输出到buffer完后再映射回来
        RegionMapping(clrregion, -1);
        Hdrv_SetWindow(clrregion);
        Hdrv_BuffDataTrans(temp_DisplayBuffer, ImgDataLen);
        RegionMapping(clrregion, -1);
    }

    return;
}

#endif
