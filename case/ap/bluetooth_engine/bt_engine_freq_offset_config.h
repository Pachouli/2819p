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
 * \file     bt_engine_freq_offset_config.h
 * \brief    global variable模块
 * \author   anbang
 * \version  1.0
 * \date  2018/4/10
 *******************************************************************************/
#ifndef __BT_ENGINE_FREQ_OFFSET_CONFIG_H__
#define __BT_ENGINE_FREQ_OFFSET_CONFIG_H__

enum
{
    CALIB_RESULT_SUCCESS = 0,   //频偏调整成功
    CALIB_RESULT_OVER_MIN,  //输入参数过小
    CALIB_RESULT_OVER_MAX,  //输入参数过大
    CALIB_RESULT_ERROR,     //输入参数失败
    CALIB_RESULT_MAX,
};

typedef enum
{
    CALIB_INIT,             //未校准
    CALIB_READY,            //已连接标准发射机
    CALIB_CALC_FREQ_OFFSET, //计算当前频偏
    CALIB_GET_FREQ_OFFSET,  //获取当前频偏
    CALIB_SETUP,            //开始校准频偏
    CALIB_SUCCESS,           //校准成功
    CALIB_FAILED,           //校准失败
} bt_engine_freq_offset_calibration_status_e;

typedef struct
{
    int16 freq_offset;      //当前频偏值，注意32768为无效值
    int16 delta_fo;         //需要调节的频偏值，调节后期望频偏为:des_fo = src_fo - delta_fo
} freq_offset_val_st;

typedef struct
{
    bt_engine_freq_offset_calibration_status_e calibration_status;  //校准状态
    freq_offset_val_st fo_val_info;                                 //频偏值信息
    uint32 last_print_time_stamp;                                   //上一次打印的一次时间戳
    uint8 last_link_type;
} bt_engine_freq_offset_config_t;

//初始化频偏校准需要配置的环境变量
void bt_engine_freq_offset_config_init(void);
//频偏校准循环处理函数
void bt_engine_freq_offset_config_loop(void);

#endif
