/*! \cond MMM_MP_API*/
/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao    2010-08-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     record_module_extern.h
 * \brief    ����¼��ģ�飬����Ϊ����ģ���ṩ¼��
 * \author   cailizhen
 * \version  1.0
 * \date     2018-8-18
 *******************************************************************************/
/*! \endcond*/
#ifndef __RECORD_MODULE_EXTERN_H__
#define __RECORD_MODULE_EXTERN_H__


#define SUPPORT_RECORD_FUNC  0 //�Ƿ�֧��¼������


#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ����ϵͳͷ�ļ�
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#endif

/*! \cond MMM_MP_API */

#define ENCODE_WAV_HEAD_SIZE    0x200 //¼���ļ�WAV�ļ�ͷ����

#define ENCODE_OUT_FRAME_SIZE   64 //֡������ȣ��޸ĺ���Ҫȷ�� ENCODE_OUT_BUFFER_SIZE �� ENCODE_IN_FRAME_SIZE �Ƿ���Ҫ�޸ģ������������������Ҫ���м����2
#define ENCODE_OUT_BUFFER_SIZE  0x200 //����2������1.��С��512�ֽ��ļ�ͷ���ȡ�2.��С��ENCODE_OUT_FRAME_SIZE

#define ENCODE_IN_FRAME_SIZE    256 //��С��ENCODE_OUT_FRAME_SIZE��4�������������������Ҫ���м����2

#define BYPASS_FRAME_SIZE       128 //BYPASSģʽ֡����

//������¼��ʱ����Դѡ��
#define RECORD_SOURCE_LR_MIX    0 //���һ�ϣ�����ͨ����˵�������Ⱥ�MIC����
#define RECORD_SOURCE_L_CH      1 //������������ͨ����˵���ǵ�¼����
#define RECORD_SOURCE_R_CH      2 //������������ͨ����˵���ǵ�¼MIC

#define RECORD_FREQ_MIPS        10 //����¼����Ҫ10M������

/*!
 * \brief
 *      ¼��ģ���ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! ��¼��ģ�� */
    RECORD_MODULE_OPEN = 0,
    /*! �ر�¼��ģ�� */
    RECORD_MODULE_CLOSE,

    /*! ����¼���ļ���� */
    RECORD_MODULE_SET_FS,
    /*! ����¼������������ */
    RECORD_MODULE_SET_ENCODE,
    /*! ���û����� */
    RECORD_MODULE_SET_BUFFER,

    /*! ����¼��ģ�� */
    RECORD_MODULE_START,
    /*! ֹͣ¼��ģ�� */
    RECORD_MODULE_STOP,

    /*! ֡��������б���ͱ��룬û�о�ֱ�����͵�output cache */
    RECORD_MODULE_FRAME_DEAL,

    /*! ��ȡ����ϸ����Ϣ������֡�����С */
    RECORD_MODULE_GET_ENCODE_DETAIL,

    /*! ��ȡ״̬��һЩ��Ϣ */
    RECORD_MODULE_GET_STATUS,
} record_module_cmd_e;

/*!
 *  \brief
 *      ¼��ģ��״̬
 */
typedef enum
{
    RECORD_MODULE_STA_CLOSE = 0,
    RECORD_MODULE_STA_OPEN,
    RECORD_MODULE_STA_START,
    RECORD_MODULE_STA_STOP,
    RECORD_MODULE_STA_ERROR,
} record_module_status_e;

/*!
 * \brief
 *      ����Ŷ���,��mmm_mr_status_t��err_noȡֵ
 */
typedef enum
{
    /*! һ��δ֪���� */
    MMM_MR_ERR_UNEXPECTED = -4,
    /*! �ļ�������С���� */
    MMM_MR_ERR_FILE_SIZE_OVER_LIMIT = -3,
    /*! ���������� */
    MMM_MR_ERR_DISK_FULL = -2,
    /*! �ļ�д����󣬵����ļ�ϵͳδ���쳣 */
    MMM_MR_ERR_WRITE_FILE_FAIL = -1,
    /*! ���� */
    MMM_MR_ERR_OK = 0,
} record_module_err_e;

typedef struct
{
    record_module_status_e status;
    record_module_err_e    err_no;
    uint32 start_cluster;
    uint32 write_pages;
    uint32 time;
} record_module_status_t;

typedef enum
{
    MMM_RECORD_STOP = 0,
    MMM_RECORD_RECORDING = 1,
    MMM_RECORD_ERR_PAUSE = 2,
} mmm_record_status_e;

typedef struct
{
    uint16 encode_frame_len;
} record_module_encode_detail_t;

/*!
 *  \brief
 *     ���ļ�����Ĳ�����Ϣ
 */
typedef struct
{
    /*! 0: vfs, 1: sd file, 2: file handle*/
    int fs_type;
    /*! need mount id when fs_type == 0*/
    void *file_mount_id;
    /*! need file handle when fs_type == 2, need file name when fs_type == 1*/
    void *file_handle;
    /*! �ļ���󳤶ȣ���λΪ���� */
    uint32 file_max_length;
} record_module_fs_para_t;

/*!
 * \brief
 *      �����ʽ
 */
typedef enum
{
    /*! PCM */
    ENCODE_PCM = 0,
    /*! ADPCM */
    ENCODE_ADPCM,
    /*! MP3 */
    ENCODE_MP3,
    /*! ACT */
    ENCODE_ACT,
    /*! BYPASS���������� */
    ENCODE_BYPASS,
} record_module_encode_mode_e;

/*!
 *  \brief
 *     ��Ƶ����Ĳ�������
 */
typedef struct
{
    /*! ��Ƶ�����ģʽ */
    record_module_encode_mode_e encode_mode;
    /*! ��Ƶ������ͨ������:1 ��������2 ˫����*/
    uint8 channel;
    /*! ¼����Դ���� RECORD_SOURCE_LR_MIX */
    uint8 record_source;
    /*! �������ȣ�Ĭ��16*/
    uint8 bps;
    /*! ����ı����ʣ���64000,128000��*/
    int bitrate;
    /*! ����Ƶ����8000��16000��44100��*/
    int sample_rate_hz;
} record_module_encode_para_t;

/*!
 *  \brief
 *     ջ�ռ�ͻ���������
 */
typedef struct
{
    uint8 *ptos;
    uint32 stk_size;
    uint8 *output_cache;
    uint8 *output_buffer;
    uint16 output_cache_len;
} record_module_buffer_set_t;

#define RECORD_MODULE_CMD_ENTRY     (*((uint32*)(RECORD_MODULE_ENTRY_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     ����API����api.h
 */
#define record_module_cmd(a,b,c)    ((int32(*)(void *handel, record_module_cmd_e cmd, uint32 args))RECORD_MODULE_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
