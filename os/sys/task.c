/** \addtogroup sources_sys
* @{ \defgroup task_c
* @{
* \brief Documentaion of task.c source file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
* @}
* This is source file for Task header
*/

#ifndef DISABLE_OS

#include "task.h"
#include "ptimer.h"

#ifdef TASK_DEBUG_LEVEL_CONF
	/// if user defines TASK_DEBUG_LEVEL_CONF macro in config.h file, set it as debug level for this file
    #define TASK_DEBUG_LEVEL TASK_DEBUG_LEVEL_CONF
#else
	/// else by default set debug level as none
    #define TASK_DEBUG_LEVEL TASK_DEBUG_NONE
#endif

#if TASK_DEBUG_LEVEL > TASK_DEBUG_NONE
    #include "../lib/debug.h"
#endif

#define TASK_DEBUG_MSG_TEMP(msg,task_name) 			DEBUG_MSG_CONST3_LN("<DEBUG TASK> ",task_name,msg)
#define TASK_DEBUG_MSG_USER_TEMP(msg,task_name) 	DEBUG_MSG_CONST3_LN("<DEBUG TASK USER> ",task_name,msg)

#if TASK_DEBUG_LEVEL == TASK_DEBUG_NONE
    #define TASK_DEBUG_MSG_USER(msg)
    #define TASK_DEBUG_MSG1(msg,task_name)
    #define TASK_DEBUG_MSG2(msg,task_name)
    #define TASK_DEBUG_MSG3(msg,task_name)

#elif TASK_DEBUG_LEVEL == TASK_DEBUG_USER
        
    #define TASK_DEBUG_MSG_USER(msg,task_name) 	TASK_DEBUG_MSG_USER_TEMP(msg,task_name)
    #define TASK_DEBUG_MSG1(msg,task_name)
    #define TASK_DEBUG_MSG2(msg,task_name)
    #define TASK_DEBUG_MSG3(msg,task_name)
    
#elif TASK_DEBUG_LEVEL == TASK_DEBUG_LEVEL1

    #define TASK_DEBUG_MSG_USER(msg,task_name)
    #define TASK_DEBUG_MSG1(msg,task_name) 		TASK_DEBUG_MSG_TEMP(msg,task_name)
    #define TASK_DEBUG_MSG2(msg,task_name)
    #define TASK_DEBUG_MSG3(msg,task_name)
     
#elif TASK_DEBUG_LEVEL == TASK_DEBUG_LEVEL2

    #define TASK_DEBUG_MSG_USER(msg,task_name)
    #define TASK_DEBUG_MSG1(msg,task_name) 		TASK_DEBUG_MSG_TEMP(msg,task_name)
    #define TASK_DEBUG_MSG2(msg,task_name) 		TASK_DEBUG_MSG_TEMP(msg,task_name)
    #define TASK_DEBUG_MSG3(msg,task_name)
    
#elif TASK_DEBUG_LEVEL == TASK_DEBUG_LEVEL3

    #define TASK_DEBUG_MSG_USER(msg,task_name)
    #define TASK_DEBUG_MSG1(msg,task_name) 		TASK_DEBUG_MSG_TEMP(msg,task_name)
    #define TASK_DEBUG_MSG2(msg,task_name) 		TASK_DEBUG_MSG_TEMP(msg,task_name)
    #define TASK_DEBUG_MSG3(msg,task_name) 		TASK_DEBUG_MSG_TEMP(msg,task_name)         

#endif

///Pointer to the currently running task structure.
struct task_struct_os *task_list = NULL;
struct task_struct_os *task_current = NULL;

///Structure used for keeping the queue of active events.
struct event_data {
  task_event_t ev;
  task_data_t data;
  struct task_struct_os *p;
};

static task_num_events_t nevents, fevent;
static struct event_data events[TASK_NUMEVENTS];
static volatile mos_uint8_t poll_requested;

#define TASK_STATE_NONE        0
#define TASK_STATE_RUNNING     1
#define TASK_STATE_CALLED      2

static void call_task(struct task_struct_os *p, task_event_t ev, task_data_t data);

//---------------------------------------------------------------------------//
void task_init(void)
{
  nevents = fevent = 0;
  task_current = task_list = NULL;
  TASK_DEBUG_MSG3("Task kernel Initiated",NULL);
}

