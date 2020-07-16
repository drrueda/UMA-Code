/*
 * NOMBRE:    es.c
 * AUTOR:     Carlos Cotta
 * FECHA:     4-11-98
 * PROPOSITO: funciones basicas de una estrategia de evolucion simple.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.JAN/2000
 */
 

#include <stdio.h>    /* para FILE, printf */
#include <stdlib.h>   /* para exit, malloc, free */
#include <string.h>   /* para memcmp, strcmp */
#include <values.h>   /* para MAXFLOAT */
#include <math.h>     /* para sqrt */

#include "es.h"
#include "esdefaul.h" /* valores por defecto */
#include "esindiv.h"  /* manejo de individuos */
#include "espop.h"    /* manejo de poblaciones */
#include "esprob.h"   /* para LimiteAlcanzado */
#include "util.h"     /* constantes y macros */
#include "esconf.h"   /* para la configuracion */
#include "esreemp.h"  /* para SortAndUpdateBest */
#include "esstat.h"   /* para ActivarEstadisticas */

#ifdef __GRAFEVOL__
#include "multigraf.h"
#endif

#ifdef __PARELELIZACION_GLOBAL__
#include "esthread.h"
#endif


/*----------------*\
|    Funciones     |
\*----------------*/

/* 
 * Reserva toda la memoria necesaria. Asume que los parametros
 * tales como mu, lambda o la longitud de los individuos ya estan
 * fijados.
 *
 */
int IniciarES (ES mi_es)
{
	int i;

	mi_es->pop = CrearPoblacion (mi_es->mu, DEFPOP, mi_es->longen);
	mi_es->offspring = CrearPoblacion (mi_es->lambda, TRUE, mi_es->longen);
	mi_es->aux = CrearPoblacion (mi_es->aridad, TRUE, mi_es->longen);

	mi_es->ordoff = (int*) malloc (mi_es->lambda*sizeof(int));
	mi_es->ordpop = (int*) malloc (mi_es->mu*sizeof(int));
	if (!mi_es->ordoff || !mi_es->ordpop) {
		printf ("\nERROR: no hay memoria para arrays de ordenacion.\n");
		exit(1);
	}

	mi_es->injected = (int*) malloc (mi_es->mu*sizeof(int));
	if (!mi_es->injected) {
		printf ("\nERROR: no hay memoria para arrays de inyectados.\n");
		exit(1);
	}
	memset (mi_es->injected, FALSE, mi_es->mu*sizeof(int));

	mi_es->numsamp = mi_es->aridad*mi_es->lambda;
	mi_es->samples = (int*) malloc (mi_es->numsamp*sizeof(int));
	if (!mi_es->samples) {
		printf ("\nERROR: no hay memoria para muestras.\n");
		exit(1);
	}
	switch (mi_es->select) {
		case RANDOM_SIN_REP:
			mi_es->perm = (int*) malloc (mi_es->mu*sizeof(int));
			if (!mi_es->perm) {
				printf ("\nERROR: no hay memoria para perm.\n");
				exit(1);
			}
			break;

		default:
			mi_es->perm = NULL;
	}


	mi_es->mejor = CrearIndividuo (mi_es->longen);

	/*
 	 * (ANTONIO BUENO MOLINA)  v.DEC/1999
	 * 	Reservar memoria para los limites iniciales de las
	 * variables y verificar que existe memoria suficiente.
	 */
	mi_es->varLinSup = (Variable*) malloc (mi_es->longen*sizeof(Variable));
	if (!mi_es->varLinSup) {
		printf ("\nERROR: no hay memoria para limites iniciales (%d).\n", mi_es->longen);
		exit(1);
	}

	mi_es->varLSup  = (Variable*) malloc (mi_es->longen*sizeof(Variable));

 	if (!mi_es->varLSup) {

		printf ("\nERROR: no hay memoria para limites.\n");

		exit(1);

	}

	for (i=0; i<mi_es->longen; i++) {
		mi_es->varLSup[i] = MAXVAR;

		/*
		 * (ANTONIO BUENO MOLINA)  v.DEC/1999
		 * Actualizar los limites inciales, por defecto se
		 * toman los valores de los limites de ejecucion.
		 */
		mi_es->varLinSup[i] = MAXVAR;
	}

#ifdef __PARALELIZACION_GLOBAL__

	/*
	 * (ANTONIO BUENO MOLINA)  v.JAN/2000
	 *    Se van a utilizar para evaluar la poblacion inicial y
	 * para evaluar las generaciones de descendientes.
         *    Una vez evaluada la poblacion incial se reajusta el
	 * numero de argumentos al numero de descendientes.
	 */
	mi_es->eval = CreateEvalArgs(Size(mi_es->pop));

#endif  /* __PARALELIZACION_GLOBAL__ */

 	return 1;
}




