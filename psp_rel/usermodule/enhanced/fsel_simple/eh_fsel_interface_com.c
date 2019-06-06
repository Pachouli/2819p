/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-27 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

static const uint8 card_drv_name[] =
{ "card.drv" };
static const uint8 uhost_drv_name[] =
{ "uhost.drv" };

/******************************************************************************/
/*
 * \par  Description:��ʼ����������
 *
 * \param[in]    none

 * \param[out]      none

 * \return        none

 * \note
 *******************************************************************************/
void fsel_init_sysdata(void)
{
    //��ǰĿ¼�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
    eh_dir_layer = 0;
    //��Ŀ¼����Ŀ¼ָ�룬1,2��������
    eh_root_dir = 0;
    //��ǰ�ļ���Ŀ¼���б��е����
    eh_file_no = 0;
    //��ǰĿ¼���б�����ļ�����
    eh_file_total = 0;

    //��ǰ�����ļ����
    eh_file_no_all = 0;

    eh_dir_no = 1;

    //Ĭ�Ͻ����Ŀ¼
    vfs_cd(eh_vfs_mount, CD_ROOT, 0);
}

/******************************************************************************/
/*
 * \par  Description:enhanced ģ��ĳ�ʼ��
 *
 * \param[in]    init_param ��ʼ�������ṹ��
 * \param[in]
 * \param[in]    drv_mode  �洢��������ģʽ

 * \param[out]      vfs��ʶ��

 * \return        vfs��ʶ��(0��Ч)

 * \note
 *******************************************************************************/
uint32 fsel_init(fsel_param_t *init_param, uint8 drv_mode)
{
    //������������
    uint8 *drv_name;
    //��������
    uint8 drv_type;
    uint8 tmp_fsel_type = 0;
    //������Ϣ
    fsel_error = FSEL_NO_ERR;
    //����Ϊ����ֱ�ӷ���
    if (init_param == NULL)
    {
        return 0;
    }

    eh_scan_mode = init_param->mode;

    tmp_fsel_type = (init_param->fsel_type & 0x7f);
    //�����ʼ������,�ļ�ѡ�����˳�ʱʹ��
    libc_memcpy(&fsel_init_val, init_param, sizeof(fsel_param_t));

    //ѭ��ģʽ
    fsel_init_val.fsel_mode &= 0x0f;

#ifdef SUPPORT_SHUFFLE_MODE
    //shuffle�Ƿ��
    if ((init_param->fsel_mode & 0xf0) != 0)
    {
        eh_shuffle_sw = TRUE;
    }
    else
    {
        eh_shuffle_sw = FALSE;
    }
#else
    eh_shuffle_sw = FALSE;
#endif

    if (init_param->disk == DISK_H)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) &card_drv_name;
    }
    else if (init_param->disk == DISK_U)
    {
        drv_type = DRV_GROUP_STORAGE;
        drv_name = (uint8 *) &uhost_drv_name;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return 0;
    }

    //��װ�洢����
    if (sys_drv_install(drv_type, drv_mode, drv_name) != 0)
    {
        fsel_error = FSEL_ERR_STG;
        return 0;
    }

    //����FS
    eh_vfs_mount = sys_mount_fs(drv_type, fsel_init_val.disk, 0);

    if (eh_vfs_mount == 0)
    {
        sys_drv_uninstall(drv_type);
        fsel_error = FSEL_ERR_FS;
        return 0;
    }

    /*if (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL) == 0)
     {
     eh_vfs_mount = -1;
     }
     else*/
    //{
    //��ʼ������
    fsel_init_sysdata();
    //}

    //�ļ�ѡ��ģʽΪĿ¼ģʽ
    eh_fsel_type = 0;

    eh_fsel_filter_record_dir_flag = FALSE;
#ifdef FILTER_RECORD_DIR
    if (eh_fsel_type == FSEL_TYPE_COMMONDIR)
    {
        if (init_param->mode < 2)
        {
            eh_fsel_filter_record_dir_flag = TRUE;
        }
    }
#endif

    return eh_vfs_mount;
}

/******************************************************************************/
/*
 * \par  Description: �˳�enhanced
 *
 * \param[in]    none

 * \param[out]      none

 * \return        true or false

 * \note
 *******************************************************************************/
bool fsel_exit(void)
{
    int32 ret_val;
    //��������
    uint8 drv_type;

    fsel_error = FSEL_NO_ERR;

    //�ļ�ϵͳ���سɹ�
    if (eh_vfs_mount != 0)
    {
        ret_val = sys_unmount_fs(eh_vfs_mount);
        eh_vfs_mount = 0;
    }
    else
    {
        return TRUE;
    }

    //�ļ�ϵͳж�سɹ�
    if (ret_val < 0)
    {
        fsel_error = FSEL_ERR_FS;
        return FALSE;
    }

    if ((fsel_init_val.disk == DISK_H) || (fsel_init_val.disk == DISK_U))
    {
        drv_type = DRV_GROUP_STORAGE;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return -1;
    }
    //ж�ؽ�������
    ret_val = sys_drv_uninstall(drv_type);

    if (ret_val < 0)
    {
        fsel_error = FSEL_ERR_STG;
        return FALSE;
    }

    return TRUE;
}

