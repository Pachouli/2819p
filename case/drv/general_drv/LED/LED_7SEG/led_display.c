
/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*    brief    显示LED，供AP调用
*   ICON集合到某一个具体驱动ICON顺序转换表,0xff表示该驱动下没有此ICON
*   表中其他值为该驱动下ICON的排列顺序；此表顺序与显示硬件模块列表顺序一致
*      <author>       <time>
*       Wekan       2015-3-26pm
*******************************************************************************/


#include "led_inner.h"

bool led_light_flag;

uint8 cur_disval[8];
//7 segment matrix
/********************************************************
 -----------------------------------------
 7    6    5    4    3    2    1    0  -- BYTE INDEX(LINE/COMX)
 ------------------------------------------
 6    /    4A   K8   /    K6   /    /
 ------------------------------------------
 5   4B    /    4F   4D   3E   K3   3D
 -----------------------------------------
 4   4G   4C    /    3A   3G   2C   1D
 -----------------------------------------
 3   /    4E   3C    /    3F   2G   1C
 -----------------------------------------
 2   K7   K2   3B   K5    /    2F   1G
 -----------------------------------------
 1   /    2D   2E   2B    2A   /    1F
 -----------------------------------------
 0   /    K1   K4   1E    1B   1A    /
 -----------------------------------------
 BIT(COLUME/PINX)
 *******************************************************/

const uint8 number_seg_val[] =
{
    0x3f, //number0
    0x06, //number1
    0x5b, //number2
    0x4f, //number3
    0x66, //number4
    0x6d, //number5
    0x7d, //number6
    0x07, //number7
    0x7f, //number8
    0x6f, //number9
};

const uint8 dash_line_seg_val = 0x40;
const uint8 error_seg_val = 0x40;

const uint8 alphabet_upper_seg_val[] =
{
    0x77, //'A'
    0x7f, //'B'
    0x39, //'C'
    0xff, //'D'不显示，会显示为error-
    0x79, //'E'
    0x71, //'F'
    0x7d, //'G'
    0x76, //'H'
    0x06, //'I'
    0x0f, //'J'
    0xff, //'K'不显示，会显示为error-
    0x38, //'L'
    0xff, //'M'不显示，会显示为error-
    0xff, //'N'不显示，会显示为error-
    0x3f, //'O'
    0x73, //'P'
    0x67, //'Q'
    0x72, //'R'
    0x6d, //'S'
    0x78, //'T'
    0x3e, //'U'
    0x3e, //'V'
    0xff, //'W'不显示，会显示为error-
    0xff, //'X'不显示，会显示为error-
    0xff, //'Y'不显示，会显示为error-
    0x5b, //'Z'
};

const uint8 alphabet_lower_seg_val[] =
{
    0xff, //'a'不显示，会显示为error-
    0x7C, //'b'
    0xff, //'c'不显示，会显示为error-
    0x3f, //'d'
    0xff, //'e'不显示，会显示为error-
    0xff, //'f'不显示，会显示为error-
    0xff, //'g'不显示，会显示为error-
    0xff, //'h'不显示，会显示为error-
    0xff, //'i'不显示，会显示为error-
    0xff, //'j'不显示，会显示为error-
    0xff, //'k'不显示，会显示为error-
    0xff, //'l'不显示，会显示为error-
    0xff, //'m'不显示，会显示为error-
    0x37, //'n'
    0xff, //'o'不显示，会显示为error-
    0xff, //'p'不显示，会显示为error-
    0xff, //'1'不显示，会显示为error-
    0xff, //'r'不显示，会显示为error-
    0xff, //'s'不显示，会显示为error-
    0xff, //'t'不显示，会显示为error-
    0xff, //'u'不显示，会显示为error-
    0xff, //'v'不显示，会显示为error-
    0xff, //'w'不显示，会显示为error-
    0xff, //'x'不显示，会显示为error-
    0x6e, //'y'
    0xff, //'z'不显示，会显示为error-
};

//icon show or hide vals
const uint8 icon_vals[8][2] =
{//[bit, byte]
    { 0x00, 0x05 }, //k1 5a PLAY
    { 0x02, 0x05 }, //k2 5b PAUSE
    { 0x05, 0x01 }, //k3 5c USB
    { 0x00, 0x04 }, //k4 5d SD
    { 0x02, 0x03 }, //k5 5e COL
    { 0x06, 0x02 }, //k6 5f AUX
    { 0x02, 0x06 }, //k7 5g FM
    { 0x06, 0x04 }  //k8 5h DOT
};

