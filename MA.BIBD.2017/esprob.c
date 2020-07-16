/*
 * NOMBRE:	esprob.c
 * AUTOR:	Carlos Cotta
 * FECHA:	5-11-98
 * PROPOSITO:	modulo dependiente del problema.
 *
 */

#include <stdio.h>	/* para printf */
#include <stdlib.h>	/* para exit   */
#include <string.h> /* para strdup, memcpy */
#include <time.h>
#include "es.h"
#include "BLocal.h"
#include "esprob.h"
#include "util.h"
#include "random.h"
#include "esshell.h"



#ifdef __GRAFSOL__
#include "multigraf.h"
#endif


#define FSOL "bestsol.log"




/*------------------*\
|     Variables      |
\*------------------*/

#ifdef __GRAFSOL__
Grafico* sol;
#endif
int v,b,k,r,lmd,algo,heur;
long nEval;
double prob,nodos;
int restart = 0;
//int prob;


/*------------------*\
|      Funciones     |
\*------------------*/

/*
 * Procesa los parametros dependientes del problema que se suministran
 * en linea de comando.
 *
 */
#ifdef __LINUX__
#pragma argused
#endif
void ProcesarParametrosProblema (int argc, char* argv[]){
/*BEGIN*/

    int i;
    
#ifdef __GRAFSOL__
	sol = CreaGrafico (FSOL, 2, 1);
	DefineVariable (sol, 1 ,2);
	DefineTitulo (sol, 1, "mejor solucion");
	DefineVariable (sol, 2, 3);
	DefineTitulo (sol, 2, "solucion actual");
#endif

    if (argc < 9) {
       printf ("\nFaltan parámetros: v b r k lmd T.Algoritmo(0=TsBf,1=TsSw) prob.LS\n");
       system("pause");
       exit(1);
    }
    
    v = atoi(argv[0]);
    b = atoi(argv[1]);
    r = atoi(argv[2]);
    k  = atoi(argv[3]);
    lmd = atoi(argv[4]);
    algo = atoi(argv[5]);
    prob=atof(argv[6]);
    nodos=atof(argv[7]); // Numero de Evaluaciones totales a realizar
    heur=atoi(argv[8]); // Tipo de Incializacion (0=Aleatoria 9=Cooperativo)
    nEval=v*b*(v+1)/(v+b+v*(v-1)/2)*v*b;
}/*END*/

/*
 * Interaccion entre la ES y los datos del problema
 *
 */
void IniciarProblemaES (ES mi_es){
/*BEGIN*/
         ProblemInfo* p;
         int i;
         
         p = (ProblemInfo*) malloc (sizeof(ProblemInfo));
         if (!p) {
            printf ("\nERROR: No hay memoria para crear ProblemInfo (mi_es->ID).\n");
            system("pause");
            exit(1);
         }
         p->v = v;
         p->b = b;
         p->r = r;
         p->k = k;
         p->lmd = lmd;
         p->nEval=nEval;
         p->tEval=0;
         p->prob=prob;
         p->algo=algo;
         p->heur=heur;
         p->vMp=(v+b+(v*(v-1))/2)/(1+v);
         
         mi_es->infoProblem = (void*)p;
         DefineNumeroVariables(mi_es, v*b);
         for (i=0; i<mi_es->longen; i++)
                  DefineLimitesVariable(mi_es, i, 2);
         mi_es->stopAt = 1;
         mi_es->numind=(long)(nodos/(v+b+(v*(v-1))/2)*(1+v));
         p->tColab=(int)(ceil(nodos*0.000003));
         mi_es->pmutacion=1.0/(double)(v*b);
}/*END*/

/*
 * Destruye toda la informacion dependiente del problema
 *
 */
void DestruirInfoProblema (void* info){
/*BEGIN*/
    if (!info) {
        printf ("\nDestruirProblem: info should not be NULL...\n");
        system("pause");
        exit(1);
    }
    free(((ProblemInfo*)info));
#ifdef __GRAFSOL__
	CierraGrafico(sol);
#endif
}/*END*/


/*
 * Genera un individuo con informacion dependiente del problema.
 *
 */