/*
 * Crea una ES vacia, usando los valores por defecto.
 *
 */
ES CrearES (int ID, ESConfiguration c)
{
	ES mi_es;
	int created;
	int i;

	mi_es = (ES) malloc (sizeof(ESInfo));
	if (!mi_es) {
		printf ("\nERROR: no hay memoria para crear ES.\n");
		exit(1);
	}

	mi_es->ID = ID;

	mi_es->fevol     = NULL;
	mi_es->fupd      = NULL;
	mi_es->fmej      = NULL;

	mi_es->pop       = NULL;
	mi_es->offspring = NULL;
	mi_es->aux       = NULL;
	mi_es->ordoff    = NULL;
	mi_es->ordpop    = NULL;
	mi_es->injected  = NULL;
	mi_es->mejor     = NULL;

	/*
	 * (ANTONIO BUENO MOLINA) v.DEC/1999
	 * Inicializar los limites iniciales de las variables
	 * y de las varianzas.
	 */
	mi_es->varLinSup = NULL;

	mi_es->varLSup   = NULL;

	/*
 	 * (ANTONIO BUENO MOLINA)  v.OCT/1999
 	 * Faltan por inicializar los parámetros: samples, perm.
 	 */
 	mi_es->samples   = NULL;
	mi_es->perm	 = NULL;


	if (!c) {
		c = CreateESConfiguration();
		created = TRUE;
	}
	else
		created = FALSE;


	mi_es->mu        = c->mu;
	mi_es->lambda    = c->lambda;
	mi_es->numind    = c->numind;
	mi_es->stopAt    = c->stopAt;
	mi_es->select    = c->select;
	mi_es->TSize     = c->TSize;
    mi_es->cruce     = c->cruce;
    mi_es->mutacion  = c->mutacion;
	mi_es->pcruce    = c->pcruce;
    mi_es->pmutacion = c->pmutacion;
	mi_es->tipoCiclo = c->tipoCiclo;
	mi_es->reemp     = c->reemp;
	mi_es->sOpt      = c->sOpt;
	mi_es->inicPob   = c->inicPob;
	mi_es->longen    = c->longen;
	mi_es->aridad    = c->aridad;
	mi_es->actg      = c->actg;
	mi_es->stats     = c->stats;


	IniciarES (mi_es);

/*      for (i=0; i<mi_es->longen; i++) {
              DefineLimitesVariable (mi_es, i, c->varLSup);
*/
 		/*
		 * (ANTONIO BUENO MOLINA)  v.DEC/1999
		 * Actualiza los limites iniciales de cada variable.
		 */
/*		DefineLimitesInicialesVariable (mi_es, i, c->varLinSup);
        }
*/
       	if (created)
		DestroyESConfiguration(c);


	/*
	 * (ANTONIO BUENO MOLINA)  v.DEC/1999
	 * Activar estadisticas si la seccion del es.cfg [STATISTICS] = ON
	 */
	if (mi_es->stats)
		ActivarEstadisticas(mi_es);



#ifdef __GRAFEVOL__

	mi_es->evol = CreaGrafico (FLOG, 2, 1);
        DefineVariable (mi_es->evol, 1, 2);
        DefineTitulo (mi_es->evol, 1, "mejor");
        DefineVariable (mi_es->evol, 2, 3);
        DefineTitulo (mi_es->evol, 2, "actual");

#endif /* __GRAFEVOL__ */


#ifdef __PARALELIZACION_GLOBAL__

	/*
	 * (ANTONIO BUENO MOLINA)  v.JAN/2000
	 * No hay que inicializar las hebras de evaluacion con los
	 * individuos de la poblacion inicial ya que aun no han sido
	 * creados. En el modulo ESINITP se inicializan los individuos
	 * a medida que se van creando.
	 */

#endif /* __PARALELIZACION_GLOBAL__ */

	return mi_es;
}


/*
 * Libera la memoria ocupada por la ES.
 * El flag indica si se deben destruir tambien los individuos de
 * la poblacion. Normalmente, sera TRUE salvo cuando se quieran
 * conservar estos por algun motivo (estadisticas, ...).
 *
 */
