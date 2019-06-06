/******************************************************************************/
/*!
 * \par  Description:
 * \static 
 * \xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 * \param[in]    
 * \param[out]   
 * \return       
 * \retval       none
 * \ingroup      bt_engine_sco_unit_test.c
 * \note
 */
/*******************************************************************************/
#include "bluetooth_engine.h"

#if  (SUPPORT_SCO_UNIT_TEST == 1)

sco_unit_test_info_st *gpst_sco_ut_info = NULL;

const uint8 sine_wave_sample_data[60] =
{
    0x00,0x00,0x04,0x24,0xCF,0x41,0x37,0x54,0x11,0x58,0xB0,0x4C,0x0C,0x34,0x69,0x12,
    0x98,0xED,0xF4,0xCB,0x51,0xB3,0xEF,0xA7,0xC8,0xAB,0x32,0xBE,0xFB,0xDB,0x00,0x00,
    0x04,0x24,0xCF,0x41,0x37,0x54,0x11,0x58,0xB0,0x4C,0x0C,0x34,0x69,0x12,0x98,0xED,
    0xF4,0xCB,0x51,0xB3,0xEF,0xA7,0xC8,0xAB,0x32,0xBE,0xFB,0xDB
};

uint8 sco_unit_test_get_status(void)
{
    if (gpst_sco_ut_info == NULL)
    {
        return STATUS_THREAD_NO_INIT;
    }

    return gpst_sco_ut_info->sco_unit_test_status;
}

void sco_unit_test_set_status(uint8 status)
{
    if (gpst_sco_ut_info == NULL)
    {
        return;
    }
    gpst_sco_ut_info->sco_unit_test_status = status;
}

void sco_unit_test_init(void)
{
    //防止有重入时发生内存泄露，一般不允许重入
    if (gpst_sco_ut_info != NULL)
    {
        return;
    }

    //分配内存
    gpst_sco_ut_info = sys_malloc(sizeof(sco_unit_test_info_st));
    if (gpst_sco_ut_info == NULL)
    {
        SCO_UT_DEBUG_PRINT_ERR("gpst_sco_ut_info\n");
        return;
    }

    //清零变量信息
    libc_memset(gpst_sco_ut_info,0,sizeof(sco_unit_test_info_st));

    sco_unit_test_set_status(STATUS_THREAD_READY);
    SCO_UT_DEBUG_PRINT("sco unit test ready!\r\n");
}

sco_unit_test_info_st* sco_unit_test_get_info_point(void)
{
    return gpst_sco_ut_info;
}

void sco_unit_test_deinit(void)
{
    //已经为空则无需释放空间
    if (gpst_sco_ut_info == NULL)
    {
        return;
    }

    sys_free(gpst_sco_ut_info);
    gpst_sco_ut_info = NULL;
    SCO_UT_DEBUG_PRINT("sco unit test exit!\r\n");
}

#define COUNT_PKG_MAX           200
#define PKG_STATUS_ERR          0
#define PKG_STATUS_TRUE         1

void pkg_statu_deal_display_count_result(sco_unit_test_info_st *info)
{
    SCO_UT_DEBUG_PRINT("\r\ntotal:%d  err:%d  max_continue_err:%d\r\n",info->sco_total_pkg_count,info->err_pkg_count,info->continue_err_pkg_max);
}

void pkg_statu_deal_reset_all_sco_count_info(sco_unit_test_info_st *info)
{
    if (info == NULL)
    {
        return;
    }

    info->sco_total_pkg_count = 0;
    info->err_pkg_count = 0;
    info->last_pkg_retsult = 0;
    info->continue_err_pkg_count = 0;
    info->continue_err_pkg_max = 0;
}