/*
//---------------------------------------------------------------------------//
void task_start(struct task_struct_os *p, task_data_t data)
{
  struct task_struct_os *q;

  //First make sure that we don't try to start a task that is already running.
  for(q = task_list; q != NULL && q != p; q = q->next);

  //If we found the task on the task list, we bail out. //
  if(q == p) {
    TASK_DEBUG_MSG1("Task already running, start failed", p);    
    return;
  }
  else
  {
    TASK_DEBUG_MSG1("Task started", p);  
  }
  
  // Put on the task list.//
  p->next = task_list;
  task_list = p;
  p->state = TASK_STATE_RUNNING;
  p->position = 0;

  // Post a synchronous initialization event to the task, i.e. call this task at least once 
  q = task_current;
  call_task(p, TASK_EVENT_INIT , data);
  task_current = q;
}
*/

//---------------------------------------------------------------------------//
void task_start(struct task_struct_os *p, task_data_t data)
{
    struct task_struct_os *q;

    // First make sure that we don't try to start a task that is already running.
    for (q = task_list; q != NULL && q != p; q = q->next);

    // If we found the task on the task list, we bail out.
    if (q == p) {
        TASK_DEBUG_MSG1("Task already running, start failed", p);
        return;
    } else {
        TASK_DEBUG_MSG1("Task started", p);
    }

    // Initialize all task fields properly before linking
    p->next = task_list;              // link into task list
    p->state = TASK_STATE_RUNNING;    // mark as running
    p->position = 0;                  // start from beginning
    p->needspoll = 0;                 // clear poll request
    p->data = NULL;                   // clear previous data
    p->data2 = NULL;
    p->ptimer_next = NULL;            // no pending ptimer link
    p->expiry_seconds = 0;            // clear timers
    p->expiry_milliseconds = 0;
    p->ptimer_state = 0;

    // Insert this task at head of the task list
    task_list = p;

    // Post a synchronous initialization event to the task, i.e. call this task once
    q = task_current;
    call_task(p, TASK_EVENT_INIT, data);
    task_current = q;
}


//---------------------------------------------------------------------------//
void task_exit(struct task_struct_os *p)
{
  struct task_struct_os *q;

  // Make sure the task is in the task list before we try to exit it. //
  for(q = task_list; q != p && q != NULL; q = q->next);
  if(q == NULL) 
  {
    TASK_DEBUG_MSG1("Task not running, exit failed", p);
    return;
  }
  	
  // if task was running, change its state to none //
  if(task_is_running(p))    
    p->state = TASK_STATE_NONE;

  // Finally exiting the current task
  if(p == task_list) 
  {
    task_list = task_list->next;
  } 
  else 
  {
    for(q = task_list; q != NULL; q = q->next) 
    {
      if(q->next == p) 
      {
        q->next = p->next;
        break;
      }
    }
  }
  
  TASK_DEBUG_MSG1("Task exited", p);
}

/** 
* \brief		This fuction calls task thread function and passes arguments ev and data to it
* \param p   	A pointer to the task
* \param ev   	Task event ev of type task_event_t
* \param data 	Data to pass to the task of type task_data_t	
* \return void	
*
* This Function actually runs each task call, by running task thread function. It first checks
* if task state is **TASK_STATE_RUNNING** and Task thread is not NULL, then it calls task
* thread function. After execution, if task thread function returns **PT_EXITED** , **PT_ENDED**, 
* or Event **ev** passed to it is **TASK_EVENT_EXIT**, task is exited by calling function
* task_exit(). 
*/
static void call_task(struct task_struct_os *p, task_event_t ev, task_data_t data)
{
  mos_uint8_t ret;

  if((p->state & TASK_STATE_RUNNING) && p->thread != NULL) 
  {
    #if TASK_DEBUG_LEVEL == TASK_DEBUG_LEVEL1
		// If TASK_DEBUG_LEVEL is TASK_DEBUG_LEVEL1, we display calling of all tasks except Ptimer task
		if(p != &ptimer_task)
			TASK_DEBUG_MSG1("Calling Task", p);
	#else
		// Otherwise we display calling of all tasks including Ptimer task
		TASK_DEBUG_MSG2("Calling Task", p);
	#endif
	
    task_current = p;
    p->state = TASK_STATE_CALLED;
    p->data = data;
    ret = p->thread(ev);
    // We can exit task in four ways, TASK_EXIT() macro, TASK_END() macro
    // posting a TASK_EVENT_EXIT event to a task and task_exit() function
    if(ret == PT_EXITED || ret == PT_ENDED || ev == TASK_EVENT_EXIT) 
    {
      task_exit(p);
    } 
    else 
    {
      p->state = TASK_STATE_RUNNING;
    }
  }
}

