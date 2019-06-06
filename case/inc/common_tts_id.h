/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TTS ��ԴID�Ŷ��塣
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_TTS_ID_H_
#define _COMMON_TTS_ID_H_

#include "psp_includes.h"

typedef enum
{
    TTS_ENTER_BT,          //��������
    TTS_WAIT_BT_CON,       //�ȴ�����
    TTS_BT_CON_OK,         //�������ӳɹ�
    TTS_BT_DISCON,         //�����Ͽ�
    TTS_PLAY_SONG,         //����
    TTS_PAUSE_SONG,        //��ͣ
    TTS_LAST_SONG,         //��һ��
    TTS_NEXT_SONG,         //��һ��
    TTS_PLAY_SDCARD,       //������
    TTS_PLAY_UHOST,        //uhost����
    TTS_PLAY_MDISK,        //���̲���
    TTS_ENTER_RADIO,       //������
    TTS_PLAY_LINEIN,       //��Ƶ����
    TTS_PLAY_SPDIF,        //��������
    TTS_ENTER_USBBOX,      //USB����
    TTS_ENTER_UDISK,       //����U��
    TTS_ENTER_PHOTO,       //��������
    TTS_BAT_LOW_CHARGE,    //����������
    TTS_BATTERY_LOW,       //��������
    TTS_POWERON,           //����
    TTS_POWEROFF,          //�ػ�
    TTS_SECTION_NO,        //��
    TTS_SECTION_NUM_0,     //��
    TTS_SECTION_NUM_1,     //һ
    TTS_SECTION_NUM_2,     //��
    TTS_SECTION_NUM_3,     //��
    TTS_SECTION_NUM_4,     //��
    TTS_SECTION_NUM_5,     //��
    TTS_SECTION_NUM_6,     //��
    TTS_SECTION_NUM_7,     //��
    TTS_SECTION_NUM_8,     //��
    TTS_SECTION_NUM_9,     //��
    TTS_SECTION_TEN,       //ʮ
    TTS_SECTION_HUNDRED,   //��
    TTS_SECTION_THOUSAND,  //ǧ
    TTS_SECTION_DOT,       //��
    TTS_SECTION_SONG,      //��
    TTS_SECTION_CHAR_FM,   //FM
    TTS_WAIT_BT_PAIR,      //�ȴ����
    TTS_PAIR_SUCCEED,      //��Գɹ�
    TTS_PAIR_DISCON,       //��ӶϿ�
    TTS_RECORD,            //¼��
    TTS_REPLAY,            //�ط�
    TTS_PLAY_HDMI,         //��������
    TTS_TWS_LEFT_BOX,      //����
    TTS_TWS_RIGHT_BOX,     //����
    TTS_CARD_RECORD,       //��¼��
    TTS_UHOST_RECORD,      //U��¼��
    TTS_CARD_REPLAY,       //���ط�
    TTS_UHOST_REPLAY,      //U�̻ط�
    TTS_SENTENCE_MAX,
} tts_id_e;

#endif
