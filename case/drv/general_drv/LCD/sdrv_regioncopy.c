/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_regioncopy.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

////*******************************************************************************/
///* void Sdrv_RegionCopy(region_t sregion, region_t tregion)
//** FUNCTION:     Sdrv_RegionCopy
//**
//** Description:  把源区域的数据复制到目的区域
//**  input
//**     region_t *sregion: 指定源区域 ;
//**     region_t *tregion: 指定目的区域
//**
//**  output
//**     none
//********************************************************************************/
void Sdrv_RegionCopy(region_t *sregion, region_t *tregion)
{

    uint8 i,j,k;
    uint8 pagenum;
    
    region_t sregionram;//临时使用
    region_t tregionram;//临时使用
    
    //把源目区域数据放到临时区作运算
    libc_memcpy(&sregionram, sregion, sizeof(region_t));
    libc_memcpy(&tregionram, tregion, sizeof(region_t));
    pagenum = sregionram.height/8;
    j=tregionram.y/8;
    pagenum+=j;
    //如果是阿拉伯和希伯莱语,会先将坐标进行映射处理,然后输出到buffer完后再映射回来 by cailizhen
    RegionMapping(&sregionram, -1);
    RegionMapping(&tregionram, -1);
    if ((sregionram.x) >= (tregionram.x))
    {
        //源在目标的下面时，可从上面开始读数据（这样不会覆盖）
        //读出余下不足一个块的区域内容
        for(k=j;k<pagenum;k++)
        {
            for(i=0;i<sregionram.width;i++)
            {
                DisplayBuffer[tregionram.x+i+(7-k)*Display_Length]=DisplayBuffer[sregionram.x+i+(7-k)*Display_Length];
            }
        }
        //把暂时显存区的数据送到LCM
        //Hdrv_SetWindow(&tregionram);
        //Hdrv_BuffDataTrans(DisplayBuffer, Displaylength);
    }
    else
    {
        //源在目标的上面时，要从下面开始读数据（这样不会覆盖）
        for(k=j;k<pagenum;k++)
        {
            for(i=sregionram.width;i>0;i--)
            {
                DisplayBuffer[tregionram.x+i-1+(7-k)*Display_Length]=DisplayBuffer[sregionram.x+i-1+(7-k)*Display_Length];
            }
        }
        //把暂时显存区的数据送到LCM
        //Hdrv_SetWindow(&tregionram);
        //Hdrv_BuffDataTrans(DisplayBuffer, Displaylength);
    }
    RegionMapping(&sregionram, -1);
    RegionMapping(&tregionram, -1);
}

#endif
