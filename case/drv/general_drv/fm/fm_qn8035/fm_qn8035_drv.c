/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM QN8035 模组驱动程序实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "fm_drv.h"
#include "fm_qn8035.h"

#if (SUPPORT_FM_FUNC == 1)

//设置数据的全局变量
set_value_t g_set_value;

//reg save
uint8 qnd_div1;
uint8 qnd_div2;
uint8 qnd_nco;
uint8 qnd_Chip_VID;//FM 芯片版本号
//QN8035模组bug，855设置不成功
uint8 g_freq_855;

const uint16 rssi_snr_TH_tbl[10] =
{
    CCA_SENSITIVITY_LEVEL_0, CCA_SENSITIVITY_LEVEL_1,
    CCA_SENSITIVITY_LEVEL_2, CCA_SENSITIVITY_LEVEL_3,
    CCA_SENSITIVITY_LEVEL_4, CCA_SENSITIVITY_LEVEL_5,
    CCA_SENSITIVITY_LEVEL_6, CCA_SENSITIVITY_LEVEL_7,
    CCA_SENSITIVITY_LEVEL_8, CCA_SENSITIVITY_LEVEL_9

};

/******************************************************************************
 * \int _is_freq_valid(uint32 freq)
 * \par  Description:
 * \是否为有效频点值
 * \param[in]   freq param1 输入的频率值
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int _is_freq_valid(uint32 freq)
{
    if ((freq < g_set_value.freq_low) || (freq > g_set_value.freq_high))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*********************************************************************************
 * \         int QND_WriteReg(uint8 RegAddr, uint8 Data)
 * \
 * \Description : 写寄存器
 * \
 * \Arguments :  RegAddr:要写的寄存器;Data:要写入的寄存器值
 * \Returns :   0 success -1 fail
 * \
 * \Notes :
 * \
*********************************************************************************/
int QND_WriteReg(uint8 RegAddr, uint8 Data)
{
    uint8 i;
    uint32 flags;
    int result = 0;
    uint8 tempbuf[2];
    uint8 ret = 0;

    //register addr
    tempbuf[0] = RegAddr;
    //register data
    tempbuf[1] = Data;

    for (i = 0; i < 10; i++)
    {
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        flags = sys_local_intc_save(0x01|(1<<IRQ_TIMER3));//关中断
#else
        sys_os_sched_lock();
#endif
        ret = twi_write_bytes(FM_TWI_MASTER_MODE, twi_master_idx, tempbuf, I2C_WRITE_ADDR, 2, TWI_READ_MODE_NO);
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        sys_local_intc_restore(flags);//开中断
#else
        sys_os_sched_unlock();
#endif

        //send 3 byte to fm module
        if (ret != 0)
        {
            break;
        }
    }

    if (i == 10)
    {
        result = -1;
    }

    if (result == -1)
    {
        libc_printf_warning("QND_WriteReg fail!\n");
    }

    return result;
}
/*********************************************************************************
 * \           uint8 QND_ReadReg(uint8 RegAddr)
 * \
 * \Description : 读寄存器的值
 * \
 * \Arguments :  RegAddr:要读的寄存器
 * \Returns :   读出的寄存器值
 * \
 * \Notes :
 * \
*********************************************************************************/
uint8 QND_ReadReg(uint8 RegAddr)
{
    uint8 i;
    uint8 Data = 0;
    uint32 flags;
    uint8 tempbuf[2];
    uint8 ret = 0;

    for (i = 0; i < 10; i++)
    {
        tempbuf[0] = RegAddr;

#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        flags = sys_local_intc_save(0x01|(1<<IRQ_TIMER3));//关中断
#else
        sys_os_sched_lock();
#endif
        ret = twi_write_bytes(FM_TWI_MASTER_MODE, twi_master_idx, tempbuf, I2C_WRITE_ADDR, 1, TWI_READ_MODE_YES);
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        sys_local_intc_restore(flags);//开中断
#else
        sys_os_sched_unlock();
#endif

        if (ret != 0)
        {
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
            flags = sys_local_intc_save(0x01|(1<<IRQ_TIMER3));//关中断
#else
            sys_os_sched_lock();
#endif
            ret = twi_read_bytes(FM_TWI_MASTER_MODE, twi_master_idx, tempbuf, I2C_READ_ADDR, 1);
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
            sys_local_intc_restore(flags);//开中断
#else
            sys_os_sched_unlock();
#endif

            if (ret != 0)
            {
                Data = tempbuf[0];
                break;
            }
        }
    }

    if (i == 10)
    {
        libc_printf_warning("QND_ReadReg fail!\n");
    }

    return Data;
}

