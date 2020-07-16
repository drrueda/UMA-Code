/*
 * NOMBRE:    esselect.c
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: modulo para la seleccion de la poblacion.
 *
 */
 

#include "es.h"
#include "random.h"
#include "esindiv.h"   /*Para ObtenerFitness*/
#include "esselect.h"
#include "esreemp.h"




/*----------------*\
|    Funciones     |
\*----------------*/

/*
 * Crea una permutacion parcial de 'num' elementos de taman~o 'size'
 *
 */

void PermutacionParcial (int* perm, int num, int size)
{
	int i;
	int j;
	int temp;
	
	for (i=0; i<num; i++)
		perm[i] = i;
		
	for (i=0; i<size; i++) {
		j = URand(num-i);
		temp = perm[i+j];
		perm[i+j] = perm[i];
		perm[i] = temp;
	}
}



/*
 * Selecciona la poblacion.
 *
 */
void Seleccionar (ES mi_es)
{
	int i;
	int j;
	int n1, n2;

	Individuo* i1;
    Individuo* i2;


	switch (mi_es->select) {
		case RANDOM_CON_REP:
			for (i=0; i<mi_es->numsamp; i++)
				mi_es->samples[i] = URand(GetPopulationSize(mi_es));
		break;

		case RANDOM_SIN_REP:
			i = 0;
			while (i<mi_es->numsamp) {
				PermutacionParcial (mi_es->perm, GetPopulationSize(mi_es), mi_es->aridad);
				for (j=0; j<mi_es->aridad; j++) {
					mi_es->samples[i+j] = mi_es->perm[j];  /* j */
				}
				i+= j;
			}
		break;

		case TOURNAMENT:
			i=0;
			while (i<mi_es->numsamp) {
				n1 = URand(GetPopulationSize(mi_es));
       	        i1 = ObtenerIndividuo(mi_es->pop, n1);
                mi_es->samples[i] = n1;
				for (j=1; j<mi_es->TSize; j++) {
                     n2 = URand(GetPopulationSize(mi_es));
               	     i2 = ObtenerIndividuo(mi_es->pop, n2);
				     if (Mejor(ObtenerFitness(i2),ObtenerFitness(i1),mi_es->sOpt)) {
						mi_es->samples[i] = n2;
                        i1 = i2;
                     }
                }
				i++;
			}/*while*/
		break;

		case PLAIN:
			for (i=0; i<mi_es->numsamp; i++)
				mi_es->samples[i] = i%GetPopulationSize(mi_es);
		break;
		
	}/*switch*/

}



