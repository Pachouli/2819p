#ifndef __HAL_IRC_EXTERN_H__
#define __HAL_IRC_EXTERN_H__

#include <attr.h>
#include <typeext.h>

#define	IRC_MODE_9012      0
#define	IRC_MODE_Nec	   1
#define	IRC_MODE_RC5	   2
#define	IRC_MODE_RC6	   3

//IRC cbks
typedef void (*__irc_detect_key_cbk)(uint8 key_data);
typedef void (*__irc_repeat_key_cbk)(void);

#endif

