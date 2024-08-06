#include "debug.h"
#if DEBUG_LEVEL > DEBUG_NONE
#ifndef DISABLE_OS
	#include "../sys/clock.h"
#endif

#include "../util/stdlib.h"           

#ifndef DISABLE_OS
        
	void print_time(void)
	{
		clock_second_t seconds;
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
		CONST_PRINT("[ ");            
		print_time();
		CONST_PRINT(" ] ");
	}

	void debug_print_time_ln(void)
	{
		debug_print_time();
		PRINT_LN("");
	}
			
	void debug_msg(char* initial_text,  struct task_struct_os * task_name,  char * data)
	{
		(void)task_name;
		PRINT(initial_text);
		#ifdef TASK_HAS_STRING_NAMES_CONF
			if(task_name != NULL)
			{
				PRINT("{");
				PRINT((char *)TASK_NAME_STRING(task_name));
				PRINT("} ");
			}
		#endif          
		PRINT(data);        
	}

	void debug_msg_ln(char* initial_text,  struct task_struct_os * task_name,  char * data)
	{
		debug_msg(initial_text, task_name, data);         
		PRINT_LN("");         
	}

	void debug_msg_with_time(char* initial_text,  struct task_struct_os * task_name,  char * data)
	{
	  debug_print_time();
	  debug_msg(initial_text, task_name, data);       
	}

	void debug_msg_with_time_ln(char* initial_text,  struct task_struct_os * task_name,  char * data)
	{
	  debug_print_time();
	  debug_msg_ln(initial_text, task_name, data);         
	}

#else
	void debug_msg_no_os(char* initial_text, char * data)
	{
		PRINT(initial_text);         
		PRINT(data);        
	}

	void debug_msg_no_os_ln(char* initial_text, char * data)
	{
		debug_msg_no_os(initial_text, data);         
		PRINT_LN("");         
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

void debug_print_hex(mos_uint8_t * data, mos_uint16_t len)
{
	char s[3];
	mos_uint8_t low, high;
	
	while(len)
	{
		high = *data >> 4;
		low = *data & 0xF;
		
		if( high >= 0 && high <= 9)
			s[0] = (char)(high + '0');
		else if( high >= 10 && high <= 15)
			s[0] = (char)(high + 'A' - 10);
		
		if( low >= 0 && low <= 9)
			s[1] = (char)(low + '0');
		else if( low >= 10 && low <= 15)
			s[1] = (char)(low + 'A' - 10);
		
		s[2] = '\0';
		PRINT(s);
		data++;
		len--;
	}
}

void debug_print_hex_ln(mos_uint8_t * data, mos_uint16_t len)
{
	debug_print_hex(data,len);
	PRINT_LN("");
}

#endif
