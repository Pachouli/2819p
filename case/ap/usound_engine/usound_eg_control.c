/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入引擎应用主循环，应用消息循环，获取中间件状态并响应变化。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_usound_eg.h"
#include "common_sound.h"
usound_status_t g_status_data;
bool s_have_data = TRUE;//FALSE无数据，TRUE有数据
usound_oper_t us_oper;
usound_timer_t tusound;
uint8 usb_no_data_cnt = 0;
bool g_usb_drv_set_over = FALSE;
int16 energy_save = 0, valid_energy = 0;
uint32 sample_rate_save = 44;

void usound_energy_deal(void)
{
    static uint8 energy_low_cnt = 0,energy_high_cnt = 0;
    static uint8 is_lower_energy = 0;

    if (g_status_data.sound_energy == 0)
    {
        if (energy_low_cnt < 15)
        {
            energy_low_cnt++;
            is_lower_energy = 1;
            //libc_printf("\nfl");
        }
        else
        {
            energy_low_cnt = 0;
            valid_energy = 0;
        }
    }
    else
    {
        energy_low_cnt = 0;
        if(is_lower_energy == 1)
        {
            if(energy_high_cnt < 20)//监听 200ms过滤一些非正常音频的数据
            {
                energy_high_cnt ++;
                //libc_printf("\nfh");
                valid_energy = 0;
            }
            else
            {
                energy_high_cnt = 0;
                valid_energy = 1;
                is_lower_energy = 0;
            }
        }
    }
}

void usound_timer_init(void)
{
    tusound.hid_idle_cnt = 0;
    tusound.send_data_cnt = 0;
}

void usound_timer_counter(void)
{
    if (tusound.hid_idle_cnt < 0xff)
    {
        tusound.hid_idle_cnt++;
    }

    if (tusound.send_data_cnt < 0xff)
    {
        tusound.send_data_cnt++;
    }
}

void play_status_sync(void)
{
    do
    {
        usound_get_status(&g_status_data);
        g_eg_status.vol_sync.volume_value = g_status_data.volume_value;
        g_eg_status.vol_sync.volume_max = g_status_data.volume_max;
        g_eg_status.vol_sync.volume_min = g_status_data.volume_min;
        if (g_status_data.volume_chg == 1)
        {
            g_eg_status.vol_sync.volume_chg = 1;
        }

        if ((us_oper.s_manual_set_pause != 0) || (us_oper.s_manual_set_play != 0))
        {
            //if ((us_oper.s_manual_set_pause == 1) && (valid_energy == 0))
            if (us_oper.s_manual_set_pause == 1)
            {
                libc_printf_info("manual set pause!\n");
                g_eg_status.play_status = PauseSta;
                us_oper.s_manual_set_pause = 0;
            }

            if ((us_oper.s_manual_set_play == 1) && (s_have_data == TRUE))
            {
                libc_printf_info("manual set play!\n");
                g_eg_status.play_status = PlaySta;
                us_oper.s_manual_set_play = 0;
            }
            energy_save = valid_energy;
            break;
        }

        if ((g_stop_by_tts_flag == FALSE) && (g_status_data.set_sample != 1))
        {
            if ((valid_energy != energy_save) && (get_mmm_state() != MMM_STATE_KT_PLAYING))
            {
                //pc set
                if ((valid_energy != 0) && (g_eg_status.play_status == PauseSta))
                {
                    libc_printf_info("pc set play!\n");
                    if (pp_handle == NULL)
                    {
                        _play(adc_sample);
                    }
                    g_start_play = FALSE;
                    g_eg_status.play_status = PlaySta;
                }

                if (((valid_energy == 0) && (g_eg_status.play_status == PlaySta)) || (s_have_data == FALSE))
                {

                    libc_printf_info("pc set pause!\n");
                    if (pp_handle != NULL)
                    {
                        _stop();
                    }
                    g_start_play = FALSE;
                    g_eg_status.play_status = PauseSta;
                }
                energy_save = valid_energy;
            }

        }
    } while (0);
}

void enter_usound_auto_play(void)
{
    static uint32 cancle_auto_cnt = 0;

    if (g_start_play == TRUE)
    {
        if (cancle_auto_cnt < 200)
        {
            cancle_auto_cnt++;
        }
        else
        {
            if ((s_have_data == FALSE) || (valid_energy == 0))
            {
                usound_set_cmd(SET_HID_OPERS, VOL_ADD_CMD);
                sys_os_time_dly(2);
                usound_set_cmd(SET_HID_OPERS, VOL_ADD_CMD);
                sys_os_time_dly(5);
                usound_set_cmd(SET_HID_OPERS, PLAY_CMD);
            }
            g_start_play = FALSE;
            libc_printf_info("cancle auto set play!\n");
        }
    }
}