/**********************************************************************
 * \int QNF_SetRegBit(uint8 reg, uint8 bitMask, uint8 data_val)
 * \Description: set register specified bit
 * \
 * \Parameters:
 * \reg:        register that will be set
 * \bitMask:    mask specified bit of register
 * \data_val:    data will be set for specified bit
 * \Return Value:0 success; -1 fail
 * \None
 **********************************************************************/
int QNF_SetRegBit(uint8 RegAddr, uint8 bitMask, uint8 data_val)
{
    uint8 result;
    //read register
    result = QND_ReadReg(RegAddr);
    //mask bits clear
    result &= (uint8) (~bitMask);
    //mask bits set
    result |= (data_val & bitMask);

    return QND_WriteReg(RegAddr, result);
}

/******************************************************************************
 * \uint16 freq_to_chan(uint32 freq)
 * \par  Description:
 * \将频率值转换为chan寄存器值
 * \param[in]   freq param1 输入的频率值
 * \param[out]  none
 * \return      uint16
 * \retval      寄存器值
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
uint16 freq_to_chan(uint32 freq)
{
    uint16 reg_val = 0;

    //freq = ((chan +60)*0.05)MHZ
    reg_val = (uint16) ((freq - 60000) / 50);

    //reg_val /= 50;

    //转换频率值
    return reg_val;
}

/******************************************************************************
 * \uint32 chan_to_freq(uint16 chan)
 * \par  Description:
 * \将chan寄存器值转换为频率值
 * \param[in]   freq param1 输入的寄存器值
 * \param[out]  none
 * \return      uint32
 * \retval      频率值 单位为khz
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
uint32 chan_to_freq(uint16 chan)
{
    uint32 freq = 0;

    //freq = ((chan +60)*0.05)MHZ
    freq = chan * 50;

    freq += 60000;

    return freq;
}

/**********************************************************************
 * \void QNF_ConfigScan(uint32 start, uint32 stop, uint8 span)
 * \Description: config start, stop, span register for FM/AM CCA or CCS
 * \Parameters:
 * \start
 * \Set the frequency (1kHz) where scan to be started,
 * \eg: 76000 for 76.00MHz.
 * \stop
 * \Set the frequency (1kHz) where scan to be stopped,
 * \eg: 108000 for 108.00MHz
 * \span 50/100/200
 * \Return Value:
 * \None
 **********************************************************************/
void QNF_ConfigScan(uint32 start, uint32 stop, uint8 span)
{
    // calculate ch para
    uint8 tStep = 0;

    uint8 tS;

    uint16 fStart;//start frequency

    uint16 fStop;//stop frequency

    fStart = freq_to_chan(start);

    fStop = freq_to_chan(stop);
    // set to reg: CH_START
    tS = (uint8) (fStart & 0xff);

    QND_WriteReg(CH_START, tS);

    tStep |= ((uint8) (fStart >> 6) & CH_CH_START);
    // set to reg: CH_STOP
    tS = (uint8) (fStop & 0xff);

    QND_WriteReg(CH_STOP, tS);

    tStep |= ((uint8) (fStop >> 4) & CH_CH_STOP);

    if (span == 50)
    {
        span = QND_FSTEP_50KHZ;
    }
    else if (span == 100)
    {
        span = QND_FSTEP_100KHZ;
    }
    else
    {
        span = QND_FSTEP_200KHZ;
    }
    tStep |= span << 6;

    // set to reg: CH_STEP
    QNF_SetRegBit(CH_STEP, 0xfc, tStep);
}

