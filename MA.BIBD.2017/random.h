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


#ifdef __TCPP__ 
	/* Declaraciones para mi PC */ 
#define IniciarAleatorios(s) srand(s); 
#define URand01()   ((double)rand()/(double)RAND_MAX)
#define URand(x)    ((int)(URand01()*(double)(x) - FLT_EPSILON))

#else 
#include <values.h>  /* para MAXINT */ 
#ifdef __SOLARIS__ 
	/* Declaraciones para Solaris */ 
#define IniciarAleatorios(s)  srand48((long)(s)); 
#define URand01()            (drand48()) 
#define URand(x)             (unsigned)(drand48()*(x)) 

#else 
	/* Declaraciones para la SUN */ 
#define IniciarAleatorios(s) srandom(s); 
#define URand01()            ((double)random()/(double)MAXINT) 
#define URand(x)             (unsigned)(((double)random()/(double)MAXINT)*(double)(x)) 
#endif 

#endif


/*------------------------------- *\ 
|             Funciones            | 
\*--------------------------------*/ 
 
/* 
 * Devuelve un numero aleatorio que sigue una distribucion normal. 
 * 
 */ 
double NRand (double mean, double sigma); 


/*
 *  Genera una permutación de los números 0..n-1.
 *
 */
int* RandPerm (int n);

/*
 * Genera un numero de una distribucion binomial.
 *
 */
int BinomialRand(double p, int n);


#endif
