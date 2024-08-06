#ifndef DEBUG_H
#define DEBUG_H

#include "../../platform.h"
#ifndef DISABLE_OS
	#include "../sys/task.h"
#endif

#define DEBUG_NONE 1
#define DEBUG_TEXT_ONLY 2
#define DEBUG_TEXT_WITH_TIME 3

#ifdef DEBUG_LEVEL_CONF	
	#if !(DEBUG_LEVEL_CONF == DEBUG_NONE || DEBUG_LEVEL_CONF == DEBUG_TEXT_ONLY || DEBUG_LEVEL_CONF == DEBUG_TEXT_WITH_TIME)
		#error Invalid parameter for DEBUG_LEVEL_CONF
	#endif
	
    #define DEBUG_LEVEL DEBUG_LEVEL_CONF
#else
    #define DEBUG_LEVEL DEBUG_NONE
#endif

#include "debug_config.h"  

#if DEBUG_LEVEL > DEBUG_NONE
    #define DEBUG_PRINT(msg) 			PRINT(msg) 					
    #define DEBUG_PRINT_LN(msg) 		PRINT_LN(msg) 				
	
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
		#define DEBUG_CONST_PRINT(msg) 		CONST_PRINT(msg) 		
		#define DEBUG_CONST_PRINT_LN(msg) 	CONST_PRINT_LN(msg)		
	#else
		#define DEBUG_CONST_PRINT(msg) 		DEBUG_PRINT(msg) 		
		#define DEBUG_CONST_PRINT_LN(msg) 	DEBUG_PRINT_LN(msg)		
	#endif
	
	#ifndef DISABLE_OS
		#define DEBUG_PRINT_TIME() 		debug_print_time()
		#define DEBUG_PRINT_TIME_LN() 	debug_print_time_ln()
	#else
		#define DEBUG_PRINT_TIME()
		#define DEBUG_PRINT_TIME_LN()
	#endif
#else
    #define DEBUG_PRINT(msg)
    #define DEBUG_PRINT_LN(msg)
	#define DEBUG_CONST_PRINT(msg)
    #define DEBUG_CONST_PRINT_LN(msg)	
	#define DEBUG_PRINT_TIME()
    #define DEBUG_PRINT_TIME_LN()
#endif

#if DEBUG_LEVEL == DEBUG_NONE

    #define DEBUG_USER(msg)
    #define DEBUG_USER_LN(msg)
	#define DEBUG_USER_CONST(msg)
	#define DEBUG_USER_CONST_LN(msg)
	
#elif DEBUG_LEVEL == DEBUG_TEXT_ONLY

	#ifndef DISABLE_OS
		#define DEBUG_USER(msg) 				debug_msg("",TASK_CURRENT(),msg)
		#define DEBUG_USER_LN(msg) 				debug_msg_ln("",TASK_CURRENT(), msg)
			
		#ifdef PLATFORM_SUPPORT_CONST_PRINT
			#define DEBUG_USER_CONST(msg) 		{ debug_msg("",TASK_CURRENT(),""); DEBUG_CONST_PRINT(msg); }
			#define DEBUG_USER_CONST_LN(msg) 	{ debug_msg("",TASK_CURRENT(),""); DEBUG_CONST_PRINT_LN(msg); }
		#else
			#define DEBUG_USER_CONST(msg) 		debug_msg("",TASK_CURRENT(),msg)
			#define DEBUG_USER_CONST_LN(msg) 	debug_msg_ln("",TASK_CURRENT(), msg)
		#endif
	#else
		#define DEBUG_USER(msg) 				DEBUG_PRINT(msg)
		#define DEBUG_USER_LN(msg) 				DEBUG_PRINT_LN(msg)
		#define DEBUG_USER_CONST(msg) 			DEBUG_USER(msg)
		#define DEBUG_USER_CONST_LN(msg) 		DEBUG_USER_LN(msg)
	#endif
	