/** 
* \brief	Declare function to call poll handler of each Task.
*
* This function is called inside task_run() function. It runs all tasks
* which has requested to poll itself . After running each polled task, poll
* flag is cleared. Which means to continously poll a task, task_poll()
* function must be called repeatedly inside task thread function.
*/
static void do_poll(void)
{
  struct task_struct_os *p;

  poll_requested = 0;
  
  // Call the tasks that needs to be polled. //
  for(p = task_list; p != NULL; p = p->next) {
    if(p->needspoll) {
      TASK_DEBUG_MSG3("Polling Task", p);
      p->state = TASK_STATE_RUNNING;
      p->needspoll = 0;
      call_task(p, TASK_EVENT_POLL, NULL);
    }
  }
}


/** 
* \brief	Process the next event in the event queue and deliver it to listening Tasks.
*
* This function is called inside task_run() function. It checks for next event 
* in the event queue and deliver it to listening Tasks.
*/
static void do_event(void)
{
  task_event_t ev;
  task_data_t data;
  struct task_struct_os *receiver;
  struct task_struct_os *p;
  
  // If there are any events in the queue, take the first one and walk
  // through the list of tasks to see if the event should be
  // delivered to any of them. If so, we call the event handler
  // function for the task. We only process one event at a time and
  // call the poll handlers in between.

  if(nevents > 0) 
  {
    // There are events that we should deliver. //
    ev = events[fevent].ev;
    data = events[fevent].data;
    receiver = events[fevent].p;

    // Since we have seen the new event, we move pointer upwards
    // and decrease the number of events.
    fevent = (fevent + 1) % TASK_NUMEVENTS;
    --nevents;

    // If this is a broadcast event, we deliver it to all events, in
    // order of their priority.
    TASK_DEBUG_MSG3("Do Event", receiver);
    if(receiver == TASK_BROADCAST) 
    {
      for(p = task_list; p != NULL; p = p->next) 
      {
        // If we have been requested to poll a task, we do this in
        // between processing the broadcast event.
        if(poll_requested) 
        {
          do_poll();
        }
                
        call_task(p, ev, data);
      }
    } 
    else 
    {
      // This is not a broadcast event, so we deliver it to the specified task.
      // If the event was an INIT event, we should also update the state of the task.
      if(ev == TASK_EVENT_INIT) 
      {
        receiver->state = TASK_STATE_RUNNING;
      }
              
      // Make sure that the task actually is running. //
      call_task(receiver, ev, data);
    }
  }
}

//---------------------------------------------------------------------------//
int task_run(void)
{
	TASK_DEBUG_MSG3("Task run",NULL);
	// Task poll events. //
	if(poll_requested) 
	{
		do_poll();
		TASK_DEBUG_MSG3("do_poll complete",NULL);
	}

	// Task one event from the queue
	do_event();
	TASK_DEBUG_MSG3("do_event complete",NULL);
	
	return nevents + poll_requested;
}

//---------------------------------------------------------------------------//
int task_post(struct task_struct_os *p, task_event_t ev, task_data_t data)
{
  task_num_events_t snum;

  if(nevents == TASK_NUMEVENTS) 
  {
    TASK_DEBUG_MSG2("Task post failed", p);
    return TASK_ERR_FULL;
  }
  
  TASK_DEBUG_MSG2("Posting Task", p);
  snum = (task_num_events_t)(fevent + nevents) % TASK_NUMEVENTS;
  events[snum].ev = ev;
  events[snum].data = data;
  events[snum].p = p;
  ++nevents;

  return TASK_ERR_OK;
}

//---------------------------------------------------------------------------//
void task_poll(struct task_struct_os *p)
{
  if(p != NULL) 
  {
    if(p->state == TASK_STATE_RUNNING || p->state == TASK_STATE_CALLED) 
    {
      p->needspoll = 1;
      poll_requested = 1;
      TASK_DEBUG_MSG2("Requesting Task Poll", p);
    }
  }
}

//---------------------------------------------------------------------------//
int task_is_running(struct task_struct_os *p)
{
  // return p->state != TASK_STATE_NONE;
  return (p != NULL) && (p->state != TASK_STATE_NONE);

}

//---------------------------------------------------------------------------//
void task_autostart(struct task_struct_os * const tasks[])
{
  int i;

  for(i = 0; tasks[i] != NULL; ++i) {
    task_start(tasks[i], NULL);
  }
}

#endif


///@} @} @} 