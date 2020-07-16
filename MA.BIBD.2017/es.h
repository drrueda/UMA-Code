/*
 * NOMBRE:    es.h
 * AUTOR:     Carlos Cotta
 * FECHA:     4-11-98
 * PROPOSITO: Fichero de cabecera de una estrategia de evolucion simple.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.DEC/1999
 */

#ifndef __ES_H__
#define __ES_H__
#include <time.h> 
#include <stdio.h>   /* para FILE */
#include "esindiv.h" /* manejo de individuos */
#include "espop.h"   /* manejo de poblaciones */
#include "esconf.h"  /* configuracion del algoritmo */

#ifdef __GRAFEVOL__
#include "multigraf.h"
#endif

#ifdef __PARALELIZACION_GLOBAL__
#include "esthread.h"
#endif


/*----------------*\
|      Tipos       |
\*----------------*/
typedef enum {
	NO_TERMINATION = 0,
	MAXEVALS_REACHED = 1,
	SOLUTION_REACHED = 2,
	PROB_TERMINATION = 3
} TerminationStatus;

typedef struct {
	int           ID;	/* un identificador del algoritmo */
	Poblacion     pop;	/* poblacion del algoritmo */
	Poblacion     offspring;/* poblacion para los descendientes */
	Poblacion     aux;	/* poblacion auxiliar */
	int           mu;	/* numero de padres */
	int           lambda;	/* numero de hijos */
	int           longen;	/* numero de variables (si es comun) */
	TipoInicPob   inicPob;  /* tipo de generacion de la pob. inicial */
	TipoReempl    reemp;	/* reemplazo empleado */
	TipoCiclo     tipoCiclo;/* tipo de ciclo evolutivo empleado */
	TipoCruce     cruce;    /* operador de cruce */
	TipoMutacion  mutacion; /* operador de mutacion*/
	double        pcruce;   /* para la prob de cruce */
	double        pmutacion;/* para la prob de mutacion */
	int           aridad;	/* numero de padres para recombinacion */
	SentidoOpt    sOpt;	/* sentido de la optimizacion */
	long          numind;	/* numero maximo de evaluaciones */
	long          generados;/* numero de individuos generados hasta el momento */
	Individuo*    mejor;	/* mejor individuo generado */
	int           mejorUpd; /* si TRUE, mejor se ha actualizado */
	ValorFitness  stopAt;   /* if this fitness values is reached, the algorithm stops */
	int           desierto; /* iteraciones sin mejora */
    Variable*     varLinSup;/* limites inic. Sup. de las variables */
	Variable*     varLSup;	/* limites superiores de las variables */
	TipoSeleccion select;	/* tipo de seleccion */
    int           TSize;    /* Tamaño del torneo */
	int           numsamp;	/* numero de muestras */
	int*          perm;  	/* auxiliar para seleccion */
	int*          samples;	/* individuos muestreados */
	int*          injected;	/* para controlar individuos inyectados */
	int*          ordoff;	/* para mantener ordenada la poblacion offspring */
	int*          ordpop;	/* idem con pop */
	int           actg;     /* intervalo de refresco en pantalla */
	int           upgr;     /* iteraciones desde el ultimo refresco */
	int           stats;	/* si TRUE se vuelca salida a fichero */
	FILE*         fevol;    /* fichero donde se vuelca la evolucion */
	FILE*         fupd;     /* iteraciones con mejora */	
	FILE*         fmej;     /* fichero donde se vuelca el mejor */
	int           semilla;  /* semilla actual */
	int           numruns;  /* número de ejecuciones */
	void*         infoProblem; /*Para almacenar la informacion del problema*/
	double        TotalEval; // Agregado por David Rodriguez.  Determina el Numero de Evaluaciones de FO

#ifdef __GRAFEVOL__
	Grafico*      evol;	/* grafico de evolucion */
#endif
#ifdef __PARALELIZACION_GLOBAL__
	EvalArg*      eval;	/* Arguments for Individuals Evaluation */
#endif
} ESInfo;			/* informacion de la estrategia de evolucion */

typedef ESInfo* ES;


typedef struct {
        int v,b,k,r,lmd,algo,vMp,nrun,van,heur,tColab;
        long nEval,TotalEval,tEval;
        double nEvalMin,prob,tti;
        clock_t tstart;
} ProblemInfo;
/*----------------*\ 
|      Macros      | 
\*----------------*/ 

/*
 * Macros to Get the parameters of an ES
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 */
#define GetID(my_es)			(my_es)->ID
#define GetPopulationSize(my_es)	(my_es)->mu
#define GetNumberOfOffspring(my_es)	(my_es)->lambda


/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Crea una ES, reservando memoria segun los valores por defecto.
 * Se le pasa un identificador.
 *
 */
ES CrearES (int ID, ESConfiguration c);

/* 
 * Libera la memoria ocupada por la ES.
 * El flag indica si se deben destruir tambien los individuos de
 * la poblacion. Normalmente, sera TRUE salvo cuando se quieran 
 * conservar estos por algun motivo (estadisticas, ...).
 *
 */
void DestruirES (ES mi_es, int flag);

/*
 * Define el tipo de seleccion
 *
 */
void DefineSeleccion (ES mi_es, TipoSeleccion sel);

                   
/* 
 * Cambia el taman~o de la poblacion de padres.
 *
 */   
void DefineTamanoPoblacion (ES mi_es, int mu);


/* 
 * Cambia el numero de descendientes.
 *
 */   
void DefineNumeroDescendientes (ES mi_es, int lambda);


/* 
 * Cambia la aridad de la recombinacion. Si no hay cruce,
 * se ignora esta llamada.
 *
 */   
void DefineAridad (ES mi_es, int aridad);


/*
 * Cambia el numero de variables contenidas en cada individuo.
 * Realoja la memoria necesaria para los limites y el taman~o de la
 * poblacion.  
 *
 */
void DefineNumeroVariables (ES mi_es, int numvar);


/* 
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 * Cambia los limites iniciales de la i-esima variable.
 *
 */
void DefineLimitesInicialesVariable (ES mi_es, int i, Variable vsup);


/*
 * Cambia los limites de la i-esima variable.
 *
 */
void DefineLimitesVariable (ES mi_es, int i, Variable vsup);


/*
 * Returns the termination status.
 *
 */
TerminationStatus Terminacion (ES mi_es);

/*
 * Forces a "SortAndUpdateBest" of the algoritm
 *
 */
void RefreshES (ES mi_es);

/*
 * Return de Computational effort of the algoritm.
 * (ANTONIO BUENO MOLINA)  v.DEC/1999
 */
#define ComputationalEffortOfES(mi_es)  ((mi_es)->generados + (mi_es)->mu)

#endif

