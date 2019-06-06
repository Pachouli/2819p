
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

void hal_I2C_slave_hw_init(void);
void hal_I2C_slave_hw_exit(void);
void hal_I2C_slave_int_handle(void);

static twi_slave_hw_info_t g_I2C_slave_hw_info;
static bool g_I2C_slave_enable;
static bool g_I2C_slave_read_mode;

void hal_TWI_slave_set_hw_info(twi_slave_hw_info_t *p_hw_info)
{
    if ((p_hw_info != NULL) && (p_hw_info->r_handle != NULL) && (p_hw_info->w_handle != NULL))
    {
        g_p_sys_hal_cbks->hal_memcpy(&g_I2C_slave_hw_info, p_hw_info, sizeof(twi_slave_hw_info_t));
        if (g_I2C_slave_enable == FALSE)
        {
            hal_I2C_slave_hw_init();
            g_I2C_slave_enable = TRUE;
        }
    }
    else
    {
        g_p_sys_hal_cbks->hal_memset(&g_I2C_slave_hw_info, 0x00, sizeof(twi_slave_hw_info_t));
        if (g_I2C_slave_enable == TRUE)
        {
            hal_I2C_slave_hw_exit();
            g_I2C_slave_enable = FALSE;
        }
    }
}

void hal_I2C_slave_hw_init(void)
{
    // I2C module reset
    *((REG32)(MRCR)) &= (~(1<<MRCR_IIC0_RESET));
    // I2C module reset disable
    *((REG32)(MRCR)) |= (1<<MRCR_IIC0_RESET);

    // I2C clock select HOSC
    // I2C clock enable
    *((REG32)(CMU_DEVCLKEN)) |= (1<<CMU_DEVCLKEN_I2C0CLKEN);

    // enable I2C interrupt pending
    *((REG32)(INTC_MSK)) |= (1<<INTC_MSK_IIC0_IM);

    g_p_sys_hal_cbks->hal_request_irq(IRQ_I2C0, hal_I2C_slave_int_handle);

    //*((REG32)(PADPUPD)) |= (1 << PADPUPD_I2C); // I2C 10k PU Enable

    *((REG32)(I2C0_CTL)) = (1<<I2C0_CTL_EN)|(1<<5); // enable I2C, slave mode, bitrate default 400kbps
    *((REG32)(I2C0_ADDR)) = g_I2C_slave_hw_info.addr;    // I2C slave address
    *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_IRQE);   // enable irq
}

void hal_I2C_slave_hw_exit(void)
{
    *((REG32)(I2C0_CTL)) &= ~(1<<I2C0_CTL_IRQE);  // disable irq

    g_p_sys_hal_cbks->hal_free_irq(IRQ_I2C0);

    // disable I2C interrupt pending
    *((REG32)(INTC_MSK)) &= (~(1<<INTC_MSK_IIC0_IM));

    // I2C clock diable
    *((REG32)(CMU_DEVCLKEN)) &= (~(1<<CMU_DEVCLKEN_I2C0CLKEN));

    // I2C module reset
    *((REG32)(MRCR)) &= (~(1<<MRCR_IIC0_RESET));
}

void hal_I2C_slave_int_handle(void)
{
    // I2C irq pending check
    if((*((REG32)(I2C0_STAT)) & (1<<I2C0_STAT_IRQP)) != 0)
    {
        *((REG32)(I2C0_STAT)) = (1<<I2C0_STAT_IRQP);//clear I2C IRQ pending

        // bus err detect
        if ((*((REG32)(I2C0_STAT)) & (1<<I2C0_STAT_BEB)) != 0)
        {
            if (g_I2C_slave_hw_info.err_handle != NULL)
            {
                g_I2C_slave_hw_info.err_handle();
            }

            // clear BEB
            *((REG32)(I2C0_STAT)) = (1<<I2C0_STAT_BEB);

            *((REG32)(I2C0_STAT)) = 0x1ff;   // reset status register
            *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_RB);//Release Bus.

            return;
        }

        // stop detect
        if ((*((REG32)(I2C0_STAT)) & (1<<I2C0_STAT_STPD)) != 0)
        {
            if (g_I2C_slave_hw_info.stop_handle != NULL)
            {
                g_I2C_slave_hw_info.stop_handle();
            }

            //clear stop detect pending bit
            *((REG32)(I2C0_STAT)) = (1<<I2C0_STAT_STPD);

            *((REG32)(I2C0_STAT)) = 0x1ff;   // reset status register
            *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_RB);//Release Bus.

            return;
        }

        // transfer complete detect
        if ((*((REG32)(I2C0_STAT)) & (1<<I2C0_STAT_TCB)) != 0)
        {
            // clear transfer complete detect
            *((REG32)(I2C0_STAT)) = (1<<I2C0_STAT_TCB);

            // data/addr branch, 1: data  0: addr
            if ((*((REG32)(I2C0_STAT)) & (1<<I2C0_STAT_LBST)) != 0)
            {
                // data process

                // write/read branch, 1: read  0: write
                // master read mode --> slave write mode
                if (g_I2C_slave_read_mode == FALSE)
                {
                    // NACK received
                    if ((*((REG32)(I2C0_STAT)) & (1<<I2C0_STAT_RACK)) == 0)
                    {
                        *((REG32)(I2C0_STAT)) = 0x1ff;   // reset status register
                        *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_RB);     // Release Bus
                        return;
                    }

                    // write tx reg
                    *((REG32)(I2C0_TXDAT)) = g_I2C_slave_hw_info.w_handle();

                    *((REG32)(I2C0_STAT)) = 0x1ff;   // reset status register
                    *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_RB);     // Release Bus
                    return;
                }
                // master write mode --> slave read mode
                else
                {
                    bool next_byte_ack;
                    // prepare to send ACK
                    *((REG32)(I2C0_CTL)) &= (~(1<<I2C0_CTL_GRAS));

                    next_byte_ack = g_I2C_slave_hw_info.r_handle(*((REG32)(I2C0_RXDAT)));
                    if (next_byte_ack == FALSE)
                    {
                        *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_GRAS);
                    }

                    *((REG32)(I2C0_STAT)) = 0x1ff;   // reset status register
                    *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_RB);     // Release Bus
                    return;
                }
            }
            else
            {
                // addr process

                g_I2C_slave_read_mode = TRUE;
                if ((*((REG32)(I2C0_FIFOSTAT)) & (1<<I2C0_FIFOSTAT_WRS)) != 0)
                {//1 --> master read
                    g_I2C_slave_read_mode = FALSE;
                }

                // master write mode --> slave read mode, set first byte ack
                if (g_I2C_slave_read_mode == TRUE)
                {
                    // prepare to send ACK
                    *((REG32)(I2C0_CTL)) &= (~(1<<I2C0_CTL_GRAS));
                }
                // master read mode --> slave write mode, we need to prepare 1 byte in advance
                else
                {
                    *((REG32)(I2C0_TXDAT)) = g_I2C_slave_hw_info.w_handle();
                }

                *((REG32)(I2C0_STAT)) = 0x1ff;           // reset status register
                *((REG32)(I2C0_CTL)) |= (1<<I2C0_CTL_RB); // Release Bus
                return;
            }
        }
    }
}