#elif DEBUG_LEVEL == DEBUG_TEXT_WITH_TIME
	#ifndef DISABLE_OS
		#define DEBUG_USER(msg) 				debug_msg_with_time("",TASK_CURRENT(),msg)
		#define DEBUG_USER_LN(msg) 				debug_msg_with_time_ln("",TASK_CURRENT(),msg)
		
		#ifdef PLATFORM_SUPPORT_CONST_PRINT
			#define DEBUG_USER_CONST(msg) 		{ debug_msg_with_time("",TASK_CURRENT(),""); DEBUG_CONST_PRINT(msg); }
			#define DEBUG_USER_CONST_LN(msg) 	{ debug_msg_with_time("",TASK_CURRENT(),""); DEBUG_CONST_PRINT_LN(msg); } 
		#else
			#define DEBUG_USER_CONST(msg) 		debug_msg_with_time("",TASK_CURRENT(),msg)
			#define DEBUG_USER_CONST_LN(msg) 	debug_msg_with_time_ln("",TASK_CURRENT(), msg)
		#endif
	#else
		#define DEBUG_USER(msg) 				DEBUG_PRINT(msg)
		#define DEBUG_USER_LN(msg) 				DEBUG_PRINT_LN(msg)
		#define DEBUG_USER_CONST(msg) 			DEBUG_USER(msg)
		#define DEBUG_USER_CONST_LN(msg) 		DEBUG_USER_LN(msg)
	#endif
#endif

#ifndef DISABLE_OS

	#if DEBUG_LEVEL == DEBUG_NONE
		
		#define DEBUG_MSG(initial_msg,task_name,msg)
		#define DEBUG_MSG_LN(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST1(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST1_LN(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST2(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST2_LN(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST3(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST3_LN(initial_msg,task_name,msg)
		
	#elif DEBUG_LEVEL == DEBUG_TEXT_ONLY

		#define DEBUG_MSG(initial_msg,task_name,msg) 			debug_msg(initial_msg,task_name,msg)
		#define DEBUG_MSG_LN(initial_msg,task_name,msg) 		debug_msg_ln(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST1(initial_msg,task_name,msg)		{ DEBUG_CONST_PRINT(initial_msg); debug_msg("",task_name,msg); }
		#define DEBUG_MSG_CONST1_LN(initial_msg,task_name,msg)	{ DEBUG_CONST_PRINT(initial_msg); debug_msg_ln("",task_name,msg); }
		#define DEBUG_MSG_CONST2(initial_msg,task_name,msg)		{ debug_msg(initial_msg,task_name,""); DEBUG_CONST_PRINT(msg); }
		#define DEBUG_MSG_CONST2_LN(initial_msg,task_name,msg)	{ debug_msg(initial_msg,task_name,""); DEBUG_CONST_PRINT_LN(msg); }
		#define DEBUG_MSG_CONST3(initial_msg,task_name,msg)		{ DEBUG_CONST_PRINT(initial_msg); debug_msg("",task_name,""); DEBUG_CONST_PRINT(msg); }
		#define DEBUG_MSG_CONST3_LN(initial_msg,task_name,msg)	{ DEBUG_CONST_PRINT(initial_msg); debug_msg("",task_name,""); DEBUG_CONST_PRINT_LN(msg); }
		
	#elif DEBUG_LEVEL == DEBUG_TEXT_WITH_TIME

		#define DEBUG_MSG(initial_msg,task_name,msg) 			debug_msg_with_time(initial_msg,task_name,msg)
		#define DEBUG_MSG_LN(initial_msg,task_name,msg) 		debug_msg_with_time_ln(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST1(initial_msg,task_name,msg)		{ debug_print_time(); DEBUG_CONST_PRINT(initial_msg); debug_msg("",task_name,msg); }
		#define DEBUG_MSG_CONST1_LN(initial_msg,task_name,msg)	{ debug_print_time(); DEBUG_CONST_PRINT(initial_msg); debug_msg_ln("",task_name,msg); }
		#define DEBUG_MSG_CONST2(initial_msg,task_name,msg)		{ debug_print_time(); debug_msg(initial_msg,task_name,""); DEBUG_CONST_PRINT(msg); }
		#define DEBUG_MSG_CONST2_LN(initial_msg,task_name,msg)	{ debug_print_time(); debug_msg(initial_msg,task_name,""); DEBUG_CONST_PRINT_LN(msg); }
		#define DEBUG_MSG_CONST3(initial_msg,task_name,msg)		{ debug_print_time(); DEBUG_CONST_PRINT(initial_msg); debug_msg("",task_name,""); DEBUG_CONST_PRINT(msg); }
		#define DEBUG_MSG_CONST3_LN(initial_msg,task_name,msg)	{ debug_print_time(); DEBUG_CONST_PRINT(initial_msg); debug_msg("",task_name,""); DEBUG_CONST_PRINT_LN(msg); }	

	#endif
	
