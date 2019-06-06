/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2013-03-06 9:42     1.0             build this file
 *******************************************************************************/

#include "common_record_inner.h"

#if (SUPPORT_RECORD_FUNC == 1)

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    �����ַ���תunicode�ַ���
 * \param[in]   src��Դ�ַ���ָ��
 * \param[in]   len��Ҫת���������ַ�������
 * \param[out]  dest��Ŀ���ַ���ָ��
 * \return      �Ƿ�ת���ɹ�
 * \retval      TRUE ת���ɹ�
 * \retval      FALSE ת��ʧ��
 * \ingroup     string
 * \note        Ŀ���ַ������������û���֤�����������������С��Ҫ���Ͻ�������
 *******************************************************************************/
bool char_to_unicode(uint8 *dest, uint8 *src, uint16 len)
{
    int s_cnt = 0;
    int d_cnt = 0;
    uint16 font_code;

    while (1)
    {
        if ((s_cnt >= len) || (src[s_cnt] == 0))//��β���߽�����
        {
            break;
        }

        if ((uint8) src[s_cnt] >= 0x80)
        {
            font_code = 0x3f;
        }
        else
        {
            font_code = src[s_cnt];
        }
        s_cnt++;

        dest[d_cnt] = *((uint8*) &font_code); //���ֽ�
        d_cnt++;
        dest[d_cnt] = *((uint8*) &font_code + 1); //���ֽ�
        d_cnt++;
    }
    dest[d_cnt] = 0;
    d_cnt++;
    dest[d_cnt] = 0;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����������ļ���ת��ΪUnicode�����ļ�����
 * \param[in]    file_name ��������ļ���
 * \param[out]   file_name ���Unicode�����ļ���
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1������������ļ���ת��ΪUnicode�����ļ���
 const char const_rec_name[] = "rec_001.wav";
 uint8 new_rec_name[26];

 libc_memcpy(new_rec_name, const_rec_name, 12);
 com_ansi_to_unicode(new_rec_name);
 //���ţ��Ϳ���ʹ�� new_rec_name ����������Ϊ rec_001.wav ���ļ���
 * \endcode
 * \note
 * \li  ���ڴ����ļ�ǰ���ļ���ת��ΪUnicode���룬����Ӧ exFat �ļ�ϵͳ��
 * \li  �û����Լ���֤ file_name �����Unicode�����ļ��������� Ӣ�������ļ���
 *      rec_001.wav��ת��Unicode�����ļ�������Ҫ��������СΪ 26 ���ֽڣ���
 *      Unicode�����־�ַ�0xfffe 2�ֽ� + 11���ַ� 2 * 11�ֽ� + ������0x0 0x0 2�ֽڡ�
 * \li  �����ڸ�����������С��52�ֽڣ����ļ������ܳ��� 24 ���ַ����ȡ�
 *******************************************************************************/
void com_ansi_to_unicode(uint8 *file_name)
{
    uint8 tmp_buffer[32];
    uint16 ansi_len = (uint16) libc_strlen(file_name);
    uint16 i;

    //�����ƶ�2�ֽڣ�ǰ����� 0xfffe
    for (i = ansi_len; i > 0; i--)
    {
        file_name[(i - 1) + 2] = file_name[i - 1];
    }
    file_name[0] = 0xff;
    file_name[1] = 0xfe;
    file_name[ansi_len + 2] = 0x0;

    //���ļ���ת��ΪUnicode���� com_name_buffer
    char_to_unicode(tmp_buffer, file_name + 2, ansi_len);

    //����Unicode����
    libc_memcpy(file_name + 2, tmp_buffer, ansi_len * 2 + 2);
}

/*! \endcond */

#endif
