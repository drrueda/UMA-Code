/*
 * NOMBRE:    esstat.h
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: modulo de estadisticas.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.DEC/1999
 */
 
#include <stdio.h>
#include "es.h"
#include "esio.h"
#include "esstat.h"
#include "esprob.h" 
#include "util.h"


#if defined( __GRAFEVOL__) || defined(__GRAFSOL__)
#include "multigraf.h"
#endif



/*
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 *    Se han modificado los nombres de los ficheros de estadisticas
 * para que en la version paralela del algorimo cada granulo genere
 * sus propios ficheros de estadisticas. 
 *    El nombre del fichero contendra el ID del granulo al que pertenece.
 */  
#define FLOG "esevol%d.log" 
#define FUPD "esupdt%d.log" 
#define FMEJ "esbest%d.log" 


/*
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 *
 * NOTA IMPORTANTE: (Para la version paralela del algoritmo).
 *
 *	Si el numero de granulos es muy alto, puede que se supere el
 * limite de descriptores de ficheros disponibles para cada proceso.
 * Esto se puede comprobar con la orden UNIX 'limit' que da el limite 
 * actual, 'limit -h' da los valores maximos y 'limit descriptor XX'
 * pone el limite al valor indicado.
 */



/*----------------*\
|    Funciones     |
\*----------------*/

/* 
 * Activa las estadisticas a fichero. 
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 *   Funcion modificada para permitir que los ficheros de estadisticas
 *   lleven en el nombre el ID del granulo al que pertenecen.
 */ 
void ActivarEstadisticas (ES mi_es) 
{ 
	char filename[100];

	mi_es->stats = TRUE; 
	if (!mi_es->fevol) {
		sprintf (filename, FLOG, GetID(mi_es)); 
		mi_es->fevol = fopen (filename, "wt"); 
		sprintf (filename, FUPD, GetID(mi_es)); 
		mi_es->fupd  = fopen (filename, "wt");	
		sprintf (filename, FMEJ, GetID(mi_es));  
		mi_es->fmej  = fopen (filename, "wt"); 
	} 
 
} 
 

/* 
 * Desactiva las estadisticas a fichero. 
 * 
 */ 
void DesactivarEstadisticas (ES mi_es) 
{ 
	mi_es->stats = FALSE; 
} 


/*
 * Genera estadisticas de la poblacion.
 *
 */
void GenerarEstadisticas (ES mi_es)
{
   Individuo* mejorActual;
   ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
   if (mi_es->stats) {
		mejorActual = ObtenerIndividuo(mi_es->pop, mi_es->ordpop[0]);
		if(mejorActual->fitness<1||(mi_es->generados+p->tEval)>=mi_es->numind){
        p->van++;
        fprintf (mi_es->fmej, "\n%3d\t%ld\t%10ld", p->nrun,mi_es->generados+p->tEval,mi_es->generados+p->tEval-mi_es->desierto);
        EscribirIndividuo (mi_es->fmej, mi_es->mejor);
        fflush (mi_es->fmej);}
/*
		fprintf (mi_es->fevol, "\n%ld\t%f\t%f", mi_es->generados, mi_es->mejor->fitness, mejorActual->fitness);
		fflush (mi_es->fevol);
*/
	/*	if (mi_es->mejorUpd) {
			fprintf (mi_es->fupd, "%ld\t%f\n", p->TotalEval+mi_es->generados*p->vMp, mi_es->mejor->fitness);
			fflush (mi_es->fupd);
			//fprintf (mi_es->fmej, "\n%3d\t%ld\t%ld ", p->nrun,mi_es->generados+p->TotalEval,mi_es->generados+p->TotalEval-mi_es->desierto);
			//EscribirIndividuo (mi_es->fmej, mi_es->mejor);
			//fflush (mi_es->fmej);
			mi_es->mejorUpd = FALSE;
		}*/
	}//}
	GenerarEstadisticasProblema(mi_es);
	
	mi_es->upgr++;
	if (mi_es->actg && (!mi_es->generados || (!(mi_es->upgr%mi_es->actg)))) {
#ifdef __GRAFEVOL__
		ActualizaGrafico (mi_es->evol);
#endif
#ifdef __GRAFSOL__
		ActualizarGraficosProblema (mi_es);
#endif
		EscribirMejorFitness (stdout, mi_es);
	}
}

