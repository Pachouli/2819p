#ifndef _BT_CON_INTERFACE_H        
#define _BT_CON_INTERFACE_H

typedef enum
{
    CON_INIT,                               //初始化
    CON_DEINIT,                             //逆初始化
    CON_BUF_REQ,                            //host向con申请buf，由commit负责释放
    CON_BUF_COMMIT,                         //host发送数据到con
    CON_SCHEDULE_HIGH_LOOP,                 //高优先级任务调度器
    CON_SCHEDULE_LOW_LOOP,                  //低优先级任务调度器
    CON_GET_BT_CLK,                         //获取con时钟频率
    CON_SET_BT_CLK,                         //设置con时钟频率
//    CON_FREQ_OFFSET_CAL_INIT,               //进行一次频偏值估算（不返回结果，通过下一个api获取结果）
//    CON_GET_AVG_FREQ_OFFSET_TRY,            //获取频偏值估算结果（必须先进行估算）
    CON_CALIB_FREQ_OFFSET_START,
    CON_CALIB_FREQ_OFFSET_GET_FO,
    CON_CALIB_FREQ_OFFSET_CALIB,
    CON_SET_DEBUG_SEL,                      //debug信号相关
    CON_GENERAL_REGISTER_CBK,               //向con注册回调函数
    CON_ADJUST_LINK_TIME,                   //调整链接时间参数
    CON_MONITOR_RF_TEMPERATURE,             //获取当前温度值，监测RF温度变化
//    CON_ADJ_FREQ_WITH_OFFSET,               //通过频偏偏差值调节频偏
//    CON_GET_CUR_FREQ_OFFSET,                //获取当前频偏值
    CON_THREAD_CLOSE,                       //退出Controller线程
    CON_CHECK_HW_ERROR_NO,                  //检查是否发生硬件错误
} bt_con_cmd_e;             //bt_con_op_entry.c

typedef enum
{
    BT_REQUEST_FO = 0, BT_ADJUST_FO,
} bt_frequency_offset_cmd_e;

#define BT_CON_ENTRY_ADDR(x)                (BT_CON_OP_ENTRY_VA_START + (4 * x ))

#define CON_INIT_ENTRY                      (*((uint32*)(BT_CON_ENTRY_ADDR(CON_INIT))))
#define CON_DEINIT_ENTRY                    (*((uint32*)(BT_CON_ENTRY_ADDR(CON_DEINIT))))
#define CON_BUF_REQ_ENTRY                   (*((uint32*)(BT_CON_ENTRY_ADDR(CON_BUF_REQ))))
#define CON_BUF_COMMIT_ENTRY                (*((uint32*)(BT_CON_ENTRY_ADDR(CON_BUF_COMMIT))))
#define CON_SCHEDULE_HIGH_LOOP_ENTRY        (*((uint32*)(BT_CON_ENTRY_ADDR(CON_SCHEDULE_HIGH_LOOP))))
#define CON_SCHEDULE_LOW_LOOP_ENTRY         (*((uint32*)(BT_CON_ENTRY_ADDR(CON_SCHEDULE_LOW_LOOP))))
#define CON_GET_BT_CLK_ENTRY                (*((uint32*)(BT_CON_ENTRY_ADDR(CON_GET_BT_CLK))))
#define CON_SET_BT_CLK_ENTRY                (*((uint32*)(BT_CON_ENTRY_ADDR(CON_SET_BT_CLK))))
//#define CON_FREQ_OFFSET_CAL_INIT_ENTRY      (*((uint32*)(BT_CON_ENTRY_ADDR(CON_FREQ_OFFSET_CAL_INIT))))
//#define CON_GET_AVG_FREQ_OFFSET_TRY_ENTRY   (*((uint32*)(BT_CON_ENTRY_ADDR(CON_GET_AVG_FREQ_OFFSET_TRY))))
#define CON_CALIB_FREQ_OFFSET_START_ENTRY   (*((uint32*)(BT_CON_ENTRY_ADDR(CON_CALIB_FREQ_OFFSET_START))))
#define CON_CALIB_FREQ_OFFSET_GET_FO_ENTRY  (*((uint32*)(BT_CON_ENTRY_ADDR(CON_CALIB_FREQ_OFFSET_GET_FO))))
#define CON_CALIB_FREQ_OFFSET_CALIB_ENTRY  (*((uint32*)(BT_CON_ENTRY_ADDR(CON_CALIB_FREQ_OFFSET_CALIB))))
#define CON_SET_DEBUG_SEL_ENTRY             (*((uint32*)(BT_CON_ENTRY_ADDR(CON_SET_DEBUG_SEL))))
#define CON_GENERAL_REGISTER_CBK_ENTRY      (*((uint32*)(BT_CON_ENTRY_ADDR(CON_GENERAL_REGISTER_CBK))))
#define CON_ADJUST_LINK_TIME_ENTRY          (*((uint32*)(BT_CON_ENTRY_ADDR(CON_ADJUST_LINK_TIME))))
#define CON_MONITOR_RF_TEMPERATURE_ENTRY    (*((uint32*)(BT_CON_ENTRY_ADDR(CON_MONITOR_RF_TEMPERATURE))))
//#define CON_ADJ_FREQ_WITH_OFFSET_ENTRY      (*((uint32*)(BT_CON_ENTRY_ADDR(CON_ADJ_FREQ_WITH_OFFSET))))
//#define CON_GET_CUR_FREQ_OFFSET_ENTRY       (*((uint32*)(BT_CON_ENTRY_ADDR(CON_GET_CUR_FREQ_OFFSET))))
#define CON_THREAD_CLOSE_ENTRY              (*((uint32*)(BT_CON_ENTRY_ADDR(CON_THREAD_CLOSE))))
#define CON_CHECK_HW_ERROR_NO_ENTRY         (*((uint32*)(BT_CON_ENTRY_ADDR(CON_CHECK_HW_ERROR_NO))))