void DestruirES (ES mi_es, int flag)
{

	DestruirPoblacion (mi_es->pop, flag);
	DestruirPoblacion (mi_es->offspring, TRUE);
	DestruirPoblacion (mi_es->aux, TRUE);

	DestruirIndividuo (mi_es->mejor);

	if (mi_es->samples)
		free (mi_es->samples);
	if (mi_es->perm)
		free (mi_es->perm);

	/*
	 * (ANTONIO BUENO MOLINA)  v.OCT/1999
	 * Falta liberar la memoria de los Individuos Injected.
	 */
	if (mi_es->injected)
		free(mi_es->injected);

	/*
	 * (ANTONIO BUENO MOLINA)  v.DEC/1999
	 * Liberar la memoria ocupada por los limites inciales.
	 */
	if (mi_es->varLinSup)
		free (mi_es->varLinSup);

	if (mi_es->ordoff)
		free (mi_es->ordoff);
	if (mi_es->ordpop)
		free (mi_es->ordpop);


	if (mi_es->varLSup)
		free (mi_es->varLSup);


	fclose (mi_es->fevol);
	fclose (mi_es->fupd);
	fclose (mi_es->fmej);

#ifdef __GRAFEVOL__

	CierraGrafico (mi_es->evol);

#endif

#ifdef __PARALELIZACION_GLOBAL__

	DestroyEvalArgs (mi_es->eval);

#endif

}


/*
 * Reconfigura el algoritmo.
 *
 */
void Reconfigure (ES mi_es, ESConfiguration c)
{
	int i;

	if (!c)
		return;

	DefineSeleccion (mi_es, c->select);
	DefineTamanoPoblacion (mi_es, c->mu);
	DefineNumeroDescendientes (mi_es, c->lambda);
	DefineAridad (mi_es, c->aridad);
	DefineNumeroVariables (mi_es, c->longen);
/*      for (i=0; i<mi_es->longen; i++) {
                DefineLimitesVariable (mi_es, i, c->varLSup);
*/
		/*
		 * (ANTONIO BUENO MOLINA)  v.DEC/1999
		 * Actualiza los limites iniciales de cada variable.
		 */
/*		DefineLimitesInicialesVariable (mi_es, i, c->varLinSup);

     }
*/

	mi_es->numind    = c->numind;
	mi_es->cruce     = c->cruce;
    mi_es->mutacion  = c->mutacion;
	mi_es->pcruce    = c->pcruce;
    mi_es->pmutacion = c->pmutacion;
	mi_es->tipoCiclo = c->tipoCiclo;
	mi_es->reemp     = c->reemp;
	mi_es->sOpt      = c->sOpt;
	mi_es->inicPob   = c->inicPob;
	mi_es->TSize     = c->TSize;
	mi_es->stopAt    = c->stopAt;
	mi_es->actg      = c->actg;
	mi_es->stats     = c->stats;
}

/*
 * Devuelve TRUE cuando se cumple el criterio de finalizacion.
 *
 */
TerminationStatus Terminacion (ES mi_es)
{
	if (mi_es->numind <= mi_es->generados)
		return MAXEVALS_REACHED;

	if (Mejor(ObtenerFitness(mi_es->mejor), mi_es->stopAt, mi_es->sOpt))
		return SOLUTION_REACHED;

	if (LimiteAlcanzado (mi_es))
		return PROB_TERMINATION;

	return NO_TERMINATION;
}


/*
 * Define el tipo de seleccion
 *
 */
void DefineSeleccion (ES mi_es, TipoSeleccion sel)
{
	mi_es->select = sel;

	switch (mi_es->select) {
		case RANDOM_SIN_REP:
			mi_es->perm = (int*) realloc (mi_es->perm, mi_es->aridad*sizeof(int));
			if (!mi_es->perm) {
				printf ("\nERROR: no hay memoria para perm.\n");
				exit(1);
			}
			break;

		default:
			if (mi_es->perm)
				free (mi_es->perm);
			mi_es->perm = NULL;
	}


}





/*
 *  Cambia el numero de variables contenidas en cada individuo.
 * Realoja la memoria necesaria para los limites y el taman~o de cada
 * una de las poblaciones.
 *
 */
void DefineNumeroVariables (ES mi_es, int longen)
{
	int i;
	int oldlong;


	if (mi_es->longen == longen)
		return;

	oldlong = mi_es->longen;
	mi_es->longen = longen;


	/*
	 * (ANTONIO BUENO MOLINA)  v.DEC/1999
	 * Cambiar tamaño de los limite iniciales de las variables,
	 * y verificar que existe memoria.
	 */

	mi_es->varLinSup = (Variable*) realloc (mi_es->varLinSup,
	                                        mi_es->longen*sizeof(Variable));

	if (!mi_es->varLinSup) {
		printf ("\nERROR: no hay memoria para limites iniciales.\n");
		exit(1);
	}


	mi_es->varLSup = (Variable*) realloc (mi_es->varLSup,
	                                      mi_es->longen*sizeof(Variable));

	if (!mi_es->varLSup) {
		printf ("\nERROR: no hay memoria para limites.\n");
		exit(1);
	}

	for (i=oldlong; i<longen; i++) {
		mi_es->varLSup[i] = MAXVAR;

		/*
		 * (ANTONIO BUENO MOLINA)  v.DEC/1999
		 * Actualizar los limites iniciales, por defecto se
		 * toman los limites de ejecucion.
		 */
		mi_es->varLinSup[i] = MAXVAR;
	}

	ModificarNumeroVariablesPoblacion (mi_es->pop, longen);
	ModificarNumeroVariablesPoblacion (mi_es->offspring, longen);
	ModificarNumeroVariablesPoblacion (mi_es->aux, longen);
}


