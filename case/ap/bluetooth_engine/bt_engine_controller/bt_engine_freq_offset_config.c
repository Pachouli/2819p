/*******************************************************************************
 *                              US281B
 *                            Module: MainMenu
 *                 Copyright(c) 2018-2028 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       anbang   2018-4-10 19:11:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_freq_offset_config.c
 * \brief    global variableģ��
 * \author   anbang
 * \version  1.0
 * \date  2018/4/10
 *******************************************************************************/
#include "bluetooth_engine.h"

bt_engine_freq_offset_config_t bt_freq_offset_config_st;

/******************************************************************************/
/*!
 * \par  Description:
 *    ͬ��Ƶƫֵ�����������
 * \param[in]    frequency_offset   crystal oscillator frequency offset
 * \param[out]   none
 * \return       the result
 * \retval       none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void bt_engine_send_freq_offset_data(int frequency_offset)
{
    uint8 acl_data[16];
    uint32 temp_flag;
    bt_dev_t *dev;

    dev = btengine_global_variable.g_cur_rmt_dev;
    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->acl_handle != 0))
    {
        BTEG_DBG_PRINTF("send fo\n");
        acl_data[0] = 0x02;
        acl_data[1] = (uint8) (dev->p_dev_status->acl_handle);
        acl_data[2] = (uint8) ((dev->p_dev_status->acl_handle) >> 8);
        acl_data[3] = 0x09;
        acl_data[4] = 0x00;
        acl_data[5] = 0x05;
        acl_data[6] = 0x00;
        acl_data[7] = 0x00;
        acl_data[8] = 0x00;

        //type, 01��ʾƵƫֵ
        acl_data[9] = 0x01;

        //Ƶƫֵ������
        acl_data[10] = (uint8) frequency_offset;
        acl_data[11] = (uint8) ((uint32) frequency_offset >> 8);
        acl_data[12] = (uint8) ((uint32) frequency_offset >> 16);
        acl_data[13] = (uint8) ((uint32) frequency_offset >> 24);

        temp_flag = sys_local_intc_save(0x01);
        HCI_Set_Acl_packet_num(dev->p_dev_status->acl_handle, 1, 0);
        sys_local_intc_restore(temp_flag);

        Send(0, acl_data, 14);
    }
}

void bt_engine_freq_offset_config_init(void)
{
    bt_engine_freq_offset_config_t *gpst_freq_offset_config = &bt_freq_offset_config_st;

    gpst_freq_offset_config->calibration_status = CALIB_INIT;
    gpst_freq_offset_config->fo_val_info.freq_offset = 32768;
    gpst_freq_offset_config->fo_val_info.delta_fo = 0;
    gpst_freq_offset_config->last_print_time_stamp = sys_get_ab_timer();    //���ڿ���5s��ӡһ��Ƶƫ
    gpst_freq_offset_config->last_link_type = 0;
}

uint8 get_cur_link_type(void)
{
    uint8 bt_status;

    bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);

    if ((bt_status == STATUS_CALL_HFP) || (bt_status == STATUS_CALL_OUTGOING) || (bt_status == STATUS_SIRI))
    {
        return HCIT_SCODATA;
    }

    if (bt_status == STATUS_A2DP_PLAY)
    {
        return HCIT_ACLDATA;
    }

    return 0;
}

bool data_trans_in_acl(void)
{
    uint8 bt_status;

    bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);

    if (bt_status == STATUS_A2DP_PLAY)
    {
        return TRUE;
    }
    return FALSE;
}

#define CALC_TIMES      1000

void bt_engine_freq_offset_config_loop(void)
{
    bool ret = FALSE;

    bt_engine_freq_offset_config_t *gpst_freq_offset_config = &bt_freq_offset_config_st;

    //���������ӱ�׼�����������²Ž���ƵƫУ׼
    if (bt_check_dev_exist(btengine_global_variable.g_cur_rmt_dev) == FALSE)
    {
        gpst_freq_offset_config->calibration_status = CALIB_INIT;
        return;
    }

    if (0 != libc_memcmp(btengine_global_variable.g_cur_rmt_dev->dev_info.name, "ZZZZ's Audio Si", 15))
    {
        return;
    }

    //�����ã��Զ�����Ƶƫ��ȡ�Լ�У׼����
    if (gpst_freq_offset_config->calibration_status == CALIB_INIT)
    {
        gpst_freq_offset_config->calibration_status = CALIB_READY;
    }

    //ʹ��sco�������Ƶƫֵ��ʹ��acl�������Ƶƫֵ���׼ֵ�в�ͬ��ƫ��У׼������
    //�����ͳ�ƹ����У��������ݰ�����ת��ʱ����Ҫ���·���Ƶƫͳ��

    //step 1 ����Ƶƫͳ�ƣ���ͳ��һ���������ȡ��Ƶƫֵ
    if (gpst_freq_offset_config->calibration_status == CALIB_READY)
    {

        if (get_cur_link_type() == HCIT_ACLDATA)
        {
            ret = con_calib_freq_offset_start(HCIT_ACLDATA, CALC_TIMES);
            BTEG_DBG_PRINTF("acl fo calc setup %d\n", CALC_TIMES);
        }

        if (get_cur_link_type() == HCIT_SCODATA)
        {
            ret = con_calib_freq_offset_start(HCIT_SCODATA, CALC_TIMES);
            BTEG_DBG_PRINTF("sco fo calc setup %d\n", CALC_TIMES);
        }

        if (ret == FALSE)
        {
            return;
        }

        gpst_freq_offset_config->calibration_status = CALIB_CALC_FREQ_OFFSET;
        gpst_freq_offset_config->last_link_type = get_cur_link_type();
    }

    //step 2 Э��ջ����Ƶƫֵ�У��ȴ��������
    if (gpst_freq_offset_config->calibration_status == CALIB_CALC_FREQ_OFFSET)
    {
        ret = con_calib_freq_offset_get_fo(&gpst_freq_offset_config->fo_val_info.freq_offset);
        if (ret != FALSE)
        {
            //��ȡƵƫֵ�ɹ�
            gpst_freq_offset_config->calibration_status = CALIB_GET_FREQ_OFFSET;
            //����Ƶƫֵ�������
            bt_engine_send_freq_offset_data(gpst_freq_offset_config->fo_val_info.freq_offset);
        }
        else
        {
            //�ڼ���ʱ����״̬��·״̬�ı䣬�����¿�ʼ����Ƶƫֵ
            if (get_cur_link_type() != gpst_freq_offset_config->last_link_type)
            {
                gpst_freq_offset_config->calibration_status = CALIB_READY;
                return;
            }
        }
    }

    //step 3 Ƶƫ������ɺ󣬽���Ƶƫ��ӡ����ѯ�Ƿ���ҪУ׼Ƶƫ
    if (gpst_freq_offset_config->calibration_status == CALIB_GET_FREQ_OFFSET)
    {
        if (sys_get_ab_timer() - gpst_freq_offset_config->last_print_time_stamp > 5000)
        {
            BTEG_DBG_PRINTF("freq offset %d\n", gpst_freq_offset_config->fo_val_info.freq_offset);
            gpst_freq_offset_config->last_print_time_stamp = sys_get_ab_timer();
        }
        if (btengine_global_variable.enable_calib_bt_freq_offset == TRUE)
        {
            gpst_freq_offset_config->calibration_status = CALIB_SETUP;
        }
    }

    //step 4 ��ʼ����У׼Ƶƫ
    if (gpst_freq_offset_config->calibration_status == CALIB_SETUP)
    {
        ret = con_calib_freq_offset_calib(0 - (gpst_freq_offset_config->fo_val_info.freq_offset / 2));
        BTEG_DBG_PRINTF("fo calib ret %d\n", ret);
        if (ret == CALIB_RESULT_SUCCESS)
        {
            gpst_freq_offset_config->calibration_status = CALIB_SUCCESS;
        }
        else if (ret == CALIB_RESULT_ERROR)
        {
            gpst_freq_offset_config->calibration_status = CALIB_FAILED;
        }
    }
}

