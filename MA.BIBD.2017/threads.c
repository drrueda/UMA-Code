/* 
 * NOMBRE:    threads.c 
 * AUTOR:     Carlos Cotta 
 * FECHA:     1-01-2000 
 * PROPOSITO: funciones basicas para el manejo de hebras. 
 * 
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */ 


#if defined(__PARALELIZACION_GLOBAL__) || defined(__PARALELIZACION_TAREAS__)


#include <stdio.h>     		/* printf */
#include <stdlib.h>		/* malloc, free */
#include "threads.h"


/*
 * Create a Barrier to wait 'num' threads and initilize it.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
Barrier BarrierCreate (int num)
{
  Barrier my_barrier;

  my_barrier = (Barrier) malloc (sizeof(BarrierInfo));
  if (!my_barrier) {
	printf ("\nERROR: Not enought memory to create the Barrier.\n");
	exit(1);
  }
  
  pthread_mutex_init(&(my_barrier->mutex), NULL);
  pthread_cond_init(&(my_barrier->cond), NULL);
  my_barrier->numtowait = num;
  my_barrier->numact = 0;
  
  return my_barrier;
}


/*
 * Initialize the barrier to wait newnum threads.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void BarrierInit (Barrier my_barrier, int newnum)
{
   pthread_mutex_lock(&(my_barrier->mutex)); 
   
   my_barrier->numtowait = newnum;
   my_barrier->numact    = 0;

   pthread_mutex_unlock(&(my_barrier->mutex)); 
}


/*
 * Reset the barrier to nothing threads received.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void BarrierReset (Barrier my_barrier)
{
   pthread_mutex_lock(&(my_barrier->mutex)); 

   my_barrier->numact = 0;

   pthread_mutex_unlock(&(my_barrier->mutex)); 
}


/*
 * Destroy a Barrier.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void BarrierDestroy (Barrier my_barrier)
{
  pthread_mutex_destroy(&(my_barrier->mutex));
  pthread_cond_destroy(&(my_barrier->cond));
  free(my_barrier);
}


/*
 * Send a signal to a Barrier.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void BarrierSignal (Barrier my_barrier)
{
  pthread_mutex_lock(&(my_barrier->mutex));
  my_barrier->numact++;
  pthread_mutex_unlock(&(my_barrier->mutex));

  pthread_cond_signal(&(my_barrier->cond));
}


/*
 * Wait the threads.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void BarrierWait (Barrier my_barrier)
{
  pthread_mutex_lock(&(my_barrier->mutex));

  while (my_barrier->numact < my_barrier->numtowait) {
	pthread_cond_wait(&(my_barrier->cond), &(my_barrier->mutex));
  }

  pthread_mutex_unlock(&(my_barrier->mutex));
}


#endif  /* (__PARALELIZACION_GLOBAL__) || (__PARALELIZACION_TAREAS__) */
