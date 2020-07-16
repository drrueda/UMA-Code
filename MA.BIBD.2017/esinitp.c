/*
 * NOMBRE:    esinitp.c
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: modulo para la creacion de la poblacion inicial.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */

#include <stdio.h>   	/* para printf */
#include <stdlib.h>  	/* para exit */
#include <values.h>
#include "es.h"
#include "esinitp.h"
#include "util.h"	/* TRUE, FALSE, MAXFLOAT */ 
#include "random.h"	/* para URand01 */
#include "esprob.h"
#include "esreemp.h"
#include "esindiv.h"
#include "espop.h"

#ifdef __PARALELIZACION_GLOBAL__
#include "esthread.h"
#endif



/*----------------*\ 
|    Funciones     | 
\*----------------*/ 


/*
 * Genera la poblacion inicial.
 * 	ORTOGONAL    : asumiendo ortogonalidad.
 *      NO_ORTOGONAL : dependiente del problema. 
 *
 */
void GeneracionInicial (ES mi_es)
{
   int i, j;	/* para bucles */
   Individuo* nuevo;


#ifdef __PARALELIZACION_GLOBAL__
   
   /*
    * Los argumentos de las hebras no se inicializan con los individuos
    * de la poblacion inicial porque todavia no han sido creados.
    * (ANTONIO BUENO MOLINA)  v.JAN/2000
    */

   InitEvalArgs (mi_es->eval, NULL);

   StartEvaluation (mi_es->eval);

#endif

   for (i=0; i<GetPopulationSize(mi_es); i++) {
	nuevo = ObtenerIndividuo(mi_es->pop, i);
	if (mi_es->injected[i]) 
   	    	mi_es->injected[i] = FALSE;
	else {
		if (!nuevo) {
				nuevo = CrearIndividuo (mi_es->longen);
				InsertarIndividuo (mi_es->pop, nuevo, i);
		}

		switch (mi_es->inicPob) 
		{ 
		   case ORTOGONAL: 
			/* 
			 * Crear los individuos con los limites iniciales.
			 * (ANTONIO BUENO MOLINA)  v.DEC/1999
			 */
			for (j=0; j<mi_es->longen; j++) { 
				AsignarGen (nuevo, j, (int)URand( mi_es->varLinSup[j]) ); 	
			} 
			break; 

		   case NO_ORTOGONAL: 
			GenerarIndividuo (nuevo, i, mi_es->infoProblem); 
			break; 
		
		   default: 
			printf ("\nERROR: tipo de inicializacion desconocido.\n"); 
			exit(1); 
		} 
			

#ifdef __PARALELIZACION_GLOBAL__
	
	/* Antes de evaluar el individuo hay que inicializarlo en
	 * el argumento de las hebras de evaluacion, ya que hasta
	 * ahora no se ha creado el individuo.
	 * (ANTONIO BUENO MOLINA)  v.JAN/2000
	 */
			SetIndivToEvalArg (mi_es->eval, i, nuevo);
			EvalIndividualPRLL(mi_es->eval, i);
			
#else
			Evaluar(nuevo, mi_es);//->infoProblem);
#endif

		}
	}


#ifdef __PARALELIZACION_GLOBAL__

	WaitEvaluation (mi_es->eval);
	
	/*
	 * Actualizar los argumentos de evaluacion con los offspring.
	 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 	 */

	InitEvalArgs (mi_es->eval, mi_es->offspring);

#endif

	/* Resetear generacion y desiertos. */ 

	mi_es->mejorUpd = FALSE; 
	mi_es->upgr = 0; 
	mi_es->generados = 0; 
	mi_es->TotalEval=0;
	mi_es->desierto = 0; 
 
 
	switch(mi_es->sOpt) { 
		case MAXIMIZAR: 
			mi_es->mejor->fitness = -INFINITO; 
			break; 
		case MINIMIZAR: 
			mi_es->mejor->fitness = INFINITO; 
			break; 
		default: 
			printf ("\nERROR: optimizacion desconocida (%d)\n.", mi_es->sOpt); 
			exit(1); 
	} 

}



/*
 * Inyecta un individuo en la poblacion. Si hay una posicion vacia la situa
 * en ella, y devuelve el indice de la posicion. En otro caso, emplea
 * la posicion pos (si force != 0) y devuelve pos. Si no, devuelve -1.
 *
 */
int InjectIndividual (ES mi_es, Individuo* ind, int force, int pos)
{
	int i;
	Individuo* old;
	
	for (i=0; i<GetPopulationSize(mi_es); i++)
		if (!ObtenerIndividuo(mi_es->pop, i)) {
			InsertarIndividuo(mi_es->pop, ind, i);
			mi_es->injected[i] = TRUE;
			return i;
		}
		
	if (force) {
		old = ObtenerIndividuo(mi_es->pop, pos);
		CopiarIndividuo (old, ind);
		mi_es->injected[pos] = TRUE;
		return pos;
	}
	else
		return -1;
}

