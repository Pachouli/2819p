/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������led��������ģ��������ݽṹ���궨�壬�ӿ�������
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _LED_INTERFACE_H
#define _LED_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"

#define NUMBER1    0
#define NUMBER2    1
#define NUMBER3    2
#define NUMBER4    3

#define LCD_PLAY   0    //ICON "PLAY"
#define LCD_PAUSE  1    //ICON "PAUSE"
#define LCD_USB    2    //ICON "USB"
#define LCD_SD     3    //ICON "SD"
#define LCD_COL    4    //ICON ":"
#define LCD_AUX    5    //ICON "AUX"
#define LCD_FM     6    //ICON "FM"
#define LCD_P1     7    //ICON "."
#define ICON_TOTAL 8

#define LED_CLR_ALL  0 //���ȫ�����ֺ�ͼ��
#define LED_CLR_NUM  1 //���������

/*!
 * \brief
 *  lcd_cmd_e��led������������ö������
 */
typedef enum
{
    LED_MODULE_INSTALL = 0,
    LED_MODULE_UNINSTALL,
    /*!��ʾLED���ֻ���ĸ��̻���*/
    LED_DISPLAY,
    /*!��ʾͼ��*/
    LED_DISPLAY_ICON,
    /*!�����Ļ*/
    LED_CLEAR_SCREEN,
    /*!��˸��Ļ*/
    LED_FLASH_SCREEN,
    /*����LED��ʾ*/
    LED_WAKE_UP,
    /*��LED��ʾ*/
    LED_SLEEP,
    /*����LED���⣬����ر���Ȼ������������ݣ����ǲ�ˢ��LED�������*/
    LED_LIGHT_ONOFF,
} led_cmd_e;

#define LED_ENTRY_ADDR(x)          (LED_ENTRY_VA_START + (4 * x))

#define LED_MODULE_INSTALL_ENTRY   (*((uint32*)(LED_ENTRY_ADDR(LED_MODULE_INSTALL))))
#define LED_MODULE_UNINSTALL_ENTRY (*((uint32*)(LED_ENTRY_ADDR(LED_MODULE_UNINSTALL))))
#define LED_DISPLAY_ENTRY          (*((uint32*)(LED_ENTRY_ADDR(LED_DISPLAY))))
#define LED_DISPLAY_ICON_ENTRY     (*((uint32*)(LED_ENTRY_ADDR(LED_DISPLAY_ICON))))
#define LED_CLEAR_SCREEN_ENTRY     (*((uint32*)(LED_ENTRY_ADDR(LED_CLEAR_SCREEN))))
#define LED_FLASH_SCREEN_ENTRY     (*((uint32*)(LED_ENTRY_ADDR(LED_FLASH_SCREEN))))
#define LED_WAKE_UP_ENTRY          (*((uint32*)(LED_ENTRY_ADDR(LED_WAKE_UP))))
#define LED_SLEEP_ENTRY            (*((uint32*)(LED_ENTRY_ADDR(LED_SLEEP))))
#define LED_LIGHT_ONOFF_ENTRY      (*((uint32*)(LED_ENTRY_ADDR(LED_LIGHT_ONOFF))))

#if 0
extern int32 led_module_init_inner(void);
#endif
#define led_drv_module_install()       ((int(*)(void))LED_MODULE_INSTALL_ENTRY)()

#if 0
extern int32 led_module_exit_inner(void);
#endif
#define led_drv_module_uninstall()     ((int(*)(void))LED_MODULE_UNINSTALL_ENTRY)()

/*!
 * \brief��ʾLED
 * input: num_idx--0-3
 *        c--�������ݣ�ascii�ַ�
 *        display--��ʾ����ʾ
 */
#if 0
extern int32 led_display_inner(uint8 num_idx, uint8 c, bool display);
#endif
#define led_drv_display(a,b,c)         ((int(*)(uint8, uint8, bool))LED_DISPLAY_ENTRY)((a),(b),(c))

/*!
 * \brief��ʾLED
 * input: icon_idx--0-7
 *        display--��ʾ����ʾ
 */
#if 0
extern int32 led_display_icon_inner(uint8 icon_idx, bool display);
#endif
#define led_drv_display_icon(a,b)      ((int(*)(uint8, bool))LED_DISPLAY_ICON_ENTRY)((a),(b))

/*!
 * \brief
 *   �����Ļ
 */
#if 0
extern int32 led_clear_screen_inner(uint8 clr_mode);
#endif
#define led_drv_clear_screen(a)        ((int(*)(uint8))LED_CLEAR_SCREEN_ENTRY)((a))

/*!
 * \brief
 *   ��˸��Ļ
 *input: onoff--��˸���أ�ȡֵ��1��ʾ������ʾԭ���ݣ�0��ʾ�أ�������ݣ�
 */
#if 0
extern int32 led_flash_screen_inner(uint8 onoff);
#endif
#define led_drv_flash_screen(a)        ((int(*)(bool))LED_FLASH_SCREEN_ENTRY)((a))

#if 0
extern int32 led_wake_up_display_inner(void);
#endif
#define led_drv_wake_up_display()      ((int(*)(void))LED_WAKE_UP_ENTRY)()

#if 0
extern int32 led_sleep_display_inner(void);
#endif
#define led_drv_sleep_display()        ((int(*)(void))LED_SLEEP_ENTRY)()

#if 0
extern int32 led_light_onoff_inner(bool onoff);
#endif
#define led_drv_light_onoff(a)         ((int(*)(bool))LED_LIGHT_ONOFF_ENTRY)((a))

/*! \endcond */

#endif
