
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

pcmbuf_irq_cbk audio_pcmbuf_irq_cbk;

void pcmbuf_isr(void)
{
    uint8 pending = 0;

    if ((act_readl(PCM_BUF_STAT) & (0x01 << PCM_BUF_STAT_PCMBHEIP)) != 0)
    {
        *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBHEIP);
        pending |= 0x01;
    }

    if ((act_readl(PCM_BUF_STAT) & (0x01 << PCM_BUF_STAT_PCMBEIP)) != 0)
    {
        *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBEIP);
        pending |= 0x02;
    }

    if ((act_readl(PCM_BUF_STAT) & (0x01 << PCM_BUF_STAT_PCMBHFIP)) != 0)
    {
        *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBHFIP);
        pending |= 0x04;
    }

    if ((act_readl(PCM_BUF_STAT) & (0x01 << PCM_BUF_STAT_PCMBFIP)) != 0)
    {
        *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBFIP);
    }

    if ((audio_pcmbuf_irq_cbk != NULL) && (pending != 0))
    {
        audio_pcmbuf_irq_cbk(pending);
    }
}

void hal_audio_reset_pcmbuf(void)
{
    audio_pcmbuf_irq_cbk = NULL;
    g_p_sys_hal_cbks->hal_free_irq (IRQ_DAC_IIS_TX);
}

void hal_audio_set_pcmbuf(pcmbuf_setting_t *pcmbuf_sett)
{
    //clear irq pending
    *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBHEIP);
    *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBEIP);
    *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBHFIP);
    *((REG32)(PCM_BUF_STAT)) = (1 << PCM_BUF_STAT_PCMBFIP);

    *((REG32)(PCM_BUF_CTL)) = 0;

    //enable PCM BUF Interrupt
    *((REG32)(PCM_BUF_CTL)) |= (pcmbuf_sett->ep_ie << PCM_BUF_CTL_PCMBEPIE)
            | (pcmbuf_sett->he_ie << PCM_BUF_CTL_PCMBHEIE);
    *((REG32)(PCM_BUF_CTL)) |= (pcmbuf_sett->hf_ie << PCM_BUF_CTL_PCMBHFIE)
            | (pcmbuf_sett->fu_ie << PCM_BUF_CTL_PCMBFUIE);

    //set half empth threshold
    *((REG32)(PCM_BUF_THRES_HE)) = pcmbuf_sett->half_empty_thres;

    //NOTE！！ 必须先达到半满后才能够触发半空，所以我们将半满阈值设置为半空阈值略加几个采样点
    *((REG32)(PCM_BUF_THRES_HF)) = pcmbuf_sett->half_full_thres;

    audio_pcmbuf_irq_cbk = pcmbuf_sett->irq_cbk;
    if ((pcmbuf_sett->he_ie == TRUE) || (pcmbuf_sett->ep_ie == TRUE) || (pcmbuf_sett->hf_ie == TRUE)
            || (pcmbuf_sett->fu_ie == TRUE))
    {
        g_p_sys_hal_cbks->hal_request_irq(IRQ_DAC_IIS_TX, pcmbuf_isr);
    }
}

//PCMBUF 获取 length 需要去抖
uint32 hal_audio_get_pcmbuf_length(void)
{
    uint32 empty_length = 0x5fff, temp_empty_length;
    uint32 temp_flag;
    uint8 i, debounce = 0;

    temp_flag = g_p_sys_hal_cbks->hal_int_close();
    for (i = 0; i < 40; i++)
    {
        temp_empty_length = *((REG32)(PCM_BUF_STAT)) & 0x1fff;
        if ((temp_empty_length == empty_length) || (temp_empty_length == (empty_length - 1)))
        {
            empty_length = temp_empty_length;
            debounce++;
            if (debounce >= 5)
            {
                break;
            }
        }
        else
        {
            empty_length = temp_empty_length;
            debounce = 0;
        }
    }
    g_p_sys_hal_cbks->hal_int_open(temp_flag);

    if (i == 40)
    {
        return (uint32) (-1);
    }
    else
    {
        return (0x1000 - empty_length);
    }
}

