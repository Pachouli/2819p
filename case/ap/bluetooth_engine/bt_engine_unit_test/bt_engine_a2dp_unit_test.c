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

#if  (SUPPORT_A2DP_UNIT_TEST == 1)

#define DISPLAY_A2DP_PKG_TIME_INTERVAL_DISTRIBUTION

a2dp_unit_test_info_st *gpst_a2dp_ut_info = NULL;

const a2dp_time_interval_level_threshold_value_st a2dp_tiltv_array[A2DP_TIME_INTERVAL_LEVEL_MAX] =
{
    {   0,19},
    {   20,29},
    {   30,39},
    {   40,49},
    {   50,79},
    {   80,119},
    {   120,159},
    {   160,199},
    {   200,499},
    {   500,999},
    {   1000,1999},
    {   2000,0xffffffff}};

uint8 a2dp_unit_test_get_status(void)
{
    if (gpst_a2dp_ut_info == NULL)
    {
        return STATUS_THREAD_NO_INIT;
    }

    return gpst_a2dp_ut_info->a2dp_unit_test_status;
}

void a2dp_unit_test_set_status(uint8 status)
{
    if (gpst_a2dp_ut_info == NULL)
    {
        return;
    }
    gpst_a2dp_ut_info->a2dp_unit_test_status = status;
}

void a2dp_unit_test_init(void)
{
    //防止有重入时发生内存泄露，一般不允许重入
    if (gpst_a2dp_ut_info != NULL)
    {
        return;
    }

    //分配内存
    gpst_a2dp_ut_info = sys_malloc(sizeof(a2dp_unit_test_info_st));
    if (gpst_a2dp_ut_info == NULL)
    {
        A2DP_UT_DEBUG_PRINT_ERR("gpst_a2dp_ut_info\n");
        return;
    }

    //清零变量信息
    libc_memset(gpst_a2dp_ut_info,0,sizeof(a2dp_unit_test_info_st));

    a2dp_unit_test_set_status(STATUS_THREAD_READY);
    A2DP_UT_DEBUG_PRINT("a2dp unit test ready!\r\n");
}

void a2dp_unit_test_deinit(void)
{
    //已经为空则无需释放空间
    if (gpst_a2dp_ut_info == NULL)
    {
        return;
    }

    sys_free(gpst_a2dp_ut_info);
    gpst_a2dp_ut_info = NULL;
    A2DP_UT_DEBUG_PRINT("a2dp unit test exit!\r\n");
}

void a2dp_unit_test_display_time_rec(a2dp_unit_test_info_st *info)
{
    uint8 i;

#ifdef  DISPLAY_A2DP_PKG_TIME_INTERVAL_DISTRIBUTION
    for (i=0;i<(A2DP_TIME_INTERVAL_LEVEL_MAX-1);i++)
    {
        if (info->level[i] > 0)
        {
            libc_printf("<time interval> %d~%d (ms) : %d\n",a2dp_tiltv_array[i].min_value,a2dp_tiltv_array[i].max_value,info->level[i]);
        }
    }
    if (info->level[i] > 0)
    {
        libc_printf("<time interval> >= %d (ms) : %d\n",a2dp_tiltv_array[(A2DP_TIME_INTERVAL_LEVEL_MAX-1)].min_value,info->level[i]);
    }
#endif
}

void a2dp_unit_test_time_interval_statistics(a2dp_unit_test_info_st *info,uint32 time_interval)
{
    uint8 i;
    for (i=0;i<A2DP_TIME_INTERVAL_LEVEL_MAX;i++)
    {
        if ((time_interval >= a2dp_tiltv_array[i].min_value) && (time_interval <= a2dp_tiltv_array[i].max_value))
        {
            info->level[i] += 1;
            break;
        }
    }
}

void a2dp_unit_test_reset_info(a2dp_unit_test_info_st *info)
{
    info->rec_num = 0;
    libc_memset(info->level,0,sizeof(info->level));
}

uint32 get_sbc_freq_value_with_code(uint8 freq_code)
{
    if (freq_code == 0)
    {
        return 16;
    }
    else if (freq_code == 1)
    {
        return 32;
    }
    else if (freq_code == 2)
    {
        return 44;
    }
    else if (freq_code == 3)
    {
        return 48;
    }
    return 0;
}

void a2dp_unit_test_get_sbc_frame_header(uint8 *data_addr, uint16 data_len)
{
    uint32 i;
    uint16 syncword_index[10];
    uint16 syncword_count = 0;
    uint16 frame_size, temp_frame_size;
    a2dp_sbc_frame_header_st *p_sbc_frame_header = NULL;

    for (i = 0; i < data_len; i++)
    {
        if (data_addr[i] == SBC_FRAME_SYNC_WORD)
        {
            if (syncword_count == 0)
            {
                p_sbc_frame_header = (a2dp_sbc_frame_header_st *)data_addr;
            }
            syncword_index[syncword_count] = i;
            syncword_count++;
        }
    }

    if (syncword_count < 3)
    {
        return;
    }

    frame_size = 0;
    for (i = 0; i < (syncword_count - 1); i++)
    {
        temp_frame_size = syncword_index[i+1] - syncword_index[i];
        if (temp_frame_size > frame_size)
        {
            frame_size = temp_frame_size;
        }
    }

    if (frame_size >= 64)
    {
        gpst_a2dp_ut_info->frame_size = frame_size;
    }

    if (p_sbc_frame_header != NULL)
    {
        gpst_a2dp_ut_info->sample_rate = get_sbc_freq_value_with_code(p_sbc_frame_header->sample_freq);
    }
}

void a2dp_unit_test_get_package(void *data_addr, uint16 data_len)
{
    uint32 cur_time = sys_get_ab_timer();
    uint32 time_interval;
    uint32 cur_play_time;

    if (gpst_a2dp_ut_info->sample_rate == 0)
    {
        //获取采样率和帧长等信息
        a2dp_unit_test_get_sbc_frame_header(data_addr, data_len);

        libc_printf("\r\nSBC FRAME HEADER INFO:\nsampel rate = %d\nframe size = %d\n",
                gpst_a2dp_ut_info->sample_rate, gpst_a2dp_ut_info->frame_size);
    }

    if (gpst_a2dp_ut_info->sample_rate == 0)
    {
        return;
    }

    gpst_a2dp_ut_info->frame_total += ((data_len+20)/gpst_a2dp_ut_info->frame_size);

    cur_play_time = gpst_a2dp_ut_info->frame_total*128/gpst_a2dp_ut_info->sample_rate/1000;
    if (cur_play_time != gpst_a2dp_ut_info->last_play_time)
    {
        libc_printf("PLAY TIME:%d\n", cur_play_time);
        gpst_a2dp_ut_info->last_play_time = cur_play_time;
    }

    if (gpst_a2dp_ut_info->last_time != 0)
    {
        time_interval = cur_time - gpst_a2dp_ut_info->last_time;

        if (time_interval < 10000)
        {
            a2dp_unit_test_time_interval_statistics(gpst_a2dp_ut_info,time_interval);

            gpst_a2dp_ut_info->rec_num++;
            if (gpst_a2dp_ut_info->rec_num >= A2DP_TIME_INTERVAL_REC_MAX)
            {
                a2dp_unit_test_display_time_rec(gpst_a2dp_ut_info);
                a2dp_unit_test_reset_info(gpst_a2dp_ut_info);
            }
        }
    }

    gpst_a2dp_ut_info->last_time = cur_time;
}

#endif

