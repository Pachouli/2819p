#include "ap_test_head.h"
#include "ap_autotest_channel_entry.h"

__section__(".op_entry") const uint32 bank_mic_channel_entry[]={
	(uint32)act_test_mic_channel_test_ext,
	(uint32)act_test_linein_channel_test_ext,
};
