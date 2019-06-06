/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ��  ��ʼ����������
 ********************************************************************************/

#include "app_mengine.h"

//globle variable
//�����м�����
void *mp_handle = NULL;
mmm_mp_status_t g_mp_status;

//����ļ�ϵͳid
uint32 g_file_sys_id = 0;

//����ģʽ
mengine_enter_mode_e g_mengine_enter_mode;

//������Ϣ
mengine_info_t g_mengine_info;

//��ʼ��ʱ���̷�
uint8 g_open_disk;

//�ļ���buffer
uint8 g_file_name[MAX_FILE_NAME];

//�ؼ�����ֵ
app_result_e g_mengine_result;

//�������ñ����ṹ��ָ��
mengine_config_t *g_eg_cfg_p;

//����״̬�����ṹ��ָ��
mengine_status_t *g_eg_status_p;

//���沥����Ϣ�ṹ��ָ��
mengine_playinfo_t *g_eg_playinfo_p;

#ifdef DIR_NAME_SUPPORT
uint8 g_dir_name_buf[32];
#endif

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_BACK_LOW_PRIO;

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
bool g_stop_by_tts_flag;

bool g_need_clear_vm_flag;

uint16 play_file_num = 1;

const uint8 rec_name_less[] =
    { "RECORD     " };
const uint8 rec_name_more[] =
    {
        0xff, 0xfe, //flag
        'R', 0x00,
        'E', 0x00,
        'C', 0x00,
        'O', 0x00,
        'R', 0x00,
        'D', 0x00,
        0x00, 0x00 //end flag
    };

const uint8 alarm_name_less[] =
    { "ALARM      " };
const uint8 alarm_name_more[] =
    {
        0xff, 0xfe, //flag
        'A', 0x00,
        'L', 0x00,
        'A', 0x00,
        'R', 0x00,
        'M', 0x00,
        0x00, 0x00 //end flag
    };

const uint8 dir_001_less[] =
    { "DIR_01     " };
const uint8 dir_001_more[] =
    {
        0xff, 0xfe, //flag
        'D', 0x00,
        'I', 0x00,
        'R', 0x00,
        '_', 0x00,
        '0', 0x00,
        '1', 0x00,
        0x00, 0x00 //end flag
    };

const uint8 dir_002_less[] =
    { "DIR_02     " };
