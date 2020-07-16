/* 
 * NOMBRE:    esthread.h 
 * AUTOR:     Carlos Cotta 
 * FECHA:     11-1-2000 
 * PROPOSITO: funciones para la evaluacion de individuos usando hebras. 
 * 
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */ 

#ifndef __ES_THREADS_H__
#define __ES_THREADS_H__


#ifdef __PARALELIZACION_GLOBAL__


#include "espop.h"	/* Poblacion */
#include "esindiv.h"    /* Individuo */
#include "threads.h"


/*
 * NOTA: Para usar la version de paralelizacion global que deja las 
 *       hebras de evaluacion dormidas hay que incluir los defines:
 *         __PARALELIZACION_GLOBAL__  y  __PGv2__
 *
 *       Para la version que crea y destruye las hebras cada vez que 
 *       necesita evaluar un individuo incluir solo el define:
 *         __PARALELIZACION_GLOBAL__
 *
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */


/*
 * Argument for the Evaluation Threads.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
typedef struct {
		  Individuo*    indiv;    /* Individual to evaluate */
		  Barrier	barrier;  /* Barrier to send the signal */
#ifdef __PGv2__
		  Mutex		mutex;    /* where the thread waits */
		  int		flagend;  /* use to destroy the threads */
#endif
} EvalIndivInfo;

typedef EvalIndivInfo* EvalIndivArg;

typedef struct {
		   EvalIndivArg*  args;		/* thread arguments */
		   Barrier	  barrier;	/* Barrier to wait */
		   int		  numarg;	/* number of arguments */
} EvalArg;



/*----------------*\ 
|    Funciones     | 
\*----------------*/ 


/*
 * Create the arguments of the threads.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
EvalArg* CreateEvalArgs (int numargs);


/*
 * Destroy the arguments of the threads.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void DestroyEvalArgs (EvalArg* arg);


/*
 * Initialize the arguments use for the threads that evaluate individuals,
 * the function adjust the number of arguments to the number of individuals.
 * If Population is NULL then initialize only the Barrier.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void InitEvalArgs (EvalArg* arg, Poblacion pop);


/*
 * Assign an individual to an argument.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
#define SetIndivToEvalArg(evalarg,i,ind)  (evalarg)->args[(i)]->indiv=(ind)


/*
 * Create a Thread for Evaluate an Individual.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
void EvalArgument (EvalArg* evalarg, int i);


/*
 * Initialize the Barrier. Must be used before the evaluation.
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
#define StartEvaluation(evalarg)	BarrierReset((evalarg)->barrier)


/*
 * Wait all the Evaluation threads.
 * (ANTONIO BUENO MOLINA)  V.JAN/2000
 */
#define WaitEvaluation(evalarg)		BarrierWait((evalarg)->barrier)


/*
 * Evaluate an Individual using threads
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 */
#ifndef __PGv2__
#define EvalIndividualPRLL(evalarg,i)   EvalArgument(evalarg,i)
#else
#define EvalIndividualPRLL(evalarg,i)   MutexUnlock(&((evalarg)->args[(i)]->mutex))
#endif


#endif  /* __PARALELIZACION_GLOBAL__ */


#endif
