/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ�GPIO ͨ·���ԡ�
 * ���ߣ�wangli
 ********************************************************************************/

#include "ap_test_head.h"
#include "bank_include/ap_autotest_char_convert.h"
#include "ap_autotest_main_entry.h"
#include "bank_common/ap_autotest_common_entry.h"


#define PAD_GPIO_MID 32
#define PAD_GPIO_MAX 64

uint8 pad_gpio_arr[PAD_GPIO_MAX]=
{
    PAD_GPIO0,
    PAD_GPIO1,
    PAD_GPIO2,
    PAD_GPIO3,
    PAD_GPIO4,
    PAD_GPIO5,
    PAD_GPIO6,
    PAD_GPIO7,
    PAD_GPIO8,
    PAD_GPIO9,
    PAD_GPIO10,
    PAD_GPIO11,
    PAD_GPIO12,
    PAD_GPIO13,
    PAD_GPIO14,
    PAD_GPIO15,
    PAD_GPIO16,
    PAD_GPIO17,
    PAD_GPIO18,
    PAD_GPIO19,
    PAD_GPIO20,
    PAD_GPIO21,
    PAD_GPIO22,
    PAD_GPIO23,
    PAD_GPIO24,
    PAD_GPIO25,
    PAD_GPIO26,
    PAD_GPIO27,
    PAD_GPIO28,
    PAD_GPIO29,
    PAD_GPIO30,
    PAD_GPIO31,
    PAD_GPIO32,
    PAD_GPIO33,
    PAD_GPIO34,
    PAD_GPIO35,
    PAD_GPIO36,
    PAD_GPIO37,
    PAD_VRO,   //GPIO38
    PAD_VRO_S, //GPIO39
    PAD_AUX0L, //GPIO40
    PAD_AUX0R, //GPIO41
    PAD_AOUTL, //GPIO42
    PAD_AOUTR, //GPIO43
    PAD_AUX1L, //GPIO44
    PAD_AUX1R, //GPIO45
    PAD_MIC0,  //GPIO46
    PAD_MIC1,  //GPIO47
    PAD_GPIO48,
    PAD_GPIO49,
    PAD_GPIO50,
    PAD_GPIO51,
    PAD_WIO0,  //SIO0
    PAD_WIO1,  //SIO1
} ;

void led_flash_ok(void)
{
}

void led_flash_fail(void)
{
}


void act_test_report_gpio_result(gpio_test_arg_t *gpio_test_arg, test_result_e result, uint32 test_mode, \
    uint8 * log_buffer, uint32 * cur_total_len)
{
    int ret_val;
    return_result_t *return_data;
    uint16 trans_bytes = 0;

    if (result == TEST_PASS)
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }

    if (test_mode != TEST_MODE_CARD)
    {
        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer)
        {
            libc_printf("report_gpio_result: stub_rw_buffer null\n");
            while(1){}
        }

        return_data = (return_result_t *) (stub_rw_buffer);

        return_data->test_id = TESTID_GPIO_TEST;

        return_data->test_result = ret_val;

        uint32_to_unicode(gpio_test_arg->gpioA_value, return_data->return_arg, &trans_bytes, 16);// gpioA_value�ǳ���������Ǹ�ֵ

        //��ʹû�м��gpioB��sio�����ǻ�Ҫ���ո�ʽ����д
        //��Ӳ����ָ���','
        return_data->return_arg[trans_bytes++] = 0x002c;

        gpio_test_arg->gpioB_value = 0x11;

        uint32_to_unicode(gpio_test_arg->gpioB_value, &(return_data->return_arg[trans_bytes]), &trans_bytes, 16);

        //��Ӳ����ָ���','
        return_data->return_arg[trans_bytes++] = 0x002c;

        gpio_test_arg->gpioSIO_value = 0x11;

        uint32_to_unicode(gpio_test_arg->gpioSIO_value, &(return_data->return_arg[trans_bytes]), &trans_bytes, 16);

        //��ӽ�����
        return_data->return_arg[trans_bytes++] = 0x0000;

        //�������δ���ֽڶ��룬Ҫ���ֽڶ��봦��
        if ((trans_bytes % 2) != 0)
        {
            return_data->return_arg[trans_bytes++] = 0x0000;
        }

        STUB_REPORT_RESULT((uint8*)return_data, trans_bytes * 2 + 4);

        sys_free(stub_rw_buffer);
    }
    else
    {
        ATT_TEST_ADD_RESULT_TO_SD_LOG("gpio test", ret_val, log_buffer, cur_total_len);
    }
}


