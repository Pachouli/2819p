/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�ΪFM������������Ӧ�ù���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _FM_INTERFACE_H
#define _FM_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"

typedef enum
{
    FM_MONO = 0, //[1]������
    STERE, //[0]������
} FM_Audio_e;

typedef enum
{
    releaseMUTE = 0, //�������
    SetMUTE, //����
} FM_MUTE_e;

#define DE_EMPHASIS_TC_50US  0
#define DE_EMPHASIS_TC_75US  1

typedef struct
{
    UINT32 FM_CurrentFreq; //��ǰƵ��
    UINT16 FM_SignalStg; //��ǰƵ���ź�ǿ��
    FM_Audio_e FM_Stereo_Status; //0������������0��������
    FM_MUTE_e FM_Mute_Status; //����/����
} FM_Drv_Status_t;

typedef struct
{
    uint32 freq_init; //frequency init
    uint32 freq_low; //frequency lowest
    uint32 freq_high; //frequency highest
    uint8 span; //channel span: 100KHZ, or 50KHZ
    uint8 threshold; //search threshold
    uint8 de_emphasis_tc; //ȥ����ʱ�������0-50us��1-75us
} FM_Drv_Init_Para_t;

#define FM_CLK_SRC_INNER   0 //����ICʱ��Դ
#define FM_CLK_SRC_EXTERN  1 //��Ҿ���ʱ��Դ
typedef struct
{
    uint8  clk_src;
    uint32 clk_hz; //��λΪHz��֧��32.768KHz��ֻ֧����Ҿ��񣩡�6.5MHz��13MHz��24MHz��26MHz
} FM_install_Para_t;

#define FM_STANDBY_MODE  0 //AOUT�迹�����ߣ���AUXӰ��󣬵��ǹ��ĵͣ�����Ӧ�ú���ػ�ʱӦ�ý�������״̬
#define FM_IDLE_MODE     1 //AOUT�迹�ܸߣ���AUXӰ��С�����ǹ��ĸߣ�����AUXʱӦ�ý�������״̬

/* FM ��������ö������*/
typedef enum
{
    FM_MODULE_INSTALL = 0,
    FM_MODULE_UNINSTALL,
    /*FMģ���ʼ��*/
    FM_INIT,
    /*FMģ�����standby*/
    FM_STANDBY,
    /*����Ƶ�㿪ʼ����*/
    FM_SETFREQ,
    /*��ȡ��ǰ״̬��Ϣ*/
    FM_GETSTATUS,
    /*������������*/
    FM_MUTE_VOL,
    /* ������̨����*/
    FM_SETTHROD,
    /*���������̨����*/
    FM_SEARCH,
} fm_cmd_e;

/* FM �������ϲ��ṩ��ͳһ��ڶ���*/
typedef void *(*fm_op_func)(void *, void *, void *);

/* �����ڲ�ʵ�ֵľ���ӿں�������*/
typedef struct
{
    fm_op_func FmModuleInstall;
    fm_op_func FmModuleUninstall;
    fm_op_func Init;
    fm_op_func Standby;
    fm_op_func SetFreq;
    fm_op_func GetStatus;
    fm_op_func Mute;
    fm_op_func SetThrod;
    fm_op_func Search;
} fm_driver_operations;

#define FM_ENTRY_ADDR(x)        (FM_ENTRY_VA_START + (4 * x))

#define FM_MODULE_INSTALL_ENTRY    (*((uint32*)(FM_ENTRY_ADDR(FM_MODULE_INSTALL))))
#define FM_MODULE_UNINSTALL_ENTRY  (*((uint32*)(FM_ENTRY_ADDR(FM_MODULE_UNINSTALL))))
#define FM_INIT_ENTRY           (*((uint32*)(FM_ENTRY_ADDR(FM_INIT))))
#define FM_STANDBY_ENTRY        (*((uint32*)(FM_ENTRY_ADDR(FM_STANDBY))))
#define FM_SETFREQ_ENTRY        (*((uint32*)(FM_ENTRY_ADDR(FM_SETFREQ))))
#define FM_GETSTATUS_ENTRY      (*((uint32*)(FM_ENTRY_ADDR(FM_GETSTATUS))))
#define FM_MUTE_VOL_ENTRY       (*((uint32*)(FM_ENTRY_ADDR(FM_MUTE_VOL))))
#define FM_SETTHROD_ENTRY       (*((uint32*)(FM_ENTRY_ADDR(FM_SETTHROD))))
#define FM_SEARCH_ENTRY         (*((uint32*)(FM_ENTRY_ADDR(FM_SEARCH))))

#if 0
extern int32 fmdrv_init(uint32 para);
#endif
#define fm_module_install(a)    ((int(*)(FM_install_Para_t*))FM_MODULE_INSTALL_ENTRY)((a))

#if 0
extern int32 fmdrv_exit(void);
#endif
#define fm_module_uninstall()   ((int(*)(void))FM_MODULE_UNINSTALL_ENTRY)()

#if 0
extern int sFM_Init(FM_Drv_Init_Para_t *p_init_para);
#endif
#define fm_open(a)              ((int(*)(FM_Drv_Init_Para_t*))FM_INIT_ENTRY)((a))

#if 0
extern int sFM_Standby(uint8 mode);
#endif
#define fm_close(a)             ((int(*)(uint8))FM_STANDBY_ENTRY)((a))

#if 0
extern int sFM_SetFreq(uint32 freq);
#endif
#define fm_set_freq(a)          ((int(*)(uint32))FM_SETFREQ_ENTRY)((a))

#if 0
extern int sFM_GetStatus(FM_Drv_Status_t *p_Drv_Status);
#endif
#define fm_get_status(a)        ((int(*)(FM_Drv_Status_t *))FM_GETSTATUS_ENTRY)((a))

#if 0
extern int sFM_Mute(FM_MUTE_e mode);
#endif
#define fm_mute(a)              ((int(*)(FM_MUTE_e))FM_MUTE_VOL_ENTRY)((a))

#if 0
extern int sFM_SetThrod(uint32 level);
#endif
#define fm_set_throd(a)         ((int(*)(uint32))FM_SETTHROD_ENTRY)((a))

#if 0
extern int sFM_Search(uint32 freq);
#endif
#define fm_soft_seek(a)         ((int(*)(uint32))FM_SEARCH_ENTRY)((a))


#endif

