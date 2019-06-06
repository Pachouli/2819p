/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏字符串滚屏实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint8 scroll_str_buffer[104];
uint8 *str_pt;
uint8 *strhead_pt;
region_t scroll_region;
uint8 scroll_cnt;

uint8 gInvertScrollRegion = 0;  // 是否反白滚动区域
uint8 g_is_str_end = 0;  // 中间变量，在Sdrv_CharOutRect()中设置，用于判断字符串是否结束
uint8 g_last_char_width;  // 中间变量，在Sdrv_CharOutRect()中设置，用于保存最近字符宽度
uint8 g_last_scan_len;  // 中间变量，在Sdrv_CharOutRect()中设置，用于保存最近显示字符字节数



bool Sdrv_ScrollString(uint8 *str, region_t *r_scroll)
{
    uint8 result;
    region_t kregion;//余下部分区域
    uint8 language = UNICODELANGUAGE; //TODO
    uint8 is_unicode = 0;
    uint8 str_end_flag = 0;
    bool retval = TRUE;

    libc_memcpy(scroll_str_buffer, str, 104);

    scroll_str_buffer[102] = 0;  // 保持最后两位为0
    scroll_str_buffer[103] = 0;

    str_pt = strhead_pt = scroll_str_buffer;
    libc_memcpy(&scroll_region, r_scroll, sizeof(region_t));
    scroll_cnt = 0;
    strhead_pt = scroll_str_buffer; //// str;
    Sdrv_SetTextPos(scroll_region.x, scroll_region.y);
    Sdrv_SetStrEndPosition(scroll_region.x + scroll_region.width);

    //对阿拉伯和西伯来语进行转换，然后输出，返回结束字符的指针 by cailizhen
    //// str_pt = Sdrv_PutStringExt(str, -1, (language == UNICODELANGUAGE) || (language == (uint8)(ARABICHASJOIN)), 0x04);

    // by jc0245
    if ((str[0]==0xff) && (str[1]==0xfe))
    {
        is_unicode = 1;
        language = UNICODELANGUAGE;  // ToDo : ARABICHASJOIN
    }
    else
    {  // 内码
        is_unicode = 0;
        language = 0;
    }

    str_pt = Sdrv_PutStringExt(scroll_str_buffer, -1, is_unicode, 0x04);

    Sdrv_SetStrEndPosition(0);


    if(is_unicode)
    {
        //if((0x00 == str_pt[0]) && (0x00 == str_pt[1])
        str_end_flag = str_pt[0] | str_pt[1];
        str_end_flag = !str_end_flag;
    }
    else
    {
        str_end_flag = (str_pt[0] != 0) ? 0 : 1;
    }

    //// if (*str_pt != 0)  // "一"的UNICODE编码为0x4e00，因此不能仅仅这么判断

    if(str_end_flag != 1)  // 串未结束
    {
        Sdrv_GetTextPos(&kregion.x, &kregion.y);
        kregion.width = scroll_region.width + scroll_region.x - kregion.x;
        kregion.height = scroll_region.height;

        //输出最后一个字，传递str_pt，以便对泰语后面的字符叠加到当前字符上 by cailizhen
        result = Sdrv_CharOutRect(str_pt, &kregion, scroll_cnt, language);
        /*if (result != 0)
        {
            return FALSE;
        }
        scroll_cnt = kregion.width;*/


        /*
            result等于0有两种情况，
            只显示一半，肯定要滚屏；已完全显示，可能要滚屏或不滚屏

            result不等于0时，因为未显示，因此需要滚屏
        */
        if(result == 0)
        {
            if(1 == g_is_str_end)  // 字符串已经结束
            {
                if(kregion.width >= g_last_char_width)
                {
                    retval = FALSE;
                    //return FALSE;
                }
                else  // 字符未能完全显示
                {
                    ;
                }
            }
            else  // 字符串未结束
            {
                ;
            }

            scroll_cnt = kregion.width;
        }
        else  // result != 0
        {
            // if (CRregion->width == 0) : scroll_cnt = 0
            // if(colcnt >= CharPointBuffer[32] )  : 这个条件不会成立

            scroll_cnt = 0;
        }

        //return TRUE;
    }
    else
    {
        retval = FALSE;
    }


    if(gInvertScrollRegion)  // add by jc0245
    {
        Sdrv_Invert(&scroll_region);
    }

    //return FALSE;

    if(1 == IsArabicOrHebrew)  // 暂不支持阿拉伯文和希伯来文滚动
    {
        libc_printf("#ToDo : ArabicOrHebrew scroll\r\n");
        retval = FALSE;
    }

    return retval;
}

bool Sdrv_ScrollMultiString(uint32 str_id, region_t *r_scroll)
{
    return FALSE;
}



//==========================================================
/*
 字符串滚动
 */


