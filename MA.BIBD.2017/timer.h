/*
 * NAME:    timer.h
 * AUTHOR:  Carlos Cotta
 * DATE:    17-12-99
 * PURPOSE: Header file for timer structures.
 *
 */
 
#ifndef __TIMER_H__
#define __TIMER_H__




/*----------------*\
|      Types       |
\*----------------*/

#if defined(__SOLARIS__) || defined(__DIGITAL__)
#include <time.h>
#define TimeType struct timeval
#else 
#if defined(__TCPP__)
#include <time.h>
#define TimeType clock_t
#endif
#endif

typedef enum {
	STOPPED = 0,
	RUNNING = 1,
	PAUSED  = 2
} TimerStatus;

typedef struct {
	TimeType    beginTime;	 /* time at which the timer was started */
	double      elapsedTime; /* time elapsed since last start */
	TimerStatus status;	 /* status of the timer */
} Timer;	



/*----------------*\
|    Functions     |
\*----------------*/


/*
 *  Resets a timer, i.e., stops it if it was running and
 * sets the elapsed time to 0.
 *
 */
void ResetTimer (Timer* t);

/*
 *  Starts the timer. If it was stopped, the elapsed time
 * is reset to 0. Otherwise (it was paused) the elapsed time
 * is retained.
 *
 */
void StartTimer (Timer* t);


/*
 *  Pauses a timer. Any further call to GetElapsedTime will
 * return the total time measured since it was started. The
 * timer can be further resumed by calling StartTimer.
 *
 */
void PauseTimer (Timer* t);



/*
 *  Stops a timer. This function is similar to PauseTimer,
 * with the diference that the timer cannot be resumed.
 *
 */
void StopTimer (Timer* t);


/*
 *  Gets the elapsed time. This function works in any status
 * of the timer but notice that this function requires some
 * time and, if the timer is running, the final measure will 
 * include it.
 *
 */
double GetElapsedTime (Timer *t);

#endif
