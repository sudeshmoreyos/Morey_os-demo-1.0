#ifndef PROCESS_H
#define PROCESS_H

/** \defgroup os
* @{ \addtogroup header
* @{ \defgroup process_h
* @{ \addtogroup process_h 
* @{
* \brief Documentaion of process.h header file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
* 
* **USER DEFINED MACROS** that can be used in process.h (These macros should be declared in config.h file) are :
*1. **PROCESS_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {PROCESS_DEBUG_NONE, PROCESS_DEBUG_USER, PROCESS_DEBUG_LEVEL1, PROCESS_DEBUG_LEVEL2, PROCESS_DEBUG_LEVEL3} \n
*=> Eg. Declaration : #define PROCESS_DEBUG_LEVEL_CONF PROCESS_DEBUG_LEVEL1 \n
*=> Default value : PROCESS_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of process.h, with increasing levels
*(i.e. level 1 2 3) deeper execution steps can be diagnosed. PROCESS_DEBUG_USER level enables PROCESS_DEBUG_MSG_USER Macro.
*To use this, user should manually write PROCESS_DEBUG_MSG_USER(msg,process_name) inside code sections of process.h to be diagnosed\n
*\n
*2. **PROCESS_HAS_STRING_NAMES_CONF** \n
*=> Allowed values : MACRO defined or undefined \n
*=> Eg. Declaration : #define PROCESS_HAS_STRING_NAMES_CONF \n
*=> Default value : macro not defined \n
*=> Usage Description : This macro allows use of string name allotted to each process. This string name can be used \n
*during debuging process. By default string names are not used by morey_os to reduce memory usage \n
* \n
*3. **PROCESS_NUMEVENTS_CONF** \n
*=> Allowed values : 1 to 255 \n
*=> Eg. Declaration : #define PROCESS_NUMEVENTS_CONF 64 \n
*=> Default value : 32 \n
*=> Usage Description : This macro declares number of events that can be queued \n
* @}
*/

// platform.h header provides platform dependent and platform independent datatypes
// NULL (NULL pointer) & data_address_t is platform dependent 
#include "../../platform.h"
#ifndef DISABLE_OS

/**
* \name MACROS : Process Debug Levels
* \brief Define different levels for process debugging
* @{
*/
#define PROCESS_DEBUG_NONE 0
#define PROCESS_DEBUG_USER 1
#define PROCESS_DEBUG_LEVEL1 2
#define PROCESS_DEBUG_LEVEL2 3
#define PROCESS_DEBUG_LEVEL3 4
///@}
 
/**
* \name Typedefs
* \brief Define typedefs for process header
* @{
*/
typedef program_address_t process_position_t; 	///< typedef for process position from where process should resume
typedef mos_uint8_t process_event_t; 			///< typedef for process event
typedef void * process_data_t;  				///< typedef for process data
typedef mos_uint8_t process_num_events_t;		///< typedef for variables keeping the count of events 
///@}

/**
* \name MACROS : Process function return types
* @{
*/
/// return value indicating that an operation was successful
#define PROCESS_ERR_OK 0
/// return value indicating that event queue is full
#define PROCESS_ERR_FULL 1
///@}

/**
* \name MACROS : Standard Process Events
* @{
*/
// most events are never used Need to review this in future
#define PROCESS_EVENT_NONE                  0x80
#define PROCESS_EVENT_INIT                  0x81
#define PROCESS_EVENT_POLL                  0x82
#define PROCESS_EVENT_EXIT                  0x83
#define PROCESS_EVENT_SERVICE_REMOVED       0x84
#define PROCESS_EVENT_CONTINUE              0x85
#define PROCESS_EVENT_MSG                   0x86
#define PROCESS_EVENT_EXITED                0x87
#define PROCESS_EVENT_TIMER                 0x88
#define PROCESS_EVENT_COM                   0x89
#define PROCESS_EVENT_MAX                   0x8a
#define PROCESS_EVENT_SUBPROCESS_CALL       0x8b
#define PROCESS_EVENT_SUBPROCESS_RETURN     0x8c
#define PROCESS_EVENT_PTIMER                0x8d
#define PROCESS_BROADCAST                   0x00
///@}

