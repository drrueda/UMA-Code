/*
 * NOMBRE:    esreprod.c
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: modulo para la generacion de la siguiente generacion.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */

#include <stdio.h>
#include <stdlib.h>
#include "es.h"
#include "esreprod.h"
#include "esoper.h"
#include "esprob.h"
#include "random.h"
#include "util.h"

#ifdef __PARALELIZACION_GLOBAL__
#include "esthread.h"		
#endif


/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Recombinacion + Mutacion.
 *
 */
void CicloRecombinacionMutacion (ES mi_es)
{
	int j; 		/* para bucles*/
	int l;		/* indice de muestras */
	int i;		/* indices de individuos */
	int k;
	Individuo* temp;
	Individuo* orig;
	Individuo* dest;
	int nummut;


#ifdef __PARALELIZACION_GLOBAL__
        StartEvaluation (mi_es->eval);
#endif

	l = 0;

	for (j=0; j<GetNumberOfOffspring(mi_es); j++) {
		dest = ObtenerIndividuo (mi_es->offspring, j);
		if ( (mi_es->cruce != SIN_CRUCE) && (URand01() < mi_es->pcruce) ){
			for (i=0; i<mi_es->aridad; i++) {
				k = mi_es->samples[l++];
				temp = ObtenerIndividuo(mi_es->aux, i);
				orig = ObtenerIndividuo(mi_es->pop, k);
				CopiarIndividuo (temp, orig);
			}/*for*/                                         /*Copiar*/
			Cruzar (dest, mi_es->aux, mi_es);                /*Cruzar*/
		}else{
			k = mi_es->samples[l++];                 /*No hay cruce, una sola muestra*/
			orig = ObtenerIndividuo(mi_es->pop, k);
			CopiarIndividuo (dest, orig);                    /*Copiar*/
		}/*if*/

		nummut = BinomialRand(mi_es->pmutacion, mi_es->longen);
		   
		for (i=0; i<nummut; i++){
			Mutar (dest, mi_es);
		}/*for*/                                                  /*Mutar*/



#ifdef __PARALELIZACION_GLOBAL__

		EvalIndividualPRLL (mi_es->eval, j);
#else
		Evaluar (dest, mi_es);//->infoProblem);
#endif


#ifdef __VERBOSE__
		printf ("\nIndividuo %d evaluado: %f", j, dest->fitness);
#endif

	}/*for*/

#ifdef __PARALELIZACION_GLOBAL__

	WaitEvaluation (mi_es->eval); 

#endif	

}/*END*/


/*
 * Mutacion + Recombinacion.
 *
 */
void CicloMutacionRecombinacion (ES mi_es)
{
	int j; 		/* para bucles*/
	int l;		/* indice de muestras */
	int i;		/* indices de individuos */
	int k,m;
	Individuo* temp=NULL;
	Individuo* orig=NULL;
	Individuo* dest=NULL;
	int nummut;

	l = 0;

	for (j=0; j<GetNumberOfOffspring(mi_es); j++) {
		if ( (mi_es->cruce != SIN_CRUCE) && (URand01() < mi_es->pcruce) ){
                        dest = ObtenerIndividuo(mi_es->offspring, j);
			for (i=0; i<mi_es->aridad; i++){
				k = mi_es->samples[l++];
				temp = ObtenerIndividuo(mi_es->aux, i);
				orig = ObtenerIndividuo(mi_es->pop, k);
				CopiarIndividuo (temp, orig);
				nummut = BinomialRand(mi_es->pmutacion, mi_es->longen);
				for (m=0; m<nummut; m++){
					Mutar(temp, mi_es);
				}/*for*/                                /*1º Mutar*/
			}/*for*/
			Cruzar (dest, mi_es->aux, mi_es);               /*2ºCruzar*/
		}else{						/*O solo mutar*/
			k = mi_es->samples[l++];
			orig = ObtenerIndividuo(mi_es->pop, k);
			CopiarIndividuo (dest, orig);
			nummut = BinomialRand(mi_es->pmutacion, mi_es->longen);
			if (!nummut)
			   nummut = 1;
			for (i=0; i<nummut; i++){
				Mutar (dest, mi_es);
			}/*for*/
		}/*if*/

		Evaluar (dest, mi_es);//->infoProblem); 	/* eval the individual */

		
#ifdef __VERBOSE__
		printf ("\nIndividuo %d evaluado: %f", j, dest->fitness);
#endif
	}/*for*/

}/*END*/


/*
 * Genera la siguiente generacion.
 *
 */
void CrearNuevos (ES mi_es){
/*BEGIN*/
	switch (mi_es->tipoCiclo) {
		case MUTREC:
			CicloMutacionRecombinacion (mi_es);
			break;
		case RECMUT:
			CicloRecombinacionMutacion (mi_es);
			break;
		default:
			printf ("\nERROR: tipo de ciclo desconocido (%d).\n", mi_es->tipoCiclo);
			exit(1);
	}/*switch*/
}/*END*/
