/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ���������ṹ�嶨�塣
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_VAR_H
#define _COMMON_VAR_H

#include "psp_includes.h"

/*!
 * \brief
 *  comval_t ϵͳ����������Ͻṹ��
 */
typedef struct
{
    /*! ֧�ֿ�ѡ��  0:��֧��  1:֧�� */
    uint8 support_cardplay;
    /*!֧��lineinѡ�� 0:��֧�� 1:֧��*/
    uint8 support_linein;
    /*! ֧��FMѡ��  0:��֧��  1:֧�� */
    uint8 support_fmradio;
    /*! ֧�ֺ�̨���� 0:��֧��  1:֧�� */
    uint8 support_bt_inback;
    /*! ֧�������Ƹ� 0:��֧��  1:֧�� */
    uint8 support_btplay;
    /*! ֧���������� 0:��֧��  1:֧�� */
    uint8 support_btcall;
    /*! ֧��USB���� 0:��֧��  1:֧�� */
    uint8 support_usbbox;
    /*! ֧�ִ��������� 0:��֧��  1:֧�� */
    uint8 support_usbmsc;
    /*!֧��USB���� UHOST 0:��֧�� 1:֧��*/
    uint8 support_uhostplay;
    /*! Ӳ���������� */
    uint8 hard_support_card;
    /*! �Ƿ�֧��USB�ڸ�IPhone��� */
    uint8 support_iphonecharge;

    /*! �Ƿ�֧��LED����ʾ */
    uint8 support_led_light;
    /*! �Ƿ�֧��LED�������ʾ */
    uint8 support_led_segment;
    /*! �Ƿ�֧�ֺ���ң������ */
    uint8 support_ir_key;
    /*! �Ƿ�֧���ⲿ�����������DCDC��ѹ���� */
    uint8 support_extern_pa;
    /*! �Ƿ�֧��LINEIN�Ȱβ��� */
    uint8 support_aux_detect;
    /*! �Ƿ�֧��DC5V �Ȱβ��� */
    uint8 support_dc5v_detect;
    /*! USB Device����Ƿ�����DC5V��� */
    uint8 is_udev_det_base_dc5v_det;
    /*! �Ƿ�֧�ֵ�ص������ */
    uint8 support_bat_lradc;
} comval_t;

typedef struct
{
    /*! magic��ֵΪ 0x55AA ʱ��Ч */
    uint16 magic;

    /*! ��ǰ������ */
    uint8 main_volume;
    /*! ��ǰ�绰���� */
    uint8 call_volume;
    /*! �Ƿ�֧�� Graphics EQ ���� */
    uint8 eq_enable;
    /*! Graphics EQ ���� */
    uint8 eq_type;

    /*! �����ɫ���� */
    dev_role_e dev_role;
    /*! �������������������� */
    dev_spk_pos_e spk_pos;

#if (SUPPORT_FM_TX == 1)
    /*! FM����Ƶ�� */
    uint16 fm_tx_freq;
#endif

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    /*! LCD�Աȶ� */
    uint8 contrast;
#endif
} env_var_t;

#endif //_COMMON_VAR_H
