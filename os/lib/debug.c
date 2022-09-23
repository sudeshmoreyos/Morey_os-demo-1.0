#include "debug.h"
#if DEBUG_LEVEL > DEBUG_NONE
#ifndef DISABLE_OS
	#include "../sys/clock.h"
#endif

#include "../util/stdlib.h"

void debug_print(char * data)
{
    PRINT(data);
}

void debug_print_ln(char * data)
{
    PRINT_LN(data);
}                

#ifndef DISABLE_OS
        
	void print_time(void)
	{
		clock_second_t seconds ;
		clock_millisecond_t milliseconds;
		float time;
		char s[10];
					
		get_clock_time( &seconds , &milliseconds ); 
		time = (float)seconds;
		time = time + ((float)milliseconds)/1000;            
		ftoa(time,3,s);
		PRINT(s);        
	}
			
	void debug_print_time(void)
	{
		PRINT("[ ");            
		print_time();
		PRINT(" ] ");                
	}

	void debug_print_time_ln(void)
	{
		PRINT("[ ");            
		print_time();
		PRINT_LN(" ] ");
	}
			
	void debug_msg(char* initial_text,  struct process * process_name,  char * data)
	{
		(void)process_name;
		PRINT(initial_text);
		#ifdef PROCESS_HAS_STRING_NAMES_CONF
			if(process_name != NULL)
			{
				PRINT("{");
				PRINT((char *)PROCESS_NAME_STRING(process_name));
				PRINT("} ");
			}
		#endif          
		PRINT(data);        
	}

	void debug_msg_ln(char* initial_text,  struct process * process_name,  char * data)
	{
		(void) process_name;
		PRINT(initial_text);
		#ifdef PROCESS_HAS_STRING_NAMES_CONF
			if(process_name != NULL)
			{
				PRINT("{");
				PRINT((char *)PROCESS_NAME_STRING(process_name));
				PRINT("} ");
			}
		#endif          
		PRINT_LN(data);         
	}

	void debug_msg_with_time(char* initial_text,  struct process * process_name,  char * data)
	{
	  debug_print_time();
	  debug_msg(initial_text, process_name, data);       
	}

	void debug_msg_with_time_ln(char* initial_text,  struct process * process_name,  char * data)
	{
	  debug_print_time();
	  debug_msg_ln(initial_text, process_name, data);         
	}

#endif

void debug_print_integer(int data)
{
  char s[10];
  
  itoa(data,s);
  PRINT(s);
}

void debug_print_integer_ln(int data)
{
  char s[10];
  
  itoa(data,s);
  PRINT_LN(s);
}

void debug_print_integer_long(long int data)
{
  char s[20];
  
  ltoa(data,s);
  PRINT(s);
}

void debug_print_integer_long_ln(long int data)
{
  char s[20];
  
  ltoa(data,s);
  PRINT_LN(s);
}

void debug_print_float(float data)
{
  char s[20];
  
  ftoa(data,3,s);
  PRINT(s);
}

void debug_print_float_ln(float data)
{
  char s[20];
  
  ftoa(data,3,s);
  PRINT_LN(s);
}

#endif