/*
// 本函数支持非1步进滚动，
// 待完善支持阿拉伯和希伯来语后再使用
void Sdrv_ScrollStringScrolling(uint8 step)
{
    region_t tregion;
    region_t sregion;
    region_t kregion;//余下部分区域
    uint8 Result;
    uint8 language = UNICODELANGUAGE; //TODO
    uint8 dx, is_unicode = 0;


    // 检查参数
    if(0 == step)
    {
        return;
    }

    // 判断字符
    if((scroll_str_buffer[0] == 0xff) && (scroll_str_buffer[1] == 0xfe))
    {
        language = UNICODELANGUAGE;
        is_unicode = 1;
    }
    else
    {
        language = 0;
        is_unicode = 0;
    }

    // 滚动反显开启
    if(gInvertScrollRegion)
    {
        Sdrv_Invert(&scroll_region);
    }


    //余下部分区域
    kregion.x = scroll_region.x + scroll_region.width - step;
    kregion.y = scroll_region.y;
    kregion.width = step;
    kregion.height = scroll_region.height;

    //设定移动的源，目标区域
    sregion.x = scroll_region.x + step;//暂时未考虑为负数的情况
    tregion.x = scroll_region.x;
    tregion.y = sregion.y = scroll_region.y;
    tregion.height = sregion.height = scroll_region.height;
    tregion.width = sregion.width = scroll_region.width - step;
    //向左移step列
    Sdrv_RegionCopy(&sregion, &tregion);

    // 清除step列, by jc0245
    //Sdrv_ClearScreen(&kregion);


    test:
    if ((language == UNICODELANGUAGE) || (language == (uint8)(ARABICHASJOIN)))
    {
        if (*(uint16 *) str_pt == 0x0000)
        {
            str_pt = strhead_pt;
        }
    }
    else if (*str_pt == '\0')
    {
        str_pt = strhead_pt;
    }
    else
    {
        //for QAC
    }

    if(kregion.width == 0)
    {
        goto scroll_end_label;
    }

    //传递str_pt进去，以便对泰语进行特殊处理 by cailizhen
    Result = Sdrv_CharOutRect(str_pt, &kregion, scroll_cnt, language);


    // 计算下一个字符列偏移
    if(g_last_char_width > scroll_cnt)
    {
        dx = g_last_char_width - scroll_cnt;

        if(dx < kregion.width) {  // 要显示的小于区域，下一个字符从头
            scroll_cnt = 0;
        }
        else {
            scroll_cnt += kregion.width;
        }
    }
    else
    {
        scroll_cnt = 0;
    }


    // 当scroll_cnt不为0时，上一次的字符一定没有显示完全! by jc0245

    if (Result != 0) //更换最后一个符号
    {
        str_pt += Result;
        scroll_cnt = 0;
        goto test;
    }
    else  // add by jc0245
    {
        // 这里Result等于0
        //if(g_last_char_width > scroll_cnt)  // 断言: 这个肯定成立，否则Result != 0

        //uint8 dx = g_last_char_width - scroll_cnt;

        if(dx < kregion.width)  // 有部分不能显示了, 需要再显示
        {
            str_pt += g_last_scan_len;  // 下一个显示字符

            kregion.x += dx;
            kregion.width -= dx;

            goto test;
        }
    }

scroll_end_label:

    // 滚动反显开启
    if(gInvertScrollRegion)
    {
        Sdrv_Invert(&scroll_region);
    }


    //TODO
//    if (InvertFlag == TRUE)
//    {
//        InvertRegion(&kregion);
//    }
    //// scroll_cnt += step;

    Hdrv_UpdateScreen(&scroll_region);
}
*/



void Sdrv_ScrollStringScrolling(uint8 step)
{
    region_t tregion;
    region_t sregion;
    region_t kregion;//余下部分区域
    uint8 Result;
    uint8 language = UNICODELANGUAGE; //TODO
    uint8 is_unicode;


    // 检查参数
    if(0 == step)
    {
        return;
    }

    step = 1;  // ToDo : 目前只支持逐列滚动


    // 判断字符
    if((scroll_str_buffer[0] == 0xff) && (scroll_str_buffer[1] == 0xfe))
    {
        language = UNICODELANGUAGE;
        is_unicode = 1;
    }
    else
    {
        language = 0;
        is_unicode = 0;
    }

    // 滚动反显开启
    if(gInvertScrollRegion)
    {
        Sdrv_Invert(&scroll_region);
    }


    //余下部分区域
    kregion.x = scroll_region.x + scroll_region.width - step;
    kregion.y = scroll_region.y;
    kregion.width = step;
    kregion.height = scroll_region.height;

    //设定移动的源，目标区域
    sregion.x = scroll_region.x + step;//暂时未考虑为负数的情况
    tregion.x = scroll_region.x;
    tregion.y = sregion.y = scroll_region.y;
    tregion.height = sregion.height = scroll_region.height;
    tregion.width = sregion.width = scroll_region.width - step;
    //向左移step列
    Sdrv_RegionCopy(&sregion, &tregion);

    // 清除step列, by jc0245
    //Sdrv_ClearScreen(&kregion);

    test:
    if ((language == UNICODELANGUAGE) || (language == (uint8)(ARABICHASJOIN)))
    {
        if (*(uint16 *) str_pt == 0x0000)
        {
            str_pt = strhead_pt;
        }
    }
    else if (*str_pt == '\0')
    {
        str_pt = strhead_pt;
    }
    else
    {
        //for QAC
    }
    //传递str_pt进去，以便对泰语进行特殊处理 by cailizhen
    Result = Sdrv_CharOutRect(str_pt, &kregion, scroll_cnt, language);
    if (Result != 0) //更换最后一个符号
    {
        str_pt += Result;
        scroll_cnt = 0;
        goto test;
    }


    // 滚动反显开启
    if(gInvertScrollRegion)
    {
        Sdrv_Invert(&scroll_region);
    }

    //TODO
//    if (InvertFlag == TRUE)
//    {
//        InvertRegion(&kregion);
//    }
    scroll_cnt += step;

    Hdrv_UpdateScreen(&scroll_region);
}


#endif
