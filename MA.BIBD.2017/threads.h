/* 
 * NOMBRE:    threads.h 
 * AUTOR:     Carlos Cotta 
 * FECHA:     1-01-2000 
 * PROPOSITO: funciones basicas para el manejo de hebras. 
 * 
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */ 

#ifndef __THREADS_H__
#define __THREADS_H__


#if defined(__PARALELIZACION_GLOBAL__) || defined(__PARALELIZACION_TAREAS__)

#ifdef __LINUX__
#include <pthread/mit/pthread.h>	/* Posix Threads */
#else
#include <pthread.h>
#endif


/* * * * * * * * * * * * * * * *   TYPES   * * * * * * * * * * * * * * * * * */

typedef struct {
		  pthread_mutex_t  mutex;	/* Mutex for the Barrier */
		  pthread_cond_t   cond;	/* Condition Variable */
		  int		   numtowait;	/* Number os threads to wait */
		  int		   numact;	/* Number of threads received */
} BarrierInfo;

	   

typedef pthread_t	  Thread;
typedef pthread_mutex_t   Mutex;
typedef pthread_cond_t	  Cond;
typedef BarrierInfo*      Barrier;	



/* * * * * * * * * * * * * * * *  THREADS  * * * * * * * * * * * * * * * * * */

/* 
 * Create a new thread. 
 *    args:   Thread* ID, void*(*func)(void*), void* arg;
 */ 
#define ThreadCreate(ID,func,arg)	pthread_create(ID,NULL,func,arg)

/*
 * Finish the execution of a thread and return 'retvalue'.
 *     arg:  	void* retvalue;   (the returned value for the thread)
 */
#define ThreadExit(retvalue)		pthread_exit(retvalue)

/*
 * Wait the returned value of a thread.
 *    args:	Thread  ID, void* retvalue;
 */
#define ThreadJoin(ID,retvalue)		pthread_join(ID,retvalue)

/*
 * Finish the execution of a thread.
 *     arg:  	Thread  ID;
 */
#define ThreadDetach(ID)		pthread_detach(ID)

/*
 * Return de Thread ID.
 *
 */
#define ThreadSelfID()			pthread_self()



/* * * * * * * * * * * * * * * *   MUTEX   * * * * * * * * * * * * * * * * * */

/*
 * Initialize a Mutex with default values.
 *     arg: 	Mutex* mutex;
 */
#define MutexInit(mutex)		pthread_mutex_init(mutex,NULL)

/*
 * Destroy a Mutex.
 *     arg:	Mutex* mutex;
 */
#define MutexDestroy(mutex)		pthread_mutex_destroy(mutex)

/*
 * Lock a Mutex. It waits if the mutex if locked.
 *     arg:	Mutex* mutex;
 */
#define MutexLock(mutex)		pthread_mutex_lock(mutex)

/*
 * Unlock a Mutex.
 *     arg: 	Mutex* mutex;
 */
#define MutexUnlock(mutex)		pthread_mutex_unlock(mutex)

/* 
 * Try to Lock a Mutex. It does not wait if the mutex is locked.
 *     arg:	Mutex* mutex;
 */
#define MutexTryLock(mutex)		pthread_mutex_trylock(mutex)



/* * * * * * * * * * * *    CONDITION VARIABLES    * * * * * * * * * * * * * */

/*
 * Initialize the condition variable with default values.
 *     arg: 	Cond* cond;
 */
#define CondInit(cond)			pthread_cond_init(cond,NULL)

/*
 * Destroy a condition variable.
 *     arg:  	Cond* cond;
 */
#define CondDestroy(cond)		pthread_cond_destroy(cond)

/*
 * Send a signal to a condition variable. Wake up one waiting thread.
 *    arg: 	Cond* cond;
 */
#define CondSignal(cond)		pthread_cond_signal(cond)

/*
 * Send a signal to a condition variable. Wake up all waiting threads.
 *     arg:	Cond* cond;
 */
#define CondBroadcast(cond)		pthread_cond_broadcast(cond)

/*
 * Wait a signal. Sleep until the signal is received.
 *    args: 	Cond* cond, Mutex* mutex;
 */
#define CondWait(cond,mutex)		pthread_cond_wait(cond,mutex)



/* * * * * * * * * * * * * * *    BARRIER   * * * * * * * * * * * * * * * * */ 

/*
 * Create a Barrier to wait 'num' threads and initilize it.
 *
 */
Barrier BarrierCreate (int num);

/*
 * Initialize the barrier to wait newnum threads.
 *
 */
void BarrierInit (Barrier barrier, int newnum);

/*
 * Reset the barrier to nothing threads received.
 *
 */
void BarrierReset (Barrier barrier);

/*
 * Destroy a Barrier.
 *
 */
void BarrierDestroy (Barrier my_barrier);

/*
 * Send a signal to a Barrier.
 *
 */
void BarrierSignal (Barrier my_barrier);

/*
 * Wait the threads.
 *
 */
void BarrierWait (Barrier my_barrier);


#endif  /* (__PARALELIZACION_GLOBAL__) || (__PARALELIZACION_TAREAS__) */


#endif

