/** \addtogroup os
* @{ \addtogroup source
* @{ \defgroup process_c
* @{ \addtogroup process_c 
* @{
* \brief Documentaion of process.c source file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
* @}
* This is source file for process header
*/

#ifndef DISABLE_OS

#include "process.h"
#include "ptimer.h"

#ifdef PROCESS_DEBUG_LEVEL_CONF
	/// if user defines PROCESS_DEBUG_LEVEL_CONF macro in config.h file, set it as debug level for this file
    #define PROCESS_DEBUG_LEVEL PROCESS_DEBUG_LEVEL_CONF
#else
	/// else by default set debug level as none
    #define PROCESS_DEBUG_LEVEL PROCESS_DEBUG_NONE
#endif

#if PROCESS_DEBUG_LEVEL > PROCESS_DEBUG_NONE
    #include "../lib/debug.h"
#endif

#define PROCESS_DEBUG_MSG_TEMP(msg,process_name) DEBUG_MSG_LN("<DEBUG PROCESS> ",process_name,msg)
#define PROCESS_DEBUG_MSG_USER_TEMP(msg,process_name) DEBUG_MSG_LN("<DEBUG PROCESS USER> ",process_name,msg)

#if PROCESS_DEBUG_LEVEL == PROCESS_DEBUG_NONE
    #define PROCESS_DEBUG_MSG_USER(msg)
    #define PROCESS_DEBUG_MSG1(msg,process_name)
    #define PROCESS_DEBUG_MSG2(msg,process_name)
    #define PROCESS_DEBUG_MSG3(msg,process_name)

#elif PROCESS_DEBUG_LEVEL == PROCESS_DEBUG_USER
        
    #define PROCESS_DEBUG_MSG_USER(msg,process_name) PROCESS_DEBUG_MSG_USER_TEMP(msg,process_name)
    #define PROCESS_DEBUG_MSG1(msg,process_name)
    #define PROCESS_DEBUG_MSG2(msg,process_name)
    #define PROCESS_DEBUG_MSG3(msg,process_name)
    
#elif PROCESS_DEBUG_LEVEL == PROCESS_DEBUG_LEVEL1

    #define PROCESS_DEBUG_MSG_USER(msg,process_name)
    #define PROCESS_DEBUG_MSG1(msg,process_name) PROCESS_DEBUG_MSG_TEMP(msg,process_name)
    #define PROCESS_DEBUG_MSG2(msg,process_name)
    #define PROCESS_DEBUG_MSG3(msg,process_name)
     
#elif PROCESS_DEBUG_LEVEL == PROCESS_DEBUG_LEVEL2

    #define PROCESS_DEBUG_MSG_USER(msg,process_name)
    #define PROCESS_DEBUG_MSG1(msg,process_name) PROCESS_DEBUG_MSG_TEMP(msg,process_name)
    #define PROCESS_DEBUG_MSG2(msg,process_name) PROCESS_DEBUG_MSG_TEMP(msg,process_name)
    #define PROCESS_DEBUG_MSG3(msg,process_name)
    
#elif PROCESS_DEBUG_LEVEL == PROCESS_DEBUG_LEVEL3

    #define PROCESS_DEBUG_MSG_USER(msg,process_name)
    #define PROCESS_DEBUG_MSG1(msg,process_name) PROCESS_DEBUG_MSG_TEMP(msg,process_name)
    #define PROCESS_DEBUG_MSG2(msg,process_name) PROCESS_DEBUG_MSG_TEMP(msg,process_name)
    #define PROCESS_DEBUG_MSG3(msg,process_name) PROCESS_DEBUG_MSG_TEMP(msg,process_name)         

#endif

///Pointer to the currently running process structure.
struct process *process_list = NULL;
struct process *process_current = NULL;

///Structure used for keeping the queue of active events.
struct event_data {
  process_event_t ev;
  process_data_t data;
  struct process *p;
};

static process_num_events_t nevents, fevent;
static struct event_data events[PROCESS_NUMEVENTS];
static volatile mos_uint8_t poll_requested;

