/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_file.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *              leiming    2011-10-27          v1.0                 create this file
 ********************************************************************************/

#include "common_record_inner.h"

#if (SUPPORT_RECORD_FUNC == 1)

static const uint8  g_rec_folder_short[] = "RECORD     ";
static void _record_file_scan_file_number(void);

static const uint8 g_rec_folder_long[] =
{
    0xff, 0xfe,  //flag
    'R',  0x00,
    'E',  0x00,
    'C',  0x00,
    'O',  0x00,
    'R',  0x00,
    'D',  0x00,
    0x00, 0x00  //end flag
};

//¼���ļ���
static const uint8 rec_filehead[4] = "REC";

//¼����׺��
static const uint8 rec_ext[][4] =
{ "WAV", "MP3" }; //wav��ʽ��MP3��ʽ


uint8 _record_save_file_info_to_vram(void)
{
    libc_memset(&(p_record_file_info->file_path), 0, sizeof(record_file_path_t));
    if (!vfs_file_dir_offset(p_record_file_info->fs_param.vfs_handle, &(p_record_file_info->file_path.ptr_layer),
            &(p_record_file_info->file_path.ptr_file_offset), 0))
    {
        return REC_FS_ERR_SCENCE_GET_FILE_INFO_ERR;
    }

    //wav�ļ���дͷ�Ľṹ
    libc_memcpy(p_record_file_info->file_path.rec_filename, p_record_file_info->fs_param.rec_filename,
            sizeof(p_record_file_info->fs_param.rec_filename));

    rec_file_recover_save_fs_vram(&(p_record_file_info->file_path), p_record_file_info->disk_type);

    return REC_FS_ERR_NONE;
}

