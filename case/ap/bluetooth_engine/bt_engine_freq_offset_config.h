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
 * \brief    global variableģ��
 * \author   anbang
 * \version  1.0
 * \date  2018/4/10
 *******************************************************************************/
#ifndef __BT_ENGINE_FREQ_OFFSET_CONFIG_H__
#define __BT_ENGINE_FREQ_OFFSET_CONFIG_H__

enum
{
    CALIB_RESULT_SUCCESS = 0,   //Ƶƫ�����ɹ�
    CALIB_RESULT_OVER_MIN,  //���������С
    CALIB_RESULT_OVER_MAX,  //�����������
    CALIB_RESULT_ERROR,     //�������ʧ��
    CALIB_RESULT_MAX,
};

typedef enum
{
    CALIB_INIT,             //δУ׼
    CALIB_READY,            //�����ӱ�׼�����
    CALIB_CALC_FREQ_OFFSET, //���㵱ǰƵƫ
    CALIB_GET_FREQ_OFFSET,  //��ȡ��ǰƵƫ
    CALIB_SETUP,            //��ʼУ׼Ƶƫ
    CALIB_SUCCESS,           //У׼�ɹ�
    CALIB_FAILED,           //У׼ʧ��
} bt_engine_freq_offset_calibration_status_e;

typedef struct
{
    int16 freq_offset;      //��ǰƵƫֵ��ע��32768Ϊ��Чֵ
    int16 delta_fo;         //��Ҫ���ڵ�Ƶƫֵ�����ں�����ƵƫΪ:des_fo = src_fo - delta_fo
} freq_offset_val_st;

typedef struct
{
    bt_engine_freq_offset_calibration_status_e calibration_status;  //У׼״̬
    freq_offset_val_st fo_val_info;                                 //Ƶƫֵ��Ϣ
    uint32 last_print_time_stamp;                                   //��һ�δ�ӡ��һ��ʱ���
    uint8 last_link_type;
} bt_engine_freq_offset_config_t;

//��ʼ��ƵƫУ׼��Ҫ���õĻ�������
void bt_engine_freq_offset_config_init(void);
//ƵƫУ׼ѭ��������
void bt_engine_freq_offset_config_loop(void);

#endif
