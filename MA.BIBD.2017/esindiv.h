/*
 * NOMBRE:    esindiv.h
 * AUTOR:     Carlos Cotta
 * FECHA:     08-03-99
 * PROPOSITO: Fichero de cabecera del modulo de manejo de individuos.
 *
 */
 
#ifndef __ES_INDIVIDUAL_H__
#define __ES_INDIVIDUAL_H__



/*----------------*\
|      Tipos       |
\*----------------*/
 
typedef int Variable; 	/* variables que evolucionan */

typedef double ValorFitness;	/* para medir la aptitud de una solucion */

typedef struct {
	Variable*    genoma;	/* variables que se desea optimizar */
	int          longen;	/* numero de variables */
	ValorFitness fitness;	/* adecuacion del individuo */
	void*        Phenotype; /* Para el fenotipo */
} Individuo;



/*----------------*\
|    Funciones     |
\*----------------*/



/*
 * Crea un individuo. Se le indica la longitud del genotipo.
 *
 */
Individuo* CrearIndividuo (int longen);


/* 
 * Libera la memoria ocupada por un individuo.
 *
 */
void DestruirIndividuo (Individuo* i);


/* 
 * Copia un individuo.
 *
 */
void CopiarIndividuo (Individuo* dest, Individuo* orig);


/*
 *  Cambia el numero de variables contenidas en un individuo.
 *
 */
void ModificarNumeroVariablesIndividuo (Individuo* ind, int longen);


/*
 * Devuelve el fitness de un individuo.
 *
 */
#define ObtenerFitness(i) ((i)->fitness)

/*
 * Devuelve el gen i-esimo de un individuo.
 *
 */
#define ObtenerGen(ind, i) ((ind)->genoma[(i)])

/*
 * Asigna el gen i-esimo de un individuo.
 *
 */
#define AsignarGen(ind, i, v) (ind)->genoma[(i)] = v

#endif
