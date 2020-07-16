/*
 * NOMBRE:    esconf.c
 * AUTOR:     Carlos Cotta
 * FECHA:     08-03-99
 * PROPOSITO: modulo de configuration.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.DEC/1999
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>      /*Para que no proteste TurboC 2.0 con strcmp*/
#include "esconf.h"
#include "esdefaul.h"
#include "util.h"


/*-----------------*\
|       Tipos       |
\*-----------------*/


#define NUMBPAR 17

typedef enum {			/* Posibles parametros del algoritmo */
	ERROR        = -1,
	TIPOSEL      = 0,
	TIPOREEMP    = 1,
	TIPOOPT      = 2,
	TIPOINIC     = 3,
	TIPOCRUCE    = 4,
    TIPOMUTACION = 5,
	TIPOCICLO    = 6,
    PCRUCE       = 7,
    PMUTACION    = 8,
	NUMBPOB      = 9,
	NUMBOFF      = 10,
	NUMBARI      = 11,
	NUMBIND      = 12,
	STOPAT       = 13,
	NUMBVAR      = 14,
	NUMACTG      = 15,
	STATSONOFF   = 16
} Parameter;

/*-----------------*\
|     Variables     |
\*-----------------*/

struct Keyword {
	char* string;
	Parameter par;
} keywords[]= {	{"SELECTION]",          TIPOSEL},
		{"REPLACEMENT]",        TIPOREEMP},
		{"OPTIMISATION]",       TIPOOPT},
		{"INITIALISATION]",     TIPOINIC},
		{"RECOMBINATION]",      TIPOCRUCE},
		{"MUTATION]",           TIPOMUTACION},
		{"CYCLE]",              TIPOCICLO},
		{"PROB-RECOMBINATION]", PCRUCE},
		{"PROB-MUTATION]",      PMUTACION},
		{"POPULATION]",         NUMBPOB},
		{"OFFSPRING]",          NUMBOFF},
		{"EVALUATIONS]",        NUMBIND},
		{"STOP-AT]",            STOPAT},
		{"ARITY]",              NUMBARI},
		{"VARIABLES]",          NUMBVAR},
		{"DUMP-INTERVAL]",      NUMACTG},
		{"STATISTICS]",         STATSONOFF}
		};





/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Crea una estructura de configuration por defecto.
 *
 */
ESConfiguration CreateESConfiguration (void)
{
	ESConfiguration c;

	c = (ESConfigurationInfo*) malloc (sizeof(ESConfigurationInfo));
	if (!c) {
		printf ("\nERROR: no hay memoria para configuration\n");
		exit(1);
	}

	c->mu        = MU;
	c->lambda    = LAMBDA;
	c->numind    = NUMBIND;
	c->select    = SELECT;
	c->cruce     = CRUCE;
	c->mutacion  = MUTACION;
	c->pcruce    = PROBCRUCE;
	c->pmutacion = PROBMUTACION;
	c->tipoCiclo = CICLO;
	c->reemp     = REEMP;
	c->sOpt      = SOPT;
	if (c->sOpt == MAXIMIZAR)
		c->stopAt = INFINITO;
	else
		c->stopAt = -INFINITO;
	c->inicPob   = INICPOB;
	c->longen    = LONGEN;
	c->aridad    = ARIDAD;
	c->actg      = ACTG;
	c->stats     = STATS;

	return c;
	
}

/*
 * Destruye la informacion de configuration
 *
 */
void DestroyESConfiguration (ESConfiguration c)
{
	if (c)
		free (c);
}
 


/*
 *  Devuelve el codigo de la palabra clave leida.
 *
 */
Parameter Translate (char* string)
{
	int i;
	
	for (i=0; i<NUMBPAR; i++)
		if (!strcmp(string, keywords[i].string))
			return keywords[i].par;
			
	return ERROR;
}
 
 
/*
 * Lee la configuration de un fichero. Devuelve 0 si hubo algun error.
 *
 */