/**
* \name MACROS : PROCESS THREAD (PT) return values
* @{
*/
#define PT_WAITING 0  // This state is never used so far hence commented
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3
///@}

/**
* \name MACROS : PROCESS THREAD (PT) OS Core MACROS
* @{
*/
/// This macro starts the execution part of process thread (PT)
#define PROCESS_BEGIN()                     \
{                                           \
    mos_uint8_t PT_FLAG_VAR_ = 1;           \
    switch (process_current->position)      \
    {   case 0:                             
    
/// This macro ends the execution part of process thread (PT)   
#define PROCESS_END()                       \
    }                                       \
    process_current->position = 0;          \
    return PT_ENDED;                        \
}

/// This macro returns control from a process thread to OS
#define PROCESS_WAIT_EVENT()                \
do {                                        \
    PT_FLAG_VAR_ = 0;                       \
    process_current->position = __LINE__;   \
    case __LINE__ :                         \
          if( PT_FLAG_VAR_ == 0 )           \
           {                                \
            return PT_YIELDED;              \
           }                                \
} while(0)

/// This macro is same as PROCESS_WAIT_EVENT except that it also checks a condition   
#define PROCESS_WAIT_EVENT_UNTIL(condition) \
do {                                        \
    PT_FLAG_VAR_ = 0;                       \
    process_current->position = __LINE__;   \
    case __LINE__ :                         \
        if((PT_FLAG_VAR_ == 0) || !(condition))  \
        {                                   \
            return PT_YIELDED;              \
        }                                   \
} while(0)

/// This macro exits the current process before it reaches PROCESS_END
#define PROCESS_EXIT()                      \
do {                                        \
        process_current->position = 0;      \
        return PT_EXITED;                   \
} while(0)

///If there are more than one user auto start processes, this macro is used to pause current process untill all other processes are started after that this process start again from this point only
#define PROCESS_PAUSE()                     \
do {                                        \
        process_post(PROCESS_CURRENT(), PROCESS_EVENT_CONTINUE, NULL);  \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);         \
}while(0)
///@}


/**
* \name MACROS : PROCESS THREAD (PT) OS other important MACROS
* @{
*/
#ifdef PROCESS_HAS_STRING_NAMES_CONF
    #define PROCESS_HAS_STRING_NAMES
#endif      
    
/// This MACRO defines process thread function                                     
#define PROCESS_THREAD(name)                                                    \
static mos_uint8_t process_thread_##name(process_event_t ev)
                       
/// This macro is used to declare the name of a process typically in a header file
#define PROCESS_GLOBAL(name) extern struct process name

