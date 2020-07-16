/*
 * NOMBRE:    esoper.c
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: modulo con operadores evolutivos.
 *
 */
 
#include <math.h>
#include "es.h"
#include "esoper.h"
#include "random.h"
#include "esprob.h"

/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Muta un individuo segun RANDOM_SUBSTITUTION.
 *
 */
void MutarRANDOM_SUBSTITUTION(Individuo* ind, ES mi_es){
/*BEGIN*/
	int pos, valor;

	pos = URand(ind->longen);
	do{
		valor =  URand(mi_es->varLSup[pos]);
	}while (valor == ind->genoma[pos]);
	ind->genoma[pos]  = valor;
}/*END*/


/*
 * Muta un individuo.
 *
 */
void Mutar (Individuo* i, ES mi_es){
/*BEGIN*/
   #ifdef __VERBOSE__
      printf ("\ncomienza mutacion...\n");
   #endif

	switch (mi_es->mutacion) {
        case SIN_MUTACION:
             break;
		case RANDOM_SUBSTITUTION: /* RANDOM_SUBSTITUTION */
			MutarRANDOM_SUBSTITUTION (i, mi_es);
			break;
		case PROB_MUTATION: /* mutacion dependiente del problema */
			MutacionProblema (i, mi_es);
			break;
		default:
			printf ("\nERROR: operador de mutacion desconocido (%d).\n", mi_es->mutacion);
			exit(1);
	}/*switch*/

   #ifdef __VERBOSE__
      printf ("\nFinaliza mutacion...\n");
   #endif
}/*END*/




/*
 * Realiza un cruce uniforme. 
 *
 */
void CruceUniforme (Individuo* h1, Poblacion padres, ES mi_es){
/*BEGIN*/ 
	int         i, numParents;
 	Individuo*  p;

	numParents = Size(padres);
	for (i=0; i<mi_es->longen; i++){
		p = ObtenerIndividuo(padres, URand(numParents));
		h1->genoma[i] = p->genoma[i];
	}/*for*/
}/*END*/


/*
 * Realiza un cruce SPX.
 * a a a a a a      a a a  b b b
 * b b b b b b      b b b  a a a
 */
void CruceSPX (Individuo* h1, Poblacion padres, ES mi_es){
/*BEGIN*/ 
	int i,punto_cruce;
	Individuo* p1;
	Individuo* p2;
	
	p1 = ObtenerIndividuo(padres, 0);
	p2 = ObtenerIndividuo(padres, 1);
	punto_cruce = URand(mi_es->longen);

	for (i=0; i<punto_cruce; i++){
		h1->genoma[i] = p1->genoma[i];
	}/*for*/
	for (i=punto_cruce; i<mi_es->longen; i++){
		h1->genoma[i] = p2->genoma[i];
	}/*for*/
}/*END*/

/*
 * Realiza un cruce DPX.
 * a a a a a a      a a  b b  a a     b b  b b  a a  ...
 * b b b b b b      b b  a a  b b     a a  a a  b b  ...
 */
void CruceDPX (Individuo* h1, Poblacion padres, ES mi_es){
/*BEGIN*/
	int i, aux;
	int pto1, pto2;
	Individuo* p1;
	Individuo* p2;

	p1 = ObtenerIndividuo(padres, 0);
	p2 = ObtenerIndividuo(padres, 1);
	pto1 = URand(mi_es->longen);
	do{
		pto2 = URand(mi_es->longen);
	}while (pto1 == pto2);

	if (pto1 > pto2){
		aux = pto1;
                pto1 = pto2;
                pto2 = aux;
	}/*if*/


	for (i=0; i<pto1; i++){
		h1->genoma[i] = p1->genoma[i];
	}/*for*/
	for (i=pto1; i<pto2; i++){
		h1->genoma[i] = p2->genoma[i];
	}/*for*/
	for (i=pto2; i<mi_es->longen; i++){
		h1->genoma[i] = p1->genoma[i];
	}/*for*/
}/*END*/




/*
 * Cruza un numero variable de individuos.
 *
 */
void Cruzar (Individuo* hijo, Poblacion padres, ES mi_es){
/*BEGIN*/
	
	#ifdef __VERBOSE__
		printf ("\nEmpieza cruce...\n");
	#endif

	switch (mi_es->cruce) {
		case UX: /* Uniforme */
			CruceUniforme (hijo, padres, mi_es);
			break;
		case SPX: /* SPX */
			CruceSPX (hijo, padres, mi_es);
			break;
		case DPX: /* DPX */
			CruceDPX (hijo, padres, mi_es);
			break;
		case PROB: /* cruce dependiente del problema */
			CruceProblema (hijo, padres, mi_es);
			break;
		default:
			printf ("\nERROR: operador de cruce desconocido (%d).\n", mi_es->cruce);
			exit(1);
	}/*switch*/
	
#ifdef __VERBOSE__
	printf ("\nFinaliza cruce...\n");
#endif

}/*END*/

