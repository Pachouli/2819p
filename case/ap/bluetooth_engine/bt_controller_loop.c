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
 * \brief    MainMenu的main函数模块
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
 *    根据当前获取到的频偏值计算校准频偏需要传入的的偏移值
 * \param[in]    int freq_offset        当前获取到的频偏值
 * \return       int result  
 * \retval       校准频偏需要传入的的偏移值
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
 *    当前频偏值是否在合理范围内，是否还需要进行校准
 * \param[in]    int freq_offset        当前获取到的频偏值
 * \return       int result  
 * \retval           true   合理范围内，不需要校准
 * \retval           false  不在合理范围内，需要校准
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
 *    进行频偏校准
 * \param[in]    无
 * \return       int result  
 * \retval           CALIB_COMPLETE                     0   校准完成，不需要再校准
 * \retval           CALIB_UNCOMPLETE                   -1  校准未完成
 * \retval           CALIB_GET_FREQ_OFFSET_FAILED       -2  获取频偏值失败
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
 *    连接发射机时是否进入播放状态
 * \param[in]    无
 * \return       int result  
 * \retval           TRUE       正在播放状态
 * \retval           false      不在播放状态
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
 *    是否连接发射机
 * \param[in]    无
 * \return       int result  
 * \retval           TRUE       连接中
 * \retval           false      没有连接
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

        //连接到蓝牙发射机且做推送歌曲测试时
//        if (bt_engine_connect_sender(bt_status) && bt_engine_sender_in_playing(bt_status))
//        {
//            bt_engine_freq_offset_calibration();
//        }

    }

    libc_pthread_exit();
    return NULL;
}

