/*
 * NOMBRE:    BLocal.c
 * AUTOR:     David Rodriguez
 * FECHA:     01-10-08
 * PROPOSITO: modulo para las operaciones de Busqueda Local.
 *
 */

#include <stdio.h>   	/* para printf */
#include <stdlib.h>  	/* para exit */
#include <windows.h>
#include <values.h>
//#include <dos.h>
#include <time.h>
#include "es.h"
#include "esinitp.h"
#include "util.h"	/* TRUE, FALSE, MAXFLOAT */ 
#include "random.h"	/* para URand01 */
#include "esprob.h"
#include "esreemp.h"
#include "esindiv.h"
#include "espop.h"
#include "BLocal.h"

#ifdef __PARALELIZACION_GLOBAL__
#include "esthread.h"
#endif

clock_t st;
float cpu;
int kxSeed=0;
float convert_time(clock_t t_start,clock_t t_end)
{ 
    float xx;

    xx = (float)(t_end - t_start)/CLOCKS_PER_SEC;
    return(xx);
}

///////////////////////////////////////////////////////////////////////////////////////
// Leer Solucion Colaborativa
///////////////////////////////////////////////////////////////////////////////////////
void GetSolution(Individuo* i, ProblemInfo *p)
{
   FILE *fpP;
   int k,xpos;
   fpP=fopen("tempo.txt","rt");
   if(!fpP)
     {
         printf("\n!!!!!Error taking Collaborative Solution!!!!!!");
         system("pause");
         return;
     }
   fscanf(fpP,"%d",&xpos); 
   k=0;
   if(xpos==-1)
     {
       printf("\n!!!!!Error taking Collaborative Solution!!!!!!\n");
       system("pause");
       exit(0);
    }
   for(int fi=0;fi<p->v*p->b;fi++)
    {
       fscanf(fpP,"%d",&xpos);
       if(xpos==-1)
          break;
       else
          {
            i->genoma[fi]=xpos;
            k++;
          }
    }
  fclose(fpP);
  if(k<p->v*p->b)
     {
        printf("\n!!!!!!Fatal Error!!!!!");
        system("pause");
        exit(0);
     }
}

