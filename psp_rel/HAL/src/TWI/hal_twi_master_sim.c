#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

//��Ҫ�ȴ�TWI SlaveоƬ�жϴ�����Щ�ӿ�оƬ�������Cache��������ʱ��ϳ���������Ҫ�ȴ�
//ATS2819XоƬTWI Slave����Ҫ�ȴ�
//#define NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE

twi_hw_info_t t_twi_hw_info;

static void TWI_Init(void);
static void TWI_Delay(void);
static void TWI_SDA_high_hold(bool delay);
static void TWI_SDA_low_hold(bool delay);
static void TWI_SCL_high_hold(bool delay);
static void TWI_SCL_low_hold(bool delay);

static void TWI_SDA_Output_En(uint8 level, bool delay);
static void TWI_SDA_Input_En(bool delay);
static uint8 TWI_Data_Input(void);

static void TWI_Start(void);
static void TWI_Stop(void);

static void TWI_WriteByte(uint8 dat);
static uint8 TWI_ReadByte(void);
static void TWI_SendAck(uint8 ack);
static uint8 TWI_GetAck(void);


/*
 *******************************************************************************
 *             void TWI_Init(gpio_init_cfg_t* gpio, uint8* delay)
 *
 * Description : I2C��ʼ��
 *
 * Arguments   : ģ��I2C  �������õ�IO ,  ������ʱ

 * Returns     : NULL

 * Notes       :
 *
 *******************************************************************************
 */
static void TWI_Init(void)
{
    TWI_SDA_high_hold(TRUE);
    TWI_SCL_high_hold(TRUE);
}

/************************************************************************
 * Description:  ģ��TWI  ����TWI ������ʱ
 * Input:   ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_Delay(void)
{
    g_p_sys_hal_cbks->hal_udelay(t_twi_hw_info.delay_chain_us);
}

/************************************************************************
 * Description:  ģ��TWI  ��������������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_SDA_high_hold(bool delay)
{
    hal_ioctl_pad(t_twi_hw_info.sda_pad, t_twi_hw_info.sda_func, (void *) 1);

    if (delay == TRUE)
    {
        TWI_Delay();
    }
}

/************************************************************************
 * Description:  ģ��TWI  ��������������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_SDA_low_hold(bool delay)
{
    hal_ioctl_pad(t_twi_hw_info.sda_pad, t_twi_hw_info.sda_func, (void *) 0);

    if (delay == TRUE)
    {
        TWI_Delay();
    }
}

/************************************************************************
 * Description:  ģ��TWI  ����ʱ��������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_SCL_high_hold(bool delay)
{
    hal_ioctl_pad(t_twi_hw_info.scl_pad, t_twi_hw_info.scl_func, (void *) 1);

    if (delay == TRUE)
    {
        TWI_Delay();
    }
}

/************************************************************************
 * Description:  ģ��TWI  ����ʱ��������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_SCL_low_hold(bool delay)//g_twi_hw_info
{
    hal_ioctl_pad(t_twi_hw_info.scl_pad, t_twi_hw_info.scl_func, (void *) 0);

    if (delay == TRUE)
    {
        TWI_Delay();
    }
}

/************************************************************************
 * Description:  ģ��TWI  �������������ʹ��
 * Input:   ģ��I2C  �������õ�IO
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_SDA_Output_En(uint8 level, bool delay)
{
    hal_set_pad_function(t_twi_hw_info.sda_pad, t_twi_hw_info.sda_func, &(t_twi_hw_info.sda_phy), (void *)(uint32) level);

    if (delay == TRUE)
    {
        TWI_Delay();
    }
}

/************************************************************************
 * Description:  ģ��TWI  ��������������ʹ��
 * Input:  ģ��I2C  �������õ�IO
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_SDA_Input_En(bool delay)
{
    hal_set_pad_function(t_twi_hw_info.sda_pad, t_twi_hw_info.sda_in_func, &(t_twi_hw_info.sda_phy), (void *) 0);

    if (delay == TRUE)
    {
        TWI_Delay();
    }
}

/************************************************************************
 * Description:  ģ��TWI  ���߲���������������
 * Input:   ģ��I2C  �������õ�IO
 * Output: ���������ϲ�������bit ֵ��ret  bit0
 * Note:
 *************************************************************************
 **/
static uint8 TWI_Data_Input(void)
{
    uint8 value;

    hal_ioctl_pad(t_twi_hw_info.sda_pad, t_twi_hw_info.sda_in_func, &value);

    return value;
}

