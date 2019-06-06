/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_controller.c
 * \brief    MainMenu��main����ģ��
 * \author
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "bluetooth_engine.h"

#define CALIB_COMPLETE                  0
#define CALIB_UNCOMPLETE                -1
#define CALIB_GET_FREQ_OFFSET_FAILED    -2

void *bt_con_high_handle(void *param)
{
    int schedule_num;
    int32 ret;

    schedule_num = 0xFF;
    while (1)
    {
        ret = con_schedule_high_loop(schedule_num);
        if (get_RxReadToHCI_flag() == TRUE)
        {
            BtDev_RxReadToHCI(NULL, 0);
            set_RxReadToHCI_flag(FALSE);
        }
        if (ret == TRUE)
        {
            break;
        }
    }

    set_RxReadToHCI_flag(FALSE);
    libc_pthread_exit();
    return NULL;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���ݵ�ǰ��ȡ����Ƶƫֵ����У׼Ƶƫ��Ҫ����ĵ�ƫ��ֵ
 * \param[in]    int freq_offset        ��ǰ��ȡ����Ƶƫֵ
 * \return       int result  
 * \retval       У׼Ƶƫ��Ҫ����ĵ�ƫ��ֵ
 * \ingroup      bt_engine
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int bt_engine_get_freq_adj_offset(int freq_offset)
{
    return freq_offset;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ǰƵƫֵ�Ƿ��ں���Χ�ڣ��Ƿ���Ҫ����У׼
 * \param[in]    int freq_offset        ��ǰ��ȡ����Ƶƫֵ
 * \return       int result  
 * \retval           true   ����Χ�ڣ�����ҪУ׼
 * \retval           false  ���ں���Χ�ڣ���ҪУ׼
 * \ingroup      bt_engine
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
bool bt_engine_freq_offset_inbounds(int freq_offset)
{
    return TRUE;
}

/*!                    
 * \par  Description:
 *    ����ƵƫУ׼
 * \param[in]    ��
 * \return       int result  
 * \retval           CALIB_COMPLETE                     0   У׼��ɣ�����Ҫ��У׼
 * \retval           CALIB_UNCOMPLETE                   -1  У׼δ���
 * \retval           CALIB_GET_FREQ_OFFSET_FAILED       -2  ��ȡƵƫֵʧ��
 * \ingroup      bt_engine
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
//int bt_engine_freq_offset_calibration(void)
//{
//    int freq_offset = 0;
//    int freq_adj_offset = 0;
//    static bool freq_offset_has_send = FALSE;
//    
//    if (con_get_cur_freq_offset(&freq_offset))
//    {
//        return FALSE;
//    }
//    
//    if (!bt_engine_freq_offset_inbounds(freq_offset))
//    {
//        //freq_adj_offset = calc_adj_offset_with_error_compensation(freq_offset);
//        freq_adj_offset = bt_engine_get_freq_adj_offset(freq_offset);
//        con_adj_freq_with_offset(freq_adj_offset);
//        freq_offset_has_send = FALSE;
//        return FALSE;
//    }
//    else 
//    {
//        if (!freq_offset_has_send)
//        {
//            send_freq_offset_data(freq_offset);
//            freq_offset_has_send = TRUE;
//        }
//    }
//
//    return TRUE;
//}
/*!                    
 * \par  Description:
 *    ���ӷ����ʱ�Ƿ���벥��״̬
 * \param[in]    ��
 * \return       int result  
 * \retval           TRUE       ���ڲ���״̬
 * \retval           false      ���ڲ���״̬
 * \ingroup      bt_engine
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
bool bt_engine_sender_in_playing(uint8 bt_status)
{
    if ((STATUS_A2DP_PLAY == bt_status) || (STATUS_LINKED == bt_status))
    {
        return TRUE;
    }
    return FALSE;
}

/*!                    
 * \par  Description:
 *    �Ƿ����ӷ����
 * \param[in]    ��
 * \return       int result  
 * \retval           TRUE       ������
 * \retval           false      û������
 * \ingroup      bt_engine
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
bool bt_engine_connect_sender(uint8 bt_status)
{
    if (0 == libc_memcmp(btengine_global_variable.g_cur_rmt_dev->dev_info.name, "ZZZZ's Audio Si", 15))
    {
        return TRUE;
    }
    return FALSE;
}

void *bt_con_low_handle(void *param)
{
    int schedule_num;
    int32 ret;
    uint8 bt_status;

    schedule_num = 0xFF;
    while (1)
    {
        ret = con_schedule_low_loop(schedule_num);
        if (ret == TRUE)
        {
            break;
        }
        bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);

        //���ӵ�����������������͸�������ʱ
//        if (bt_engine_connect_sender(bt_status) && bt_engine_sender_in_playing(bt_status))
//        {
//            bt_engine_freq_offset_calibration();
//        }

    }

    libc_pthread_exit();
    return NULL;
}

