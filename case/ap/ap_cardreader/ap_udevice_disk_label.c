/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：设置卡卷标。
 * 作者：
 ********************************************************************************/


#include  "ap_udevice.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    转换文件名，将长文件名转化为unicode字符
 * \param[in]    src_name 源文件名缓冲区 UTF8
 * \param[in]   dest_name 目标文件名缓冲区 UNICODE16
 * \return       none
 * \ingroup      common
 * \note
 *******************************************************************************/
void _convert_string(const uint8 * src_name, uint8 * dest_name, uint8 char_max)
{
    uint8 char_count = 0;
    uint8 in_val;
    uint8 t1, t2, t3;
    

    *dest_name = 0xff;
    *(dest_name + 1) = 0xfe;
    dest_name += 2;

    while (*src_name != '\0')
    {
        in_val = *src_name;
        if ((in_val > 0) && (in_val <= 0x7f))
        {
            *dest_name = in_val;
            *(dest_name + 1) = 0;
        }
        else if ((in_val & (0xff << 5)) == 0xc0) //高2bit 是否为1
        {
            t1 = (uint8)(in_val & (0xff >> 3));//取低5bit
            src_name++;
            in_val = *src_name;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//取低6bit
            //(t1的低2bit 放在最高两bit,或上t2的低6bit)
            *dest_name = (uint8)(t2 | ((t1 & (0xff >> 6)) << 6));
            *(dest_name + 1) = t1 >> 2;//取低中间3bit
        }
        else if ((in_val & (0xff << 4)) == 0xe0)
        {
            t1 = (uint8)(in_val & (0xff >> 3));//取低5bit
            src_name++;
            in_val = *src_name;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//取低6bit
            src_name++;
            in_val = *src_name;
            if (in_val == 0)
            {
                break;
            }
            t3 = (uint8)(in_val & (0xff >> 2));//取低6bit

            //Little Endian
            //(t2低2bit 放在最高2bit,或上t3的低6bit);
            *dest_name = (uint8)(((t2 & (0xff >> 6)) << 6) | t3);
            //(t1 的低4bit放在字节的高4bit,或上t2的中间4bit)
            *(dest_name + 1) = (t1 << 4) | (t2 >> 2);
        }
        else
        {
            //do nothing for QAC
        }
        src_name++;
        dest_name += 2;

        char_count++;
        if (char_count >= char_max)
        {
            break;
        }
    }

    *dest_name = 0x00;
    *(dest_name + 1) = 0x00;

    return;
}

/******************************************************************************/
/*!
 * \par  Description:_config_disk_label
 * \volume set
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool set_disk_label(void)
{
    bool ret_val;
    uint32 file_sys_id;

    uint8 volume_label_src[12]; //最多5个汉字，11个英文字符

    uint8 volume_label_dst[28];

    if (sys_card_detect() == FALSE)
    {
        return FALSE;
    }

    libc_memset(volume_label_src, 0, sizeof(volume_label_src));

    libc_memset(volume_label_dst, 0, sizeof(volume_label_dst));

    file_sys_id = com_file_open(DISK_H, NULL, 0);
    if (file_sys_id == 0)
    {
        return FALSE;
    }

    //从配置项读取卷标名字,最多支持11个英文字符
    com_get_config_struct(INF_UDISK_LABEL, volume_label_src, 12);

    //短名转换成长名形式
    _convert_string(volume_label_src, volume_label_dst, 11);

    //统一由长名创建卷标名
    ret_val = vfs_create_volume(file_sys_id, volume_label_dst);

    com_file_close(0);

    return ret_val;
}

