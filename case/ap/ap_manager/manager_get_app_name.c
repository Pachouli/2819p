/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用管理器所管理的所有应用的映像文件名字列表，名字必须是8+.+ap的格式。
 * 作者：cailizhen
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
 *    根据ap的ID，获取ap的名称，系统创建ap的接口需要用到ap的名称
 * \param[in]    char *namebuf，存放应用名称的buffer地址
 * \param[in]    uint8 ap_id，应用ID
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

