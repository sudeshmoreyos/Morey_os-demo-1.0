/** \addtogroup sources_sys
* @{ \defgroup ptimer_c
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

#define PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name) 	DEBUG_MSG_CONST3("<DEBUG PTIMER> ",task_name,msg)
#define PTIMER_DEBUG_MSG_TEMP(msg,task_name) 			DEBUG_MSG_CONST3_LN("<DEBUG PTIMER> ",task_name,msg)
#define PTIMER_DEBUG_MSG_USER_TEMP(msg,task_name) 		DEBUG_MSG_CONST3_LN("<DEBUG PTIMER USER> ",task_name,msg)              

#if PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_NONE
	#define PTIMER_DEBUG_MSG_USER(msg,task_name)
	#define PTIMER_DEBUG_MSG1(msg,task_name)
	#define PTIMER_DEBUG_MSG2(msg,task_name)
	#define PTIMER_DEBUG_MSG3(msg,task_name)
	#define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,task_name)
	#define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,task_name)
	#define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,task_name)
    
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_USER
	#define PTIMER_DEBUG_MSG_USER(msg,task_name) 		PTIMER_DEBUG_MSG_USER_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG1(msg,task_name)
	#define PTIMER_DEBUG_MSG2(msg,task_name)
	#define PTIMER_DEBUG_MSG3(msg,task_name)
	#define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,task_name)
	#define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,task_name)
	#define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,task_name) 

#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_LEVEL1
	#define PTIMER_DEBUG_MSG_USER(msg,task_name)
	#define PTIMER_DEBUG_MSG1(msg,task_name) 		PTIMER_DEBUG_MSG_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG2(msg,task_name)
	#define PTIMER_DEBUG_MSG3(msg,task_name)
	#define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,task_name) 	PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,task_name)
	#define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,task_name)    
     
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_LEVEL2
	#define PTIMER_DEBUG_MSG_USER(msg,task_name)
	#define PTIMER_DEBUG_MSG1(msg,task_name) 		PTIMER_DEBUG_MSG_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG2(msg,task_name) 		PTIMER_DEBUG_MSG_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG3(msg,task_name)
	#define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,task_name) 	PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,task_name) 	PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,task_name) 
    
#elif PTIMER_DEBUG_LEVEL == PTIMER_DEBUG_LEVEL3
	#define PTIMER_DEBUG_MSG_USER(msg,task_name)
	#define PTIMER_DEBUG_MSG1(msg,task_name) 		PTIMER_DEBUG_MSG_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG2(msg,task_name) 		PTIMER_DEBUG_MSG_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG3(msg,task_name) 		PTIMER_DEBUG_MSG_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG1_NO_NEWLINE(msg,task_name) 	PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG2_NO_NEWLINE(msg,task_name) 	PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name)
	#define PTIMER_DEBUG_MSG3_NO_NEWLINE(msg,task_name) 	PTIMER_DEBUG_MSG_NO_NEWLINE_TEMP(msg,task_name)
               
#endif

static struct task_struct_os * ptimer_list = NULL;

void ptimer_init(void)
{
    ptimer_list = NULL;
    PTIMER_DEBUG_MSG3("PTIMER Task Initiated",NULL);
}

static void schedule_ptimer(struct task_struct_os * p);
static mos_uint8_t compare_ptimer_expiry(struct task_struct_os *p_compare , struct task_struct_os *p_reference );

#ifdef PLATFORM_SUPPORT_TICKLESS
	static void schedule_tickless_timer(struct task_struct_os * p);
#endif

TASK_CREATE(ptimer_task, "PTIMER TASK");

/*
//---------------------------------------------------------------------------//
TASK_RUN(ptimer_task)
{	
	TASK_BEGIN();
	while(1) 
	{	  
		TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_POLL);
		PTIMER_DEBUG_MSG3("PTIMER Called",&ptimer_task);
	    
		while(ptimer_list != NULL)
		{
			// checking ptimer_state is redundant because if a ptimer is
			// active means it will be on ptimer_list linked list else not
			if( ptimer_expired(ptimer_list) )
			{                
				if(task_post(ptimer_list, TASK_EVENT_PTIMER, NULL) == TASK_ERR_OK)
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
	
		#ifdef PLATFORM_SUPPORT_TICKLESS
			if(ptimer_list != NULL)
			{
				PTIMER_DEBUG_MSG3("check for tickless schedule", ptimer_list);
				schedule_tickless_timer(ptimer_list);
			}
		#endif
	   
		PTIMER_DEBUG_MSG3_NO_NEWLINE("ptimer list = ", NULL);
		#if (PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL3)
			#ifdef TASK_HAS_STRING_NAMES
				{
					struct task_struct_os * p1;
					for( p1 = ptimer_list ; p1 != NULL ; p1 = p1->ptimer_next)
					{
						DEBUG_PRINT((char *)TASK_NAME_STRING(p1));
						if(p1->ptimer_next != NULL)
							DEBUG_PRINT(", ");
					}
					DEBUG_PRINT_LN("");
				}
			#else
			    DEBUG_PRINT_LN("TASK_HAS_STRING_NAMES_CONF macro not defined");
			#endif
		#endif         
  }

  TASK_END();
}
*/