void pc_change_sample_deal(void)
{
    libc_printf_info("\nusoundeg set sample:%d kHz\n", g_status_data.sample_rate);
    if (pp_handle != NULL)
    {
        _stop();
    }
    usound_set_cmd(FIX_SAMPLE_RATE, 0);
    adc_sample = g_status_data.sample_rate;

    if (pp_handle == NULL)
    {
        _play(adc_sample);
    }
}

void get_usound_status_deal(void)
{
    usound_get_status(&g_status_data);
    g_eg_status.vol_sync.volume_value = g_status_data.volume_value;
    g_eg_status.vol_sync.volume_max = g_status_data.volume_max;
    g_eg_status.vol_sync.volume_min = g_status_data.volume_min;
    if (g_status_data.volume_chg == 1)
    {
        g_eg_status.vol_sync.volume_chg = 1;
    }

    g_usb_drv_set_over = g_status_data.usb_set_ok;

    if (g_usb_drv_set_over == TRUE)
    {
        if (g_status_data.start_cnt == 1)
        {
            if (usb_no_data_cnt < 10)//2
            {
                usb_no_data_cnt++;
            }
            else
            {
                s_have_data = FALSE;
                usb_no_data_cnt = 0;
            }
        }
        else
        {
            usb_no_data_cnt = 0;
            s_have_data = TRUE;
        }
        enter_usound_auto_play();
        usound_energy_deal();
    }
    else
    {
        g_status_data.sound_energy = 0;
    }

    play_status_sync();
}

void usoundeg_play_deal(void)
{
    /*获取usound驱动播放状态并处理，周期10ms*/
    get_usound_status_deal();

    /*hid change处理*/
    if ((g_status_data.hid_idle_time != 0) && (g_status_data.hid_idle_time < (tusound.hid_idle_cnt * 10)))
    {
        tusound.hid_idle_cnt = 0;
        usound_set_cmd(SET_HID_CHANGE, 0);
    }

    /*pc 采样率改变 重新设置并加载解码库*/
    if ((sample_rate_save != g_status_data.sample_rate) && (g_stop_by_tts_flag == FALSE) && (get_mmm_state() != MMM_STATE_KT_PLAYING))
    {
        pc_change_sample_deal();
        g_status_data.set_sample = 0;
        sample_rate_save = g_status_data.sample_rate;
    }

    /*100ms usound 定时处理函数*/
    if (tusound.send_data_cnt >= 10)
    {
        usound_timer_deal();
        tusound.send_data_cnt = 0;
    }
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))

record_status_e g_last_record_sta = RECORD_STA_STOP;

/*更新当前录音文件大小到vram*/
void usoundeg_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void usoundeg_recording_check_status(void)
{
    bool ret ;
    //更新录音信息到vram
    if (g_eg_status.record_sta == RECORD_STA_RECORDING)
    {
        usoundeg_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_eg_status.record_sta  != RECORD_STA_RECORDING))
    {
        //从play到pause(录音暂停时，需要强制更新时间)
        libc_printf("record pause , update record file now\n");
        usoundeg_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_eg_status.record_sta ;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void lineineg_control_block(void)
 * \引擎功能处理函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 *******************************************************************************/
app_result_e usoundeg_control_block(void)
{
    private_msg_t cur_msg_val;
    app_result_e mengine_retval = RESULT_NULL;
    bool ret_vals;

    while (1)
    {
        usoundeg_play_deal();

        if (pp_handle != NULL)
        {
            ret_vals = _get_mmm_status();
            if (ret_vals == TRUE)
            {
                sound_energy_t tmp_sound_energy;

                change_ktp_status(g_pp_status.ktp_status);

                if ((g_pp_status.status == MMM_PP_ENGINE_PLAYING)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEIN)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEOUT)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEOUT_PAUSE))
                {
                    tmp_sound_energy.play_cur_energy = g_pp_status.adc_energy.adc_cur_energy;
                    tmp_sound_energy.play_cur_energy_rms = g_pp_status.adc_energy.adc_cur_energy_rms;
                    tmp_sound_energy.mic_cur_energy = g_pp_status.adc_energy.mic_cur_energy;
                    tmp_sound_energy.mic_cur_energy_rms = g_pp_status.adc_energy.mic_cur_energy_rms;
                    libc_memcpy(tmp_sound_energy.freq_point_energy, g_pp_status.freq_point_energy, sizeof(tmp_sound_energy.freq_point_energy));
                    tmp_sound_energy.play_cur_energy = g_status_data.sound_energy;
                    com_update_sound_energy(&tmp_sound_energy);
                }
                else
                {
                    //清除声音能量值
                    com_update_sound_energy(NULL);
                }

                g_eg_status.lowpw_time = g_pp_status.adc_energy.adc_lowpw_time;

                if (g_pp_status.status == MMM_PP_ENGINE_ERROR)
                {
                    //检测到错误，暂时没有这样的状态返回  TODO
                }
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();

            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    //record_module出错
                    g_eg_status.record_sta = RECORD_STA_ERROR;
                }
            }
