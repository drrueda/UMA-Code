/*
 * NOMBRE:    esreemp.h
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: Fichero de cabecera del modulo para el reemplazo de la poblacion.
 *
 */
 
#ifndef __ES_REEMP_H__
#define __ES_REEMP_H__

#include "es.h"



/*----------------*\
|      Macros      |
\*----------------*/

#define Mejor(i1,i2,s) ((s==MAXIMIZAR)?((i1)>(i2)):((i1)<(i2)))


/*----------------*\
|    Funciones     |
\*----------------*/

/*
 * Ordena la poblacion que se le indica.
 *
 */
void OrdenarPoblacion (int* indices, Poblacion pop, SentidoOpt s);


/*
 * Sorts the population and updates the best individual created if necessary.
 * Returns TRUE if the best individual has been updated.
 *
 */
int SortAndUpdateBest (int* indices, Poblacion pop, SentidoOpt s, Individuo* i);


/*
 * Reemplaza la poblacion.
 *
 */
void Reemplazar (ES mi_es);


#endif