/**********************************************************************
 * \void QNF_RXInit()
 * \Description: set to SNR based MPX control. Call this function before
 * \tune to one specific channel

 * \Parameters:
 * \None
 * \Return Value:
 * \None
 **********************************************************************/
void QNF_RXInit(void)
{
    QNF_SetRegBit(0x1b, 0x08, 0x00); //Let NFILT adjust freely
    QNF_SetRegBit(0x2c, 0x3f, 0x12);
    QNF_SetRegBit(0x1d, 0x40, 0x00);
    QNF_SetRegBit(0x41, 0x0f, 0x0a);
    QND_WriteReg(0x45, 0x50);
    QNF_SetRegBit(0x3e, 0x80, 0x80);
    QNF_SetRegBit(0x41, 0xe0, 0xc0);
    if(qnd_Chip_VID==CHIPSUBID_QN8035A0)
    {
        QNF_SetRegBit(0x42, 0x10, 0x10);
    }
}

/***********************************************************************
 * \void QND_RXSetTH(uint8 db)
 * \Description: Setting the threshold value of automatic scan channel
 * \db:
 * \Setting threshold for quality of channel to be searched,
 * \the db value range:0~(63-CCA_THRESHOLD).
 * \Return Value:
 * \None
 ***********************************************************************/
void QND_RXSetTH(uint8 db)
{
    uint8 rssiTH;
    uint8 snrTH;
    uint16 rssi_snr_TH;

    rssi_snr_TH = rssi_snr_TH_tbl[db];
    rssiTH = (uint8) (rssi_snr_TH >> 8);
    snrTH = (uint8) (rssi_snr_TH & 0xff);
    QND_WriteReg(0x4f, 0x00); //enable auto tunning in CCA mode
    QNF_SetRegBit(REG_REF, ICPREF, 0x0a);
    QNF_SetRegBit(GAIN_SEL, 0x08, 0x08); //NFILT program is enabled
    //selection filter:filter3
    QNF_SetRegBit(CCA1, 0x30, 0x30);
    //Enable the channel condition filter3 adaptation,Let ccfilter3 adjust freely
    QNF_SetRegBit(SYSTEM_CTL2, 0x40, 0x00);
    QND_WriteReg(CCA_CNT1, 0x00);
    QNF_SetRegBit(CCA_CNT2, 0x3f, 0x03);
    //selection the time of CCA FSM wait SNR calculator to settle:20ms
    //0x00:     20ms(default)
    //0x40:     40ms
    //0x80:     60ms
    //0xC0:     100m
    QNF_SetRegBit(CCA_SNR_TH_1, 0xc0, 0x00);
    //selection the time of CCA FSM wait RF front end and AGC to settle:20ms
    //0x00:     10ms
    //0x40:     20ms(default)
    //0x80:     40ms
    //0xC0:     60ms
    QNF_SetRegBit(CCA_SNR_TH_2, 0xc0, 0x40);
    QNF_SetRegBit(CCA, 0x3f, rssiTH); //setting RSSI threshold for CCA
    QNF_SetRegBit(CCA_SNR_TH_1, 0x3f, snrTH); //setting SNR threshold for CCA

}

/*********************************************************************************
 * \         int QND_RXValidCH(uint32 freq, uint8 span)
 * \
 * \Description : 判断设置的频率是否是有效电台
 * \
 * \Arguments :  freq:要写的寄存器
 * \                span:步进100KHZ
 * \Returns :   是否有效电台的标志
 * \
 * \Notes :
 **********************************************************************************/
