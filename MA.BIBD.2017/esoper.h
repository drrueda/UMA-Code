/*
 * NOMBRE:    esinitp.h
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: Fichero de cabecera del modulo con operadores evolutivos.
 *
 */
 
#ifndef __ES_OPER_H__
#define __ES_OPER_H__

#include "es.h"


/*----------------*\
|    Funciones     |
\*----------------*/

/*
 * Muta un individuo.
 *
 */
void Mutar (Individuo* i, ES mi_es);

/*
 * Muta un individuo segun xx...
 *
 */
void Mutarxx(Individuo* ind, ES mi_es);

/*
 * Cruza un numero variable de individuos.
 *
 */
void Cruzar (Individuo* hijo, Poblacion padres, ES mi_es);


/*
 * Realiza un cruce uniforme.
 *
 */
void CruceUniforme (Individuo* hijo, Poblacion padres, ES mi_es);

/*
 * Realiza un cruce SPX.
 *
 */
void CruceSPX (Individuo* hijo, Poblacion padres, ES mi_es);

/*
 * Realiza un cruce DPX.
 *
 */
void CruceDPX (Individuo* hijo, Poblacion padres, ES mi_es);



#endif
