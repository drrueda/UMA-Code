/*
 * NOMBRE:    esreemp.h
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: modulo para el reemplazo de la poblacion.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.NOV/1999
 */
 
#include <string.h>

#include "es.h"
#include "util.h"
#include "esreemp.h"
#include "espop.h"


/*----------------*\
|    Funciones     |
\*----------------*/

void OrdenarPoblacion (int* indices, Poblacion pop, SentidoOpt s)
{
	int i;		/* para bucles */
	int j;
	int temp;	/* para intercambiar indices */
	Individuo* i1;	/* individuos que se comparan */
	Individuo* i2;
	int num;	/* numero de individuos en la poblacion */
	
	num = Size(pop);

	for (i=0; i<num; i++)
		indices[i] = i;	
		
	for (i=0; i<num; i++) {
		for (j=i+1; j<num; j++) {
			i1 = ObtenerIndividuo(pop, indices[i]);
			i2 = ObtenerIndividuo(pop, indices[j]);			
			if (!Mejor(i1->fitness, i2->fitness, s)) {
				temp = indices[i];
				indices[i] = indices[j];
				indices[j] = temp;
			}
		}
	}
}



/*
 * Sorts the population and updates the best individual created if necessary.
 * Returns TRUE if the best individual has been updated.
 *
 */
int SortAndUpdateBest (int* indices, Poblacion pop, SentidoOpt s, Individuo* i)
{
	Individuo* ind;

	OrdenarPoblacion (indices, pop, s);
	ind = ObtenerIndividuo(pop, indices[0]);
	if (Mejor(ObtenerFitness(ind), ObtenerFitness(i), s)) {
		CopiarIndividuo(i, ind);
		return TRUE;
	}
	else
		return FALSE;
}

int Duplicado (Individuo* ind, Poblacion pop)
{
    int i;
    Individuo* ind2;
    
    for (i=0; i<Size(pop); i++) {
        ind2 = ObtenerIndividuo(pop, i);
        if (!memcmp(ind->genoma, ind2->genoma, ind->longen*sizeof(Variable))) {
//           printf ("Duplicado\n");
           return 1;
        }
    }
    
    return 0;
}


/*
 * Reemplaza la poblacion.
 *
 */
void Reemplazar (ES mi_es)
{
	int i;			/* para bucles */
	int lim;		/* numero de iteraciones */
	Individuo* i1;		/* para recorrer las poblaciones */
	Individuo* i2;

	
	lim = MIN(GetPopulationSize(mi_es), GetNumberOfOffspring(mi_es));

	/*
	 * (ANTONIO BUENO MOLINA)  v.NOV/1999
	 * El primer OrdenarPoblacion es innecesario. Ya que la primera vez
	 * RefresES ordena la poblacion y las siguientes veces lo hace la 
	 * funcion al final del reemplazo, luego siempre entran ordenados.
	 */
 
	/* OrdenarPoblacion (mi_es->ordpop, mi_es->pop, mi_es->sOpt);
*/
	if (SortAndUpdateBest (mi_es->ordoff, mi_es->offspring, mi_es->sOpt, mi_es->mejor)) {
		mi_es->desierto = 0;
		mi_es->mejorUpd = TRUE;
	}
	else
		mi_es->desierto += GetNumberOfOffspring(mi_es);	
	
#ifdef __VERBOSE__
	printf ("\nAntiguos:\n");
	for (i=0; i<GetPopulationSize(mi_es); i++)
		printf (" %f ", mi_es->pop->inds[mi_es->ordpop[i]]->fitness);
	printf ("\n");
	printf ("\nNuevos:\n");
	for (i=0; i<GetNumberOfOffspring(mi_es); i++)
		printf (" %f ", mi_es->offspring->inds[mi_es->ordoff[i]]->fitness);
	printf ("\n");
#endif	

	for (i=0; i<lim; i++) {
		i1 = ObtenerIndividuo(mi_es->pop, mi_es->ordpop[GetPopulationSize(mi_es)-1-i]);
		i2 = ObtenerIndividuo(mi_es->offspring, mi_es->ordoff[i]);
		if ((mi_es->reemp == COMMA) ||
		    ((mi_es->reemp == PLUS) && Mejor(i2->fitness, i1->fitness, mi_es->sOpt)))
      			CopiarIndividuo (i1, i2);		    
		else if ((mi_es->reemp == COMMA_ND) ||
		        ((mi_es->reemp == PLUS_ND) && Mejor(i2->fitness, i1->fitness, mi_es->sOpt))) {
      	                   if (!Duplicado(i2, mi_es->pop)) 
                          			CopiarIndividuo (i1, i2);
             }
	}
	
	OrdenarPoblacion (mi_es->ordpop, mi_es->pop, mi_es->sOpt);
	mi_es->generados += GetNumberOfOffspring(mi_es);


#ifdef __VERBOSE__
	printf ("Tras Reemplazar:\nAntiguos:\n");
	for (i=0; i<GetPopulationSize(mi_es); i++)
		printf (" %f ", mi_es->pop->inds[mi_es->ordpop[i]]->fitness);
	printf ("\n");
	printf ("\nNuevos:\n");
	for (i=0; i<GetNumberOfOffspring(mi_es); i++)
		printf (" %f ", mi_es->offspring->inds[mi_es->ordoff[i]]->fitness);
	printf ("\n");
#endif	

}