/* GPIO�ڲ��� */

void test_set_gpio_in(ic_pad_e pad,bool pu_en,bool pd_en)
{
    pad_phy_para_t pad_phy;
    pad_phy.pad_pu_en = pu_en;
    pad_phy.pad_pd_en = pd_en;
    pad_phy.smit_en = 0;
    pad_phy.pad_driver = 1;
    set_pad_function(pad,  PAD_FUNC_GPIOIN, &(pad_phy), (void *) 0);
}

void test_set_gpio_out(ic_pad_e pad,uint8 level,bool pu_en,bool pd_en)
{
    pad_phy_para_t pad_phy;
    pad_phy.pad_pu_en = pu_en;
    pad_phy.pad_pd_en = pd_en;
    pad_phy.smit_en = 0;
    pad_phy.pad_driver = 3;
    set_pad_function(pad,  PAD_FUNC_GPIOOUT, &(pad_phy), (void *)(uint32) level);
}

uint8 test_get_gpio_level(ic_pad_e pad)
{
    uint8 value;

    ioctl_pad(pad, PAD_FUNC_GPIOIN, &value);

    return value;
}

void test_set_gpio_level(ic_pad_e pad,uint8 level)
{
    ioctl_pad(pad, PAD_FUNC_GPIOOUT, (void *)(uint32) level);
}
/* GPIO�ڲ��� end */


int32 test_gpio_short(uint32 reg_val_A, uint32 reg_val_B,uint32 reg_valSIO,uint8 *index)
{
    uint8 i,j;
    uint32 reg_value;
    uint32 gpio_dat;
    ic_pad_e cur_cir_pad,tmp_cir_pad;

    uint32 reg_val;

    //gpio5��Ϊ���Կ���io

    //ȡ������������
    //GPIO5Ĭ������ߵ�ƽ��IO�ڽ���������
    test_set_gpio_out(PAD_GPIO5,1,0,0);
    test_set_gpio_level(PAD_GPIO5,1);
    sys_mdelay(1);

    //gpio[0~63]����
    for (i = 0; i < PAD_GPIO_MAX; i++)
    {
        if(i<PAD_GPIO_MID)
        {
            reg_val = reg_val_A;
            reg_value = (1 << i);
        }
        else
        {
            reg_val = reg_val_B;
            reg_value = (1 << (i-32));
        }

        cur_cir_pad = pad_gpio_arr[i];
        if ((reg_val & reg_value) != 0)
        {
            //���Թܽ�Ϊ����״̬
            test_set_gpio_in(cur_cir_pad,0,0);
            sys_mdelay(1);

            //ʹ�������ܽ�Ϊ���״̬
            //�����ܽ�����ߵ�ƽ

            for (j = 0; j < PAD_GPIO_MAX; j++)
            {
                if(j<PAD_GPIO_MID)
                {
                    reg_val = reg_val_A;
                    reg_value = (1 << j);
                }
                else
                {
                    reg_val = reg_val_B;
                    reg_value = (1 << (j-32));
                }
                tmp_cir_pad = pad_gpio_arr[j];
                if (((reg_val & reg_value) != 0) &&(j!=i))
                {
                    test_set_gpio_out(tmp_cir_pad,1,1,0);
                }
            }

            sys_mdelay(1);

            //��ȡ������io��level
            gpio_dat = test_get_gpio_level(cur_cir_pad);

            //�����ܽ�����͵�ƽ

            for (j = 0; j < PAD_GPIO_MAX; j++)
            {
                if(j<PAD_GPIO_MID)
                {
                    reg_val = reg_val_A;
                    reg_value = (1 << j);
                }
                else
                {
                    reg_val = reg_val_B;
                    reg_value = (1 << (j-32));
                }

                tmp_cir_pad = pad_gpio_arr[j];
                if (((reg_val & reg_value) != 0) &&(j!=i))
                {
                    test_set_gpio_out(tmp_cir_pad,0,0,1);
                }
            }

            sys_mdelay(1);

            //���Թܽ������ƽ�б仯��˵�����ڶ�·
            if(test_get_gpio_level(cur_cir_pad) != gpio_dat)
            {
                libc_printf_error("TEST_GPIO_SHORT\n");
                *index = i;
                return TEST_GPIO_SHORT;
            }
            else
            {
                //һֱ��⵽�ߵ�ƽ��˵��VCC��·
                if (gpio_dat != 0)
                {
                    libc_printf_error("TEST_GPIO_SHORT_VCC\n");
                    *index = i;
                    return TEST_GPIO_SHORT_VCC;
                }
            }
        }
    }

    return TEST_PASS;
}