///////////////////////////////////////////////////////////////////////////////////////
/////                 Colaborative Diversification Function                    ////////
///////////////////////////////////////////////////////////////////////////////////////
void DColaborative(Individuo* i, ProblemInfo *pp)
{
   int fi,co,ki;
   solution xcand;
   FILE *fp;
   HINSTANCE pLib1;
   typedef void (WINAPI *hFunc1)(int,int,int,int,int,float,int,int);
   hFunc1 hDiversifica;
   pLib1 = LoadLibrary("inicpool.dll");
   if(!pLib1)
     {
       printf("\nError al Tomar Puntero de la DLL\n");
       FreeLibrary(pLib1);
       system("pause");
     }       
    fp = fopen ("tempo.txt","wt");
    Esc_Sol(i,pp,fp);
    fclose(fp);

    hDiversifica=(void (__stdcall *) (int,int,int,int,int,float,int,int)) GetProcAddress(pLib1,"Cdiversification");
    hDiversifica(pp->v,pp->b,pp->r,pp->k,pp->lmd,pp->tColab*1.0,kxSeed++,9);
    FreeLibrary(pLib1);

    GetSolution(i,pp);
    i->fitness=EvaluarRestricion(i,pp);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion de evaluacion del Fitness del Individuo
///////////////////////////////////////////////////////////////////////////////////////////
ValorFitness EvaluarRestricion(Individuo* i, ProblemInfo *p){
    int j,k,r,s;
    int fila[100],col[100];
    
    i->fitness = 0;
    for (j=0; j<p->v; j++)
       fila[j]=0;
    for (j=0; j<p->b; j++)
       col[j]=0;
    r=0;
    for (j=0; j<p->v; j++)
      for(k=0;k<p->b;k++)
        {
           fila[j]+=i->genoma[r];
           col[k]+=i->genoma[r];
           r++;
        }
    for (j=0; j<p->v; j++)
      for(k=j+1;k<p->v;k++)
        {
           s=0;
          for(r=0;r<p->b;r++)
            s+=i->genoma[(j)*p->b+r]*i->genoma[(k)*p->b+r];
          i->fitness+=abs(s-p->lmd);
        }
    for (j=0; j<p->v; j++)
       i->fitness+=abs(fila[j]-p->r);
    for (j=0; j<p->b; j++)
       i->fitness+=abs(col[j]-p->k);
    return i->fitness;
}/*END*/

////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion de evaluacion del Fitness del Individuo
///////////////////////////////////////////////////////////////////////////////////////////
void Imp_Indiv(Individuo* ind, ProblemInfo *p)
{
  int i,j,k=0;
  printf("\n\n");
  for(i=0;i<p->v;i++)
   {
     for(j=0;j<p->b;j++)
      {
        printf("%2d",ind->genoma[k]);
        k++;
      }
     printf("\n");
  }
 printf("\n\nFitness=  %5.1lf",ind->fitness);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion de evaluacion del Fitness del Individuo
///////////////////////////////////////////////////////////////////////////////////////////
void Esc_Indiv(Individuo* ind, ProblemInfo *p,FILE *fp,double kEvalMin)
{
  int i,j,k=0;
  fprintf(fp,"\n%d  %12ld   %10.0lf  %8.0lf  ",p->nrun,p->TotalEval,kEvalMin,ind->fitness);
  for(i=0;i<p->v*p->b;i++)
   {
        fprintf(fp,"%2d",ind->genoma[k]);
        k++;
    }
}   

void Write_Indiv(Individuo* ind, ProblemInfo *p,FILE *fp,double kEvalMin)
{
  int i,j,k=0;
  fprintf(fp,"\n%d  %12ld   %10.0lf  %8.0lf  ",p->nrun,p->tEval,kEvalMin,ind->fitness);
  for(i=0;i<p->v*p->b;i++)
   {
        fprintf(fp,"%2d",ind->genoma[k]);
        k++;
    }
}   

////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion Escribe Individuo en Archivo
///////////////////////////////////////////////////////////////////////////////////////////
void Esc_Sol(Individuo* ind, ProblemInfo *p,FILE *fp)
{
  int i,j,k=0;
  fprintf(fp,"%d\n",p->v);
  for(i=0;i<p->v*p->b;i++)
   {
        fprintf(fp,"%2d",ind->genoma[k]);
        k++;
    }
}     




///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Hill Climbing
// Consideramos la Vecindad Swap Bits (2 bits 0-->1 or  1-->0 
/////////////////////////////////////////////////////////////////////////////////////////////////
void HcSwap(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval) //,FILE *fp)
{

  int nc=0,enc=FALSE;
  double nu=0,dif=p->v*p->r*(p->b-p->r)*(2+p->v-1)/(p->v+p->b+p->v*(p->v-1)/2);
  
  while((nu<(double)xNum_Eval)&&!enc)  
   {
     if(ind->fitness<(double)1)
          enc=TRUE;
     else
         enc=!vecinos(ind,p);
     nu+=dif;
   }

  p->nEvalMin+=nu;

    xNum_Eval=(long)nu;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera operar sobre las Filas de la matriz  (Hacer swap sobre cada par de (0,1) o (1,0)
/////////////////////////////////////////////////////////////////////////////////////////////////
int  vecinos(Individuo *ind,ProblemInfo *p)
{
  int i=0,j,fi,sw=FALSE,t=0;
  Individuo *vecino,*zmajor;
  vecino=CrearIndividuo(p->v*p->b);
  zmajor=CrearIndividuo(p->v*p->b);

  CopiarIndividuo(zmajor,ind);
  for(fi=0;fi<p->v;fi++)
    {
      
      for(i=0;i<p->b;i++)
        for(j=i+1;j<p->b;j++)
          {
            if(ind->genoma[fi*p->b+i]+ind->genoma[fi*p->b+j]==1) 
             {
               CopiarIndividuo(vecino,ind);
               vecino->genoma[fi*p->b+i]=1-vecino->genoma[fi*p->b+i];
               vecino->genoma[fi*p->b+j]=1-vecino->genoma[fi*p->b+j];
               vecino->fitness=EvaluarRestricion(vecino,p);
               if(vecino->fitness<zmajor->fitness)
                 {
                   CopiarIndividuo(zmajor,vecino);
                   sw=TRUE;
                 }
             }
          }
    }
  if(sw)
    CopiarIndividuo(ind,zmajor);
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;

}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Hill Climbing
// Consideramos la Vecindad Bit.Flip 
/////////////////////////////////////////////////////////////////////////////////////////////////
void HcBitFlip(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval)
{
  int nc=0,enc=FALSE;
  double nu=0,dif=(p->v*p->b*(2+(p->v-1)))/(p->v+p->b+p->v*(p->v-1)/2);
 
  while((nu<xNum_Eval)&&!enc)  
   {
     if(ind->fitness<1)
          enc=TRUE;
     else
         enc=!vecinosBF(ind,p);
     nu+=dif;
   }
 
  p->nEvalMin+=nu;
  xNum_Eval=(long)(nu);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera operar sobre las Filas de la matriz  (Hacer Bit Flip sobre cada Bit
/////////////////////////////////////////////////////////////////////////////////////////////////
int vecinosBF(Individuo *ind,ProblemInfo *p)
{
  int i=0,j,fi,sw=FALSE,t=0;
  Individuo *vecino,*zmajor;
  vecino=CrearIndividuo(p->v*p->b);
  zmajor=CrearIndividuo(p->v*p->b);

  CopiarIndividuo(zmajor,ind);
  for(fi=0;fi<p->v*p->b;fi++)
    {
      CopiarIndividuo(vecino,ind);
      vecino->genoma[fi]=1-vecino->genoma[fi];
      vecino->fitness=EvaluarRestricion(vecino,p);
      if(vecino->fitness<zmajor->fitness)
       {
         CopiarIndividuo(zmajor,vecino);
         sw=TRUE;
       }
    }
  if(sw)
    CopiarIndividuo(ind,zmajor);
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;   
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Tabu Search
// Consideramos la Vecindad Swap Bits (2 bits 0-->1 or  1-->0 
/////////////////////////////////////////////////////////////////////////////////////////////////
void TsSwap(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,long nEvalIntens)
{
  //Individuo *aqui;
  TabuList *Listas;
  double sinMejora=0;
  int enc=FALSE,change=0; 
  double nu=0,dif=p->v*p->r*(p->b-p->r)*(2+p->v-1)/(p->v+p->b+p->v*(p->v-1)/2);
  Listas=(TabuList *)malloc(sizeof(TabuList)*(p->v));
  for(int i=0;i<p->v;i++)
    Listas[i].Lt=(int *)malloc(sizeof(int)*(p->b*p->b));
  InicTl(Listas,p->v,p->b);

   int b1,b2,bdif;
   b1=(p->v*p->b*p->r)/2;
   b2=3*(p->v*p->b*p->r)/2;
   time_t ttt;
   bdif=b2-b1;
  Individuo *xmejor;
  xmejor=CrearIndividuo(p->v*p->b);
  CopiarIndividuo(xmejor,ind);
  double xEvalIntens=(double)nEvalIntens*dif;
  //***********************************************************
  while((nu<xNum_Eval)&&!enc) 
   {
     if(ind->fitness<1)
          enc=TRUE;
     else
        {                                      
          vecinosTsSw(ind,p,Listas,xmejor,URand(bdif)+b1);
          if(ind->fitness<xmejor->fitness)
            { 
               sinMejora=0;
               CopiarIndividuo(xmejor,ind);
               p->nEvalMin=nu+dif;
            }
          else
            {
                  sinMejora+=dif;
                  if(sinMejora>xEvalIntens&&xmejor->fitness>0)
                   {
                     CopiarIndividuo(ind,xmejor);
                     if(p->heur==9)
                      {
                        if(change<3)
                         {
                           DColaborative(ind,p); // Colaborative Diversification
                           if(ind->fitness<xmejor->fitness)
                             { 
                               sinMejora=0;
                               CopiarIndividuo(xmejor,ind);
                               p->nEvalMin=nu+dif;
                             }
                           change++;
                         }
                        nu+=dif*3;
                      }
                     sinMejora=0;
                   }
             }
            
        }
     nu+=dif;
    
   }

  xNum_Eval=(long)(nu);
  if(xmejor->fitness<ind->fitness)
   {
     CopiarIndividuo(ind,xmejor);
     p->nEvalMin=p->nEval;
   }

  for(int i=0;i<p->v;i++)
    if(Listas[i].Lt)
      free(Listas[i].Lt);
  free(Listas);
  DestruirIndividuo(xmejor);
}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera operar sobre las Filas de la matriz  (Hacer Swap Bits
/////////////////////////////////////////////////////////////////////////////////////////////////
int vecinosTsSw(Individuo *ind,ProblemInfo *p,TabuList *List,Individuo *xmejor,int mov)
{
   int max_mov=mov,primero=1; // max_mov sera el valor tenure de las Lista Tabu
   int i,j,fi,xcan=0,sw=FALSE,posfi,posi,posj;
   Individuo *vecino,*zmajor;
   vecino=CrearIndividuo(p->v*p->b);
   zmajor=CrearIndividuo(p->v*p->b);
   CopiarIndividuo(zmajor,ind);

   for(fi=0;fi<p->v;fi++)
    {
      for(i=0;i<p->b;i++)
        for(j=i+1;j<p->b;j++)
          {
            
            if(ind->genoma[fi*p->b+i]+ind->genoma[fi*p->b+j]==1) 
             {
               int *aux=List[fi].Lt+i*p->b,tabu;
               tabu=aux[j]; 
               CopiarIndividuo(vecino,ind);              
               vecino->genoma[fi*p->b+i]=1-vecino->genoma[fi*p->b+i];
               vecino->genoma[fi*p->b+j]=1-vecino->genoma[fi*p->b+j];
               vecino->fitness=EvaluarRestricion(vecino,p);
               if(tabu>0)
                {
                  if(vecino->fitness<xmejor->fitness)
                   {
                    CopiarIndividuo(zmajor,vecino);
                    posfi=fi;
                    posi=i;
                    posj=j;
                    sw=TRUE;
                   }
                }
               else
                  if(tabu==0)
                    {
                      if(primero)
                      {
                          CopiarIndividuo(zmajor,vecino);
                          posfi=fi;
                          posi=i;
                          posj=j;
                          primero=0;
                          sw=TRUE;
                      }else
                      if(vecino->fitness<zmajor->fitness)
                       {
                         CopiarIndividuo(zmajor,vecino);
                         posfi=fi;
                         posi=i;
                         posj=j;
                         sw=TRUE;
                       }
                    }
                 
             }
          }
    }
  if(sw)
    {
      CopiarIndividuo(ind,zmajor);
      // Actualizamos Lista Tabu
      int *aux=List[posfi].Lt+posi*p->b;
      aux[posj]=max_mov+1;              
    }
  decLt(List,p->v,p->b); // Decrementamos Lista Tabu
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;  
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Tabu Search
// Consideramos la Vecindad Bit.Flip 
/////////////////////////////////////////////////////////////////////////////////////////////////
void TsBF(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,long nEvalIntens)//,FILE *fp)
{
  //Individuo *aqui;
  TabuList *Listas;
  double sinMejora=0;
  int nc=0,enc=FALSE,primero=1;
  double nu=0,dif=p->v*p->b*(2+p->v-1)/(p->b+p->v*(p->v-1)/2);
  Listas=(TabuList *)malloc(sizeof(TabuList)*(p->v));
  for(int i=0;i<p->v;i++)
    Listas[i].Lt=(int *)malloc(sizeof(int)*(p->b));
  InicTlFlip(Listas,p->v,p->b);
    //**************************************
  // Determinamos el Valor Tenure de la Lista Tabu   
   int b1,b2,bdif;
   b1=(p->v*p->b*p->r)/2;
   b2=3*(p->v*p->b*p->r)/2;
   time_t ttt;

   bdif=b2-b1;

     // Opcion con reinicio desde otro punto (aleatorio)
  Individuo *xmejor,*punto;
  xmejor=CrearIndividuo(p->v*p->b);
  punto=CrearIndividuo(p->v*p->b);
  CopiarIndividuo(xmejor,ind);
  CopiarIndividuo(punto,ind);
  //***********************************************************
  while((nu<xNum_Eval)&&!enc)  // Para esta instancia se prueba que con 500 vueltas es
   {
     if(ind->fitness<1)
          enc=TRUE;
     else
        {
          vecinosTsBF(ind,p,Listas,xmejor,punto,URand(bdif)+b1);
          if(ind->fitness<xmejor->fitness)
            {
               sinMejora=0;
              CopiarIndividuo(xmejor,ind);
              p->nEvalMin=nu+dif;
           
            }
          else
            {
                  sinMejora+=dif;
                  if(sinMejora>nEvalIntens){
                     CopiarIndividuo(ind,xmejor);
                     sinMejora=0;
           
                     }
             }
        }
     nu+=dif;
     
   }
  xNum_Eval=(long)(nu);
  if(xmejor->fitness<ind->fitness)
     CopiarIndividuo(ind,xmejor);

  for(int i=0;i<p->v;i++)
    if(Listas[i].Lt)
      free(Listas[i].Lt);
  free(Listas);
  DestruirIndividuo(xmejor);
  DestruirIndividuo(punto);
 
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera aplicar Bit Flip
/////////////////////////////////////////////////////////////////////////////////////////////////
int vecinosTsBF(Individuo *ind,ProblemInfo *p,TabuList *List,Individuo *xmejor,Individuo *punto,int mov)
{
    int max_mov=mov,primero=1;  // max_mov == Valor Tenure de laLista Tabu
   int i,j,fi,xcan=0,sw=FALSE,posi,posj;
   Individuo *vecino,*zmajor;
   int b1,b2,bdif;

   vecino=CrearIndividuo(p->v*p->b);
   zmajor=CrearIndividuo(p->v*p->b);

   CopiarIndividuo(zmajor,ind);

   for(fi=0;fi<p->v*p->b;fi++)
    {
      i=(int)(fi/p->b);
      j=(int)(fi%p->b);
      CopiarIndividuo(vecino,ind);
      vecino->genoma[fi]=1-vecino->genoma[fi];
      vecino->fitness=EvaluarRestricion(vecino,p);
      int tabu=List[i].Lt[j];
    
      if(tabu>0)
        {
           if(vecino->fitness<xmejor->fitness)
             {
               CopiarIndividuo(zmajor,vecino);
               posi=i;
               posj=j;
               sw=TRUE;
             }
        }
      else
        if(tabu==0)
            {
              if(primero)
                {
                 CopiarIndividuo(zmajor,vecino);
                 posi=i;
                 posj=j;
                 primero=0;
                 sw=TRUE;
             }else
               if(vecino->fitness<zmajor->fitness)
                 {
                   CopiarIndividuo(zmajor,vecino);
                   posi=i;
                   posj=j;
                   sw=TRUE;
                 }
          }
    }
  if(sw)
    {
      CopiarIndividuo(ind,zmajor);
      // Actualizamos Lista Tabu
      List[posi].Lt[posj]=max_mov+1;              
      
    }
  decLtFlip(List,p->v,p->b); // Decrementamos Lista Tabu
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;  
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Inicializar la Lista Tabu
/////////////////////////////////////////////////////////////////////////////////////////////////
void InicTl(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
   {
     for(unsigned int i=0;i<tList;i++)
       {
          int *aux=List[t].Lt +i*tList;
          for(unsigned int j=i+1;j<tList;j++)
            aux[j]=0;
        }
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para DEcrementar la Lista Tabu
/////////////////////////////////////////////////////////////////////////////////////////////////
void decLt(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
   {
     for(unsigned int i=0;i<tList;i++)
       {
          int *aux=List[t].Lt +i*tList;
          for(unsigned int j=i+1;j<tList;j++)
            if(aux[j]>0)
              aux[j]--;
       }
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Inicializar la Lista Tabu (caso Bit Flip)
/////////////////////////////////////////////////////////////////////////////////////////////////
void InicTlFlip(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
    for(unsigned int i=0;i<tList;i++)
       List[t].Lt[i]=0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para DEcrementar la Lista Tabu 
/////////////////////////////////////////////////////////////////////////////////////////////////
void decLtFlip(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
    for(unsigned int i=0;i<tList;i++)
      if(List[t].Lt[i]>0)
         List[t].Lt[i]-=1;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Realizar Una reparacion por Columnas a un Individuo con 'r' unos por Fila
/////////////////////////////////////////////////////////////////////////////////////////////////
void ReparaCol(Individuo *ind,ProblemInfo *p)
{
   int j,k,r,i,cp,ka;
   int col[100],point[100];
   Individuo *cand,*xmej;
   cand=CrearIndividuo(p->v*p->b);
   xmej=CrearIndividuo(p->v*p->b);
   CopiarIndividuo(cand,ind);
   CopiarIndividuo(xmej,ind);
   for (j=0; j<p->b; j++)
     col[j]=0;
   r=0;
   for (i=0; i<p->v; i++)
     for(j=0;j<p->b;j++)
      {
        col[j]+=ind->genoma[r];
        r++;
      }
   for(j=0;j<p->b;j++)
     col[j]-=p->k;
   while(1)
   {
     i=0; //punto donde sobran unos
     while(i<p->b&&col[i]<=0)
       i++;
     j=0; //punto donde faltan unos
     while(j<p->b&&col[j]>=0)
       j++;
     if(i>=p->b||j>=p->b)
       break;
     else
      {
        cp=0;// buscamos un par de (1,0) en la misma fila
        for(ka=0;ka<p->v;ka++) //para hacer swap y evaluar
         {
           k=ka*p->b+i; // [k,r] = Posicion en Genoma a evaluar celda de la matriz
           r=ka*p->b+j;  
           if(ind->genoma[k]==1&&ind->genoma[r]==0)
            {
              point[cp]=ka; // fila donde hay un par de (1,0)
              cp++;  // Cantidad de puntos factibles de swap
            }        // Num. de candidatos
          }
         if(cp)
           {
             CopiarIndividuo(xmej,ind);
             ka=URand(cp);
             CopiarIndividuo(cand,ind);
             cand->genoma[point[ka]*p->b+i]=0;
             cand->genoma[point[ka]*p->b+j]=1;
              CopiarIndividuo(xmej,cand); //candidato con menor fitness
              CopiarIndividuo(ind,xmej);
              col[i]--;
              col[j]++;
           }
      }
   }
  DestruirIndividuo(cand);
  DestruirIndividuo(xmej);
  ind->fitness=EvaluarRestricion(ind,p);
}



