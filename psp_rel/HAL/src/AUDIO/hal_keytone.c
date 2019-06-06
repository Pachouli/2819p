
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

/*volume: (0-63，default 8), -64db - -12db， 推荐2-32即-30db - -10db*/
void hal_audio_set_key_tone_vol(uint32 volume)
{
    uint32 temp;
    temp = *((REG32)(KT_CTL)) & (~(KT_CTL_KTVOLL_MASK));
    temp |= (volume << KT_CTL_KTVOLL_SHIFT) & KT_CTL_KTVOLL_MASK;
    *((REG32)(KT_CTL)) = temp;
}

/*frequency: 0-3( default 2), 大致0.25, 0.33, 0.5, 1.0 kHz*/
void hal_audio_set_key_tone_freq(uint32 frequency)
{
    uint32 temp;
    temp = *((REG32)(KT_CTL)) & (~(KT_CTL_KTFR_MASK));
    temp |= (frequency << KT_CTL_KTFR_SHIFT) & KT_CTL_KTFR_MASK;
    *((REG32)(KT_CTL)) = temp;
}

/*volume: (0-3 default 2), 32-80, step 16*/
void hal_audio_set_key_tone_period(uint32 period)
{
    uint32 temp;
    temp = *((REG32)(KT_CTL)) & (~(KT_CTL_KTCNT_MASK));
    temp |= (period << KT_CTL_KTCNT_SHIFT) & KT_CTL_KTCNT_MASK;
    *((REG32)(KT_CTL)) = temp;
}

/*return 1表示正在播放，0表示停止播放*/
uint8 hal_audio_get_key_tone_status(void)
{
    uint8 temp;
    temp = (*((REG32)(KT_CTL)) & (1 << KT_CTL_SKT)) >> KT_CTL_SKT;
    return temp;
}

/*start keytone play*/
void hal_audio_start_key_tone(kt_setting_t *kt_sett)
{
    hal_audio_set_key_tone_vol(kt_sett->volume);
    hal_audio_set_key_tone_freq(kt_sett->freq);
    hal_audio_set_key_tone_period(kt_sett->period);

    *((REG32)(KT_CTL)) |= (1 << KT_CTL_SKT);
}

