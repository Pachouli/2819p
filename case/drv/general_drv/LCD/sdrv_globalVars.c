 /*
********************************************************************************
*                       ACTOS
*                  driver demo
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : UI_S6B33B0A.A51
* Version: 1> v1.00     first version     date
********************************************************************************
*/

#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint8 CharPointBuffer[33];

//设定的输出窗口区域（初始值为屏幕的大小） 
region_t region_win = {0, 0, Display_Length, Display_Height};

uint8 PutCharErrFlag;
uint8 StrEndX;
uint8 Thai_lan;
uint8 TwoByteLan;
int16 PrevAffectsNextJoining;
uint8 backup_endstrx;
uint8 backup_curstrx;
uint8 Language_ID;
uint8 UniBuffer[4] = {0, 0, 0x26, 0};
uint8 ArabicBuf[4] = {0, 0, 0x27, 0};
uint8 AsciiSelect = 4; //large font
uint8 TextColumn;
uint8 TextPage;
uint8 IsArabicOrHebrew;
uint8 AuxiliaryBuffer[AuxiliaryBufferLen];
uint8 MBCodeTwoFlag;
uint8 StrHeight;
uint8 AscFontSmall;
uint8 CharWidth;

#endif
