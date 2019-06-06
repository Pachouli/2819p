/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM����������Ӧ����ڣ���ʼ�������١�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "App_fmengine.h"

//globle variable
//FM ����״̬��Ϣ
FM_Drv_Status_t g_fmengine_status;

//FM ģ������״̬
Module_Status_e g_module_status;

uint32 fm_clock_freq;

fmeg_init_para_t g_init_para_backup; //for standby

//globle variable
//�����м�����
void *pp_handle = NULL;
mmm_pp_status_t g_pp_status;

//����״̬�����ṹ��ָ��
fmeg_status_t g_eg_status;

//��������ģʽ��FM_EG_ENTER_NORMAL   0 ������������, FM_EG_ENTER_STANDBY  1  ����FM Standby����
uint8 g_enter_mode;

//FM ��������ջ
INT8U prio = AP_BACK_LOW_PRIO;

//��TTS��������ͣ������TTS������������Ҫ�ָ�����
bool g_stop_by_tts_flag;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
record_param_t g_record_param;
record_module_status_t g_rm_status;
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_load_cfg(void)
{

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_save_cfg(void)
 * \����������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_save_cfg(void)
{

}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \��ȡ������Ϣ����װFM  ����
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval           0 sucess
 * \retval          -1 failed
 * \note
 */
/*******************************************************************************/
bool fmengine_init(void)
{
    int app_init_ret = 0;
    bool ret_vals = TRUE;
    FM_install_Para_t tmp_ins_para;
    uint8 clk_val_sel;

    //��ȡ������Ϣ
    fmengine_load_cfg();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_FMENGINE, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    //��װfm  ����
    tmp_ins_para.clk_src = com_get_config_default(RADIO_CLK_SRC_SEL);
    clk_val_sel = com_get_config_default(RADIO_CLK_VAL_SEL);
    if (clk_val_sel == 0)
    {
        tmp_ins_para.clk_hz = 32768;
    }
    else if (clk_val_sel == 1)
    {
        tmp_ins_para.clk_hz = 6500000;
    }
    else if (clk_val_sel == 2)
    {
        tmp_ins_para.clk_hz = 13000000;
    }
    else if (clk_val_sel == 3)
    {
        tmp_ins_para.clk_hz = 24000000;
    }
    else
    {
        tmp_ins_para.clk_hz = 26000000;
    }
    fm_clock_freq = tmp_ins_para.clk_hz;
    app_init_ret = fm_module_install(&tmp_ins_para);

    if (app_init_ret == -1)
    {
        ret_vals = FALSE;
    }

    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fmengine_deinit(void)
 * \�˳�app�Ĺ��ܺ���,����������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool fmengine_deinit(void)
{
    //ж��fm ����
    fm_module_uninstall();

    //ִ��applib���ע������
    applib_quit(THIS_APP_TYPE);

    //save config дvram
    fmengine_save_cfg();

    return TRUE;
}

app_result_e fmengine_let_fm_standby(void)
{
    fmeg_init_para_t tmp_fmeg_init_para;
    private_msg_t fmengine_msg;
    app_result_e result = RESULT_NULL;

    tmp_fmeg_init_para.freq_init = 0;
    tmp_fmeg_init_para.threshold = 5;
    tmp_fmeg_init_para.span = 100;
    tmp_fmeg_init_para.freq_low = 87500;
    tmp_fmeg_init_para.freq_high = 108000;
    tmp_fmeg_init_para.de_emphasis_tc = DE_EMPHASIS_TC_50US;

    libc_printf_info("let fm standby\n");
    if (fm_module_open_do(&tmp_fmeg_init_para) == FALSE)
    {
        libc_printf_warning("fm init fail!\n");
    }

    while (1)
    {
        //��ȡ��������Ϣ
        if (get_app_msg(THIS_APP_TYPE, &fmengine_msg) == TRUE)
        {
            switch (fmengine_msg.msg.type)
            {
            case MSG_APP_QUIT: //�˳�app
                fm_module_standby();
                result = RESULT_APP_QUIT;
                break;
            default:
                break;
            }
        }

        if (result == RESULT_APP_QUIT)
        {
            break;
        }

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \Description: FM Engine ap entry function
 * \int main(int argc, const void *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e result = RESULT_NULL;

    g_enter_mode = (uint8) (uint32) argv[0];

    //��ʼ��
    if (fmengine_init() == TRUE)
    {
        if (g_enter_mode == FM_EG_ENTER_NORMAL)
        {
            result = fmengine_control_block();
        }
        else //������FM����Standby״̬���˳�
        {
            result = fmengine_let_fm_standby();
        }
    }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");

    extern void fmeg_record_update_record_file(bool force);
    fmeg_record_update_record_file(TRUE);

    _record_close_file();
#endif

    fmengine_deinit(); //�˳�

    return result;
}

