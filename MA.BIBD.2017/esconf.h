/*
 * NOMBRE:    esconf.h
 * AUTOR:     Carlos Cotta
 * FECHA:     08-03-99
 * PROPOSITO: Fichero de cabecera del modulo de configuracion.
 *
 * MODIFICADO: ANTONIO BUENO MOLINA  v.DEC/1999
 */
 
#ifndef __ES_CONFIGURATION_H__
#define __ES_CONFIGURATION_H__

#include <stdio.h>
#include "esindiv.h"

/*----------------*\
|      Tipos       |
\*----------------*/

typedef enum {
	ORTOGONAL    = 0,
	NO_ORTOGONAL = 1
} TipoInicPob;			/* tipo de generacion inicial de la poblacion */

typedef enum {
	RANDOM_CON_REP = 0,	/* seleccion aleatoria con reemplazo */
	RANDOM_SIN_REP = 1,	/* seleccion aleatoria sin reemplazo */
	TOURNAMENT     = 2, /* se toman TSize al azar y se elige el mejor */
	PLAIN          = 3  /* selección como en EP -> cada individuo un hijo */
} TipoSeleccion;

typedef enum {			
	PLUS = 0, 
	COMMA = 1,
	PLUS_ND = 2,
	COMMA_ND = 3
} TipoReempl;			/* tipo de reempl en el algoritmo */

typedef enum {
	MAXIMIZAR = 0,
	MINIMIZAR = 1
} SentidoOpt;			/* tipo de optimizacion */

typedef enum {
	SIN_CRUCE = 0,		/* solo mutacion */
	UX        = 1, 		/* cruce uniforme    */
	SPX       = 2, 		/* cruce de 1 punto  */
	DPX       = 3, 		/* cruce de 2 puntos */
	PROB      = 4		/* cruce dependiente del problema */
} TipoCruce;

typedef enum{
	SIN_MUTACION        = 0,
	RANDOM_SUBSTITUTION = 1,
	PROB_MUTATION       = 2
} TipoMutacion;

typedef enum {
	MUTREC = 0,		/* se muta y luego se recombina */
	RECMUT = 1		/* se recombina y luego se muta */
} TipoCiclo;			/* tipo de ciclo evolutivo */


typedef struct {
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
	ValorFitness  stopAt;   /* desired fitness value */
	TipoSeleccion select;	/* tipo de seleccion */
    int           TSize;    /* Tamaño del torneo */
 	int           actg;     /* interval for refresh update */
	int           stats;	/* flag: statistics activated? */
} ESConfigurationInfo;
 

typedef ESConfigurationInfo* ESConfiguration;


/*----------------*\
|    Funciones     |
\*----------------*/


/*
 * Crea una estructura de configuracion por defecto.
 *
 */
ESConfiguration CreateESConfiguration (void);

/*
 * Destruye la informacion de configuracion
 *
 */
void DestroyESConfiguration (ESConfiguration c);
 
/*
 * Lee la configuracion de un fichero.
 * Devuelve 0 si hubo algun error.
 *
 */
int ReadESConfigurationFromFile (FILE* f, ESConfiguration c);


/*
 * Define el tamanio de la poblacion.
 *
 */
#define SetConfigPopulationSize(c, nmu) (c)->mu = (nmu)

/*
 * Devuelve el tamanio de la poblacion.
 *
 */
#define GetConfigPopulationSize(c) (c)->mu 


/*
 * Define el numero de descendientes.
 *
 */
#define SetConfigNumberOfOffspring(c, nlambda) (c)->lambda = (nlambda)


/*
 * Devuelve el numero de descendientes.
 *
 */
#define GetConfigNumberOfOffspring(c) (c)->lambda


/*
 * Define el numero de variables.
 *
 */
#define SetConfigNumberOfVariables(c, l) (c)->longen = (l)


/*
 * Devuelve el numero de variables.
 *
 */
#define GetConfigNumberOfVariables(c) (c)->longen


/*
 * Devuelve el intervalo de actualizacion grafica.
 *
 */
#define GetConfigRefreshInterval(c) (c)->actg

/*
 * Define el intervalo de actualizacion grafica.
 *
 */
#define SetConfigRefreshInterval(c, a) (c)->actg = (a)


/*
 * Devuelve el sentido de optimizacion 
 *
 */
#define GetConfigOptimisationSense(c) (c->sOpt)


#endif

