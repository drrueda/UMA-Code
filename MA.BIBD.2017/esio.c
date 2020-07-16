/*
 * NOMBRE:    esio.h
 * AUTOR:     Carlos Cotta
 * FECHA:     4-11-98
 * PROPOSITO: Modulo de entrada/salida.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.DEC/1999
 */

#include <stdio.h> 	/* para printf, fprintf */
#include <stdlib.h>	/* para atoi */
#include "es.h"
#include "esio.h"
#include "esprob.h"
#include "espop.h"

#define VERSION 1
#define REVISION 9





void EscribirCabecera ()
/* Muestra una cabecera al inicial el algoritmo. */
{
	printf ("\n\n");
	printf ("-                 Estrategia de Evolucion v%1d.%1d            -\n", VERSION, REVISION);
	printf ("-                   por Carlos Cotta Porras               -\n");
	printf ("-                    E.T.S.I Informatica                  -\n\n");
}



/*
 * Escribe los parametros de forma abreviada.
 *
 */
void WriteESParametersInShortForm (ES mi_es)
{
	switch (mi_es->sOpt) {
		case MAXIMIZAR:
			printf ("MAX ");
			break;
		case MINIMIZAR:
			printf ("MIN ");
			break;
		default:
			printf ("\nERROR: tipo de optimizacion desconocido.\n");
			break;
	}
	printf ("(%d", GetPopulationSize(mi_es));
	switch (mi_es->reemp) {
		case COMMA:
			printf (",");
			break;
		case PLUS:
			printf ("+");
			break;
		default:
			printf ("\nERROR: tipo de reemplazo desconocido.\n");
			exit(1);
	}
	printf ("%d)-ES: ", GetNumberOfOffspring(mi_es));
	if (mi_es->cruce != SIN_CRUCE) {
		switch (mi_es->tipoCiclo) {
			case MUTREC:
				printf ("mut + ");
				switch (mi_es->cruce) {
					case UX:
						printf ("UX");
						break;
					case SPX:
						printf ("SPX");
						break;
					case DPX:
						printf ("DPX");
						break;
					case PROB:
						printf ("PROBX");
						break;
					default:
						printf ("desconocido (%d).\n", mi_es->cruce);
						exit(1);
				}
				break;
			case RECMUT:
				switch (mi_es->cruce) {
					case UX:
						printf ("UX");
						break;
					case SPX:
						printf ("SPX");
						break;
					case DPX:
						printf ("DPX");
						break;
					case PROB:
						printf ("PROBX");
						break;
					default:
						printf ("desconocido (%d).\n", mi_es->cruce);
						exit(1);
				}
				printf (" + mut");
				break;
			default:
				printf ("desconocido (%d).\n", mi_es->tipoCiclo);
				exit(1);
		}
	}
	else
		printf ("mut");
}



