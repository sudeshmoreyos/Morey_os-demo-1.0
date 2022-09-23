/** \addtogroup os
* @{ \addtogroup source
* @{ \defgroup ptimer_c
* @{ \addtogroup ptimer_c 
* @{
* \brief Documentaion of ptimer.c source file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
* This is source file for ptimer header
* @}
*/

#ifndef DISABLE_OS

#include "ptimer.h"
#include "clock.h"

#ifdef PTIMER_DEBUG_LEVEL_CONF
    #define PTIMER_DEBUG_LEVEL PTIMER_DEBUG_LEVEL_CONF
#else
    #define PTIMER_DEBUG_LEVEL PTIMER_DEBUG_NONE
#endif

#if PTIMER_DEBUG_LEVEL > PTIMER_DEBUG_NONE
    #include "../lib/debug.h"
#endif

#define PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name) DEBUG_MSG("<DEBUG PTIMER> ",process_name,msg)
#define PTIMER_DEBUG_MSG_TEMP(msg,process_name) DEBUG_MSG_LN("<DEBUG PTIMER> ",process_name,msg)
#define PTIMER_DEBUG_MSG_USER_TEMP(msg,process_name) DEBUG_MSG_LN("<DEBUG PTIMER USER> ",process_name,msg)              

#if PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_NONE
    #define PTIMER_DEBUG_MSG_USER(msg,process_name)
    #define PTIMER_DEBUG_MSG1(msg,process_name)
    #define PTIMER_DEBUG_MSG2(msg,process_name)
    #define PTIMER_DEBUG_MSG3(msg,process_name)
    #define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,process_name)
    #define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,process_name)
    #define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,process_name)
    
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_USER
    #define PTIMER_DEBUG_MSG_USER(msg,process_name) PTIMER_DEBUG_MSG_USER_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG1(msg,process_name)
    #define PTIMER_DEBUG_MSG2(msg,process_name)
    #define PTIMER_DEBUG_MSG3(msg,process_name)
    #define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,process_name)
    #define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,process_name)
    #define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,process_name) 
    
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_LEVEL1
    #define PTIMER_DEBUG_MSG_USER(msg,process_name_name)
    #define PTIMER_DEBUG_MSG1(msg,process_name) PTIMER_DEBUG_MSG_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG2(msg,process_name)
    #define PTIMER_DEBUG_MSG3(msg,process_name)
    #define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,process_name) PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,process_name)
    #define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,process_name)    
     
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_LEVEL2
    #define PTIMER_DEBUG_MSG_USER(msg,process_name_name)
    #define PTIMER_DEBUG_MSG1(msg,process_name) PTIMER_DEBUG_MSG_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG2(msg,process_name) PTIMER_DEBUG_MSG_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG3(msg,process_name)
    #define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,process_name) PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,process_name) PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,process_name) 
    
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_LEVEL3
    #define PTIMER_DEBUG_MSG_USER(msg,process_name)
    #define PTIMER_DEBUG_MSG1(msg,process_name) PTIMER_DEBUG_MSG_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG2(msg,process_name) PTIMER_DEBUG_MSG_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG3(msg,process_name) PTIMER_DEBUG_MSG_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,process_name) PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,process_name) PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name)
    #define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,process_name) PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,process_name)
               
#endif

struct process * ptimer_list = NULL;
uint8_t ptimer_poll_flag = 0;

void ptimer_init(void)
{
    ptimer_list = NULL;
    PTIMER_DEBUG_MSG3("PTIMER Process Initiated",NULL);
}

static void schedule_ptimer(struct process * p);
static mos_uint8_t compare_ptimer_expiry(struct process *p_compare , struct process *p_reference );

#ifdef PLATFORM_SUPPORT_TICKLESS
	static void schedule_tickless_timer_process(struct process * p);
#endif

PROCESS(ptimer_process, "PTIMER PROCESS");