static bool _record_set_path(void)
{
    bool dir_exsit = FALSE;

    //��λ����Ŀ¼
    if (!vfs_cd(p_record_file_info->fs_param.vfs_handle, CD_ROOT, 0))
    {
        libc_printf("cd root fail\n");
        return REC_FS_ERR_SCENCE_PATH_SET_ERR;
    }

    //recordĿ¼�Ƿ����
    if (vfs_cd(p_record_file_info->fs_param.vfs_handle, CD_SUB, (uint8 * )g_rec_folder_short))
    {
        dir_exsit = TRUE;
    }
    else
    {
        if (vfs_cd(p_record_file_info->fs_param.vfs_handle, CD_SUB, (uint8 * ) g_rec_folder_long))
        {
            dir_exsit = TRUE;
        }
    }

    //recordĿ¼�������ڣ�����RECORDĿ¼
    if (dir_exsit == FALSE)
    {
        libc_printf("dir not exits\n");
        if (!vfs_make_dir(p_record_file_info->fs_param.vfs_handle, (uint8* ) g_rec_folder_long))
        {
            return REC_FS_ERR_SCENCE_DIR_CREATE_ERR;
        }

        if (!vfs_cd(p_record_file_info->fs_param.vfs_handle, CD_SUB, (uint8 * ) g_rec_folder_long))
        {
            if (!vfs_cd(p_record_file_info->fs_param.vfs_handle, CD_SUB, (uint8 * )g_rec_folder_short))
            {
                return REC_FS_ERR_SCENCE_DIR_CD_ERR;
            }
        }
    }

    return REC_FS_ERR_NONE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _parse_total_num(void)
 * \���㵱ǰĿ¼�µ���¼���ļ�����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static bool _record_parse_filenum(void)
{
    //cur number
    p_record_file_info->rec_num = p_record_file_info->file_path.file_num;

    return REC_FS_ERR_NONE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _record_format_filename(void)
 * \��ʽ��¼���ļ���
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       NULL
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static void _record_format_filename(void)
{
    //����buffer���
    //libc_memset(&p_record_file_info->fs_param.rec_filename, 0, 12);

    //¼���ļ�����Ϊ"RECxxx.xxx"
    libc_strncpy(p_record_file_info->fs_param.rec_filename, rec_filehead, sizeof(rec_filehead));
    //record filename
    libc_itoa(p_record_file_info->rec_num, &p_record_file_info->fs_param.rec_filename[3], 3);
    //�ļ����ӵ�
    p_record_file_info->fs_param.rec_filename[6] = '.';
    //¼����׺��
    // libc_memcpy(&p_record_file_info->fs_param.rec_filename[7], rec_ext[g_record_vars.file_type], 3);

    libc_memcpy(&p_record_file_info->fs_param.rec_filename[7], rec_ext[REC_FORMAT_WAV], 3);

    //REC_FORMAT_WAV
}

/******************************************************************************/
/*!
 * \par  Description:
 * \BOOL _record_generate_filename(void)
 * \����¼������ţ���001��ʼ����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       NULL
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static void _record_file_set_bit(char *rec_save_addr, uint16 rec_file_num)
{
    uint8 number_left_shifts = 0;
    uint16 addr_jump_number = 0;
    addr_jump_number = (rec_file_num - 1) / 8;
    number_left_shifts = (rec_file_num - 1) % 8;

    rec_save_addr[addr_jump_number] |= (1 << number_left_shifts);
}

static bool _record_file_get_bit(char *rec_save_addr, uint16 rec_file_num)
{
    uint8 number_left_shifts = 0;
    uint16 addr_jump_number = 0;
    addr_jump_number = (rec_file_num - 1) / 8;
    number_left_shifts = (rec_file_num - 1) % 8;

    if (rec_save_addr[addr_jump_number] & (1 << number_left_shifts))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static bool _record_generate_filename(void)
{
    uint16 find_num;
    uint8 new_name[12];
    uint8 num_buf[4];
    int16 i;

    //���Ƶ�new_name
    libc_memcpy(new_name, p_record_file_info->fs_param.rec_filename, 12);

    for (i = FILE_NUM_MAX; i >= 1; i--)
    {
        if (_record_file_get_bit(p_record_file_info->record_file_bitmap_addr, i) == TRUE)
        {
            break;
        }
    }

    if (i == 0)
    {
        i++;
    }
    else if (i == FILE_NUM_MAX)
    {

        for (i = 1; i <= FILE_NUM_MAX; i++)
        {
            if (_record_file_get_bit(p_record_file_info->record_file_bitmap_addr, i) == FALSE)
            {
                break;
            }
        }
        if (i == (FILE_NUM_MAX + 1))
        {
            return FALSE;
        }
    }
    else
    {
        i++;
    }

    find_num = i;

    p_record_file_info->rec_num = find_num;

    p_record_file_info->file_path.file_num = p_record_file_info->rec_num;
    libc_itoa(find_num, num_buf, 3);

    libc_memcpy(&new_name[3], num_buf, 3);

    //��ǰ�ļ������ڣ��Ե�ǰ�ļ�����record�ļ�������
    libc_memcpy(p_record_file_info->fs_param.rec_filename, new_name, 11);

    libc_printf("rec_filename %s \n", p_record_file_info->fs_param.rec_filename);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_create_filename(void)
 * \����¼���ļ���
 * \param[in]    void
 * \param[out]   none
 * \return       BOOL
 * \retval       0:file num full; 1-gen file success
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static bool _record_create_filename(void)
{
    libc_memset(p_record_file_info->fs_param.rec_filename, 0, sizeof(p_record_file_info->fs_param.rec_filename)); //����ļ���buf

    //��ʽ���ļ���
    _record_format_filename();

    //�����ļ���
    if (!_record_generate_filename())
    {
        //return FALSE;
        return REC_FS_ERR_GEN_FILENAME_NG;
    }

    //ǰ���ʽ���ļ�����ʹ�ö���������
    com_ansi_to_unicode(p_record_file_info->fs_param.rec_filename);

    return REC_FS_ERR_NONE;
}

static bool _record_create_file(void)
{
    p_record_file_info->fs_param.fhandle = vfs_file_create(p_record_file_info->fs_param.vfs_handle,
            (uint8 * )p_record_file_info->fs_param.rec_filename, 0);
    if (!p_record_file_info->fs_param.fhandle)
    {
        return REC_FS_ERR_SCENCE_CREATE_FILE_ERR;
    }
    return REC_FS_ERR_NONE;
}

static bool _record_seek_file_head(void)
{
    if (!vfs_file_seek(p_record_file_info->fs_param.vfs_handle, 0, SEEK_SET, p_record_file_info->fs_param.fhandle))
    {
        libc_printf("REC_FS_SCENCE_SEEK_FILE_ERR\n");
        return REC_FS_SCENCE_SEEK_FILE_ERR;
    }
    return REC_FS_ERR_NONE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_file_form(void)
 * \¼��֮ǰ��Ŀ¼���飬���̿ռ���飬¼���ļ�����
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       false: ʧ�� true:�ɹ�
 * \ingroup      record_file.c
 * \notere
 */
/*******************************************************************************/
bool _record_file_first_create(void)
{
    uint16 file_bitmap_len = FILE_NUM_MAX / 8;
    if (FILE_NUM_MAX % 8 != 0)
    {
        file_bitmap_len++;
    }

    libc_printf("pre creat rec\n"); //for debug

    record_fs_err_type_e ret_err_type;

    ret_err_type = _record_set_path();
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        fsel_exit();
        return ret_err_type;
    }

    ret_err_type = _record_parse_filenum();
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        fsel_exit();
        return ret_err_type;
    }

    //����ѿռ�
    p_record_file_info->record_file_bitmap_addr = (char *) sys_malloc(file_bitmap_len);
    if (p_record_file_info->record_file_bitmap_addr == NULL)
    {
        libc_printf("file_bitmap_len\n");
        return REC_FS_ERR_OTHER;
    }
    libc_memset(p_record_file_info->record_file_bitmap_addr, 0, file_bitmap_len);
    _record_file_scan_file_number();

    /*���ɵ�ǰ¼���ļ���*/
    ret_err_type = _record_create_filename();

    sys_free(p_record_file_info->record_file_bitmap_addr);
    p_record_file_info->record_file_bitmap_addr = NULL;

    if (ret_err_type != REC_FS_ERR_NONE)
    {
        fsel_exit();
        return ret_err_type;
    }

    /*����¼���ļ�*/
    ret_err_type = _record_create_file();
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        fsel_exit();
        return ret_err_type;
    }

    ret_err_type = _record_seek_file_head();
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        _record_file_close();
        fsel_exit();
        return ret_err_type;
    }

    ret_err_type = _record_save_file_info_to_vram();
    if (ret_err_type != REC_FS_ERR_NONE)
    {
        _record_file_close();
        fsel_exit();
        return ret_err_type;
    }
    return REC_FS_ERR_NONE;
}

