/*
 * NOMBRE:    esshell.c
 * AUTOR:     Carlos Cotta
 * FECHA:     4-02-99
 * PROPOSITO: Frontend para el uso de ES.
 *
 */
 

#include "es.h"
#include "esindiv.h"
#include "esinitp.h"
#include "esstat.h"
#include "esselect.h"
#include "esreprod.h"
#include "esreemp.h"
#include "esshell.h"



/*----------------*\
|    Funciones     |
\*----------------*/

/*
 * Lleva a cabo un ciclo del algoritmo.
 * Devuelve TRUE si se ha llegado a la terminacion.
 *
 */
TerminationStatus StepUp (ES mi_es)
{
/*	int i;
	int j;
	int k;
	double f;

	printf ("%d %ld %f --->", GetID(mi_es), mi_es->generados,mi_es->mejor->fitness);
 	
	f = 0;
	for (i=0; i<GetPopulationSize(mi_es); i++)  {
		for (j=0; j<mi_es->longen; j++)
			mi_es->pop->inds[i]->genoma[j] += 1.0;
		mi_es->pop->inds[i]->fitness = mi_es->pop->inds[i]->genoma[0];
		if (mi_es->pop->inds[i]->fitness > f) {
			f = mi_es->pop->inds[i]->fitness;
			k = i;
		}
	}
	mi_es->generados+=1;
	CopiarIndividuo(mi_es->mejor, mi_es->pop->inds[k]);
	printf ("%d %ld %f\n", mi_es->ID, mi_es->generados, f);
	return mi_es->generados>1;	
*/
	
	GenerarEstadisticas (mi_es);
	
	Seleccionar (mi_es);
	
	CrearNuevos (mi_es);
	
	Reemplazar  (mi_es);
	
	return Terminacion (mi_es);	
}


/*
 * Ejecuta el algoritmo hasta su terminacion.
 *
 */
void RunES (ES mi_es)
{
	GeneracionInicial (mi_es);
	RefreshES(mi_es);
	while (!StepUp(mi_es));
	GenerarEstadisticas (mi_es);
}

/*
 * Devuelve la mejor solucion encontrada por el algoritmo.
 *
 */
Variable* BestSolutionES (ES mi_es)
{
	return mi_es->mejor->genoma;
}


/*
 * Devuelve el mejor individuo encontrado por el algoritmo.
 *
 */
Individuo* BestIndividualOfES (ES mi_es)
{
	return mi_es->mejor;
}


/*
 * (ANTONIO BUENO MOLINA)  v.NOV/1999
 * Return the individual i in the sorted list of 
Individuals.
 *
 */
Individuo* GetSortedIndividual (ES mi_es, int i)
{
  int k;
  k = mi_es->ordpop[i]; 
  return mi_es->pop->inds[k];
} 

