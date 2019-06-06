
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

uint8 g_ain_input_mode[AIN_SOURCE_MAX];
bool  g_adc_lch_need_enable;
bool  g_adc_rch_need_enable;

extern uint8 __map_sr2reg_48(uint8 sr);
extern uint8 __map_sr2reg_44(uint8 sr);

void init_adc_digital(void)
{
    //adc fifo set
    *((REG32)(ADC_FIFOCTL)) = ((*((REG32)(ADC_FIFOCTL))) | (1<<ADC_FIFOCTL_ADFOS)) ; //ADC FIFO OUTPUT TO DMA
    *((REG32)(ADC_FIFOCTL)) = (*((REG32)(ADC_FIFOCTL))) | (1<<ADC_FIFOCTL_ADFFDE);  //ADC FIFO DRQ
    *((REG32)(ADC_FIFOCTL)) = (*((REG32)(ADC_FIFOCTL))) & (~(1<<ADC_FIFOCTL_ADFRT));//ADC FIFO RESET
    *((REG32)(ADC_FIFOCTL)) = (*((REG32)(ADC_FIFOCTL))) | (1<<ADC_FIFOCTL_ADFRT);   //ADC FIFO NORMAL
    //adc digital set
    *((REG32)(ADC_DIGCTL)) = (*((REG32)(ADC_DIGCTL))) | (0x3<<ADC_DIGCTL_HPFREN);       //ADC HPF L&R EN
    *((REG32)(ADC_DIGCTL)) = ((*((REG32)(ADC_DIGCTL))) & ~(ADC_DIGCTL_ADCGC_MASK)) | (0x0 << ADC_DIGCTL_ADCGC_SHIFT); //ADC GAIN SET 0dB
}

void reset_adc_digital_fifo(void)
{
    *((REG32)(ADC_FIFOCTL)) = (*((REG32)(ADC_FIFOCTL))) & (~(1 << ADC_FIFOCTL_ADFRT)); //adc fifo reset
    *((REG32)(ADC_FIFOCTL)) = (*((REG32)(ADC_FIFOCTL))) | (1 << ADC_FIFOCTL_ADFRT); //adc fifo normal
}

void init_adc_analog(void)
{
    *((REG32)(ADC_ANACTL0)) |= (1 << ADC_ANACTL0_ADVREN); //VRDA enable
    if (g_adc_lch_need_enable == TRUE)
    {
        *((REG32)(ADC_ANACTL0)) |= (1 << ADC_ANACTL0_ADLPFLEN); //adc LPF lch enable
        *((REG32)(ADC_ANACTL0)) |= (1<<ADC_ANACTL0_ADLEN);//ADC Analog lch enable
    }
    if (g_adc_rch_need_enable == TRUE)
    {
        *((REG32)(ADC_ANACTL0)) |= (1 << ADC_ANACTL0_ADLPFREN); //adc LPF rch enable
        *((REG32)(ADC_ANACTL0)) |= (1<<ADC_ANACTL0_ADREN);//ADC Analog rch enable
    }

    //ADC_BIAS set
    *((REG32)(ADC_BIAS)) = 0x60050542;
}

