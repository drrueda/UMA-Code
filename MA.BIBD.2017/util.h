/*
 * NOMBRE:    util.h
 * AUTOR:     Carlos Cotta
 * FECHA:     26-11-98
 * PROPOSITO: Fichero de cabecera con funciones de utilidad.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */


#ifndef __UTIL_H__
#define __UTIL_H__

#include <math.h>    /* para floor */ 
#include <float.h>   /* para FLT_MAX */


/*
 * (ANTONIO BUENO MOLINA)  v.JAN/2000
 * Nueva Macro:
 *		MAX(x,y) : devuelve el maximo.
 */


/*------------------------------- *\
|           Constantes             |
\*--------------------------------*/

#define PRIMO_GRANDE 9973 
#define TRUE         1
#define FALSE        0
#ifndef PI
#define PI           3.141593
#endif
#define EXP1         2.718282
#ifndef INFINITO
#define INFINITO     FLT_MAX
#endif




/*------------------------------- *\
|             Macros               |
\*--------------------------------*/

#define ABS(x)   ((x)>0?(x):(-(x)))
#define SGN(x)   ((x)>0?1:((x)<0?-1:0))
#define SQR(x)   ((x) * (x))
#define FRAC(x)  ((x) - floor(x))
#define MIN(x,y) ((y)>(x)?(x):(y))
#define MAX(x,y) ((y)<(x)?(x):(y))

#ifdef __TCPP__
	/* Declaraciones para mi PC */
#include <conio.h>	/* para getch */
#define LeeTecla() getch()
#else
#define LeeTecla() ;
#endif

/*------------------------------- *\
|           Funciones              |
\*--------------------------------*/

long IntPow (int a, int n);

#endif
