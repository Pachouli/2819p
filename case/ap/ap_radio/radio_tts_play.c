/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用播报频点TTS。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

//生成频点播报TTS
uint8 radio_tts_fm_freq(uint16 value, bool europe_fm, uint8 *tts_sec_ids)
{
    uint8 section_cnt = 0;
    uint8 hundred, ten, one, dot, dot_euro;

    dot_euro = (uint8) (value % 2) * 5; //50Khz为单位
    value = value / 2; //100Khz为单位
    hundred = (uint8) (value / 1000);
    ten = (uint8) ((value % 1000) / 100);
    one = (uint8) ((value % 100) / 10);
    dot = (uint8) (value % 10);

    //FM
    tts_sec_ids[section_cnt] = TTS_SECTION_CHAR_FM;
    section_cnt++;

    //百位数处理
    if (hundred > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + hundred;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_HUNDRED;
        section_cnt++;
    }

    //十位数处理
    if (ten > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + ten;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_TEN;
        section_cnt++;
    }
    else if ((hundred > 0) && (one > 0)) //百位数不为0，且个位数不为0
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0;
        section_cnt++;
    }
    else
    {
        ; //for QAC
    }

    //个位数处理
    if (one > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + one;
        section_cnt++;
    }

    //点
    tts_sec_ids[section_cnt] = TTS_SECTION_DOT;
    section_cnt++;

    //100Khz处理
    tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + dot;
    section_cnt++;
    if (europe_fm == TRUE)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + dot_euro;
        section_cnt++;
    }

    return section_cnt;
}

//频点播报
void radio_freq_tts_play(uint32 freq)
{
    uint16 tmp_freq;
    uint8 tts_section_cnt;

    if (com_get_config_default(RADIO_ENABLE_FREQ_TTS) == 0)
    {
        return;
    }

    tmp_freq = (uint16) (freq / 50);

    tts_section_cnt = radio_tts_fm_freq(tmp_freq, (bool) (g_radio_config.band_mode == Band_Europe), g_freq_tts_sec_ids);

    com_tts_state_play_list(g_freq_tts_sec_ids, tts_section_cnt, 0);
}

/*! \endcond */

