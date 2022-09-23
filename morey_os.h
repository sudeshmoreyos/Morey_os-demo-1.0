#ifndef _MOREY_OS_H
#define _MOREY_OS_H

#include "platform.h"
#define TASK_CREATE(...) PROCESS(__VA_ARGS__)
#define TASK_AUTOSTART(...) AUTOSTART_PROCESSES(__VA_ARGS__)
#define TASK_RUN(...) PROCESS_THREAD(__VA_ARGS__)

#ifdef DISABLE_OS
	#include "util/delay_blocking.h"
	#define PROCESS(...) 
	#define AUTOSTART_PROCESSES(...) 
	#define PROCESS_THREAD(...) void loop(void)
	#define BEGIN()
	#define END()
	#define DELAY_SEC(seconds) 			_delay_ms(seconds*1000)
	#define DELAY_SEC_PRECISE(seconds) 	DELAY_SEC(seconds)
#else
	
	#include "os/sys/process.h"
	#include "os/sys/ptimer.h"
	#include "os/sys/clock.h"

	#define OS_INIT()                         \
	PLATFORM_INIT();                          \
	process_init();                           \
	ptimer_init();                            \
	process_start(&ptimer_process, NULL);     \
	autostart_start(autostart_processes);     \
	while (1)                                 \
	{                                         \
		mos_uint8_t r;                        \
		do                                    \
		{                                     \
			r = process_run();                \
			WATCHDOG_PERIODIC();              \
		}while(r>0);                          \
		OS_SLEEP();                           \
	}
	  
	#define DELAY_SEC(second)                                               \
	{                                                                       \
		ptimer_set_reset(PROCESS_CURRENT(), second, PTIMER_SET);            \
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_PTIMER);               \
	}

	#define DELAY_SEC_PRECISE(second)                                       \
	{                                                                       \
		ptimer_set_reset(PROCESS_CURRENT(), second, PTIMER_RESET);          \
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_PTIMER);               \
	}

	#define BEGIN()		                	\
	PROCESS_BEGIN();                        \
	PROCESS_PAUSE();                        \
	ptimer_start(PROCESS_CURRENT())
	
	#define END() PROCESS_END()

		 
	#define SUBPROCESS_CALL(subprocess_name, data_pointer)                                  \
	if(!process_is_running(&subprocess_name))                                               \
	process_start(&subprocess_name,NULL);													\
	subprocess_name.data2 = PROCESS_CURRENT();                                              \
	process_post(&subprocess_name, PROCESS_EVENT_SUBPROCESS_CALL,data_pointer);             \
	PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_SUBPROCESS_RETURN);                        \
	ptimer_start(PROCESS_CURRENT())

	#define SUBPROCESS_CATCH(data_pointer)         							\
	PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_SUBPROCESS_CALL);      	\
	data_pointer = PROCESS_CURRENT()->data;			                        \
	ptimer_start(PROCESS_CURRENT())


	#define SUBPROCESS_RETURN()						                                                	\
	process_post(PROCESS_CURRENT()->data2, PROCESS_EVENT_SUBPROCESS_RETURN,PROCESS_CURRENT()->data);    \
	ptimer_start(PROCESS_CURRENT())
	
	
#endif                                                     
#endif