void GenerarIndividuo (Individuo* i, int pos, void* info)
{
     ProblemInfo *p;
     p=(ProblemInfo *)info;
     int *vec,k,j,xpos=0;
     FILE *fpP;
     switch(p->heur)
      {
         case 0: vec=(int *)malloc(sizeof(int)*(p->b));
                 do{
                    for(j=0;j<p->b;j++)
                      vec[j]=0;
                    j=0;
                    while(j<p->r)
                     {
                       k=URand(p->b);
                       int enc=0;
                       if(vec[k]==0)
                        {
                          vec[k]=1;
                          j++;
                        }
                    }
                  for(j=0;j<p->b;j++)
                   {
                     AsignarGen(i,xpos,vec[j]);
                     xpos++;
                   }
                 }while(xpos<(p->v*p->b));
                 free(vec);
                 break;
         case 9: 
                 fpP=fopen("temp.txt","rt");
                 if(!fpP)
                  {
                    printf("\nError al Intentar tomar Solucion del Cooperador :: temp.txt");
                    system("pause");
                    return;
                  }
                 fscanf(fpP,"%d",&xpos); 
                 if(pos<xpos)
                  {
                     for(j=0;j<pos+1;j++)
                      {
                        k=0;
                        for(int fi=0;fi<p->v*p->b;fi++)
                         {
                           fscanf(fpP,"%d",&xpos);
                           if(xpos==-1)
                             break;
                           else
                           {
                             AsignarGen(i,k,xpos);
                             k++;
                           }
                         }
                      }
                  }
                 else
                  {
                    xpos=0;
                    vec=(int *)malloc(sizeof(int)*(p->b));
                    do{
                        for(j=0;j<p->b;j++)
                          vec[j]=0;
                        j=0;
                        while(j<p->r)
                         {
                           k=URand(p->b);
                           int enc=0;
                           if(vec[k]==0)
                            {
                              vec[k]=1;
                              j++;
                            }
                         }
                        for(j=0;j<p->b;j++)
                         {
                           AsignarGen(i,xpos,vec[j]);
                           xpos++;
                         }
                     }while(xpos<(p->v*p->b));
                   free(vec);
                  } 
                 fclose(fpP);
                 
      }
         
        
               
	/* To be implemented if needed */
    //    printf ("ERROR: GenerarIndividuo not implemented\n");
    //    exit(1);
}

/*
 * Genera estadisticas dependientes del problema.
 *
 */
#ifdef __LINUX__
#pragma argused
#endif
void GenerarEstadisticasProblema (ES mi_es)
{
     Individuo* ind;
     int i;
     int PRESERVAR = (int)(GetPopulationSize(mi_es)*.1);
     ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
     if ((mi_es->desierto * (1.0 + (double)p->tEval/(double)mi_es->generados)) > ((restart+1)*mi_es->numind/10)) {
        for (i=PRESERVAR; i<GetPopulationSize(mi_es); i++) {
            ind = GetSortedIndividual (mi_es, i);
            GenerarIndividuo(ind, i, mi_es->infoProblem);
            ind->fitness = Evaluar(ind, mi_es);
            mi_es->generados++;
        }
        restart++;
     }
}



/*
 * Muestra los parametros dependientes del problema.
 *
 */
void EscribirParametrosProblema (void* info){
/*BEGIN*/
	printf ("pbibd - (%d,%d,%d,%d,%d)\n", ((ProblemInfo*)info)->v,((ProblemInfo*)info)->b,((ProblemInfo*)info)->r,((ProblemInfo*)info)->k,((ProblemInfo*)info)->lmd);
}/*END*/


void ResetRun(ES mi_es)
{
     ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
     mi_es->TotalEval=0;
     mi_es->generados=0;
     p->TotalEval=0;
     p->van=0;
     p->tEval=0;    
     p->nEvalMin=0; 
     restart = 0;
}




/*
 * Cruce dependiente del problema.
 *
 */
