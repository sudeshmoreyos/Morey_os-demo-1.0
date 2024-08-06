#ifndef CLOCK_H
#define CLOCK_H

/** \addtogroup headers_sys
* @{ \defgroup clock_h
* @{
* \brief Documentaion of clock.h header file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file implements platform dependent clock functions
*
* **USER DEFINED MACROS** that can be used in clock.h (These macros should be declared in config.h file) are :
*1. **CLOCK_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {CLOCK_DEBUG_NONE, CLOCK_DEBUG_USER, CLOCK_DEBUG_LEVEL1, CLOCK_DEBUG_LEVEL2} \n
*=> Eg. Declaration : #define CLOCK_DEBUG_LEVEL_CONF CLOCK_DEBUG_LEVEL1 \n
*=> Default value : CLOCK_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of clock.h or clock.c, with increasing levels
*(i.e. level 1 2 3) deeper execution steps can be diagnosed. CLOCK_DEBUG_USER level enables CLOCK_DEBUG_MSG_USER Macro.
*To use this, user should manually write CLOCK_DEBUG_MSG_USER(msg,task_name) inside code sections of clock.h  or clock.c to be diagnosed.
*clock.c source file is platform dependent and can be found in boot folder of selected controller  \n
* @}
*/

#include "../../platform.h"

/// Defining CLOCK DEBUG MACROS

/**
* \name MACROS : Clock Debug Levels
* \brief Define different levels for clock debugging
* @{
*/
#define CLOCK_DEBUG_NONE 0
#define CLOCK_DEBUG_USER 1
#define CLOCK_DEBUG_LEVEL1 2
#define CLOCK_DEBUG_LEVEL2 3
///@}

/** 
* \brief			Get the current clock time.
* \param seconds_value   	A pointer to variable of type clock_second_t
* \param milliseconds_value  	A pointer to variable of type clock_millisecond_t
* \return void
*
* This function takes a pointer **seconds_value** to variable of type clock_second_t and
* a pointer to **milliseconds_value** to variable of type clock_millisecond_t. after 
* execution **seconds_value** holds current clock time second value while **milliseconds_value**
* holds current clock time millisecond value.
*/
void get_clock_time(clock_second_t * seconds_value, clock_millisecond_t * milliseconds_value );

/** 
* \brief	Schedule tickless timer for called task.
* \param p   	A pointer to task
* \return void
*
* This function checks expiry time of called task **p**. If condition satisfies it schedules
* it in the next tickless timer cycle. Tickless timer is platform dependent. If platform doesn't
* support tickless timer, this function does nothing.  
*/
void schedule_tickless_timer_platform(clock_millisecond_t milliseconds_next);


#endif /* CLOCK_H */
