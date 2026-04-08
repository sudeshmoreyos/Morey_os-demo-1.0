#ifdef BOOT_HELP_ENABLE

#include "../../../../../platform.h"
#pragma message("Controller Boot Help is Enabled")

#ifdef DISABLE_OS
#pragma message("OS Functionality is disabled, Only one task can be executed")
#endif

#ifdef OS_TIMER_TYPE_CONF

#if OS_TIMER_TYPE_CONF == TIMER_0
#pragma message("User configured OS TIMER to TIMER0")
#elif OS_TIMER_TYPE_CONF == TIMER_1
#pragma message("User configured OS TIMER to TIMER1")
#elif OS_TIMER_TYPE_CONF == TIMER_2
#pragma message("User configured OS TIMER to TIMER2")
#endif //OS_TIMER_TYPE_CONF == TIMER_0 

#else
	
#pragma message("By Default OS TIMER is configured to TIMER1 ")

#endif // OS_TIMER_TYPE_CONF


#endif