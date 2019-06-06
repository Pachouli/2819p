
#ifndef __HAL_IRC_H__
#define __HAL_IRC_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_irc_extern.h"

//private defines
extern void hal_irc_phy_init(uint8 irc_mode, uint32 usercode, __irc_detect_key_cbk detect_key_cbk, __irc_repeat_key_cbk repeat_key_cbk);
extern void hal_irc_phy_exit(void);
extern int32 hal_irc_correct_usercode(uint32 *usercode);

#endif
