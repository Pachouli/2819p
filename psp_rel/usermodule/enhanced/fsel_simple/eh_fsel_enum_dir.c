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
 �̶�"����"
 */
static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

static const uint8 rec_dir_name[] = "RECORD  ";
static const uint8 alarm_dir_name[] = "ALARM   ";

/*
 ȫ�ֱ���
 */
static uint8 g_filesys_disk;
static uint32 g_filesys_handle;

static uint32 g_music_bitmap;
static uint32 g_dir_bitmap;

/*
 �ڲ���������
 */
static bool filter_record_dir(uint32 vfs_mount_hdl);
static int get_music_num_by_offset(uint32 vfs_mount_hdl, uint32 cluster_no, uint32 dir_entry);
static int fsel_enum_dir_cmd(uint32 vfs_mount_hdl, uint8 dir_cmd, uint32 file_type_mask, DIRITEM *p_dir_item);

/*
 ����¼��/����Ŀ¼
 ����Դ��bool filtrate_dir(void)����
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
        //8+3 ����
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
 * \���������ļ���������λ�ü���ѡ�������ڴ��̵������
 * \Parameters:
 * \      [in]    vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ���������
 * \      [in]    cluster_no, dir_entry - ѡ���ļ�λ�ڴ��̵ľ���λ��
 * \Return:       int the result
 * \           0 failed
 * \           other value is music file number
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:        �ڲ�����
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
    uint8 search_flag = 0; // �����׶α�־��0��ʾ��ǰ�������ļ���1��ʾ����Ŀ¼

    libc_printf("#get_file_num_by_entry() enter\r\n");

    // ����pdir_layer_t���ڴ�
    p_dir_layer = sys_malloc(sizeof(pdir_layer_t));
    if (NULL == p_dir_layer)
    {
        libc_printf("#get_file_num_by_entry() : malloc pdir_layer_t fail!\r\n");
        return 0;
    }

    // ���浱ǰvfs_mount_hdl
    retval = vfs_file_dir_offset(vfs_mount_hdl, p_dir_layer->buffer, old_dir_offset, 0);
    if (0 == retval)
    {
        libc_printf("#get_file_num_by_entry() : get offset fail!\r\n");
        sys_free(p_dir_layer);
        return 0;
    }

    // �Ӹ�Ŀ¼��ʼѰ��
    vfs_cd(vfs_mount_hdl, CD_ROOT, NULL);
    vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);

    file_type_mask = g_music_bitmap;

    // ѭ���������������ļ���������Ŀ¼
    do
    {
        retval = vfs_dir(vfs_mount_hdl, DIR_NEXT, NULL, file_type_mask);
        if (retval != 0)
        {
            attr = vfs_file_attralter(vfs_mount_hdl, 0, NULL, 1);

            if (attr & ATTR_DIRECTORY)
            {
                //libc_printf("##ATTR_DIRECTORY\r\n");

                if ((dir_layer == 0) && (eh_fsel_filter_record_dir_flag == TRUE)) //ֻ����˸�Ŀ¼�µ�RECORD��ALARMĿ¼
                {
                    if (filter_record_dir(vfs_mount_hdl) == TRUE)
                    {
                        libc_printf("#filter record dir!\n");
                        retval = 1;
                        continue; //�����˵��ˣ���Ҫ����������һ��Ŀ¼
                    }
                }

                retval = vfs_cd(vfs_mount_hdl, CD_SUB, NULL); // ���뵱ǰĿ¼
                if (retval != 1) // ����ʧ���п����ǽ����˳���8����Ŀ¼��20180907
                {
                    libc_printf("#vfs_cd() fail!%d\r\n", dir_layer); // ����ʱ��ӡ��: dir_layer == 8 , by jc0245
                    retval = 0;
                    break;
                }

                // ����Ŀ¼��ʼ�����ļ�
                dir_layer++;

                search_flag = 0;
                file_type_mask = g_music_bitmap;

            }
            else // �ļ�����Ŀ¼
            {
                cur_num++;

                // �Աȼ��
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
        else // ʧ�ܣ��˻ص���һ��Ŀ¼��������ʱ���ض��ڼ����ļ�����
        {
            if (search_flag == 0) // �����ļ���������Ŀ¼
            {
                if (dir_layer < 8) // С��8��Ŀ¼����������Ŀ¼
                {
                    search_flag = 1;
                    file_type_mask = g_dir_bitmap;
                    retval = 1;

                    vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
                    continue;
                }
                else // ���ڵ���8��Ŀ¼�����ܱ���Ŀ¼��
                {
                    // �������Ҳ��ʧ�ܵģ�ʧ��֮��ᵼ�º���ĸ�������ȡ������ȷ�����
                    libc_printf("#serach_flag==0 && dir_layer >= 8!\r\n");
                }
            }

            if (dir_layer == 0) // ����Ŀ¼�ˣ�����û����
            {
                libc_printf("#dir_layer == 0!!!\r\n");
                break;
            }

            retval = vfs_cd(vfs_mount_hdl, CD_BACK, NULL); // �˻ص���һ��Ŀ¼, �˻غ���ļ�λ��???
            if (retval != 0) // 0-ʧ�ܣ�1-�ɹ�
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

    // �ָ����
    vfs_file_dir_offset(vfs_mount_hdl, p_dir_layer->buffer, old_dir_offset, 1);

    // �ͷ���Դ
    sys_free(p_dir_layer);

    if (0 == retval) // ʧ��
    {
        cur_num = 0;
    }

    return cur_num;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int fsel_enum_dir_first(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item)
 * \�ļ�ϵͳ����Ŀ¼�ӿ������װ
 * \Parameters:
 * \      [in]    vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ����
 * \      [in]    dir_cmd - DIR����: DIR_HEAD��DIR_TAIL��DIR_PREV��DIR_NEXT
 * \      [in]    file_type_mask - �ļ����͹��ˣ��μ�vfs_interface.h�е�"DIR ��չ����ӳ��"����
 * \      [out]  p_dir_item - ���Ŀ¼��ṹָ�룬����μ�DIRITEM�ṹ˵��
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       �ڲ�����
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
        len = vfs_get_name(vfs_mount_hdl, p_dir_item->name_buf, (sizeof(p_dir_item->name_buf) / 2)); // ע������longname_length����Ϊ2�ֽڵ�λ!
        if (len != 0) // lenҪô����2��Ҫô����11
        {
            p_dir_item->attr = vfs_file_attralter(vfs_mount_hdl, 0, NULL, 1);

            if ((p_dir_item->name_buf[0] == 0xff) && (p_dir_item->name_buf[1] == 0xfe))
            {
                len = len * 2; // ���ļ�������ʵ���ַ�������������ͷ�ͽ�β��־
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
 * \��ʼ��λҪö�ٵ�Ŀ¼
 * \Parameters:
 * \      [in]    vfs_mount_hdl  - �Ѽ��ص��ļ�ϵͳ����
 * \      [in]    dir_name - Ŀ¼���ƻ��壬�������Ʋμ������note
 * \      [in]    path_len - Ŀ¼���ƻ��峤�ȣ��ֽڵ�λ
 * \Return:     int the result
 * \    1 sucess
 * \    0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:        ����Ŀ¼����ת��:
 * \                    "/" - ��ʾ��λ����Ŀ¼��ʼö�٣�dir_name[]={'/',0}
 * \                    ".." - ��ʾ��λ����һ��Ŀ¼��dir_name[]={'.','.',0}
 * \                    "."  -��ʾ��λ����ǰĿ¼��ͷ��dir_name[]={'.',0}
 * \            �������Ʊ�ʾ8.3��ʽĿ¼���ƻ�UNICODE��ʽĿ¼����
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

    if (path_len >= 4) // ������ǰĿ¼�ڵ�����
    {
        retval = vfs_cd(vfs_mount_hdl, CD_SUB, dir_name);

        //retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
    }
    else if (3 == path_len)
    {
        if ((dir_name[0] == '.') && (dir_name[1] == '.') && (dir_name[2] == 0)) // ������һ��Ŀ¼
        {
            retval = vfs_cd(vfs_mount_hdl, CD_BACK, NULL);
        }
    }
    else if (2 == path_len)
    {
        if ((dir_name[0] == '/') && (dir_name[1] == 0)) // �Ӹ�Ŀ¼��ʼ����
        {
            retval = vfs_cd(vfs_mount_hdl, CD_ROOT, NULL);
            retval = vfs_cd(vfs_mount_hdl, CD_HEAD, NULL);
        }
        else if ((dir_name[0] == '.') && (dir_name[1] == 0)) // ��ͷ��ʼ������ǰĿ¼
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
 * \ѡ��ǰĿ¼��һ��ָ�����͵��ļ�(��)��������Ŀ¼����Ϣ
 * \Parameters:
 * \      [in]    vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ����
 * \      [in]    file_type_mask - �ļ����͹��ˣ��μ�vfs_interface.h�е�"DIR ��չ����ӳ��"����
 * \      [out]  p_dir_item - ���Ŀ¼��ṹָ�룬����μ�DIRITEM�ṹ˵��
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) ��DIRITEM�ṹ�е�name_len��Ա�����ļ�������11�����ļ�������ʵ���ֽ���(������ͷ��β)
 * \               (2) �û����ڻ�ȡ�����ļ�������Ҫ����ת��Ϊ��������
 * \               (3) ֮�������fsel_enum_dir_next()��������
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
 * \ѡ��ǰĿ¼���һ��ָ�����͵��ļ�(��)��������Ŀ¼����Ϣ
 * \Parameters:
 * \      [in]    vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ����
 * \      [in]    file_type_mask - �ļ����͹��ˣ��μ�vfs_interface.h�е�"DIR ��չ����ӳ��"����
 * \      [out]  p_dir_item - ���Ŀ¼��ṹָ�룬����μ�DIRITEM�ṹ˵��
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) ��DIRITEM�ṹ�е�name_len��Ա�����ļ�������11�����ļ�������ʵ���ֽ���(������ͷ��β)
 * \               (2) �û����ڻ�ȡ�����ļ�������Ҫ����ת��Ϊ��������
 * \               (3) ֮�������fsel_enum_dir_prev()��������
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
 * \ѡ��ǰĿ¼��ָ���ļ�����һ���ļ�(��)��������Ŀ¼����Ϣ
 * \Parameters:
 * \      [in]    vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ����
 * \      [in]    file_type_mask - �ļ����͹��ˣ��μ�vfs_interface.h�е�"DIR ��չ����ӳ��"����
 * \      [out]  p_dir_item - ���Ŀ¼��ṹָ�룬����μ�DIRITEM�ṹ˵��
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) ��DIRITEM�ṹ�е�name_len��Ա�����ļ�������11�����ļ�������ʵ���ֽ���(������ͷ��β)
 * \               (2) �û����ڻ�ȡ�����ļ�������Ҫ����ת��Ϊ��������
 * \               (3) �����øú���ʧ��ʱ�������fsel_enum_dir_set(vfs_mount_hdl,".",2)��ͷ��ʼ����
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
 * \ѡ��ǰĿ¼��ָ���ļ�����һ���ļ�(��)��������Ŀ¼����Ϣ
 * \Parameters:
 * \    [in] vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ����
 * \    [in] file_type_mask - �ļ����͹��ˣ��μ�vfs_interface.h�е�"DIR ��չ����ӳ��"����
 * \    [out] p_dir_item - ���Ŀ¼��ṹָ�룬����μ�DIRITEM�ṹ˵��
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:       (1) DIRITEM�ṹ�е�name_len��Ա�����ļ�������11�����ļ�������ʵ���ֽ���(������ͷ��β)
 * \               (2) �û����ڻ�ȡ�����ļ�������Ҫ����ת��Ϊ��������
 * \               (3) �����øú���ʧ��ʱ�������fsel_enum_dir_prev()����ָ��ʧ��ǰѡ����ļ�
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
 * \��ȡ��ǰ����ѡ���ļ�/�ļ��е�λ����Ϣ
 * \Parameters:
 * \      [in]    vfs_mount_hdl - �ѹ��ص��ļ�ϵͳ����
 * \      [out]    p_file_loc - �ļ�λ����Ϣ�ṹ
 * \Return:       int the result
 * \           1 success
 * \           0 failure
 * \Ingroup:       eh_fsel_enum_dir.c
 * \Note:           �ú����ɻ�ȡѡ���ļ�λ����Ϣ�����ں���ָ������
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

    // ��ȡ��չ��
    len = vfs_get_name(vfs_mount_hdl, p_file_loc->filename, 0);
    if (len == 0)
    {
        libc_printf("#get extname fail!\r\n");
        return 0;
    }

    // ��ȡ��ǰѡ���ļ�·����Ϣ
    ret_val = vfs_file_dir_offset(vfs_mount_hdl, p_file_loc->dir_layer_info.buffer, &p_file_loc->cluster_no, 0);
    if (ret_val == 0)
    {
        libc_printf("#get offset fail!\r\n");
        return 0;
    }

    p_file_loc->dir_layer = p_file_loc->dir_layer_info.buffer[72];

    p_file_loc->disk = g_filesys_disk;

    // ��ʹ��MSG_MENGINE_SET_FILEPATH_SYNCѡ���ļ�����ʱ
    // ����������������fsel_dir_set_location�л��и�ֵ
    // ������ﲻ��Ҫ��ֵ������㸳����ֵ
    p_file_loc->file_total = 0;
    p_file_loc->file_total_disk = 0;

    // ��ȡѡ�������ļ��ڴ������
    p_file_loc->file_num = get_music_num_by_offset(vfs_mount_hdl, p_file_loc->cluster_no, p_file_loc->dir_entry);
    if (p_file_loc->file_num == 0) // ��ȡʧ����
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
 * \��ʼ��һ���ļ�ϵͳ����
 * \Parameters:
 * \      [in]    disk - �洢�豸��ʶ����ΪDISK_H(TF��)��DISK_U(U��)
 * \Return:       uint32 the result
 * \           0 failed
 * \           other value represent a filesystem handle
 * \Ingroup:       eh_fsel_enum_dir.c
 * \Note:           281X���֧�������ļ�ϵͳ��
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

    // װ�ش�������
    if (sys_drv_install(DRV_GROUP_STORAGE, 0, drv_name) != 0) // SLOW_DRIVER
    {
        libc_printf("#fsel_vfs_init() : install storage drv fail!\r\n");
        return 0;
    }

    // װ���ļ�ϵͳ����
    filesys_handle = sys_mount_fs(DRV_GROUP_STORAGE, disk, 0);
    if (filesys_handle != 0)
    {
        g_filesys_handle = filesys_handle;

        g_filesys_disk = disk;
    }
    else // װ��ʧ��
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
 * \ע���ѳ�ʼ�����ļ�ϵͳ����
 * \Parameters:
 * \    [in] vfs_mount_hdl - �Ѽ��ص��ļ�ϵͳ����
 * \Return:       int the result
 * \           1 sucess
 * \           0 failed
 * \Ingroup:      eh_fsel_enum_dir.c
 * \Note:         �ú���ͬʱ��ж�س�ʼ��ʱ�Ѽ��صĴ洢����
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

