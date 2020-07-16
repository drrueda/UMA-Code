/*
 * NOMBRE:    esio.h
 * AUTOR:     Carlos Cotta
 * FECHA:     4-11-98
 * PROPOSITO: fichero de cabecera del modulo de entrada/salida.
 *
 */


#ifndef __ES_IO_H__
#define __ES_IO_H__

#include <stdio.h>  /* para FILE */
#include "es.h"

void EscribirCabecera (void);
/* Muestra una cabecera al iniciar el algoritmo. */

/*
 * Escribe los parametros de forma abreviada.
 *
 */
void WriteESParametersInShortForm (ES mi_es);

void EscribirParametros (ES mi_es);
/* Muestra en pantalla los parametros generales del algoritmo, 
   asi como aquellos dependientes del problema. */

void EscribirMejorFitness (FILE* f, ES mi_es);
/* Muestra el mejor fitness generado */

/*
 * Muestra el fitness, las variables y las varianzas del mejor individuo.
 */
void EscribirIndividuo (FILE* f, Individuo* i);


#endif
