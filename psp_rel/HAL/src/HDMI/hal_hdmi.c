
//_manfan 2018.05.18
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

static hdmi_hw_info_t t_hdmi_hw;
static uint8 IIC_Busy[SMBUS_MAX] = {0,0,0,0};
static SMBUS_SPEED IIC_Speed[SMBUS_MAX] = {0,0,0,0};
static uint8 IO_Sel = 0, DELAY_Sel = 0;

static uint8 SCL_GET(void)
{
    uint8 value;

    hal_ioctl_pad(t_hdmi_hw.scl_pad, t_hdmi_hw.scl_in_func, &value);

    return value;
}

static void SCL_SET(bool flag)
{
	hal_hdmi_set_pad_function(t_hdmi_hw.scl_pad, t_hdmi_hw.scl_func, &t_hdmi_hw.scl_phy, (void *) (flag?1:0));
	hal_hdmi_set_pad_function(t_hdmi_hw.scl_pad, t_hdmi_hw.scl_in_func, &t_hdmi_hw.scl_phy, (void *) 0);
}

static uint8 SDA_GET(void)
{
    uint8 value;

    hal_ioctl_pad(t_hdmi_hw.sda_pad, t_hdmi_hw.sda_in_func, &value);

    return value;
}

static void SDA_SET(bool flag)
{
	hal_hdmi_set_pad_function(t_hdmi_hw.sda_pad, t_hdmi_hw.sda_func, &t_hdmi_hw.sda_phy, (void *) (flag?1:0));
	hal_hdmi_set_pad_function(t_hdmi_hw.sda_pad, t_hdmi_hw.sda_in_func, &t_hdmi_hw.sda_phy, (void *) 0);
}

static void hdmi_delay(uint8_t delay)
{
    g_p_sys_hal_cbks->hal_udelay(delay);
}

static void IIC_sel(SMBUS_ID BusSel, SMBUS_SPEED Speed)
{
	if(BusSel >= SMBUS_MAX) return;

	IIC_Speed[BusSel] = Speed;
}

void iic_init(SMBUS_ID BusSel, SMBUS_SPEED Speed, hdmi_hw_info_t *p_hdmi_hw_info)
{
	if(BusSel >= SMBUS_MAX) return;

	IIC_Busy[BusSel] = 0;
	IIC_sel(BusSel, Speed);

	g_p_sys_hal_cbks->hal_memcpy(&t_hdmi_hw, p_hdmi_hw_info, sizeof(hdmi_hw_info_t));
	g_p_sys_hal_cbks->hal_printf("\n<HDMI> hal i2c init @%d\n\n", g_p_sys_hal_cbks->hal_get_ab_timer_us()/1000);
}

static void IIC_Speed_Adj(void)
{
	switch(DELAY_Sel)
	{
		case SMBUS_DividedBy1024:
			hdmi_delay(6);//25 KHz
		case SMBUS_DividedBy512:
			hdmi_delay(4);//50 KHz
		case SMBUS_DividedBy256:  // 100 KHz
		case SMBUS_DividedBy128:  // 100 KHz
			;
	}
}

static SMBUS_STATUS IIC_Signal(uint8_t SCL, uint8_t SDA)
{
	uint32_t t, t1;

	t1 = g_p_sys_hal_cbks->hal_get_ab_timer_us();

	if(!SCL)
	{
		SCL_SET(0);
		IIC_Speed_Adj();
	}

	SDA_SET(SDA);
	IIC_Speed_Adj();

	SCL_SET(1);

	do
	{
		t = g_p_sys_hal_cbks->hal_get_ab_timer_us() - t1;
	}while((t < 25000) && (SCL_GET() == 0));
	if(t >= 25000) return SMBUS_STATUS_TimeOut;

	if(SDA_GET() != SDA) return SMBUS_STATUS_SDA_Low;
	
	return SMBUS_STATUS_Success;
}

static SMBUS_STATUS start_condition(void)
{ 
	SMBUS_STATUS status = SMBUS_STATUS_Success;

	status = IIC_Signal(0,1);
	status = IIC_Signal(1,1);
	status = IIC_Signal(1,0);

	return status;
}

static SMBUS_STATUS stop_condition(void)
{
	SMBUS_STATUS status = SMBUS_STATUS_Success;

	status = IIC_Signal(0,0);
	status = IIC_Signal(1,1);
	
	return status;
}

static SMBUS_STATUS tx8(uint8_t tx_data)
{
	uint8_t i;
	SMBUS_STATUS status = SMBUS_STATUS_Success;

	for(i=0; i<8; ++i)
	{
		if(tx_data & 0x80)
		{
			status = IIC_Signal(0,1);
		}
		else 
		{
			status = IIC_Signal(0,0);
		}

		if(status) return status;
		tx_data <<= 1;
	}

	status = IIC_Signal(0,1);

	if(status == SMBUS_STATUS_SDA_Low) return SMBUS_STATUS_Success;
	if(status == SMBUS_STATUS_Success) return SMBUS_STATUS_NoAct;

	return status;
}

static SMBUS_STATUS rx8(uint8_t* rx_data, uint8_t last)
{
	uint8_t i;
	SMBUS_STATUS status = SMBUS_STATUS_Success;

	*rx_data = 0;

	for(i=0; i<8; ++i)
	{
		*rx_data <<= 1;
		status = IIC_Signal(0,1);

		if(status == SMBUS_STATUS_Success) *rx_data |= 1;
		if(status == SMBUS_STATUS_TimeOut) return SMBUS_STATUS_TimeOut;
	}

	if(last) 
	{
		status = IIC_Signal(0,1);
	}
	else 
	{
		status = IIC_Signal(0,0);
	}

	return status;
}

SMBUS_STATUS iic_sync(SMBUS_ID BusSel, uint8_t Addr, uint8_t *pDatas, uint16_t Length, SMBUS_MODE Mode)
{
	uint16_t i;
	SMBUS_STATUS status = SMBUS_STATUS_Success;

	if(IIC_Busy[BusSel]) return SMBUS_STATUS_Pending;

	IO_Sel = BusSel;
	IIC_Busy[BusSel] = 1;
	DELAY_Sel = IIC_Speed[BusSel];

	if(!(Mode & SMBUS_SkipStart))
	{
		status = start_condition();
		if(status == SMBUS_STATUS_Success)
		{
			status = tx8(Addr);
		}
	}

	if(status == SMBUS_STATUS_Success)
	{
		if(Addr & 0x01)
		{
			for(i=0; (i<Length) && (status == SMBUS_STATUS_Success); ++i)
			{
				if((i == (Length-1)) && !(Mode & SMBUS_SkipStop))
				{
					status = rx8(&pDatas[i], 1);
				}
				else 
				{
					status = rx8(&pDatas[i], 0);
				}
			}
		}
		else 
		{
			for(i=0; (i<Length) && (status == SMBUS_STATUS_Success); ++i)
			{
				status = tx8(pDatas[i]);
			}
		}

		if(!(Mode & SMBUS_SkipStop))
		{
			status = stop_condition();
		}
	}

	IIC_Busy[BusSel] = 0;

	return status;
}

