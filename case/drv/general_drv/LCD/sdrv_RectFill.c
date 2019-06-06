/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Sdrv_RectFill.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

/*
*******************************************************************************
*
** FUNCTION:     CheckDisplaySpace
**
** Description:  检测屏幕空间是否满足显示要求，如超出范围则报错
**
**  input
**     codemod: 点阵的模式，字符点阵和图片数据
**
**  output
**     True: 屏幕空间能满足显示要求
**     False: 屏幕空间不能满足显示要求
********************************************************************************
*/
bool CheckDisplaySpace_Rect(uint8 codemod,uint8 fontwidth)
{
    uint8 justwidth;
    
    switch(codemod)
    {
    case UnicodeMode:
        justwidth = fontwidth;
        if( (StrEndX != 0) && ( (region.x+justwidth) > StrEndX))
        {
//          DUMP(RESULT_PUTCHAR_X_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X轴超出
            return FALSE;
        }
        goto NextStep;
    case PicDisMode:
        justwidth = region.width;
        NextStep:
        //判断是否X轴和y轴超出
        if ((region.x+justwidth) > (region_win.x+region_win.width))
        {
//          DUMP(RESULT_PUTCHAR_X_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X轴超出
            return FALSE;
        }
        if ((region.y+region.height) > (region_win.y+region_win.height))
        {
//          DUMP(RESULT_PUTCHAR_Y_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_Y_ERR;//Y轴超出
            return FALSE;
        }
        break;
    default:
//      DUMP(RESULT_PUTCHAR_OTHER_ERR);
        //PutCharErrFlag = RESULT_PUTCHAR_OTHER_ERR;//不知类型的也不显示
        return FALSE;
    }
    
    return TRUE;
}

////*******************************************************************************/
///* 把指定区域填充成画笔颜色
//** FUNCTION:     Sdrv_FillRect
//**
//** Description:  
//**
//**  input
//**        region_t * Fillregion:要填充的区域 
//**        
//**  output
//**     none
//********************************************************************************/

void Sdrv_FillRect (region_t * Fillregion)
{

    uint16 pixelcnt;
    libc_memcpy(&region, Fillregion, sizeof(region_t));

    if ( (region.x > (Display_Length-1))
      || (region.y > (Display_Height-1)) )
    {
        return; //坐标超出，退出
    }     

    if (CheckDisplaySpace_Rect(PicDisMode,0) == TRUE) //检测显示区域是否超出
    {
        Hdrv_SetWindow(&region);//设定区域 //设定 要填入数据的 起始地址 和 结束地址
        pixelcnt = region.width * region.height;
        //RectBuffFillColor(DisplayBufLen/2);
        while(pixelcnt > (DisplayBufLen/2))
        {
            //找出整个PAGE的送出
//            RectBuffFillColor(DisplayBufLen/2);
            Hdrv_BuffDataTrans(DisplayBuffer, DisplayBufLen);//把暂时显存区DisplayBuffer里面的数据送到LCM
            //这里只管 填入数据，不管地方，因为在这之前 Hdrv_SetWindow 已经设定好了起始位置地点
            
            pixelcnt = pixelcnt - (DisplayBufLen/2);
        }
        //把剩余的数据送出
//        RectBuffFillColor(pixelcnt);
        Hdrv_BuffDataTrans(DisplayBuffer, pixelcnt*2);//把暂时显存区的数据送到LCM                
    }    
    
    return;
}

#endif
