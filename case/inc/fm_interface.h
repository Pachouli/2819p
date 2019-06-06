/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM驱动相关宏定义，枚举类型，结构体定义等，为FM驱动和收音机应用共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _FM_INTERFACE_H
#define _FM_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"

typedef enum
{
    FM_MONO = 0, //[1]单声道
    STERE, //[0]立体声
} FM_Audio_e;

typedef enum
{
    releaseMUTE = 0, //解除静音
    SetMUTE, //静音
} FM_MUTE_e;

#define DE_EMPHASIS_TC_50US  0
#define DE_EMPHASIS_TC_75US  1

typedef struct
{
    UINT32 FM_CurrentFreq; //当前频率
    UINT16 FM_SignalStg; //当前频点信号强度
    FM_Audio_e FM_Stereo_Status; //0，立体声；非0，单声道
    FM_MUTE_e FM_Mute_Status; //播放/静音
} FM_Drv_Status_t;

typedef struct
{
    uint32 freq_init; //frequency init
    uint32 freq_low; //frequency lowest
    uint32 freq_high; //frequency highest
    uint8 span; //channel span: 100KHZ, or 50KHZ
    uint8 threshold; //search threshold
    uint8 de_emphasis_tc; //去加重时间参数，0-50us，1-75us
} FM_Drv_Init_Para_t;

#define FM_CLK_SRC_INNER   0 //主控IC时钟源
#define FM_CLK_SRC_EXTERN  1 //外挂晶振时钟源
typedef struct
{
    uint8  clk_src;
    uint32 clk_hz; //单位为Hz，支持32.768KHz（只支持外挂晶振）、6.5MHz、13MHz、24MHz、26MHz
} FM_install_Para_t;

#define FM_STANDBY_MODE  0 //AOUT阻抗不够高，对AUX影响大，但是功耗低，其他应用和软关机时应该进入这种状态
#define FM_IDLE_MODE     1 //AOUT阻抗很高，对AUX影响小，但是功耗高，进入AUX时应该进入这种状态

/* FM 驱动命令枚举类型*/
typedef enum
{
    FM_MODULE_INSTALL = 0,
    FM_MODULE_UNINSTALL,
    /*FM模组初始化*/
    FM_INIT,
    /*FM模组进入standby*/
    FM_STANDBY,
    /*设置频点开始播放*/
    FM_SETFREQ,
    /*获取当前状态信息*/
    FM_GETSTATUS,
    /*静音与解除静音*/
    FM_MUTE_VOL,
    /* 设置搜台门限*/
    FM_SETTHROD,
    /*开启软件搜台过程*/
    FM_SEARCH,
} fm_cmd_e;

/* FM 驱动向上层提供的统一入口定义*/
typedef void *(*fm_op_func)(void *, void *, void *);

/* 驱动内部实现的具体接口函数声明*/
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

