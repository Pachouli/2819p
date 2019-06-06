/*
 ********************************************************************************
 *                       ACTOS
 *                  ui dirver show picture
 *
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : sdrv_init.c
 * By     : Mars
 * Version: 1> v1.00    first version     2008/07/30  14:40
 * Version: 1> v1.01     reagan            2010-11-10 11:00
 * Description: Encapsulating LCD file operation interface
 ********************************************************************************
 */
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

void * Sdrv_open_lib(const uint8*filename)
{
    void *fd;

#if(SUPPORT_FONT_STORAGE_TYPE == 1)
    fd = sys_sd_fopen(filename);
#else
    fd = lib_fopen(filename);
#endif

    return fd;
}

void Sdrv_read_lib(void *fp, void *buffer, uint32 len)
{
#if(SUPPORT_FONT_STORAGE_TYPE == 1)
    sys_sd_fread(fp, buffer, len);
#else
    lib_fread(fp, buffer, len);
#endif

}
void Sdrv_seek_lib(void * fp, uint8 whence, int32 offset)
{
#if(SUPPORT_FONT_STORAGE_TYPE == 1)
    sys_sd_fseek(fp, SEEK_SET, offset);
#else
    lib_fseek(fp, SEEK_SET, offset);
#endif
}

void Sdrv_close_lib(void * fp)
{
#if(SUPPORT_FONT_STORAGE_TYPE == 1)
    sys_sd_fclose(fp);
#else
    lib_fclose(fp);
#endif
}

#endif