/*
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 * Cambia los limites iniciales de la i-esima variable.
 *
 */
void DefineLimitesInicialesVariable (ES mi_es, int i, Variable vsup)
{
	mi_es->varLinSup[i] = vsup;
}


/*
 * Cambia los limites de la i-esima variable.
 *
 */
void DefineLimitesVariable (ES mi_es, int i, Variable vsup)
{
	mi_es->varLSup[i] = vsup;
}




/*
 * Cambia el taman~o de la poblacion de padres.
 *
 */
void DefineTamanoPoblacion (ES mi_es, int mu)
{
	int oldmu = mi_es->mu;
	int i;

	mi_es->pop = CambiaTamanoPoblacion (mi_es->pop, mu, mi_es->longen);
	mi_es->mu = mu;

	mi_es->ordpop = (int*) realloc (mi_es->ordpop, mi_es->mu*sizeof(int));
	mi_es->injected = (int*) realloc (mi_es->injected, mi_es->mu*sizeof(int));
	if (!mi_es->ordpop || !mi_es->injected) {
		printf ("\nERROR: no hay memoria para array de ordenacion o inyeccion.\n");
		exit(1);
	}

	for (i=oldmu; i<mu; i++)
		mi_es->injected[i] = FALSE;

	switch (mi_es->select) {
		case RANDOM_SIN_REP:
			mi_es->perm = (int*) realloc (mi_es->perm, mi_es->mu*sizeof(int));
			if (!mi_es->perm) {
				printf ("\nERROR: no hay memoria para perm.\n");
				exit(1);
			}
			break;

		default:
			mi_es->perm = NULL;
	}

}


/*
 * Cambia el taman~o de la poblacion de descendientes.
 *
 */
void DefineNumeroDescendientes (ES mi_es, int lambda)
{

	mi_es->offspring = CambiaTamanoPoblacion (mi_es->offspring,
		                                  lambda, mi_es->longen);
	mi_es->lambda = lambda;

	mi_es->numsamp = mi_es->aridad*mi_es->lambda;
	mi_es->samples = (int*) realloc (mi_es->samples, mi_es->numsamp*sizeof(int));
	if (!mi_es->samples) {
		printf ("\nERROR: no hay memoria para %d muestras.\n", mi_es->numsamp);
		exit(1);
	}

	/*
	 * (ANTONIO BUENO MOLINA)  v.OCT/1999
	 * Error al cambiar el tamaño del mi_es->ordoff
	 * mi_es->ordoff = (int*) malloc (mi_es->lambda*sizeof(int));
	 */
	mi_es->ordoff = (int*) realloc (mi_es->ordoff, mi_es->lambda*sizeof(int));

	if (!mi_es->ordoff) {
		printf ("\nERROR: no hay memoria para array de ordenacion.\n");
		exit(1);
	}

}



/*
 * Cambia la aridad de la recombinacion. Si no hay cruce,
 * se ignora esta llamada.
 *
 */
void DefineAridad (ES mi_es, int aridad)
{
	if (mi_es->cruce == SIN_CRUCE)
		return;

	mi_es->aux = CambiaTamanoPoblacion (mi_es->aux, aridad, mi_es->longen);
	mi_es->aridad = aridad;

	mi_es->numsamp = mi_es->aridad*mi_es->lambda;
	mi_es->samples = (int*) realloc (mi_es->samples, mi_es->numsamp*sizeof(int));
	if (!mi_es->samples) {
		printf ("\nERROR: no hay memoria para %d muestras.\n", mi_es->numsamp);
		exit(1);
	}
}


/*
 * Forces a "SortAndUpdateBest" of the algoritm
 *
 */
void RefreshES (ES mi_es)
{
	mi_es->mejorUpd =
            SortAndUpdateBest (mi_es->ordpop, mi_es->pop, mi_es->sOpt,
                               mi_es->mejor);
}

