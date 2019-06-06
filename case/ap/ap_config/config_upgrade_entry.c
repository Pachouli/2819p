/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开机应用磁盘检测是否有升级固件，如果有则进入升级程序。
 * 作者：cailizhen
 ********************************************************************************/

#include "ap_config.h"

const uint8 fw_file_name[] =
{
    0xff, 0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '.', 0x00,
    'H', 0x00,
    'E', 0x00,
    'X', 0x00,
    0x00, 0x00
};

const uint8 fwu_file_name[] =
{
    0xff, 0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '.', 0x00,
    'F', 0x00,
    'W', 0x00,
    'U', 0x00,
    0x00, 0x00
};

const uint8 fw_ascfile_name[] =
{ "UPGRADE HEX" };
const uint8 fwu_ascfile_name[] =
{ "UPGRADE FWU" };

/******************************************************************************/
/*!
 * \par  Description:
 *  检测是否需要进入测试
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_upgrade_entry(uint8 disk_type)
{
    app_result_e ret_val = RESULT_NULL;
    uint32 fhandle = 0;
    bool has_upgrade_fw = FALSE;
    uint32 fs_mount_id = 0;

    #if 1
    if ((get_cable_state()   != CABLE_STATE_CABLE_IN) &&
        (get_adaptor_state() != ADAPTOR_STATE_CABLE_IN))
    {
        bool is_lowpower = FALSE;
        /* 低电状态时不进行升级?
         */
        if (disk_type == DISK_U)
        {
            if (key_get_battery() <= 3)
            {
                is_lowpower = TRUE;
            }
        }
        else
        {
            if (key_get_battery() <= 1)
            {
                is_lowpower = TRUE;
            }
        }

        if (is_lowpower == TRUE)
        {
            log_warning("BAT_LOW");

            return RESULT_NULL;
        }
    }
    #endif

    do
    {
        fs_mount_id = com_file_open(disk_type, NULL, 0);

        if (fs_mount_id == 0)
            break;

        vfs_cd(fs_mount_id, CD_ROOT, 0);

        fhandle = vfs_file_open(fs_mount_id, (uint8 *) fw_file_name, R_NORMAL_SEEK);
        if (fhandle != 0)
        {
            has_upgrade_fw = TRUE;
            break;
        }

        fhandle = vfs_file_open(fs_mount_id, (uint8 *) fwu_file_name, R_NORMAL_SEEK);
        if (fhandle != 0)
        {
            has_upgrade_fw = TRUE;
            break;
        }

        fhandle = vfs_file_open(fs_mount_id, (uint8 *) fw_ascfile_name, R_NORMAL_SEEK);
        if (fhandle != 0)
        {
            has_upgrade_fw = TRUE;
            break;
        }

        fhandle = vfs_file_open(fs_mount_id, (uint8 *) fwu_ascfile_name, R_NORMAL_SEEK);
        if (fhandle != 0)
        {
            has_upgrade_fw = TRUE;
            break;
        }
    } while (0);

    if (has_upgrade_fw == TRUE)
    {
        vfs_file_close(fs_mount_id, fhandle);

        if (disk_type == DISK_H)
        {
            ret_val = RESULT_ENTER_CARD_UPGRADE;
        }
        else if (disk_type == DISK_U)
        {
            ret_val = RESULT_ENTER_UHOST_UPGRADE;
        }
    }

    if (fs_mount_id != 0)
    {
        com_file_close(0);
    }

    return ret_val;
}