int QND_RXValidCH(uint32 freq, uint8 span)
{
    uint8 regValue;
    uint8 timeOut = 200;
    //for fake channel judge
    uint8 snr, readCnt, stereoCount;
    int isValidChannelFlag;

    QNF_ConfigScan(freq, freq, span);
    //enter CCA mode,channel index is decided by internal CCA
    QNF_SetRegBit(SYSTEM1, RXCCA_MASK, RX_CCA);
    do
    {
        regValue = QND_ReadReg(SYSTEM1);
        //delay 5ms
        sys_mdelay(5);

        timeOut--;
    }
    while ((regValue & CHSC) && timeOut);  //when seeking a channel or time out,be quited the loop
    //read out the rxcca_fail flag of RXCCA status
    regValue = QND_ReadReg(STATUS1) & 0x08;
    if (regValue != 0)
    {
        isValidChannelFlag = -1;
    }
    else
    {
        isValidChannelFlag = 0;
    }
    //special search  handle ways for fake channel
    if (isValidChannelFlag == 0)
    {
        snr = QND_ReadReg(SNR);
        //delay 60ms
        sys_mdelay(60);
        //信噪比
        if (snr <= 25)
        {
            stereoCount = 0;
            for (readCnt = 0; readCnt < 10; readCnt++)
            {
                sys_mdelay(2);
                if ((QND_ReadReg(STATUS1) & 0x01) == 0)
                {
                    stereoCount++;
                }
                if (stereoCount >= 3)
                {
                    isValidChannelFlag = 1;
                    break;
                }
            }
        }
    }

    return isValidChannelFlag;
}

/**********************************************************************
 * \uint32 QNF_GetCh()
 * \Description: get current channel frequency
 * \
 * \Parameters:
 * \None
 * \Return Value:
 * \channel frequency
 **********************************************************************/
uint32 QNF_GetCh(void)
{
    uint8 tCh;
    uint8 tStep;
    uint16 ch = 0;
    uint32 freq;
    // set to reg: CH_STEP
    tStep = QND_ReadReg(CH_STEP);
    tStep &= CH_CH;
    //high 2 bits of total 10 bits
    ch = tStep;
    //low 8 bits of total 10 bits
    tCh = QND_ReadReg(CH);
    //chan = CH_STEP low 2 bits & CH 8 bits
    ch = (ch << 8) + tCh;
    freq = chan_to_freq(ch);

    return freq;
}

/**********************************************************************
 * \uint8 QNF_SetCh(uint32 freq)
 * \Description: set channel frequency
 * \Parameters:
 * \freq:  channel frequency to be set
 * \Return Value:
 * \1: success
 **********************************************************************/
uint8 QNF_SetCh(uint32 freq)
{
    // calculate ch parameter used for register setting
    uint8 tStep;
    uint8 tCh;
    uint16 f;

    if (freq == 85500)
    {
        QND_WriteReg(NCO_COMP_VAL, 0x69);
        freq = 85700;
        g_freq_855 = 1;
    }
    else
    {
        QND_WriteReg(XTAL_DIV1, qnd_div1);
        QND_WriteReg(XTAL_DIV2, qnd_div2);
        QND_WriteReg(NCO_COMP_VAL, qnd_nco);
        g_freq_855 = 0;
    }
    //Manually set RX Channel index
    QNF_SetRegBit(SYSTEM1, CCA_CH_DIS, CCA_CH_DIS);

    f = freq_to_chan(freq);
    // set to reg: CH
    tCh = (UINT8) f;
    QND_WriteReg(CH, tCh);
    // set to reg: CH_STEP
    tStep = QND_ReadReg(CH_STEP);
    tStep &= ~CH_CH;
    tStep |= ((UINT8) (f >> 8) & CH_CH);
    QND_WriteReg(CH_STEP, tStep);

    return 1;
}

/**********************************************************************
 * \void QND_TuneToCH(uint32 ch)
 * \Description: Tune to the specific channel. call QND_SetSysMode() before
 * \call this function
 * \Parameters:
 * \ch
 * \Set the frequency (1kHz) to be tuned,
 * \eg: 101.30MHz will be set to 101300.
 * \Return Value:
 * \None
 **********************************************************************/
