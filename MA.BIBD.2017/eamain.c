/*
 * NOMBRE:	esmain.c
 * AUTOR:	Carlos Cotta
 * FECHA:	5-11-98
 * PROPOSITO:   modulo principal de una estrategia de evolucion simple
 *
 */

#include <stdio.h>   /* para printf, FILE, fopen, fclose */
#include <stdlib.h>  /* para exit */
#include <time.h>
#include "es.h"
#include "esio.h"
#include "util.h"
#include "random.h"  /* para IniciarSemilla */
#include "esprob.h"
#include "esshell.h"
#include "esconf.h"
#include "Blocal.h"


#define FCONF "es.cfg"



/*
 * Procesa los parametros de linea de comando
 */
void ProcesarParametros (int argc, char* argv[], ES mi_es)
{

	if (argc<3) {
		printf ("\nusage: %s <semilla> <numruns> <parametros del problema>\n",argv[0]);
		system("pause");
		exit(1);
	}
	mi_es->semilla = atoi(argv[1]);
	mi_es->numruns = atoi(argv[2]);

	ProcesarParametrosProblema (argc-3,&(argv[3]) );
}

int main (int argc, char* argv[])
{
	ES mi_es;
	FILE* f,*fpm;
	int i;
	ESConfiguration c;
//	Timer t;
	double  runtime;


	c = CreateESConfiguration();
	f = fopen (FCONF,"rt");
	if (!f) {
		printf ("\nERROR: no se encuentra el fichero de configuracion %s.\n",FCONF);
		system("pause");
        exit(1);
	}
	ReadESConfigurationFromFile(f, c);
	fclose(f);


	mi_es = CrearES (0, c);			/* crea estructura basica */

	DestroyESConfiguration(c);

	ProcesarParametros (argc, argv, mi_es); /* parametros en linea de comando */

	IniciarProblemaES (mi_es);		/* Inicializacion del problema  */
    fpm = fopen ("mejor.txt","wt");
	if (mi_es->actg) {
		EscribirCabecera ();
		EscribirParametros (mi_es);
  		printf ("Ejecuciones:	%d\n",mi_es->numruns);
		printf ("Semilla:	    %d\n",mi_es->semilla);
	}
     
	for (i=0; i<mi_es->numruns; i++) {
	    IniciarAleatorios(mi_es->semilla);
            mi_es->semilla++;
        ProblemInfo *p=(ProblemInfo *)mi_es->infoProblem;
        p->nrun=i;
        p->tstart=clock();
		RunES(mi_es);
		if (mi_es->actg) {
			printf("\nEl mejor individuo es:\n");
			EscribirIndividuo(stdout, mi_es->mejor);
		}
		else
			{
              printf ("\n%d\t%ld\t%ld\t%f", i, mi_es->generados+p->tEval,mi_es->generados+p->tEval-mi_es->desierto ,mi_es->mejor->fitness);
              Write_Indiv(mi_es->mejor,p,fpm,mi_es->generados+p->tEval-mi_es->desierto);
           }
        fflush(stdout);
        ResetRun(mi_es);
	}
	fclose(fpm);
 
	DestruirInfoProblema (mi_es->infoProblem);
	DestruirES (mi_es, TRUE);

	return (1);
}