void hal_audio_enable_ain(ain_source_type_e ain_src, ain_setting_t *ain_sett)
{
    uint32 temp;

    g_ain_input_mode[ain_src] = ain_sett->ain_input_mode;

    if (ain_sett->ain_pa_mode == FALSE)
    {
        if (ain_src == AIN_SOURCE_MIC1)
        {
            if (ain_sett->ain_input_mode == AIN_INPUT_MIC0_L)
            {
                temp = *((REG32)(ADC_ANACTL1));

                temp |=  (1<< ADC_ANACTL1_MOP0EN);  //enable mic0 op
                temp |= (0x1<<ADC_ANACTL1_MIC0EN); //mic0 channel enable

                temp |=  (1<< ADC_ANACTL1_MOP02LPFL);//mic0 mix to LPF Lchannel

                temp &= (~ADC_ANACTL1_MIC0G_MASK);
                temp |= (ain_sett->op_gain.mic_op_gain << ADC_ANACTL1_MIC0G_SHIFT); //mic gain set

                *((REG32)(ADC_ANACTL1)) = temp;

                g_adc_lch_need_enable = TRUE;
            }
            else
            {
                temp = *((REG32)(ADC_ANACTL1));

                temp |=  (1<< ADC_ANACTL1_MOP1EN);  //enable mic1 op
                temp |= (0x1<<ADC_ANACTL1_MIC1EN); //mic1 channel enable

                temp |=  (1<< ADC_ANACTL1_MOP12LPFR);//mic1 mix to LPF Rchannel

                temp &= (~ADC_ANACTL1_MIC1G_MASK);
                temp |= (ain_sett->op_gain.mic_op_gain << ADC_ANACTL1_MIC1G_SHIFT); //mic gain set

                *((REG32)(ADC_ANACTL1)) = temp;

                g_adc_rch_need_enable = TRUE;
            }
        }
        else
        {
            temp = *((REG32)(ADC_ANACTL0));

            if (ain_sett->ain_input_mode == AIN_INPUT_L0_R0_DUAL)
            {
                temp |= (1<< ADC_ANACTL0_OP0LEN);  //auxin op0 L enable
                temp |= (1<< ADC_ANACTL0_OP0REN);  //auxin op0 R enable
                temp |= (0x1<<ADC_ANACTL0_AUX0LEN) | (0x1<<ADC_ANACTL0_AUX0REN); //aux0 l&r channel enable
                temp |=  (1<< ADC_ANACTL0_OP0L2LPFL);//op0 L mix to LPF Lchannel;
                temp |=  (1<< ADC_ANACTL0_OP0R2LPFR);//op0 R mix to LPF Rchannel;

                temp &= (~ADC_ANACTL0_OP0G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP0G_SHIFT);//aux gain set

                g_adc_lch_need_enable = TRUE;
                g_adc_rch_need_enable = TRUE;
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_L1_R1_DUAL)
            {
                temp |= (1<< ADC_ANACTL0_OP1LEN);  //auxin op0 L enable
                temp |= (1<< ADC_ANACTL0_OP1REN);  //auxin op0 R enable
                temp |= (0x1<<ADC_ANACTL0_AUX1LEN) | (0x1<<ADC_ANACTL0_AUX1REN); //aux1 l&r channel enable
                temp |=  (1<< ADC_ANACTL0_OP1L2LPFL);//op0 L mix to LPF Lchannel;
                temp |=  (1<< ADC_ANACTL0_OP1R2LPFR);//op0 R mix to LPF Rchannel;

                temp &= (~ADC_ANACTL0_OP1G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP1G_SHIFT);//aux gain set

                g_adc_lch_need_enable = TRUE;
                g_adc_rch_need_enable = TRUE;
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_L0_L1_MIX)
            {
                temp |= (1<< ADC_ANACTL0_OP0LEN);  //auxin op0 L enable
                temp |= (1<< ADC_ANACTL0_OP1LEN);  //auxin op1 L enable
                temp |= (0x1<<ADC_ANACTL0_AUX0LEN) | (0x1<<ADC_ANACTL0_AUX1LEN); //aux0 l & aux1 l channel enable
                temp |=  (1<< ADC_ANACTL0_OP0L2LPFL);//op0 L mix to LPF Lchannel;
                temp |=  (1<< ADC_ANACTL0_OP1L2LPFL);//op1 L mix to LPF Lchannel;

                temp &= (~ADC_ANACTL0_OP0G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP0G_SHIFT);//aux gain set
                temp &= (~ADC_ANACTL0_OP1G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP1G_SHIFT);//aux gain set

                g_adc_lch_need_enable = TRUE;
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_R0_R1_MIX)
            {
                temp |= (1<< ADC_ANACTL0_OP0REN);  //auxin op0 R enable
                temp |= (1<< ADC_ANACTL0_OP1REN);  //auxin op1 R enable
                temp |= (0x1<<ADC_ANACTL0_AUX0REN) | (0x1<<ADC_ANACTL0_AUX1REN); //aux0 r & aux1 r channel enable
                temp |=  (1<< ADC_ANACTL0_OP0R2LPFR);//op0 R mix to LPF Rchannel;
                temp |=  (1<< ADC_ANACTL0_OP1R2LPFR);//op1 R mix to LPF Rchannel;

                temp &= (~ADC_ANACTL0_OP0G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP0G_SHIFT);//aux gain set
                temp &= (~ADC_ANACTL0_OP1G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP1G_SHIFT);//aux gain set

                g_adc_rch_need_enable = TRUE;
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_L0_R0_MIX)
            {
                //无效
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_ONLY_L0)
            {
                temp |= (1<< ADC_ANACTL0_OP0LEN);  //auxin op0 L enable
                temp |= (0x1<<ADC_ANACTL0_AUX0LEN); //auxl channel enable, auxr channel disable
                temp &= ~(0x1<<ADC_ANACTL0_AUX0REN);
                temp |=  (1<< ADC_ANACTL0_OP0L2LPFL);//op0 L mix to LPF Lchannel;

                temp &= (~ADC_ANACTL0_OP0G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP0G_SHIFT);//aux gain set

                g_adc_lch_need_enable = TRUE;
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_ONLY_R0)
            {
                temp |= (1<< ADC_ANACTL0_OP0REN);  //auxin op0 R enable
                temp &= ~(0x1<<ADC_ANACTL0_AUX0LEN); //auxl channel disable, auxr channel enable
                temp |= (0x1<<ADC_ANACTL0_AUX0REN);
                temp |=  (1<< ADC_ANACTL0_OP0R2LPFR);//op0 R mix to LPF Rchannel;

                temp &= (~ADC_ANACTL0_OP0G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP0G_SHIFT);//aux gain set

                g_adc_rch_need_enable = TRUE;
            }
            else if (ain_sett->ain_input_mode == AIN_INPUT_ONLY_L1)
            {
                temp |= (1<< ADC_ANACTL0_OP1LEN);  //auxin op1 L enable
                temp |= (0x1<<ADC_ANACTL0_AUX1LEN); //auxl channel enable, auxr channel disable
                temp &= ~(0x1<<ADC_ANACTL0_AUX1REN);
                temp |=  (1<< ADC_ANACTL0_OP1L2LPFL);//op0 L mix to LPF Lchannel;

                temp &= (~ADC_ANACTL0_OP1G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP1G_SHIFT);//aux gain set

                g_adc_lch_need_enable = TRUE;
            }
            else
            {
                temp |= (1<< ADC_ANACTL0_OP1REN);  //auxin op1 R enable
                temp &= ~(0x1<<ADC_ANACTL0_AUX1LEN); //auxl channel disable, auxr channel enable
                temp |= (0x1<<ADC_ANACTL0_AUX1REN);
                temp |=  (1<< ADC_ANACTL0_OP1R2LPFR);//op0 R mix to LPF Rchannel;

                temp &= (~ADC_ANACTL0_OP1G_MASK);
                temp |= (ain_sett->op_gain.ain_op_gain << ADC_ANACTL0_OP1G_SHIFT);//aux gain set

                g_adc_rch_need_enable = TRUE;
            }

            *((REG32)(ADC_ANACTL0)) = temp;
        }
    }
    else //ain -> pa 直通通道
    {
        //TODO 暂不支持
    }
}

