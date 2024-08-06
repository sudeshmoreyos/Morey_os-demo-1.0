#ifndef TASK_H
#define TASK_H

/** \addtogroup headers_sys
* @{ \defgroup task_h
* @{
* \brief Documentaion of task.h header file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
* 
* **USER DEFINED MACROS** that can be used in task.h (These macros should be declared in config.h file) are :
*1. **TASK_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {TASK_DEBUG_NONE, TASK_DEBUG_USER, TASK_DEBUG_LEVEL1, TASK_DEBUG_LEVEL2, TASK_DEBUG_LEVEL3} \n
*=> Eg. Declaration : #define TASK_DEBUG_LEVEL_CONF TASK_DEBUG_LEVEL1 \n
*=> Default value : TASK_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of task.h, with increasing levels
*(i.e. level 1 2 3) deeper execution steps can be diagnosed. TASK_DEBUG_USER level enables TASK_DEBUG_MSG_USER Macro.
*To use this, user should manually write TASK_DEBUG_MSG_USER(msg,task_name) inside code sections of task.h to be diagnosed\n
*\n
*2. **TASK_HAS_STRING_NAMES_CONF** \n
*=> Allowed values : MACRO defined or undefined \n
*=> Eg. Declaration : #define TASK_HAS_STRING_NAMES_CONF \n
*=> Default value : macro not defined \n
*=> Usage Description : This macro allows use of string name allotted to each task. This string name can be used \n
*during debuging task. By default string names are not used by morey_os to reduce memory usage \n
* \n
*3. **TASK_NUMEVENTS_CONF** \n
*=> Allowed values : 1 to 255 \n
*=> Eg. Declaration : #define TASK_NUMEVENTS_CONF 64 \n
*=> Default value : 32 \n
*=> Usage Description : This macro declares number of events that can be queued \n
* @}
*/

// platform.h header provides platform dependent and platform independent datatypes
// NULL (NULL pointer) & data_address_t is platform dependent 
#include "../../platform.h"

// If user disables OS functionality Effectively task.h header file is removed
#ifndef DISABLE_OS

/**
* \name MACROS : Task Debug Levels
* \brief Define different levels for task debugging
* @{
*/
#define TASK_DEBUG_NONE 0
#define TASK_DEBUG_USER 1
#define TASK_DEBUG_LEVEL1 2
#define TASK_DEBUG_LEVEL2 3
#define TASK_DEBUG_LEVEL3 4
///@}
 
/**
* \name Typedefs
* \brief Define typedefs for task header
* @{
*/
typedef program_address_t task_position_t;	///< typedef for task position from where task should resume
typedef mos_uint8_t task_event_t; 		///< typedef for task event
typedef void * task_data_t;  			///< typedef for task data
typedef mos_uint8_t task_num_events_t;		///< typedef for variables keeping the count of events 
///@}

/**
* \name MACROS : Process function return types
* @{
*/
/// return value indicating that an operation was successful
#define TASK_ERR_OK 0
/// return value indicating that event queue is full
#define TASK_ERR_FULL 1
///@}

/**
* \name MACROS : Standard Process Events
* @{
*/
// most events are never used Need to review this in future
#define TASK_EVENT_NONE                 0x80
#define TASK_EVENT_INIT                 0x81
#define TASK_EVENT_POLL                 0x82
#define TASK_EVENT_EXIT                 0x83
#define TASK_EVENT_SERVICE_REMOVED      0x84
#define TASK_EVENT_CONTINUE             0x85
#define TASK_EVENT_MSG                  0x86
#define TASK_EVENT_EXITED               0x87
#define TASK_EVENT_TIMER                0x88
#define TASK_EVENT_COM                  0x89
#define TASK_EVENT_MAX                  0x8a
#define TASK_EVENT_SUBTASK_CALL		0x8b
#define TASK_EVENT_SUBTASK_RETURN     	0x8c
#define TASK_EVENT_PTIMER               0x8d
#define TASK_BROADCAST                  0x00
///@}

