/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：用于对uhost 及 card写速度测试
 * 作者：
 ********************************************************************************/
#include "./../app_music.h"


//#define DISK_WRITE_TEST


#ifdef DISK_WRITE_TEST
#define START_RW 0
#define STOP_RW 1
#define WAIT_RW 2

#define RRECORD_FILE_BUFFER          (0x9FC25700)
#define RRECORD_FILE_BUFFER_LEN      (1024 * 2)
#define SPEED_MIN                    (50) //Kbytes/s
#define RECORD_LENGTH_M(x)           (1024*1024*(x))

typedef struct
{
    uint32 total;
    uint32 total_cnt;
    uint32 t200ms;
    uint32 c200_cnt;
}timer_type;

timer_type timer;

static const uint8 card_drv_name[] =
{   "card.drv"};
static const uint8 uhost_drv_name[] =
{   "uhost.drv"};

uint32 g_file_length = RECORD_LENGTH_M(1);
uint32 g_record_length = 0;
uint32 g_file_sys_id;

uint8 record_disk_type;

const uint8 record_filename[] =
{   0xff, 0xfe, 'r', 0x00, 'e', 0x00, 'c', 0x00, 'o', 0x00, 'r', 0x00, 'd', 0x00, 't', 0x00, 'e', 0x00, 's', 0x00, 't',
    0x00, '.', 0x00, 'b', 0x00, 'i', 0x00, 'n', 0x00, 0x00, 0x00};

void timer_init(void)
{
    timer.c200_cnt = 0;
    timer.t200ms = 0;
    timer.total = 0;
    timer.total_cnt = 0;
}

void timer_data_counter(void)
{
    timer.c200_cnt++;
    timer.total_cnt++;
}