void QND_TuneToCH(uint32 ch)
{
    uint8 reg;

    QNF_RXInit();
    //for imr image rejection
    if ((ch == 84300) || (ch == 72900) || (ch == 69100))
    {
        QNF_SetRegBit(CCA, IMR, IMR);
    }
    else
    {
        QNF_SetRegBit(CCA, IMR, 0x00);
    }
    //set search param
    QNF_ConfigScan(ch, ch, g_set_value.span);

    QNF_SetCh(ch);

    if ((qnd_Chip_VID == CHIPSUBID_QN8035A0) || (qnd_Chip_VID == CHIPSUBID_QN8035A1))
    {
        QNF_SetRegBit(SYSTEM1, CHSC, CHSC);
    }

    //Auto tuning
    QND_WriteReg(0x4F, 0x80);
    reg = QND_ReadReg(0x4F);
    reg >>= 1;
    QND_WriteReg(0x4F, reg);

    //avoid the noise which are "POP" and "sha sha" noise.
    sys_mdelay(200);
}

/******************************************************************************
 * \int sFM_SetFreq(uint32 freq, void* null2, void* null3)
 * \par  Description:
 * \设置频点,进行tune操作
 * \param[in]   freq param1 输入的频率值
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \note check freq is valid tune
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int sFM_SetFreq(uint32 freq)
{
    int flag = -1;

    //是否为无效频点
    flag = _is_freq_valid(freq);
    if (flag == 0)
    {
        uint32 check_freq;

        //设置当前频点
        QND_TuneToCH(freq);

        //设置频点后读取频点检查是否OK
        check_freq = QNF_GetCh();
        if (freq != check_freq)
        {
            flag = -1;
        }
    }

    return flag;
}


/******************************************************************************
 * \int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
 * \par  Description:
 * \ FM  设置静音
 * \param[in]   mode param1 输入的频率值
 typedef enum
 {
 releaseMUTE = 0,        //解除静音
 SetMUTE,                   //静音
 } FM_MUTE_e;
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int sFM_Mute(FM_MUTE_e mode)
{
    int flag = -1;
    //mute
    if (mode != 0)
    {
        if(qnd_Chip_VID==CHIPSUBID_QN8035A0)
        {
            flag=QNF_SetRegBit(REG_DAC, 0x0b, 0x0b);
        }
        else
        {
            flag=QNF_SetRegBit(0x4a, 0x20, 0x20);
        }
    }
    else//release mute
    {
        if(qnd_Chip_VID==CHIPSUBID_QN8035A0)
        {
            flag=QNF_SetRegBit(REG_DAC, 0x0b, 0x00);
        }
        else
        {
            flag=QNF_SetRegBit(0x4a, 0x20, 0x00);
        }
    }
    return flag;
}

/******************************************************************************
 * \int sFM_SetThrod(uint8 level, void* null2, void* null3)
 * \par  Description:
 * \ 设置搜台门限
 * \param[in]   level param1 输入的门限值
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/

int sFM_SetThrod(uint32 level)
{

    //设置门限
    QND_RXSetTH(level);

    return 0;
}

/********************************************************************************

 * \int sFM_Search(uint32 freq)
 * \par  Description:
 * \FM  软件搜台，tune 单频点，判断是否有效电台
 * \param[in]   freq param1 输入的频率值
 * \param[in]   direct param2 搜台方向
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \note     check freq is valid tune
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int sFM_Search(uint32 freq)
{
    int result;

    //whether valid channel
    result = QND_RXValidCH(freq, g_set_value.span);

    return result;
}

/******************************************************************************
 * \int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
 * \par  Description:
 * \获取当前电台的相关信息，包括当前电台频率、信号强度值，立体声状态，当前波段
 * \param[in]   pstruct_buf param1 保存电台信息的结构体指针
 * \param[in]   mode param2 当前模式
 *                   mode==0,  正常播放( 非seek 过程中)  取信息
 *                   mode==1,  在硬件seek 过程中取信息
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \note check freq is valid tune
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int sFM_GetStatus(FM_Drv_Status_t *p_Drv_Status)
{
    if (g_freq_855 == 1)
    {
        p_Drv_Status->FM_CurrentFreq = 85500;
    }
    else
    {
        p_Drv_Status->FM_CurrentFreq = QNF_GetCh();
    }

    return 0;
}

/**********************************************************************
 * \ uint8 QN_ChipInitialization(void)
 * \Description: chip first step initialization, called only by QND_Init()
 * \Parameters:
 * \None
 * \Return Value: Chip OK
 ***********************************************************************/