void sco_unit_test_pkg_status_analyse(sco_unit_test_info_st *info,uint8 pkg_status_flag)
{
    if (info == NULL)
    {
        return;
    }

    info->sco_total_pkg_count++;

    if (pkg_status_flag
            != SCO_PKG_STATE_CORRECTLY_RECEIVED)
    {
        info->err_pkg_count++;

        if (info->last_pkg_retsult == PKG_STATUS_TRUE) //出现不连续错误包，此时开始计算最大连续错误包
        {
            info->continue_err_pkg_count++;
            if (info->continue_err_pkg_count > info->continue_err_pkg_max)
            {
                info->continue_err_pkg_max = info->continue_err_pkg_count;
            }
            info->continue_err_pkg_count = 0;
        }
        else
        {
            info->continue_err_pkg_count++;
        }

        info->last_pkg_retsult = PKG_STATUS_ERR;
    }
    else
    {
        info->last_pkg_retsult = PKG_STATUS_TRUE;
    }

    if (info->sco_total_pkg_count >= COUNT_PKG_MAX)
    {
        //一般当出现不再连续的错误才会计算最大连续错误包数，所以打印时需要强制计算一次
        if (info->continue_err_pkg_count > info->continue_err_pkg_max)
        {
            info->continue_err_pkg_max = info->continue_err_pkg_count;
        }

        pkg_statu_deal_display_count_result(info);

        pkg_statu_deal_reset_all_sco_count_info(info);
    }
}

void sco_unit_test_receive_err_pkg_count(uint8 *buf, int32 len, uint8 pkg_status_flag)
{
    sco_unit_test_info_st *info;
    if (buf == NULL)
    {
        SCO_UT_DEBUG_PRINT("param buf is null\r\n");
        return;
    }

    info = sco_unit_test_get_info_point();

    //统计接收到的sco包错误信息
    sco_unit_test_pkg_status_analyse(info,pkg_status_flag);

    //统计接收到的sco包，用于后续的正弦波数据发送，接收到一个sco就发送一次sco包。
    info->sco_rec_pkg_count++;

}

bool send_sine_wave_package(void)
{
    uint8 *p_buf;
    uint8 hci_header[2];
    static uint32 count = 0;

    count++;

    if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL))
    {
        //申请sco数据包空间
        hci_header[0] = (uint8)((btengine_global_variable.g_cur_rmt_dev->p_dev_status->sco_handle_setup) & 0xff);
        hci_header[1] = (uint8)((btengine_global_variable.g_cur_rmt_dev->p_dev_status->sco_handle_setup) >> 8);
        p_buf = con_buf_req(HCIT_SCODATA, hci_header,SCO_OUT_PAYLOAD_SIZE);

        if (p_buf != NULL)
        {
            //装载正弦波采样数据
            libc_memcpy(p_buf,sine_wave_sample_data,SCO_OUT_PAYLOAD_SIZE/2);
            libc_memcpy(p_buf + SCO_OUT_PAYLOAD_SIZE/2,sine_wave_sample_data,SCO_OUT_PAYLOAD_SIZE/2);

            //发送sco数据包
            con_buf_commit(HCIT_SCODATA, p_buf,SCO_OUT_PAYLOAD_SIZE);
            return TRUE;
        }
        else
        {
            //TODO 临时处理，避免死机
            BTEG_PRINTF("con_buf_req null\n");
            return FALSE;
        }

    }
    return FALSE;
}

void sco_unit_test_send_micout_data(void)
{
    sco_unit_test_info_st *info;
    uint32 temp_flag;

    info = sco_unit_test_get_info_point();

    temp_flag = sys_local_intc_save(0x01);
    {
        //根据接收到的 sco 数据包个数发送 micout sco 数据包
        while (info->sco_rec_pkg_count > 0)
        {
            info->sco_rec_pkg_count--;

            if (send_sine_wave_package() == FALSE)
            {
                SCO_UT_DEBUG_PRINT("send failed\r\n");
                //防止因发送失败导致的死循环
                info->sco_rec_pkg_count = 0;
                break;
            }
        }
    }
    sys_local_intc_restore(temp_flag);
}

#endif