//---------------------------------------------------------------------------//
TASK_RUN(ptimer_task)
{	
    TASK_BEGIN();

    while (1) 
    {	  
        TASK_WAIT_EVENT_UNTIL(ev == TASK_EVENT_POLL);
        PTIMER_DEBUG_MSG3("PTIMER Called", &ptimer_task);

        struct task_struct_os *p = ptimer_list;
        struct task_struct_os *prev = NULL;
        struct task_struct_os *next = NULL;

        // Traverse the ptimer list safely
        while (p != NULL)
        {
            next = p->ptimer_next;   // store next pointer early (important!)

            if (ptimer_expired(p))
            {
                // Try to post event to expired timer task
                if (task_post(p, TASK_EVENT_PTIMER, NULL) == TASK_ERR_OK)
                {
                    PTIMER_DEBUG_MSG3("Timer event post success", p);

                    // Remove this task from ptimer list
                    if (prev == NULL)
                    {
                        // Removing head
                        ptimer_list = next;
                    }
                    else
                    {
                        // Removing middle or tail node
                        prev->ptimer_next = next;
                    }

                    // Clean up task's timer linkage and mark inactive
                    p->ptimer_next = NULL;
                    p->ptimer_state = PTIMER_RESET;

                    // Do not update prev here, because current node was removed
                }
                else
                {
                    // Failed to post event -> reschedule poll
                    PTIMER_DEBUG_MSG3("Timer event post failure", p);
                    ptimer_request_poll();

                    // Stop scanning for now to retry later
                    break;
                }
            }
            else
            {
                // No more expired timers; since list is sorted by expiry,
                // we can safely break early for efficiency.
                break;
            }

            // Advance iteration pointer
            p = next;
        }

#ifdef PLATFORM_SUPPORT_TICKLESS
        // If at least one timer remains, schedule the next tickless wakeup
        if (ptimer_list != NULL)
        {
            PTIMER_DEBUG_MSG3("check for tickless schedule", ptimer_list);
            schedule_tickless_timer(ptimer_list);
        }
#endif

        // Optional: Debug the full timer list contents (level 3)
        PTIMER_DEBUG_MSG3_NO_NEWLINE("ptimer list = ", NULL);
#if (PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL3)
#ifdef TASK_HAS_STRING_NAMES
        {
            struct task_struct_os *p1;
            for (p1 = ptimer_list; p1 != NULL; p1 = p1->ptimer_next)
            {
                DEBUG_PRINT((char *)TASK_NAME_STRING(p1));
                if (p1->ptimer_next != NULL)
                    DEBUG_PRINT(", ");
            }
            DEBUG_PRINT_LN("");
        }
#else
        DEBUG_PRINT_LN("TASK_HAS_STRING_NAMES_CONF macro not defined");
#endif
#endif
    }

    TASK_END();
}


//---------------------------------------------------------------------------//
void ptimer_request_poll(void)
{  
	task_poll(&ptimer_task);	
	PTIMER_DEBUG_MSG3("Ptimer poll requested",NULL);
}

