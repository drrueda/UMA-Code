/*
 * NOMBRE:    esshell.h
 * AUTOR:     Carlos Cotta
 * FECHA:     4-02-99
 * PROPOSITO: Fichero de cabecera del frontend para el uso de ES.
 *
 */
 
#ifndef __ES_SHELL_H__
#define __ES_SHELL_H__

#include "es.h"
#include "esindiv.h"


/*----------------*\
|    Funciones     |
\*----------------*/

/*
 * Lleva a cabo un ciclo del algoritmo.
 * Devuelve TRUE si se ha llegado a la terminacion.
 *
 */
TerminationStatus StepUp (ES mi_es);



/*
 * Ejecuta el algoritmo hasta su terminacion.
 *
 */
void RunES (ES mi_es);

/*
 * Devuelve la mejor solucion encontrada por el algoritmo.
 *
 */
Variable* BestSolutionES (ES mi_es);


/*
 * Devuelve el mejor individuo encontraoa por el algoritmo.
 *
 */
Individuo* BestIndividualOfES (ES mi_es);


/*
 * (ANTONIO BUENO MOLINA)  v.NOV/1999
 * Return the individual i in the sorted list of  Individuals.
 *
 */
Individuo* GetSortedIndividual (ES mi_es, int i);


#endif