void hal_audio_disable_ain(ain_source_type_e ain_src)
{
    uint32 temp;

    if (ain_src == AIN_SOURCE_MIC1)
    {
        if (g_ain_input_mode[ain_src] == AIN_INPUT_MIC0_L)
        {
            temp = *((REG32)(ADC_ANACTL1));
            temp &= ~(1<< ADC_ANACTL1_MOP02LPFL);//mic0 mix to LPF Lchannel
            temp &= ~(0x1<<ADC_ANACTL1_MOP0EN); //mic0 OP disable
            temp &= ~(0x1<<ADC_ANACTL1_MIC0EN); //mic channel disable
            *((REG32)(ADC_ANACTL1)) = temp;
        }
        else
        {
            temp = *((REG32)(ADC_ANACTL1));
            temp &=  ~(1<< ADC_ANACTL1_MOP1EN);  //disable mic1 op
            temp &= ~(0x1<<ADC_ANACTL1_MIC1EN); //mic1 channel disable
            temp &= ~(1<< ADC_ANACTL1_MOP12LPFR);//mic1 mix to LPF Rchannel
            *((REG32)(ADC_ANACTL1)) = temp;
        }
    }
    else if (ain_src == AIN_SOURCE_MIC2)
    {

    }
    else
    {
        temp = *((REG32)(ADC_ANACTL0));

        if (g_ain_input_mode[ain_src] == AIN_INPUT_L0_R0_DUAL)
        {
            temp &= ~(1<< ADC_ANACTL0_OP0LEN);//op0 L disable
            temp &= ~(1<< ADC_ANACTL0_OP0REN);//op0 R disable
            temp &= ~(1<< ADC_ANACTL0_OP0L2LPFL);//op0 L mix to LPF Lchannel disable
            temp &= ~(1<< ADC_ANACTL0_OP0R2LPFR);//op0 R mix to LPF Rchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX0LEN) | (0x1<<ADC_ANACTL0_AUX0REN)); //aux0 l&r channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_L1_R1_DUAL)
        {
            temp &= ~(1<< ADC_ANACTL0_OP1LEN);//op1 L disable
            temp &= ~(1<< ADC_ANACTL0_OP1REN);//op1 R disable
            temp &= ~(1<< ADC_ANACTL0_OP1L2LPFL);//op1 L mix to LPF Lchannel disable
            temp &= ~(1<< ADC_ANACTL0_OP1R2LPFR);//op1 R mix to LPF Rchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX1LEN) | (0x1<<ADC_ANACTL0_AUX1REN)); //aux1 l&r channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_L0_L1_MIX)
        {
            temp &= ~(1<< ADC_ANACTL0_OP0LEN);//op0 L disable
            temp &= ~(1<< ADC_ANACTL0_OP1LEN);//op1 L disable
            temp &= ~(1<< ADC_ANACTL0_OP0L2LPFL);//op0 L mix to LPF Lchannel disable
            temp &= ~(1<< ADC_ANACTL0_OP1L2LPFL);//op1 L mix to LPF Lchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX0LEN) | (0x1<<ADC_ANACTL0_AUX1LEN)); //aux0 l & aux1 l channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_R0_R1_MIX)
        {
            temp &= ~(1<< ADC_ANACTL0_OP0REN);//op0 R disable
            temp &= ~(1<< ADC_ANACTL0_OP1REN);//op1 R disable
            temp &= ~(1<< ADC_ANACTL0_OP0R2LPFR);//op0 R mix to LPF Rchannel disable
            temp &= ~(1<< ADC_ANACTL0_OP1R2LPFR);//op1 R mix to LPF Rchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX0REN) | (0x1<<ADC_ANACTL0_AUX1REN)); //aux0 r & aux1 r channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_ONLY_L0)
        {
            temp &= ~(1<< ADC_ANACTL0_OP0LEN);//op0 L disable
            temp &= ~(1<< ADC_ANACTL0_OP0L2LPFL);//op0 L mix to LPF Lchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX0LEN)); //aux0 l channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_ONLY_R0)
        {
            temp &= ~(1<< ADC_ANACTL0_OP0REN);//op0 R disable
            temp &= ~(1<< ADC_ANACTL0_OP0R2LPFR);//op0 R mix to LPF Rchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX0REN)); //aux0 r channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_ONLY_L1)
        {
            temp &= ~(1<< ADC_ANACTL0_OP1LEN);//op1 L disable
            temp &= ~(1<< ADC_ANACTL0_OP1L2LPFL);//op1 L mix to LPF Lchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX1LEN)); //aux0 l channel disable
        }
        else if (g_ain_input_mode[ain_src] == AIN_INPUT_ONLY_R1)
        {
            temp &= ~(1<< ADC_ANACTL0_OP1REN);//op1 R disable
            temp &= ~(1<< ADC_ANACTL0_OP1R2LPFR);//op1 R mix to LPF Rchannel disable
            temp &= ~((0x1<<ADC_ANACTL0_AUX1REN)); //aux1 r channel disable
        }

        *((REG32)(ADC_ANACTL0)) = temp;
    }
}

void hal_audio_set_adc_gain(uint8 adc_gain)
{
    *((REG32)(ADC_DIGCTL)) &= ~(ADC_DIGCTL_ADCGC_MASK);
    *((REG32)(ADC_DIGCTL)) |= (adc_gain << ADC_DIGCTL_ADCGC_SHIFT);
}

/*adc_rate 8 - 48*/
void hal_audio_set_adc_rate(uint8 adc_rate)
{
    uint32 adc_div_val = 0xff;
    uint32 aps_val;

    adc_div_val = __map_sr2reg_48(adc_rate);
    if (adc_div_val == 0xff)
    {
        adc_div_val = __map_sr2reg_44(adc_rate);
        if (adc_div_val == 0xff)
        {
            g_p_sys_hal_cbks->hal_printf("invalid sample rate!");
            return;
        }

        aps_val = 0x04; //fast 0%
    }
    else
    {
        aps_val = 0x0C; //fast 0%
    }

    //set audio pll clk
    *((REG32)(AUDIO_PLL_CTL)) &= (~AUDIO_PLL_CTL_APS_MASK);
    *((REG32)(AUDIO_PLL_CTL)) |= (aps_val << AUDIO_PLL_CTL_APS_SHIFT);

    //ADC clock divisor
    *((REG32)(CMU_ADDACLK)) &= (~CMU_ADDACLK_ADCCLKDIV_MASK);
    *((REG32)(CMU_ADDACLK)) |= (adc_div_val << CMU_ADDACLK_ADCCLKDIV_SHIFT);
}

void hal_audio_enable_adc(ain_source_type_e ain_src, adc_setting_t *adc_sett)
{
    uint32 cpu_sr;

    cpu_sr = g_p_sys_hal_cbks->hal_int_close();
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_ADCCLKEN);
    g_p_sys_hal_cbks->hal_int_open(cpu_sr);

    init_adc_digital();
    init_adc_analog();

    hal_audio_set_adc_gain(adc_sett->gain);

    hal_audio_set_adc_rate(adc_sett->sample_rate);

    //ADC刚开始采样的一些sample点是不确定不可信的，需要滤除
    if (adc_sett->sample_rate >= 8)
    {
        g_p_sys_hal_cbks->hal_udelay((128/adc_sett->sample_rate + 1)*1000);
    }
    //reset_adc_digital_fifo(); //TODO 会导致左右声道相反，应该有杂音问题，后续再处理，可以考虑放到应用层丢弃
}

void hal_audio_disable_adc(ain_source_type_e ain_src)
{
    uint32 cpu_sr;

    *((REG32)(ADC_ANACTL0)) = (*((REG32)(ADC_ANACTL0))) & (~((1 << ADC_ANACTL0_ADLPFLEN)|(1 << ADC_ANACTL0_ADLPFREN))); //adc LPF lch enable
    *((REG32)(ADC_ANACTL0)) = (*((REG32)(ADC_ANACTL0))) & (~((0x01 << ADC_ANACTL0_ADLEN)|(0x01 << ADC_ANACTL0_ADREN)));
    g_adc_lch_need_enable = FALSE;
    g_adc_rch_need_enable = FALSE;

    *((REG32)(ADC_FIFOCTL)) = (*((REG32)(ADC_FIFOCTL))) & (~(1<<ADC_FIFOCTL_ADFRT));//ADC FIFO RESET

    cpu_sr = g_p_sys_hal_cbks->hal_int_close();
    *((REG32)(CMU_ADDACLK)) |= (0x01<<CMU_ADDACLK_ADCCLKDIV_SHIFT); //adc clock set(sr set) 48k
    *((REG32)(CMU_DEVCLKEN)) &= ~(1 << CMU_DEVCLKEN_ADCCLKEN); //adc clock gating enable
    g_p_sys_hal_cbks->hal_int_open(cpu_sr);
}

bool hal_audio_is_sim_adc_dual(void)
{
    return FALSE;
}
