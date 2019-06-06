
#include "hdmi_interface.h"

void hdmi_module_install_inner(void);
void hdmi_sirq_set_inner(uint8 flag);
uint8 hdmi_sirq_get_inner(void);
void hdmi_intb_set_inner(uint8_t enable);
uint8 hdmi_intb_get_inner(void);

const const uint32 __section__(".rodata.op") hdmi_op0[] =
{
    (uint32) hdmi_module_install_inner,
};

static uint8 sirq_signal = 0;

static void hdmi_module_reset(uint8_t level)
{
    //Reset
    set_pad_function(HDMI_RST_PIN, HDMI_RST_FUNC, NULL, (void *) (level?1:0));
}

void hdmi_module_install_inner(void)
{
    hdmi_hw_info_t hdmi_hw_info;
    pad_phy_para_t phy_para;

    phy_para.pad_pu_en = 1;
    phy_para.pad_pd_en = 0;
    phy_para.pad_driver = 1;

    //SCL
    hdmi_hw_info.scl_pad = HDMI_SCL_PIN;
    hdmi_hw_info.scl_func = HDMI_SCL_FUNC;
    hdmi_hw_info.scl_in_func = HDMI_SCL_IN_FUNC;
    hdmi_hw_info.scl_phy.pad_pu_en = 2;
    hdmi_hw_info.scl_phy.pad_pd_en = 0;
    hdmi_hw_info.scl_phy.smit_en = 0;
    hdmi_hw_info.scl_phy.pad_driver = 1;

    //SDA
    hdmi_hw_info.sda_pad = HDMI_SDA_PIN;
    hdmi_hw_info.sda_func = HDMI_SDA_FUNC;
    hdmi_hw_info.sda_in_func = HDMI_SDA_IN_FUNC;
    hdmi_hw_info.sda_phy.pad_pu_en = 2;
    hdmi_hw_info.sda_phy.pad_pd_en = 0;
    hdmi_hw_info.sda_phy.smit_en = 0;
    hdmi_hw_info.sda_phy.pad_driver = 1;

    //HY Demo: HDMI share I2C pin with FM, so, just initial pad in FM
    //SCL
    //set_pad_function(HDMI_SCL_PIN, HDMI_SCL_FUNC, &hdmi_hw_info.scl_phy, (void *) 1);
    //set_pad_function(HDMI_SCL_PIN, HDMI_SCL_IN_FUNC, &hdmi_hw_info.scl_phy, (void *) 0);

    //SDA
    //set_pad_function(HDMI_SDA_PIN, HDMI_SDA_FUNC, &hdmi_hw_info.sda_phy, (void *) 1);
    //set_pad_function(HDMI_SDA_PIN, HDMI_SDA_IN_FUNC, &hdmi_hw_info.sda_phy, (void *) 0);

    //INTB
    set_pad_function(HDMI_INT_PIN, HDMI_INT_FUNC, &phy_para, (void *) 0);

    //I2C bus for HDMI module
    hal_i2c_init(SMBUS_0, SMBUS_DividedBy512, &hdmi_hw_info);

    //Reset HDMI module
    hdmi_module_reset(0);
    sys_mdelay(30);
    hdmi_module_reset(1);
}


void hdmi_sirq_set_inner(uint8 flag)
{
	sirq_signal = flag ? TRUE:FALSE;
}

uint8 hdmi_sirq_get_inner(void)
{
	return (sirq_signal ? TRUE:FALSE);
}

static void hdmi_SIRQ_handle(uint8_t triggle_type)
{
	sirq_set_para_t sirq_set;
	sirq_set.handle = hdmi_SIRQ_handle;

	//just only set the flag
	//the read operation will be done in timer
	//this will reduce the time of interrupt disable
	hdmi_sirq_set_inner(TRUE);

	sirq_set.triggle = SIRQ_TRIGGLE_FALLINGEDGE;
	sirq_set.default_edge = 0;
	sirq_set.realtime_mode = 0;
	hal_set_sirq((HDMI_INT_PIN-PAD_GPIO0), &sirq_set);
}

void hdmi_intb_set_inner(uint8_t enable)
{
	if(!enable) return;

	sirq_set_para_t sirq_set;
	sirq_set.handle = hdmi_SIRQ_handle;

	sirq_set.triggle = SIRQ_TRIGGLE_FALLINGEDGE;
	sirq_set.default_edge = 0;
	sirq_set.realtime_mode = 0;
	hal_set_sirq((HDMI_INT_PIN-PAD_GPIO0), &sirq_set);
}

uint8 hdmi_intb_get_inner(void)
{
	uint8_t value;

    ioctl_pad(HDMI_INT_PIN, HDMI_INT_FUNC, &value);
	return (value ? TRUE:FALSE);
}

