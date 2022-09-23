#ifndef DEBUG_H
#define DEBUG_H

#include "../../platform.h"
#ifndef DISABLE_OS
	#include "../sys/process.h"
#endif

#define DEBUG_NONE 0
#define DEBUG_TEXT_ONLY 1
#define DEBUG_TEXT_WITH_TIME 2

#ifdef DEBUG_LEVEL_CONF
    #define DEBUG_LEVEL DEBUG_LEVEL_CONF
#else
    #define DEBUG_LEVEL DEBUG_NONE
#endif

#include "debug_config.h"  

#if DEBUG_LEVEL > DEBUG_NONE
    #define DEBUG_PRINT(msg) debug_print(msg)
    #define DEBUG_PRINT_LN(msg) debug_print_ln(msg)
	#ifndef DISABLE_OS
		#define DEBUG_PRINT_TIME() debug_print_time()
		#define DEBUG_PRINT_TIME_LN() debug_print_time_ln()
	#else
		#define DEBUG_PRINT_TIME() debug_print(msg)
		#define DEBUG_PRINT_TIME_LN() debug_print_ln(msg)
	#endif
#else
    #define DEBUG_PRINT(msg)
    #define DEBUG_PRINT_LN(msg)
    #define DEBUG_PRINT_TIME()
    #define DEBUG_PRINT_TIME_LN()   
#endif

#if DEBUG_LEVEL == DEBUG_NONE
    #define DEBUG_USER(msg)
    #define DEBUG_USER_LN(msg)
#elif DEBUG_LEVEL == DEBUG_TEXT_ONLY
	#ifndef DISABLE_OS
		#define DEBUG_USER(msg) debug_msg("",PROCESS_CURRENT(),msg)
		#define DEBUG_USER_LN(msg) debug_msg_ln("",PROCESS_CURRENT(), msg)
	#else
		#define DEBUG_USER(msg) DEBUG_PRINT(msg)
		#define DEBUG_USER_LN(msg) DEBUG_PRINT_LN(msg)
	#endif
#elif DEBUG_LEVEL == DEBUG_TEXT_WITH_TIME
	#ifndef DISABLE_OS
		#define DEBUG_USER(msg) debug_msg_with_time("",PROCESS_CURRENT(),msg)
		#define DEBUG_USER_LN(msg) debug_msg_with_time_ln("",PROCESS_CURRENT(),msg)
	#else
		#define DEBUG_USER(msg) DEBUG_PRINT(msg)
		#define DEBUG_USER_LN(msg) DEBUG_PRINT_LN(msg)
	#endif
#endif

#if (DEBUG_LEVEL == DEBUG_NONE) || defined(DISABLE_OS)
    #define DEBUG_MSG(initial_msg,process_name,msg)
    #define DEBUG_MSG_LN(initial_msg,process_name,msg)
#elif DEBUG_LEVEL == DEBUG_TEXT_ONLY
    #define DEBUG_MSG(initial_msg,process_name,msg) debug_msg(initial_msg,process_name,msg)
    #define DEBUG_MSG_LN(initial_msg,process_name,msg) debug_msg_ln(initial_msg,process_name,msg)
#elif DEBUG_LEVEL == DEBUG_TEXT_WITH_TIME
    #define DEBUG_MSG(initial_msg,process_name,msg) debug_msg_with_time(initial_msg,process_name,msg)
    #define DEBUG_MSG_LN(initial_msg,process_name,msg) debug_msg_with_time_ln(initial_msg,process_name,msg)
#endif

#if DEBUG_LEVEL == DEBUG_NONE
    #define DEBUG_PRINT_INTEGER(msg)
    #define DEBUG_PRINT_INTEGER_LN(msg)
    #define DEBUG_PRINT_INTEGER_LONG(msg)
    #define DEBUG_PRINT_INTEGER_LONG_LN(msg)
    #define DEBUG_PRINT_FLOAT(msg)
    #define DEBUG_PRINT_FLOAT_LN(msg) 
#elif DEBUG_LEVEL >= DEBUG_TEXT_ONLY
    #define DEBUG_PRINT_INTEGER(msg) debug_print_integer(msg)
    #define DEBUG_PRINT_INTEGER_LN(msg) debug_print_integer_ln(msg)
    #define DEBUG_PRINT_INTEGER_LONG(msg) debug_print_integer_long(msg)
    #define DEBUG_PRINT_INTEGER_LONG_LN(msg) debug_print_integer_long_ln(msg)
    #define DEBUG_PRINT_FLOAT(msg) debug_print_float(msg)
    #define DEBUG_PRINT_FLOAT_LN(msg) debug_print_float_ln(msg)
#endif

#if DEBUG_LEVEL > DEBUG_NONE
    void debug_print(char * data);
    void debug_print_ln(char * data);
	#ifndef DISABLE_OS
		void print_time(void);
		void debug_print_time(void);
		void debug_print_time_ln(void);
		void debug_msg(char* initial_text, struct process * process_name,  char * data);
		void debug_msg_ln(char* initial_text,  struct process * process_name,  char * data);
		void debug_msg_with_time(char* initial_text,  struct process * process_name,  char * data);
		void debug_msg_with_time_ln(char* initial_text,  struct process * process_name,  char * data);
	#endif
    void debug_print_integer(int data);
    void debug_print_integer_ln(int data);
    void debug_print_integer_long(long int data);
    void debug_print_integer_long_ln(long int data);
    void debug_print_float(float data);
    void debug_print_float_ln(float data);
        
#endif

#endif
