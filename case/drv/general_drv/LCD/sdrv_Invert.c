/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Sdrv_Invert.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

////*******************************************************************************/
///* ��ָ��������������ʾ
//** FUNCTION:     Sdrv_Invert
//**
//** Description:
//**
//**  input
//**        region_t * D3region: Ҫ��������������С
//**
//**  output
//**     none
//**
//** Note: Ŀǰֻ֧��Page���뷴�ԣ���8��ʾ�ж���
//********************************************************************************/

void Sdrv_Invert (region_t * Invregion)
{
    uint8 ii;
    uint8 i,j,k,l;
    uint16 ir_m,ir_n;
    //���������Ļ�����˳�
    //ybfu
    if( (Invregion->x > (Display_Length-1)) || (Invregion->y > (Display_Height-1)) )
    {
        return;
    }
    //���׳���Ļ�Ĵ���
    j = (Invregion->y)>>3;
    l = (Invregion->height)>>3;
    l = j+l;        //�����ǡ�L�������ǡ�1��
    //ybfu
    if(l>DisplaySuppPage)
    {
        l=DisplaySuppPage;
    }

    k = Invregion->width;
    //ybfu
    if(k > (Display_Length-Invregion->x))
    {
        k=Display_Length-Invregion->x;
    }
    //����ǰ�������ϣ������,���Ƚ��������ӳ�䴦��,Ȼ�������buffer�����ӳ����� by cailizhen
    RegionMapping(Invregion, -1);
    for(i=j; i<l ; i++)
    {
        //ybfu
        /*ir_m = k + i*Display_Length+Invregion->x;       //ʵ��Ҫ���ֹ��ַ
        for(ir_n=(i*Display_Length)+Invregion->x ; ir_n<ir_m ; ir_n++)
        {
            DisplayBuffer[ir_n]=~DisplayBuffer[ir_n];
        }*/

        ii = 7 - i;
        /*if(i==0)ii=7;
        if(i==1)ii=6;
        if(i==2)ii=5;
        if(i==3)ii=4;
        if(i==4)ii=3;
        if(i==5)ii=2;
        if(i==6)ii=1;
        if(i==7)ii=0;*/

        ir_m = k + ii*Display_Length+Invregion->x;       //ʵ��Ҫ���ֹ��ַ
        for(ir_n=(ii*Display_Length)+Invregion->x ; ir_n<ir_m ; ir_n++)
        {
            DisplayBuffer[ir_n]=~DisplayBuffer[ir_n];
        }
    }
    RegionMapping(Invregion, -1);
    //Hdrv_SetFontWindow(Invregion);
    //Hdrv_BuffDataTrans(DisplayBuffer,ir_m);
    return;
}

#endif
