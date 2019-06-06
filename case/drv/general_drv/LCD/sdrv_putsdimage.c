/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Sdrv_putsdimage.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

uint32 pic_entry_offset;
uint32 string_entry_offset;
void *res_lib_fd;
uint8 temp_DisplayBuffer[DisplayBufLen];

void ui30_file_parse(void)
{
	res_head_t res_head;
    	
	//������Դ�ļ�
	if(res_lib_fd != NULL)
	{
		/* ������ʵҲ���Բ�close��
		     ��Ϊres_lib_fd�ǿգ������Ѿ������ˣ�
		     ֻҪ���ټ��ؼ��ɣ����������޸�
		*/
		libc_printf("#res_lib_fd != NULL\r\n");
		Sdrv_close_lib(res_lib_fd);
		res_lib_fd = NULL;
	}
	//����ͼƬ���ַ�����ʼλ��
	res_lib_fd = Sdrv_open_lib("ui30.res");
	if (res_lib_fd == NULL)
	{
		libc_printf_error("can't find ui30.res!\n");
		while (1);
	}
	
	Sdrv_read_lib(res_lib_fd, &res_head, sizeof(res_head_t));
	
	/*�ж��ļ���ʽ�Ƿ�Ϸ� */
	if ((res_head.magic[0] != 'R') || (res_head.magic[1] != 'U') || (res_head.magic[2] != '2') || (res_head.magic[3]
        != 0x19))
    {
    	libc_printf_error("wrong format ui30.res!\n");
		while (1);
    }
    
    Sdrv_seek_lib(res_lib_fd, SEEK_SET, RESHEADITEM);
    Sdrv_read_lib(res_lib_fd, &pic_entry_offset, 4);
    
    Sdrv_seek_lib(res_lib_fd, SEEK_SET, RESHEADITEM*2);
    Sdrv_read_lib(res_lib_fd, &string_entry_offset, 4);
    Sdrv_seek_lib(res_lib_fd, SEEK_SET, string_entry_offset + RESHEADITEM*0); //TODO 0�����滻Ϊlangid
    Sdrv_read_lib(res_lib_fd, &string_entry_offset, 4);
}

void Sdrv_PutSDImage(uint8 x, uint8 y, uint32 img_id)
{
    region_t r_pic;
    res_infor_t res_infor;
    uint8 true_width;
    uint8 true_height;
    uint16 cnt;

    //���������Ļ�����˳�
    if ((x >= Display_Length) || (y >= Display_Height))
    {
        return;
    }

    if (pic_entry_offset == 0)
    {
    	ui30_file_parse();
    }

    Sdrv_seek_lib(res_lib_fd, SEEK_SET, pic_entry_offset + (uint32) (img_id * RESHEADITEM)); //�ҵ���N��picture������Ŀ¼
    Sdrv_read_lib(res_lib_fd, &res_infor, sizeof(res_infor_t));
    true_width = res_infor.wWidth[1]*256 + res_infor.wWidth[0];
    true_height = res_infor.wHeight[1]*256 + res_infor.wHeight[0];
    r_pic.x = x;
    r_pic.y = y;
    r_pic.width = true_width;
    r_pic.height = true_height;

    if ((r_pic.height + r_pic.y) > Display_Height)
    {
        r_pic.height = Display_Height - r_pic.y;
    }
    if ((r_pic.width + r_pic.x) > Display_Length)
    {
        r_pic.width = Display_Length - r_pic.x;
    }

    Sdrv_seek_lib(res_lib_fd, SEEK_SET, res_infor.dwOffset);
    cnt = (res_infor.dwLength > DisplayBufLen)? DisplayBufLen : res_infor.dwLength;
    Sdrv_read_lib(res_lib_fd, temp_DisplayBuffer, cnt);
    RegionMapping(&r_pic, -1);
    Hdrv_SetWindow(&r_pic);
    Hdrv_BuffDataTrans(temp_DisplayBuffer, cnt);
    RegionMapping(&r_pic, -1);
}

#endif
