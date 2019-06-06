/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：COMMON 一些公共功能接口相关的宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_FUNC_H_
#define _COMMON_FUNC_H_

#include "applib.h"
#include "key_interface.h"
#include "hdmi_interface.h"
#include "ext_rtc_interface.h"
#include "common_stub.h"
#include "record_common.h"

/*!
 * \brief
 *  card_state_e 卡状态枚举类型
 */
typedef enum
{
    /*! 没卡 */
    CARD_STATE_CARD_NULL = 0,
    /*! 有卡但没播放 */
    CARD_STATE_CARD_IN = 1,
} card_state_e;

/*!
 * \brief
 *  cable_state_e USB数据线状态枚举类型
 */
typedef enum
{
    /*! 没插 USB 线 */
    CABLE_STATE_CABLE_NULL = 0,
    /*! 有插 USB 线 */
    CABLE_STATE_CABLE_IN = 1,
} cable_state_e;

/*!
 * \brief
 *  uhost_state_e uhost状态枚举类型
 */
typedef enum
{
    /*! 没有u盘 */
    UHOST_NULL = 0,
    /*! 有u盘 */
    UHOST_IN = 1,
} uhost_state_e;

/*!
 * \brief
 *  adaptor_state_e USB充电线缆状态枚举类型
 */
typedef enum
{
    /*! 没插 USB 充电线 */
    ADAPTOR_STATE_CABLE_NULL = 0,
    /*! 有插 USB 充电线 */
    ADAPTOR_STATE_CABLE_IN = 1,
} adaptor_state_e;

/*!
 * \brief
 *  linein_state_e linein状态枚举类型
 */
typedef enum
{
    /*! 没linein */
    LINEIN_STATE_LINEIN_NULL = 0,
    /*! 有linein */
    LINEIN_STATE_LINEIN_IN = 1,
} linein_state_e;

#if 0
extern void set_cur_func_id_inner(uint8 func_id);
#endif
#define set_cur_func_id(a)    ((void(*)(uint8))COM_SET_CUR_FUNC_ID)((a))

#if 0
extern card_state_e get_card_state_inner(void);
#endif
#define get_card_state()      ((card_state_e(*)(void))COM_GET_CARD_STATE)()

#if 0
extern void change_card_state_inner(card_state_e card_state);
#endif
#define change_card_state(a)  ((void(*)(card_state_e))COM_CHANGE_CARD_STATE)((a))

#if 0
extern uhost_state_e get_uhost_state_inner(void);
#endif
#define get_uhost_state()     ((uhost_state_e(*)(void))COM_GET_UHOST_STATE)()

#if 0
extern linein_state_e get_linein_state_inner(void);
#endif
#define get_linein_state()    ((linein_state_e(*)(void))COM_GET_LINEIN_STATE)()

#if 0
extern cable_state_e get_cable_state_inner(void);
#endif
#define get_cable_state()     ((cable_state_e(*)(void))COM_GET_CABLE_STATE)()

#if 0
extern adaptor_state_e get_adaptor_state_inner(void);
#endif
#define get_adaptor_state()   ((adaptor_state_e(*)(void))COM_GET_ADAPTOR_STATE)()

#if 0
extern bool common_init_inner(void);
#endif
#define common_init()         ((void(*)(void))COM_COMMON_INIT)()

#if 0
extern void common_fmdrv_standby_inner(uint8 mode);
#endif
#define common_fmdrv_standby(a)  ((void(*)(uint8))COM_COMMON_FMDRV_STANDBY)((a))

#if 0
extern bool sys_card_detect_inner(void);
#endif
#define sys_card_detect()     ((bool(*)())COM_SYS_CARD_DETECT)()

#if 0
extern bool sys_usb_detect_inner(void);
#endif
#define sys_usb_detect()      ((bool(*)())COM_SYS_USB_DETECT)()

#if 0
extern bool sys_uhost_detect_inner(void);
#endif
#define sys_uhost_detect()    ((bool(*)())COM_SYS_UHOST_DETECT)()

//for ap switch
#if 0
extern void com_ap_switch_deal_inner(app_result_e app_retsult);
#endif
#define com_ap_switch_deal(a)           ((void(*)(app_result_e))COM_COM_AP_SWITCH_DEAL)((a))

