#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define MAX_SIRQ_ID  4
interrupt_handle_cb g_sirq_handle[MAX_SIRQ_ID];
uint8 sirq_gpio[MAX_SIRQ_ID];
uint8 sirq_handle_used[MAX_SIRQ_ID];

void hal_sirq_handle(void)
{
    uint8 trig_type;
    uint8 cur_idx;
    uint32 GPIO_CTRL_X ;
    uint32 gpio_pd;

    if (*((REG32) GPIO_PD0) != 0)
    {
        //sirq 0 ~ 31
        gpio_pd = *((REG32) GPIO_PD0);

        //find idx
        for (cur_idx = 0; cur_idx < MAX_SIRQ_ID; cur_idx++)
        {
            if ((sirq_handle_used[cur_idx] == 1) && (sirq_gpio[cur_idx] <= 31) && (((gpio_pd & (1<<sirq_gpio[cur_idx])) != 0)))
            {
                break;
            }
        }

        if (cur_idx < MAX_SIRQ_ID)
        {
            *((REG32) GPIO_PD0) |= (1<<sirq_gpio[cur_idx]);
        }
        else
        {
            g_p_sys_hal_cbks->hal_printf("sirq invalid pending, clear\n");
            *((REG32) GPIO_PD0) = *((REG32) GPIO_PD0);
            return;
        }
    }
    else if (*((REG32) GPIO_PD1) != 0)
    {
        //sirq 32 ~ 51
        gpio_pd = *((REG32) GPIO_PD1);

        //find idx
        for (cur_idx = 0; cur_idx < MAX_SIRQ_ID; cur_idx++)
        {
            if ((sirq_handle_used[cur_idx] == 1) && (sirq_gpio[cur_idx] >= 32) && (((gpio_pd & (1<<(sirq_gpio[cur_idx]-32))) != 0)))
            {
                break;
            }
        }

        if (cur_idx < MAX_SIRQ_ID)
        {
            *((REG32) GPIO_PD1) |= (1<<(sirq_gpio[cur_idx]-32));
        }
        else
        {
            g_p_sys_hal_cbks->hal_printf("sirq invalid pending, clear\n");
            *((REG32) GPIO_PD1) = *((REG32) GPIO_PD1);
            return;
        }
    }
    else
    {
        g_p_sys_hal_cbks->hal_printf("sirq invalid pending!\n");
        return;
    }

    GPIO_CTRL_X = GPIO0_CTL + 4 * sirq_gpio[cur_idx];

    trig_type = (*((REG32) GPIO_CTRL_X) >> GPIO0_CTL_GPIO_TRIG_CTL_SHIFT) & 0x07;
    if (g_sirq_handle[cur_idx] == NULL)
    {
        g_p_sys_hal_cbks->hal_printf("sirq handler is null\n");
    }

    (g_sirq_handle[cur_idx])(trig_type);
}

void hal_set_sirq_inner(uint8 sirq_num, sirq_set_para_t *sirq_set)
{
    uint32 GPIO_CTRL_X;
    uint8 cur_idx;

    if (((sirq_num > 37) && (sirq_num < 48)) || (sirq_num > 51))
    {
        g_p_sys_hal_cbks->hal_printf("invalid sirq gpio, valid range[0,37][48,51]\n");
        return;
    }

    //find idx
    for (cur_idx = 0; cur_idx < MAX_SIRQ_ID; cur_idx++)
    {
        if ((sirq_handle_used[cur_idx] == 1) && (sirq_gpio[cur_idx] == sirq_num))
        {
            break;
        }
    }
    if (cur_idx < MAX_SIRQ_ID)
    {
        goto update_sirq;
    }

    for (cur_idx = 0; cur_idx < MAX_SIRQ_ID; cur_idx++)
    {
        if (sirq_handle_used[cur_idx] == 0)
        {
            break;
        }
    }
    if (cur_idx == MAX_SIRQ_ID)
    {
        g_p_sys_hal_cbks->hal_printf("sirq idx is not available\n");
        return;
    }

    update_sirq:

    if (sirq_set->triggle < SIRQ_TRIGGLE_MAX)
    {
        sirq_handle_used[cur_idx] = 1;
        sirq_gpio[cur_idx] = sirq_num;
        g_sirq_handle[cur_idx] = sirq_set->handle;

        GPIO_CTRL_X = GPIO0_CTL + 4 * sirq_num;

        if (sirq_num < 32)
        {
            *((REG32) GPIO_PD0) |= (1<<sirq_num);
        }
        else
        {
            *((REG32) GPIO_PD1) |= (1<<(sirq_num-32));
        }
        *((REG32) GPIO_CTRL_X) |= (1 << GPIO0_CTL_GPIO_INTC_MSK);//GPIO INTC MASK

        *((REG32) GPIO_CTRL_X) &= (~(0x0f << GPIO0_CTL_MFP_SHIFT));
        *((REG32) GPIO_CTRL_X) |= (0x01 << GPIO0_CTL_MFP_SHIFT); //set mfp to gpio

        *((REG32) GPIO_CTRL_X) &= (~(0x01 << GPIO0_CTL_GPIOOUTEN));//disable output
        *((REG32) GPIO_CTRL_X) |= (0x01 << GPIO0_CTL_GPIOINEN); //enable input

        *((REG32) GPIO_CTRL_X) &= (~(0x07 << GPIO0_CTL_GPIO_TRIG_CTL_SHIFT));
        *((REG32) GPIO_CTRL_X) |= (sirq_set->triggle << GPIO0_CTL_GPIO_TRIG_CTL_SHIFT);//set gpio triggle mode

        *((REG32) GPIO_CTRL_X) |= (1 << GPIO0_CTL_GPIO_INTCEN);

        g_p_sys_hal_cbks->hal_request_irq(IRQ_GPIO, hal_sirq_handle);
    }
    else
    {
        g_p_sys_hal_cbks->hal_printf("sirq triggle param is invalid\n");
    }
}

void hal_disable_sirq_inner(uint8 sirq_num)
{
    uint32 GPIO_CTRL_X;
    uint8 cur_idx;

    if (((sirq_num > 37) && (sirq_num < 48)) || (sirq_num > 51))
    {
        g_p_sys_hal_cbks->hal_printf("invalid sirq gpio, valid range[0,37][48,51]\n");
        return;
    }

    //find idx
    for (cur_idx = 0; cur_idx < MAX_SIRQ_ID; cur_idx++)
    {
        if ((sirq_handle_used[cur_idx] == 1) && (sirq_gpio[cur_idx] == sirq_num))
        {
            break;
        }
    }
    if (cur_idx < MAX_SIRQ_ID)
    {
        GPIO_CTRL_X = GPIO0_CTL + 4 * sirq_num;
        if (sirq_num < 32)
        {
            *((REG32) GPIO_PD0) |= (1<<sirq_num);
        }
        else
        {
            *((REG32) GPIO_PD1) |= (1<<(sirq_num-32));
        }
        *((REG32) GPIO_CTRL_X) &= ~(1 << GPIO0_CTL_GPIO_INTC_MSK);//GPIO INTC MASK
        *((REG32) GPIO_CTRL_X) &= ~(1 << GPIO0_CTL_GPIO_INTCEN);

        g_sirq_handle[cur_idx] = NULL;
        sirq_handle_used[cur_idx] = 0;
    }
    else
    {
        g_p_sys_hal_cbks->hal_printf("sirq idx is not exist:%d\n", sirq_num);
    }
}

