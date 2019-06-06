/******************************************************************************/
/*!
 * \par  Description:
 * \static 
 * \xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 * \param[in]    
 * \param[out]   
 * \return       
 * \retval       none
 * \ingroup      bluetooth_engine_unit_test.h
 * \note
 */
/*******************************************************************************/
#ifndef __BT_ENGINE_UNIT_TEST_H__
#define __BT_ENGINE_UNIT_TEST_H__

#define SCO_UT_STR_PRE      "<sco ut>"
#define SCO_UT_ERR_END      "  FILE:%s LINE:%d"
#define A2DP_UT_STR_PRE      "<a2dp ut>"
#define A2DP_UT_ERR_END      "  FILE:%s LINE:%d"

#define SCO_UT_DEBUG_PRINT(format,...)          btengine_global_variable.printf.log_dbg(SCO_UT_STR_PRE format,##__VA_ARGS__);
#define SCO_UT_DEBUG_PRINT_ERR(format,...)      btengine_global_variable.printf.log_dbg(SCO_UT_STR_PRE format SCO_UT_ERR_END,##__VA_ARGS__,__FILE__,__LINE__);
#define A2DP_UT_DEBUG_PRINT(format,...)         btengine_global_variable.printf.log_dbg(A2DP_UT_STR_PRE format,##__VA_ARGS__);
#define A2DP_UT_DEBUG_PRINT_ERR(format,...)     btengine_global_variable.printf.log_dbg(A2DP_UT_ERR_END format SCO_UT_ERR_END,##__VA_ARGS__,__FILE__,__LINE__);

//SCO PKG STATUS相关宏定义
#define SCO_PKG_STATE_CORRECTLY_RECEIVED  0
#define SCO_PKG_STATE_POSSIBLE_INVALID    1
#define SCO_PKG_STATE_NO_DATA_RECEIVED    2
#define SCO_PKG_STATE_PARTIALLY_LOST      3

#define A2DP_TIME_INTERVAL_LEVEL_MAX        12
#define A2DP_TIME_INTERVAL_REC_MAX          1000

#define SBC_FRAME_SYNC_WORD                 0x9c
#define SBC_FRAME_BITPOOL_VALUE             0x35

typedef enum thread_st_e_
{
    STATUS_THREAD_NO_INIT, STATUS_THREAD_READY,
} thread_st_e;

typedef struct _sco_unit_test_info_st
{
    uint8 sco_unit_test_status;
    bool destroy_thread;
    uint8 sco_total_pkg_count;
    uint8 err_pkg_count;
    uint8 continue_err_pkg_count;
    uint8 last_pkg_retsult;
    uint8 continue_err_pkg_max;
    uint8 sco_rec_pkg_count;
} sco_unit_test_info_st;

typedef struct _a2dp_sbc_frame_header_st
{
    uint8 syncword;
    uint8 subbands :1;
    uint8 allocation_method :1;
    uint8 channel_mode :2;
    uint8 block_length :2;
    uint8 sample_freq :2;
    uint8 max_bitpool_value;
} a2dp_sbc_frame_header_st;

typedef struct _a2dp_time_interval_level_threshold_value_st
{
    uint32 min_value;
    uint32 max_value;
} a2dp_time_interval_level_threshold_value_st;

typedef struct _a2dp_unit_test_info_st
{
    uint8 a2dp_unit_test_status;
    uint8 sample_rate;
    uint8 frame_size;
    uint32 rec_num;
    uint32 frame_total;
    uint32 last_play_time;
    uint32 last_time;
    uint16 level[A2DP_TIME_INTERVAL_LEVEL_MAX];
} a2dp_unit_test_info_st;

#endif

