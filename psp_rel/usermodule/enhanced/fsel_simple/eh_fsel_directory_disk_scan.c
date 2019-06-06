/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     rongkuai     2018-10-30       1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"
#include "pthread.h"
#include "task_manager.h"

#define DISK_SCAN_THREAD_PTOS       (MUSIC_FSEL_DISK_SCAN_DATA_STACK_ADDR + MUSIC_FSEL_DISK_SCAN_DATA_STACK_LEN)
#define DISK_SCAN_THREAD_STACK_LEN  (MUSIC_FSEL_DISK_SCAN_DATA_STACK_LEN - sizeof(disk_scan_info_t) - 8)

#define DIR_ENTRY_ATTR       2105376
#define DISK_SCAN_MAX_TIMER  8000

bool is_current_dir_location(file_location_t* cur_file_location)
{
    uint8 layer1 = g_p_disk_scan_info->search_location.dir_layer_info.buffer[72];

    uint8 layer2 = cur_file_location->dir_layer_info.buffer[72];

    if (layer1 == layer2)
    {
        if (layer1 == 0)
        {
            return TRUE;
        }
        else
        {
            if (libc_memcmp(g_p_disk_scan_info->search_location.dir_layer_info.buffer,
                    cur_file_location->dir_layer_info.buffer, 8*layer1) == 0)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

bool disk_scan_search_file(file_location_t* cur_file_location)
{
    uint32 ret;
    uint8 bret;
    uint8 filter_duplicate_scan_directory = 0;
    bool is_max_layer = FALSE;
    
    if (eh_dir_layer >= (DIRLAYER_MAX - 1))
    {
        is_max_layer = TRUE;
    }
    
    do
    {
        ret = vfs_dir(eh_vfs_mount, g_p_disk_scan_info->dir_type, NULL, fsel_init_val.file_type_bit | EXT_NAME_ALL_DIR);

        //获取当前文件的路径信息
        if (FALSE == vfs_file_dir_offset(eh_vfs_mount, &(g_p_disk_scan_info->search_location.dir_layer_info),
                (pfile_offset_t * ) &(g_p_disk_scan_info->search_location.cluster_no), 0)) //cluster_no&dir_entry
        {
            return FALSE;
        }

        filter_duplicate_scan_directory = is_current_dir_location(cur_file_location);

        if ((ret != DIR_ENTRY_ATTR) && (ret != 0))
        {
            if (filter_duplicate_scan_directory == 1)
            {
                uint8 temp_offset_buf[8];

                vfs_file_dir_offset(eh_vfs_mount, NULL, (pfile_offset_t* ) temp_offset_buf, 0);

                g_p_disk_scan_info->current_file++;

                if (libc_memcmp((uint8*)&cur_file_location->cluster_no, temp_offset_buf, 8) == 0)
                {
                    eh_file_no = g_p_disk_scan_info->current_file;
                }
            }

            eh_file_total_all++;
            if (eh_file_total_all >= SUPPORT_MUSIC_FILE)
            {
                libc_printf("disk scan max file number! %d\n", SUPPORT_MUSIC_FILE);
                return TRUE;
            }

            if ((sys_get_ab_timer() - g_p_disk_scan_info->search_start_timer) >= DISK_SCAN_MAX_TIMER)
            {
                libc_printf("disk scan max timer! %d\n", DISK_SCAN_MAX_TIMER);
                return TRUE;
            }
            g_p_disk_scan_info->dir_type = DIR_NEXT;
        }
        else if (ret == DIR_ENTRY_ATTR)
        {
            filter_duplicate_scan_directory = 0;

            if ((TRUE == filtrate_dir()) || (is_max_layer == TRUE))
            {
                g_p_disk_scan_info->dir_type = DIR_NEXT;
                continue;
            }
            else if (0 != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                bool is_scan_over = FALSE;
                eh_dir_layer++;
                //libc_printf("eh_dir_layer %d\n", eh_dir_layer);
                g_p_disk_scan_info->dir_type = DIR_HEAD;

                is_scan_over = disk_scan_search_file(cur_file_location);
                if (is_scan_over == TRUE)
                {
                    return TRUE;
                }
            }
        }
        else if (ret == 0) //当前目录扫描完
        {
            filter_duplicate_scan_directory = 0;

            //切到上一层，继续下一个
            if (0 != vfs_cd(eh_vfs_mount, CD_BACK, NULL))
            {
                //libc_printf("..cd _back\n");
                eh_dir_layer--;
                g_p_disk_scan_info->dir_type = DIR_NEXT;
            }
            return FALSE;
        }

    }
    while (1);

    return FALSE;
}

uint16 * disk_scan_search_file_entry(file_location_t* cur_file_location)
{
    uint32 t1, t2;

    t1 = sys_get_ab_timer();
    libc_printf_info("creat_scan_music_file_thread @%d\n", t1);

    disk_scan_search_file(cur_file_location);

    t2 = sys_get_ab_timer();
    libc_printf("..%d @ %d @ %d \n", eh_file_total_all, eh_file_no, g_p_disk_scan_info->current_file);
    libc_printf("scan_time %d \n", t2 - t1);

    g_p_disk_scan_info->disk_scan_over_flag = 1;
    eh_file_total = g_p_disk_scan_info->current_file;

    libc_pthread_exit();
}

uint8 *creat_disk_scan_thread(file_location_t *cur_file_location)
{
    pthread_param_t temp_disk_scan_thread;

    pthread_param_t *p_thread_param = &temp_disk_scan_thread;

    //初始化创建线程的参数
    p_thread_param->start_rtn = (uint8 *) disk_scan_search_file_entry;
    p_thread_param->arg = (void *) cur_file_location;
    p_thread_param->ptos = (OS_STK *) DISK_SCAN_THREAD_PTOS;
    p_thread_param->stk_size = (uint32) DISK_SCAN_THREAD_STACK_LEN;

    //禁止调度
    sys_os_sched_lock();

    g_p_disk_scan_info = (disk_scan_info_t *) (MUSIC_FSEL_DISK_SCAN_DATA_STACK_ADDR);
    libc_memset(g_p_disk_scan_info, 0, sizeof(disk_scan_info_t));
    g_p_disk_scan_info->dir_type = DIR_HEAD;
    g_p_disk_scan_info->current_file = 0;
    g_p_disk_scan_info->disk_scan_over_flag = 0;
    g_p_disk_scan_info->search_start_timer = sys_get_ab_timer();

    //创建扫描歌曲子线程
    if (libc_pthread_create(p_thread_param, AP_FRONT_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        libc_printf_error("Create scan music file thread Fail!!\n");
        sys_os_sched_unlock();
        return NULL;
    }
    sys_os_sched_unlock();

    while (1)
    {
        if (g_p_disk_scan_info->disk_scan_over_flag == 0)
        {
            sys_os_time_dly(1);
        }
        else
        {
            break;
        }
    }

    return NULL;
}
