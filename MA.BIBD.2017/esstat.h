/*
 * NOMBRE:    esstat.h
 * AUTOR:     Carlos Cotta
 * FECHA:     5-11-98
 * PROPOSITO: Fichero de cabecera del modulo de estadisticas.
 *
 */
 
#ifndef __ES_STAT_H__
#define __ES_STAT_H__

#include "es.h"


/*----------------*\
|    Funciones     |
\*----------------*/

/* 
 * Activa las estadisticas a fichero. 
 * 
 */ 
void ActivarEstadisticas (ES mi_es); 

/*
 * Desactive las estadisticas a fichero.
 *
 */ 
void DesactivarEstadisticas (ES mi_es); 

/*
 * Genera estadisticas de la poblacion.
 *
 */
void GenerarEstadisticas (ES mi_es);

#endif