#else
	
	#if DEBUG_LEVEL == DEBUG_NONE
		
		#define DEBUG_MSG(initial_msg,task_name,msg)
		#define DEBUG_MSG_LN(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST1(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST1_LN(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST2(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST2_LN(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST3(initial_msg,task_name,msg)
		#define DEBUG_MSG_CONST3_LN(initial_msg,task_name,msg)
		
	#elif (DEBUG_LEVEL == DEBUG_TEXT_ONLY) || (DEBUG_LEVEL == DEBUG_TEXT_WITH_TIME)

		#define DEBUG_MSG(initial_msg,task_name,msg) 			debug_msg_no_os(initial_msg,msg)
		#define DEBUG_MSG_LN(initial_msg,task_name,msg) 		debug_msg_no_os_ln(initial_msg,msg)
		#define DEBUG_MSG_CONST1(initial_msg,task_name,msg)		{ DEBUG_CONST_PRINT(initial_msg); debug_msg_no_os("",msg); }
		#define DEBUG_MSG_CONST1_LN(initial_msg,task_name,msg)	{ DEBUG_CONST_PRINT(initial_msg); debug_msg_no_os_ln("",msg) }
		#define DEBUG_MSG_CONST2(initial_msg,task_name,msg)		{ debug_msg_no_os(initial_msg,""); DEBUG_CONST_PRINT(msg); }
		#define DEBUG_MSG_CONST2_LN(initial_msg,task_name,msg)	{ debug_msg_no_os(initial_msg,""); DEBUG_CONST_PRINT_LN(msg); }
		#define DEBUG_MSG_CONST3(initial_msg,task_name,msg)		{ DEBUG_CONST_PRINT(initial_msg); DEBUG_CONST_PRINT(msg); }
		#define DEBUG_MSG_CONST3_LN(initial_msg,task_name,msg)	{ DEBUG_CONST_PRINT(initial_msg); DEBUG_CONST_PRINT_LN(msg); }
	#endif
#endif

#if DEBUG_LEVEL == DEBUG_NONE
    #define DEBUG_PRINT_INTEGER(msg)
    #define DEBUG_PRINT_INTEGER_LN(msg)
    #define DEBUG_PRINT_INTEGER_LONG(msg)
    #define DEBUG_PRINT_INTEGER_LONG_LN(msg)
    #define DEBUG_PRINT_FLOAT(msg)
    #define DEBUG_PRINT_FLOAT_LN(msg)
	#define DEBUG_PRINT_HEX(msg,len)
    #define DEBUG_PRINT_HEX_LN(msg,len)
#elif DEBUG_LEVEL >= DEBUG_TEXT_ONLY
    #define DEBUG_PRINT_INTEGER(msg) 			debug_print_integer(msg)
    #define DEBUG_PRINT_INTEGER_LN(msg) 		debug_print_integer_ln(msg)
    #define DEBUG_PRINT_INTEGER_LONG(msg) 		debug_print_integer_long(msg)
    #define DEBUG_PRINT_INTEGER_LONG_LN(msg) 	debug_print_integer_long_ln(msg)
    #define DEBUG_PRINT_FLOAT(msg) 				debug_print_float(msg)
    #define DEBUG_PRINT_FLOAT_LN(msg) 			debug_print_float_ln(msg)
	#define DEBUG_PRINT_HEX(msg,len)			debug_print_hex(msg,len)
    #define DEBUG_PRINT_HEX_LN(msg,len)			debug_print_hex_ln(msg,len)
#endif

#if DEBUG_LEVEL > DEBUG_NONE
	#ifndef DISABLE_OS
		void print_time(void);
		void debug_print_time(void);
		void debug_print_time_ln(void);
		void debug_msg(char* initial_text, struct task_struct_os * task_name,  char * data);
		void debug_msg_ln(char* initial_text,  struct task_struct_os * task_name,  char * data);
		void debug_msg_with_time(char* initial_text,  struct task_struct_os * task_name,  char * data);
		void debug_msg_with_time_ln(char* initial_text,  struct task_struct_os * task_name,  char * data);
	#else
		void debug_msg_no_os(char* initial_text, char * data);
		void debug_msg_no_os_ln(char* initial_text, char * data);
	#endif
    void debug_print_integer(int data);
    void debug_print_integer_ln(int data);
    void debug_print_integer_long(long int data);
    void debug_print_integer_long_ln(long int data);
    void debug_print_float(float data);
    void debug_print_float_ln(float data);
	void debug_print_hex(mos_uint8_t * data, mos_uint16_t len);
	void debug_print_hex_ln(mos_uint8_t * data, mos_uint16_t len);       
#endif

#endif
