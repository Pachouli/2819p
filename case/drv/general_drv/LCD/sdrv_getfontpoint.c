/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_getfontpoint.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

typedef struct
{
    uint16 free_start;
    uint16 free_count;
} unicode_free_t;

const unicode_free_t s_unicode_free_blocks[] =
{
    {0x0500, 0x90},
    {0x0700, 0x700},
    {0x0e80, 0x1180},
    {0x2700, 0x900},
    {0x3400, 0x19c0},
    {0xa000, 0xc00},
    {0xd800, 0x800},
    {0xfb00, 0x330}
};

/*
*******************************************************************************

** FUNCTION:    GetCharPoint
**
** PURPOSE:     Get Char Point Info by dma2
**
** PARAMETERS:
**  input
**      code=区位码
**      picbuf=输出数据的存放地址
**  output
**      none
** DESCRIPTION:
****************************************************************************
*/
void Sdrv_GetFontPoint(uint16 ucode, uint8 *font_point_buf)
{
    if (((uint16)ucode<0x0080)&&(AsciiSelect==0))
    {
        Sdrv_seek_lib(FontAscLib_lib_fd, SEEK_SET, (uint32) ucode*8);
        Sdrv_read_lib(FontAscLib_lib_fd, font_point_buf, 8);
    }
    else
    {
        bool is_free = FALSE;
        uint16 free_offset;
        uint8 i;

        free_offset = 0;
        for (i = 0; i < (sizeof(s_unicode_free_blocks)/sizeof(unicode_free_t)); i++)
        {
            if ((ucode >= s_unicode_free_blocks[i].free_start)
                && (ucode < (s_unicode_free_blocks[i].free_start + s_unicode_free_blocks[i].free_count)))
            {
                is_free = TRUE;
                break;
            }
            else if (ucode < s_unicode_free_blocks[i].free_start)
            {
                break;
            }
            else
            {
                free_offset += s_unicode_free_blocks[i].free_count;
            }
        }

        if (is_free == TRUE)
        {
            ucode = '?';
            free_offset = 0;
        }

        ucode -= free_offset;
        Sdrv_seek_lib(FontLib_lib_fd, SEEK_SET, (uint32) ucode*33);
        Sdrv_read_lib(FontLib_lib_fd, font_point_buf, 33);
    }
}

#endif
