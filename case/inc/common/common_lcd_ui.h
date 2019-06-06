/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：用户交互功能接口相关的宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_LCD_UI_H_
#define _COMMON_LCD_UI_H_

#include "common_ui.h"
#include "ui_res.h"

typedef struct
{
	int16 min;
	int16 max;
	int16 value;
	int16 step;
} slider_t;

typedef void (*slider_callback)(int16 value);

#define   SLIDER_BAR_WIDTH      (98)
#define   SLIDER_BG_WIDTH       (4)
#define   POSITION_SLIDERBAR    (15)

#if 0
extern app_result_e ui_slider_inner(slider_t *slider, slider_callback s_cbk);
#endif
#define ui_slider(a,b)  ((app_result_e(*)(slider_t *, slider_callback))COM_UI_SLIDER)((a),(b))

#endif
