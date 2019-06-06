/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：lcd驱动模块相关数据结构，宏定义，接口声明等
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _LCD_INTERFACE_H
#define _LCD_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"
#include "lang_id.h"

//#define LCD_HDRV_TEST

#define Display_Length      128
#define Display_Height      64
#define DisplayBufLen       1024//显示缓冲区的大小

#define	AuxiliaryBufferLen	54
#define RowWordsCountMax	26

#define ASCIIDISCODE        0x80
#define DisplaySuppPage     0x08

//屏幕大小
#define   SCR_WIDTH         128
#define   SCR_HEIGHT        64

/**************************字符的点阵模式********************************/ 
#define UnicodeMode         0    //16*16点阵模式
#define PicDisMode          1    //图形显示模式

/**************************定义UNICODE字符 SIZE*****************************/
#define UNICODE_HEIGHT_DEF     16
#define UNICODE_WIDTH_DEF      16

#define  UNICODELANGUAGE  0x7f    //字符资源为unicode编码模式  

#define UNICODEARABICDONTJOIN 3
#define ISNARABICDONTJOIN  4

//for arabic ligature
#define  ARABICHASJOIN    -2


/********************** lcd_set_option选项参数 ***********************/
#define LCD_OPTION_LANGUAGE			0
#define LCD_OPTION_INVERTSCROLL		1


typedef struct
{
    uint8 x;         //列位置
    uint8 y;         //行位置
    uint8 width;     //宽度
    uint8 height;    //高度
} region_t;

/*!
 * \brief
 *  lcd_cmd_e：led驱动命令类型枚举类型
 */
typedef enum
{
    LCD_MODULE_INSTALL = 0,
    LCD_MODULE_UNINSTALL,
    LCD_MODULE_UPDATE_SCREEN,
    LCD_MODULE_SET_WINDOW,
    LCD_MODULE_STANDBY_SCREEN,
    LCD_MODULE_SET_CONTRAST,
    LCD_MODULE_CLEAR_SCREEN,
    LCD_MODULE_SHOW_IMAGE,
    LCD_MODULE_SHOW_MULTI_STRING,
    LCD_MODULE_SHOW_STRING,
    LCD_MODULE_SET_TEXT_POS,
    LCD_MODULE_SCROLL_STRING,
    LCD_MODULE_SCROLL_MULTI_STRING,
    LCD_MODULE_SCROLL_STRING_SCROLLING,
    LCD_MODULE_INVERT,
    LCD_MODULE_SET_OPTION,
} lcd_cmd_e;

#define LCD_ENTRY_ADDR(x)                         (LCD_ENTRY_VA_START + (4 * x))

#define LCD_MODULE_INSTALL_ENTRY                  (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_INSTALL))))
#define LCD_MODULE_UNINSTALL_ENTRY                (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_UNINSTALL))))
#define LCD_MODULE_UPDATE_SCREEN_ENTRY            (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_UPDATE_SCREEN))))
#define LCD_MODULE_SET_WINDOW_ENTRY               (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SET_WINDOW))))
#define LCD_MODULE_STANDBY_SCREEN_ENTRY           (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_STANDBY_SCREEN))))
#define LCD_MODULE_SET_CONTRAST_ENTRY             (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SET_CONTRAST))))
#define LCD_MODULE_CLEAR_SCREEN_ENTRY             (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_CLEAR_SCREEN))))
#define LCD_MODULE_SHOW_IMAGE_ENTRY               (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SHOW_IMAGE))))
#define LCD_MODULE_SHOW_MULTI_STRING_ENTRY        (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SHOW_MULTI_STRING))))
#define LCD_MODULE_SHOW_STRING_ENTRY              (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SHOW_STRING))))
#define LCD_MODULE_SET_TEXT_POS_ENTRY             (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SET_TEXT_POS))))
#define LCD_MODULE_SCROLL_STRING_ENTRY            (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SCROLL_STRING))))
#define LCD_MODULE_SCROLL_MULTI_STRING_ENTRY      (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SCROLL_MULTI_STRING))))
#define LCD_MODULE_SCROLL_STRING_SCROLLING_ENTRY  (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SCROLL_STRING_SCROLLING))))
#define LCD_MODULE_INVERT_ENTRY                   (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_INVERT))))	
#define LCD_MODULE_SET_OPTION_ENTRY               (*((uint32*)(LCD_ENTRY_ADDR(LCD_MODULE_SET_OPTION))))


