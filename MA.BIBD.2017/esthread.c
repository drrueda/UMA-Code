/* 
 * NOMBRE:    esthread.c
 * AUTOR:     Carlos Cotta 
 * FECHA:     11-1-2000 
 * PROPOSITO: funciones para la evaluacion de individuos usando hebras. 
 * 
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */

#ifdef __PARALELIZACION_GLOBAL__


#include <stdlib.h>		/* free, malloc */
#include "esthread.h"
#include "espop.h"		/* Poblacion */
#include "esprob.h"		/* Evaluar */
#include "threads.h"		/* BarrierSignal, ThreadCreate */
#include "util.h"		/* TRUE, FALSE */



/*
 * Threat that evaluate an individual.  
 * When the evaluation finish the thread is Destroyed.
 * if define __PRv2__ : 
 * When the thread finish the evaluation is keeping Slept for the 
 * next evaluation, and when is destroyed free its argument.
 */
void* IndividualThread (void* my_arg)
{
   Thread  myID;
   EvalIndivArg arg = (EvalIndivArg) my_arg;

#ifdef __PGv2__  
 
   while(1) {
		MutexLock(&arg->mutex);
		if (arg->flagend) {
			free(arg);
			break;
		}
		Evaluar(arg->indiv);
   		BarrierSignal(arg->barrier); 
   }
   
#else

   Evaluar(arg->indiv);
   BarrierSignal(arg->barrier);

#endif

   myID = ThreadSelfID();
   ThreadDetach(myID);

   return NULL;
}



/*
 * Create the arguments of the threads.
 *
 */
EvalArg* CreateEvalArgs (int numargs)
{
   int i;
   EvalArg*  neweval;

   neweval = (EvalArg*) malloc (sizeof(EvalArg));
   if (!neweval) {
	printf("\nERROR: Not enought memory for Eval Arguments.\n");
	exit(1);
   }

   neweval->args = (EvalIndivArg*) malloc (numargs*sizeof(EvalIndivArg));
   if (!(neweval->args)) {
	printf("\nERROR: Not enoght memory for Eval Arguments.\n"); 
	exit(1);
   }

   for (i=0; i<numargs; i++) {
	 neweval->args[i] = (EvalIndivArg) malloc (sizeof(EvalIndivInfo));
	 if (!(neweval->args[i])) {
	 	printf("\nERROR: Not enought memory for Individuals Arguments.\n");
		exit(1);
	 }
   }

   neweval->numarg  = numargs;
   neweval->barrier = BarrierCreate(numargs);

   return neweval;
}


/*
 * Destroy the arguments of the threads.
 * if define __PRv2__
 *    Destroy all threads created for evaluate the individuals that are
 * slept, and the threads free the arguments when are destroyed.
 *
 */
void DestroyEvalArgs (EvalArg* evalarg)
{
   int i;

   for (i=0; i<evalarg->numarg; i++) {

#ifdef __PGv2__
	  evalarg->args[i]->flagend = TRUE;
	  MutexUnlock(&(evalarg->args[i]->mutex));   
#else
	  free(evalarg->args[i]);
#endif
   }

   free(evalarg->args);
   BarrierDestroy(evalarg->barrier);
   free(evalarg);
}


/*
 * Initialize the arguments use for the threads that evaluate individuals,
 * the function adjust the number of arguments to the number of individuals.
 * If Population is NULL then initialize only the Barrier.
 * if define __PGv2__ 
 *	      Initialize and lock the mutex and create the threads. 
 */
void InitEvalArgs (EvalArg* evalarg, Poblacion pop)
{
  int i;
  int popsize;
  int inicio, fin;
  int numargs = evalarg->numarg;

#ifdef __PGv2__
  Mutex* argmutex;
#endif


  if (pop) {	
      popsize = Size(pop);
      if (numargs != popsize) {

#ifdef __PGv2__
	 /* 
	  * If popsize < numargs there are to destroy the threads 
	  * that won't be used.
	  */
	 for(i=popsize; i<numargs; i++) {
	 	evalarg->args[i]->flagend = TRUE;
		MutexUnlock(&(evalarg->args[i]->mutex));
	 }	
#endif
	 evalarg->args = (EvalIndivArg*) realloc (evalarg->args, popsize*sizeof(EvalIndivArg));
	 if (!(evalarg->args)) {
	      printf("\nERROR: Not enought memory to change Eval Arguments.");
	      exit(1);
	 }

	 /*
          * if popsize > numargs then create the new arguments.
 	  */
	 for (i=numargs; i<popsize; i++) {
	      evalarg->args[i] = (EvalIndivArg) malloc (sizeof(EvalIndivInfo));
	      if (!(evalarg->args[i])) {
	          printf("\nERROR: Not enought memory for Individuals Arguments.\n");
		  exit(1);
	      }
   	 }
         BarrierInit(evalarg->barrier, popsize);
      }
      for (i=0; i<popsize; i++) {
	    evalarg->args[i]->indiv = ObtenerIndividuo(pop,i);
      }
      inicio = numargs;
      fin = popsize;
  }
  else {
	inicio = 0;
	fin = numargs;	
  }   

  for (i=inicio; i<fin; i++) 
  {
	evalarg->args[i]->barrier = evalarg->barrier;
        
#ifdef __PGv2__
	evalarg->args[i]->flagend = FALSE;
	argmutex = &(evalarg->args[i]->mutex);
	MutexInit(argmutex);
	MutexLock(argmutex);
	EvalArgument(evalarg, i);
#endif
  }

  evalarg->numarg = fin;
 
}


/*
 * Create a Thread for Evaluate an Individual.
 *
 */
void EvalArgument (EvalArg* evalarg, int i)
{
   Thread threadID;
   EvalIndivArg argtoeval = evalarg->args[i];  
   int status; 

   status = ThreadCreate (&threadID, IndividualThread, (void*) argtoeval);
   if (status) {
	printf("\nERROR: Can not create a Thread for Evaluate the Individual.\n");
 	exit(1);
   }
  
}


#endif  /* __PARALELIZACION_GLOBAL__ */
