/*
 * NOMBRE:	esmain.c
 * AUTOR:	Carlos Cotta
 * FECHA:	5-11-98
 * PROPOSITO:   modulo principal de una estrategia de evolucion simple
 * NOTA: Reutilizacion de código.  Se desactiva la estrategia evolutiva y se activa la búsqueda local.  David Rodriguez 12/12/2008
 */

#include <stdio.h>   /* para printf, FILE, fopen, fclose */
#include <stdlib.h>  /* para exit */
//#include <dos.h>
#include <time.h>
#include "es.h"
#include "esio.h"
#include "util.h"
#include "random.h"  /* para IniciarSemilla */
#include "esprob.h"
#include "esshell.h"
#include "esconf.h"
//#include "timer.h"
#include "BLocal.h"


#define FCONF "es.cfg"


/*
 * Procesa los parametros de linea de comando
 */
void ProcesarParametros (int argc, char* argv[], ES mi_es)
{

	if (argc<3) {
		printf ("\nusage: %s <semilla> <numruns> <parametros del problema>\n",argv[0]);
		exit(1);
	}
	mi_es->semilla = atoi(argv[1]);
	mi_es->numruns = atoi(argv[2]);

	ProcesarParametrosProblema (argc-3,&(argv[3]) );
}

int main (int argc, char* argv[])
{
	ES mi_es;
	FILE* f,*fpt,*fps,*fpm,*fl;
	int i;
	ESConfiguration c;
	double  runtime;
	Individuo *enteA,*enteB;
	ProblemInfo *p;
	long xx=500000;
    double xmin;
	char log[]="Trazas.txt",logt[]="sol.txt",out[]="mejor.txt";
	


	c = CreateESConfiguration();
	f = fopen (FCONF,"rt");
	fpt = fopen (log,"wt");
	fps = fopen (logt,"wt");
	fpm = fopen (out,"wt");
//	fl = fopen ("filas.txt","wt");
	if (!f) {
		printf ("\nERROR: no se encuentra el fichero de configuracion %s.\n",FCONF);
        exit(1);
	}
	ReadESConfigurationFromFile(f, c);
	fclose(f);


	mi_es = CrearES (0, c);			/* crea estructura basica */

	DestroyESConfiguration(c);

	ProcesarParametros (argc, argv, mi_es); /* parametros en linea de comando */
	IniciarProblemaES (mi_es);
	p=(ProblemInfo *)mi_es->infoProblem;
	enteA=CrearIndividuo(p->v*p->b);
	enteB=CrearIndividuo(p->v*p->b);
	enteB->fitness=100000;
	time_t tt;
    for(int run=0;run<mi_es->numruns;run++){
      IniciarAleatorios(mi_es->semilla);
      enteB->fitness=100000;
      p->nEvalMin=0;
      p->TotalEval=0;
      p->van=0;
      xmin=-1;
      p->nrun=run;
      mi_es->semilla++;   // 1 = Genera Indiv. Incial de Forma Aleatoria con Restriccion de r unos por Fila
      xx=mi_es->numind;   // 0 = Genera Indiv. Incial de Forma Aleatoria sin Restriccion 
      switch(p->algo)
       {
         case 1: // Hill Climbing Bit.Flip
                 while(xx>0)
                   {                      
	                 GenerarIndividuo(enteA,p->tInic,p);
                     enteA->fitness=EvaluarRestricion(enteA, p);
                     HcBitFlip(enteA,p,xx,fps);
                     if(enteA->fitness<enteB->fitness)
                     {
                       CopiarIndividuo(enteB,enteA);
                       xmin=p->nEvalMin;
                     }
                    if(enteB->fitness<1)
                       break;
                   }
                  break;
         case 2: // Hill Climbing Swap
                 while(xx>0)
                   {                      // 1 = Aleatorio con Restriccion de r unos por Fila 
	                 GenerarIndividuo(enteA,p->tInic,p);
                     enteA->fitness=EvaluarRestricion(enteA, p);
                     HcSwap(enteA,p,xx,fps);
                     if(enteA->fitness<enteB->fitness)
                     {
                       CopiarIndividuo(enteB,enteA);
                       xmin=p->nEvalMin;
                     }
                     if(enteB->fitness<1)
                       break;
                   }
                  break;
         case 3: // Tabu Search Bit.Flip
                 GenerarIndividuo(enteA,p->tInic,p);
                 enteA->fitness=EvaluarRestricion(enteA, p);
                 TsBF(enteA,p,mi_es->numind,mi_es->numind/1000,fps);   
                 CopiarIndividuo(enteB,enteA);
                 xmin=p->nEvalMin; 
                 break;
         case 4: // Tabu Search Swap
                 GenerarIndividuo(enteA,p->tInic,p);
                 enteA->fitness=EvaluarRestricion(enteA, p);
                 p->tstart=clock();
                 TsSwap(enteA,p,mi_es->numind,p->v*10,fps);   
                 CopiarIndividuo(enteB,enteA);
                 xmin=p->nEvalMin; 
                 break;
         }

      fprintf(fpt,"\n%d   %12ld   %12.0lf  %10.0lf   ",run,p->TotalEval,xmin*p->vMp,enteB->fitness);
      Esc_Indiv(enteB,p,fps,xmin*p->vMp);
      Esc_Indiv(enteB,p,fpm,xmin*p->vMp);
      printf("\n%d    %10.0lf     %10.0lf",run,xmin*p->vMp,enteB->fitness);
      i=0;
     /* for(int ki=0;ki<p->v;ki++)
        {
          for(int kj=0;kj<p->b;kj++)
           {
             fprintf(fl,"%d ",enteB->genoma[i]);
             i++;
           }
          fprintf(fl,"\n");
        }*/
          
    }
    DestruirIndividuo(enteA);
    DestruirIndividuo(enteB);


	if (mi_es->actg) 
	     printf ("Elapsed time: %f\n", runtime);
 
	DestruirInfoProblema (mi_es->infoProblem);
	DestruirES (mi_es, TRUE);
	fclose(fpt);
	fclose(fpm);
    //fclose(fl);
    //system("pause");
	return (1);
}