/**
* \name MACROS : TASK THREAD (PT) return values
* @{
*/
#define PT_WAITING 0  // This state is never used so far hence commented
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3
///@}

/**
* \name MACROS : TASK THREAD (PT) OS Core MACROS
* @{
*/
/// This macro starts the execution part of task thread (PT)
#define TASK_BEGIN()				\
{                                           	\
    mos_uint8_t PT_FLAG_VAR_ = 1;           	\
    switch (task_current->position)      	\
    {   case 0:                             
    
/// This macro ends the execution part of task thread (PT)   
#define TASK_END()                       	\
    }                                       	\
    task_current->position = 0;          	\
    return PT_ENDED;                        	\
}

/// This macro returns control from a task thread to OS
#define TASK_WAIT_EVENT()                	\
do {                                        	\
    PT_FLAG_VAR_ = 0;                       	\
    task_current->position = __LINE__;   	\
    case __LINE__ :                         	\
          if( PT_FLAG_VAR_ == 0 )           	\
           {                                	\
		return PT_YIELDED; 	        \
           }                            	\
} while(0)

/// This macro is same as TASK_WAIT_EVENT except that it also checks a condition   
#define TASK_WAIT_EVENT_UNTIL(condition) 		\
do {                                        		\
    PT_FLAG_VAR_ = 0;                       		\
    task_current->position = __LINE__;   		\
    case __LINE__ :                         		\
        if((PT_FLAG_VAR_ == 0) || !(condition))  	\
        {                                   		\
            return PT_YIELDED;              		\
        }                                   		\
} while(0)

/// This macro exits the current task before it reaches TASK_END
#define TASK_EXIT()				\
do {                                        	\
        task_current->position = 0;      	\
        return PT_EXITED;                   	\
} while(0)

///If there are more than one user auto start taskes, this macro is used to pause current task untill all other taskes are started after that this task start again from this point only
#define TASK_PAUSE()							\
do {                                        				\
        task_post(TASK_CURRENT(), TASK_EVENT_CONTINUE, NULL);  		\
        TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_CONTINUE);         	\
}while(0)
///@}


/**
* \name MACROS : TASK THREAD (PT) OS other important MACROS
* @{
*/
#ifdef TASK_HAS_STRING_NAMES_CONF
    #define TASK_HAS_STRING_NAMES
#endif      
    
/// This MACRO defines task thread function                                     
#define TASK_RUN(name)									\
static mos_uint8_t task_run_##name(task_event_t ev)
                       
/// This macro is used to declare the name of a task typically in a header file
#define TASK_GLOBAL(name) extern struct task_struct_os name