#if 0
extern void lcd_module_install_inner(void);
#endif
#define lcd_module_install()            ((void(*)(void))LCD_MODULE_INSTALL_ENTRY)()

#if 0
extern void lcd_module_uninstall_inner(void);
#endif
#define lcd_module_uninstall()          ((void(*)(void))LCD_MODULE_UNINSTALL_ENTRY)()

#if 0
extern void Hdrv_UpdateScreen(region_t *updateregion);
#endif
#define lcd_update_screen(a)            ((void(*)(region_t *))LCD_MODULE_UPDATE_SCREEN_ENTRY)((a))

#if 0
extern void Hdrv_SetWindow(region_t *winreg);
#endif
#define lcd_set_window(a)               ((void(*)(region_t *))LCD_MODULE_SET_WINDOW_ENTRY)((a))

#if 0
extern void Hdrv_StandbyScreen(bool bStandby);
#endif
#define lcd_standby_screen(a)           ((void(*)(bool))LCD_MODULE_STANDBY_SCREEN_ENTRY)((a))

#if 0
extern void Hdrv_SetContrast(uint8 ContrastValue);
#endif
#define lcd_set_contrast(a)             ((void(*)(uint8))LCD_MODULE_SET_CONTRAST_ENTRY)((a))

#if 0
extern void Sdrv_ClearScreen(region_t *clrregion);
#endif
#define lcd_clear_screen(a)             ((void(*)(region_t *))LCD_MODULE_CLEAR_SCREEN_ENTRY)((a))

#if 0
extern void Sdrv_PutSDImage(uint8 x, uint8 y, uint32 img_id);
#endif
#define lcd_show_image(a,b,c)           ((void(*)(uint8, uint8, uint32))LCD_MODULE_SHOW_IMAGE_ENTRY)((a),(b),(c))

#if 0
extern void Sdrv_PutMultiS(uint8 x, uint8 y, uint32 str_id);
#endif
#define lcd_show_multi_string(a,b,c)    ((void(*)(uint8, uint8, uint32))LCD_MODULE_SHOW_MULTI_STRING_ENTRY)((a),(b),(c))

#if 0
extern uint8* Sdrv_PutS(uint8 *str, uint32 str_len);
#endif
#define lcd_show_string(a,b)            ((uint8 *(*)(uint8 *, uint32))LCD_MODULE_SHOW_STRING_ENTRY)((a),(b))

#if 0
extern void Sdrv_SetTextPos(uint8 x,uint8 y);
#endif
#define lcd_set_text_pos(a,b)           ((void(*)(uint8, uint8))LCD_MODULE_SET_TEXT_POS_ENTRY)((a),(b))

#if 0
extern bool Sdrv_ScrollString(uint8 *str, region_t *r_scroll);
#endif
#define lcd_scroll_string(a,b)          ((bool(*)(uint8*, region_t *))LCD_MODULE_SCROLL_STRING_ENTRY)((a),(b))

#if 0
extern bool Sdrv_ScrollMultiString(uint32 str_id, region_t *r_scroll);
#endif
#define lcd_scroll_multi_string(a,b)    ((bool(*)(uint32, region_t *))LCD_MODULE_SCROLL_MULTI_STRING_ENTRY)((a),(b))

#if 0
extern void Sdrv_ScrollStringScrolling(uint8 step);
#endif
#define lcd_scroll_string_scrolling(a)  ((void(*)(uint8))LCD_MODULE_SCROLL_STRING_SCROLLING_ENTRY)((a))

#if 0
extern void Sdrv_Invert (region_t * Invregion);
#endif
#define lcd_invert(a)	                ((void(*)(region_t *))LCD_MODULE_INVERT_ENTRY)((a))

#if 0
extern void Sdrv_SetOption(uint8 type, uint8 param);
#endif
#define lcd_set_option(a,b)             ((void(*)(uint8, uint8))LCD_MODULE_SET_OPTION_ENTRY)((a),(b))

#endif
