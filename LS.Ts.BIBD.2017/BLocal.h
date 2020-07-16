/*
 * NOMBRE:    BLocal.h
 * AUTOR:     David Rodriguez
 * FECHA:     01-10-08
 * PROPOSITO: Fichero de cabecera del modulo de Busqueda Local
 *
 */
 
#ifndef __ES_BUSQUEDA_H__
#define __ES_BUSQUEDA_H__

#include <stdio.h>
#include <time.h>

typedef struct {   // Lista Tabu
                 int *Lt;
                 }TabuList;
/////////////////////////
#define MAXI 20
#define MAX_VAL 120
//clock_t st,ed,tt;
//double cpu=0.0;
typedef struct para{
          int v,b,k,r,lmd,seed,cual;
          float tiempo;
        };
typedef struct solution{
         int gen[MAX_VAL][MAX_VAL],xi[MAX_VAL];
         int fit,tabu,tl;
         };

////////////////////////////////////
// Lee Solucion
void GetSolution(Individuo* i, ProblemInfo *p,char *xarch);
////////////////////////////////////


////////////////////////////////////
// Cualcula CPU Time
float elapsed_time(clock_t t_start,clock_t t_end);

//////////////////////////////////////


// Hill Climbing con Swap
void HcSwap(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,FILE *fp);
int vecinos(Individuo *ind,ProblemInfo *p);


ValorFitness EvaluarRestricion(Individuo* i, ProblemInfo *p);

// Hill Climbing con Bit-Flip
void HcBitFlip(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,FILE *fp);
int vecinosBF(Individuo *ind,ProblemInfo *p);

// Tabu Search con Swap
void TsSwap(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,long,FILE *fp);
int vecinosTsSw(Individuo *ind,ProblemInfo *p,TabuList *List,Individuo *xmejor,int mov);

void InicTl(TabuList *List,unsigned int nList,unsigned int tList);
void decLt(TabuList *List,unsigned int nList,unsigned int tList);

void InicTlFlip(TabuList *List,unsigned int nList,unsigned int tList);
void decLtFlip(TabuList *List,unsigned int nList,unsigned int tList);

// Tabu Search con Bit-Flip
void TsBF(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,long ,FILE *fp);
int vecinosTsBF(Individuo *ind,ProblemInfo *p,TabuList *List,Individuo *xmejor,Individuo *punto,int );

void Imp_Indiv(Individuo* ind, ProblemInfo *p);
void Esc_Indiv(Individuo* ind, ProblemInfo *p,FILE *fp,double);
void Esc_Sol(Individuo* ind, ProblemInfo *p,FILE *fp);

////////////////////////////
// Reparacion de Columnas para acomodar los k unos por columnas
void ReparaCol(Individuo *ind,ProblemInfo *p);


#endif