int QN_ChipInitialization(void)
{
    uint8 chipid,IsQN8035B;
    uint8 qnd_xtal_div0;
    uint8 qnd_xtal_div1;
    uint8 qnd_xtal_div2;

    //复位
    QND_WriteReg(SYSTEM1, 0x81);

    sys_mdelay(50); //延时10MS以上

    //根据这里打印出来的值，取设置宏定义qnd_ChipID, qnd_IsQN8035B
    chipid = QND_ReadReg(CID2);
    qnd_Chip_VID = (QND_ReadReg(CID1) & 0x03);
    IsQN8035B = QND_ReadReg(0x58) & 0x1f;
    libc_printf_info("FM ChipID = 0x%x, ChipID version = 0x%x, qnd_IsQN8035B = 0x%x\n", chipid, qnd_Chip_VID, IsQN8035B);

    if((chipid != CHIPID_QN8035)||(IsQN8035B != 0x13))
    {
        libc_printf_error("chip id err\n");
        return -1;
    }

    QND_WriteReg(0x58, 0x13);

    if (fm_clk_source_sel == FM_CLK_SRC_INNER)
    {//主控IC时钟源，方波
        //Following is where change the input clock type.as crystal or oscillator.
        QNF_SetRegBit(0x58, 0x80, QND_INPUT_CLOCK);
        //Following is where change the input clock wave type,as sine-wave or square-wave.
        QNF_SetRegBit(0x01, 0x80, QND_DIGITAL_CLOCK);//方波
    }
    else
    {//外挂晶振时钟源，正弦波
        //Following is where change the input clock type.as crystal or oscillator.
        QNF_SetRegBit(0x58, 0x80, QND_CRYSTAL_CLOCK);
        //Following is where change the input clock wave type,as sine-wave or square-wave.
        QNF_SetRegBit(0x01, 0x80, QND_SINE_WAVE_CLOCK);//正弦波
    }

    //32.768k
    if (fm_clk_hz == 32768)
    {
        qnd_xtal_div0 = QND_XTAL_DIV0_32K;
        qnd_xtal_div1 = QND_XTAL_DIV1_32K;
        qnd_xtal_div2 = QND_XTAL_DIV2_32K;
    }
    //13Mhz
    else if (fm_clk_hz == 13000000)
    {
        qnd_xtal_div0 = QND_XTAL_DIV0_13M;
        qnd_xtal_div1 = QND_XTAL_DIV1_13M;
        qnd_xtal_div2 = QND_XTAL_DIV2_13M;
    }
    //24Mhz
    else if (fm_clk_hz == 24000000)
    {
        qnd_xtal_div0 = QND_XTAL_DIV0_24M;
        qnd_xtal_div1 = QND_XTAL_DIV1_24M;
        qnd_xtal_div2 = QND_XTAL_DIV2_24M;
    }
    //26Mhz
    else if (fm_clk_hz == 26000000)
    {
        qnd_xtal_div0 = QND_XTAL_DIV0_26M;
        qnd_xtal_div1 = QND_XTAL_DIV1_26M;
        qnd_xtal_div2 = QND_XTAL_DIV2_26M;
    }
    else
    {
        libc_printf_error("NO support FM CLK:%d\n", fm_clk_hz);
        while (1);
    }

    //Following is where change the input clock frequency.
    QND_WriteReg(XTAL_DIV0, qnd_xtal_div0);
    QND_WriteReg(XTAL_DIV1, qnd_xtal_div1);
    QND_WriteReg(XTAL_DIV2, qnd_xtal_div2);
    sys_mdelay(10);
    /********User sets chip working clock end ********/
    QND_WriteReg(0x54, 0x47); //mod PLL setting
    QND_WriteReg(SMP_HLD_THRD, 0xc4); //select SNR as filter3,SM step is 2db
    QNF_SetRegBit(0x40, 0x70, 0x70);
    QND_WriteReg(0x33, 0x9c); //set HCC Hystersis to 5db
    QND_WriteReg(0x2d, 0xd6); //notch filter threshold adjusting
    QND_WriteReg(0x43, 0x10); //notch filter threshold enable
    QNF_SetRegBit(SMSTART, 0x7f, SMSTART_VAL);
    QNF_SetRegBit(SNCSTART, 0x7f, SNCSTART_VAL);
    QNF_SetRegBit(HCCSTART, 0x7f, HCCSTART_VAL);
    if(qnd_Chip_VID==CHIPSUBID_QN8035A1)
    {
        QNF_SetRegBit(0x47, 0x0c, 0x08);
    }
    qnd_div1 = QND_ReadReg(XTAL_DIV1);
    qnd_div2 = QND_ReadReg(XTAL_DIV2);
    qnd_nco = QND_ReadReg(0x46);
    return 0;

}

