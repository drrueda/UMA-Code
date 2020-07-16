/*
 * NOMBRE:    espop.c
 * AUTOR:     Carlos Cotta
 * FECHA:     4-11-98
 * PROPOSITO: funciones basicas de manejo de poblaciones e individuos.
 *
 */
 

#include <stdio.h>   /* para printf */
#include <stdlib.h>  /* para exit, malloc, free, realloc */
#include <string.h>  /* para memcpy */

#include "espop.h"
#include "esindiv.h"


/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Crea una poblacion. Si flag != 0, se crean los individuos de la 
 * longitud indicada. De lo contrario, todos los individuos son nulos.
 *
 */
Poblacion CrearPoblacion (int num, int flag, int longen)
{
	Poblacion pop;
	int j;
	
	pop = (PopulationInfo*) malloc (sizeof(PopulationInfo));
	if (!pop) {
		printf ("\nERROR: not enough memory for population.\n");
		exit(1);
	}
	
	pop->inds = (Individuo**) malloc (num*sizeof(Individuo*));
	if (!pop->inds) {
		printf ("\nERROR: not enough memory for population of %d individuals.\n", num);
		exit(1);
	}
	pop->num = num;
	if (flag) {
		for (j=0; j<num; j++) 
			pop->inds[j] = CrearIndividuo (longen);
	}
	else {
		for (j=0; j<num; j++) 
			pop->inds[j] = NULL;
	}
	return pop;
}




/* 
 * Libera la memoria ocupada por una poblacion.
 * Si flag != 0, destruye los individuos tambien.
 *
 */
void DestruirPoblacion (Poblacion i, int flag)
{
	int j;
	
	if (i) {
		if (flag)
			for (j=0; j<i->num; j++) 
				DestruirIndividuo (i->inds[j]);
		free (i->inds);
		free (i);
	}
}




/*
 *  Cambia el numero de variables contenidas en cada individuo.
 *
 */
void ModificarNumeroVariablesPoblacion (Poblacion pop, int longen)
{
	int i;
	
	for (i=0; i<pop->num; i++) 
		if (pop->inds[i]) 
			ModificarNumeroVariablesIndividuo(pop->inds[i], longen);
}
 

/* 
 * Cambia el taman~o de la poblacion.
 *
 */   
Poblacion CambiaTamanoPoblacion (Poblacion pop, int newnum, int longen)
{
	int i;
	
	if (pop->num != newnum) {
		for (i=newnum; i<pop->num; i++) 	
			DestruirIndividuo (pop->inds[i]);
			
		pop->inds = (Individuo**) realloc (pop->inds, newnum*sizeof(Individuo*));
		if (!pop->inds) {
			printf ("\nERROR: no hay memoria para poblacion.\n");
			exit(1);
		}
		for (i=pop->num; i<newnum; i++) 
			pop->inds[i] = CrearIndividuo (longen);
		pop->num = newnum;
	}
	
	return pop;
}


