/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������Ϣ�߼�����ö�����͡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _KEY_MSG_VALUE_H
#define _KEY_MSG_VALUE_H

#include "psp_includes.h"
#include "vr_adckey_msg_value.h"

/*!
 * \brief
 *  key_value_e �߼�����ö������
 */
typedef enum
{
    /*! �հ��� */
    KEY_NULL = 0x00,
    /*! ����/��ͣ���� */
    KEY_PLAY = 0x01,
    /*! ��һ��󣩰��� */
    KEY_PREV = 0x02,
    /*! ��һ��ң����� */
    KEY_NEXT = 0x03,
    /*! �������ڿ�ݰ��� */
    KEY_VOL = 0x04,
    /*! �����ӿ�ݰ��� */
    KEY_VADD = 0x05,
    /*! ��������ݰ��� */
    KEY_VSUB = 0x06,
    /*! �˵�/ѡ��� */
    KEY_MODE = 0x07,
    /*! ����/�Ͽ����Ӱ��� */
    KEY_CONNECT = 0x08,
    /*! ���ܰ��� */
    KEY_F1 = 0x09,
    /*! mute���� */
    KEY_MUTE = 0x0a,
    /*!���ػ�����*/
    KEY_POWER = 0xb,
    /*! folder+���� */
    KEY_FOLDER_ADD = 0x0c,
    /*! folder-���� */
    KEY_FOLDER_SUB = 0x0d,
    /*!next_vol_add���ð���*/
    KEY_NEXT_VOLADD = 0x0e,
    /*!prev_vol_sub���ð���*/
    KEY_PREV_VOLSUB = 0x0f,
    /*! ����0���� */
    KEY_NUM0 = 0x10,
    /*! ����1���� */
    KEY_NUM1 = 0x11,
    /*! ����2���� */
    KEY_NUM2 = 0x12,
    /*! ����3���� */
    KEY_NUM3 = 0x13,
    /*! ����4���� */
    KEY_NUM4 = 0x14,
    /*! ����5���� */
    KEY_NUM5 = 0x15,
    /*! ����6���� */
    KEY_NUM6 = 0x16,
    /*! ����7���� */
    KEY_NUM7 = 0x17,
    /*! ����8���� */
    KEY_NUM8 = 0x18,
    /*! ����9���� */
    KEY_NUM9 = 0x19,
    /*! FMƵ��΢���� */
    KEY_CHADD = 0x1a,
    /*! FMƵ��΢���� */
    KEY_CHSUB = 0x1b,

    /*! K�豦ר�ã��̰�����+��������һ�� */
    KEY_DEPTH_ADD_NEXT = 0x1c,
    /*! K�豦ר�ã��̰�����-��������һ�� */
    KEY_DEPTH_SUB_PREV = 0x1d,
    /*! K�豦ר�ã��̰�MIC����+�������鳪����+ */
    KEY_MIC_ADD_VADD = 0x1e,
    /*! K�豦ר�ã��̰�MIC����-�������鳪����- */
    KEY_MIC_SUB_VSUB = 0x1f,

    /*! ��ת��ť˳ʱ��� */
    KEY_ROTARY_ADD = 0x20,
    /*! ��ת��ť��ʱ��� */
    KEY_ROTARY_SUB = 0x21,

    /*! ��Ͳ���ȿ��� */
    KEY_DUCKING_SWITCH = 0x22,
    /*! ԭ���������� */
    KEY_REMOVAL_SWITCH = 0x23,
    /*! ¼������ */
    KEY_RECORD_SWITCH = 0x24,
    /*! ¼���ط� */
    KEY_REPLAY = 0x25,

    KEY_VALUE_MAX,

    /*! ���������һ��ADFU��������Ҫ���˵� */
    KEY_ADFU_NO_USE = 0x40,
    /*! ʹ�ܿ���Ԥ��ģʽʱ���л�Ӧ�ð����ᱻת��ΪDUMMY����ʵ��һ������Ӧ���������飬���л�Ӧ�õ��������� fast preview ͨ��ϵͳ��Ϣ����� */
    KEY_DUMMY = 0x41,

    /*! ��ָ���ְ��� */
    KEY_NUMBER = 0x7e,
    /*! ���ⰴ���������û�����ӳ�����ȡ�� KEY_NULL �������һ���ʾ����ʹ�ÿ�ݼ�ӳ���
     �����÷�������Ƕ���ӳ�����ʹ�ã�����Ƕ�ײ�μ��� */
    KEY_NULL_NO_SHORTCUT = 0x7f,
} key_value_e;


#ifdef USE_KEY_NAME
const uint8 key_name_list[KEY_VALUE_MAX][8] =
{
    "NULL",
    "PLAY",
    "PREV",
    "NEXT",
    "VOL",
    "VOL+",
    "VOL-",
    "MODE",
    "LINK",
    "F1",
    "MUTE",
    "POWER",
    "FADD",
    "FSUB",
    "NEXT/V+",
    "PREV/V-",
    "NUM0",
    "NUM1",
    "NUM2",
    "NUM3",
    "NUM4",
    "NUM5",
    "NUM6",
    "NUM7",
    "NUM8",
    "NUM9",
    "CHADD",
    "CHSUB",
    "D+/NEXT",
    "D-/PREV",
    "M+/VADD",
    "M-/VSUB",
    "RADD",
    "RSUB",
    "DUCKING",
    "REMOVAL",
    "RECORD",
    "REPLAY",
};
#endif

#endif

