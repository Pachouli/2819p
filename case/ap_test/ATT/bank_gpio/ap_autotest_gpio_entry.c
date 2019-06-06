#include "ap_test_head.h"
#include "ap_autotest_gpio_entry.h"

__section__(".op_entry") const uint32 bank_gpio_entry[]={
	(uint32)act_test_gpio_test_ext,
};