//---------------------------------------------------------------------------//
PROCESS_THREAD(ptimer_process)
{
  #if PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL3
    #ifdef PROCESS_HAS_STRING_NAMES
        //struct process * p1;
    #endif
  #endif
  
  PROCESS_BEGIN();

  while(1) {
    
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL);
    PTIMER_DEBUG_MSG3("PTIMER Called",&ptimer_process);
    
    while(ptimer_list != NULL)
    {
        // checking ptimer_state is redundant because if a ptimer is
        // active means it will be on ptimer_list linked list else not
		if( ptimer_expired(ptimer_list) )
		{                
			if(process_post(ptimer_list, PROCESS_EVENT_PTIMER, NULL) == PROCESS_ERR_OK)
			{                    
				PTIMER_DEBUG_MSG3("Timer event post success", ptimer_list);
				ptimer_list = ptimer_list->ptimer_next;
			}
			else
			{
				PTIMER_DEBUG_MSG3("Timer event post failure", ptimer_list);
				ptimer_request_poll();                    
				break;
			}
		}            
		else
			break;              
    }   
    ptimer_poll_flag = 0;
	
	#ifdef PLATFORM_SUPPORT_TICKLESS
		if(ptimer_list != NULL)
		{
			PTIMER_DEBUG_MSG3("check for tickless schedule", ptimer_list);
			schedule_tickless_timer_process(ptimer_list);
		}
	#endif
   
    PTIMER_DEBUG_MSG3_NO_NEWLINE("ptimer list = ", NULL);
    #if ( (PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL3) && (DEBUG_LEVEL > DEBUG_NONE) )
        #ifdef PROCESS_HAS_STRING_NAMES
		{
            struct process * p1;
			for( p1 = ptimer_list ; p1 != NULL ; p1 = p1->ptimer_next)
                debug_msg("",p1,"");
				debug_print_ln("");
		}
        #else
            debug_print_ln("PROCESS_HAS_STRING_NAMES_CONF macro not defined");
        #endif
    #endif         
  }

  PROCESS_END();
}

//---------------------------------------------------------------------------//
void ptimer_request_poll(void)
{  
    if( ptimer_poll_flag == 0 )
	{
		process_poll(&ptimer_process);
		ptimer_poll_flag = 1;
		PTIMER_DEBUG_MSG3("Ptimer poll requested",NULL);
	}
}

//---------------------------------------------------------------------------//
void ptimer_start(struct process *p)
{ 
  get_clock_time(&p->expiry_seconds, &p->expiry_milliseconds);
  PTIMER_DEBUG_MSG2("Ptimer started", p);  
}

//---------------------------------------------------------------------------//
void ptimer_set_reset(struct process *p, float delay, mos_uint8_t mode)
{
  clock_second_t seconds, seconds_temp;
  clock_millisecond_t milliseconds, milliseconds_temp;
  mos_uint32_t temp;
  
  if( mode == PTIMER_RESET )
  {
    seconds = p->expiry_seconds;
    milliseconds = p->expiry_milliseconds;
  }    
  else
    get_clock_time(&seconds, &milliseconds);

  // Please note minimum delay supported by ptimer_set_resets is 1 milli second
  if(delay < 0.001)
    delay = 0.001;
    
  // converting float value to integer value with 1 milli second precision
  temp = (mos_uint32_t)(delay * 1000); 
  seconds_temp = temp / 1000;
  milliseconds_temp = temp % 1000 ;
  
  milliseconds_temp = milliseconds_temp + milliseconds; 
  
  p->expiry_seconds = seconds + seconds_temp + milliseconds_temp / 1000;
  p->expiry_milliseconds =  milliseconds_temp % 1000;
  
  PTIMER_DEBUG_MSG1_NO_NEWLINE("Delay Scheduled, Delay = ", p);
  #if ((PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL1) && (DEBUG_LEVEL > DEBUG_NONE))
    debug_print_float_ln(delay);
  #endif
  
  PTIMER_DEBUG_MSG2_NO_NEWLINE("Ptimer set, Expiry Seconds = ", p);
  #if ((PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL2) && (DEBUG_LEVEL > DEBUG_NONE))
    debug_print_integer_long(p->expiry_seconds);
    DEBUG_PRINT(", Milliseconds = ");
    debug_print_integer_ln(p->expiry_milliseconds);
  #endif
  
  schedule_ptimer(p); 
}

//---------------------------------------------------------------------------//
mos_uint8_t ptimer_expired(struct process *p)
{
    clock_second_t seconds;
    clock_millisecond_t milliseconds;
    
    PTIMER_DEBUG_MSG2("Checking if Ptimer expired",p);
    
    get_clock_time(&seconds, &milliseconds);    
    
    if(seconds > p->expiry_seconds)
    {
        PTIMER_DEBUG_MSG2("Ptimer expired seconds",p);
        return 1;
    }   
    else if ( seconds == p->expiry_seconds )
    {
        if( milliseconds >= p->expiry_milliseconds )
        {
            PTIMER_DEBUG_MSG2("Ptimer expired milliseconds",p);
            return 1;
        }            
        else
        {
            PTIMER_DEBUG_MSG2("Ptimer not expired",p);
            return 0;
        }    
    } 
    else
    {
        PTIMER_DEBUG_MSG2("Ptimer not expired",p);
        return 0;
    }
} 

