/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������LCD�������ַ�������ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint8 scroll_str_buffer[104];
uint8 *str_pt;
uint8 *strhead_pt;
region_t scroll_region;
uint8 scroll_cnt;

uint8 gInvertScrollRegion = 0;  // �Ƿ񷴰׹�������
uint8 g_is_str_end = 0;  // �м��������Sdrv_CharOutRect()�����ã������ж��ַ����Ƿ����
uint8 g_last_char_width;  // �м��������Sdrv_CharOutRect()�����ã����ڱ�������ַ����
uint8 g_last_scan_len;  // �м��������Sdrv_CharOutRect()�����ã����ڱ��������ʾ�ַ��ֽ���



bool Sdrv_ScrollString(uint8 *str, region_t *r_scroll)
{
    uint8 result;
    region_t kregion;//���²�������
    uint8 language = UNICODELANGUAGE; //TODO
    uint8 is_unicode = 0;
    uint8 str_end_flag = 0;
    bool retval = TRUE;

    libc_memcpy(scroll_str_buffer, str, 104);

    scroll_str_buffer[102] = 0;  // ���������λΪ0
    scroll_str_buffer[103] = 0;

    str_pt = strhead_pt = scroll_str_buffer;
    libc_memcpy(&scroll_region, r_scroll, sizeof(region_t));
    scroll_cnt = 0;
    strhead_pt = scroll_str_buffer; //// str;
    Sdrv_SetTextPos(scroll_region.x, scroll_region.y);
    Sdrv_SetStrEndPosition(scroll_region.x + scroll_region.width);

    //�԰������������������ת����Ȼ����������ؽ����ַ���ָ�� by cailizhen
    //// str_pt = Sdrv_PutStringExt(str, -1, (language == UNICODELANGUAGE) || (language == (uint8)(ARABICHASJOIN)), 0x04);

    // by jc0245
    if ((str[0]==0xff) && (str[1]==0xfe))
    {
        is_unicode = 1;
        language = UNICODELANGUAGE;  // ToDo : ARABICHASJOIN
    }
    else
    {  // ����
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

    //// if (*str_pt != 0)  // "һ"��UNICODE����Ϊ0x4e00����˲��ܽ�����ô�ж�

    if(str_end_flag != 1)  // ��δ����
    {
        Sdrv_GetTextPos(&kregion.x, &kregion.y);
        kregion.width = scroll_region.width + scroll_region.x - kregion.x;
        kregion.height = scroll_region.height;

        //������һ���֣�����str_pt���Ա��̩�������ַ����ӵ���ǰ�ַ��� by cailizhen
        result = Sdrv_CharOutRect(str_pt, &kregion, scroll_cnt, language);
        /*if (result != 0)
        {
            return FALSE;
        }
        scroll_cnt = kregion.width;*/


        /*
            result����0�����������
            ֻ��ʾһ�룬�϶�Ҫ����������ȫ��ʾ������Ҫ�����򲻹���

            result������0ʱ����Ϊδ��ʾ�������Ҫ����
        */
        if(result == 0)
        {
            if(1 == g_is_str_end)  // �ַ����Ѿ�����
            {
                if(kregion.width >= g_last_char_width)
                {
                    retval = FALSE;
                    //return FALSE;
                }
                else  // �ַ�δ����ȫ��ʾ
                {
                    ;
                }
            }
            else  // �ַ���δ����
            {
                ;
            }

            scroll_cnt = kregion.width;
        }
        else  // result != 0
        {
            // if (CRregion->width == 0) : scroll_cnt = 0
            // if(colcnt >= CharPointBuffer[32] )  : ��������������

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

    if(1 == IsArabicOrHebrew)  // �ݲ�֧�ְ������ĺ�ϣ�����Ĺ���
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
 �ַ�������
 */


/*
// ������֧�ַ�1����������
// ������֧�ְ�������ϣ���������ʹ��
void Sdrv_ScrollStringScrolling(uint8 step)
{
    region_t tregion;
    region_t sregion;
    region_t kregion;//���²�������
    uint8 Result;
    uint8 language = UNICODELANGUAGE; //TODO
    uint8 dx, is_unicode = 0;


    // ������
    if(0 == step)
    {
        return;
    }

    // �ж��ַ�
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

    // �������Կ���
    if(gInvertScrollRegion)
    {
        Sdrv_Invert(&scroll_region);
    }


    //���²�������
    kregion.x = scroll_region.x + scroll_region.width - step;
    kregion.y = scroll_region.y;
    kregion.width = step;
    kregion.height = scroll_region.height;

    //�趨�ƶ���Դ��Ŀ������
    sregion.x = scroll_region.x + step;//��ʱδ����Ϊ���������
    tregion.x = scroll_region.x;
    tregion.y = sregion.y = scroll_region.y;
    tregion.height = sregion.height = scroll_region.height;
    tregion.width = sregion.width = scroll_region.width - step;
    //������step��
    Sdrv_RegionCopy(&sregion, &tregion);

    // ���step��, by jc0245
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

    //����str_pt��ȥ���Ա��̩��������⴦�� by cailizhen
    Result = Sdrv_CharOutRect(str_pt, &kregion, scroll_cnt, language);


    // ������һ���ַ���ƫ��
    if(g_last_char_width > scroll_cnt)
    {
        dx = g_last_char_width - scroll_cnt;

        if(dx < kregion.width) {  // Ҫ��ʾ��С��������һ���ַ���ͷ
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


    // ��scroll_cnt��Ϊ0ʱ����һ�ε��ַ�һ��û����ʾ��ȫ! by jc0245

    if (Result != 0) //�������һ������
    {
        str_pt += Result;
        scroll_cnt = 0;
        goto test;
    }
    else  // add by jc0245
    {
        // ����Result����0
        //if(g_last_char_width > scroll_cnt)  // ����: ����϶�����������Result != 0

        //uint8 dx = g_last_char_width - scroll_cnt;

        if(dx < kregion.width)  // �в��ֲ�����ʾ��, ��Ҫ����ʾ
        {
            str_pt += g_last_scan_len;  // ��һ����ʾ�ַ�

            kregion.x += dx;
            kregion.width -= dx;

            goto test;
        }
    }

scroll_end_label:

    // �������Կ���
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
    region_t kregion;//���²�������
    uint8 Result;
    uint8 language = UNICODELANGUAGE; //TODO
    uint8 is_unicode;


    // ������
    if(0 == step)
    {
        return;
    }

    step = 1;  // ToDo : Ŀǰֻ֧�����й���


    // �ж��ַ�
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

    // �������Կ���
    if(gInvertScrollRegion)
    {
        Sdrv_Invert(&scroll_region);
    }


    //���²�������
    kregion.x = scroll_region.x + scroll_region.width - step;
    kregion.y = scroll_region.y;
    kregion.width = step;
    kregion.height = scroll_region.height;

    //�趨�ƶ���Դ��Ŀ������
    sregion.x = scroll_region.x + step;//��ʱδ����Ϊ���������
    tregion.x = scroll_region.x;
    tregion.y = sregion.y = scroll_region.y;
    tregion.height = sregion.height = scroll_region.height;
    tregion.width = sregion.width = scroll_region.width - step;
    //������step��
    Sdrv_RegionCopy(&sregion, &tregion);

    // ���step��, by jc0245
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
    //����str_pt��ȥ���Ա��̩��������⴦�� by cailizhen
    Result = Sdrv_CharOutRect(str_pt, &kregion, scroll_cnt, language);
    if (Result != 0) //�������һ������
    {
        str_pt += Result;
        scroll_cnt = 0;
        goto test;
    }


    // �������Կ���
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
