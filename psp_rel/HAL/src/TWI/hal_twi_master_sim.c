#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

//需要等待TWI Slave芯片中断处理，有些接口芯片代码放在Cache运行起来时间较长，所以需要等待
//ATS2819X芯片TWI Slave就需要等待
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
 * Description : I2C初始化
 *
 * Arguments   : 模拟I2C  操作所用的IO ,  操作延时

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
 * Description:  模拟TWI  总线TWI 操作延时
 * Input:   操作延时
 * Output: none
 * Note:
 *************************************************************************
 **/
static void TWI_Delay(void)
{
    g_p_sys_hal_cbks->hal_udelay(t_twi_hw_info.delay_chain_us);
}

/************************************************************************
 * Description:  模拟TWI  总线数据线拉高
 * Input:   模拟I2C  操作所用的IO ,  操作延时
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
 * Description:  模拟TWI  总线数据线拉低
 * Input:   模拟I2C  操作所用的IO ,  操作延时
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
 * Description:  模拟TWI  总线时钟线拉高
 * Input:   模拟I2C  操作所用的IO ,  操作延时
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
 * Description:  模拟TWI  总线时钟线拉低
 * Input:   模拟I2C  操作所用的IO ,  操作延时
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
 * Description:  模拟TWI  总线数据线输出使能
 * Input:   模拟I2C  操作所用的IO
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
 * Description:  模拟TWI  总线数据线输入使能
 * Input:  模拟I2C  操作所用的IO
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
 * Description:  模拟TWI  总线采样数据线上数据
 * Input:   模拟I2C  操作所用的IO
 * Output: 在数据线上采样到的bit 值，ret  bit0
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
 * Description :  模拟TWI   Start  流程
 *
 * Arguments   : 模拟I2C  操作所用的IO ,  操作延时
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
    TWI_SCL_low_hold(TRUE); /*钳住TWI总线，SCL为低电平，准备发送或接收数据 */
    TWI_SDA_high_hold(TRUE); /*发送起始条件的数据信号*/
    TWI_SCL_high_hold(TRUE); /*起始条件建立时间大于4.7us,延时*/
    TWI_SDA_low_hold(TRUE); /*发送起始信号*//* 起始条件锁定时间大于4μs*/
    TWI_SCL_low_hold(TRUE); /*钳住TWI总线，SCL为低电平，准备发送或接收数据 */
}

/*
 *******************************************************************************
 *             TWI_Stop
 *
 * Description :  模拟TWI  Stop  流程
 *
 * Arguments   : 模拟I2C  操作所用的IO ,  操作延时
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
    TWI_SDA_low_hold(TRUE); /*发送结束条件的数据信号*/
    TWI_SCL_high_hold(TRUE); /*结束条件建立时间大于4μs*/
    TWI_SDA_high_hold(TRUE); /*发送TWI总线结束信号*/
}

/*
 *******************************************************************************
 *             void TWI_WriteByte(uint8 dat)
 *
 * Description : 写1个字节的数据到slave
 *
 * Arguments   : 1个字节数据

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
        if (((dat << i) & 0x80) != 0) //对于一个字节，从高bit 到低bit 依次发送
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
 * Description : 读取1个字节的数据,并回ack或者nack给slave
 *
 * Arguments   : unsigned char ack: 1 ack; 0 nack

 *
 * Returns     : 读到的数据
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
        } //从I2C 数据线上采样一个字节的数据，是从高bit 到低bit 采样

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
 * Description : 发送ack还是nack
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
 * Notes       :   获取从设备端发送过来的响应信号Ack  or  NAck
 *
 *******************************************************************************
 */
static uint8 TWI_GetAck(void)
{
    uint8 ack;

    TWI_SDA_Input_En(TRUE);

    TWI_SCL_high_hold(TRUE);
    if (TWI_Data_Input() != 0) //只采样一个bit
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

//设置TWI总线信息，注意该接口不会配置PIN脚
void TWI_set_hw_info_sim(uint8 idx, twi_hw_info_t *p_twi_hw_info)
{
    g_p_sys_hal_cbks->hal_memcpy(&t_twi_hw_info, p_twi_hw_info, sizeof(twi_hw_info_t));
}

/*
 ******************************************************************************
 *             uint8 hal_TWI_write_bytes(uint8 *buf, uint8 length)
 *
 * Description : 将存放到*buf地址里长度为length的数据发送出去
 *
 * Arguments   : unsigned char *buf: 数据存放地址
 unsigned char length: 数据长度, 以byte为单位
 *
 * Returns     : FALSE: 发送数据接收到nack
 TRUE: 发送数据接收到ack
 *
 * Notes       : 由主控向I2C 设备发送数据，接收来自设备的响应
 *                  驱动内部函数，不向外提供接口
 ********************************************************************************
 */
uint8 TWI_write_bytes_sim(uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode)
{
    uint8 ret = 0;
    bool ack;

    TWI_Init();

    TWI_Start();

    /*发送slave 地址*/
    TWI_WriteByte(address);
    ack = TWI_GetAck();

    //wait TWI slave interrupt handle
#ifdef NEED_WAIT_TWI_SLAVE_INTERRUPT_HANDLE
    g_p_sys_hal_cbks->hal_udelay(200);
#endif

    if (ack != 0)
    {
        //发送数据
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

    //读取模式会紧接着发送读命令，无须STOP
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
 * Description : 读取长度length
 *
 * Arguments   : unsigned char *buf: 数据存放地址
 unsigned char address: slave地址
 *               unsigned char length: 数据长度,以byte为单位
 *
 * Returns     : FALSE: slave不响应
 TRUE: 收到数据
 *
 * Notes       :  驱动内部接口，不向外提供
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

    /*发送slave 地址*/
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

