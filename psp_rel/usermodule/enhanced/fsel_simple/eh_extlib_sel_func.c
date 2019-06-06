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

static uint16 disk_c_file_total_num = 0; //c盘歌曲文件总数
static uint16 disk_c_file_num = 0; //当前播放的歌曲文件序号
static const uint8 music_ext_name[] = ".mp3";

static uint16 extlib_filename_strlen(uint8 *extlib_file_name)
{
    return libc_strlen(extlib_file_name) - 2; //extlib文件名字末尾有2个!字符
}

static bool is_music_file(uint8 *extlib_file_name)
{
    uint8 name_len;
    uint8 ext_index, i;
    uint8 lowcase_ext_name[5];

    name_len = extlib_filename_strlen(extlib_file_name);
    if (name_len <= 4)
    {
        return FALSE;
    }

    ext_index = name_len - 4;

    for (i = 0; i < 4; i++, ext_index++)
    {
        lowcase_ext_name[i] = extlib_file_name[ext_index];
        if ((lowcase_ext_name[i] >= 'A') && (lowcase_ext_name[i] <= 'Z'))
        {
            lowcase_ext_name[i] += ('a' - 'A');
        }
    }
    lowcase_ext_name[4] = 0;

    if (libc_memcmp(music_ext_name, lowcase_ext_name, 4) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 扫描文件系统目录项，返回条目总数
 */
uint16 extlib_get_disk_total_num(void)
{
    uint32 map_index;
    uint32 map_index_address;
    uint8 buf[32];
    res_dir_info_t *res_dir_t = NULL;
    if (disk_c_file_total_num == 0)
    {
        map_index = 0;
        while (map_index < RES_MAP_SIZE)
        {
            map_index_address = map_index * 32;
            lib_read_data(map_index_address, buf, 32);
            res_dir_t = (res_dir_info_t *) buf;
            if (0 == res_dir_t->length)
            {
                break;
            }

            if (is_music_file(res_dir_t->name) == TRUE)
            {
                disk_c_file_total_num++;
            }

            map_index++;
        }
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
bool extlib_get_current_file_name(uint16 num, uint8 *filename)
{
    res_dir_info_t *res_dir;
    uint32 map_index = 0;
    uint32 file_num = 0;
    uint32 map_index_address = 0;
    uint8 buf[32];
    bool ret = FALSE;

    if ((num > disk_c_file_total_num) || (num == 0))
    {
        return FALSE;
    }

    map_index = 0;
    file_num = 0;
    while (map_index < RES_MAP_SIZE)
    {
        map_index_address = map_index * 32;
        lib_read_data(map_index_address, buf, 32);
        res_dir = (res_dir_info_t *) buf;
        if (0 == res_dir->length)
        {
            break;
        }

        if (is_music_file(res_dir->name) == TRUE)
        {
            file_num++;
            if (file_num == num)
            {
                libc_memcpy(filename, res_dir->name, extlib_filename_strlen(res_dir->name));
                filename[extlib_filename_strlen(res_dir->name)] = 0;
                disk_c_file_num = num;
                ret = TRUE;
                break;
            }
        }

        map_index++;
    }

    return ret;
}

/*
 根据文件名获取当前文件数目
 input :in_filename 输入文件名
 output:out_index 输出对应的文件序列号

 */

bool extlib_get_current_file_num(uint8*in_filename, uint16 *out_index)
{
    uint32 map_index;
    uint32 map_index_address;
    uint8 buf[32];
    uint16 temp_name_len, name_len;
    uint16 file_num;
    res_dir_info_t *res_dir = NULL;
    bool ret = FALSE;

    name_len = libc_strlen(in_filename);
    map_index = 0;
    file_num = 0;
    while (map_index < RES_MAP_SIZE)
    {
        map_index_address = map_index * 32;
        lib_read_data(map_index_address, buf, 32);

        res_dir = (res_dir_info_t *) buf;

        if (is_music_file(res_dir->name) == TRUE)
        {
            file_num++;

            temp_name_len = extlib_filename_strlen(res_dir->name);
            if ((temp_name_len == name_len)
                    && (0 == libc_memcmp(in_filename, res_dir->name, name_len)))
            {
                *out_index = file_num;
                ret = TRUE;
                break;
            }
        }

        map_index++;
    }

    if (ret == TRUE)
    {
        disk_c_file_num = file_num;
    }

    return ret;
}

/*
 根据当前播放的曲目，获取下一个文件
 */

bool extlib_get_next_file(uint8*filename)
{
    bool ret;

    if ((disk_c_file_num > disk_c_file_total_num) || (disk_c_file_num == 0))
    {
        return FALSE;
    }

    if (disk_c_file_num < disk_c_file_total_num)
    {
        disk_c_file_num += 1;
    }
    else
    {
        disk_c_file_num = 1;
    }

    ret = extlib_get_current_file_name(disk_c_file_num, filename);
    return ret;
}

/*
 根据当前播放的曲目，获取上一个文件
 */
bool extlib_get_prev_file(uint8*filename)
{
    if ((disk_c_file_num > disk_c_file_total_num) || (disk_c_file_num == 0))
    {
        return FALSE;
    }

    if (disk_c_file_num == 1)
    {
        disk_c_file_num = disk_c_file_total_num;
    }
    else
    {
        disk_c_file_num -= 1;
    }

    return extlib_get_current_file_name(disk_c_file_num, filename);
}

bool extlib_disk_c_fs_init(void)
{
    bool bret = TRUE;
    disk_c_file_num = 0;
    extlib_get_disk_total_num();
    if (disk_c_file_total_num == 0)
    {
        bret = FALSE;
    }

    return bret;
}

