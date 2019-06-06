
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define DAC_VOL_REG_0DB    0x4000 //0db是相对音量，最大音量可以为-3db，用于在小音箱中以失真度换取响度

//0db(vol_p1_db = 0) ~ -60db(vol_p1_db = -600)
//vol_p1_db < -600 for mute
//return: >= 0 ok, < 0 fail
int32 hal_audio_set_audio_volume(int32 db_exp)
{
    uint32 temp;

    //0x4000 -- 0db
    //20lg(reg/0x4000)

    *((REG32)(DAC_VOL)) |= (1 << DAC_VOL_VOLZCEN);
    *((REG32)(DAC_VOL)) |= (1 << DAC_VOL_VOLZCTOEN);

    temp = *((REG32)(DAC_VOL)) & (~(DAC_VOL_VOL_MASK));
    temp |= (DAC_VOL_REG_0DB * db_exp / 32768) & DAC_VOL_VOL_MASK;
    *((REG32)(DAC_VOL)) = temp;

    return 1;
}