uint32 set_card_status(uint8 mode)
{
    uint32 ret = 0;

    switch (mode)
    {
        case START_RW:
        device_set_rw_status(START_RW);
        break;

        case STOP_RW:
        device_set_rw_status(STOP_RW);
        break;

        case WAIT_RW:
        ret = device_set_rw_status(WAIT_RW);
        break;

        default:
        break;
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  装载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static uint32 _fs_init(uint8 disk_type)
{
    uint8 drv_type = 0;
    uint8 *drv_name = NULL;
    uint32 file_sys_id = 0;
    uint8 drv_mode = 0;

    record_disk_type = disk_type;

    if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) card_drv_name;
    }

    if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) uhost_drv_name;
    }

    //装载物理介质
    if (sys_drv_install(drv_type, drv_mode, drv_name) != 0)
    {
        return 0;
    }

    //装载文件系统
    file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

    if (file_sys_id == 0)
    {
        //卸载驱动程序
        sys_drv_uninstall(drv_type);
        return 0;
    }

    return file_sys_id;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  卸载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static bool _fs_deinit(uint8 disk_type, uint32 file_sys_id)
{
    int32 ret_val = 0;
    uint8 drv_type = 0;

    if (file_sys_id != 0)
    {
        ret_val = sys_unmount_fs(file_sys_id);
        file_sys_id = 0;
    }
    else
    {
        return TRUE;
    }

    if (ret_val < 0)
    {
        return FALSE;
    }

    if ((disk_type == DISK_H) || (disk_type == DISK_U))
    {
        drv_type = DRV_GROUP_STORAGE;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (ret_val < 0)
    {
        return FALSE;
    }

    return TRUE;
}

uint32 _com_file_open(uint8 disk_type, uint8 *file_name, uint8 open_mode)
{
    record_disk_type = disk_type;
    g_file_sys_id = _fs_init(disk_type);
    if (g_file_sys_id == 0)
    {
        return 0;
    }

    if (file_name == NULL)
    {
        return g_file_sys_id;
    }

    return 0;
}

//fhandle 为 0，表示磁盘操作，只需要卸载文件系统
bool _com_file_close(uint32 fhandle)
{
    if (fhandle != 0)
    {
        if (vfs_file_close(g_file_sys_id, fhandle) == FALSE)
        {
            return FALSE;
        }
    }

    return _fs_deinit(record_disk_type, g_file_sys_id);
}

int32 create_record_file(uint8* file_name)
{
    uint32 file_handle, t_start = 0;
    uint8 *file_buffer = (uint8 *) RRECORD_FILE_BUFFER;

    //加载文件系统,打开磁盘
    g_file_sys_id = _com_file_open(record_disk_type, NULL, 0);
    if (g_file_sys_id == 0)
    {
        return 0;
    }

    //在根目录下创建录音文件
    vfs_cd(g_file_sys_id, CD_ROOT, 0);

    libc_printf("create %d MBytes file...", g_file_length / 1024 / 1024);

    t_start = sys_get_ab_timer();
    file_handle = vfs_file_create(g_file_sys_id, file_name, g_file_length);
    if (0 != file_handle)
    {
        libc_printf("time of creat file:%d ms\n", sys_get_ab_timer() - t_start);
    }

    libc_memset(file_buffer, 0x22, RRECORD_FILE_BUFFER_LEN);
    vfs_file_close(g_file_sys_id, file_handle);

    return file_handle;
}

bool disk_plug_check(void)
{
    bool bret = TRUE;

    if (record_disk_type == DISK_H)
    {
        bret = sys_card_detect();
    }
    else if (record_disk_type == DISK_U)
    {
        bret = sys_uhost_detect();
    }
    else
    {
        bret = FALSE;
    }

    return bret;
}

void write_record_test(uint8 disk_type)
{
    uint8 data = 0;
    uint8 *file_buffer = (uint8 *) RRECORD_FILE_BUFFER;
    uint32 t_start = 0, t_end = 0, total_time = 0, ret = 0, i = 0;
    uint32 file_handle = 0;

    record_disk_type = disk_type;
    timer_init();

    sys_adjust_clk(FREQ_42M, SET_SYS_CLK);
    file_handle = create_record_file(record_filename);
    if (file_handle == 0)
    {
        libc_printf("creat file failed!\n");
        goto exit_record;
    }

    vfs_cd(g_file_sys_id, CD_ROOT, 0);
    libc_memset(file_buffer, data, RRECORD_FILE_BUFFER_LEN);

    file_handle = vfs_file_open(g_file_sys_id, record_filename, R_NORMAL_SEEK);
    g_record_length = (g_file_length / (1024*1024));

    libc_printf("start to write %d MB data to file...\n", g_record_length);

    timer.t200ms = sys_get_ab_timer();

    for (i = 0; i < (g_record_length * 512); i++) //write 2KB once
    {
        t_start = sys_get_ab_timer();
        ret = vfs_file_write(g_file_sys_id, file_buffer, RRECORD_FILE_BUFFER_LEN, file_handle);
        libc_printf("*");
        if (0 == ret)
        {
            libc_printf("write file failed! times:%d\n", i);
            goto exit_record;
        }
        t_end = sys_get_ab_timer();

        total_time += (t_end - t_start);
        timer_data_counter();

        if (data < 0xff)
        {
            data++;
        }
        else
        {
            data = 0;
        }

        libc_memset(file_buffer, data, RRECORD_FILE_BUFFER_LEN);

        if ((t_end - timer.t200ms) >= 200)
        {
            if (((timer.c200_cnt) * 10) <= SPEED_MIN)
            {
                libc_printf("200ms c:%d s:%d KB/s\n", timer.c200_cnt, (timer.c200_cnt) * 10);
            }
            timer.c200_cnt = 0;
            timer.t200ms = t_end;
        }
    }
    libc_printf("Test over! total time:%d ms, speed:%d KB/s\n",
        total_time, (g_record_length * 1024 * 1000) / total_time);

    exit_record:
    _com_file_close(file_handle);
}
#endif
