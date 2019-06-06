/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������ÿ���ꡣ
 * ���ߣ�
 ********************************************************************************/


#include  "ap_udevice.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ת���ļ����������ļ���ת��Ϊunicode�ַ�
 * \param[in]    src_name Դ�ļ��������� UTF8
 * \param[in]   dest_name Ŀ���ļ��������� UNICODE16
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
        else if ((in_val & (0xff << 5)) == 0xc0) //��2bit �Ƿ�Ϊ1
        {
            t1 = (uint8)(in_val & (0xff >> 3));//ȡ��5bit
            src_name++;
            in_val = *src_name;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//ȡ��6bit
            //(t1�ĵ�2bit ���������bit,����t2�ĵ�6bit)
            *dest_name = (uint8)(t2 | ((t1 & (0xff >> 6)) << 6));
            *(dest_name + 1) = t1 >> 2;//ȡ���м�3bit
        }
        else if ((in_val & (0xff << 4)) == 0xe0)
        {
            t1 = (uint8)(in_val & (0xff >> 3));//ȡ��5bit
            src_name++;
            in_val = *src_name;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//ȡ��6bit
            src_name++;
            in_val = *src_name;
            if (in_val == 0)
            {
                break;
            }
            t3 = (uint8)(in_val & (0xff >> 2));//ȡ��6bit

            //Little Endian
            //(t2��2bit �������2bit,����t3�ĵ�6bit);
            *dest_name = (uint8)(((t2 & (0xff >> 6)) << 6) | t3);
            //(t1 �ĵ�4bit�����ֽڵĸ�4bit,����t2���м�4bit)
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

    uint8 volume_label_src[12]; //���5�����֣�11��Ӣ���ַ�

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

    //���������ȡ�������,���֧��11��Ӣ���ַ�
    com_get_config_struct(INF_UDISK_LABEL, volume_label_src, 12);

    //����ת���ɳ�����ʽ
    _convert_string(volume_label_src, volume_label_dst, 11);

    //ͳһ�ɳ������������
    ret_val = vfs_create_volume(file_sys_id, volume_label_dst);

    com_file_close(0);

    return ret_val;
}