bool _record_file_close(void)
{
    uint32 file_sys_id;
    fhandle_t *file_handle;

    file_sys_id = p_record_file_info->fs_param.vfs_handle;
    if (!file_sys_id)
    {
        return FALSE;
    }
    file_handle = (fhandle_t *) p_record_file_info->fs_param.fhandle;
    if (file_handle == NULL || file_handle->mode == FHANDLE_FREE)
    {
        return FALSE;
    }
    libc_printf("file close...\n");
    if (vfs_file_close(file_sys_id, (uint32 )file_handle))
    {
        return TRUE;
    }

    libc_printf("rec_file close fail\n");
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ƥ��¼���ļ���
 *
 * \param[in]
 * \param[out]
 * \return          ¼���ļ����
 * \note
 ******************************************************************************/
static bool _record_file_match(uint16 name_len, uint8 *buf, uint16 *file_index)
{
    const uint8 rec_file_name[] = "REC";
    uint8 i;
    uint16 rec_file_num_temp = 0;
    bool is_unicode;
    bool match_flag = TRUE;

    is_unicode = FALSE;
    if ((buf[0] == 0xff) && (buf[1] == 0xfe))
    {
        is_unicode = TRUE;
    }

    if (is_unicode == FALSE)
    {
        //8+3 ����
        name_len = 8;

        if (libc_memcmp(buf, rec_file_name, 3) != 0)
        {
            match_flag = FALSE;
            return FALSE;
        }

        for (i = 0; i < 3; i++)
        {
            if (buf[3 + i] >= '0' && buf[3 + i] <= '9')
            {
                rec_file_num_temp = rec_file_num_temp * 10 + (buf[3 + i] - '0');
            }
            else
            {
                match_flag = FALSE;
                return FALSE;
            }
        }

        if (libc_memcmp(buf + 6, "  WAV", 6) != 0) //WAV��׺�ж�
        {
            match_flag = FALSE;
            return FALSE;
        }
    }
    else
    {
        if (((buf[2 + 0] == 'R') && (buf[2 + 1] == 0x00))
                && ((buf[4 + 0] == 'E') && (buf[4 + 1] == 0x00))
                && ((buf[6 + 0] == 'C') && (buf[6 + 1] == 0x00)))
        {

        }
        else
        {
            match_flag = FALSE;
            return FALSE;
        }

        for (i = 0; i < 3; i++)
        {
            if ((buf[8 + i * 2] >= '0' && buf[8 + i * 2] <= '9') && (buf[8 + i * 2 + 1] == 0x0))
            {
                rec_file_num_temp = rec_file_num_temp * 10 + (buf[8 + i * 2] - '0');
            }
            else
            {
                match_flag = FALSE;
                return FALSE;
            }
        }

        if (((buf[14 + 0] == '.') && (buf[14 + 1] == 0x00))
                && ((buf[16 + 0] == 'W') && (buf[16 + 1] == 0x00))
                && ((buf[18 + 0] == 'A') && (buf[18 + 1] == 0x00))
                && ((buf[20 + 0] == 'V') && (buf[20 + 1] == 0x00))
                && ((buf[22 + 0] == 0x00) && (buf[22 + 1] == 0x00)))
        {

        }
        else
        {
            match_flag = FALSE;
            return FALSE;
        }
    }

    *file_index = rec_file_num_temp;

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ��ǰĿ¼�µ��¸��ļ�
 *
 * \param[in]
 * \param[out]      strfile--����ļ�����չ��
 * \return          TRUE OR FALSE
 * \note
 ******************************************************************************/
static bool _record_file_next(uint8 dir_type)
{
    uint32 rec_file_type_bit = RECORD_BITMAP;
    uint16 name_len = 0;
    uint8 file_name_buffer[32];
    uint16 rec_file_num;

    //dir�ļ�
    if (vfs_dir(p_record_file_info->fs_param.vfs_handle, dir_type, NULL, rec_file_type_bit) != 0)
    {
        name_len = vfs_get_name(p_record_file_info->fs_param.vfs_handle, file_name_buffer, 16);
        if (_record_file_match(name_len, file_name_buffer, &rec_file_num) == TRUE)
        {
            _record_file_set_bit(p_record_file_info->record_file_bitmap_addr, rec_file_num);
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/*
 * \par  Description:ɨ��¼���ļ�
 *
 * \param[in]

 * \param[out]

 * \return:

 * \note
 *******************************************************************************/
static void _record_file_scan_file_number(void)
{
    uint8 dir_type;

    if (p_record_file_info->fs_param.vfs_handle == 0)
    {
        return;
    }

    //���Ϊ0���ͷ��ʼ����
    dir_type = DIR_HEAD;
    do
    {
        //scan disk for all file
        if (_record_file_next(dir_type) == FALSE)
        {
            break;
        }

        //��Ų�Ϊ0��dir��һ��
        dir_type = DIR_NEXT;
    }

    while (1);
}

/*! \endcond */

#endif
