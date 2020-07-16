/*
 * NAME:    timer.c
 * AUTHOR:  Carlos Cotta
 * DATE:    17-12-99
 * PURPOSE: Module for timer structures.
 *
 */
 

#include "timer.h"


/*----------------*\
|    Functions     |
\*----------------*/


#if defined(__SOLARIS__) || defined(__DIGITAL__)
#define GetTimeFunction(t)   gettimeofday(&(t), NULL)
#define TimeDifference(t1,t0) (t1.tv_sec-t0.tv_sec+((t1.tv_usec-t0.tv_usec)/1000000.0))
#else
#if defined(__TCPP__)
#define GetTimeFunction(t)   t=clock()
#define TimeDifference(t1,t0) ((double)(t1-t0)/(double)CLK_TCK)
#endif
#endif


/*
 *  Resets a timer, i.e., stops it if it was running and
 * sets the elapsed time to 0.
 *
 */
void ResetTimer (Timer* t)
{
	t->status = STOPPED;
	t->elapsedTime = 0.0;
}

/*
 *  Starts the timer. If it was stopped, the elapsed time
 * is reset to 0. Otherwise (it was paused) the elapsed time
 * is retained.
 *
 */
void StartTimer (Timer* t)
{
	if (t->status == STOPPED)
		t->elapsedTime = 0.0;
	t->status = RUNNING;
	GetTimeFunction(t->beginTime);
}




/*
 *  Pauses a timer. Any further call to GetElapsedTime will
 * return the total time measured since it was started. The
 * timer can be further resumed by calling StartTimer.
 *
 */
void PauseTimer (Timer* t)
{
	TimeType timeNow;
	
	GetTimeFunction(timeNow);
	t->elapsedTime += TimeDifference(timeNow, t->beginTime);
	t->status = PAUSED;
}




/*
 *  Stops a timer. This function is similar to PauseTimer,
 * with the diference that the timer cannot be resumed.
 *
 */
void StopTimer (Timer* t)
{
	TimeType timeNow;
	
	GetTimeFunction(timeNow);
	t->elapsedTime += TimeDifference(timeNow, t->beginTime);
	t->status = STOPPED;
}



/*
 *  Gets the elapsed time. This function works in any status
 * of the timer but notice that this function requires some
 * time and, if the timer is running, the final measure will 
 * include it.
 *
 */
double GetElapsedTime(Timer *t)
{
	if (t->status == RUNNING) {
		TimeType timeNow;
	
		GetTimeFunction(timeNow);
		return (t->elapsedTime + TimeDifference(timeNow, t->beginTime));
	}
	
	return t->elapsedTime;
}
