/*
 * NOMBRE:    esdefaul.h
 * AUTOR:     Carlos Cotta
 * FECHA:     25-02-99
 * PROPOSITO: valores por defecto de los parametros de una estrategia
 *            de evolucion.
 *
 */

#ifndef __ES_DEFAULT_H__
#define __ES_DEFAULT_H__

#define MU        16    		/* tamanio de la poblacion    */
#define LAMBDA    100	        /* numero de descendientes    */
#define NUMIND    100000		/* numero de evaluaciones     */
#define CRUCE     SIN_CRUCE	    /* no hay cruce               */
#define MUTACION  RANDOM_SUBSTITUTION
#define PROBCRUCE 0.9
#define PROBMUTACION 0.01
#define CICLO     MUTREC  		/* mutacion + recombinacion   */
#define REEMP     COMMA	        /* estrategia 'comma'         */
#define SELECT    RANDOM_CON_REP     /* aleatoria con repeticion   */
#define SOPT      MINIMIZAR		/* minimizacion               */
#define	INICPOB   ORTOGONAL		/* inicializacion ortogonal   */
#define LONGEN    25			/* 25 variables               */
#define ARIDAD    2			    /* recombinacion binaria      */
#define ACTG      0			    /* sin actualizacion grafica  */
#define MAXVAR    2			    /* limite superior variables  */
#define STATS     FALSE			/* no hay estadisticas        */
#define DEFPOP    FALSE			/* no se crean los individuos */ 
					            /* de la poblacion            */
	
#endif
