#ifndef PTIMER_H
#define PTIMER_H 

/** \addtogroup os
* @{ \addtogroup header
* @{ \defgroup ptimer_h
* @{ \addtogroup ptimer_h
* @{
* \brief Documentaion of ptimer.h header file    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header implements event timer named as **ptimer** embedded in each process struct. 
* MACROS : **DELAY_SEC()** and **DELAY_SEC_PRECISE()** uses ptimer process functionality
* to implement desired delay requested by code. 
* 
* **USER DEFINED MACROS** that can be used in ptimer.h (These macros should be declared in config.h file) are :
*1. **PTIMER_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {PTIMER_DEBUG_NONE, PTIMER_DEBUG_USER, PTIMER_DEBUG_LEVEL1, PTIMER_DEBUG_LEVEL2, PTIMER_DEBUG_LEVEL3} \n
*=> Eg. Declaration : #define PTIMER_DEBUG_LEVEL_CONF PTIMER_DEBUG_LEVEL1 \n
*=> Default value : PTIMER_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of ptimer.h or ptimer.c, with increasing levels
*(i.e. level 1 2 3) deeper execution steps can be diagnosed. PROCESS_DEBUG_USER level enables PROCESS_DEBUG_MSG_USER Macro.
*To use this, user should manually write PTIMER_DEBUG_MSG_USER(msg,process_name) inside code sections of ptimer.h  or ptimer.c to be diagnosed \n
* @}
*/

#include "process.h"


/**
* \name MACROS : Ptimer Debug Levels
* \brief Define different levels for ptimer debugging
* @{
*/
#define PTIMER_DEBUG_NONE 0
#define PTIMER_DEBUG_USER 1
#define PTIMER_DEBUG_LEVEL1 2
#define PTIMER_DEBUG_LEVEL2 3
#define PTIMER_DEBUG_LEVEL3 4
///@}

#define PTIMER_SET 0
#define PTIMER_RESET 1

/** 
* \brief	Function to init ptimer module
*/
void ptimer_init(void);

/**
* \brief		This function calls ptimer process when system clock event occurs
*
*	Morey_Os implements tickless timer, hence ptimer process or scheduler do not need to be called after
*	every clock change. Morey OS Clock event occurs in two conditions. First when maximum allowed tickless timer span
*	has elapsed. Second when some process is scheduled to be called before expiry of maximum allowed tickelss timer span.
* 	When this function is called, ptimer checks if any process need service. If yes then it wakes up that process
*	else it returns back to OS idle process. Typically this function is called in system clock header funtions.
*/
void ptimer_request_poll(void);

/** 
* \brief		This fuctions intiates ptimer clock paramaters of the called process
* \param p   	A pointer to the process
* \return void	
*
* Ptimer is an event timer which is integrated in each process. Once a process starts
* by calling this function, process initiates its ptimer clock parameters. This function 
* must be called at least once, hence PROCESS_START() macro includes it in its expansion.
*/
void ptimer_start(struct process *p);

/** 
* \brief		Set or reset a process ptimer.
* \param p   	A pointer to the process
* \param delay  The interval before the timer expires of type float
* \param mode	mode of operation ( PTIMER_SET or PTIMER_RESET ) of type mos_uint8_t
* \return void
*
* This function is used to set or reset the ptimer of a process **p** for a time
* **delay** sometime in the future. When the ptimer of respective process expires,
* it wakes up its process so that it can be serviced by OS. This function has two modes
* given by input parameter **mode** : PTIMER_SET or PTIMER_RESET. PTIMER_SET mode sets 
* delay starting from instance when this function is called. While in PTIMER_RESET mode
* it sets delay starting from previous expiry of process ptimer.
*/
void ptimer_set_reset(struct process *p, float delay, mos_uint8_t mode);

/** 
* \brief				This fuctions checks if a ptimer has expired.
* \param p   			A pointer to the process
* \return mos_uint8_t	Returns one if the ptimer has expired, zero otherwise.
*
*/
mos_uint8_t ptimer_expired(struct process *p);

/**
* \name Declaration
* @{
*/
/// This macro makes ptimer process global, i.e. process is visible to all processes.
PROCESS_GLOBAL(ptimer_process);
///@}

#endif /* PTIMER_H_ */