#if 0
extern uint8 com_ap_switch_ask_next_func_inner(app_result_e app_result);
#endif
#define com_ap_switch_ask_next_func(a)  ((uint8(*)(app_result_e))COM_COM_AP_SWITCH_ASK_NEXT_FUNC)((a))

#if 0
extern bool com_ap_switch_check_inner(uint8 app_func_id);
#endif
#define com_ap_switch_check(a)          ((bool(*)(uint8))COM_COM_AP_SWITCH_CHECK)((a))

#if 0
extern freq_level_e com_adjust_sys_clk_inner(freq_level_e freq_level, bool sys_clk_work);
#endif
#define com_adjust_sys_clk(a,b)   ((freq_level_e(*)(freq_level_e , bool ))COM_COM_ADJUST_SYS_CLK)((a),(b))

//文件操作
#if 0
extern uint32 com_file_open_inner(uint8 disk_type, uint8 *file_name, uint8 open_mode);
#endif
#define com_file_open(a,b,c)   ((uint32(*)(uint8 , uint8 *, uint8))COM_COM_FILE_OPEN)((a),(b),(c))

#if 0
extern bool com_file_close_inner(uint32 fhandle);
#endif
#define com_file_close(a)      ((bool(*)(uint32))COM_COM_FILE_CLOSE)((a))

#if 0
extern uint32 com_file_read_inner(uint8 *read_buf, uint32 byte_count, uint32 fhandle);
#endif
#define com_file_read(a,b,c)   ((uint32(*)(uint8 *, uint32 , uint32))COM_COM_FILE_READ)((a),(b),(c))

#if 0
extern bool com_file_write_inner(uint8 *write_buf, uint32 byte_count, uint32 fhandle);
#endif
#define com_file_write(a,b,c)  ((bool(*)(uint8 *, uint32 , uint32))COM_COM_FILE_WRITE)((a),(b),(c))

#if 0
extern bool com_file_seek_inner(int32 file_offset, uint8 seek_type, uint32 fhandle);
#endif
#define com_file_seek(a,b,c)   ((bool(*)(int32 , uint8 , uint32))COM_COM_FILE_SEEK)((a),(b),(c))

#if 0
extern uint32 com_file_tell_inner(uint32 fhandle);
#endif
#define com_file_tell(a)       ((uint32(*)(uint32))COM_COM_FILE_TELL)((a))

#if 0
extern void set_need_update_vm_inner(uint8 flag);
#endif
#define set_need_update_vm(a)       ((void(*)(uint8))COM_SET_NEED_UPDATE_VM)((a))

#if 0
extern uint8 get_need_update_vm_inner(void);
#endif
#define get_need_update_vm()        ((uint8(*)(void))COM_GET_NEED_UPDATE_VM)()

#if 0
extern void set_ic_pa_inited_inner(bool inited);
#endif
#define set_ic_pa_inited(a)         ((void(*)(bool))COM_SET_IC_PA_INITED)((a))

#if 0
extern bool get_ic_pa_inited_inner(void);
#endif
#define get_ic_pa_inited()          ((bool(*)(void))COM_GET_IC_PA_INITED)()

#if 0
extern void set_enter_dut_test_inner(uint8 test);
#endif
#define set_enter_dut_test(a)       ((void(*)(uint8))COM_SET_ENTER_DUT_TEST)((a))

#if 0
extern uint8 get_enter_dut_test_inner(void);
#endif
#define get_enter_dut_test()        ((uint8(*)(void))COM_GET_ENTER_DUT_TEST)()

#if 0
extern void clear_fs_vram_info_inner(void);
#endif
#define clear_fs_vram_info()        ((void(*)(void))COM_CLEAR_FS_VRAM_INFO)()

typedef struct
{
    short play_cur_energy;
    short play_cur_energy_rms;
    short mic_cur_energy;
    short mic_cur_energy_rms;
    short freq_point_energy[10];
} sound_energy_t;

#if 0
extern void com_update_sound_energy_inner(sound_energy_t *p_energy);
#endif
#define com_update_sound_energy(a)  ((void(*)(sound_energy_t*))COM_COM_UPDATE_SOUND_ENERGY)((a))