/// This macro declares a Process thread function and a task structure variable.
/// If TASK_HAS_STRING_NAMES is defined string name of task is store in data structure variable else not.
#ifdef TASK_HAS_STRING_NAMES
    #define TASK_CREATE(name, strname)                  \
        TASK_RUN(name);                             	\
        struct task_struct_os name = { NULL, strname,   \
                          task_run_##name }
#else
    #define TASK_CREATE(name, strname)			\
        TASK_RUN(name);                             	\
        struct task_struct_os name = { NULL,            \
                          task_run_##name }
#endif        

/// Define total allowed events that can be queued at a time.
/// To change default value TASK_NUMEVENTS_CONF should be defined
/// either in board.h default files or define in config.h by user 
#ifdef TASK_NUMEVENTS_CONF
    #define TASK_NUMEVENTS TASK_NUMEVENTS_CONF
#else 
    #define TASK_NUMEVENTS 32
#endif
///@}

struct task_struct_os {
  struct task_struct_os *next;				///< points to next task in the queue
  #ifdef TASK_HAS_STRING_NAMES
    const char *name; 					///< includes string nname of task if enabled   
  #endif
  mos_uint8_t (* thread)(task_event_t); 		///< Task thread function pointer
  task_position_t position; 				///< Task position variable from where task should resume
  task_data_t data, data2; 				///< Task data variables
  mos_uint8_t state, needspoll;				///< Task state and needspoll variable
  struct task_struct_os *ptimer_next;			///< Pointer for next ptimer queued by Ptimer task
  clock_second_t expiry_seconds;			///< Ptimer varible for seconds expiry
  clock_millisecond_t expiry_milliseconds;		///< Ptimer variable for milliseconds expiry
  mos_uint8_t ptimer_state;				///< Ptimer variable for its active status
};

/**
* \name Process Functions
* @{
*/
 
/** 
* \brief	Declare function to init task module
*/
void task_init(void);

/** 
* \brief		Declare function to start a task
* \param p   	A pointer to the task
* \param data 	Data to pass to the task of type task_data_t
* \return void
*
*	This function starts a task whose pointer is provided by input parameter **p**.
*	Optionally we can provide initial data to the task by input parameter **data**.
*	If no data has to be passed **NULL** can be passed as **data** parameter.
*/
void task_start(struct task_struct_os *p, task_data_t data);


/** 
* \brief		Declare function to exit any task whether current or any other which is running
* \param p   	A pointer to the task
* \return void	
*
*	This function exits a task whose pointer is provided by input parameter **p**.
*	This task could be current or any other running task.
*/
void task_exit(struct task_struct_os * p);


/** 
* \brief		Declare function which posts a asynchronous event.
* \param p   	A pointer to the task
* \param data 	Data to pass to the task of type task_data_t
* \return int	Returns TASK_ERR_OK on success else TASK_ERR_FULL
*
*	This function post a asynchronous event of a task whose pointer is provided by input parameter **p**.
*	Asynchronous event means, OS will not service this event immediately but somtime in future.
*	Optionally we can provide initial data to the task by input parameter **data**.
*	If no data has to be passed **NULL** can be passed as **data** parameter.
*/
int task_post(struct task_struct_os * p, task_event_t ev, task_data_t data);

/** 
* \brief		Declare a function to poll a task
* \param p   	A pointer to the task
* \return void	
*
*	This function polls a task whose pointer is provided by input parameter **p**.
*	Polling is method to service high priority taskes.
*	In every run loop, OS serivce a Polled event and a normal event.
*	By this way polled event will have higher chances of getting serviced.
*	However it still can't gaurantee real time service.
*/
void task_poll(struct task_struct_os *p);

/**
* \brief Run the system once - call poll handlers and task one event.
* \return The number of events that are currently waiting in the event queue.
*
*	This function should be called repeatedly from the main() program
*	to actually run the Contiki system. It calls the necessary poll
*	handlers, and taskes one event. The function returns the number
*	of events that are waiting in the event queue so that the caller
*	may choose to put the CPU to sleep when there are no pending
*	events.
*/
int task_run(void);

/** 
* \brief		Declare function to check if a task is running or not
* \param p   	A pointer to the task
* \return int	Returns 1 for success else 0
*/
int task_is_running(struct task_struct_os *p);

/// Function to autostart taskes
void task_autostart(struct task_struct_os * const tasks[]);
///@}

/**
* \name MACROS : TASK THREAD (PT) OS miscellaneous MACROS
* @{
*/

/// Declare TASK_NAME_STRING(task) macro, used to print task string name, if enabled
#ifdef TASK_HAS_STRING_NAMES										
	#define TASK_NAME_STRING(task) (task)->name    
#else
    #define TASK_NAME_STRING(task) ""    
#endif

/// This macro is used to get the pointer to current task
#define TASK_CURRENT() task_current

/// Declare task_current structure visible to all other files 
extern struct task_struct_os *task_current;

/// Declare task_list structure visible to all other files
extern struct task_struct_os *task_list;

/// This macro is used to get the pointer to task_list
#define TASK_LIST() task_list

/// Declare AUTOSTART_TASKES used to auto start selected taskes after boot
#define TASK_AUTOSTART(...)						\
struct task_struct_os * autostart_tasks[] = {__VA_ARGS__, NULL}

///@}

#endif
#endif

