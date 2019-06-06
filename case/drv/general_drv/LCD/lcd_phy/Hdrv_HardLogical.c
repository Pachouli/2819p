/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LCD点阵屏物理驱动实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "lcd_phy_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

#define LCD_DMA_CHAN DMA_CHAN_DMA4

/*
 * x : 0 ~ 127
 * y : 0
 * y : ~
 * y : 63
 * 数据分为8行（从顶到底），128列（从左到右），每行物理高度为8像素点，
 * 对应一个字节，像素点从底到顶按字节从低Bit到高Bit排序。
 * 允许设置高度小于8的窗口进行填充，但是要求数据最低Bit对应最高像素点。
 * 而刷新时如果窗口跨越不完整行时会按照向上取完整行进行刷新。
 */
uint8 __section__(".bss.DisplayBuffer") DisplayBuffer[DisplayBufLen];

region_t region;

volatile uint8 lcd_display_screen_state; //0表示初始状态，1表示正在发送数据
volatile uint8 lcd_display_screen_line;  //表示当前正在发送第几行
volatile uint8 lcd_display_screen_end_line; //刷新到那一行结束，该值为index+1

static void Hdrv_UpdateScreen_auto_load(void)
{
    dma_chan_setting_t dma_sett;
    uint8 tmp[1];

    //等待SPI1传输结束
    spi1_trans_over(NULL, 1);

    if (lcd_display_screen_state == 1)
    {
        lcd_display_screen_line++;
        if (lcd_display_screen_line == lcd_display_screen_end_line)
        {
            return;
        }
    }
    lcd_display_screen_state = 1;

    hal_gpio_write_gpio(25, 0); //RS,0-write cmd,1-write data
    tmp[0] = 0x40;
    spi1_write_data(NULL, tmp, 1, 0);
    tmp[0] = 0xB0+(7-lcd_display_screen_line);
    spi1_write_data(NULL, tmp, 1, 0);//页坐标
    tmp[0] = 0x10;
    spi1_write_data(NULL, tmp, 1, 0);//横坐标高4位
    tmp[0] = 0x00;
    spi1_write_data(NULL, tmp, 1, 0);//横坐标低4位

    hal_gpio_write_gpio(25, 1); //RS,0-write cmd,1-write data

    sys_reset_dma_chan (LCD_DMA_CHAN);

    libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
    dma_sett.src_mem_addr = (uint8 *) (DisplayBuffer+Display_Length*(7-lcd_display_screen_line));
    dma_sett.frame_len = Display_Length;
    dma_sett.irq_cbk = Hdrv_UpdateScreen_auto_load;
    dma_sett.src_type = DMA_SRC_MEM;
    dma_sett.dst_type = DMA_DST_SPI1_TX;
    dma_sett.tc_ie = TRUE;
    dma_sett.start = TRUE;
    sys_set_dma_chan(LCD_DMA_CHAN, &dma_sett);

    spi1_write_data(NULL, NULL, 0, 2);
}

void Hdrv_UpdateScreen(region_t *updateregion)
{
    uint8 lines;

    Hdrv_spi1_switch_cs(CS_LCD_MODULE);

    spi1_set_cs(NULL, 0);

    lcd_display_screen_state = 0;
    if (updateregion == NULL)
    {
        lcd_display_screen_line = 0;
        lines = 8;
    }
    else
    {
        uint8 remain_height;
        lines = 0;
        lcd_display_screen_line = updateregion->y/8;
        remain_height = updateregion->height;
        if ((updateregion->y%8) != 0)
        {
            uint8 line_height;
            lines += 1;
            line_height = 8 - (updateregion->y%8);
            if (line_height > remain_height)
            {
                line_height = remain_height;
            }
            remain_height -= line_height;
        }
        lines += remain_height/8;
        remain_height %= 8;
        if (remain_height > 0)
        {
            lines += 1;
        }
    }
    lcd_display_screen_end_line = lcd_display_screen_line + lines;

    //libc_printf("display start:%d\n", sys_get_ab_timer_us());
    Hdrv_UpdateScreen_auto_load();
    //libc_printf("display :%d\n", sys_get_ab_timer_us());
    while (1)
    {
        if (lcd_display_screen_line == lcd_display_screen_end_line)
        {
            break;
        }
    }
    //libc_printf("display over:%d\n", sys_get_ab_timer_us());

    spi1_set_cs(NULL, 1);

	//默认切换到spinor
    Hdrv_spi1_switch_cs(CS_SPINOR);
}