//---------------------------------------------------------------------------//
void ptimer_start(struct task_struct_os *p)
{ 
	get_clock_time(&p->expiry_seconds, &p->expiry_milliseconds);
	PTIMER_DEBUG_MSG2("Ptimer started", p);  
}

//---------------------------------------------------------------------------//
void ptimer_set_reset(struct task_struct_os *p, float delay, mos_uint8_t mode)
{
	clock_second_t seconds, seconds_temp;
	clock_millisecond_t milliseconds, milliseconds_temp;
	mos_uint32_t temp;
	
	if (p == NULL) 
	{
		PTIMER_DEBUG_MSG1("Null task pointer", NULL);
		return;
	}
	if (delay < 0.0f) delay = 0.0f;

  
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
	#if PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL1
	   DEBUG_PRINT_FLOAT_LN(delay);
	#endif
	  
	PTIMER_DEBUG_MSG2_NO_NEWLINE("Ptimer set, Expiry Seconds = ", p);
	#if PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL2
		DEBUG_PRINT_INTEGER_LONG(p->expiry_seconds);
		DEBUG_PRINT(", Milliseconds = ");
		DEBUG_PRINT_INTEGER_LN(p->expiry_milliseconds);
	#endif
	  
	schedule_ptimer(p); 
}

//---------------------------------------------------------------------------//
mos_uint8_t ptimer_expired(struct task_struct_os *p)
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
* \brief	This functions includes ptimer of called Task in ptimer linked list
* \param p   	A pointer to the Task
* \return void	
*
* Whenever ptimer of any Task is set or reset using function **ptimer_set_reset**,
* this function is called. This function includes ptimer of called Task **p** into
* ptimer linked list. By putting it on ptimer linked list, called Task **p** will
* be woken up by ptimer Task after expiry of set time. In Ptimer linked list,
* ptimers of different tasks are arranged in ascending order of their respective
* expiry time.
*/
static void schedule_ptimer(struct task_struct_os * p)
{
	struct task_struct_os * q, * q_last;

	if( (ptimer_list == NULL) || (compare_ptimer_expiry(p , ptimer_list) == 1))
	{
		p->ptimer_next = ptimer_list;
		ptimer_list = p;
		PTIMER_DEBUG_MSG3("Schedule next tickless period request", ptimer_list);
		#ifdef PLATFORM_SUPPORT_TICKLESS
			schedule_tickless_timer(ptimer_list);
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
* \brief				This fuction compares expiry time of ptimers of input tasks
* \param p_compare  	A pointer to Task to be compared
* \param p_reference  	A pointer to reference Task
* \return mos_uint8_t	Returns 1 if expiry of p_compare is less than p_reference, else 0	
*
* This fuction compares expiry time of ptimer of Task **p_compare** with Task
* **p_reference**. Function returns 1 if expiry of **p_compare** is less than **p_reference**,
* otherwise returns 0.
*/
static mos_uint8_t compare_ptimer_expiry(struct task_struct_os *p_compare , struct task_struct_os *p_reference )
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
* \brief		Schedule tickless timer for the called Task.
* \param p   	A pointer to Task
* \return void
*
* This function checks expiry time of called Task **p**. If condition satisfies it schedules
* it in the next tickless timer cycle. Tickless timer is platform dependent. If platform doesn't
* support tickless timer, this function is not included.  
*/

static void schedule_tickless_timer(struct task_struct_os * p)
{   
	clock_second_t temp_seconds=0;
	clock_millisecond_t temp_milliseconds=0;
			
	get_clock_time(&temp_seconds, &temp_milliseconds);
	
	#if PTIMER_DEBUG_LEVEL >= PTIMER_DEBUG_LEVEL2
		if( temp_seconds < p->expiry_seconds) 
		{
			PTIMER_DEBUG_MSG2("No tickless schedule", p);
		}
	#endif
	 
	if(temp_seconds == p->expiry_seconds ) 
	{
		if( temp_milliseconds >= p->expiry_milliseconds )
		{
			PTIMER_DEBUG_MSG2("Task Expired milliseconds", p);
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
		PTIMER_DEBUG_MSG2("Task Expired seconds", p);
		ptimer_request_poll();    
	}
}


#endif
#endif