/**********************************************************************
 * \int QND_Init(void)
 * \Description: Initialize device to make it ready to have all functionality ready for use.
 * \Parameters:
 * \None
 * \Return Value:
 * \0: Device is ready to use.
 * \-1: Device is not ready to serve function.
 ************************************************************************/
int QND_Init(void)
{
    int ChipStatus;
    ChipStatus = QN_ChipInitialization();
    if (ChipStatus == 0)
    {
        //enter RX mode
        QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, RX_MODE);
        //set mute
        sFM_Mute(SetMUTE);
        //音量设置到0dB(最大)
        QNF_SetRegBit(VOL_CTL, 0x3f, 0x07);
        //设置去加重时间常数
        if (g_set_value.de_emphasis_tc == DE_EMPHASIS_TC_50US)
        {
            QNF_SetRegBit(VOL_CTL, 0x40, 0x00);
        }
        else
        {
            QNF_SetRegBit(VOL_CTL, 0x40, 0x40);
        }
    }
    return ChipStatus;
}

/******************************************************************************
 * \int sFM_Init(FM_Drv_Init_Para_t *p_init_para)
 * \par  Description:
 * \ FM初始化,设置搜索门限,波段,步进等
 * \param[in]   band param1 输入的波段值
 * \param[in]   level param2 搜台门限
 * \param[in]   freq param3 频率
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 ********************************************************************************/
int sFM_Init(FM_Drv_Init_Para_t *p_init_para)
{
    int result;

    //保存搜台门限
    g_set_value.threshold = p_init_para->threshold;
    //去加重时间常数
    g_set_value.de_emphasis_tc = p_init_para->de_emphasis_tc;
    //步长
    g_set_value.span = p_init_para->span;
    //最低频率
    g_set_value.freq_low = p_init_para->freq_low;
    //最高频率
    g_set_value.freq_high = p_init_para->freq_high;

    result = QND_Init(); //初始化
    //设置搜台门限值
    QND_RXSetTH(g_set_value.threshold);

    //初始化OK后等待500ms，否则外部时钟会搜不了台
    if (fm_clk_source_sel == FM_CLK_SRC_EXTERN)
    {
        sys_mdelay(500);
    }

    return result;
}

/******************************************************************************
 * \int sFM_Standby(void* null1, void* null2, void* null3)
 * \par  Description:
 * \ 进入fm休眠
 * \param[in]   none
 * \param[out]  none
 * \return      int
 * \retval   成功 返回0;
 * \retval   失败 返回-1
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int sFM_Standby(uint8 mode)
{
    //FM MUTE
    sFM_Mute(SetMUTE);
    //make sure that Power down control by FSM control
    QNF_SetRegBit(REG_DAC, 0x08, 0x00);
    //SYSTEM1 bit5 enter standby
    if (mode == FM_STANDBY_MODE)
    {
        QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, STNBY_MODE);
    }
    else
    {
        QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, IDLE_MODE); //idle状态AOUT阻抗高很多，但是功耗会达到mA级别；相比Standby功耗则为50uA级别
    }
    return 0;
}

#endif
