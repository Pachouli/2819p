/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������û��������ܽӿ���صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
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
