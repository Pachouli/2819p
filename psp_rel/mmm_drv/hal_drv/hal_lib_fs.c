/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——外置SPINOR文件系统 相关接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"

lib_file_t *lib_handle_array;
lib_dir_info_t *lib_dir;

//读取外部norflash数据
int32 hal_lib_read_data_inner(uint32 byte_addr, uint8 *buf, uint32 byte_num)
{
    hal_spi1nor_read(byte_addr, buf, byte_num);
    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    search resource file from nor
 * \param[in]    name      file name
 * \param[out]   res_dir_info_t
 * \return      none
 * \retval           none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
lib_dir_info_t * lib_dir_search(uint8 *name)
{
    uint32 map_index = 0;
    uint32 map_index_address = 0;
    uint8 buf[32];
    uint8 nmae_len = 0;
    while (1)
    {
        nmae_len++;
        if (name[nmae_len] == '\0')
        {
            break;
        }
    }

    while (map_index < LIB_MAP_SIZE)
    {
        map_index_address = map_index * 32;
        hal_spi1nor_read(map_index_address, buf, 32);

        lib_dir = (lib_dir_info_t *) buf;
        if ((0 == libc_memcmp(name, lib_dir->name, nmae_len)) || (0 == lib_dir->length))
        {
            break;
        }
        map_index++;
    }

    if (map_index == LIB_MAP_SIZE)
    {
        lib_dir->length = 0;
    }
    return lib_dir;
}

bool handle_valid(lib_file_t *fp)
{
    uint8 i;
    for (i = 0; i < MAX_LIB_HEANDLE_NUM; i++)
    {
        if (&(lib_handle_array[i]) == fp)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//filename:8 + 3
lib_file_t *hal_lib_fopen_inner(const char *filename)
{
    uint8 i;
    lib_dir_info_t *t_lib_dir;

    if ((filename[0] == 0) || (filename[0] == 0x5e))
    {
        return NULL;
    }

    t_lib_dir = (lib_dir_info_t *) lib_dir_search((uint8*) filename);
    if (0 == t_lib_dir->length)
    {
        return NULL;
    }
    for (i = 0; i < MAX_LIB_HEANDLE_NUM; i++)
    {
        if (lib_handle_array[i].isvalid == 0)
        {
            lib_handle_array[i].isvalid = TRUE;
            break;
        }
    }
    if (i == MAX_LIB_HEANDLE_NUM)
    {
        return NULL;
    }
    lib_handle_array[i].startaddr = t_lib_dir->offset;
    lib_handle_array[i].rwpointer = lib_handle_array[i].startaddr;
    lib_handle_array[i].endaddr = lib_handle_array[i].startaddr + t_lib_dir->length - 0x1;
    return &(lib_handle_array[i]);
}

void hal_lib_fs_init_inner(void)
{
    uint8 i;

    lib_handle_array = (lib_file_t *) sys_malloc(sizeof(lib_file_t) * MAX_LIB_HEANDLE_NUM);
    if (lib_handle_array == NULL)
    {
        libc_printf_error("lib_handle_array\n");
        while (1)
            ;
    }
    for (i = 0; i < MAX_LIB_HEANDLE_NUM; i++)
    {
        libc_memset(&lib_handle_array[i], 0, sizeof(lib_file_t));
        lib_handle_array[i].isvalid = 0;
    }
}

int hal_lib_fclose_inner(lib_file_t *fp)
{
    if (handle_valid(fp) == 0)
    {
        return -1;
    }

    fp->isvalid = 0;
    return 0;
}

int hal_lib_fseek_inner(lib_file_t *fp, uint8 whence, int32 offset)
{
    if (whence == SEEK_SET)
    {
        if (((uint32) offset) > (fp->endaddr - fp->startaddr))
        {
            return -1;
        }
        else
        {
            fp->rwpointer = fp->startaddr + ((uint32) offset);
        }
    }
    else if (whence == SEEK_CUR)
    {
        if (offset >= 0)
        {
            if (((uint32) offset) > (fp->endaddr - fp->rwpointer))
            {
                return -1;
            }
        }
        else if ((((int32) fp->rwpointer) + offset) < ((int32) fp->startaddr))
        {
            return -1;
        }
        else
        {

        }
        fp->rwpointer = (uint32) (((int32) (fp->rwpointer)) + offset);

    }
    else if (whence == SEEK_END)
    {
        if (((uint32) offset) > (fp->endaddr - fp->startaddr))
        {
            return -1;
        }
        else
        {
            fp->rwpointer = fp->endaddr - (uint32) offset;
        }
    }
    else
    {
        return -1;
    }

    return 0;

}

uint32 hal_lib_ftell_inner(lib_file_t *fp)
{
    return (fp->rwpointer - fp->startaddr);
}

//读到结尾后不能再往前seek，要直接从头开始seek。
int32 hal_lib_fread_inner(lib_file_t *fp, void *buffer, uint32 len)
{
    if ((handle_valid(fp) == 0) || (fp->rwpointer > fp->endaddr))
    {
        return -1;
    }

    if ((fp->rwpointer + len - 1) > fp->endaddr)
    {
        len = fp->endaddr - fp->rwpointer + 1;
    }

    hal_spi1nor_read(fp->rwpointer, (uint8 *) buffer, len);

    fp->rwpointer += len;

    return len;
}

