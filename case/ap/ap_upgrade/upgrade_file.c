/*!
 * \file      upgrade_file.c
 * \brief     升级文件接口
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#include "upgrade_app.h"


const u8_t*  const upgrade_names_factory[] =
{
    "\xFF\xFE" "U\0" "P\0" "G\0" "R\0" "A\0" "D\0" "E\0" ".\0" "F\0" "W\0" "U\0" "\0\0",
    "UPGRADE FWU",
};


const u8_t*  const upgrade_names_end_user[] =
{
    "\xFF\xFE" "U\0" "P\0" "G\0" "R\0" "A\0" "D\0" "E\0" ".\0" "H\0" "E\0" "X\0" "\0\0",
    "UPGRADE HEX",
};


const u8_t*  const upgrade_new_names[] =
{
    "\xFF\xFE" "U\0" "P\0" "G\0" "R\0" "A\0" "D\0" "E\0" "~\0" ".\0" "H\0" "E\0" "X\0" "\0\0",
    "UPGRADE~HEX",
};


/* 打开升级文件
 */
bool upgrade_file_open(u8_t disk)
{
    upgrade_app_context_t*  upg = upgrade_app_get_context();

    char*  err_str = "NOT_FOUND";
    int    i;

    log_debug();

    upg->vfs_mount = com_file_open(disk, NULL, 0);

    if (upg->vfs_mount == 0)
    {
        err_str = "FS_ERR";
        goto err;
    }

    vfs_cd(upg->vfs_mount, CD_ROOT, 0);

    for (i = 0; i < sizeof(upgrade_names_factory) / sizeof(u8_t*); i++)
    {
        void*  file_name = (void*)upgrade_names_factory[i];

        upg->upg_file = vfs_file_open(upg->vfs_mount, file_name, R_NORMAL_SEEK);

        if (upg->upg_file != 0)
        {
            upg->upg_mode = UPGRADE_MODE_FACTORY;
            upg->upg_name = file_name;

            log_info("NAME_%d, MODE_%d", i, upg->upg_mode);
            
            return _OK;
        }
    }

    for (i = 0; i < sizeof(upgrade_names_end_user) / sizeof(u8_t*); i++)
    {
        void*  file_name = (void*)upgrade_names_end_user[i];
        void*  new_name  = (void*)upgrade_new_names[i];
        
        upg->upg_file = vfs_file_open(upg->vfs_mount, file_name, R_NORMAL_SEEK);

        if (upg->upg_file != 0)
        {
            upg->upg_mode = UPGRADE_MODE_END_USER;
            
            log_info("NAME_%d, MODE_%d", i, upg->upg_mode);
            
            vfs_file_close(upg->vfs_mount, upg->upg_file);

            if (vfs_file_rename(upg->vfs_mount, new_name, file_name, 1) == 0)
            {
                err_str = "RENAME_FAIL";
                goto err;
            }

            upg->upg_file = vfs_file_open(upg->vfs_mount, new_name, R_NORMAL_SEEK);

            if (upg->upg_file == 0)
            {
                err_str = "REOPEN_FAIL";
                goto err;
            }

            upg->upg_name = new_name;

            return _OK;
        }
    }

err:
    log_error("%s", err_str);

    return _FAIL;
}


/* 读取升级文件
 */
bool upgrade_file_read(void* buf, u32_t offs, int len)
{
    upgrade_app_context_t*  upg = upgrade_app_get_context();

    if (vfs_file_seek(upg->vfs_mount, offs, SEEK_SET, upg->upg_file) == 0)
        goto err;

    if (vfs_file_read(upg->vfs_mount, buf, len, upg->upg_file) != len)
        goto err;

    return _OK;

err:
    return _FAIL;
}


