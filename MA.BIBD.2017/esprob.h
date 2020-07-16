/*
 * NOMBRE:	esprob.h
 * AUTOR:	Carlos Cotta
 * FECHA:	5-11-98
 * PROPOSITO:	fichero de cabecera del modulo dependiente del problema.
 *
 */

#ifndef __ES_PROB_H__
#define __ES_PROB_H__

#include "es.h"


/*-------------------*\
|      Funciones      |
\*-------------------*/

/*
 * Procesa los parametros dependientes del problema que se suministran en
 * linea de comando.
 *
 */
void ProcesarParametrosProblema (int argc, char* argv[]);

/*
 * Iteracion entre la ES y los datos del problema.
 *
 */
void IniciarProblemaES (ES mi_es);

/*
 * Destruye toda la información dependiente del problema.
 *
 */
void DestruirInfoProblema (void* info);

/*
 * Genera un individuo con informacion dependiente del problema.
 * pos es el índice del individuo (para realizar inicialización heurística
 * en algunos individuos por ejemplo), e info es la información del
 * problema.
 *
 */
void GenerarIndividuo (Individuo* i, int pos, void* info);

/*
 * Genera estadisticas dependientes del problema.
 *
 */
void GenerarEstadisticasProblema (ES mi_es);

/*
 * Muestra los parametros dependientes del problema.
 *
 */
void EscribirParametrosProblema (void* info);

/*
 * Evalua un individuo.
 *
 */
ValorFitness Evaluar (Individuo* i, ES mi_es);

/*
 * Cruce de varios individuos dependientes del problema
 *
 */
void CruceProblema (Individuo* hijo, Poblacion padres, ES mi_es);

/*
 * Se le pasa un individuo que será mutado en una posicion aleatoria.
 * El valor que tomara sera otro aleatorio entre cero y mi_es->varLSup
 */
void MutacionProblema(Individuo* ind, ES mi_es);

/*
 * Devuelve TRUE cuando se alcanza el numero de calculos equivalente
 * al de 'num' evaluaciones completas.
 */
int LimiteAlcanzado (ES mi_es);

/*
 * 
 * 
 */
void *CopiarFenotipo(void *dest, void *orig);

/*
 * 
 * 
 */
void DestruirFenotipo(void* phen);


void ResetRun(ES mi_es);


#ifdef __GRAFSOL__
/*
 * Actualiza los graficos del problema que hubiere.
 *
 */
void ActualizarGraficosProblema (ES mi_es);
#endif

#endif