const uint8 dir_002_more[] =
    {
        0xff, 0xfe, //flag
        'D', 0x00,
        'I', 0x00,
        'R', 0x00,
        '_', 0x00,
        '0', 0x00,
        '2', 0x00,
        0x00, 0x00 //end flag
    };

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \���vm ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _clear_vm_deal(uint8 clr_sel)
{
    //(��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ)
    libc_memset(&g_eg_cfg_p->location.dirlocation, 0, sizeof(musfile_location_u));
    //����Ĭ��Ϊudisks
    g_eg_cfg_p->location.dirlocation.disk = g_open_disk;
    //�ϵ���Ϣ
    libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));
    libc_memset(g_p_esd_bk_info->bk_info, 0, sizeof(mmm_mp_bp_info_t));

    //¼��Ӧ�ò���Ҫ����ϵ�
    if (g_mengine_enter_mode == ENTER_MUSIC_START)
    {
        if (clr_sel == DISK_H)
        {
            if (g_p_global_var->att_flag == 0)
            {
                sys_vm_write(g_eg_cfg_p, VM_CARD_BKINFO, sizeof(mengine_config_t));
            }
        }
        else
        {
            if (clr_sel == DISK_U)
            {
                if (g_p_global_var->att_flag == 0)
                {
                    sys_vm_write(g_eg_cfg_p, VM_UHOST_BKINFO, sizeof(mengine_config_t));
                }
            }
            else if (clr_sel == DISK_C)
            {
                if (g_p_global_var->att_flag == 0)
                {
                    sys_vm_write(g_eg_cfg_p, VM_RES_BKINFO, sizeof(mengine_config_t));
                }
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    uint16 magic = 0;
    int32 ret = 0;
    //avoid vram to be coverd we read vram anay mode
    ret = mengine_vram_read(g_open_disk, g_eg_cfg_p);
    if (ret < 0)
    {
        g_eg_cfg_p->magic = 0xffff;
    }
    //�ָ��ϵ�
    g_eg_cfg_p->location.dirlocation.disk = g_open_disk;

    if (g_open_disk == DISK_H)
    {
        magic = VRAM_MAGIC(VM_CARD_BKINFO);
        g_eg_cfg_p->fsel_type = FSEL_TYPE_COMMONDIR;
    }
    else if (g_open_disk == DISK_U)
    {
        magic = VRAM_MAGIC(VM_UHOST_BKINFO);
        g_eg_cfg_p->fsel_type = FSEL_TYPE_COMMONDIR;
    }
    else if (g_open_disk == DISK_C)
    {
        magic = VRAM_MAGIC(VM_RES_BKINFO);
        g_eg_cfg_p->fsel_type = FSEL_TYPE_RESFILE;
    }

    //music engine ������ȡ
    if ((g_eg_cfg_p->magic != magic) || (1 == g_eg_cfg_p->bk_infor_invalid_flag))
    {
        libc_printf("\nset bk_info invalid! magic:0x%x invalid:%d\n", g_eg_cfg_p->magic,
                g_eg_cfg_p->bk_infor_invalid_flag);
        g_eg_cfg_p->magic = magic;
        //�ļ�ѭ��ģʽ
        //g_eg_cfg_p->repeat_mode = FSEL_MODE_LOOPALL;
        //shuffle flag
        g_eg_cfg_p->shuffle_flag = 0;
        //�ļ�ѡ������
        //g_eg_cfg_p->fsel_type = FSEL_TYPE_COMMONDIR;
        g_eg_cfg_p->normal_exit_flag = 1;
        g_eg_cfg_p->bk_infor_invalid_flag = 0;
        _clear_vm_deal(g_open_disk);
    }

    if (g_need_clear_vm_flag == TRUE)
    {
        _clear_vm_deal(g_open_disk);
    }

    //�ļ�ѡ��������ȷ
    //g_eg_cfg_p->fsel_type = FSEL_TYPE_COMMONDIR;

    //����ļ���Ч��
    //��Ҫ������ȡ�ļ���Ϣ
    g_eg_playinfo_p->cur_file_switch |= 0x01;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*����VM����*/
    //��ǰ���ŵ���music����
    mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
}

bool specific_dir_play_file_init(uint8 enter_mode, file_location_t *ploaction_ptr)
{
    bool bret = TRUE;
    bool f_clear_bk_infor = FALSE;
    uint8 dir_direction = DIR_HEAD;

    uint8*dir_name_less = NULL;
    uint8*dir_name_more = NULL;
    switch (enter_mode)
    {
        case ENTER_RECORD_PLAY:
        {
            dir_name_less = (uint8*) &rec_name_less;
            dir_name_more = (uint8*) &rec_name_more;
            f_clear_bk_infor = TRUE;
            dir_direction = DIR_TAIL;
        }
        break;

        case ENTER_ALARM_PLAY:
        {
            dir_name_less = (uint8*) &alarm_name_less;
            dir_name_more = (uint8*) &alarm_name_more;
            f_clear_bk_infor = TRUE;
        }
        break;

        case ENTER_DIR1_PLAY:
        {
            dir_name_less = (uint8*) &dir_001_less;
            dir_name_more = (uint8*) &dir_001_more;
            f_clear_bk_infor = FALSE;
        }
        break;

        case ENTER_DIR2_PLAY:
        {
            dir_name_less = (uint8*) &dir_002_less;
            dir_name_more = (uint8*) &dir_002_more;
            f_clear_bk_infor = FALSE;
        }
        break;

        default:
        break;

    }

    bret = fsel_enter_dir(dir_name_less, dir_name_more, dir_direction);
    if (bret == TRUE)
    {
        bret = fsel_get_location(ploaction_ptr);
        if (f_clear_bk_infor)
        {
            libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));
        }
    }
    return bret;
}

bool normal_play_file_init(uint8 enter_mode, file_location_t *ploaction_ptr)
{
    bool bret = FALSE;
    uint8 exname[4];

    //�����ϴ��˳����ļ�
    bret = fsel_set_location(ploaction_ptr);
    if (bret == TRUE)
    {
        //�ȽϺ�׺��
        vfs_get_name(g_file_sys_id, exname, 0);
        if (libc_strncmp(exname, ploaction_ptr->filename, sizeof(exname)) != 0)
        {
            bret = FALSE;
        }
        else
        {
            uint8 temp_buf[4];
            temp_buf[0] = 'W';
            temp_buf[1] = 'A';
            temp_buf[2] = 'V';
            temp_buf[3] = 0;
            //�޸Ĳ�δ���ʱwav���ļ�λ����Ϣ�����¶ϵ��޷����ŵ�����
            if (libc_strncmp(exname, temp_buf, sizeof(exname)) == 0)
            {
                //����ļ�λ����Ϣ
                g_eg_cfg_p->bk_infor.bp_file_offset = 0;
            }
            vfs_get_name(g_file_sys_id, g_file_name, 16);

            if (libc_strncmp(g_file_name, g_eg_cfg_p->name_buf, 32) != 0)
            {
                bret = FALSE;
            }
        }
    }

    //����ʧ��
    if (bret == FALSE)
    {
        //����ϵ���Ϣ
        libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));
        libc_memset(g_p_esd_bk_info->bk_info, 0, sizeof(mmm_mp_bp_info_t));

        if (g_mengine_enter_mode == ENTER_MUSIC_START)
        {
            //��ȡ��ǰģʽ�µĵ�һ���ɲ����ļ�
            bret = fsel_get_nextfile(ploaction_ptr->filename);
        }
        //�ҵ��ɲ����ļ�
        if (bret == TRUE)
        {
            //��ȡ��ǰ�ļ���location��Ϣ
            bret = fsel_get_location(ploaction_ptr);
        }
    }

