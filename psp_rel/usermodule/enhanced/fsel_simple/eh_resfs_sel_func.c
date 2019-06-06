/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-27 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"
static uint8 disk_c_file_total_num = 0; //c盘文件总数
static uint8 disk_c_file_num = 0; //当前播放的文件序号

/*
 扫描文件系统目录项，返回条目总数
 */
int32 get_disk_total_num(void)
{
    uint32 map_index = 0;
    uint32 map_index_address = 0;
    uint8 buf[32] =
        { 0 };
    res_dir_info_t *res_dir_t = NULL;

    if (disk_c_file_total_num == 0)
    {
        while (map_index < RES_MAP_SIZE)
        {
            map_index_address = map_index * 32;
            base_res_read(map_index_address, buf, 32);

            res_dir_t = (res_dir_info_t *) buf;
            if (0 == res_dir_t->length)
            {
                break;
            }
            map_index++;
        }
        disk_c_file_total_num = map_index;
    }

    return disk_c_file_total_num;
}

/*
 依据index 获取文件名
 input:idnex,文件序列
 output:filename 序列对应文件名

 注意:
 序列数据最小值为1,最大值为 RES_MAP_SIZE
 */
bool get_current_file_name(uint8 index, uint8 *filename)
{
    res_dir_info_t *res_dir;
    uint32 map_index = 0;
    uint32 map_index_address = 0;
    uint8 buf[32] =
        { 0 }, i = 0;
    res_dir_info_t *res_dir_t = NULL;

    disk_c_file_total_num = (uint8) get_disk_total_num();

    if ((index > disk_c_file_total_num) || (index <= 0))
    {
        return FALSE;
    }

    map_index = index - 1;

    map_index_address = map_index * 32;
    base_res_read(map_index_address, buf, 32);

    res_dir = (res_dir_info_t *) buf;

    uint8*name = (uint8*) res_dir->name;
    libc_memset(filename, 0, 13);
    if (0 == res_dir->length)
    {
        return FALSE;
    }
    else
    {
        for (i = 0; i < 12; i++)
        {
            if (name[i] != '!')
            {
                filename[i] = name[i];
            }
            else
            {
                break;
            }
        }
    }
    disk_c_file_num = index; //记录当前文件序列
    return TRUE;
}

/*
 根据文件名获取当前文件数目
 input :in_filename 输入文件名
 output:out_index 输出对应的文件序列号

 */
bool get_current_file_num(uint8*in_filename, uint8 *out_index)
{
    uint32 map_index = 0;
    uint32 map_index_address = 0;
    uint8 buf[32] =
        { 0 }, name_len = 0, now_num = 0;

    res_dir_info_t *res_dir = NULL;

    disk_c_file_total_num = (uint8) get_disk_total_num();

    while (1)
    {
        name_len++;
        if (in_filename[name_len] == '\0')
        {
            break;
        }
    }

    if (map_index > RES_MAP_SIZE)
    {
        return FALSE;
    }
    while (map_index < RES_MAP_SIZE)
    {
        map_index_address = map_index * 32;
        base_res_read(map_index_address, buf, 32);

        res_dir = (res_dir_info_t *) buf;
        if (0 == libc_memcmp(in_filename, res_dir->name, name_len))
        {
            break;
        }
        map_index++;
    }

    now_num = map_index + 1;
    if (now_num > disk_c_file_total_num)
    {
        return FALSE;
    }

    disk_c_file_num = now_num; //记录当前文件序列
    *out_index = now_num;
    return TRUE;
}

/*
 根据当前播放的曲目，获取下一个文件
 */
bool get_next_file(uint8*filename)
{
    uint32 map_index = 0;
    uint32 map_index_address = 0;
    uint8 buf[32] =
        { 0 }, i = 0;
    res_dir_info_t *res_dir = NULL;
    uint8*name = NULL;

    if ((disk_c_file_num > disk_c_file_total_num) || (disk_c_file_num <= 0))
    {
        return FALSE;
    }

    if (disk_c_file_num < disk_c_file_total_num)
    {
        disk_c_file_num++;
    }
    else
    {
        if (disk_c_file_num == disk_c_file_total_num)
        {
            disk_c_file_num = 1;
        }
    }

    map_index = disk_c_file_num - 1;
    map_index_address = map_index * 32;
    base_res_read(map_index_address, buf, 32);

    res_dir = (res_dir_info_t *) buf;

    name = (uint8*) res_dir->name;
    libc_memset(filename, 0, 13);
    if (0 == res_dir->length)
    {
        return FALSE;
    }
    else
    {
        for (i = 0; i < 12; i++)
        {
            if (name[i] != '!')
            {
                filename[i] = name[i];
            }
            else
            {
                break;
            }
        }
    }
    return TRUE;
}

/*
 根据当前播放的曲目，获取上一个文件
 */
bool get_prev_file(uint8*filename)
{
    uint32 map_index = 0;
    uint32 map_index_address = 0;
    uint8 buf[32] =
        { 0 }, i = 0;
    res_dir_info_t *res_dir;
    uint8*name = NULL;

    if ((disk_c_file_num > disk_c_file_total_num) || (disk_c_file_num <= 0))
    {
        return FALSE;
    }

    if (disk_c_file_num == 1)
    {
        disk_c_file_num = disk_c_file_total_num;
    }
    else
    {
        if (disk_c_file_num >= 2)
        {
            disk_c_file_num -= 1;
        }
    }

    map_index = disk_c_file_num - 1;
    map_index_address = map_index * 32;
    base_res_read(map_index_address, buf, 32);

    res_dir = (res_dir_info_t *) buf;

    name = (uint8*) res_dir->name;
    if (0 == res_dir->length)
    {
        return FALSE;
    }
    else
    {
        for (i = 0; i < 12; i++)
        {
            if (name[i] != '!')
            {
                filename[i] = name[i];
            }
            else
            {
                break;
            }
        }
    }
    return TRUE;
}

bool disk_c_fs_init(void)
{
    bool bret = TRUE;

    disk_c_file_total_num = 0;
    disk_c_file_num = 0;

    get_disk_total_num();

    if ((disk_c_file_total_num <= 0) || (disk_c_file_total_num > RES_MAP_SIZE))
    {
        bret = FALSE;
    }

    return bret;
}