/*
 *******************************************************************************
 *             TWI_Start
 *
 * Description :  ģ��TWI   Start  ����
 *
 * Arguments   : ģ��I2C  �������õ�IO ,  ������ʱ
 *
 * Returns     : NULL
 *
 * Notes       :
 *
 *******************************************************************************
 */
/*
 ____
 SDA:    |_______

 ______
 SCL:      |_____
 */

static void TWI_Start(void) /*TWI start command*/
{
    TWI_SCL_low_hold(TRUE); /*ǯסTWI���ߣ�SCLΪ�͵�ƽ��׼�����ͻ�������� */
    TWI_SDA_high_hold(TRUE); /*������ʼ�����������ź�*/
    TWI_SCL_high_hold(TRUE); /*��ʼ��������ʱ�����4.7us,��ʱ*/
    TWI_SDA_low_hold(TRUE); /*������ʼ�ź�*//* ��ʼ��������ʱ�����4��s*/
    TWI_SCL_low_hold(TRUE); /*ǯסTWI���ߣ�SCLΪ�͵�ƽ��׼�����ͻ�������� */
}

/*
 *******************************************************************************
 *             TWI_Stop
 *
 * Description :  ģ��TWI  Stop  ����
 *
 * Arguments   : ģ��I2C  �������õ�IO ,  ������ʱ
 *
 * Returns     : NULL
 *
 * Notes       :
 *
 *******************************************************************************
 */
/*
 _______
 SDA: _______|
 ________
 SCL: ______|
 */

static void TWI_Stop(void) /*TWI stop command*/
{
    TWI_SDA_low_hold(TRUE); /*���ͽ��������������ź�*/
    TWI_SCL_high_hold(TRUE); /*������������ʱ�����4��s*/
    TWI_SDA_high_hold(TRUE); /*����TWI���߽����ź�*/
}

/*
 *******************************************************************************
 *             void TWI_WriteByte(uint8 dat)
 *
 * Description : д1���ֽڵ����ݵ�slave
 *
 * Arguments   : 1���ֽ�����

 *
 * Returns     : 1 ack; 0 nack
 *
 * Notes       :
 *
 *******************************************************************************
 */
static void TWI_WriteByte(uint8 dat)
{
    uint8 i;
    for (i = 0; i < 8; i++)
    {
        if (((dat << i) & 0x80) != 0) //����һ���ֽڣ��Ӹ�bit ����bit ���η���
        {
            TWI_SDA_high_hold(TRUE);
        }
        else
        {
            TWI_SDA_low_hold(TRUE);
        }
        TWI_SCL_high_hold(TRUE);
        TWI_SCL_low_hold(FALSE);
    }
}

/*
 *******************************************************************************
 *             uint8 TWI_ReadByte(void)
 *
 * Description : ��ȡ1���ֽڵ�����,����ack����nack��slave
 *
 * Arguments   : unsigned char ack: 1 ack; 0 nack

 *
 * Returns     : ����������
 *
 * Notes       :
 *
 *******************************************************************************
 */
static uint8 TWI_ReadByte(void)
{
    uint8 t = 8, dat = 0, bit;

    TWI_SDA_Input_En(TRUE);

    do
    {
        TWI_SCL_high_hold(TRUE);

        dat <<= 1;
        bit = TWI_Data_Input();
        if (bit != 0)
        {
            dat |= 0x01;
        } //��I2C �������ϲ���һ���ֽڵ����ݣ��ǴӸ�bit ����bit ����

        TWI_SCL_low_hold(TRUE);
        --t;
    }
    while (t != 0);

    TWI_SDA_Output_En(bit, TRUE);

    return dat;
}

/*
 *******************************************************************************
 *             void TWI_SendAck(uint8 ack)
 *
 * Description : ����ack����nack
 *
 * Arguments   : unsigned char ack: 1 noack; 0 ack

 *
 * Returns     :
 *
 * Notes       :
 *
 *******************************************************************************
 */
static void TWI_SendAck(uint8 ack)
{
    if (ack != 0)
    {
        TWI_SDA_high_hold(TRUE); //nack
    }
    else
    {
        TWI_SDA_low_hold(TRUE); //ack
    }
    TWI_SCL_high_hold(TRUE);
    TWI_SCL_low_hold(TRUE);
}
/*
 *******************************************************************************
 *             uint8 TWI_GetAck(void)
 *
 * Description : Return Ack From the slave device, one is Ack, but zero NoAck.
 *
 * Arguments   : unsigned char ack: 1 noack; 0 ack

 *
 * Returns     :
 *
 * Notes       :   ��ȡ���豸�˷��͹�������Ӧ�ź�Ack  or  NAck
 *
 *******************************************************************************
 */