int ReadESConfigurationFromFile (FILE* f, ESConfiguration c)
{
	char cad[100];
	Parameter par;
	int aridad;
	
	while (!feof(f)) {
		do 
			fscanf (f, "%c", cad);
		while ((cad[0] != '[') && (!feof(f)));
		if (feof(f))
			return 1;
		
		fscanf(f, "%s\n", cad);
		par = Translate (cad);
		switch (par) {
			case TIPOSEL:
				fscanf (f, "%s", cad);
				if (!strcmp(cad, "RANDOM-WITH-REPLACEMENT")){
					c->select = RANDOM_CON_REP;
				}else{
					if (!strcmp(cad, "RANDOM-WITHOUT-REPLACEMENT")) {
						c->select = RANDOM_SIN_REP;
						if (c->aridad>c->mu) 
							c->aridad = c->mu;
					}else{ 
						if (!strcmp(cad, "TOURNAMENT")){
							c->select = TOURNAMENT;
                            fscanf (f, "%d", &(c->TSize));
						}else{
							printf ("\nERROR: seleccion desconocida (%s).\n", cad);
							printf ("Solo puede ser RANDOM-WITH-REPLACEMENT, RANDOM-WITHOUT-REPLACEMENT o TOURNAMENT.\n");
							exit(1);
						}/*if*/
					}/*if*/
				}/*if*/
				break;
				
			case TIPOREEMP:
				fscanf (f, "%s", cad);			
				if (!strcmp(cad, "COMMA")) 
					c->reemp = COMMA;
				else
					if (!strcmp(cad, "PLUS"))
						c->reemp = PLUS;
					else
					    if (!strcmp(cad, "PLUS-NO-DUPLICATES"))
					    	c->reemp = PLUS_ND;
				    	else
					        if (!strcmp(cad, "COMMA-NO-DUPLICATES"))
					    	   c->reemp = COMMA_ND;
					         else {
					     	      printf ("\nERROR: reemplazo desconocido (%s)\n", cad);
   	                              printf ("Solo puede ser COMMA o PLUS.\n");
					       	      exit(1);
	                        }
				break;

			case TIPOOPT:
				fscanf (f, "%s", cad);
				if (!strcmp(cad, "MAXIMISATION")) {
					c->sOpt = MAXIMIZAR;
					c->stopAt = INFINITO;
				}
				else
					if (!strcmp(cad, "MINIMISATION")) {
						c->sOpt = MINIMIZAR;
						c->stopAt = -INFINITO;						
					}
					else {
						printf ("\nERROR: reemplazo desconocido (%s)\n", cad);
						printf ("Solo puede ser MINIMISATION (minimizar) o MAXIMISATION (maximizar).\n");
						exit(1);
					}
				break;
				
			case TIPOINIC:
				fscanf (f, "%s", cad);
				if (!strcmp(cad, "ORTHOGONAL")) 
					c->inicPob = ORTOGONAL;
				else
					if (!strcmp(cad, "NON-ORTHOGONAL"))
						c->inicPob = NO_ORTOGONAL;
					else {
						printf ("\nERROR: tipo de inicializacion desconocido (%s)\n", cad);
						printf ("Solo puede ser ORTHOGONAL, NON-ORTHOGONAL.\n");
						exit(1);
					}
				break;
				
			case TIPOCRUCE:
				fscanf (f, "%s", cad);
				if (!strcmp(cad, "NONE")) {
					c->cruce = SIN_CRUCE;
					c->aridad = 1;
				}
				else
					if (!strcmp(cad, "UX"))
						c->cruce = UX;
					else
						if (!strcmp(cad, "SPX"))
							c->cruce = SPX;
						else
							if (!strcmp(cad, "DPX"))
								c->cruce = DPX;
							else
								if (!strcmp(cad, "HEURISTIC"))
									c->cruce = PROB;
								else {
									printf ("\nERROR: tipo de cruce desconocido (%s)\n", cad);
									printf ("Solo puede ser NONE, UX, SPX, DPX, HEURISTIC.\n");
									exit(1);
								}

				break;

			case TIPOMUTACION:				
				fscanf (f, "%s", cad);
				if (!strcmp(cad, "NONE")) {
					c->mutacion = SIN_MUTACION;
				}
				else
					if (!strcmp(cad, "RANDOM-SUBSTITUTION"))
						c->mutacion = RANDOM_SUBSTITUTION;
					else
						if (!strcmp(cad, "HEURISTIC"))
							c->mutacion = PROB_MUTATION;
						else {
							printf ("\nERROR: tipo de mutacion desconocida (%s)\n", cad);
							printf ("Solo puede ser NONE, RANDOM-SUBSTITUTION, PROB.\n");
							exit(1);
						}
				break;

			case PCRUCE:
				fscanf (f, "%lf\n", &(c->pcruce));			
				break;

			case PMUTACION:
				fscanf (f, "%lf\n", &(c->pmutacion));			
				break;

			case TIPOCICLO:
				fscanf (f, "%s", cad);
				if (!strcmp(cad, "MUTATION+RECOMBINATION")) 
					c->tipoCiclo = MUTREC;
				else
					if (!strcmp(cad, "RECOMBINATION+MUTATION"))
						c->tipoCiclo = RECMUT;
					else {
						printf ("\nERROR: tipo de ciclo desconocido (%s)\n", cad);
						printf ("Solo puede ser RECOMBINATION+MUTATION o MUTATION+RECOMBINATION.\n");
						exit(1);
					}
				break;

			case NUMACTG:
				fscanf (f, "%d\n", &(c->actg));
				break;
				
			case NUMBPOB:
				fscanf (f, "%d\n", &(c->mu));
				break;
				
			case NUMBOFF:
				fscanf (f, "%d\n", &(c->lambda));
				break;
				
			case NUMBARI:
				fscanf (f, "%s\n", cad);
				if (c->cruce == SIN_CRUCE)
					break;
				aridad = atoi(cad);
				if (aridad<2) {
					printf ("\nERROR: la aridad no puede ser menor que 2 (es %s).\n", cad);
					exit(1);
				}
				if ((aridad>c->mu) && (c->select == RANDOM_SIN_REP))
					aridad = c->mu;
				c->aridad = aridad;
				break;
				
			case NUMBIND:
				fscanf (f, "%ld\n", &(c->numind));			
				break;
				
			case STOPAT:
				fscanf (f, "%lf\n", (double*)&(c->stopAt));			
				break;

			case NUMBVAR:
				fscanf (f, "%d\n", &(c->longen));			
				break;

			/* 
			 * (ANTONIO BUENO MOLINA)  v.DEC/1999
			 * Activar o Desactivar las estadisticas.
			 */
			case STATSONOFF:
				fscanf (f, "%s", cad); 
				if (!strcmp(cad, "ON"))  
					c->stats = TRUE; 
				else 
					if (!strcmp(cad, "OFF")) 
						c->stats = FALSE; 
					else { 
						printf ("\nERROR: Activate/Destativate Statistics (%s)\n", cad); 
						printf ("         Only can be ON or OFF .\n"); 
						exit(1); 
					} 
				break; 
				
			default:
				printf ("\nERROR: parametro desconocido (%s)\n", cad);
				exit(1);
		}
	}
	
	return 1;
}