#define PROCESS_STATE_NONE        0
#define PROCESS_STATE_RUNNING     1
#define PROCESS_STATE_CALLED      2

static void call_process(struct process *p, process_event_t ev, process_data_t data);

//---------------------------------------------------------------------------//
void process_init(void)
{
  nevents = fevent = 0;
  process_current = process_list = NULL;
  PROCESS_DEBUG_MSG3("Process kernel Initiated",NULL);
}

//---------------------------------------------------------------------------//
void process_start(struct process *p, process_data_t data)
{
  struct process *q;

  //First make sure that we don't try to start a process that is already running.
  for(q = process_list; q != p && q != NULL; q = q->next);

  //If we found the process on the process list, we bail out. //
  if(q == p) {
    PROCESS_DEBUG_MSG1("Process already running, start failed", p);    
    return;
  }
  else
  {
    PROCESS_DEBUG_MSG1("Process started", p);  
  }
  // Put on the process list.//
  p->next = process_list;
  process_list = p;
  p->state = PROCESS_STATE_RUNNING;
  p->position = 0;

  // Post a synchronous initialization event to the process, i.e. call this process at least once 
  q = process_current;
  call_process(p, PROCESS_EVENT_INIT , data);
  process_current = q;
}

//---------------------------------------------------------------------------//
void process_exit(struct process *p)
{
  struct process *q;

  // Make sure the process is in the process list before we try to exit it. //
  for(q = process_list; q != p && q != NULL; q = q->next);
  if(q == NULL) 
  {
    PROCESS_DEBUG_MSG1("Process not running, exit failed", p);
    return;
  }
  	
  // if process was running, change its state to none //
  if(process_is_running(p))    
    p->state = PROCESS_STATE_NONE;

  // Finally exiting the current process
  if(p == process_list) 
  {
    process_list = process_list->next;
  } 
  else 
  {
    for(q = process_list; q != NULL; q = q->next) 
    {
      if(q->next == p) 
      {
        q->next = p->next;
        break;
      }
    }
  }
  
  PROCESS_DEBUG_MSG1("Process exited", p);
}

/** 
* \brief		This fuction calls process thread function and passes arguments ev and data to it
* \param p   	A pointer to the process
* \param ev   	Process event ev of type process_event_t
* \param data 	Data to pass to the process of type process_data_t	
* \return void	
*
* This Function actually runs each process call, by running process thread function. It first checks
* if process state is **PROCESS_STATE_RUNNING** and Process thread is not NULL, then it calls process
* thread function. After execution, if process thread function returns **PT_EXITED** , **PT_ENDED**, 
* or Event **ev** passed to it is **PROCESS_EVENT_EXIT**, process is exited by calling function
* process_exit(). 
*/
static void call_process(struct process *p, process_event_t ev, process_data_t data)
{
  mos_uint8_t ret;

  if((p->state & PROCESS_STATE_RUNNING) && p->thread != NULL) 
  {
    #if PROCESS_DEBUG_LEVEL == PROCESS_DEBUG_LEVEL1
		if(p != &ptimer_process)
			PROCESS_DEBUG_MSG1("Calling Process", p);
	#else
		PROCESS_DEBUG_MSG2("Calling Process", p);
	#endif
	
    process_current = p;
    p->state = PROCESS_STATE_CALLED;
    p->data = data;
    ret = p->thread(ev);
    // We can exit process in four ways, PROCESS_EXIT() macro, PROCESS_END() macro
    // posting a PROCESS_EVENT_EXIT event to a process and process_exit() function
    if(ret == PT_EXITED || ret == PT_ENDED || ev == PROCESS_EVENT_EXIT) 
    {
      process_exit(p);
    } 
    else 
    {
      p->state = PROCESS_STATE_RUNNING;
    }
  }
}

