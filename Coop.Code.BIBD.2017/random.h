/* 
 * NOMBRE:    random.h 
 * AUTOR:     Carlos Cotta 
 * FECHA:     26-11-98 
 * PROPOSITO: Fichero de cabecera con funciones de numeros aleatorios. 
 * 
 */ 

#ifndef __RANDOM_H__ 
#define __RANDOM_H__ 

#include <stdlib.h>  /* para random, srand (PC), srandom (SUN)*/ 
#include <float.h>   /* para FLT_EPSILON */

#define IniciarAleatorios(s) srand(s); 
#define URand01()   ((double)rand()/(double)RAND_MAX)
#define URand(x)    ((int)(URand01()*(double)(x) - FLT_EPSILON))


/*------------------------------- *\ 
|             Funciones            | 
\*--------------------------------*/ 
 
/* 
 * Devuelve un numero aleatorio que sigue una distribucion normal. 
 * 
 */ 
double NRand (double mean, double sigma); 


/*
 *  Genera una permutaci�n de los n�meros 0..n-1.
 *
 */
int* RandPerm (int n);

/*
 * Genera un numero de una distribucion binomial.
 *
 */
int BinomialRand(double p, int n);


#endif
