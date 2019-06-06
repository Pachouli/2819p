/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ��ǰ̨Ӧ�õ绰����������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

uint8 g_pb_result;
uint8 g_pb_len;

void * pb_tts_play_handle;

uint8 tts_info_buf[MAX_TTS_INFO_LENGTH];

#ifdef SUPPORT_PB_NAME_PLAY
const uint8 pb_info_ext[] = { "  ���绰��" };
#endif

/******************************************************************************
* \par  Description:   ����绰�������ֵĵ绰��Ϣ
* \param[in]    str_info :��Ҫת�����ַ�������
* \param[out]   null
* \return       bool the result
* \retval           1 sucess
* \retval           0 failed
*   <author>    <time>
*    Wekan   2015-3-27
*******************************************************************************/
#ifdef SUPPORT_PB_NAME_PLAY
bool btcall_pb_tts_deal_name(uint8 *str_info)
{
    bool ret;

    utf8_to_unicode(str_info);

    ret = unicode_to_char(str_info);

    return ret;
}
#endif

/******************************************************************************
* \par  Description:   ����绰������Ϣ
* \param[in]   str_info :��Ҫת�����ַ�������
* \param[out]   null
* \return       bool the result
* \retval           1 sucess
* \retval           0 failed
*   <author>    <time>
*    Wekan   2015-3-27
*******************************************************************************/

uint8 btcall_pb_tts_deal_num(uint8 *str_info)
{
    uint8 *p_str_info_tts = str_info;
    uint8 pb_num;
    uint8 pb_num_len = 0;

    while (1)
    {
        pb_num = *str_info;
        str_info++;
        //���� or����ascill��
        if ((pb_num == 0x0) || (pb_num > 0x80))
        {
            break;
        }
        //������0-9֮����ڵ�tts�λ���buf�С�
        if ((pb_num >= 0x30) && (pb_num <= 0x39))
        {
            *p_str_info_tts = (TTS_SECTION_NUM_0 + pb_num - 0x30);
            p_str_info_tts++;
            pb_num_len++;
        }

        if (pb_num_len >= PB_MAX_NUM_LEN)
        {
            break;
        }
    }

    return pb_num_len;
}

/******************************************************************************
* \par  Description:   ����绰��������������Ϣ
* \param[in]   null
* \param[out]   null
* \return       bool the result
* \retval           1 sucess
* \retval           0 failed
*   <author>    <time>
*    Wekan   2015-3-27
*******************************************************************************/

bool btcall_pb_tts_init(void)
{
    msg_apps_t msg;
    pb_data_t pb_content;
    bool ret = FALSE;

    libc_memset(&pb_content, 0x00, sizeof(pb_data_t));
    pb_content.phonebuf = tts_info_buf;
    msg.content.addr = &pb_content;
    msg.type = MSG_BTENGINE_GET_PB_CONTENT_SYNC;

    send_sync_msg(APP_ID_BTENGINE, &msg, 0, 0);

    g_pb_result = pb_content.phone_status;

#ifdef SUPPORT_PB_NAME_PLAY
    if (g_pb_result == PB_NAME_VALID)
    {
        //��������Ϣ���м��
        libc_memcpy(tts_info_buf, (pb_data_t *)(pb_data_reply.content)->phonebuf, PB_MAX_NAME_LEN);
        if (btcall_pb_tts_deal_name(tts_info_buf) == FALSE)
        {//����ת�����ʧ�ܣ�ֻ�����绰����
            g_pb_result = PB_NUM_VALID;
        }
        else
        {
            str_len = (uint8) libc_strlen(tts_info_buf);
            //�绰���� or name+���绰��
            libc_memcpy(&tts_info_buf[str_len], pb_info_ext, sizeof(pb_info_ext));

            ret = TRUE;
        }
    }
#endif

    if (g_pb_result == PB_NUM_VALID)
    {
        //�Ե绰������м��
        g_pb_len = btcall_pb_tts_deal_num(tts_info_buf);
        if (g_pb_len > 0)
        {
            ret = TRUE;
        }
    }

    if (ret == FALSE)
    {
        libc_printf_info("pb tts init fail!\n");
    }

    return ret;
}
/******************************************************************************
* \par  Description:   �����绰����Ϣ����
* \param[in]   null
* \param[out]   null
* \return      null
*   <author>    <time>
*    Wekan   2015-3-27
*******************************************************************************/
void btcall_pb_tts_play(void)
{
    if ((g_pb_result == PB_NUM_VALID) && (g_pb_len > 0))
    {
        //��ʼ���ŵ绰����
        pb_tts_play_handle = com_sd_mp3_player_play_tts(tts_info_buf, g_pb_len);
        if (pb_tts_play_handle == NULL)
        {
            libc_printf_error("pb tts play fail!\n");
            while (1);
        }

        libc_printf_info("pb tts play\n");
    }
}

void btcall_pb_tts_stop(void)
{
    com_sd_mp3_player_stop(pb_tts_play_handle);
    pb_tts_play_handle = NULL;

    libc_printf_info("pb tts stop\n");
}

bool btcall_pb_tts_is_over(void)
{
    return com_sd_mp3_player_is_stop(pb_tts_play_handle);
}