//---------------------------------------------------------------------------//
/** 
* \brief		This fuctions includes ptimer of called process in ptimer linked list
* \param p   	A pointer to the process
* \return void	
*
* Whenever ptimer of any process is set or reset using function **ptimer_set_reset**,
* this function is called. This function includes ptimer of called process **p** into
* ptimer linked list. By putting it on ptimer linked list, called process **p** will
* be woken up by ptimer process after expiry of set time. In Ptimer linked list,
* ptimers of different processes are arranged in ascending order of their respective
* expiry time.
*/
static void schedule_ptimer(struct process * p)
{
    struct process * q, * q_last;
        
    if( (ptimer_list == NULL) || (compare_ptimer_expiry(p , ptimer_list) == 1))
    {
        p->ptimer_next = ptimer_list;
        ptimer_list = p;
        PTIMER_DEBUG_MSG3("Schedule next tickless period request", ptimer_list);
		#ifdef PLATFORM_SUPPORT_TICKLESS
			schedule_tickless_timer_process(ptimer_list);
		#endif
    }
    
    else
    {
        q_last = ptimer_list;
        for ( q = ptimer_list->ptimer_next ; q != NULL ; q = q->ptimer_next  )
        {
            if( compare_ptimer_expiry(p , q) == 1)
            {
                p->ptimer_next = q;
                q_last->ptimer_next = p;
                break;
            }            
            q_last = q;              
        }
            
        if (q == NULL)
        {
          p->ptimer_next = NULL;
          q_last->ptimer_next = p;
        }            
    }    
    PTIMER_DEBUG_MSG2("Ptimer scheduled",p);    
}

/** 
* \brief				This fuction compares expiry time of ptimers of input processes
* \param p_compare  	A pointer to process to be compared
* \param p_reference  	A pointer to reference process
* \return mos_uint8_t	Returns 1 if expiry of p_compare is less than p_reference, else 0	
*
* This fuction compares expiry time of ptimer of process **p_compare** with process
* **p_reference**. Function returns 1 if expiry of **p_compare** is less than **p_reference**,
* otherwise returns 0.
*/
static mos_uint8_t compare_ptimer_expiry(struct process *p_compare , struct process *p_reference )
{
    if(p_compare->expiry_seconds < p_reference->expiry_seconds)
    {
        PTIMER_DEBUG_MSG3("p expiry seconds < ptimer_list expiry", p_reference);        
        return 1;
    }
    
    else if(p_compare->expiry_seconds == p_reference->expiry_seconds)
    {
        if(p_compare->expiry_milliseconds < p_reference->expiry_milliseconds)
        {            
            PTIMER_DEBUG_MSG3("p expiry milliseconds < ptimer_list expiry", p_reference);            
            return 1;
        }
    
        else
            return 0;            
    }    
        
    else
        return 0;
}

#ifdef PLATFORM_SUPPORT_TICKLESS

/** 
* \brief		Schedule tickless timer for the called process.
* \param p   	A pointer to process
* \return void
*
* This function checks expiry time of called process **p**. If condition satisfies it schedules
* it in the next tickless timer cycle. Tickless timer is platform dependent. If platform doesn't
* support tickless timer, this function is not included.  
*/
static void schedule_tickless_timer_process(struct process * p)
{   
	clock_second_t temp_seconds=0;
	clock_millisecond_t temp_milliseconds=0;
			
	get_clock_time(&temp_seconds, &temp_milliseconds);

	if( temp_seconds < p->expiry_seconds) 
    {
        PTIMER_DEBUG_MSG2("No tickless schedule", p);
    } 
	 
	else if(temp_seconds == p->expiry_seconds ) 
	{
		if( temp_milliseconds >= p->expiry_milliseconds )
		{
			PTIMER_DEBUG_MSG2("Process Expired milliseconds", p);
			ptimer_request_poll();
		}
		else
		{            
			PTIMER_DEBUG_MSG2("Request tickless timer platform", p);
			schedule_tickless_timer_platform((p->expiry_milliseconds)); 
		} 
	}
	
	else if(temp_seconds > p->expiry_seconds )
	{
		PTIMER_DEBUG_MSG2("Process Expired seconds", p);
		ptimer_request_poll();    
	}
}

#endif


#endif

