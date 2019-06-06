/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Ӧ�ô��̼���Ƿ����Զ������Խű��������������Զ������Գ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "ap_config.h"

const uint8 autotest_script[] =
{
    0xff, 0xfe,
    'a',  0x00,
    'c',  0x00,
    't',  0x00,
    'i',  0x00,
    'o',  0x00,
    'n',  0x00,
    's',  0x00,
    't',  0x00,
    'e',  0x00,
    's',  0x00,
    't',  0x00,
    '_',  0x00,
    '2',  0x00,
    '8',  0x00,
    '1',  0x00,
    'b',  0x00,
    '.',  0x00,
    'a',  0x00,
    't',  0x00,
    'f',  0x00,
    0x00, 0x00
};

/******************************************************************************/
/*!
 * \par  Description:
 *  ����Ƿ���Ҫ�������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_autotest_entry(uint8 disk_type)
{
    app_result_e result = RESULT_NULL;
    uint32 fhandle;

    fhandle = com_file_open(disk_type, (uint8 *) autotest_script, R_NORMAL_SEEK);
    if (fhandle != 0)
    {
        com_file_close(fhandle);
        result = RESULT_TESTAP_CARD_ENTER;
    }

    return result;
}