void Hdrv_SetWindow(region_t *winreg)
{
    //设置工作窗口，用来填充像素点和刷屏
    libc_memcpy(&region, winreg, sizeof(region_t));
}

const uint8 low_mask[8]  = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
const uint8 high_mask[8] = {0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};
void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len)
{
    uint8 start_y, start_x, width;
    uint8 start_line;
    uint8 full_lines = 0;
    uint8 start_half_line_pixels = 0;
    uint8 end_half_line_pixels = 0;
    uint8 i,j;
    uint8* cur_pbuf = pbuf;
    uint8* DispBufLineStart;
    uint16 remain_len = len;
    uint16 cur_len;

    start_y = region.y;
    start_x = region.x;
    width = region.width;
    start_line = start_y/8;
    if ((start_y%8) != 0)
    {
        start_half_line_pixels = 8 - (start_y%8);
        if (start_half_line_pixels > region.height)
        {
            start_half_line_pixels = region.height;
        }
    }
    full_lines = (region.height - start_half_line_pixels)/8;
    end_half_line_pixels = (region.height - start_half_line_pixels)%8;

    //填充前半部分
    if (start_half_line_pixels > 0)
    {
        uint8 *t_lcd_cache;
        uint8 cur_low_mask = low_mask[start_half_line_pixels-1];
        uint8 temp_pixel_val;
        
        DispBufLineStart = DisplayBuffer+(7-start_line)*Display_Length;
        t_lcd_cache = DispBufLineStart + start_x;
        cur_len = width;
        if (cur_len > remain_len)
        {
        	cur_len = remain_len;
        }
        for (j = 0; j < cur_len; j++)
        {
            temp_pixel_val = cur_pbuf[j] >> (8-start_half_line_pixels);
            t_lcd_cache[j] &= ~cur_low_mask;
            t_lcd_cache[j] |= (temp_pixel_val&cur_low_mask);
        }

        start_line++;

        //数据往前挪，会覆盖前半部饭
        if ((full_lines > 0) || (end_half_line_pixels > 0))
        {
            uint8 shift_1half = 8 - start_half_line_pixels;
            uint8 shift_2half = start_half_line_pixels;
            for (i = 0; i < full_lines; i++)
            {
                if (remain_len <= 0)
                {
                	break;
                }

                cur_len = width;
		        if (cur_len > remain_len)
		        {
		        	cur_len = remain_len;
		        }
                for (j = 0; j < cur_len; j++)
                {
                    cur_pbuf[i*width + j] = cur_pbuf[i*width + j]<<shift_2half;
                    cur_pbuf[i*width + j] |= cur_pbuf[(i+1)*width + j]>>shift_1half;
                }
                remain_len -= cur_len;
            }
            if ((remain_len > 0) && (end_half_line_pixels > 0))
            {
                cur_len = width;
		        if (cur_len > remain_len)
		        {
		        	cur_len = remain_len;
		        }
                for (j = 0; j < cur_len; j++)
                {
                    cur_pbuf[full_lines*width + j] = cur_pbuf[full_lines*width + j]<<shift_2half;
                }
            }

            remain_len = len;
        }
    }

    //填充中间整行
    if (full_lines > 0)
    {
        for (i = 0; i < full_lines; i++)
        {
            if (remain_len <= 0)
            {
            	break;
            }

            cur_len = width;
	        if (cur_len > remain_len)
	        {
	        	cur_len = remain_len;
	        }
            DispBufLineStart = DisplayBuffer+(7-start_line)*Display_Length;
            libc_memcpy(DispBufLineStart+start_x, cur_pbuf, cur_len);
            cur_pbuf += cur_len;
            remain_len -= cur_len;
            start_line++;
        }
    }

    //填充后半部分
    if ((remain_len > 0) && (end_half_line_pixels > 0))
    {
        uint8 *t_lcd_cache;
        uint8 cur_high_mask = high_mask[end_half_line_pixels-1];

        DispBufLineStart = DisplayBuffer+(7-start_line)*Display_Length;
        t_lcd_cache = DispBufLineStart + start_x;
        cur_len = width;
        if (cur_len > remain_len)
        {
        	cur_len = remain_len;
        }
        for (j = 0; j < cur_len; j++)
        {
            t_lcd_cache[j] &= ~cur_high_mask;
            t_lcd_cache[j] |= (cur_pbuf[j]&cur_high_mask);
        }
    }
}

