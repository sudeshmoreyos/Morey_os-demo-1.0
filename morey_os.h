#ifndef MOREY_OS_H
#define MOREY_OS_H

#include "platform.h"

// Macros for reverse compatibility with older version morey_os code
#define PROCESS(...) 			TASK_CREATE(__VA_ARGS__)
#define AUTOSTART_PROCESSES(...) 	TASK_AUTOSTART(__VA_ARGS__)
#define PROCESS_THREAD(...) 		TASK_RUN(__VA_ARGS__)
#define SUBPROCESS_CALL(...)		SUBTASK_CALL(__VA_ARGS__)
#define SUBPROCESS_CATCH(...)		SUBTASK_CATCH(__VA_ARGS__)
#define SUBPROCESS_RETURN		SUBTASK_RETURN

#ifdef DISABLE_OS
	#include "util/delay_blocking.h"
	#define TASK_CREATE(...) 
	#define TASK_AUTOSTART(...) 
	#define TASK_RUN(...) 			void loop(void)
	#define BEGIN()
	#define END()				while(1)
	#define DELAY_SEC(seconds) 		delay_ms_blocking(seconds*1000)
	#define DELAY_SEC_PRECISE(seconds) 	DELAY_SEC(seconds)
#else
	#include "os/sys/task.h"
	#include "os/sys/ptimer.h"
	#include "os/sys/clock.h"

	#define OS_INIT()				\
	PLATFORM_INIT();                          	\
	task_init();                  		      	\
	ptimer_init();                            	\
	task_start(&ptimer_task, NULL);			\
	task_autostart(autostart_tasks);	     	\
	while (1)                                 	\
	{                                         	\
		mos_uint8_t r;                        	\
		do                                    	\
		{                                     	\
			r = task_run();	                \
			WATCHDOG_PERIODIC();            \
		} while(r>0);                          	\
		OS_SLEEP();                           	\
	}
	  
	#define DELAY_SEC(second)					\
	{                                                               \
		ptimer_set_reset(TASK_CURRENT(), second, PTIMER_SET);	\
		TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_PTIMER);         \
	}

	#define DELAY_SEC_PRECISE(second)                               \
	{                                                               \
		ptimer_set_reset(TASK_CURRENT(), second, PTIMER_RESET);	\
		TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_PTIMER);         \
	}
	
	#define BEGIN()				\
	TASK_BEGIN();                        	\
	ptimer_start(TASK_CURRENT());		\
	TASK_PAUSE()
	
	#define END() TASK_END()
		 
	#define SUBTASK_CALL(subtask_name, data_pointer)			\
	if(!task_is_running(&subtask_name))					\
	task_start(&subtask_name,NULL);						\
	subtask_name.data2 = TASK_CURRENT();					\
	task_post(&subtask_name, TASK_EVENT_SUBTASK_CALL,data_pointer);		\
	TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_SUBTASK_RETURN);			\
	ptimer_start(TASK_CURRENT())

	#define SUBTASK_CATCH(data_pointer)					\
	TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_SUBTASK_CALL);			\
	data_pointer = TASK_CURRENT()->data;					\
	ptimer_start(TASK_CURRENT())

	#define SUBTASK_RETURN()								\
	task_post(TASK_CURRENT()->data2, TASK_EVENT_SUBTASK_RETURN,TASK_CURRENT()->data);    	\
	ptimer_start(TASK_CURRENT())
	
	
#endif                                                     
#endif