//number display vals 4 number and 7 segment
const uint8 num_vals[4][7][2] =
{
    //first layer number 0
    {
        //countent 7segment [bit, byte]
        { 0x00, 0x01 }, //1a
        { 0x00, 0x02 }, //lb
        { 0x03, 0x00 }, //lc
        { 0x04, 0x00 }, //ld
        { 0x00, 0x03 }, //le
        { 0x01, 0x00 }, //lf
        { 0x02, 0x00 }  //lg
    }, //endof number 0
    //first layer number 1
    {
        //countent 7segment [bit, byte]
        { 0x01, 0x02 }, //2a
        { 0x01, 0x03 }, //2b
        { 0x04, 0x01 }, //2c
        { 0x01, 0x05 }, //2d
        { 0x01, 0x04 }, //2e
        { 0x02, 0x01 }, //2f
        { 0x03, 0x01 }  //2g
    },//end of number 1
    //first layer number 2
    {
        //countent 7segment [bit, byte]
        { 0x04, 0x03 }, //3a
        { 0x02, 0x04 }, //3b
        { 0x03, 0x04 }, //3c
        { 0x05, 0x00 }, //3d
        { 0x05, 0x02 }, //3e
        { 0x03, 0x02 }, //3f
        { 0x04, 0x02 }  //3g
    },//end of number 2
    //first layer number 3
    {
        //countent 7segment [bit, byte]
        { 0x06, 0x05 }, //4a
        { 0x05, 0x06 }, //4b
        { 0x04, 0x05 }, //4c
        { 0x05, 0x03 }, //4d
        { 0x03, 0x05 }, //4e
        { 0x05, 0x04 }, //4f
        { 0x04, 0x06 }  //4g
    }//end of number 3
};

/******************************************************************************
* \par  Description: 根据输入，显示或隐藏段码屏上相应的段
*     addr: segment address
*     content: show content;0xff表示为ICON
*     type: signature it is show or hide，1-show，0-hide
* \param[out]  none
* \return      none
*   <author>    <time>
*    Wekan   2015-3-26pm
*******************************************************************************/

int32 led_display_inner(uint8 num_idx, uint8 c, bool display)
{
#if (SUPPORT_LED_DISPLAY == 1)
    uint8 dis_val;
    uint8 tab_index, i;

#ifdef NUMBER1_DISP_1
    if ((display == TRUE) && (num_idx == NUMBER1))
    {
        if ((c != 1) && (c != '1'))
        {
            display = FALSE;
        }
    }
#endif

    //get number display address
    if (display == FALSE)
    {
        //hide
        dis_val = 0;
    }
    else
    {
        //show
        if (c <= 9)
        {
            dis_val = number_seg_val[c];
        }
        else if ((c >= '0') && (c <= '9'))
        {
            dis_val = number_seg_val[c - '0'];
        }
        else if ((c >= 'A') && (c <= 'Z'))
        {
            dis_val = alphabet_upper_seg_val[c - 'A'];
            if (dis_val == 0xff)
            {
                dis_val = alphabet_lower_seg_val[c - 'A'];
                if (dis_val == 0xff)
                {
                    dis_val = error_seg_val;
                }
            }
        }
        else if ((c >= 'a') && (c <= 'z'))
        {
            dis_val = alphabet_lower_seg_val[c - 'a'];
            if (dis_val == 0xff)
            {
                dis_val = alphabet_upper_seg_val[c - 'a'];
                if (dis_val == 0xff)
                {
                    dis_val = error_seg_val;
                }
            }
        }
        else if (c == '-')
        {
            dis_val = dash_line_seg_val;
        }
        else
        {
            dis_val = error_seg_val;
        }
    }

    //get number display value
    for (i = 0; i < 7; i++)
    {
#ifdef NUMBER1_DISP_1
        if (num_idx == NUMBER1)
        {
            if ((i != 1) && (i != 2)) //1b & 1c
            {
                continue;
            }
        }
#endif
        //byte numbers
        tab_index = num_vals[num_idx][i][1];

        if ((dis_val & (1 << i)) != 0)
        {
            //show
            cur_disval[tab_index] |= (1<<num_vals[num_idx][i][0]);
        }
        else
        {
            //hide
            cur_disval[tab_index] &= (~(1<<num_vals[num_idx][i][0]));
        }
    }

    if (led_light_flag == TRUE)
    {
        hal_led_hardware_display((uint32*) cur_disval);
    }

    return 0;
#elif (SUPPORT_LED_DISPLAY == 2)
    bool need_display = FALSE;

    if (display == TRUE)
    {
        digits[num_idx] = c;
        if (digits[num_idx] != bak_digits[num_idx])
        {
            need_display = TRUE;
        }
        bak_digits[num_idx] = digits[num_idx];
    }
    else
    {
        digits[num_idx] = ' ';
        if (digits[num_idx] != bak_digits[num_idx])
        {
            need_display = TRUE;
        }
        bak_digits[num_idx] = digits[num_idx];
    }
    if (need_display == TRUE)
    {
        sim_led_hardware_display();
    }
    return 0;
#endif
}

