#ifndef _BT_CON_INTERFACE_H        
#define _BT_CON_INTERFACE_H

typedef enum
{
    CON_INIT,                               //��ʼ��
    CON_DEINIT,                             //���ʼ��
    CON_BUF_REQ,                            //host��con����buf����commit�����ͷ�
    CON_BUF_COMMIT,                         //host�������ݵ�con
    CON_SCHEDULE_HIGH_LOOP,                 //�����ȼ����������
    CON_SCHEDULE_LOW_LOOP,                  //�����ȼ����������
    CON_GET_BT_CLK,                         //��ȡconʱ��Ƶ��
    CON_SET_BT_CLK,                         //����conʱ��Ƶ��
//    CON_FREQ_OFFSET_CAL_INIT,               //����һ��Ƶƫֵ���㣨�����ؽ����ͨ����һ��api��ȡ�����
//    CON_GET_AVG_FREQ_OFFSET_TRY,            //��ȡƵƫֵ�������������Ƚ��й��㣩
    CON_CALIB_FREQ_OFFSET_START,
    CON_CALIB_FREQ_OFFSET_GET_FO,
    CON_CALIB_FREQ_OFFSET_CALIB,
    CON_SET_DEBUG_SEL,                      //debug�ź����
    CON_GENERAL_REGISTER_CBK,               //��conע��ص�����
    CON_ADJUST_LINK_TIME,                   //��������ʱ�����
    CON_MONITOR_RF_TEMPERATURE,             //��ȡ��ǰ�¶�ֵ�����RF�¶ȱ仯
//    CON_ADJ_FREQ_WITH_OFFSET,               //ͨ��Ƶƫƫ��ֵ����Ƶƫ
//    CON_GET_CUR_FREQ_OFFSET,                //��ȡ��ǰƵƫֵ
    CON_THREAD_CLOSE,                       //�˳�Controller�߳�
    CON_CHECK_HW_ERROR_NO,                  //����Ƿ���Ӳ������
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
 ���ڷ���Ƶƫ���㣬������ѯcon_calib_freq_offset_get_fo��ȡ��ֵȷ���Ƿ������ɡ�
 �������:uint8 link_type(a) ��ǰ���������������� HCIT_ACLDATA or HCIT_SCODATA
 int16 nFreqRecordMaxNum(b) ͨ������nFreqRecordMaxNum����Ƶƫֵ��ȡƽ��ֵ
 ����ֵ: TRUE or FALSE
 */
#define con_calib_freq_offset_start(a,b)            ((bool(*)(uint8,short))CON_CALIB_FREQ_OFFSET_START_ENTRY)((a),(b))
/*
 ����Ƶƫ����󣬻�ȡ���μ����Ƶƫֵ���ڼ������֮ǰ���᷵��FALSE��������ɺ󣬷���TRUE
 �������:int16 *freq_offset_val(a) ���Ƶƫֵ�ı�����ַ
 ����ֵ: TRUE or FALSE
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
 *	      controller ���ݴ����ƫ��ֵ��С������ƵƫֵУ׼
 * \param[in]    int offset         offset��ʾ����ڵ�ǰС��Ƶ�ʵ�ƫ����
 offset > 0��ʾ���ϵ��� 
 offset > 0��ʾ���µ���
 * \return       the result  
 * \retval          0               sucess 
 * \retval          >0              ���������ڷ�Χƫ��
 * \retval          <0              ������С���ڷ�Χƫ��
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
 *	  controller ��ȡ��ǰС��Ƶƫֵ
 * \param[out]   int *p_freq_offset     ��Ż�ȡ����Ƶƫֵ��ָ��
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

