/*
 * NOMBRE:    espop.h
 * AUTOR:     Carlos Cotta
 * FECHA:     08-03-99
 * PROPOSITO: Fichero de cabecera del modulo de manejo de poblaciones.
 *
 */
 
#ifndef __ES_POP_H__
#define __ES_POP_H__

#include "esindiv.h"

/*----------------*\
|      Tipos       |
\*----------------*/
 
typedef struct {
	Individuo**  inds;	/* los indivuduos */
	int          num;	/* numero de individuos */
} PopulationInfo;	

typedef PopulationInfo* Poblacion;


/*----------------*\
|    Funciones     |
\*----------------*/



/*
 * Crea una poblacion. Si flag != 0, se crean los individuos de la 
 * longitud indicada. De lo contrario, todos los individuos son nulos.
 *
 */
Poblacion CrearPoblacion (int num, int flag, int longen);


/* 
 * Libera la memoria ocupada por una poblacion.
 * Si flag != 0, destruye los individuos tambien.
 *
 */
void DestruirPoblacion (Poblacion pop, int flag);


/*
 *  Cambia el numero de variables contenidas en cada individuo.
 *
 */
void ModificarNumeroVariablesPoblacion (Poblacion pop, int longen);

/*
 *  Devuelve el individuo i-esimo de la poblacion.
 *
 */
#define ObtenerIndividuo(pop,i) (pop)->inds[(i)]
 

/*
 * Inserta un individuo en una posicion de la poblacion.
 *
 */
#define InsertarIndividuo(pop, ind, pos) (pop)->inds[(pos)] = (ind)
 
/*
 * Devuelve el numero de individuos en la poblacion.
 *
 */
#define Size(pop) (pop->num)


/* 
 * Cambia el taman~o de la poblacion.
 *
 */   
Poblacion CambiaTamanoPoblacion (Poblacion pop, int newnum, int longen);


/*
 * (ANTONIO BUENO MOLINA)  v.NOV/1999
 * Set the number of Individuals in a Population.
 */
#define SetNumberOfIndividuals(pop, n) (pop)->num = n


#endif