//����GPIO�Ƿ���ڶ�·�ӵ�
int32 test_gpio_short_gnd(uint32 reg_val_A, uint32 reg_val_B,uint32 reg_valSIO,uint8 *index)
{
    uint8 i,j;
    uint32 reg_value;
    uint32 gpio_dat;
    ic_pad_e cur_cir_pad,tmp_cir_pad;

    uint32 reg_val;

    //����ioĬ������͵�ƽ��IO�ڽ���������
    test_set_gpio_out(PAD_GPIO5,0,0,0);
    test_set_gpio_level(PAD_GPIO5,0);
    sys_mdelay(1);

    //ȡ������������
    //���Թܽ�Ϊ����״̬

    for (i = 0; i < PAD_GPIO_MAX; i++)
    {
        if(i<PAD_GPIO_MID)
        {
            reg_val = reg_val_A;
            reg_value = (1 << i);
        }
        else
        {
            reg_val = reg_val_B;
            reg_value = (1 << (i-32));
        }

        tmp_cir_pad = pad_gpio_arr[i];
        if ((reg_val & reg_value) != 0)
        {
            test_set_gpio_in(tmp_cir_pad,0,0);
        }
    }

    //��ȡ����pin�ĵ�ƽ
    for (i = 0; i < PAD_GPIO_MAX; i++)
    {
        if(i<PAD_GPIO_MID)
        {
            reg_val = reg_val_A;
            reg_value = (1 << i);
        }
        else
        {
            reg_val = reg_val_B;
            reg_value = (1 << (i-32));
        }
        tmp_cir_pad = pad_gpio_arr[i];
        if ((reg_val & reg_value) != 0)
        {
            //���Թܽ������ƽΪ0��˵���̽ӵ���
            if (test_get_gpio_level(tmp_cir_pad) == 0)
            {
                *index = i;
                return TEST_GPIO_SHORT_GND;
            }
        }
    }
    return TEST_PASS;
}


