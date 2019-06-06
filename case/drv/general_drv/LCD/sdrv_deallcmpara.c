/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_deallcmpara.c
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
*  FUNCTION:     SetAsciiFont
*  Description:  设置Ascii字型
*  input
*  FontID: ascii的字型       
*  output
*  none
********************************************************************************
*/
void Sdrv_SetAsciiFont(uint8 FontID)
{
    AsciiSelect = FontID;
    return;
}

#endif