static uint8 TWI_GetAck(void)
{
    uint8 ack;

    TWI_SDA_Input_En(TRUE);

    TWI_SCL_high_hold(TRUE);
    if (TWI_Data_Input() != 0) //ֻ����һ��bit
    {
        ack = 0; //nack
    }
    else
    {
        ack = 1; //ack
    }

    TWI_SCL_low_hold(TRUE);

    TWI_SDA_Output_En(0, TRUE);

    return ack;
}


//=============================================================================================

//����TWI������Ϣ��ע��ýӿڲ�������PIN��
void TWI_set_hw_info_sim(uint8 idx, twi_hw_info_t *p_twi_hw_info)
{
    g_p_sys_hal_cbks->hal_memcpy(&t_twi_hw_info, p_twi_hw_info, sizeof(twi_hw_info_t));
}

/*
 ******************************************************************************
 *             uint8 hal_TWI_write_bytes(uint8 *buf, uint8 length)
 *
 * Description : ����ŵ�*buf��ַ�ﳤ��Ϊlength�����ݷ��ͳ�ȥ
 *
 * Arguments   : unsigned char *buf: ���ݴ�ŵ�ַ
 unsigned char length: ���ݳ���, ��byteΪ��λ
 *
 * Returns     : FALSE: �������ݽ��յ�nack
 TRUE: �������ݽ��յ�ack
 *
 * Notes       : ��������I2C �豸�������ݣ����������豸����Ӧ
 *                  �����ڲ��������������ṩ�ӿ�
 ********************************************************************************
 */
uint8 TWI_write_bytes_sim(uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode)
{
    uint8 ret = 0;
    bool ack;

    TWI_Init();

    TWI_Start();

    /*����slave ��ַ*/
    TWI_WriteByte(address);
    ack = TWI_GetAck();

    //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
    g_p_sys_hal_cbks->hal_udelay(200);
#endif

    if (ack != 0)
    {
        //��������
        do
        {
            TWI_WriteByte(*buf);
            buf++;
            ack = TWI_GetAck();

            //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
            g_p_sys_hal_cbks->hal_udelay(200);
#endif

            if (ack == FALSE)
            {
                break;
            }
            --length;
        }
        while (length != 0);

        if (length == 0)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }

    //��ȡģʽ������ŷ��Ͷ��������STOP
    if ((ret == 0) || (read_mode == TWI_READ_MODE_NO))
    {
        TWI_Stop();
    }

    //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
    g_p_sys_hal_cbks->hal_udelay(200);
#endif

    return ret;
}


/*
 ******************************************************************************
 *             uint8 hal_TWI_read_bytes(uint8 *buf, uint8 address, uint8 length)
 *
 * Description : ��ȡ����length
 *
 * Arguments   : unsigned char *buf: ���ݴ�ŵ�ַ
 unsigned char address: slave��ַ
 *               unsigned char length: ���ݳ���,��byteΪ��λ
 *
 * Returns     : FALSE: slave����Ӧ
 TRUE: �յ�����
 *
 * Notes       :  �����ڲ��ӿڣ��������ṩ
 *
 ********************************************************************************
 */
uint8 TWI_read_bytes_sim(uint8 idx, uint8 *buf, uint8 address, uint8 length)
{
    uint8 ret = 0;
    uint8 *p;
    bool ack;

    p = buf;

    TWI_Init();

    TWI_Start();

    /*����slave ��ַ*/
    TWI_WriteByte(address);
    ack = TWI_GetAck();

    //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
    g_p_sys_hal_cbks->hal_udelay(200);
#endif

    if (ack == FALSE)
    {
        ret = 0;
        goto recev_ret;
    }

    while (length != 0)
    {
        if (length == 1) //the last byte, sent nack
        {
            *p = TWI_ReadByte();
            TWI_SendAck(1);
        }
        else
        {
            *p = TWI_ReadByte();
            TWI_SendAck(0);
        }

        //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
        g_p_sys_hal_cbks->hal_udelay(200);
#endif

        p++;
        length--;
    }
    ret = 1;

    recev_ret:
    TWI_Stop();

    //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
    g_p_sys_hal_cbks->hal_udelay(200);
#endif

    return ret;
}

