/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏物理驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _LCD_PHY_INNER_H
#define _LCD_PHY_INNER_H

#include "psp_includes.h"
#include "lcd_interface.h"

extern uint8 DisplayBuffer[DisplayBufLen];

extern region_t region;

void Hdrv_spi1_set_mfp(void);
#define CS_SPINOR      0
#define CS_LCD_MODULE  1
//dev_id 0-spinor,1-lcd
void Hdrv_spi1_switch_cs(uint8 dev_id);
void Hdrv_lcd_write_cmd(uint8 cmd);
void Hdrv_lcd_write_data(uint8 dat);
void Hdrv_lcd_poweron(void);
void Hdrv_lcd_switch_light(bool on);

void Hdrv_UpdateScreen(region_t *updateregion);
void Hdrv_SetWindow(region_t *winreg);
void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len);
void Hdrv_StandbyScreen(bool bStandby);
void Hdrv_SetContrast(uint8 ContrastValue);

extern void lcd_module_install_inner(void);
extern void lcd_module_uninstall_inner(void);
extern void lcd_module_test_inner(void);

#endif //_LCD_INNER_H