#if 0
extern void com_get_sound_energy_inner(sound_energy_t *p_energy);
#endif
#define com_get_sound_energy(a)     ((void(*)(sound_energy_t*))COM_COM_GET_SOUND_ENERGY)((a))

/*
    FIRST表示第一次进入录音，打开文件系统，创建文件
    NEXT表示按下f1，下一个录音，这个时候重新创建下一个文件，但是文件系统不需要关闭
*/

#define FILE_FIRST 0
#define FILE_NEXT  1

typedef enum
{
    REC_FS_ERR_NONE = 0,
    REC_FS_ERR_DISK_NOT_EXIST = 1,
    REC_FS_ERR_SCENCE_DISK_ERR = 2,
    REC_FS_ERR_SCENCE_CANNOT_GET_DISK_SPACE =3,
    REC_FS_ERR_SCENCE_PATH_SET_ERR = 4,
    REC_FS_ERR_SCENCE_DIR_CREATE_ERR = 5,
    REC_FS_ERR_SCENCE_DIR_CD_ERR = 6,
    REC_FS_ERR_SCENCE_FILE_MAX = 7,
    REC_FS_SCENCE_SEEK_FILE_ERR = 8,
    REC_FS_ERR_CREATE_FILENAME_NG = 9,
    REC_FS_ERR_SCENCE_GET_FILE_INFO_ERR = 10,
    REC_FS_ERR_GEN_FILENAME_NG = 11,
    REC_FS_ERR_SCENCE_CREATE_FILE_ERR = 12,
    REC_FS_ERR_SCENCE_DISK_FULL = 13,
    REC_FS_ERR_OTHER = 14,
} record_fs_err_type_e;

typedef struct
{
    uint32 file_sys_id;
    fhandle_t *file_handle;
    uint32 fs_free_space;
    uint8* name;
} record_param_t;

#define DISK_DEFAULT  0xff //系统按照一定规则自动选择
#if 0
extern uint8 com_record_disk_init_inner(uint8 diskno);
#endif
#define com_record_disk_init(a)           ((uint8(*)(uint8))COM_RECORD_DISK_INIT)((a))

#if 0
extern uint8 com_record_create_file_inner(uint8 mode, record_param_t* p_param);
#endif
#define com_record_create_file(a,c)       ((uint8(*)(uint8, record_param_t*))COM_RECORD_CREATE_FILE)((a),(c))

#if 0
extern uint8 com_record_close_file_inner(uint8 mode);
#endif
#define com_record_close_file(a)          ((uint8(*)(uint8))COM_RECORD_CLOSE_FILE)(a)

#if 0
extern uint8 com_record_disk_exit_inner(void);
#endif
#define com_record_disk_exit()            ((uint8(*)(void))COM_RECORD_DISK_EXIT)()

#if 0
extern bool com_record_rec_file_recover_inner(void);
#endif
#define com_record_recover_record_file()  ((bool(*)(void))COM_RECORD_RECOVER_RECORD_FILE)()

typedef struct
{
    uint32 start_cluster;
    uint32 write_pages;
    uint32 record_time;
} record_file_update_t;

#if 0
extern void com_record_update_record_file_inner(bool force_update, record_file_update_t *p_file_update);
#endif
#define com_record_update_record_file(a,b)  ((void(*)(bool, record_file_update_t *))COM_RECORD_UPDATE_RECORD_FILE)((a),(b))

#if 0
extern uint8 com_record_get_disk_type_inner(void);
#endif
#define com_record_get_disk_type()          ((uint8(*)(void))COM_RECORD_GET_DISK_TYPE)()

#if 0
extern void com_record_update_record_status_inner(record_status_e record_sta);
#endif
#define com_record_update_record_status(a)  ((void(*)(record_status_e))COM_RECORD_UPDATE_RECORD_STATUS)((a))


#if 0
extern bool com_ok_mic_idle_play_inner(void);
#endif
#define com_ok_mic_idle_play()            ((bool(*)(void))COM_COM_OK_MIC_IDLE_PLAY)()

#if 0
extern bool com_ok_mic_idle_stop_inner(void);
#endif
#define com_ok_mic_idle_stop()            ((bool(*)(void))COM_COM_OK_MIC_IDLE_STOP)()

#endif
