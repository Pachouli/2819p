
//HY
#include "common_func_inner.h"

#if (SUPPORT_HDMI_MODULE == 1)

bool com_hdmi_is_hdmi_func(uint8 func_id)
{
    if (func_id == APP_FUNC_HDMI_HDMI)
    {
        return TRUE;
    }
    else if (func_id == APP_FUNC_HDMI_HDMI1)
    {
        return TRUE;
    }
    else if (func_id == APP_FUNC_HDMI_HDMI2)
    {
        return TRUE;
    }
    else if (func_id == APP_FUNC_HDMI_ARC)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool com_hdmi_is_arc_func(uint8 func_id)
{
    if (func_id == APP_FUNC_HDMI_ARC)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

HOST_SRC com_hdmi_get_host_src(uint8 func_id)
{
    if (func_id == APP_FUNC_HDMI_HDMI)
    {
        return HOST_SRC_HDMI;
    }
    else if (func_id == APP_FUNC_HDMI_HDMI1)
    {
        return HOST_SRC_HDMI;
    }
    else if (func_id == APP_FUNC_HDMI_HDMI2)
    {
        return HOST_SRC_HDMI;
    }
    else if (func_id == APP_FUNC_HDMI_ARC)
    {
        return HOST_SRC_ARC;
    }
    else
    {
        return HOST_SRC_OTHER;
    }
}

HDMI_SRC com_hdmi_get_hdmi_src(uint8 func_id)
{
    if (func_id == APP_FUNC_HDMI_HDMI)
    {
        return HDMI_SRC_HDMI0;
    }
    else if (func_id == APP_FUNC_HDMI_HDMI1)
    {
        return HDMI_SRC_HDMI1;
    }
    else if (func_id == APP_FUNC_HDMI_HDMI2)
    {
        return HDMI_SRC_HDMI2;
    }
    else if (func_id == APP_FUNC_HDMI_ARC)
    {
        return HDMI_SRC_ARC;
    }
    else
    {
        return HDMI_SRC_NONE;
    }
}

uint8 hdmi_get_cur_func_id(void)
{
    return get_cur_func_id();
}

bool hdmi_get_cec_mute(void)
{
    return get_cec_mute_enable();
}

uint8 hdmi_get_sound_volume(void)
{
    return (com_get_sound_volume_inner()*CEC_VOLUME_MAX/VOLUME_VALUE_MAX);
}

#endif