#ifdef __LINUX__
#pragma argused
#endif
void CruceProblema (Individuo* hijo, Poblacion padres, ES mi_es){
  int j, fi, k=0,i,nf=0,k1,sw,xfi,npadres,vec[110],van;
  long masEval=0;
  ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
  Individuo *padreA,*mhijo,*mejor,*son;
  int mat[500][110];
  npadres=Size(padres);
  for(i=0;i<npadres;i++){
     k=0;                            
    padreA = ObtenerIndividuo(padres, i);
    for(fi=0;fi<p->v;fi++)
    {
       sw=0;
       if(k)
       {    
            for(xfi=0;xfi<nf&&!sw;xfi++)
             {
                k1=k;
                sw=1;
                for(j=0;j<p->b&&sw;j++)
                  {
                    if(mat[xfi][j]!=padreA->genoma[k1])
                      sw=0;
                    k1++;
                  }
             }  
        }
       if(!sw)
        {
           for(j=0;j<p->b;j++) {
            mat[nf][j]=padreA->genoma[k];
            k++;}
            nf++;
        }
    }
    }
    int cual=1; 
    if(nf>p->v)
    {
      if(cual)
       {
         mejor=CrearIndividuo(p->v*p->b);
         mhijo=CrearIndividuo(p->v*p->b);
         son=CrearIndividuo(p->v*p->b);
         van=p->v;
         k=0;
         for(fi=0;fi<p->v;fi++)
           for(j=0;j<p->b;j++){
             son->genoma[k]=mat[fi][j];
             k++;}
         son->fitness=EvaluarRestricion(son,p);
         for(i=p->v;i<nf;i++)
          {  
           sw=0;
           CopiarIndividuo(mejor,son);
           for(xfi=0;xfi<p->v;xfi++)
             {
                CopiarIndividuo(mhijo,son);
                k=xfi*p->b;
                for(j=0;j<p->b;j++){
                  mhijo->genoma[k]=mat[i][j];
                  k++;}
                mhijo->fitness=EvaluarRestricion(mhijo,p);
                van++;
                if(mhijo->fitness<mejor->fitness)
                  {
                    CopiarIndividuo(mejor,mhijo);
                    sw=1;
                  }
             }
           if(sw)
              CopiarIndividuo(son,mejor);
         }
         k=0;
         for(fi=0;fi<p->v;fi++)
         for(j=0;j<p->b;j++) {
             hijo->genoma[k]=son->genoma[k];
             k++;
         }
        p->tEval+=(2*nf)/p->v-1;
        DestruirIndividuo(son);
        DestruirIndividuo(mejor);
        DestruirIndividuo(mhijo);
       }else
        {
            xfi=0;
            while(xfi<p->v)
             {
                for(i=0;i<nf;i++)
                 if(mat[i][0]!=-5)
                   if(URand01())
                    {
                       k=xfi*p->b;
                       for(j=0;j<p->b;j++){
                        hijo->genoma[k]=mat[i][j];
                        k++;}
                       mat[i][0]=-5;
                       xfi++;
                       break;                      
                    }
                }  
          
        }
     }else 
     {
       k=0;
       for(fi=0;fi<nf;fi++)
        for(j=0;j<p->b;j++){
             hijo->genoma[k]=mat[fi][j];
             k++;
        }
      for(fi=nf;fi<p->v;fi++)
       {
         for(j=0;j<p->b;j++)
          vec[j]=0;
         j=0;
         while(j<p->r)
          {
           i=URand(p->b);
           if(vec[i]==0)
            {
             vec[i]=1;
             j++;
           }
          }
         for(j=0;j<p->b;j++)
          {
           hijo->genoma[k]=vec[j];
           k++;
          }
      }   
     }
    
}/*END*/
/*
 * Se le pasa un individuo que será mutado en una posicion aleatoria.
 * El valor que tomara sera otro aleatorio entre cero y mi_es->varLSup
 */
void MutacionProblema(Individuo* ind, ES mi_es){
  ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
  long xEval=p->nEval;
  int pos=URand(p->v);
  int *per=RandPerm(p->b);
  int i=1;
  while(ind->genoma[pos*p->b+per[0]]==ind->genoma[pos*p->b+per[i]])
    i++;
  ind->genoma[pos*p->b+per[0]]=1-ind->genoma[pos*p->b+per[0]];
  ind->genoma[pos*p->b+per[i]]=1-ind->genoma[pos*p->b+per[i]];
  free(per);
}

/*
 * Evalua un individuo.
 *
 */
ValorFitness Evaluar (Individuo* i, ES mi_es){
    //static int cc=0;
    int j,k,r,s;
    long xEval;
    int fila[100],col[100];
    ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
    xEval=p->nEval;
    long pegado=(long)p->v;
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
       
    if (p->prob&&(URand01()<p->prob))   
      { 
         switch(p->algo)
          {
            case 1: // HC Bit-Flip
                    HcBitFlip(i,p,xEval);
                    break;
            case 2: // HC Bit-Flip
                    HcSwap(i,p,xEval);
                    break;
            case 3: // Tabu Search Bit-Flip
                    TsBF(i,p,xEval,10000);
                    break;
            case 4: // Tabu Search Swap
                    TsSwap(i,p,xEval,pegado);
          }
         p->tEval+=xEval;
        
      }
    return i->fitness;
}/*END*/




/*
 * Devuelve TRUE cuando se alcanza el numero de claculos equivalente
 * al de 'num evaluaciones completas
 */
#ifdef __LINUX__
#pragma argused
#endif
int LimiteAlcanzado (ES mi_es) // Sumar al genetico en el parametro es_mi->generados
{
    ProblemInfo* p=(ProblemInfo *)mi_es->infoProblem;
    if(mi_es->generados+p->tEval>mi_es->numind)
       return 1;
    else
      if(convert_time(p->tstart,clock())>=p->tti)
        return 1;
      else
        return 0;
}


void *CopiarFenotipo(void *dest, void *orig)
{
/*BEGIN*/

}/*END*/

void DestruirFenotipo(void* phen){
/*BEGIN*/
}/*END*/


#ifdef __GRAFSOL__

/*
 * Actualiza los graficos del problema que hubiere.
 *
 */
void ActualizarGraficosProblema (ES mi_es)
{
}

#endif


