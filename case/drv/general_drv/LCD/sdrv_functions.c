/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_functions.c
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
** FUNCTION:     SetStrEndPosition
**
** Description:  ������ʾ������ʾ���ֵĽ���λ��
**
**  input
**     StrEnd: ����ַ�������λ�õ����꣬0������������н���λ�ã������������������
**
**  output
**     True: NONE
********************************************************************************
*/
void Sdrv_SetStrEndPosition(uint8 str_endx)
{
    StrEndX = str_endx;
}

//��������ϣ������Щ���ҵ����Ķ���������Ե����⴦��
//justwidth -1��ʾremap������ֵ��ʾmap
void RegionMapping(region_t *region, uint8 justwidth)
{
    if(IsArabicOrHebrew != 0)
    {
        if(justwidth == (uint8)(-1))
        {
            region->x = Display_Length - region->x - region->width;
        }
        else
        {
            region->x = Display_Length - region->x - justwidth;
            TextColumn = region->x;
        }
    }
}

//��ʾ��Ƶ����Ƶ��
void Sdrv_ShowPinpu(uint8 energy, const uint8 *Pinpu_data, region_t *Pinpu_region)
{
    uint8 i, j;
    uint8 page_begin = Pinpu_region->y / 8;
    uint8 col_begin;
    uint8 height, bytes, bits;
    uint16 current_byte;
    
    if(IsArabicOrHebrew != 0)
    {
        col_begin = Display_Length - Pinpu_region->x - Pinpu_region->width;
    }
    else
    {
        col_begin = Pinpu_region->x;
    }
    //����ϴ�Ƶ�ף�Ƶ�׸߶�Ϊ4 page�����Ϊ48
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 48; j++)
        {
            DisplayBuffer[128 * (page_begin + i) + (col_begin + j)] = 0x00;
        }
    }

    //��ʾ��Ƶ�ף�12��Ƶ����
    for(i = 0; i < 12; i++)
    {
        height = Pinpu_data[i] * (energy + 1);
        if(height > 32)
        {
            height = 32;
        }
        bytes = height / 8;
        bits = height % 8;
        
        if(bytes > 0)
        {
            for(j = 0; j < bytes; j++)
            {
                current_byte = 128*(page_begin + (3-j)) + (col_begin + 4*i);
                DisplayBuffer[current_byte] = 0xff;
                DisplayBuffer[current_byte + 1] = 0xff;
            }
        }
        
        if(bits > 0)
        {
            current_byte = 128*(page_begin + (3-bytes)) + (col_begin + 4*i);
            for(j = 0; j < bits; j++)
            {
                DisplayBuffer[current_byte] |= (1<<j);
                DisplayBuffer[current_byte + 1] |= (1<<j);
            }
        }
    }
}

#endif
