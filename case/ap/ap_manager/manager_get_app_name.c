/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù����������������Ӧ�õ�ӳ���ļ������б����ֱ�����8+.+ap�ĸ�ʽ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "manager.h"

const char app_name_list[APP_ID_MAX][APP_NAME_MAX] =
{
    "music.ap", "linein.ap", "radio.ap", "btplay.ap", "btcall.ap", "usound.ap",
    "manager.ap", "config.ap", "udisk.ap",
    "mengine.ap", "lineineg.ap", "fmengine.ap", "btplayeg.ap", "btcalleg.ap", "usoundeg.ap",
    "btengine.ap", "upgrade.ap", "acttest.ap", "bthid.ap", "record.ap", "alarm.ap",
};

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ap��ID����ȡap�����ƣ�ϵͳ����ap�Ľӿ���Ҫ�õ�ap������
 * \param[in]    char *namebuf�����Ӧ�����Ƶ�buffer��ַ
 * \param[in]    uint8 ap_id��Ӧ��ID
 * \param[out]   none
 * \return       none
 * \retval
 * \note
 *******************************************************************************/
bool manager_get_app_name(char *namebuf, uint8 ap_id)
{
    if (ap_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    libc_memcpy(namebuf, app_name_list[ap_id], APP_NAME_MAX);

    return TRUE;
}

