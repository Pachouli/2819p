/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：用于对文件选择器进行测试
 * 作者：
 ********************************************************************************/
#include "./../app_music.h"


//#define FSEL_ENUM_DIR_FUNC_TEST


#ifdef FSEL_ENUM_DIR_FUNC_TEST

/*
    十六进制数值打印
    需使用CPU模式的UART打印(UART_PRINT_MODE_AHB)，
    在print_para_setting()函数中设定
*/
static void print_data(uint8 *buf, int len)
{
    while(len > 0)
    {
        if(len > 8) {
            libc_printhex(buf, 8, 0);
            buf += 8;
            len -= 8;
        }
        else {
            libc_printhex(buf, len, 0);
            len = 0;
        }
    }
}



/*
    eh_fsel_enum_dir.c导出函数测试
    (EXT_NAME_ALL_DIR | EXT_NAME_ALL_FILE) - 枚举所有文件和目录
*/
void fsel_enum_dir_func_test(uint8 disk)
{
    int seq;
    int retval;
    int file_cnt;
    uint32 vfs_mount_hdl;
    DIRITEM *p_dir_item, *t_dir_item;
    fsel_enum_param_t enum_para;
    uint32 file_type_mask = (EXT_NAME_ALL_DIR)|(MUSIC_BITMAP);
    int sys_clk_backup;

    libc_printf("\r\n\r\n###fsel_num_dir_func_test(%02x) ENTER\r\n", disk);

    p_dir_item = sys_malloc(sizeof(DIRITEM));
    if(p_dir_item == NULL)
    {
        libc_printf("#malloc p_dir_item fail!\r\n");
        goto test_end_label;
    }

    t_dir_item = sys_malloc(sizeof(DIRITEM));
    if(t_dir_item == NULL)
    {
        libc_printf("malloc t_dir_item fail!\r\n");
        goto test_end_label;
    }

    //文件浏览时提高频率，确保解码性能
    sys_clk_backup = com_adjust_sys_clk(FREQ_108M, TRUE);

    // 挂载文件系统驱动到指定的存储设备
    enum_para.disk = disk;
    enum_para.file_type_bit = MUSIC_BITMAP;
    enum_para.dir_type_bit = ALLDIR_BITMAP;
    vfs_mount_hdl = fsel_vfs_init(&enum_para);
    if(vfs_mount_hdl == 0)
    {
        libc_printf("#fsel_vfs_init() fail!\r\n");
        goto test_end_label;
    }




    // 枚举根目录所有文件，并打印目录项内容
    libc_printf("Search Root directory:\r\n\r\n");

    retval = fsel_enum_dir_set(vfs_mount_hdl, "/", 2);
    if(retval == 0)
    {
        libc_printf("set root dir fail!\r\n");
        goto test_end_label;
    }

    file_cnt = 0;

    while(1)
    {
        libc_memset(p_dir_item, 0, sizeof(DIRITEM));
        retval = fsel_enum_dir_next(vfs_mount_hdl, file_type_mask, p_dir_item);
        if(retval == 0)
        {
            libc_printf("dir next fail! break\r\n");
            break;
        }

        file_cnt++;

        if(p_dir_item->attr & ATTR_DIRECTORY)
        {
            libc_printf("[DIR] ");
        }
        else
        {
            libc_printf("[FILE] ");
        }

        print_data(p_dir_item->name_buf, p_dir_item->name_len);
        libc_printf("\r\n");
    }

    libc_printf("\r\n>>> Total dir item : %d\r\n", file_cnt);
    libc_printf("--------------------------------------------------\r\n");


    test_end_label:
    if(vfs_mount_hdl != 0)
    {
        fsel_vfs_deinit(vfs_mount_hdl);
    }

    com_adjust_sys_clk(sys_clk_backup, FALSE);

    if(t_dir_item != NULL)
    {
        sys_free(t_dir_item);
    }

    if(p_dir_item != NULL)
    {
        sys_free(p_dir_item);
    }

    libc_printf("###fsel_num_dir_func_test() EXIT\r\n\r\n");
}




/*
    选择根目录下的第三个音乐文件播放
    需要在music_engine正常工作后才能调用
*/
void select_media_file_play_test(uint8 disk)
{
    int seq = 0;  // 序号初始0
    int retval;
    uint32 vfs_mount_hdl;
    DIRITEM *p_dir_item;
    fsel_enum_param_t enum_para;
    uint32 file_type_mask = (MUSIC_BITMAP);
    int sys_clk_backup;

    libc_printf("select_mediafile_play_test()\r\n");

    p_dir_item = sys_malloc(sizeof(DIRITEM));
    if(p_dir_item == NULL)
    {
        libc_printf("#malloc p_dir_item fail!\r\n");
        return;
    }

    //文件浏览时提高频率，确保解码性能
    sys_clk_backup = com_adjust_sys_clk(FREQ_108M, TRUE);

    enum_para.disk = disk;
    enum_para.file_type_bit = MUSIC_BITMAP;
    enum_para.dir_type_bit = ALLDIR_BITMAP;
    vfs_mount_hdl = fsel_vfs_init(&enum_para);
    if(vfs_mount_hdl == 0)
    {
        libc_printf("#fsel_vfs_init() fail!\r\n");
        sys_free(p_dir_item);
        return;
    }

    retval = fsel_enum_dir_set(vfs_mount_hdl, "/", 2);
    if(retval == 0)
    {
        libc_printf("set root dir fail!\r\n");
        goto play_test_label;
    }


    while(1)
    {
        libc_memset(p_dir_item, 0, sizeof(DIRITEM));
        retval = fsel_enum_dir_next(vfs_mount_hdl, file_type_mask, p_dir_item);
        if(retval == 0)
        {
            libc_printf("dir next fail! break\r\n");
            break;
        }

        seq++;

        if(seq == 3)
        {
            libc_printf("Select File Name :\r\n");
            print_data(p_dir_item->name_buf, p_dir_item->name_len);
            libc_printf("\r\n");

            break;
        }

    }


    if(seq == 3)  // 发送命令到music_engine播放选定的文件
    {
        file_location_t *p_file_loc;
        file_path_info_t *p_locat_info;

        p_locat_info = (file_path_info_t*)sys_malloc(sizeof(file_path_info_t));
        if(p_locat_info == NULL)
        {
            libc_printf("malloc file_path_info_t fail!\r\n");
            goto play_test_label;
        }

        p_file_loc = &p_locat_info->file_path.dirlocation;

        if(fsel_get_file_location(vfs_mount_hdl, p_file_loc) == 1)
        {
            //发送同步消息
            p_locat_info->file_source = FSEL_TYPE_COMMONDIR;
            p_locat_info->reserved[0] = 0;

            if (music_send_msg(p_locat_info,  NULL,  MSG_MENGINE_SET_FILEPATH_SYNC) == TRUE)
            {
                music_play();
            }
        }

        libc_printf("select ok\r\n");

        sys_free(p_locat_info);  // sys_free释放p_locat_info
    }
    else
    {
        libc_printf("select fail!\r\n");
    }

    play_test_label:

    fsel_vfs_deinit(vfs_mount_hdl);

    com_adjust_sys_clk(sys_clk_backup, FALSE);

    sys_free(p_dir_item);

    return;
}


#endif
