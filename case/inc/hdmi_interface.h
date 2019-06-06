/********************************************************************************
 *                           Mdule: EP HDMI
 *                SHENZHEN HUANYIN ELECTRONICS Ltd.
 *                           All Rights Reserved.o
 *
 * History:
 *     <author>      <time>           <version >             <desc>
 *     manfan                                1.0             build this file
 ********************************************************************************/
/*!
 * \file    hdmi_interface.h
 * \brief
 * \author  manfan
 * \version 1.0
 * \date  2018/6/11
 *******************************************************************************/

#ifndef __hdmi_interface_h__
#define __hdmi_interface_h__

#include "psp_includes.h"
#include "case_independent.h"

/*
���������
HDMI           High Definition Multimedia Interface���������ȶ�ý��ӿ�
ARC            Audio Return Channel����Ƶ�ش�ͨ��
CEC            Consumer Electronics Control����������ӿ���
CEC VOLUME     CEC ����ͬ��
CEC MUTE       CEC ����״̬ͬ��
CEC POWER UP   CEC ����
CEC STANDBY    CEC ����
*/

typedef enum
{
    HDMI_SRC_NONE,
    HDMI_SRC_ARC,
    HDMI_SRC_HDMI0,
    HDMI_SRC_HDMI1,
    HDMI_SRC_HDMI2,
}HDMI_SRC;

typedef enum
{
    HOST_SRC_NONE,
    HOST_SRC_HDMI, //HDMI1, 2, 3
    HOST_SRC_ARC,  //ARC
    HOST_SRC_OTHER,//Non-HDMI & Non-ARC
    HOST_SRC_MAX
}HOST_SRC;

#define CEC_VOLUME_MAX  100

typedef bool (*cbk_is_hdmi_func)(uint8 func_id);
typedef bool (*cbk_is_arc_func)(uint8 func_id);
typedef HOST_SRC (*cbk_get_host_src)(uint8 func_id);
typedef HDMI_SRC (*cbk_get_hdmi_src)(uint8 func_id);

typedef uint8 (*cbk_get_cur_func_id)(void);
typedef bool (*cbk_get_cec_mute)(void);
typedef uint8 (*cbk_get_sound_volume)(void);

/* �ص����� */
typedef struct
{
    cbk_is_hdmi_func   is_hdmi_cbk;
    cbk_is_arc_func    is_arc_cbk;
    cbk_get_host_src   get_host_src_cbk;
    cbk_get_hdmi_src   get_hdmi_src_cbk;
    cbk_get_cur_func_id  get_cur_func_id_cbk;
    cbk_get_cec_mute     get_cec_mute_cbk;
    cbk_get_sound_volume get_sound_volume_cbk;
} app_callback_t;

/* �������� */
typedef struct
{
    uint8 max_volume;
} app_env_var_t;

/*
!   API
*/
typedef enum
{
    API_HDMI_MODULE_INSTALL,
    API_HDMI_POWER_CONFIG,
    API_HDMI_STARTUP,
    API_HDMI_SWITCH_INPUT,
    API_HDMI_SET_VOLUME,
    API_HDMI_SET_MUTE,
    API_HDMI_GET_ARC_STATUS,
    API_HDMI_GET_FW_VERSION,
    API_HDMI_GET_LIB_VERSION,
}hdmi_api_e;

#define HDMI_ENTRY_ADDR(x) (HDMI_ENTRY_VA_START + (4 * x))

#define HDMI_HDMI_MODULE_INSTALL (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_MODULE_INSTALL))))
#define HDMI_HDMI_POWER_CONFIG   (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_POWER_CONFIG))))
#define HDMI_HDMI_STARTUP        (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_STARTUP))))
#define HDMI_HDMI_SWITCH_INPUT   (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_SWITCH_INPUT))))
#define HDMI_HDMI_SET_VOLUME     (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_SET_VOLUME))))
#define HDMI_HDMI_SET_MUTE       (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_SET_MUTE))))
#define HDMI_HDMI_GET_ARC_STATUS (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_GET_ARC_STATUS))))
#define HDMI_HDMI_GET_FW_VERSION (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_GET_FW_VERSION))))
#define HDMI_HDMI_GET_LIB_VERSION (*((uint32*)(HDMI_ENTRY_ADDR(API_HDMI_GET_LIB_VERSION))))