/// This macro declares a Process thread function and a process structure variable.
/// If PROCESS_HAS_STRING_NAMES is defined string name of process is store in data structure variable else not.
#ifdef PROCESS_HAS_STRING_NAMES
    #define PROCESS(name, strname)                        \
        PROCESS_THREAD(name);                             \
        struct process name = { NULL, strname,            \
                          process_thread_##name }
#else
    #define PROCESS(name, strname)                        \
        PROCESS_THREAD(name);                             \
        struct process name = { NULL,                     \
                          process_thread_##name }
#endif        

/// Define total allowed events that can be queued at a time.
/// To change default value PROCESS_NUMEVENTS_CONF should be defined
/// either in board.h default files or define in config.h by user 
#ifdef PROCESS_NUMEVENTS_CONF
    #define PROCESS_NUMEVENTS PROCESS_NUMEVENTS_CONF
#else 
    #define PROCESS_NUMEVENTS 32
#endif
///@}

struct process {
  struct process *next; 									///< points to next process in the queue
  #ifdef PROCESS_HAS_STRING_NAMES
    const char *name; 										///< includes string nname of process if enabled   
  #endif
  mos_uint8_t (* thread)(process_event_t); 					///< Process thread function pointer
  process_position_t position; 								///< Process position variable from where process should resume
  process_data_t data, data2; 								///< Process data variables
  mos_uint8_t state, needspoll;								///< Process state and needspoll variable
  
  struct process *ptimer_next;								///< Pointer for next ptimer queued by Ptimer process
  clock_second_t expiry_seconds;							///< Ptimer varible for seconds expiry
  clock_millisecond_t expiry_milliseconds;					///< Ptimer variable for milliseconds expiry
  mos_uint8_t ptimer_state;									///< Ptimer variable for its active status
};

/**
* \name Process Functions
* @{
*/
 
/** 
* \brief	Declare function to init process module
*/
void process_init(void);

/** 
* \brief		Declare function to start a process
* \param p   	A pointer to the process
* \param data 	Data to pass to the process of type process_data_t
* \return void
*
*	This function starts a process whose pointer is provided by input parameter **p**.
*	Optionally we can provided initial data to the process by input parameter **data**.
*	If no data has to be passed **NULL** can be passed as **data** parameter.
*/
void process_start(struct process *p, process_data_t data);


/** 
* \brief		Declare function to exit any process whether current or any other which is running
* \param p   	A pointer to the process
* \return void	
*
*	This function exits a process whose pointer is provided by input parameter **p**.
*	This process could be current or any other running process.
*/
void process_exit(struct process * p);


/** 
* \brief		Declare function which posts a asynchronous event.
* \param p   	A pointer to the process
* \param data 	Data to pass to the process of type process_data_t
* \return int	Returns PROCESS_ERR_OK on success else PROCESS_ERR_FULL
*
*	This function post a asynchronous event of a process whose pointer is provided by input parameter **p**.
*	Asynchronous event means, OS will not service this event immediately but somtime in future.
*	Optionally we can provide initial data to the process by input parameter **data**.
*	If no data has to be passed **NULL** can be passed as **data** parameter.
*/
int process_post(struct process * p, process_event_t ev, process_data_t data);

/** 
* \brief		Declare a function to poll a process
* \param p   	A pointer to the process
* \return void	
*
*	This function polls a process whose pointer is provided by input parameter **p**.
*	Polling is method to service high priority processes.
*	In every run loop, OS serivce a Polled event and a normal event.
*	By this way polled event will have higher chances of getting serviced.
*	However it still can't gaurantee real time service.
*/
void process_poll(struct process *p);

/**
* \brief Run the system once - call poll handlers and process one event.
* \return The number of events that are currently waiting in the event queue.
*
*	This function should be called repeatedly from the main() program
*	to actually run the Contiki system. It calls the necessary poll
*	handlers, and processes one event. The function returns the number
*	of events that are waiting in the event queue so that the caller
*	may choose to put the CPU to sleep when there are no pending
*	events.
*/
int process_run(void);

/** 
* \brief		Declare function to check if a process is running or not
* \param p   	A pointer to the process
* \return int	Returns 1 for success else 0
*/
int process_is_running(struct process *p);

/// Function to autostart processes
void autostart_start(struct process * const processes[]);
///@}

/**
* \name MACROS : PROCESS THREAD (PT) OS miscellaneous MACROS
* @{
*/

/// Declare PROCESS_NAME_STRING(process) macro, used to print process string name, if enabled
#ifdef PROCESS_HAS_STRING_NAMES										
	#define PROCESS_NAME_STRING(process) (process)->name    
#else
    #define PROCESS_NAME_STRING(process) ""    
#endif

/// This macro is used to get the pointer to current process
#define PROCESS_CURRENT() process_current

/// Declare process_current structure visible to all other files 
extern struct process *process_current;

/// Declare process_list structure visible to all other files
extern struct process * process_list;

/// This macro is used to get the pointer to process_list
#define PROCESS_LIST() process_list

/// Declare AUTOSTART_PROCESSES used to auto start selected processes after boot
#define AUTOSTART_PROCESSES(...)                    \
struct process * autostart_processes[] = {__VA_ARGS__, NULL}

///@}

#endif
#endif

