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
//** Description:  ��Դ��������ݸ��Ƶ�Ŀ������
//**  input
//**     region_t *sregion: ָ��Դ���� ;
//**     region_t *tregion: ָ��Ŀ������
//**
//**  output
//**     none
//********************************************************************************/
void Sdrv_RegionCopy(region_t *sregion, region_t *tregion)
{

    uint8 i,j,k;
    uint8 pagenum;
    
    region_t sregionram;//��ʱʹ��
    region_t tregionram;//��ʱʹ��
    
    //��ԴĿ�������ݷŵ���ʱ��������
    libc_memcpy(&sregionram, sregion, sizeof(region_t));
    libc_memcpy(&tregionram, tregion, sizeof(region_t));
    pagenum = sregionram.height/8;
    j=tregionram.y/8;
    pagenum+=j;
    //����ǰ�������ϣ������,���Ƚ��������ӳ�䴦��,Ȼ�������buffer�����ӳ����� by cailizhen
    RegionMapping(&sregionram, -1);
    RegionMapping(&tregionram, -1);
    if ((sregionram.x) >= (tregionram.x))
    {
        //Դ��Ŀ�������ʱ���ɴ����濪ʼ�����ݣ��������Ḳ�ǣ�
        //�������²���һ�������������
        for(k=j;k<pagenum;k++)
        {
            for(i=0;i<sregionram.width;i++)
            {
                DisplayBuffer[tregionram.x+i+(7-k)*Display_Length]=DisplayBuffer[sregionram.x+i+(7-k)*Display_Length];
            }
        }
        //����ʱ�Դ����������͵�LCM
        //Hdrv_SetWindow(&tregionram);
        //Hdrv_BuffDataTrans(DisplayBuffer, Displaylength);
    }
    else
    {
        //Դ��Ŀ�������ʱ��Ҫ�����濪ʼ�����ݣ��������Ḳ�ǣ�
        for(k=j;k<pagenum;k++)
        {
            for(i=sregionram.width;i>0;i--)
            {
                DisplayBuffer[tregionram.x+i-1+(7-k)*Display_Length]=DisplayBuffer[sregionram.x+i-1+(7-k)*Display_Length];
            }
        }
        //����ʱ�Դ����������͵�LCM
        //Hdrv_SetWindow(&tregionram);
        //Hdrv_BuffDataTrans(DisplayBuffer, Displaylength);
    }
    RegionMapping(&sregionram, -1);
    RegionMapping(&tregionram, -1);
}

#endif
