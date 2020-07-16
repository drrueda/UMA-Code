/*
 * NOMBRE:    random.c
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: funciones relativas a numeros aleatorios.
 *
 */


#include <stdio.h>              /*printf*/
#include <math.h>
#include "util.h"		/* PI */
#include "random.h"
#include <float.h>              /*FLT_MIN*/


/*------------------------------- *\
|             Funciones            |
\*--------------------------------*/

/*
 * Devuelve un numero aleatorio que sigue una distribucion normal.
 *
 */
double NRand (double mean, double sigma)
{
	double v = URand01()+FLT_MIN;      /* +FLT_MIN para que no sea cero */
	double u = URand01()+FLT_MIN;

	double n = sqrt(-2.0*log(v))*sin(2*PI*u);

	n*= sigma;
	n+= mean;

	return n;

}

/*
 *  Genera una permutación de los números 0..n-1.
 *
 */
int* RandPerm (int n)
{
	int* perm;
	int i;
	int k;
    int temp;

	perm = (int*) malloc (sizeof (int)*n);
	if (!perm) {
		printf ("\nERROR: no hay memoria para generar permutacion.\n");
		exit (1);
	}
	for (i=0; i<n; i++) {
		perm [i] = i;
	}
	for (i=0; i<n; i++) {
		k = URand(n-i) + i;
        temp = perm[i];
		perm[i] = perm[k];
		perm[k] = temp;
	}

	return perm;
}


/*
 * Genera un numero de una distribucion binomial.
 *
 */
int BinomialRand(double p, int n){
/*BEGIN*/
   int num, cont;

   num=0;	
   for (cont=0; cont<n; cont++){
      if (URand01() < p){
         num++;
      }/*if*/
   }/*for*/
   return num;
}/*END*/