void EscribirParametros (ES mi_es)
/* Muestra en pantalla los parametros generales del algoritmo, asi como aquellos
   dependientes del problema. */
{
	int i;

	printf ("\nPARAMETROS DE LA EJECUCION\n--------------------------\n\n");
	printf ("Mu:             %d\n", GetPopulationSize(mi_es));
	printf ("Lambda:         %d\n", GetNumberOfOffspring(mi_es));
	printf ("Maxevals:       %ld\n", mi_es->numind);
	printf ("Stop At:        %f\n", mi_es->stopAt);
	printf ("Inicializacion: ");
    
    

	switch (mi_es->inicPob) {
		case ORTOGONAL:
			printf ("ORTOGONAL\n");
			break;
		case NO_ORTOGONAL:
			printf ("NO ORTOGONAL\n");
			break;
		default:
			printf ("\nERROR: tipo de inicializacion desconocido.\n");
			system("pause");
			exit(1);
	}
	printf ("Seleccion:      ");

	switch (mi_es->select) {
		case RANDOM_CON_REP:
			printf ("RANDOM CON REPETICION\n");
			break;
		case RANDOM_SIN_REP:
			printf ("RANDOM SIN REPETICION\n");
			break;
        case TOURNAMENT:
            printf("TORNEO (TSize = %d)\n", mi_es->TSize);
            break;
        case PLAIN:
            printf("PLAIN EP\n");
            break;
        default:
			printf ("\nERROR: tipo de seleccion desconocido.\n");
			system("pause");
			exit(1);
	}
	printf ("Ciclo:          ");
	switch (mi_es->tipoCiclo) {
		case MUTREC:
			printf ("Mutacion + Recombinacion\n");
			break;
		case RECMUT:
			printf ("Recombinacion + Mutacion\n");
			break;
		default:
			printf ("desconocido (%d).\n", mi_es->tipoCiclo);
			system("pause");
			exit(1);
	}

	printf ("Cruce:          ");
	switch (mi_es->cruce) {
		case SIN_CRUCE:
			printf ("no hay cruce\n");
			break;
		case UX:
			printf ("UX");
			break;
		case SPX:
			printf ("SPX");
			break;
		case DPX:
			printf ("DPX");
			break;
		case PROB:
			printf ("Cruce dependiente del problema.");
			break;
		default:
			printf ("desconocido (%d).\n", mi_es->cruce);
			system("pause");
			exit(1);
	}
	if (mi_es->cruce != SIN_CRUCE)
		printf (" (%d)\n", mi_es->aridad);

	printf ("Reemplazo:      ");
	switch (mi_es->reemp) {
		case COMMA:
			printf ("COMA\n");
			break;
		case PLUS:
			printf ("PLUS\n");
			break;
		case COMMA_ND:
			printf ("COMMA-NO-DUPLICATES\n");
			break;
		case PLUS_ND:
			printf ("PLUS-NO-DUPLICATES\n");
			break;
		default:
			printf ("\nERROR: tipo de reemplazo desconocido.\n");
		    system("pause");
			exit(1);
	}

	printf ("Optimizacion:   ");
	switch (mi_es->sOpt) {
		case MAXIMIZAR:
			printf ("maximizacion\n");
			break;
		case MINIMIZAR:
			printf ("minimizacion\n");
			break;
		default:
			printf ("\nERROR: tipo de optimizacion desconocido.\n");
			system("pause");
			break;
	}

	printf ("Variables:      %d\n", mi_es->longen);
	/*
	 * (ANTONIO BUENO MOLINA)  v.DEC/1999
	 * Muetra por pantalla el rango inicial de las variables.
	 */
	printf ("Rango Inicial Variables: \n");
	for (i=0; i<mi_es->longen; i++)
		printf ("[0, %d] ", mi_es->varLinSup[i]);
	printf ("\n");

	printf ("Rango variables: \n");

	for (i=0; i<mi_es->longen; i++)
		printf ("[0, %d] ", mi_es->varLSup[i]);
	printf ("\n");
	/*
	 * (ANTONIO BUENO MOLINA)  v.DEC/1999
	 * Mostrar por pantalla si se van a generar estadisticas.
	 */
	printf ("Statistics:     ");
	if (mi_es->stats)
		printf ("ON\n");
	else
		printf ("OFF\n");


	if (mi_es->actg)
		printf ("Actualizacion grafica:  %d\n", mi_es->actg);
	else
		printf ("Actualizacion grafica:  no hay grafico\n");

	EscribirParametrosProblema (mi_es->infoProblem);


	printf ("\n--------------------------\n\n");


}



void EscribirMejorFitness (FILE* f, ES mi_es)
/* Muestra en pantalla el mejor fitness generado */
{
	Individuo* ind;
    ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
	ind = ObtenerIndividuo (mi_es->pop, mi_es->ordpop[0]);
	fprintf (f, "\n%4d : [%10ld] -> %10.0f <- %10.0f", GetID(mi_es),
		     p->TotalEval+mi_es->generados*p->vMp, mi_es->mejor->fitness, ind->fitness);
	fflush (f);
}


void EscribirIndividuo (FILE* f, Individuo* i)
{
	int j;

	fprintf (f, "%8.0f\t", i->fitness);
	for (j=0; j<i->longen; j++)
		fprintf (f, "%d ", i->genoma[j]);
}