void Hdrv_StandbyScreen(bool bStandby)
{
    Hdrv_spi1_switch_cs(CS_LCD_MODULE);
    if (bStandby == FALSE)
    {
        //light off
        Hdrv_lcd_switch_light(FALSE);

        //enter standby
        Hdrv_lcd_write_cmd(0xac);
        Hdrv_lcd_write_cmd(0xae);
        Hdrv_lcd_write_cmd(0xa5);
    }
    else
    {
        //exit standby
        Hdrv_lcd_write_cmd(0xa4);
        Hdrv_lcd_write_cmd(0xaf);
        Hdrv_lcd_write_cmd(0xad);

        //light on
        Hdrv_lcd_switch_light(TRUE);
    }
	//默认切换到spinor
    Hdrv_spi1_switch_cs(CS_SPINOR);
}
const uint8 lcd_contrast_tbl[16] =
{
    14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44
};
void Hdrv_SetContrast(uint8 ContrastValue)
{
    Hdrv_spi1_switch_cs(CS_LCD_MODULE);
    Hdrv_lcd_write_cmd(0x81);
    if (ContrastValue > 15)
    {
        ContrastValue = 15;
    }
    Hdrv_lcd_write_cmd(lcd_contrast_tbl[ContrastValue]);
	//默认切换到spinor
    Hdrv_spi1_switch_cs(CS_SPINOR);
}


/******************************************* 测试接口 ***********************************************/

void get_one_line_data(uint8 *p_buf, uint8 index, uint8 half)
{
    uint8 char_buf[32];
    uint8 i,j,k;
    void *lib_fd;

    lib_fd = Sdrv_open_lib("UNICODE.BIN");

    Sdrv_seek_lib(lib_fd, 0, 33*('1'+index));
    Sdrv_read_lib(lib_fd, char_buf, 32);

    Sdrv_close_lib(lib_fd);

    //缓冲到Cache
    for(k=0;k<16;k++)
    {
        if (half == 0)
        {
            libc_memcpy(p_buf+8*k, char_buf, 8);
        }
        else
        {
            libc_memcpy(p_buf+8*k, char_buf+8, 8);
        }
    }
}

void lcd_module_test_inner(void)
{
    uint8 temp[Display_Length];
    uint8 i;
    region_t temp_region;

    for (i = 0; i < Display_Length; i++)
    {
        DisplayBuffer[5*Display_Length + i] = 0xe0;
    }
    for (i = 0; i < Display_Length; i++)
    {
        DisplayBuffer[3*Display_Length + i] = 0x1f;
    }
    
    temp_region.y = 8*0;
    temp_region.x = 0;
    temp_region.width = 128;
    temp_region.height = 8;
    Hdrv_SetWindow(&temp_region);
    //构造数字字符行
    get_one_line_data(temp, 0, 0);
    Hdrv_BuffDataTrans(temp, Display_Length);
    region.y += 8;
    get_one_line_data(temp, 0, 1);
    Hdrv_BuffDataTrans(temp, Display_Length);

    temp_region.y = 8*2 + 3;
    Hdrv_SetWindow(&temp_region);
    //构造数字字符行
    get_one_line_data(temp, 1, 0);
    Hdrv_BuffDataTrans(temp, Display_Length);
    region.y += 8;
    get_one_line_data(temp, 1, 1);
    Hdrv_BuffDataTrans(temp, Display_Length);

    temp_region.y = 8*5;
    Hdrv_SetWindow(&temp_region);
    //构造数字字符行
    get_one_line_data(temp, 2, 0);
    Hdrv_BuffDataTrans(temp, Display_Length);
    region.y += 8;
    get_one_line_data(temp, 2, 1);
    Hdrv_BuffDataTrans(temp, Display_Length);

    temp_region.y = 8*7;
    Hdrv_SetWindow(&temp_region);
    //构造数字字符行
    get_one_line_data(temp, 3, 0);
    Hdrv_BuffDataTrans(temp, Display_Length);
}
#endif