/** 
* \brief	Declare function to call poll handler of each process.
*
* This function is called inside process_run() function. It runs all processes
* which has requested to poll itselg . After running each polled process, poll
* flag is cleared. Which means to continously poll a process, process_poll()
* function must be called repeatedly inside process thread function.
*/
static void do_poll(void)
{
  struct process *p;

  poll_requested = 0;
  
  // Call the processes that needs to be polled. //
  for(p = process_list; p != NULL; p = p->next) {
    if(p->needspoll) {
      PROCESS_DEBUG_MSG3("Polling Process", p);
      p->state = PROCESS_STATE_RUNNING;
      p->needspoll = 0;
      call_process(p, PROCESS_EVENT_POLL, NULL);
    }
  }
}


/** 
* \brief	Process the next event in the event queue and deliver it to listening processes.
*
* This function is called inside process_run() function. It checks for next event 
* in the event queue and deliver it to listening processes.
*/
static void do_event(void)
{
  process_event_t ev;
  process_data_t data;
  struct process *receiver;
  struct process *p;
  
  // If there are any events in the queue, take the first one and walk
  // through the list of processes to see if the event should be
  // delivered to any of them. If so, we call the event handler
  // function for the process. We only process one event at a time and
  // call the poll handlers in between.

  if(nevents > 0) 
  {
    // There are events that we should deliver. //
    ev = events[fevent].ev;
    data = events[fevent].data;
    receiver = events[fevent].p;

    // Since we have seen the new event, we move pointer upwards
    // and decrease the number of events.
    fevent = (fevent + 1) % PROCESS_NUMEVENTS;
    --nevents;

    // If this is a broadcast event, we deliver it to all events, in
    // order of their priority.
    PROCESS_DEBUG_MSG3("Do Event", receiver);
    if(receiver == PROCESS_BROADCAST) 
    {
      for(p = process_list; p != NULL; p = p->next) 
      {
        // If we have been requested to poll a process, we do this in
        //   between processing the broadcast event.
        if(poll_requested) 
        {
          do_poll();
        }
                
        call_process(p, ev, data);
      }
    } 
    else 
    {
      //This is not a broadcast event, so we deliver it to the specified process.
      // If the event was an INIT event, we should also update the state of the process.
      if(ev == PROCESS_EVENT_INIT) 
      {
        receiver->state = PROCESS_STATE_RUNNING;
      }
              
      // Make sure that the process actually is running. //
      call_process(receiver, ev, data);
    }
  }
}

//---------------------------------------------------------------------------//
int process_run(void)
{
  PROCESS_DEBUG_MSG3("Process run",NULL);
  // Process poll events. //
  if(poll_requested) 
  {
    do_poll();
  }

  // Process one event from the queue
  do_event();

  return nevents + poll_requested;
}

//---------------------------------------------------------------------------//
int process_post(struct process *p, process_event_t ev, process_data_t data)
{
  process_num_events_t snum;

  if(nevents == PROCESS_NUMEVENTS) 
  {
    PROCESS_DEBUG_MSG2("Process post failed", p);
    return PROCESS_ERR_FULL;
  }
  
  PROCESS_DEBUG_MSG2("Posting process", p);
  snum = (process_num_events_t)(fevent + nevents) % PROCESS_NUMEVENTS;
  events[snum].ev = ev;
  events[snum].data = data;
  events[snum].p = p;
  ++nevents;

  return PROCESS_ERR_OK;
}

//---------------------------------------------------------------------------//
void process_poll(struct process *p)
{
  if(p != NULL) 
  {
    if(p->state == PROCESS_STATE_RUNNING || p->state == PROCESS_STATE_CALLED) 
    {
      p->needspoll = 1;
      poll_requested = 1;
      PROCESS_DEBUG_MSG2("Requesting Process Poll", p);
    }
  }
}

//---------------------------------------------------------------------------//
int process_is_running(struct process *p)
{
  return p->state != PROCESS_STATE_NONE;
}

//---------------------------------------------------------------------------//
void autostart_start(struct process * const processes[])
{
  int i;

  for(i = 0; processes[i] != NULL; ++i) {
    process_start(processes[i], NULL);
  }
}

#endif


///@} @} @} 