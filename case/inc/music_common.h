/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������ز�����غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ���ز���ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __MUSIC_COMMON_H__
#define __MUSIC_COMMON_H__

#include "psp_includes.h"
#include "enhanced.h"

#define SUPPORT_RES_DIR     1  //�Ƿ�֧�����ô�����Դ�ļ�����
#define SUPPORT_EXTLIB_PLAY 0  //�Ƿ�֧��extlib��������

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
    /*! ֹͣ */
    StopSta = 0,
    /*! ��ͣ */
    PauseSta,
    /*! ���� */
    PlaySta,
    /*! ����� */
    PlayFast
} play_status_e;

/*!
 * \brief
 *  AB����״̬
 */
typedef enum
{
    /*! ��AB����״̬ */
    PlayAB_Null,
    /*! ��A��״̬ */
    PlayAB_A,
    /*! ��B��״̬ */
    PlayAB_B,
    /*! AB�������״̬ */
    PlayAB_Do
} ab_status_e;

/*!
 * \brief
 *  �����״̬
 */
typedef enum
{
    /*! �������� */
    FBPlay_Null,
    /*! ���״̬ */
    FFPlaySta,
    /*! ����״̬ */
    FBPlaySta,
    /*! �����β�ָ��������� */
    FFPlayTailSta,
    /*! �����ͷ��ͷ��ʼ���� */
    FBPlayHeadSta,
} fast_status_e;

/*!
 * \brief
 *  ������Ϣ
 */
typedef enum
{
    /*! û�д��� */
    EG_ERR_NONE = 0,
    /*�������ļ�*/
    EG_ERR_NO_FILE,
    /*���ʰγ�*/
    EG_ERR_DISK_OUT,
    /*! ���ļ�ʧ�� */
    EG_ERR_OPEN_FILE,
    /*! �ļ���ʽ��֧�� */
    EG_ERR_NOT_SUPPORT,
    /*! ������� */
    EG_ERR_DECODER_ERROR,
    /*! ��license�����Ŵ����������ǲ���ʱ���Ѿ�����  */
    EG_ERR_NO_LICENSE,
    /*! DRMʱ�Ӵ���  */
    EG_ERR_SECURE_CLOCK,
    /*! DRM��Ϣ����  */
    EG_ERR_DRM_INFO
} eg_err_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */
    play_status_e play_status;
    /*! AB����״̬  */
    ab_status_e ab_status;
    /*! �����״̬  */
    fast_status_e fast_status;
    /*! ����״̬,��ȡ�����  */
    eg_err_e err_status;
    /*! ����������ʱ�� */
    int32 lowpw_time;
} mengine_status_t;

/*!
 * \brief
 *  ��ǰ�����ļ���Ϣ
 */
typedef struct
{
    /*! ��ǰʱ�� */
    uint32 cur_time;
    /*! ������ */
    uint32 cur_bitrate;
    /*! audibleͼƬλ��ƫ�� */
    uint32 cur_imag_pos;
    /*! audibleͼƬ���ڱ�־(�Ƿֱ�ͼ) */
    uint8 cur_imag_flag;
    /*! ��ʴ��ڱ�־ */
    uint8 cur_lyric;
    /*! �ļ��л���־ bit0: �����Ƿ��л� bit1:audible�½��Ƿ��л�  */
    uint8 cur_file_switch;
} mengine_playinfo_t;


//MUSICǰ̨����ģʽ
#define PARAM_MUSIC_CARD_DIR        0 //����Ŀ¼����
#define PARAM_MUSIC_UHOST_DIR       1 //U��Ŀ¼����
#define PARAM_MUSIC_CARD_REPLAY     2 //��¼���ط�
#define PARAM_MUSIC_UHOST_REPLAY    3 //U��¼���ط�
#define PARAM_MUSIC_RES_DIR         4 //NOR ��Դ�ļ�����
#define PARAM_MUSIC_UHOST_DIR1_PLAY 5 //U��Ŀ¼1����
#define PARAM_MUSIC_UHOST_DIR2_PLAY 6 //U��Ŀ¼2����
#define PARAM_MUSIC_CARD_DIR1_PLAY  7 //����Ŀ¼1����
#define PARAM_MUSIC_CARD_DIR2_PLAY  8 //����Ŀ¼2����

/*!
 * \brief
 *  ����music�������ģʽ����4bit������ʾ�̷� 0x00 for DISK_SD, 0x10 for DISK_H, 0x20 for DISK_U
 */
typedef enum
{
    /*! ѡ��������� */
    ENTER_MUSIC_START = 0x00,
    /*! NOR������Դ�б��� */
    ENTER_MDISK_PLAY = 0x01,
    /*! ¼���ط� */
    ENTER_RECORD_PLAY = 0x02,
    /*! ���岥�� */
    ENTER_ALARM_PLAY = 0x03,
    /*�ض�Ŀ¼1����*/
    ENTER_DIR1_PLAY = 0x04,
    /*�ض�Ŀ¼2����*/
    ENTER_DIR2_PLAY = 0x05,
} mengine_enter_mode_e;

typedef struct
{
    /*! �ļ��������� */
    uint8 *fname_buffer;
    /*! �ļ�����󳤶� */
    uint8 fname_length;
} music_file_name_t;

#endif //__MUSIC_COMMON_H__
