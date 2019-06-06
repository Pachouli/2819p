/*!
 * \file      seg_led.h
 * \brief     数码管显示接口
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */


enum SEG_LED_CHAR
{
    SEG_LED_CHAR_0 = 0x3f,
    SEG_LED_CHAR_1 = 0x06,
    SEG_LED_CHAR_2 = 0x5b,
    SEG_LED_CHAR_3 = 0x4f,
    SEG_LED_CHAR_4 = 0x66,
    SEG_LED_CHAR_5 = 0x6d,
    SEG_LED_CHAR_6 = 0x7d,
    SEG_LED_CHAR_7 = 0x07,
    SEG_LED_CHAR_8 = 0x7f,
    SEG_LED_CHAR_9 = 0x6f,

    SEG_LED_CHAR_A = 0x77,
    SEG_LED_CHAR_B = 0x7f,
    SEG_LED_CHAR_C = 0x39,
    SEG_LED_CHAR_D = 0x3f,
    SEG_LED_CHAR_E = 0x79,
    SEG_LED_CHAR_F = 0x71,
    SEG_LED_CHAR_G = 0x7d,
    SEG_LED_CHAR_H = 0x76,
    SEG_LED_CHAR_I = 0x06,
    SEG_LED_CHAR_J = 0x0f,
    SEG_LED_CHAR_K = 0x40,
    SEG_LED_CHAR_L = 0x38,
    SEG_LED_CHAR_M = 0x40,
    SEG_LED_CHAR_N = 0x40,
    SEG_LED_CHAR_O = 0x3f,
    SEG_LED_CHAR_P = 0x73,
    SEG_LED_CHAR_Q = 0x67,
    SEG_LED_CHAR_R = 0x72,
    SEG_LED_CHAR_S = 0x6d,
    SEG_LED_CHAR_T = 0x78,
    SEG_LED_CHAR_U = 0x3e,
    SEG_LED_CHAR_V = 0x3e,
    SEG_LED_CHAR_W = 0x40,
    SEG_LED_CHAR_X = 0x40,
    SEG_LED_CHAR_Y = 0x40,
    SEG_LED_CHAR_Z = 0x5b,
};


enum SEG_LED_ICON
{
    SEG_LED_ICON_PLAY  = (1 << 0),
    SEG_LED_ICON_PAUSE = (1 << 1),
    SEG_LED_ICON_USB   = (1 << 2),
    SEG_LED_ICON_SD    = (1 << 3),
    SEG_LED_ICON_COL   = (1 << 4),
    SEG_LED_ICON_AUX   = (1 << 5),
    SEG_LED_ICON_FM    = (1 << 6),
    SEG_LED_ICON_DOT   = (1 << 7),
};


/* 数字显示
 */
extern const u8_t  seg_led_number[];


/* 数码管显示
 */
void seg_led_display(u8_t* data);