#endif
        }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
        usoundeg_recording_check_status();
#endif

        //获取并处理详细
        if (get_app_msg(THIS_APP_TYPE, &cur_msg_val) == TRUE)
        {
            switch (cur_msg_val.msg.type)
            {
                case MSG_USOUNDEG_GET_STATUS_SYNC: //获取当前播放状态
                usoundeg_get_status(&cur_msg_val);
                break;

                default:
                mengine_retval = usoundeg_message_done_bank(&cur_msg_val);
                break;
            }
        }

        if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
        {
            change_engine_state(ENGINE_STATE_PAUSE);
        }
        else
        {
            change_engine_state(ENGINE_STATE_PLAYING);
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            //退出应用关闭处理
            if (pp_handle != NULL)
            {
                if (_stop() == TRUE)
                {
                    g_eg_status.play_status = PauseSta;
                }
            }
            break;
        }

        //ESD RESET 实时断点备份
        g_p_esd_bk_info->play_status = g_eg_status.play_status;

        //挂起10ms 多任务交互
        sys_os_time_dly(1);
        usound_timer_counter();
    }

    //清除声音能量值
    com_update_sound_energy(NULL);

    return mengine_retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_reply_msg(private_msg_t* msg_ptr, bool ret_vals)
 * \回复同步消息
 * \param[in]    void  msg_ptr 消息结构体
 * \param[in]    bool  ret_vals 相关事件处理结果 TRUE 成功 FALSE 失败
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR 忽略
 * \ingroup      mengine_control.c
 * \note  回复消息时，根据ret_vals的结果，设置应答成功或失败
 */
/*******************************************************************************/
void usoundeg_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
{
    if (ret_vals == TRUE)
    {
        msg_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        msg_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //回复同步消息(发送信号量)
    libc_sem_post(msg_ptr->sem);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_get_status(void* msg_ptr)
 * \获取当前播放状态
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //存放状态信息
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e usoundeg_get_status(private_msg_t *msg_ptr)
{
    //copy当前播放状态到消息指针指向的数据区
    libc_memcpy(msg_ptr->reply->content, &g_eg_status, sizeof(usoundeg_status_t));
    g_eg_status.vol_sync.volume_chg = 0;

    usoundeg_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_eg_status.record_sta == RECORD_STA_RECORDING) || (g_eg_status.record_sta == RECORD_STA_PAUSE))
    	&& (g_record_param.file_handle != NULL))
    {
        return TRUE; //已经创建
    }

    //创建文件
    common_err_ret = com_record_disk_init(disk);
    if (common_err_ret == REC_FS_ERR_NONE)
    {
        common_err_ret = com_record_create_file(FILE_FIRST, &g_record_param);
    }
    if (common_err_ret != REC_FS_ERR_NONE)
    {
        libc_printf_error("common_err_ret = %d\n", common_err_ret);
        com_record_disk_exit();
        return FALSE;
    }

    return TRUE;
}

bool _record_close_file(void)
{
    if (g_eg_status.record_sta != RECORD_STA_STOP)
    {
        com_record_close_file(FILE_FIRST);
        com_record_disk_exit();
    }

    return TRUE;
}

app_result_e usoundeg_record_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_create_file();
        if (ret_vals == TRUE)
        {
            ret_vals = _record_start(1, RECORD_SOURCE_LR_MIX);
            if (ret_vals == TRUE)
            {
                g_eg_status.record_sta = RECORD_STA_RECORDING;
            }
        }
        else
        {
            ret_vals = FALSE;
        }

        if (ret_vals == FALSE)
        {
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }
    usoundeg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

app_result_e usoundeg_record_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_stop();
        _record_close_file();
        g_eg_status.record_sta = RECORD_STA_STOP;
    }

    usoundeg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

#endif
