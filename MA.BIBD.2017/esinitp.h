/*
 * NOMBRE:    esinitp.h
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: Fichero de cabecera del modulo para la creacion de la
 *            poblacion inicial.
 */
 
#ifndef __ES_INIT_POP_H__
#define __ES_INIT_POP_H__

#include "es.h"


/*----------------*\
|    Funciones     |
\*----------------*/

/*
 * Genera la poblacion inicial. Se encarga de resetear generados y desierto.
 *
 */
void GeneracionInicial (ES mi_es);

/*
 * Inyecta un individuo en la poblacion. Si hay una posicion vacia la situa
 * en ella, y devuelve el indice de la posicion. En otro caso, emplea
 * la posicion pos (si force != 0) y devuelve pos. Si no, devuelve -1.
 *
 */
int InjectIndividual (ES mi_es, Individuo* ind, int force, int pos);

#endif