#ifdef DIR_NAME_SUPPORT
    if(bret == TRUE)
    {
        fsel_get_dir_name(g_dir_name_buf, 32, &(g_eg_cfg_p->location.dirlocation));
    }
#endif
    return bret;
}

void mengine_fsel_param_init(fsel_param_t *pinit_param, plist_location_t *ploaction_ptr)
{
    //�ļ�ѡ������ʼ���ӿڵĲ�������

    //�̷���Ϣ
    pinit_param->disk = ploaction_ptr->disk;

    //�ļ�ѡ������
    pinit_param->fsel_type = g_eg_cfg_p->fsel_type;

    g_eg_cfg_p->shuffle_flag &= (uint8) 0xf0;

    //ѭ��ģʽ
    pinit_param->fsel_mode = FSEL_MODE_LOOPALL;

    g_eg_cfg_p->repeat_mode = pinit_param->fsel_mode;

    //bitmap ����
    pinit_param->file_type_bit = MUSIC_BITMAP;

    pinit_param->dir1 = (uint8*) &dir_001_less;

    pinit_param->dir2 = (uint8*) &dir_002_less;

    pinit_param->mode = g_mengine_enter_mode; //��ǰ��Ҫ����Ŀ¼:ֻ����mode = 2,3,4,5��ʱ�����Ч��mode=0���������ţ���Ҫ���������ض�Ŀ¼
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool mengine_file_init(void)
 * \��ʼ���ļ�ѡ����,��λ���趨�ļ�or��ǰģʽ��һ���ɲ����ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool mengine_file_init(void)
{
    fsel_param_t init_param; //�ļ�ѡ������ʼ������
    freq_level_e fset_clk;
    //·����Ϣָ��
    file_location_t *loaction_ptr;
    //�ļ�ѡ������
    //��ʼ���ļ�ѡ�������
    bool init_result = FALSE;

    fset_clk = sys_adjust_clk(FREQ_156M, SET_DECODE_CLK);

    loaction_ptr = &(g_eg_cfg_p->location.dirlocation);

    if (_mengine_check_disk_in(loaction_ptr->disk) == FALSE)
    {
        goto init_over;
    }

    //�ļ�ѡ������ʼ���ӿڵĲ�������
    mengine_fsel_param_init(&init_param, loaction_ptr);

    //�ļ�ѡ������ʼ��
    g_file_sys_id = fsel_init(&init_param, 0);

    //�����忴�Ź�һ�Σ�����fsel_initö���豸ʱ��̫�����ۼ�ȫ��ɨ�賬�����Ź�11S��ʱ
    clear_watch_dog();

    //��ʼ���ɹ�
    if (g_file_sys_id != 0)
    {
        //check if disk have file now
        if (vfs_dir(g_file_sys_id, DIR_NEXT, NULL, EXT_NAME_ALL) == 0)
        {
            goto init_over;
        }

        if (g_mengine_enter_mode == ENTER_MUSIC_START)
        {
            init_result = normal_play_file_init(g_mengine_enter_mode, loaction_ptr);
        }
        else
        {
            init_result = specific_dir_play_file_init(g_mengine_enter_mode, loaction_ptr);
        }
    }

    init_over:
    if (init_result == FALSE)
    {
        //û�пɲ����ļ�
        //loaction_ptr->file_total = 0;

        //����״̬Ϊ�������ļ�
        g_eg_status_p->err_status = EG_ERR_NO_FILE;
    }

    sys_adjust_clk(fset_clk, SET_DECODE_CLK);

    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool mengine_file_init(void)
 * \��ʼ���ļ�ѡ����,��λ���趨�ļ�or��ǰģʽ��һ���ɲ����ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/

bool mengine_disk_c_file_init(void)
{
    //·����Ϣָ��
    file_location_t *loaction_ptr;
    //�ļ�ѡ������
    //��ʼ���ļ�ѡ�������
    bool init_result = FALSE;

    loaction_ptr = &(g_eg_cfg_p->location.dirlocation);

    //�ļ�ѡ������ʼ��
#if (SUPPORT_RES_DIR == 1)
    init_result = disk_c_fs_init();
#elif (SUPPORT_EXTLIB_PLAY == 1)
    init_result = extlib_disk_c_fs_init();
#endif
    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \��ʼ�����桢��ȡ������Ϣ����ʼ���ļ�ѡ�������м��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval          -1 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int _app_init(void)
{
    bool init_fsel_ret;

    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_MENGINE, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    if ((g_eg_cfg_p->fsel_type != FSEL_TYPE_SDFILE) && (g_eg_cfg_p->fsel_type != FSEL_TYPE_RESFILE))
    {
        //��ʼ���ļ���ȡ�ɲ����ļ�/�ϵ�
        init_fsel_ret = mengine_file_init();
    }
    else
    {
        if (g_eg_cfg_p->fsel_type == FSEL_TYPE_RESFILE)
        {
            init_fsel_ret = mengine_disk_c_file_init();
        }
        else
        {
            init_fsel_ret = TRUE;
        }
    }

    //ESD RESET �ϵ㲥��
    if (g_p_esd_bk_info->reset_flag == 1)
    {
        mmm_mp_bp_info_t *p_bk_infor = (mmm_mp_bp_info_t *) (g_p_esd_bk_info->bk_info);

        if (p_bk_infor->bp_time_offset != 0)
        {
            libc_memcpy(&g_eg_cfg_p->bk_infor, p_bk_infor, sizeof(mmm_mp_bp_info_t));
        }
    }

    //ESD RESET �Ҷϵ�״̬Ϊ StopSta/PauseSta ����������
    if ((g_p_esd_bk_info->reset_flag == 1)
            && ((g_p_esd_bk_info->play_status == StopSta) || (g_p_esd_bk_info->play_status == PauseSta)))
    {
        //ESD RESETǰ����ͣ״̬����Ҫ _play
    }
    else if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK)
            && ((g_p_global_var->play_status_before == StopSta) || (g_p_global_var->play_status_before == PauseSta)))
    {
        //��绰ǰ����ͣ״̬����Ҫ _play
    }
    else
    {
        if (init_fsel_ret == TRUE)
        {
            init_fsel_ret = _play(PLAY_RESUME);

            //���ų���
            if (init_fsel_ret == FALSE)
            {
                if ((g_file_sys_id > 0) && (g_open_disk != DISK_C))
                {
                    //��ȡ�ļ�����(��֤δɨ�����ʱ��ȡ����ȷ���ļ�����)
                    g_eg_cfg_p->location.dirlocation.file_total = fsel_get_total();
                }
            }
            else
            {
                //ok file
                //clear current file bk point
                libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));
                libc_memset(g_p_esd_bk_info->bk_info, 0, sizeof(mmm_mp_bp_info_t));
                //�״�Ӧ�ý�����Ϊ�ϵ���Ч
                g_eg_cfg_p->bk_infor_invalid_flag = 1;
                mengine_vram_write(g_eg_cfg_p->location.plist_location.disk);
                g_eg_playinfo_p->cur_file_switch |= 0x01;
            }
        }
    }

    if (g_mengine_enter_mode != ENTER_ALARM_PLAY) //���ӱ������ں�̨Ӧ�ã��������bg_app_sta��־
    {
        g_p_global_var->bg_app_sta = BG_STATUS_NULL;
    }

    return init_fsel_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ���,����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //save config дvram
    _save_cfg();

    //�ر��ļ�ѡ����
    fsel_exit();

    //ִ��applib���ע������
    applib_quit(THIS_APP_TYPE);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const void *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    uint8 mengine_para = (uint8) (uint32) argv[0];
    g_mengine_enter_mode = (mengine_enter_mode_e) (mengine_para & 0x07);
    g_need_clear_vm_flag = ((mengine_para & 0x08) != 0);

    //����ǰ��̨���̷�һ��
    if (g_mengine_enter_mode == ENTER_MDISK_PLAY) //��ʷԭ�����̷��滮���������̲���ʹ�� enter_mode ����ģʽָ��
    {
        g_open_disk = DISK_C;
        g_mengine_enter_mode = ENTER_MUSIC_START;
    }
    else
    {
        g_open_disk = (mengine_para & 0xf0);
    }
    //ȫ�ֱ���ָ��
    g_eg_cfg_p = &g_mengine_info.eg_config;

    g_eg_status_p = &g_mengine_info.eg_status;

    g_eg_playinfo_p = &g_mengine_info.eg_playinfo;

    //��ʼ��
    _app_init();

    //ѭ��
    g_mengine_result = mengine_control_block(); //app���ܴ���

    //�˳�
    _app_deinit();

    return g_mengine_result;
}