int32 led_display_icon_inner(uint8 icon_idx, bool display)
{
#if (SUPPORT_LED_DISPLAY == 1)
    uint8 tab_index;

    //get table values index --- the true table byte idnex
    tab_index = icon_vals[icon_idx][1];

    if (display == TRUE)
    {
        //display or
        cur_disval[tab_index] |= (1<<icon_vals[icon_idx][0]);
    }
    else
    {
        //hide and
        cur_disval[tab_index] &= (~(1<<icon_vals[icon_idx][0]));
    }

    if (led_light_flag == TRUE)
    {
        hal_led_hardware_display((uint32*) cur_disval);
    }

    return 0;
#elif (SUPPORT_LED_DISPLAY == 2)
    bool need_display = FALSE;

    if (display == TRUE)
    {
        icon_map |= (1<<icon_idx);
        if (icon_map != bak_icon_map)
        {
            need_display = TRUE;
        }
        bak_icon_map = icon_map;
    }
    else
    {
        icon_map &= ~(1<<icon_idx);
        if (icon_map != bak_icon_map)
        {
            need_display = TRUE;
        }
        bak_icon_map = icon_map;
    }
    if (need_display == TRUE)
    {
        sim_led_hardware_display();
    }
    return 0;
#endif
}

#if (SUPPORT_LED_DISPLAY == 2)
uint8 icon_map;
uint8 digits[4]={' ',' ',' ',' '};
uint8 bak_icon_map;
uint8 bak_digits[4]={' ',' ',' ',' '};

uint8 get_display_digit(uint8 ch)
{
    if (ch <= 9)
    {
        return '0'+ch;
    }
    if (ch >= '0' && ch <= '9')
    {
        return ch;
    }
    if (ch >= 'A' && ch <= 'Z')
    {
        if (alphabet_upper_seg_val[ch-'A'] != 0xff)
        {
            return ch;
        }
        else if (alphabet_lower_seg_val[ch-'A'] != 0xff)
        {
            return ch + 0x20;
        }
        else
        {
            return '-';
        }
    }
    if (ch >= 'a' && ch <= 'z')
    {
        if (alphabet_lower_seg_val[ch-'a'] != 0xff)
        {
            return ch;
        }
        else if (alphabet_upper_seg_val[ch-'a'] != 0xff)
        {
            return ch - 0x20;
        }
        else
        {
            return '-';
        }
    }
    if (ch == ' ') //模拟LED数码管用空格来清除
    {
        return ch;
    }
    if (ch == '-')
    {
        return ch;
    }
    return '-';
}

void sim_led_hardware_display(void)
{
    if (led_light_flag == FALSE)
    {
        return;
    }
    libc_printf("\n*****************************\n");
    libc_printf("*");
    if ((icon_map&0x01) != 0)
    {
        libc_printf(" PLAY  ");
    }
    else
    {
        libc_printf("       ");
    }

    if ((icon_map&0x04) != 0)
    {
        libc_printf("USB    ");
    }
    else
    {
        libc_printf("       ");
    }

    libc_printf("%c",get_display_digit(digits[0]));
    libc_printf("%c",get_display_digit(digits[1]));

    if ((icon_map&0x10) != 0)
    {
        libc_printf(":");
    }
    else
    {
        libc_printf(" ");
    }

    libc_printf("%c",get_display_digit(digits[2]));

    if ((icon_map&0x80) != 0)
    {
        libc_printf(".");
    }
    else
    {
        libc_printf(" ");
    }

    libc_printf("%c",get_display_digit(digits[3]));

    if ((icon_map&0x20) != 0)
    {
        libc_printf("  AUX  ");
    }
    else
    {
        libc_printf("       ");
    }
    libc_printf("*\n");

    libc_printf("*");

    if ((icon_map&0x02) != 0)
    {
        libc_printf(" PAUS  ");
    }
    else
    {
        libc_printf("       ");
    }

    if ((icon_map&0x08) != 0)
    {
        libc_printf("SD     ");
    }
    else
    {
        libc_printf("       ");
    }
    libc_printf("      ");

    if ((icon_map&0x40) != 0)
    {
        libc_printf("   FM  ");
    }
    else
    {
        libc_printf("       ");
    }

    libc_printf("*\n");
    libc_printf("*****************************\n");
}
#endif
