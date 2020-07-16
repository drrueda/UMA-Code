/*
 * NOMBRE:    esindiv.c
 * AUTOR:     Carlos Cotta
 * FECHA:     08-03-99
 * PROPOSITO: funciones basicas de manejo de individuos.
 *
 */
 

#include <stdio.h>   /* para printf */
#include <stdlib.h>  /* para exit, malloc, free, realloc */
#include <string.h>  /* para memcpy */

#include "esprob.h"  /* CopiarFenotipo, DestruirFenotipo */
#include "esindiv.h"


/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Crea un individuo. Se le indica la longitud del genotipo.
 *
 */
Individuo* CrearIndividuo (int longen)
{
	Individuo* i;
	
	i = (Individuo*) malloc (sizeof(Individuo));
	if (!i) {
		printf ("\nERROR: no hay memoria para crear individuo.\n");
		exit(1);
	}
	
	i->genoma = (Variable*) malloc (longen*sizeof(Variable));
	if (!i->genoma) {
		printf ("\nERROR: no hay memoria para %d variables.\n", longen);
		exit(1);
	}
	
	i->longen    = longen;
	i->Phenotype = NULL;
	
	return i;
}
	


/* 
 * Copia un individuo.
 *
 */
void CopiarIndividuo (Individuo* dest, Individuo* orig)
{
	if (!dest->genoma) {
		dest->genoma = (Variable*) malloc (orig->longen*sizeof(Variable));
		if (!dest->genoma) {
			printf ("\nERROR: no hay memoria para variables al copiar.\n");
			exit(1);
		}
	} 
	else  	
		if (dest->longen != orig->longen) {
			dest->genoma = (Variable*) realloc (dest->genoma, orig->longen*sizeof(Variable));
			if (!dest->genoma) {
				printf ("\nERROR: no hay memoria para variables al copiar.\n");
				exit(1);
			}
		}

	memcpy (dest->genoma, orig->genoma, orig->longen*sizeof(Variable));	
	dest->longen = orig->longen;
	dest->fitness = orig->fitness;		

	dest->Phenotype = CopiarFenotipo(dest->Phenotype, orig->Phenotype);
}/*END*/



/* 
 * Libera la memoria ocupada por un individuo.
 *
 */
void DestruirIndividuo (Individuo *i){
/*BEGIN*/
	if (i != NULL) {
		if (i->genoma != NULL){
			free (i->genoma);
		}/*if*/
		if (i->Phenotype != NULL){
			DestruirFenotipo(i->Phenotype);
		}/*if*/
		free (i);
	}/*if*/
}/*END*/




/*
 *  Cambia el numero de variables contenidas en un individuo.
 *
 */
void ModificarNumeroVariablesIndividuo (Individuo* ind, int longen)
{
	ind->genoma = (Variable*) realloc (ind->genoma, longen*sizeof(Variable));
	if (!ind->genoma) {
		printf ("\nERROR: no hay memoria para variables.\n");
		exit(1);
	}		                              
	ind->longen = longen;		
}
 

