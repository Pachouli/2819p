/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏逻辑驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _LCD_LOGIC_INNER_H
#define _LCD_LOGIC_INNER_H

#include "psp_includes.h"
#include "lcd_interface.h"
#include "lcd_phy/lcd_phy_inner.h"
#include "ui30_struct.h"

extern uint32 pic_entry_offset;
extern uint32 string_entry_offset;
extern void *res_lib_fd;
extern uint8 temp_DisplayBuffer[DisplayBufLen];
void ui30_file_parse(void);

extern uint8 scroll_buf[104];

extern bool Sdrv_Init(uint8 mode);

extern void *FontLib_lib_fd;
extern void *FontAscLib_lib_fd;
extern void *MBtoUnicTab_lib_fd;
extern uint8 g_table_index;

extern bool Sdrv_ScrollString(uint8 *str, region_t *r_scroll);
extern bool Sdrv_ScrollMultiString(uint32 str_id, region_t *r_scroll);
extern void Sdrv_ScrollStringScrolling(uint8 step);

extern uint8* Sdrv_PutStringExt(uint8 *str, uint32 strlen, bool IsUnicode, uint8 mode);
extern void Sdrv_SetStrEndPosition(uint8 str_endx);

extern void utf8_to_unicode(uint8* src, uint16*dest, uint16* size);

//for clear screen
void Sdrv_ClearScreen(region_t *clrregion);

void Sdrv_SetContrast(uint8 ContrastValue);

//for font size
void Sdrv_SetAsciiFont(uint8 FontID);

//for put out char
void Sdrv_PutMultiS(uint8 x, uint8 y, uint32 str_id);
uint8 * Sdrv_PutS(uint8 *str, uint32 strlen);
void ConvertEnglish(uint8 *str, uint16 CharCount);  
uint8 * Sdrv_PutUnS(uint8 *str, uint32 strlen);
uint8 Sdrv_PutChar(uint8 *str, bool IsUnicode, uint16 ucode);
//for picture display
void Sdrv_PutSDImage(uint8 x, uint8 y, uint32 img_id);
bool GUI_BIDI_IsNSM(uint16 c);
//add for trans deal
void Sdrv_GetFontPoint(uint16 ucode, uint8 *font_point_buf);
void Sdrv_GetUniCode(uint16 mbcode, uint16 *ucode);
void Sdrv_AsciiGetFontPoint(uint16 code_num,uint8* databuf);
extern void Hdrv_SetWindow(region_t *winreg);
bool Sdrv_LanguageSet(uint8 lang_id);
uint16 Sdrv_FontCodeMBToUni(uint16 code_rename);
uint8 Sdrv_CharOutRect(uint8 *str, region_t *CRregion, uint8 colcnt, uint8 language);
void Sdrv_SetTextPos(uint8 x, uint8 y);

void Sdrv_GetTextPos(uint8 *x,uint8 *y);
void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len); 

uint8 CopyStr(uint8 *Dst, uint8 *src, uint16 CharCount, bool IsSwap);
uint8 TurnArabicToUn(uint8* charbuf, bool IsUnicode, uint16 CharCount);
uint8 Sdrv_GetStringLength(uint8 *str, uint16 CharCount, uint8 str_type);
void Sdrv_ShowPinpu(uint8 energy, const uint8 *Pinpu_data, region_t *Pinpu_region);

uint16 Sdrv_UniToChar(uint16 unichar);
uint8* Sdrv_MultiLanDeal(uint8* srcstr,uint8 strtype,uint16* fontcode);
void Sdrv_MultiGetFontPoint(uint16 code_rename, uint8 *picbuf);
//add for thai language
//uint8 Sdrv_ThaiPoint(const uint8 * str, uint8 IsUnicode, uint16 sorcecode);

//add for arabic language
uint8* Sdrv_SpecialLanDeal(const uint8 *lanstr, uint32 length, uint8* chartype);

uint8 UnicodeTOArabic(uint8 *str,uint8 length);
bool Arabic_IsNSM(uint16 c);

uint8 Sdrv_ThaiOverlap(uint8 * str, bool IsUnicode);

uint8 Sdrv_AraCharToUni(uint8 *str,uint8* deststr,uint8 length);
bool CheckDisplaySpace(uint8 codemod,uint8 fontwidth);
void RegionMapping(region_t *region, uint8 justwidth);

extern void Sdrv_Invert (region_t * Invregion);
extern bool Sdrv_SetOption(uint8 type, uint8 param);


extern void * Sdrv_open_lib(const uint8*filename);
extern void Sdrv_read_lib(void *fp, void *buffer, uint32 len);
extern void Sdrv_seek_lib(void * fp, uint8 whence, int32 offset);
extern void Sdrv_close_lib(void * fp);

//以下为全局变量
extern uint8 DisplayBuffer[];
extern uint8 CharPointBuffer[];

extern region_t region_win;//设定的屏幕窗口

extern uint8 StrHeight;//当前行字符的高度
extern uint8 PutCharErrFlag;//保存字符输出时的出错类型

extern uint8 Thai_lan;
extern uint8 TwoByteLan;

extern int16 PrevAffectsNextJoining;

//extern uint8 backup_endstrx;
extern uint8 backup_curstrx;

extern uint8 Language_ID;

extern uint8 UniBuffer[];
extern uint8 ArabicBuf[];

extern uint8 cesel;

extern uint8 fontlibtype;
extern uint8 AsciiSelect;
extern uint8 StrEndX;
extern uint8  TextColumn;
extern uint8  TextPage;
extern uint8 IsArabicOrHebrew;
extern uint8 AuxiliaryBuffer[];
extern uint8 MBCodeTwoFlag;
extern uint8 AscFontSmall;
extern uint8 CharWidth;

extern uint8 gInvertScrollRegion;

extern uint8 g_is_str_end;
extern uint8 g_last_char_width;
extern uint8 g_last_scan_len;


#endif
