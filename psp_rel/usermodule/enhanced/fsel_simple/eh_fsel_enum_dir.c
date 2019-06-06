/*******************************************************************************
 *                                US281B
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2018 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>                   <version >             <desc>
 *       chenxu           2018-06-20                     1.0               build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

/*
 固定"常量"
 */
static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

static const uint8 rec_dir_name[] = "RECORD  ";
static const uint8 alarm_dir_name[] = "ALARM   ";

/*
 全局变量
 */
static uint8 g_filesys_disk;
static uint32 g_filesys_handle;

static uint32 g_music_bitmap;
static uint32 g_dir_bitmap;

/*
 内部函数声明
 */
static bool filter_record_dir(uint32 vfs_mount_hdl);
static int get_music_num_by_offset(uint32 vfs_mount_hdl, uint32 cluster_no, uint32 dir_entry);
static int fsel_enum_dir_cmd(uint32 vfs_mount_hdl, uint8 dir_cmd, uint32 file_type_mask, DIRITEM *p_dir_item);

/*
 过滤录音/闹铃目录
 代码源自bool filtrate_dir(void)函数
 */
static bool filter_record_dir(uint32 vfs_mount_hdl)
{
    uint8 *buf;
    uint16 name_len = 0;
    uint8 *filter_dir_name;
    uint8 dir_name_buffer[32];
    uint8 i, j, dir_index;
    bool is_unicode;
    bool match_flag;

    name_len = vfs_get_name(vfs_mount_hdl, dir_name_buffer, 16);
    if (name_len == 0)
    {
        return FALSE;
    }

    is_unicode = FALSE;
    if ((dir_name_buffer[0] == 0xff) && (dir_name_buffer[1] == 0xfe))
    {
        is_unicode = TRUE;
    }

    if (is_unicode == FALSE)
    {
        //8+3 短名
        buf = dir_name_buffer;
        name_len = 8;
    }
    else
    {
        if (name_len > 2)
        {
            //unciode
            buf = dir_name_buffer + 2;
            name_len = name_len - 2;
        }
        else
        {
            return FALSE;
        }
    }

    //0:normal play 2:record 3:alarm 4:dir1 5:dir2
    for (dir_index = 0; dir_index < 2; dir_index++) // FILTER_DIR_NUM
    {
        if (dir_index == 0)
        {
            filter_dir_name = rec_dir_name;
        }
        else if (dir_index == 1)
        {
            filter_dir_name = alarm_dir_name;
        }
        else
        {
            return FALSE;
        }

        match_flag = TRUE;
        i = 0;
        for (j = 0; ((j < name_len) && (j < 8)); j++) // DIR_NAME_LEN
        {
            if (filter_dir_name[j] != buf[i])
            {
                match_flag = FALSE;
                break;
            }
            if (is_unicode == FALSE)
            {
                i++;
            }
            else
            {
                i += 2;
            }
        }
        if ((match_flag == TRUE)
                && (j >= name_len)
                && ((filter_dir_name[j] == 0x00) || (filter_dir_name[j] == ' ')))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static int get_music_num_by_offset(uint32 vfs_mount_hdl, uint32 cluster_no, uint32 dir_entry)
 * \根据音乐文件绝对物理位置计算选定歌曲在磁盘的总序号
 * \Parameters:
 * \      [in]    vfs_mount_hdl - 已加载的文件系统卷驱动句柄
 * \      [in]    cluster_no, dir_entry - 选择文件位于磁盘的绝对位置
 * \Return:       int the result
 * \           0 failed
 * \           other value is music file number
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:        内部函数
 */
/*******************************************************************************/
static int get_music_num_by_offset(uint32 vfs_mount_hdl, uint32 cluster_no, uint32 dir_entry)
{
    int retval;
    int cur_num = 0;
    int dir_layer = 0;

    uint32 file_type_mask;
    uint32 dir_offset[2];

    uint32 old_dir_offset[2];
    pdir_layer_t *p_dir_layer;

    uint8 attr;
    uint8 search_flag = 0; // 搜索阶段标志，0表示当前在搜索文件，1表示搜索目录

    libc_printf("#get_file_num_by_entry() enter\r\n");

    // 申请pdir_layer_t大内存
    p_dir_layer = sys_malloc(sizeof(pdir_layer_t));
    if (NULL == p_dir_layer)
    {
        libc_printf("#get_file_num_by_entry() : malloc pdir_layer_t fail!\r\n");
        return 0;
    }

    // 保存当前vfs_mount_hdl
    retval = vfs_file_dir_offset(vfs_mount_hdl, p_dir_layer->buffer, old_dir_offset, 0);
    if (0 == retval)
    {
        libc_printf("#get_file_num_by_entry() : get offset fail!\r\n");
        sys_free(p_dir_layer);
        return 0;
    }

    // 从根目录开始寻找
    vfs_cd(vfs_mount_hdl, CD_ROOT, NULL);
    vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);

    file_type_mask = g_music_bitmap;

    // 循环搜索，先搜索文件，再搜索目录
    do
    {
        retval = vfs_dir(vfs_mount_hdl, DIR_NEXT, NULL, file_type_mask);
        if (retval != 0)
        {
            attr = vfs_file_attralter(vfs_mount_hdl, 0, NULL, 1);

            if (attr & ATTR_DIRECTORY)
            {
                //libc_printf("##ATTR_DIRECTORY\r\n");

                if ((dir_layer == 0) && (eh_fsel_filter_record_dir_flag == TRUE)) //只需过滤根目录下的RECORD和ALARM目录
                {
                    if (filter_record_dir(vfs_mount_hdl) == TRUE)
                    {
                        libc_printf("#filter record dir!\n");
                        retval = 1;
                        continue; //被过滤掉了，需要继续查找下一个目录
                    }
                }

                retval = vfs_cd(vfs_mount_hdl, CD_SUB, NULL); // 进入当前目录
                if (retval != 1) // 这里失败有可能是进入了超过8级的目录，20180907
                {
                    libc_printf("#vfs_cd() fail!%d\r\n", dir_layer); // 错误时打印出: dir_layer == 8 , by jc0245
                    retval = 0;
                    break;
                }

                // 进入目录后开始搜索文件
                dir_layer++;

                search_flag = 0;
                file_type_mask = g_music_bitmap;

            }
            else // 文件、非目录
            {
                cur_num++;

                // 对比检查
                retval = vfs_file_dir_offset(vfs_mount_hdl, NULL, dir_offset, 0);
                if (retval != 1)
                {
                    libc_printf("#vfs compare fail!\r\n");
                    retval = 0;
                    break;
                }

                if ((cluster_no == dir_offset[0]) && (dir_entry == dir_offset[1]))
                {
                    libc_printf("hit cur file!%d\r\n", cur_num);
                    retval = 1;
                    break;
                }

            }
        }
        else // 失败，退回到上一层目录，当回退时，必定在检索文件夹了
        {
            if (search_flag == 0) // 遍历文件结束，到目录
            {
                if (dir_layer < 8) // 小于8级目录，继续遍历目录
                {
                    search_flag = 1;
                    file_type_mask = g_dir_bitmap;
                    retval = 1;

                    vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
                    continue;
                }
                else // 大于等于8级目录，不能遍历目录了
                {
                    // 即便遍历也会失败的，失败之后会导致后面的歌曲都获取不到正确的序号
                    libc_printf("#serach_flag==0 && dir_layer >= 8!\r\n");
                }
            }

            if (dir_layer == 0) // 到根目录了，后面没有了
            {
                libc_printf("#dir_layer == 0!!!\r\n");
                break;
            }

            retval = vfs_cd(vfs_mount_hdl, CD_BACK, NULL); // 退回到上一级目录, 退回后的文件位置???
            if (retval != 0) // 0-失败；1-成功
            {
                dir_layer--;

                search_flag = 1;
                file_type_mask = g_dir_bitmap;
            }
            else
            {
                libc_printf("#vfs_cd CD_BACK failure!\r\n");
                break;
            }

        }

    } while (retval != 0);

    // 恢复句柄
    vfs_file_dir_offset(vfs_mount_hdl, p_dir_layer->buffer, old_dir_offset, 1);

    // 释放资源
    sys_free(p_dir_layer);

    if (0 == retval) // 失败
    {
        cur_num = 0;
    }

    return cur_num;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_first(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
 * \文件系统遍历目录接口命令封装
 * \Parameters:
 * \      [in]    vfs_mount_hdl - 已加载的文件系统卷句柄
 * \      [in]    dir_cmd - DIR命令: DIR_HEAD、DIR_TAIL、DIR_PREV、DIR_NEXT
 * \      [in]    file_type_mask - 文件类型过滤，参见vfs_interface.h中的"DIR 扩展名的映射"定义
 * \      [out]  p_dir_item - 输出目录项结构指针，具体参见DIRITEM结构说明
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       内部函数
 */
/*******************************************************************************/
static int fsel_enum_dir_cmd(uint32 vfs_mount_hdl, uint8 dir_cmd, uint32 file_type_mask, DIRITEM *p_dir_item)
{
    int len;
    int retval;

    if (0 == vfs_mount_hdl)
    {
        libc_printf("#fsel_enum_dir_cmd():vfs_mount_hdl==0!\r\n");
        return 0;
    }

    if (NULL == p_dir_item)
    {
        libc_printf("fsel_enum_dir_cmd:p_dir_item==NULL!\r\n");
        return 0;
    }

    retval = vfs_dir(vfs_mount_hdl, dir_cmd, NULL, file_type_mask);
    if (retval != 0)
    {
        len = vfs_get_name(vfs_mount_hdl, p_dir_item->name_buf, (sizeof(p_dir_item->name_buf) / 2)); // 注意最后的longname_length参数为2字节单位!
        if (len != 0) // len要么大于2，要么等于11
        {
            p_dir_item->attr = vfs_file_attralter(vfs_mount_hdl, 0, NULL, 1);

            if ((p_dir_item->name_buf[0] == 0xff) && (p_dir_item->name_buf[1] == 0xfe))
            {
                len = len * 2; // 长文件名返回实际字符个数，包括开头和结尾标志
            }

            p_dir_item->name_len = len;

            retval = 1;
        }
        else // len == 0
        {
            libc_printf("#fsel_enum_dir_cmd(%02x) : get name fail!\r\n", dir_cmd);
            retval = 0;
        }

    }
    else
    {
        libc_printf("#fsel_enum_dir_cmd(%02x):ret==0\r\n", dir_cmd);
    }

    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_set(int vfs_mount_hdl, uint8 *dir_name, uint32 path_len)
 * \初始定位要枚举的目录
 * \Parameters:
 * \      [in]    vfs_mount_hdl  - 已加载的文件系统卷句柄
 * \      [in]    dir_name - 目录名称缓冲，特殊名称参见下面的note
 * \      [in]    path_len - 目录名称缓冲长度，字节单位
 * \Return:     int the result
 * \    1 sucess
 * \    0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:        特殊目录名称转义:
 * \                    "/" - 表示定位到根目录开始枚举，dir_name[]={'/',0}
 * \                    ".." - 表示定位到上一级目录，dir_name[]={'.','.',0}
 * \                    "."  -表示定位到当前目录开头，dir_name[]={'.',0}
 * \            其他名称表示8.3格式目录名称或UNICODE格式目录名称
 */
/*******************************************************************************/
int fsel_enum_dir_set(uint32 vfs_mount_hdl, uint8 *dir_name, uint16 path_len)
{
    int retval = 0;

    //libc_printf("#fsel_enum_dir_set\r\n");

    if (0 == vfs_mount_hdl)
    {
        libc_printf("#fsel_enum_dir_set():vfs_mount_hdl==0!\r\n");
        return 0;
    }

    if (NULL == dir_name)
    {
        libc_printf("##fsel_enum_dir_set():dir_name==NULL!\r\n");
        return 0;
    }

    if (path_len >= 4) // 检索当前目录内的名称
    {
        retval = vfs_cd(vfs_mount_hdl, CD_SUB, dir_name);

        //retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
    }
    else if (3 == path_len)
    {
        if ((dir_name[0] == '.') && (dir_name[1] == '.') && (dir_name[2] == 0)) // 检索上一级目录
        {
            retval = vfs_cd(vfs_mount_hdl, CD_BACK, NULL);
        }
    }
    else if (2 == path_len)
    {
        if ((dir_name[0] == '/') && (dir_name[1] == 0)) // 从根目录开始检索
        {
            retval = vfs_cd(vfs_mount_hdl, CD_ROOT, NULL);
            retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
        }
        else if ((dir_name[0] == '.') && (dir_name[1] == 0)) // 从头开始检索当前目录
        {
            retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
        }
        else if ((dir_name[0] == '.') && (dir_name[1] == '.'))
        {
            retval = vfs_cd(vfs_mount_hdl, CD_BACK, NULL);
        }
    }

    else if (1 == path_len)
    {
        if (dir_name[0] == '/')
        {
            retval = vfs_cd(vfs_mount_hdl, CD_ROOT, NULL);
            retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
        }
        else if (dir_name[1] == '.')
        {
            retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
        }
    }
    else
    {
        libc_printf("#fsel_enum_dir_set() path_len:%d\r\n", path_len);
        retval = 0;
    }

    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_first(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
 * \选择当前目录第一个指定类型的文件(夹)，并返回目录项信息
 * \Parameters:
 * \      [in]    vfs_mount_hdl - 已加载的文件系统卷句柄
 * \      [in]    file_type_mask - 文件类型过滤，参见vfs_interface.h中的"DIR 扩展名的映射"定义
 * \      [out]  p_dir_item - 输出目录项结构指针，具体参见DIRITEM结构说明
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) 在DIRITEM结构中的name_len成员，短文件名返回11，长文件名返回实际字节数(包括开头结尾)
 * \               (2) 用户层在获取到短文件名后需要自行转换为正常名称
 * \               (3) 之后可以用fsel_enum_dir_next()继续遍历
 */
/*******************************************************************************/
int fsel_enum_dir_first(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
{
    int retval;

    retval = fsel_enum_dir_cmd(vfs_mount_hdl, DIR_HEAD, file_type_mask, p_dir_item);

    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_last(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
 * \选择当前目录最后一个指定类型的文件(夹)，并返回目录项信息
 * \Parameters:
 * \      [in]    vfs_mount_hdl - 已加载的文件系统卷句柄
 * \      [in]    file_type_mask - 文件类型过滤，参见vfs_interface.h中的"DIR 扩展名的映射"定义
 * \      [out]  p_dir_item - 输出目录项结构指针，具体参见DIRITEM结构说明
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) 在DIRITEM结构中的name_len成员，短文件名返回11，长文件名返回实际字节数(包括开头结尾)
 * \               (2) 用户层在获取到短文件名后需要自行转换为正常名称
 * \               (3) 之后可以用fsel_enum_dir_prev()继续遍历
 */
/*******************************************************************************/
int fsel_enum_dir_last(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
{
    int retval;

    retval = fsel_enum_dir_cmd(vfs_mount_hdl, DIR_TAIL, file_type_mask, p_dir_item);

    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_prev(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
 * \选择当前目录内指向文件的上一个文件(夹)，并返回目录项信息
 * \Parameters:
 * \      [in]    vfs_mount_hdl - 已加载的文件系统卷句柄
 * \      [in]    file_type_mask - 文件类型过滤，参见vfs_interface.h中的"DIR 扩展名的映射"定义
 * \      [out]  p_dir_item - 输出目录项结构指针，具体参见DIRITEM结构说明
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) 在DIRITEM结构中的name_len成员，短文件名返回11，长文件名返回实际字节数(包括开头结尾)
 * \               (2) 用户层在获取到短文件名后需要自行转换为正常名称
 * \               (3) 当调用该函数失败时，需调用fsel_enum_dir_set(vfs_mount_hdl,".",2)从头开始遍历
 */
/*******************************************************************************/
int fsel_enum_dir_prev(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
{
    int retval;

    retval = fsel_enum_dir_cmd(vfs_mount_hdl, DIR_PREV, file_type_mask, p_dir_item);

    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_next(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
 * \选择当前目录内指向文件的下一个文件(夹)，并返回目录项信息
 * \Parameters:
 * \    [in] vfs_mount_hdl - 已加载的文件系统卷句柄
 * \    [in] file_type_mask - 文件类型过滤，参见vfs_interface.h中的"DIR 扩展名的映射"定义
 * \    [out] p_dir_item - 输出目录项结构指针，具体参见DIRITEM结构说明
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) DIRITEM结构中的name_len成员，短文件名返回11，长文件名返回实际字节数(包括开头结尾)
 * \               (2) 用户层在获取到短文件名后需要自行转换为正常名称
 * \               (3) 当调用该函数失败时，需调用fsel_enum_dir_prev()才能指向失败前选择的文件
 */
/*******************************************************************************/
int fsel_enum_dir_next(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
{
    int retval;

    retval = fsel_enum_dir_cmd(vfs_mount_hdl, DIR_NEXT, file_type_mask, p_dir_item);

    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_get_file_location(uint32 vfs_mount_hdl, file_location_t *p_file_loc)
 * \获取当前卷所选择文件/文件夹的位置信息
 * \Parameters:
 * \      [in]    vfs_mount_hdl - 已挂载的文件系统卷句柄
 * \      [out]    p_file_loc - 文件位置信息结构
 * \Return:       int the result
 * \           1 success
 * \           0 failure
 * \Ingroup:       eh_fsel_enum_dir.c
 * \Note:           该函数可获取选定文件位置信息，用于后续指定播放
 */
/*******************************************************************************/
int fsel_get_file_location(uint32 vfs_mount_hdl, file_location_t *p_file_loc)
{
    int len;
    int ret_val;

    //libc_printf("#fsel_get_file_location()\r\n");

    if (0 == vfs_mount_hdl)
    {
        libc_printf("#fsel_get_file_location():vfs_mount_hdl==0!\r\n");
        return 0;
    }

    if (NULL == p_file_loc)
    {
        libc_printf("#fsel_get_file_location():p_file_loc==NULL!\r\n");
        return 0;
    }

    // 获取扩展名
    len = vfs_get_name(vfs_mount_hdl, p_file_loc->filename, 0);
    if (len == 0)
    {
        libc_printf("#get extname fail!\r\n");
        return 0;
    }

    // 获取当前选择文件路径信息
    ret_val = vfs_file_dir_offset(vfs_mount_hdl, p_file_loc->dir_layer_info.buffer, &p_file_loc->cluster_no, 0);
    if (ret_val == 0)
    {
        libc_printf("#get offset fail!\r\n");
        return 0;
    }

    p_file_loc->dir_layer = p_file_loc->dir_layer_info.buffer[72];

    p_file_loc->disk = g_filesys_disk;

    // 当使用MSG_MENGINE_SET_FILEPATH_SYNC选择文件播放时
    // 下面这两个参数在fsel_dir_set_location中会有赋值
    // 因此这里不需要赋值，或随便赋个初值
    p_file_loc->file_total = 0;
    p_file_loc->file_total_disk = 0;

    // 获取选定音乐文件在磁盘序号
    p_file_loc->file_num = get_music_num_by_offset(vfs_mount_hdl, p_file_loc->cluster_no, p_file_loc->dir_entry);
    if (p_file_loc->file_num == 0) // 获取失败了
    {
        libc_printf("#get_file_num_by_offset() fail!\r\n");
        p_file_loc->file_num = 1;
    }

    libc_printf("\r\n#p_file_loc->file_num = %d\r\n", p_file_loc->file_num);

    p_file_loc->resever = 0;
    p_file_loc->res_3byte[0] = 0;
    p_file_loc->res_3byte[1] = 0;
    p_file_loc->res_3byte[2] = 0;

    return 1;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint32 fsel_vfs_init(uint8 disk)
 * \初始化一个文件系统卷句柄
 * \Parameters:
 * \      [in]    disk - 存储设备标识，可为DISK_H(TF卡)、DISK_U(U盘)
 * \Return:       uint32 the result
 * \           0 failed
 * \           other value represent a filesystem handle
 * \Ingroup:       eh_fsel_enum_dir.c
 * \Note:           281X最多支持两个文件系统卷
 */
/*******************************************************************************/
uint32 fsel_vfs_init(fsel_enum_param_t *p_enum_para)
{
    uint32 filesys_handle;
    uint8 disk = p_enum_para->disk;
    char *drv_name;

    g_music_bitmap = p_enum_para->file_type_bit;
    g_dir_bitmap = p_enum_para->dir_type_bit;

    libc_printf("#fsel_vfs_init(%02x)\r\n", disk);

    if (disk == DISK_H)
    {
        drv_name = (char*) card_drv_name;
    }
    else if (disk == DISK_U)
    {
        drv_name = (char*) uhost_drv_name;
    }
    else
    {
        libc_printf("#fsel_vfs_init() param err:%02x\r\n", disk);
        return 0;
    }

    // 装载磁盘驱动
    if (sys_drv_install(DRV_GROUP_STORAGE, 0, drv_name) != 0) // SLOW_DRIVER
    {
        libc_printf("#fsel_vfs_init() : install storage drv fail!\r\n");
        return 0;
    }

    // 装载文件系统驱动
    filesys_handle = sys_mount_fs(DRV_GROUP_STORAGE, disk, 0);
    if (filesys_handle != 0)
    {
        g_filesys_handle = filesys_handle;

        g_filesys_disk = disk;
    }
    else // 装载失败
    {
        libc_printf("#fsel_vfs_init() :  mount fs fail!\r\n");
        sys_drv_uninstall(DRV_GROUP_STORAGE);
    }

    libc_printf("#fsel_vfs_init():%08x\r\n", filesys_handle);

    return filesys_handle;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_vfs_deinit(int vfs_mount_hdl)
 * \注销已初始化的文件系统卷句柄
 * \Parameters:
 * \    [in] vfs_mount_hdl - 已加载的文件系统卷句柄
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:         该函数同时会卸载初始化时已加载的存储驱动
 */
/*******************************************************************************/
int fsel_vfs_deinit(uint32 vfs_mount_hdl)
{
    int retval = 0;

    libc_printf("#fsel_vfs_deinit():%08x\r\n", vfs_mount_hdl);

    if (0 == vfs_mount_hdl)
    {
        libc_printf("#fsel_vfs_deinit():vfs_mount_hdl==0!\r\n");
        return 0;
    }

    if (g_filesys_handle != 0)
    {
        sys_unmount_fs(vfs_mount_hdl);

        sys_drv_uninstall(DRV_GROUP_STORAGE);

        g_filesys_handle = 0;

        retval = 1;
    }
    else
    {
        libc_printf("#g_filesys_handle == 0!\r\n");
    }

    return retval;
}