// gpio�����Ӻ���������GPIO�Ƿ���ڶ�·
bool test_gpio_open(uint32 reg_val_A, uint32 reg_val_B,uint32 reg_valSIO,uint8 *index)
{
    uint8 i,j;
    uint32 reg_value;
    ic_pad_e cur_cir_pad,tmp_cir_pad;

    uint32 reg_val;

    //����IO Ĭ������ߵ�ƽ��IO�ڽ���������
    test_set_gpio_out(PAD_GPIO5,1,1,0);

    //ȡ������������
    //ʹ�ܹܽ�����


    for (i = 0; i < PAD_GPIO_MAX; i++)
    {
        if(i<PAD_GPIO_MID)
        {
            reg_val = reg_val_A;
            reg_value = (1 << i);
        }
        else
        {
            reg_val = reg_val_B;
            reg_value = (1 << (i-32));
        }

        tmp_cir_pad = pad_gpio_arr[i];
        if ((reg_val & reg_value) != 0)
        {
            test_set_gpio_in(tmp_cir_pad,0,0);
        }
    }

    //�ֱ��ÿ������io����
    for (i = 0; i < PAD_GPIO_MAX; i++)
    {
        if(i<PAD_GPIO_MID)
        {
            reg_val = reg_val_A;
            reg_value = (1 << i);
        }
        else
        {
            reg_val = reg_val_B;
            reg_value = (1 << (i-32));
        }


        tmp_cir_pad = pad_gpio_arr[i];

        if ((reg_val & reg_value) != 0)
        {
            //ʹ���ڲ�����
            test_set_gpio_in(tmp_cir_pad,1,0);

            //�ȴ������ȶ�
            sys_mdelay(2);

            //��ȡgpio��ֵ
            if (test_get_gpio_level(tmp_cir_pad) != 0)
            {
                libc_printf_error("TEST_GPIO_OPEN\n");
                *index = i;
                return TEST_GPIO_OPEN;
            }
        }
    }


    return TEST_PASS;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    GPIO���Ժ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music_test
 * \note
 *******************************************************************************/
//��Ҫ���Ե�gpio   ----3/6/8/9/12/13/20/21/22/23/24/25/26    ʮ�����ƾ���    (gpio8�Ǵ��ڣ�
/*
gpio_a   :07f03348(����gpio8)
gpio_b   :00000000
gpio_sio :000000c0
*/

test_result_e test_gpio(gpio_test_arg_t *gpio_test_arg)
{
    test_result_e test_result;
    uint8 tmp_index;
    uint8* p_err_pin_index ;

    p_err_pin_index= &tmp_index;

    test_result = TEST_PASS;
    test_result = test_gpio_short(gpio_test_arg->gpioA_value,gpio_test_arg->gpioB_value, gpio_test_arg->gpioSIO_value, p_err_pin_index);

    if(test_result != TEST_PASS)
    {
        libc_printf_error("GPIO TEST(SHORT) NG , err pin is %ld\n",*p_err_pin_index);
        goto end_test;
    }
    libc_printf("GPIO TEST(SHORT) OK\n");

    //�̽ӵز���
    test_result = test_gpio_short_gnd(gpio_test_arg->gpioA_value,gpio_test_arg->gpioB_value, gpio_test_arg->gpioSIO_value, p_err_pin_index);
    if (test_result != TEST_PASS)
    {
        libc_printf_error("GPIO TEST(SHORT GND) NG , err pin is %ld\n",*p_err_pin_index);
        goto end_test;
    }
    libc_printf("GPIO TEST(SHORT GND) OK\n");

    //����GPIO�Ƿ���ڶ�·
    test_result = test_gpio_open(gpio_test_arg->gpioA_value,gpio_test_arg->gpioB_value, gpio_test_arg->gpioSIO_value, p_err_pin_index);
    if (test_result != TEST_PASS)
    {
        libc_printf_error("GPIO TEST(OPEN) NG , err pin is %ld\n",*p_err_pin_index);
        goto end_test;
    }
    libc_printf("GPIO TEST(OPEN) OK\n");


    return test_result;

    end_test:

    gpio_test_arg->gpioA_value   = *p_err_pin_index;
    gpio_test_arg->gpioB_value   = test_result;
    gpio_test_arg->gpioSIO_value = 0;

    return test_result;
}


int32 act_test_gpio_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    test_result_e ret_val;

    gpio_test_arg_t *gpio_test_arg = (gpio_test_arg_t *) arg_buffer;

    ret_val = test_gpio(gpio_test_arg);

    act_test_report_gpio_result(gpio_test_arg, ret_val, test_mode, log_buffer, cur_log_len);

    if (TEST_PASS == ret_val)
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}