#if 0
extern void hdmi_module_install_inner(void);
#endif
/*
 * ��װHDMIģ��������֮��פϵͳֱ������
 */
#define hdmi_module_install() ((void(*)(void))HDMI_HDMI_MODULE_INSTALL)()

#if 0
extern void hdmi_power_config_inner(bool on);
#endif
/*
 * ���ػ���a: 0->standby, 1->power on
 * ����ϵͳ״̬�趨��ϵͳ���ڹ���״̬����power on�����ڴ���״̬����standby��
 */
#define hdmi_power_config(a) ((void(*)(bool))HDMI_HDMI_POWER_CONFIG)((a))

#if 0
extern void hdmi_startup_inner(app_callback_t *p_app_cbk, app_env_var_t *p_env_var);
#endif
/*
 * ��ʼ���ص���������ر�����������ʱ����
 */
#define hdmi_startup(a,b) ((void(*)(app_callback_t *, app_env_var_t *))HDMI_HDMI_STARTUP)((a),(b))

#if 0
extern void hdmi_switch_input_inner(uint8_t cur_id);
#endif
/*
 * ϵͳӦ�����ã� ����a:���뵱ǰapp_fund_id��
 */
#define hdmi_switch_input(a) ((void(*)(uint8_t))HDMI_HDMI_SWITCH_INPUT)((a))

#if 0
extern void hdmi_set_volume_inner(uint8_t vol);
#endif
/*
 * ����ͬ��������a:����CEC ��ǰӳ������
 * ����ͨ������ң�ؿ���ϵͳ��volume add/sub��ϵͳң��volume add/sub ��ش���������ʾ(ĳЩ���Ӳ���ʾ)
 */
#define hdmi_set_volume(a) ((void(*)(uint8_t))HDMI_HDMI_SET_VOLUME)((a))

#if 0
extern void hdmi_set_mute_inner(uint8_t mute);
#endif
/*
 * mute/unmute ͬ����a: 0->unmute, 1->mute
 * ��: ͨ������ң�ؿ���ϵͳmute/unmute��ϵͳң��mute/unmute ֮���ٰ�����ң�ؾ�Ҫmute/unmute
 */
#define hdmi_set_mute(a) ((void(*)(uint8_t))HDMI_HDMI_SET_MUTE)((a))

#if 0
extern uint8_t hdmi_get_arc_status_inner(void);
#endif
/*
 * ARC ״̬��ѯ��0->ARC disabled, 1->ARC enabled��
 * ARC ״̬��ʾHDMI ģ���ARC �Ƿ��Ѿ��򿪣��ȿ��Ա�ϵͳ���ƴ�/�رգ�Ҳ���Ա�����ͨ��CEC ��/�ر�
 */
#define hdmi_get_arc_status() ((uint8_t(*)(void))HDMI_HDMI_GET_ARC_STATUS)()

#if 0
extern uint8_t hdmi_get_fw_version_inner(uint8_t* ver);
#endif
/*
 * �汾�Ų�ѯ��a:����3 �������ַ������硱100������ʾV1.0.0
 */
#define hdmi_get_fw_version(a) ((uint8_t(*)(uint8_t*))HDMI_HDMI_GET_FW_VERSION)((a))

#if 0
extern uint8_t hdmi_get_lib_version_inner(uint8_t* ver);
#endif
/*
* hdmi.a ��汾�Ų�ѯ��a:����3 �������ַ������硱100������ʾV1.0.0
*/
#define hdmi_get_lib_version(a) ((uint8_t(*)(uint8_t*))HDMI_HDMI_GET_LIB_VERSION)((a))

#endif