//controller interface
#define con_init(a)                                 ((void(*)(bt_cfg_info_t *))CON_INIT_ENTRY)((a))
#define con_deinit()                                ((int(*)(void))CON_DEINIT_ENTRY)()

#define con_buf_req(a,b,c)                          ((char*(*)(int, unsigned char*, int))CON_BUF_REQ_ENTRY)((a),(b),(c))
#define con_buf_commit(a,b,c)                       ((int(*)(int,  char*, int))CON_BUF_COMMIT_ENTRY)((a),(b),(c))
#define con_schedule_high_loop(a)                   ((int(*)(int))CON_SCHEDULE_HIGH_LOOP_ENTRY)((a))
#define con_schedule_low_loop(a)                    ((int(*)(int))CON_SCHEDULE_LOW_LOOP_ENTRY)((a))
#define con_get_bt_clk(a, b)                        ((uint32(*)(uint16,bt_clock_t *))CON_GET_BT_CLK_ENTRY)((a),(b))
#define con_set_bt_clk(a, b)                        ((uint32(*)(uint16,uint32))CON_SET_BT_CLK_ENTRY)((a),(b))
//#define con_freq_offset_cal_init(a)                 ((void(*)(short))CON_FREQ_OFFSET_CAL_INIT_ENTRY)((a))
//#define con_get_avg_freq_offset_try()               ((short(*)(void))CON_GET_AVG_FREQ_OFFSET_TRY_ENTRY)()
/*
 用于发起频偏计算，可以轮询con_calib_freq_offset_get_fo获取的值确定是否计算完成。
 输入参数:uint8 link_type(a) 当前传输数据链接类型 HCIT_ACLDATA or HCIT_SCODATA
 int16 nFreqRecordMaxNum(b) 通过计算nFreqRecordMaxNum包的频偏值，取平均值
 返回值: TRUE or FALSE
 */
#define con_calib_freq_offset_start(a,b)            ((bool(*)(uint8,short))CON_CALIB_FREQ_OFFSET_START_ENTRY)((a),(b))
/*
 发起频偏计算后，获取本次计算的频偏值，在计算结束之前，会返回FALSE，结束完成后，返回TRUE
 输入参数:int16 *freq_offset_val(a) 存放频偏值的变量地址
 返回值: TRUE or FALSE
 */
#define con_calib_freq_offset_get_fo(a)             ((bool(*)(short *))CON_CALIB_FREQ_OFFSET_GET_FO_ENTRY)((a))
#define con_calib_freq_offset_calib(a)              ((int8(*)(short))CON_CALIB_FREQ_OFFSET_CALIB_ENTRY)((a))
#define con_set_debug_sel(a)                        ((void(*)(uint32))CON_SET_DEBUG_SEL_ENTRY)((a))
#define con_general_register_cbk(a,b,c,d,e)         ((void(*)(void *, void *, void *, uint32, uint8))CON_GENERAL_REGISTER_CBK_ENTRY)((a),(b),(c),(d),(e))
#define con_adjust_link_time(a,b)                   ((void(*)(uint16,int16))CON_ADJUST_LINK_TIME_ENTRY)((a),(b))  //-16,0,16,34,64, [base 48:48+offset]
#define con_monitor_rf_temperature()                ((void(*)(void))CON_MONITOR_RF_TEMPERATURE_ENTRY)()
#define con_thread_close(a)                         ((int(*)(uint8))CON_THREAD_CLOSE_ENTRY)(a)

#define con_check_hw_error_no(a)                    ((void(*)(bt_hw_error_no_e*))CON_CHECK_HW_ERROR_NO_ENTRY)((a))

/******************************************************************************/
/*!                    
 * \par  Description:
 *	      controller 根据传入的偏移值对小机进行频偏值校准
 * \param[in]    int offset         offset表示相对于当前小机频率的偏移量
 offset > 0表示向上调节 
 offset > 0表示向下调节
 * \return       the result  
 * \retval          0               sucess 
 * \retval          >0              超出最大调节范围偏移
 * \retval          <0              超出最小调节范围偏移
 * \ingroup      bt_con.drv
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
//#define con_adj_freq_with_offset(a)                 ((int(*)(int))CON_ADJ_FREQ_WITH_OFFSET_ENTRY)(a)

/******************************************************************************/
/*!                    
 * \par  Description:
 *	  controller 获取当前小机频偏值
 * \param[out]   int *p_freq_offset     存放获取到的频偏值的指针
 * \return       the result  
 * \retval           0 sucess 
 * \retval           1 failed
 * \ingroup      bt_con.drv
 * \par          exmaple code
 * \code 
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
//#define con_get_cur_freq_offset(a)                  ((int(*)(int *))CON_GET_CUR_FREQ_OFFSET_ENTRY)(a)
#endif

